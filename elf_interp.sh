#!/usr/bin/sh


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


#
#	Fetches the first existing ELF interpreter from files in /usr/bin, /usr/lib, /bin, and /lib.
#	For use in libserum's Makefile; any errors given by commands used are suppressed.
#
#	If this script was run before, the results are cached in ./.elf_interp_cache and the next time
#	this script runs it'll fetch the results from there, unless 'skipcache' was specified as first
#	parameter.
#




CACHE=1
LOG=1
LOGFILE="./elf_interp.log"

if [ $# -gt 0 ]; then
	if [ $1 == "skipcache" ]; then
		CACHE=0
	fi
fi

if [ $CACHE == 1 ]; then
	if [ -f ./.elf_interp_cache ]; then
		cat ./.elf_interp_cache;
		exit 0;
	fi
fi


echo > "$LOGFILE";

for DIRECTORY in /usr/bin /usr/lib /bin /lib
do
	if [ $LOG == 1 ]; then echo "Entered '$DIRECTORY'" >> "$LOGFILE" 2>&1; fi
	for FILE in $(find $DIRECTORY -maxdepth 1 -type f ! -name '*.*' 2>> "$LOGFILE")
	do
		if [ $LOG == 1 ]; then echo -n "Found '$FILE':" >> "$LOGFILE" 2>&1; fi
		ELF=$(readelf -l $FILE 2> /dev/null)

		if [ $? == 0 ]; then
			if [ $LOG == 1 ]; then echo -n " valid ELF:" >> "$LOGFILE" 2>&1; fi

			ELF=$(
				grep -xoEe ".*[[]Requesting program interpreter: ([/][^ ].+)[]].*" <<< "$ELF" 2>> "$LOGFILE" |
				awk -F:\  '{print $2}'                                                        2>> "$LOGFILE" |
				awk -F] '{print $1}'                                                          2>> "$LOGFILE" |
				tr -dc [:print:]                                                              2>> "$LOGFILE"
			);

			if [ $? == 0 ]; then
				if [ -f "$ELF" ]; then
					if [ $LOG == 1 ]; then echo " valid interpreter" >> "$LOGFILE" 2>&1; fi

					# Interpreter found, and it exists: compile the string to be passed to the compiler.
					ELF="-DELF_INTERPRETER=\"\\\"$ELF\\\"\" -Wl,-e,__libserum_main";

					# Cache the results for the next run and output the compiled string.
					echo -n "$ELF" > ./.elf_interp_cache 2>> "$LOGFILE"
					echo -n "$ELF";

					exit 0;
				else
					if [ $LOG == 1 ]; then echo " interpreter does not exist" >> "$LOGFILE" 2>&1; fi
				fi
			fi
		else
			if [ $LOG == 1 ]; then echo " invalid ELF" >> "$LOGFILE" 2>&1; fi
		fi
	done
done


# We didn't fetch any results.
exit 1;
