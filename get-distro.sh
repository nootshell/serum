#!/bin/sh

if [ -f ./distro ] ; then
	echo -n "\""
	cat ./distro | tr -dc [:print:]
	echo -n "\""
else
	echo -n "\"Generic\""
fi
