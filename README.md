# Serum \[[![Build Status](https://travis-ci.org/icecubetray/serum.svg?branch=rewrite)](https://travis-ci.org/icecubetray/serum)\]
**Experimental** standalone toolkit, licensed under the [MIT license](./COPYING.md). Supposed to be cross-platform.

<sup>*All shell examples assume you're in the root directory of the repo, unless otherwise specified.*</sup>

## Compilation ##
Serum has a `Makefile` based compilation process.
```sh
# This'll let make default to /re/building the debug target, otherwise it'll
# default to building the release target.
$ touch debug

# To automatically run Doxygen if installed after building libserum, make sure
# the file 'autodoxy' exists.
$ touch autodoxy

# Do the actual compilation.
$ make

# If you're on a system with ELF, and for some reason you made ./elf_interp.sh
# give out valid output, you can run self-tests by invoking the library itself:
$ bin/libserum.so
```

## Identification ##
Serum's compilation units contain an `$Id: entifier $`.<br/>
Meaning that if, for example, you have `GNU RCS` installed, you can check the status of each compiled file after compilation by invoking `ident`:
```sh
$ ident bin/libserum.so

# bin/libserum.so:
#	$Id: <file> [(branch commit date-authored)] <description> $
```
