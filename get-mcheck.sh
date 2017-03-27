#!/bin/sh

# Quick-fix for getting mcheck to work on my system.
if [ -f "/usr/lib/libmcheck.a" ] ; then
	echo -n "-lmcheck";
fi
