#!/bin/sh

read -p "Are you sure you want to continue? (y/n): " -r -n 1
echo
if [[ $REPLY =~ ^[Yy]$ ]] ; then
	read -p "Are you very sure? (y/n): " -r -n 1
	echo
	if [[ $REPLY =~ ^[Yy]$ ]] ; then
		printf "Working..."
		find . -type f \( -name '*.c' -o -name '*.h' \) -exec sh -c "unexpand --tabs=4 --first-only '{}' > '{}.out'; mv '{}.out' '{}';" \;
		echo " done."
	else
		echo "Aborted."
	fi
else
	echo "Aborted."
fi

