#!/bin/sh



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
