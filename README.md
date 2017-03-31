# Serum #

**Experimental*** C library focused on cryptography and more, licensed under the MIT license. ***<sup><sup>\*Do not use in a production environment.</sup></sup>***

This project is basically for me to learn stuff, it aims to be the complete toolset one requires to create binary/crypto protocols.


## Features ##

- Crypto
	- Hashing
		- [SHA-2](./libserum/crypto/hashing/sha2.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/SHA-2)</sup>
	- Block ciphers
		- [Rijndael](./libserum/crypto/symmetric/rijndael.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Rijndael)</sup>
		- [XXTEA](./libserum/crypto/symmetric/xxtea.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/XXTEA)</sup>
	- Padding
		- [ISO 9797-1 methods 1 and 2]()  <sup>[Wikipedia](https://en.wikipedia.org/wiki/ISO/IEC_9797-1#Padding)</sup>
	- Pseudo-random number generation
		- [Reading from a **/dev**ice (or file)](./libserum/crypto/prng/device.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki//dev/random)</sup>
		- [ISAAC](./libserum/crypto/prng/isaac.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/ISAAC_(cipher))</sup>
- Misc
	- Time retrieval
		- [RDTSC](./libserum/core/time.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Time_Stamp_Counter)</sup>
		- [Nano, micro, milli, and full seconds](./libserum/core/time.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Orders_of_magnitude_(time))</sup>
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
		- [Packets](./libserum/networking/packet.h) <sup></sup>
	- Misc
		- [Variable size integer encoding/decoding](./libserum/core/varsize.h)

TODO
===
- Crypto
	- Key exchange
		- Curve25519 <sup>priority</sup>
	- Block ciphers
		- *None currently*
	- Stream ciphers
		- Rabbit <sup>[Wikipedia](https://en.wikipedia.org/wiki/Rabbit_(cipher))</sup>
	- Hash functions (+ HMACs)
		- SHA-3 <sup>low priority</sup> <sup>[Wikipedia](https://en.wikipedia.org/wiki/SHA-3)</sup>
		- RIPEMD-160 <sup>low priority</sup> <sup>[Wikipedia](https://en.wikipedia.org/wiki/RIPEMD)</sup>
	- KDF
		- PBKDF2 <sup>priority</sup> <sup>[Wikipedia](https://en.wikipedia.org/wiki/PBKDF2)</sup>
		- Argon2 <sup>[Wikipedia](https://en.wikipedia.org/wiki/Argon2)</sup>
		- Lyra2 <sup>[Wikipedia](https://en.wikipedia.org/wiki/Lyra2)</sup>
- Optimizations
	- ```restrict``` pointers where applicable <sup>low priority</sup> <sup>[Wikipedia](https://en.wikipedia.org/wiki/Restrict)</sup>
