if (test -z "$TARGET"); then
	echo "Target not specified.";
	exit 1;
fi

make $TARGET bin/test;
