/*
  srcmlns.h

  Copyright (C) 2005  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Definitions of srcML namespace URI's
*/

#ifndef INCLUDED_SRCMLNS_H
#define INCLUDED_SRCMLNS_H

const char* const XML_DECLARATION_STANDALONE = "yes";
const char* const XML_VERSION = "1.0";

// srcML namespace URIs
const char* const SRCML_SRC_NS_URI = "http://www.sdml.info/srcML/src";
const char* const SRCML_CPP_NS_URI = "http://www.sdml.info/srcML/cpp";
const char* const SRCML_ERR_NS_URI = "http://www.sdml.info/srcML/srcerr";

const char* const SRCML_SRC_NS_PREFIX_DEFAULT = "";
const char* const SRCML_CPP_NS_PREFIX_DEFAULT = "cpp";
const char* const SRCML_ERR_NS_PREFIX_DEFAULT = "srcerr";

// srcML extension namespace URI's
const char* const SRCML_EXT_LITERAL_NS_URI = "http://www.sdml.info/srcML/literal";
const char* const SRCML_EXT_OPERATOR_NS_URI = "http://www.sdml.info/srcML/operator";
const char* const SRCML_EXT_MODIFIER_NS_URI = "http://www.sdml.info/srcML/modifier";

const char* const SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT = "lit";
const char* const SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT = "op";
const char* const SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT = "type";

// srcML standard attributes
const char* const UNIT_ATTRIBUTE_LANGUAGE = "language";
const char* const UNIT_ATTRIBUTE_DIRECTORY = "dir";
const char* const UNIT_ATTRIBUTE_FILENAME = "filename";
const char* const UNIT_ATTRIBUTE_VERSION = "version";

#endif
