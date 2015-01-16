#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test other_prefixes

define srcml <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo">
	
	<unit language="Java">
	<foo:a/>
	</unit>

	<unit xmlns:bar="http://www.cs.uakron.edu/~collard/bar" revision="0.8.0" language="Java">
	<bar:b/>
	</unit>

	</unit>
	STDOUT

define xpathempty <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" xmlns:foo="http://www.cs.uakron.edu/~collard/foo"/>
	STDOUT

define output <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.sdml.info/srcML/src" revision="0.8.0">

	<unit revision="0.8.0" language="Java">
	<foo:a/>
	</unit>

	<unit revision="0.8.0" language="Java">
	<bar:b/>
	</unit>

	</unit>
	STDOUT

createfile sub/a.cpp.xml "$srcml"

# /src:unit
srcml2src --xpath=/src:unit sub/a.cpp.xml
check 3<<< "$output"

srcml2src --xpath=/src:unit < sub/a.cpp.xml
check 3<<< "$output"

srcml2src --xpath=/src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

srcml2src --xpath=/src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

srcml2src --xpath=/src:unit -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

# //src:unit
srcml2src --xpath=//src:unit sub/a.cpp.xml
check 3<<< "$output"

srcml2src --xpath=//src:unit < sub/a.cpp.xml
check 3<<< "$output"

srcml2src --xpath=//src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

srcml2src --xpath=//src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

srcml2src --xpath=//src:unit -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$output"

# src:unit
srcml2src --xpath=src:unit sub/a.cpp.xml
check 3<<< "$xpathempty"

srcml2src --xpath=src:unit < sub/a.cpp.xml
check 3<<< "$xpathempty"

srcml2src --xpath=src:unit sub/a.cpp.xml -o sub/b.cpp.xml
check sub/b.cpp.xml 3<<< "$xpathempty"

srcml2src --xpath=src:unit -o sub/b.cpp.xml sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$xpathempty"

srcml2src --xpath=src:unit -o sub/b.cpp.xml < sub/a.cpp.xml
check sub/b.cpp.xml 3<<< "$xpathempty"

