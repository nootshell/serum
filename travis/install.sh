if (test -z "$TARGET"); then
	echo "Target not specified.";
	exit 1;
fi

make $TARGET;
sudo make install;
make bin/test;
