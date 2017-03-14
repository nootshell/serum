Serum
===

**Experimental*** C library focused on cryptography and more, licensed under the MIT license.

This project is basically for me to learn stuff, it aims to be the complete toolset one requires to create binary/crypto protocols.

<sup>\****Do not use in a production environment.***</sup>

Features
===
- Crypto
	- Block ciphers
		- [XXTEA](https://en.wikipedia.org/wiki/XXTEA)
	- Padding
		- [ISO 9797-1 methods 1 and 2](https://en.wikipedia.org/wiki/ISO/IEC_9797-1#Padding)
	- Pseudo-random number generation
		- [Reading from a **/dev**ice (or file)](./libserum/crypto/prng/device.h)
		- [ISAAC](https://en.wikipedia.org/wiki/ISAAC_(cipher))
- Misc
	- Time retrieval
		- RDTSC
		- Nano, micro, milli, and full seconds
	- Debugging
		- [Detailed function return values](./libserum/core/result.h)
		- [Memory dump functions](./libserum/debug/memdump.h)
	- Macros
		- [I/O pointer selection (w/ and w/out copy)](./libserum/core/ptrarithmetic.h#L40)
	- Compile-time detection of:
		- [Operating system](./libserum/core/detect_os.h)
		- [Compiler](./libserum/core/detect_compiler.h)
		- [Platform](./libserum/core/detect_platform.h)
		- [Endianness](./libserum/core/detect_endianness.h)
	- Networking
		- [Universal socket](./libserum/networking/socket.h)
		- [Packets](./libserum/networking/packet.h)
	- Misc
		- [Variable size integer encoding/decoding](./libserum/core/varsize.h)

TODO
===
- Crypto
	- Key exchange
		- Curve25519 <sup>priority</sup>
		- Diffie Hellman
	- Block ciphers
		- Rijndael <sup>priority</sup>
	- Stream ciphers
		- Rabbit
	- Hash functions (+ HMACs)
		- SHA-2 <sup>being implemented</sup>
		- SHA-3
		- RIPEMD-160
	- KDF
		- PBKDF2 <sup>priority</sup>
		- Argon2
		- Lyra2
- Optimizations
	- ```restrict``` pointers where applicable <sup>low priority</sup>
