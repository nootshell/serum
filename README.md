Serum
===

Cryptography library, work in progress.

Features
===
- Crypto
	- Padding
		- [ISO 9797-1 methods 1 and 2](https://en.wikipedia.org/wiki/ISO/IEC_9797-1#Padding)
	- Block ciphers
		- [XXTEA](https://en.wikipedia.org/wiki/XXTEA)
- Misc
	- Debugging
		- [Detailed function return values](./libserum/core/result.h)
		- [Memory dump functions](./libserum/debug/memdump.h)
	- Macros
		- [I/O pointer selection (w/ and w/out copy)](./libserum/core/ptrarithmetic.h#L40)
	- Compile-time [compiler](./libserum/core/detect_compiler.h) and [platform](./libserum/core/detect_platform.h) detection
