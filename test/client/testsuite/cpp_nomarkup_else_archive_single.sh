#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test --cpp-nomarkup-else
define input <<- 'INPUT'

	#if A
	break;
	#else
	return;
	#endif
	INPUT

define markup_else <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="ece2a60901d6ccdfe670f0f7445560a55dac57a6">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	</unit>
	STDOUT

define fmarkup_else <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="ece2a60901d6ccdfe670f0f7445560a55dac57a6">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	<return>return;</return>
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	</unit>
	STDOUT

define nomarkup_else <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" options="CPP_TEXT_ELSE">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" hash="ece2a60901d6ccdfe670f0f7445560a55dac57a6">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	return;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	</unit>
	STDOUT

define fnomarkup_else <<- 'STDOUT'
	<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
	<unit xmlns="http://www.srcML.org/srcML/src" revision="REVISION" options="CPP_TEXT_ELSE">

	<unit xmlns:cpp="http://www.srcML.org/srcML/cpp" revision="REVISION" language="C++" filename="sub/a.cpp" hash="ece2a60901d6ccdfe670f0f7445560a55dac57a6">
	<cpp:if>#<cpp:directive>if</cpp:directive> <expr><name>A</name></expr></cpp:if>
	<break>break;</break>
	<cpp:else>#<cpp:directive>else</cpp:directive></cpp:else>
	return;
	<cpp:endif>#<cpp:directive>endif</cpp:directive></cpp:endif>
	</unit>

	</unit>
	STDOUT

xmlcheck "$markup_else"
xmlcheck "$fmarkup_else"
xmlcheck "$nomarkup_else"
xmlcheck "$fnomarkup_else"

createfile sub/a.cpp "$input"

# markup else by default
srcml -l C++ --archive < sub/a.cpp
check "$markup_else"

srcml --archive -l C++ < sub/a.cpp
check "$markup_else"

srcml -l C++ -o sub/b.cpp.xml --archive < sub/a.cpp
check sub/b.cpp.xml "$markup_else"

srcml -l C++ --archive -o sub/b.cpp.xml < sub/a.cpp
check sub/b.cpp.xml "$markup_else"

srcml --archive -l C++ -o sub/b.cpp.xml < sub/a.cpp
check sub/b.cpp.xml "$markup_else"

srcml -o sub/b.cpp.xml -l C++ --archive < sub/a.cpp
check sub/b.cpp.xml "$markup_else"

srcml -o sub/b.cpp.xml --archive -l C++ < sub/a.cpp
check sub/b.cpp.xml "$markup_else"

srcml --archive -o sub/b.cpp.xml -l C++ < sub/a.cpp
check sub/b.cpp.xml "$markup_else"

srcml sub/a.cpp --archive
check "$fmarkup_else"

srcml --archive sub/a.cpp
check "$fmarkup_else"

srcml sub/a.cpp -o sub/b.cpp.xml --archive
check sub/b.cpp.xml "$fmarkup_else"

srcml sub/a.cpp --archive -o sub/b.cpp.xml
check sub/b.cpp.xml "$fmarkup_else"

srcml --archive sub/a.cpp -o sub/b.cpp.xml
check sub/b.cpp.xml "$fmarkup_else"

srcml -o sub/b.cpp.xml sub/a.cpp --archive
check sub/b.cpp.xml "$fmarkup_else"

srcml -o sub/b.cpp.xml --archive sub/a.cpp
check sub/b.cpp.xml "$fmarkup_else"

srcml --archive -o sub/b.cpp.xml sub/a.cpp
check sub/b.cpp.xml "$fmarkup_else"

# don't markup else
srcml -l C++ --cpp-nomarkup-else --archive < sub/a.cpp
check "$nomarkup_else"

srcml -l C++ --archive --cpp-nomarkup-else < sub/a.cpp
check "$nomarkup_else"

srcml --archive -l C++ --cpp-nomarkup-else < sub/a.cpp
check "$nomarkup_else"

srcml -l C++ --cpp-nomarkup-else -o sub/b.cpp.xml --archive < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -l C++ --cpp-nomarkup-else --archive -o sub/b.cpp.xml < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -l C++ --archive --cpp-nomarkup-else -o sub/b.cpp.xml < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --archive -l C++ --cpp-nomarkup-else -o sub/b.cpp.xml < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -l C++ -o sub/b.cpp.xml --cpp-nomarkup-else --archive < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -l C++ -o sub/b.cpp.xml --archive --cpp-nomarkup-else < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -l C++ --archive -o sub/b.cpp.xml --cpp-nomarkup-else < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --archive -l C++ -o sub/b.cpp.xml --cpp-nomarkup-else < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -o sub/b.cpp.xml -l C++ --cpp-nomarkup-else --archive < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -o sub/b.cpp.xml -l C++ --archive --cpp-nomarkup-else < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -o sub/b.cpp.xml --archive -l C++ --cpp-nomarkup-else < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --archive -o sub/b.cpp.xml -l C++ --cpp-nomarkup-else < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -o sub/b.cpp.xml --cpp-nomarkup-else -l C++ --archive < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -o sub/b.cpp.xml --cpp-nomarkup-else --archive -l C++ < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml -o sub/b.cpp.xml --archive --cpp-nomarkup-else -l C++ < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --archive -o sub/b.cpp.xml --cpp-nomarkup-else -l C++ < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --cpp-nomarkup-else -o sub/b.cpp.xml -l C++ --archive < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --cpp-nomarkup-else -o sub/b.cpp.xml --archive -l C++ < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --cpp-nomarkup-else --archive -o sub/b.cpp.xml -l C++ < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --archive --cpp-nomarkup-else -o sub/b.cpp.xml -l C++ < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --cpp-nomarkup-else -l C++ -o sub/b.cpp.xml --archive < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --cpp-nomarkup-else -l C++ --archive -o sub/b.cpp.xml < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --cpp-nomarkup-else --archive -l C++ -o sub/b.cpp.xml < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --archive --cpp-nomarkup-else -l C++ -o sub/b.cpp.xml < sub/a.cpp
check sub/b.cpp.xml "$nomarkup_else"

srcml --cpp-nomarkup-else -l C++ --archive < sub/a.cpp
check "$nomarkup_else"

srcml --cpp-nomarkup-else --archive -l C++ < sub/a.cpp
check "$nomarkup_else"

srcml --archive --cpp-nomarkup-else -l C++ < sub/a.cpp
check "$nomarkup_else"

srcml sub/a.cpp --cpp-nomarkup-else --archive
check "$fnomarkup_else"

srcml sub/a.cpp --archive --cpp-nomarkup-else
check "$fnomarkup_else"

srcml --archive sub/a.cpp --cpp-nomarkup-else
check "$fnomarkup_else"

srcml sub/a.cpp --cpp-nomarkup-else -o sub/b.cpp.xml --archive
check sub/b.cpp.xml "$fnomarkup_else"

srcml sub/a.cpp --cpp-nomarkup-else --archive -o sub/b.cpp.xml
check sub/b.cpp.xml "$fnomarkup_else"

srcml sub/a.cpp --archive --cpp-nomarkup-else -o sub/b.cpp.xml
check sub/b.cpp.xml "$fnomarkup_else"

srcml --archive sub/a.cpp --cpp-nomarkup-else -o sub/b.cpp.xml
check sub/b.cpp.xml "$fnomarkup_else"

srcml sub/a.cpp -o sub/b.cpp.xml --cpp-nomarkup-else --archive
check sub/b.cpp.xml "$fnomarkup_else"

srcml sub/a.cpp -o sub/b.cpp.xml --archive --cpp-nomarkup-else
check sub/b.cpp.xml "$fnomarkup_else"

srcml sub/a.cpp --archive -o sub/b.cpp.xml --cpp-nomarkup-else
check sub/b.cpp.xml "$fnomarkup_else"

srcml --archive sub/a.cpp -o sub/b.cpp.xml --cpp-nomarkup-else
check sub/b.cpp.xml "$fnomarkup_else"

srcml --cpp-nomarkup-else sub/a.cpp --archive
check "$fnomarkup_else"

srcml --cpp-nomarkup-else --archive sub/a.cpp
check "$fnomarkup_else"

srcml --archive --cpp-nomarkup-else sub/a.cpp
check "$fnomarkup_else"

srcml --cpp-nomarkup-else sub/a.cpp -o sub/b.cpp.xml --archive
check sub/b.cpp.xml "$fnomarkup_else"

srcml --cpp-nomarkup-else sub/a.cpp --archive -o sub/b.cpp.xml
check sub/b.cpp.xml "$fnomarkup_else"

srcml --cpp-nomarkup-else --archive sub/a.cpp -o sub/b.cpp.xml
check sub/b.cpp.xml "$fnomarkup_else"

srcml --archive --cpp-nomarkup-else sub/a.cpp -o sub/b.cpp.xml
check sub/b.cpp.xml "$fnomarkup_else"

srcml --cpp-nomarkup-else -o sub/b.cpp.xml sub/a.cpp --archive
check sub/b.cpp.xml "$fnomarkup_else"

srcml --cpp-nomarkup-else -o sub/b.cpp.xml --archive sub/a.cpp
check sub/b.cpp.xml "$fnomarkup_else"

srcml --cpp-nomarkup-else --archive -o sub/b.cpp.xml sub/a.cpp
check sub/b.cpp.xml "$fnomarkup_else"

srcml --archive --cpp-nomarkup-else -o sub/b.cpp.xml sub/a.cpp
check sub/b.cpp.xml "$fnomarkup_else"
