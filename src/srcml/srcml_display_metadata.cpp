/**
 * @file srcml_display_info.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <srcml_display_metadata.hpp>
#include <srcml_pretty.hpp>
#include <src_prefix.hpp>
#include <srcml.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <iomanip>
#include <string.h>
#include <boost/format.hpp>

// display all files in srcml archive
/*
void srcml_list_unit_files(srcml_archive* srcml_arch) {

    int numUnits = 0;
    while (srcml_unit* unit = srcml_read_unit_header(srcml_arch)) {
        ++numUnits;
        std::cout << numUnits << '\t' << std::setw(5) << srcml_unit_get_filename(unit) << '\n';
        srcml_unit_free(unit);
    }
}*/

/*void srcml_display_info(srcml_archive* srcml_arch) {

    size_t nsSize = srcml_archive_get_namespace_size(srcml_arch);

    for (size_t i = 0; i < nsSize; ++i) {
        if (srcml_archive_get_namespace_uri(srcml_arch, i)) {
            if (strcmp(srcml_archive_get_namespace_prefix(srcml_arch, i), "") == 0) {
                std::cout << "xmlns=\"" << srcml_archive_get_namespace_uri(srcml_arch, i) << "\"\n";
            }
            else{
                std::cout << "xmlns:" << srcml_archive_get_namespace_prefix(srcml_arch, i) << "=\"" << srcml_archive_get_namespace_uri(srcml_arch, i) << "\"\n";
            }
        }
    }

    if (srcml_archive_get_xml_encoding(srcml_arch))
        std::cout << "encoding=" << "\"" << srcml_archive_get_xml_encoding(srcml_arch) << "\"\n";
    if (srcml_archive_get_language(srcml_arch))
        std::cout << "language=" << "\"" << srcml_archive_get_language(srcml_arch) << "\"\n"; 
    if (srcml_archive_get_url(srcml_arch))
        std::cout << "url=" << "\"" << srcml_archive_get_url(srcml_arch) << "\"\n";
    if (srcml_archive_get_filename(srcml_arch))
        std::cout << "filename=" << "\"" << srcml_archive_get_filename(srcml_arch) << "\"\n";
}

void srcml_display_unit_count(srcml_archive* srcml_arch) {
    int num_units = 0;
    while (srcml_unit* unit = srcml_read_unit_header(srcml_arch)) {
        ++num_units;
        srcml_unit_free(unit);
    }
    std::cout << "units=" << "\"" << num_units << "\"\n";
}*/

int srcml_unit_count(srcml_archive* srcml_arch) {

    int numUnits = 0;
    while (srcml_unit* unit = srcml_read_unit_header(srcml_arch)) {

        if (srcml_unit_get_language(unit))
            ++numUnits;

        srcml_unit_free(unit);
    }
    return numUnits;
}

void srcml_display_metadata(const srcml_request_t& srcml_request, const srcml_input_t& src_input, const srcml_output_dest&) {
    
    int display_commands =  SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE |
                            SRCML_COMMAND_DISPLAY_SRCML_URL |
                            SRCML_COMMAND_DISPLAY_SRCML_FILENAME |
                            SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION |
                            SRCML_COMMAND_DISPLAY_SRCML_ENCODING |
                            SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP | 
                            SRCML_COMMAND_DISPLAY_SRCML_HASH;

    BOOST_FOREACH(const srcml_input_src& input, src_input) {
        // create the output srcml archive
        srcml_archive* srcml_arch = srcml_archive_create();

        if (contains<int>(input)) {
            if (srcml_archive_read_open_fd(srcml_arch, input) != SRCML_STATUS_OK) {
                std::cerr << "Srcml input cannot not be opened.\n";
                return;
            }
        }
        else if (contains<FILE*>(input)){
            if (srcml_archive_read_open_FILE(srcml_arch, input) != SRCML_STATUS_OK) {
                std::cerr << "Srcml input cannot not be opened.\n";
                return;
            }   
        }
        else {
            if (srcml_archive_read_open_filename(srcml_arch, (src_prefix_resource(input).c_str())) != SRCML_STATUS_OK) {
                std::cerr << "Srcml input cannot not be opened.\n";
                return;
            }
        }

        // Overrides all others Perform a pretty output
        if (srcml_request.pretty_format) {
            srcml_pretty(srcml_arch, *srcml_request.pretty_format);
            return;
        }

        /*
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE){
            const char* archive_info = srcml_archive_get_language(srcml_arch);
            if (archive_info) {
                if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE)
                    std::cout << archive_info << "\n";
                else
                    std::cout << "language=\"" << archive_info << "\"\n";
            }
            else {
                if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE)
                    std::cout << "";
                else
                    std::cout << "language=\"\"\n";
            }
        }*/

        std::string pretty_meta = "";

        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE)
                pretty_meta += "%l\n";
            else
                pretty_meta += "language=\"%l\"\n";
        }

        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_FILENAME) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_FILENAME)
                pretty_meta += "%f\n";
            else
                pretty_meta += "filename=\"%f\"\n";
        }

        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_URL) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_URL)
                pretty_meta += "%U\n";
            else
                pretty_meta += "url=\"%U\"\n";
        }

        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION)
                pretty_meta += "%v\n";
            else
                pretty_meta += "version=\"%v\"\n";
        }

        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_ENCODING) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_ENCODING)
                pretty_meta += "%X\n";
            else
                pretty_meta += "encoding=\"%X\"\n";
        }

        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_HASH) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_HASH)
                pretty_meta += "%h\n";
            else
                pretty_meta += "hash=\"%h\"\n";
        }

        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP) {
            if ((display_commands & srcml_request.command) == SRCML_COMMAND_DISPLAY_SRCML_TIMESTAMP)
                pretty_meta += "%t\n";
            else
                pretty_meta += "timestamp=\"%t\"\n";
        }

        if (srcml_request.xmlns_prefix_query) {
            const char* prefix = srcml_archive_get_prefix_from_uri(srcml_arch, srcml_request.xmlns_prefix_query->c_str());
            if (prefix) {
                std::cout << prefix << '\n';
            }
        }

        if (pretty_meta != "") {
            srcml_pretty(srcml_arch, pretty_meta);
        }

        // units
        if (srcml_request.command & SRCML_COMMAND_UNITS) {
            std::cout << srcml_unit_count(srcml_arch) << "\n";
        }

        // srcml long info
        if (srcml_request.command & SRCML_COMMAND_LONGINFO) {
            //srcml_display_unit_count(srcml_arch);
            std::string info = "";
            info += "{ %N\n";
            info += "encoding=\"%X\"\n";
            info += "language=\"%l\"\n";
            info += "url=\"%U\"\n";
            info += "filename=\"%f\"\n } %C\n";
            srcml_pretty(srcml_arch, info);
        }

        // srcml info
        if (srcml_request.command & SRCML_COMMAND_INFO) {
            //srcml_display_info(srcml_arch);
            std::string info = "";
            info += "%N\n";
            info += "encoding=\"%X\"\n";
            info += "language=\"%l\"\n";
            info += "url=\"%U\"\n";
            info += "filename=\"%f\"\n";
            srcml_pretty(srcml_arch, info);
        }

        srcml_archive_close(srcml_arch);
        srcml_archive_free(srcml_arch);
    }
}
