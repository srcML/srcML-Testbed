/*
  xpathproc

  A simple XPath expression evaluator.

  Usage:  xpathproc xpath-expression xml-file
*/

#include "srcpatheval.h"
#include "srceval.h"
#include "srcmlns.h"

#include <cstring>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

xmlChar* unit_directory = 0;
xmlChar* unit_filename = 0;

void outputresult(xmlDocPtr doc, xmlNodePtr onode, xmlOutputBufferPtr buf) {

	   // output a unit element around the fragment, unless
	   // is is already a unit
           bool outputunit = strcmp("unit", (const char*) onode->name) != 0;

	   // if we need a unit, output the start tag
	   if (outputunit) {

	     // unit start tag
	     xmlOutputBufferWrite(buf, 5, "<unit");

	     if (unit_directory) {
	       xmlOutputBufferWrite(buf, 6, " dir=\"");
	       xmlOutputBufferWriteString(buf, (const char*) unit_directory);
	       xmlOutputBufferWrite(buf, 1, "\"");
	     }

	     if (unit_filename) {
	       xmlOutputBufferWrite(buf, 11, " filename=\"");
	       xmlOutputBufferWriteString(buf, (const char*) unit_filename);
	       xmlOutputBufferWrite(buf, 1, "\"");
	     }

	     /*
	     // TODO:  fix line numbering problem
	     xmlOutputBufferWrite(buf, 7, " line=\"");
	     char s[50] = { 0 };
	     sprintf(s, "%d", onode->line);
	     xmlOutputBufferWriteString(buf, s);
	     xmlOutputBufferWrite(buf, 1, "\"");
	     */

	     xmlOutputBufferWrite(buf, 1, ">");
	   }

	   // xpath result
	   xmlNodeDumpOutput(buf, doc, onode, 0, 0, 0);

	   // if we need a unit, output the end tag
	   if (outputunit)
	     xmlOutputBufferWrite(buf, 7, "</unit>");
}

int srcpatheval(const char* xpath, xmlTextReaderPtr reader, const char* ofilename) {

  // compile the xpath that will be applied to each unit
  xmlXPathCompExprPtr compiled_xpath = xmlXPathCompile(BAD_CAST xpath);
  if (compiled_xpath == 0) {
    return 1;
  }

  // read the first node
  int ret = xmlTextReaderRead(reader);
  if (ret != 1)
    return 1;

  // setup the context up on which the xpath will be evaluated on
  xmlXPathContextPtr context = xmlXPathNewContext(xmlTextReaderCurrentDoc(reader));

  // setup output
  xmlOutputBufferPtr buf = xmlOutputBufferCreateFilename(ofilename, NULL, 0);

  // register standard prefixes for standard namespaces
  const char* prefixes[] = {
    SRCML_SRC_NS_URI, "src",
    SRCML_CPP_NS_URI, SRCML_CPP_NS_PREFIX_DEFAULT,
    SRCML_ERR_NS_URI, SRCML_ERR_NS_PREFIX_DEFAULT,
    SRCML_EXT_LITERAL_NS_URI, SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT,
    SRCML_EXT_OPERATOR_NS_URI, SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT,
    SRCML_EXT_MODIFIER_NS_URI, SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT,
  };

  for (int i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]) / 2; i += 2)
    if (xmlXPathRegisterNs(context, BAD_CAST prefixes[i + 1], BAD_CAST prefixes[i]) == -1)
      fprintf(stderr, "Unable to register prefix %s for namespace %s\n", prefixes[i + 1], prefixes[i]);

  // register any additional namespaces on the root element
  for (xmlNsPtr pAttr = xmlTextReaderCurrentNode(reader)->nsDef; pAttr; pAttr = pAttr->next) {

    // get out if the prefix is already defined
    if (xmlXPathNsLookup(context, pAttr->prefix))
      continue;

    if (xmlXPathRegisterNs(context, pAttr->prefix ? pAttr->prefix : BAD_CAST "",
			   BAD_CAST pAttr->href) == -1)
      fprintf(stderr, "Unable to register prefix %s for namespace %s\n",
	      pAttr->prefix, pAttr->href);
  }

  // output wrapping unit
  xmlUnitDumpOutputBuffer(buf, xmlTextReaderCurrentNode(reader));
  xmlOutputBufferWrite(buf, 3, ">\n\n");

  // type of the xpath
  int nodetype = 0;

  // total of all counts for the separate units
  double total = 0;

  // resulting boolean
  bool result_bool = false;

  while (!result_bool) {

     // read a node
     int ret = xmlTextReaderRead(reader);
     if (ret != 1)
       break;

     // contine on until we reach a unit tag at the proper depth
     if (xmlTextReaderDepth(reader) == 1 &&
	 xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT &&
	 xmlTextReaderConstName(reader)[0] == 'u') {

       unit_directory = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_DIRECTORY);
       unit_filename = xmlTextReaderGetAttribute(reader, BAD_CAST UNIT_ATTRIBUTE_FILENAME);

       // expand this unit to make it the context
       context->node = xmlTextReaderExpand(reader);

       // evaluate the xpath on the context from the current document
       xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(compiled_xpath, context);
       if (result_nodes == 0) {
	 fprintf(stderr, "ERROR\n");
	 return 1;
       }

       // update the node type
       nodetype = result_nodes->type;

       // process the resulting nodes
       switch (nodetype) {

       // node set result
       case XPATH_NODESET:

	 // may not have any values
	 if (!result_nodes->nodesetval)
	   break;

	 // output all the found nodes
	 for (int i = 0; i < xmlXPathNodeSetGetLength(result_nodes->nodesetval); ++i) {
	   outputresult(xmlTextReaderCurrentDoc(reader), xmlXPathNodeSetItem(result_nodes->nodesetval, i), buf);
	   xmlOutputBufferWrite(buf, 2, "\n\n");
	 }

	 break;

       // numeric result
       case XPATH_NUMBER:
	 total += result_nodes->floatval;
	 break;

       // boolean result
       case XPATH_BOOLEAN:
         result_bool |= result_nodes->boolval;
	 break;

       default:
	 fprintf(stderr, "Unhandled type\n");
	 break;
       };

       // finished with the result nodes
       xmlXPathFreeObject(result_nodes);

       // move over this expanded node
       xmlTextReaderNext(reader);
     }
  }

  // all done with xpath
  xmlXPathFreeCompExpr(compiled_xpath);

  // finalize results
  switch (nodetype) {
  case XPATH_NODESET:

    xmlOutputBufferWrite(buf, 8, "</unit>\n");
    xmlOutputBufferClose(buf);
    break;

  case XPATH_NUMBER:
    if ((int)total == total)
      printf("%d\n", (int)total);
    else
      printf("%f\n", total);

    xmlFree(buf);
    break;

  // boolean result
  case XPATH_BOOLEAN:
    if (result_bool)
      puts("true\n");
    else
      puts("false\n");

    xmlFree(buf);
    break;

  default:
    break;
  }

  return 0;
}
