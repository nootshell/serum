# Serum
**Experimental** standalone toolkit, licensed under the [MIT license](./COPYING.md). Supposed to be cross-platform.

[![Travis CI](https://travis-ci.org/icecubetray/serum.svg?branch=rewrite)](https://travis-ci.org/icecubetray/serum)
[![GitLab CI](https://gitlab.com/icecubetray/serum/badges/rewrite/pipeline.svg)](https://gitlab.com/icecubetray/serum/commits/rewrite)

<sup>*All shell examples assume you're in the root directory of the repo, unless otherwise specified.*</sup>

## Compilation ##
Serum has a custom Makefile compilation system based on [this Makefile template](/icecubetray/make-c-template).
The base of the template was added as a submodule, requiring you to initialize it upon cloning: `git submodule update --init --remote --recursive`

## Identification ##
Serum's compilation units contain an `$Id: entifier $`.<br/>
Meaning that if, for example, you have `GNU RCS` installed, you can check the status of each compiled file after compilation by invoking `ident`:
```sh
$ ident bin/libserum.so

# bin/libserum.so:
#	$Id: <file> [(branch commit date-authored)] <description> $
```
