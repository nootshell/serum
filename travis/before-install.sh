if (test -z "$TARGET"); then
	echo "Target not set.";
	exit 1;
fi

# Clean stuff up, just in case.
make clean;

# Ensure we don't autodoxy, we don't need this.
(test -f ./autodoxy) && rm ./autodoxy;

# Ensure make will select the proper target.
case "$TARGET" in
	debug)
		touch debug;
		;;
	release)
		(test -f ./debug) && rm ./debug;
		;;
esac

exit 0;
