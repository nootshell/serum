#!/bin/sh

cd ~;

git clone https://github.com/icecubetray/serum.git --branch=rewrite ./serum;
cd ./serum;

make \
	clean \
	debug \
	install \
	bin/test \
	test;
