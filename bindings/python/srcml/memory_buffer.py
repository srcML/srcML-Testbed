##
# @file memory_buffer.py
#
# @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
#
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import ctypes
import bindings

class memory_buffer:
    def __init__(self):
        self._buf = ctypes.c_char_p()
        self._size = ctypes.c_int(0)

    def __del__(self):
        bindings.free(self._buf)

    def __getitem__(self, key):
        raise Exception("Not implemented!")

    def __len__(self):
        return self._size.value;
    
