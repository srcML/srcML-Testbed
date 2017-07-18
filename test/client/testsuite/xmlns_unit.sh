#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test
define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++"/>
	STDOUT

define foutput <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp"/>
	STDOUT

xmlcheck "$output"
xmlcheck "$foutput"
createfile sub/a.cpp ""

# separate
srcml --xmlns="http://www.srcML.org/srcML/src" sub/a.cpp
check "$foutput"

echo -n "" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src"
check "$output"

echo -n "" | srcml -l C++ --xmlns:cpp="http://www.srcML.org/srcML/cpp"
check "$output"

echo -n "" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

srcml --xmlns="http://www.srcML.org/srcML/src" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$foutput"

srcml --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp
check "$foutput"

echo -n "" | srcml -l C++ --xmlns:cpp="http://www.srcML.org/srcML/cpp" -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

srcml --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$foutput"

# multiple
echo -n "" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp"
check "$output"

echo -n "" | srcml --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp
check "$foutput"

echo -n "" | srcml -l C++ --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" -o sub/a.cpp.xml
check sub/a.cpp.xml "$output"

srcml --xmlns="http://www.srcML.org/srcML/src" --xmlns:cpp="http://www.srcML.org/srcML/cpp" sub/a.cpp -o sub/a.cpp.xml
check sub/a.cpp.xml "$foutput"
