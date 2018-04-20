#!/bin/bash


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




FIND_MISSING_FILEID=0;
FIND_MISSING_LICENSE=0;
FIND_ALL=0;




while [ $# -gt 0 ]; do
	case $1 in
		fileid|--fileid)
			FIND_MISSING_FILEID=1;
			;;
		license|--license)
			FIND_MISSING_LICENSE=1;
			;;
		all|--all)
			FIND_ALL=1;
			;;
	esac;

	shift;
done;




if [[ $FIND_MISSING_FILEID -eq 1 || $FIND_ALL -eq 1 ]]; then
	echo -n "Looking for files missing FILEID...";
	FILES=$(grep --color=never -L FILEID `find libserum -type f -name '*.c'`);
	echo " done.";
	if [ -z "$FILES" ]; then
		echo " > None found!";
	else
		for file in $FILES; do
			echo " > $file";
		done;
	fi;
	echo;
fi;


if [[ $FIND_MISSING_LICENSE -eq 1 || $FIND_ALL -eq 1 ]]; then
	echo -n "Looking for files missing license...";
	FIND='Copyright 20[0-9][0-9]-20[0-9][0-9] icecubetray';
	FILES=$(grep --color=never -L -e "$FIND" `find libserum -type f \( -name '*.c' -or -name '*.h' \)`);
	FILES="$FILES
$(grep --color=never -L -e "$FIND" `find . -type f -name '*.sh'`)";
	echo " done.";
	if [ -z "$FILES" ]; then
		echo " > None found!";
	else
		for file in $FILES; do
			echo " > $file";
		done;
	fi;
	echo;
fi;
