#!/bin/sh




TYPE=$1
TARGET=$2

case "$TARGET" in
	debug)
		;;
	release)
		;;
	*)
		if (test -f ./debug); then
			TARGET=debug
		else
			TARGET=release
		fi
		;;
esac

echo "Picked target: $TARGET";
export TARGET;




case "$TYPE" in
	before_install)
		./travis/before-install.sh;
		;;
	install)
		./travis/install.sh;
		;;
#	before_script)
#		;;
	script)
		./travis/script.sh;
		;;
#	before_cache)
#		;;
#	after_success)
#		;;
#	after_failure)
#		;;
#	before_deploy)
#		;;
#	deploy)
#		;;
#	after_deploy)
#		;;
#	after_script)
#		;;
	*)
		echo "Unknown type: $TYPE";
		echo "Assuming intended skip.";
		exit 0;
		;;
esac
