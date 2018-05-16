#!/bin/sh




CHECKOPT_PATH="$(cd -- "$(dirname -- "$0")" && pwd)/checkopt.sh"




if [ $# -lt 2 ]; then
	echo "$@";
	echo "Usage: $0 <option> [<option>, ...]";
	exit 2;
fi;




CC="$1"; shift;

if ($CHECKOPT_PATH $CC $@); then
	echo $@;
else
	exit 1;
fi;




exit 0;
