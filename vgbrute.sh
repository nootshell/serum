#!/bin/sh


###############################################################################
##                                                                           ##
##   The MIT License                                                         ##
##                                                                           ##
##   Copyright 2017-2018 icecubetray                                         ##
##                                                                           ##
##   Permission is hereby granted, free of charge, to any person             ##
##   obtaining a copy of this software and associated documentation files    ##
##   (the "Software"), to deal in the Software without restriction,          ##
##   including without limitation the rights to use, copy, modify, merge,    ##
##   publish, distribute, sublicense, and/or sell copies of the Software,    ##
##   and to permit persons to whom the Software is furnished to do so,       ##
##   subject to the following conditions:                                    ##
##                                                                           ##
##   The above copyright notice and this permission notice shall be          ##
##   included in all copies or substantial portions of the Software.         ##
##                                                                           ##
##   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         ##
##   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      ##
##   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  ##
##   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    ##
##   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    ##
##   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       ##
##   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  ##
##                                                                           ##
###############################################################################




CHECK_TYPE=1;

for arg in "$@"; do
	case "$arg" in
		"--check-exitcode")
			CHECK_TYPE=1;
			;;
		"--check-stdout-empty")
			CHECK_TYPE=2;
			;;
		"--check-stdout-nonempty")
			CHECK_TYPE=3;
			;;
		*)
			echo "Unknown argument: $arg";
			exit 3;
			;;
	esac
done

echo "Check type: $CHECK_TYPE";



echo -n "Preparing binaries...";
MAKEOUT=`make valgrind`
echo " done.";



FOUND=0
echo -n "Running";
if [ -f bin/test ]; then
	while (:);
	do
		VGOUT=`valgrind --error-exitcode=1 bin/test 2>vgbrute.vgerr.log`;
		VGRES=$?

		echo -n ".";

		if [ $CHECK_TYPE == 1 ]; then
			if [ $VGRES != 0 ]; then
				FOUND=1;
			fi
		elif [ $CHECK_TYPE == 2 ]; then
			if [ -z "$VGOUT" ]; then
				FOUND=1;
			fi
		elif [ $CHECK_TYPE == 3 ]; then
			if [ -n "$VGOUT" ]; then
				FOUND=1;
			fi
		fi

		if [ $FOUND == 1 ]; then
			echo " done: valgrind exited with exit code $VGRES.";
			echo -e "$VGOUT";
			echo "$VGOUT" > vgbrute.stdout.log;
			exit 1;
		fi
	done
else
	echo;
	exit 2;
fi

echo;
exit 0;
