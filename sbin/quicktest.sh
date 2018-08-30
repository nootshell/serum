#!/bin/sh




__print_result() {
	if [ $# -eq 2 ]; then
		echo -n "$2";
	else
		echo -n " done: ";
	fi;

	if [ $1 -ne 0 ]; then
		echo "failure";
		exit 1;
	else
		echo "success";
	fi;
}




if [ ! -r ./COPYING.md -o ! -r ./serum.creator -o ! -r README.md -o ! -d ./.git ]; then
	echo "This script must be run from the root of the repository.";
	exit 1;
fi;




echo -n "Updating project binaries...";
MAKE_OUTPUT=$(make 2>&1);
__print_result $?;


echo -n "Running CST...";
CST_OUTPUT=$(bin/test run-cst 2>&1);
__print_result $?;


echo -n "Running unit tests...";
UNIT_OUTPUT=$(bin/test unit-test 2>&1);
__print_result $?;


echo -n "Running SIOT test...";
SIOT_OUTPUT=$(sbin/test-siot.sh 2>&1);
__print_result $?;




echo "Tests passed, ready to commit and to let CI have a look";
