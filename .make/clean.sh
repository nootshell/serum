#!/bin/sh


###############################################################################
##                                                                           ##
##   The MIT License                                                         ##
##                                                                           ##
##   Copyright 2018 icecubetray                                              ##
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




if [ $# -eq 0 ]; then
	echo "Nothing to clean: no projects specified";
	exit;
fi;




CLEANED=0;


while [ $# -gt 0 ]; do
	proj="$1";
	shift;

	if [ -d "obj/$proj" ]; then
		echo -n "> Remove obj/$proj";

		RESULT=`rm -Rf "obj/$proj" 2>&1`;

		if [ $? -eq 0 ]; then
			CLEANED=1;
			echo " (done)";
		else
			echo " (failed: $RESULT)";
		fi;
	fi;
done;


if [ -d "bin/" ]; then
	echo -n "> Remove bin/";

	RESULT=`rm -Rf "bin/" 2>&1`;

	if [ $? -eq 0 ]; then
		CLEANED=1;
		echo " (done)";
	else
		echo " (failed: $RESULT)";
	fi;
fi;


if [ $CLEANED -eq 0 ]; then
	echo "> Nothing was removed.";
fi;
