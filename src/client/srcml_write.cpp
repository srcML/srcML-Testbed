/**
 * @file srcml_write.cpp
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

#include <srcml_write.hpp>
#include <srcml.h>
#include <parse_request.hpp>
#include <iostream>
#include <srcml_options.hpp>
#include <trace_log.hpp>
#include <srcml_cli.hpp>
#include <srcml_logger.hpp>

// Public consumption thread function
void srcml_write_request(ParseRequest* request, TraceLog& log) {

    if (!request)
        return;

    bool isarchive = srcml_archive_is_full_archive(request->srcml_arch);
    isarchive = 1; //Print output for all processed files

    // write the unit
    if (request->status == SRCML_STATUS_OK) {

        srcml_archive_write_unit(request->srcml_arch, request->unit);

        if (isarchive) {
            std::string s = request->filename ? *request->filename : "";
            s += "\t";
            s += request->language;
            s += "\t";

            char str[5] = { 0 };
            sprintf(str,"%ld", request->loc);

            s += str;
            //s += "\t";
            //s += "10"; //This will be processing time
            s += "\t";

            const char* hash = srcml_unit_get_hash(request->unit);
            if (hash) {
                s += std::string(hash);
            }
            else {
                s += "";
            }

            log << 'a' << s;
        }

    } else if (request->status == SRCML_STATUS_UNSET_LANGUAGE) {

        if (isarchive)
            log << '-' << (request->filename ? *request->filename : "");
        else
            SRCMLLogger::log(SRCMLLogger::WARNING_MSG, "Extension not supported");

    } else if (request->errormsg) {
        SRCMLLogger::log(SRCMLLogger::WARNING_MSG, *(request->errormsg));

    } else {
        SRCMLLogger::log(SRCMLLogger::WARNING_MSG, "Internal eror " + std::to_string(request->status));
    }

    // free the unit
    if (request->unit) {
        srcml_unit_free(request->unit);

        // close the archive (if per-unit)
        if (SRCML_COMMAND_NOARCHIVE & SRCMLOptions::get()) {
            srcml_archive_close(request->srcml_arch);
            srcml_archive_free(request->srcml_arch);
        }
    }

    delete request;
    request = 0;
}
