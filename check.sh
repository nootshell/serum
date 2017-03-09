#!/bin/sh

STAT=0;

git -c core.whitespace=space-before-tab,indent-with-non-tab,tabwidth=4 diff --check HEAD;
if [ $? != 0 ] ; then
	echo
	echo "Check failed.";
else
	echo "A-OK";
fi
