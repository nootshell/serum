#!/bin/bash

# Enable job control.
set -m;

# Start up the server.
bin/test siot-server &

# Give it a second to start up.
sleep 1s;

# Send the input file's contents to the server, and compare it to what we expect.
cat 'test/siot/input.txt' | bin/test siot-client | diff 'test/siot/output.txt' -

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
