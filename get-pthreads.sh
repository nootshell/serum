#!/bin/sh

# Quick-fix for getting pthreads to work on my system.
if [ -f "/usr/lib/libpthread.so" ] ; then
	echo -n "-lpthread";
fi
