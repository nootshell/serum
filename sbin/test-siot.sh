#!/bin/bash

EXECUTABLE="$1";
if [ ! -x "$EXECUTABLE" ]; then
	if [ ! -x 'bin/test' ]; then
		echo "Couldn't find test executable.";
		exit 10;
	else
		EXECUTABLE='bin/test';
	fi;
fi;

# Enable job control.
set -m;

# Start up the server.
"$EXECUTABLE" siot-server &

# Give it a second to start up.
sleep 1s;

# Send the input file's contents to the server, and compare it to what we expect.
cat 'test/siot/input.txt' | "$EXECUTABLE" siot-client | diff 'test/siot/output.txt' -

# Check if we got what we expected (0, equal).
RESULT=$?
if [ $RESULT -eq 0 ]; then
	echo "SIOT successful.";
else
	echo "SIOT failed (${RESULT}).";
fi;

# Terminate the server.
kill %1;

exit $RESULT;
