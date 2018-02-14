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



__do_log() {
	if (test "$LOG" -eq 1); then
		local N=''
		if (test -n "$1" && test "$1" = "-n"); then
			N="-n"
		fi

		echo $N "$@" >> "$LOGFILE";
	fi
}




CACHE=1
LOG=1
LOGFILE="./elf_interp.log"

if (test "$#" -gt 0); then
	if (test "$1" = "skipcache"); then
		CACHE=0
	fi
fi

if (test "$CACHE" -eq 1); then
	if (test -f ./.elf_interp_cache); then
		cat ./.elf_interp_cache;
		exit 0;
	fi
fi


echo > "$LOGFILE";

for DIRECTORY in /usr/bin /usr/lib /bin /lib
do
	__do_log "Entered '$DIRECTORY'";
	for FILE in $(find $DIRECTORY -maxdepth 1 -type f ! -name '*.*' 2>> "$LOGFILE")
	do
		__do_log -n "Found '$FILE':";
		ELF=$(readelf -l $FILE 2> /dev/null)

		if (test "$?" -eq 0); then
			__do_log -n " valid ELF:";

			ELF=$(
				(echo "$ELF" | grep -xoEe ".*[[]Requesting program interpreter: ([/][^ ].+)[]].*") 2>>"$LOGFILE" |
				awk -F:\  '{print $2}'                                                             2>>"$LOGFILE" |
				awk -F] '{print $1}'                                                               2>>"$LOGFILE" |
				tr -dc [:print:]                                                                   2>>"$LOGFILE"
			);

			if (test "$?" -eq 0); then
				if (test -f "$ELF"); then
					__do_log " valid interpreter";

					# Interpreter found, and it exists: compile the string to be passed to the compiler.
					ELF="-DELF_INTERPRETER=\"\\\"$ELF\\\"\" -Wl,-e,__libserum_main";

					# Cache the results for the next run and output the compiled string.
					echo -n "$ELF" > ./.elf_interp_cache 2>> "$LOGFILE"
					echo -n "$ELF";

					exit 0;
				else
					__do_log " interpreter does not exist";
				fi
			fi
		else
			__do_log " invalid ELF";
		fi
	done
done


# We didn't fetch any results.
exit 1;
