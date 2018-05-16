#!/bin/sh




if [ $# -lt 2 ]; then
	echo "Usage: $0 <cc> <option> [<option>, ...]";
	exit 2;
fi;




CC=$1; shift;

RESULT=`echo "int main() { return 0; }" | $CC -E $@ - 2>&1`;




if [ $? != 0 ]; then
	exit 1;
fi;

exit 0;
