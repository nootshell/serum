# Serum #

***Experimental*** cryptography/networking multi-tool, licensed under the [MIT license](./COPYING.md).


## Features ##

- Core
	- Built-in self tests <sup>[Wikipedia](https://en.wikipedia.org/wiki/Built-in_self-test)</sup>
		- [Complete check-up](./libserum/core/self-test.h)
		- [Cryptographic hash functions](./libserum/crypto/hashing/self-test.h)
		- [ELF entrypoint](./libserum/core/entry/executable.c)
		- [Constructor self-tests](./libserum/core/entry/main.h)
	- Compile-time detection of:
		- [Operating system](./libserum/core/detect_os.h)
		- [Compiler](./libserum/core/detect_compiler.h)
		- [Platform](./libserum/core/detect_platform.h)
		- [Endianness](./libserum/core/detect_endianness.h)
	- [Detailed function return values](./libserum/core/result.h)
	- Time retrieval
		- [RDTSC](./libserum/core/time.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Time_Stamp_Counter)</sup>
		- [Nano, micro, milli, and full seconds](./libserum/core/time.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Orders_of_magnitude_(time))</sup>
	- Universal memory helpers
		- [(Dynamic) stack allocation](./libserum/core/memory.h)
		- [Memory (un)locking](./libserum/core/memory.h)
	- [Variable size integer encoding/decoding](./libserum/core/varsize.h)
- Crypto
	- Hashing
		- [MD5](./libserum/crypto/hashing/md5.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/MD5)</sup>
		- [SHA-2](./libserum/crypto/hashing/sha2.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/SHA-2)</sup>
	- HMAC
		- [HMAC-\*](./libserum/crypto/hmac/hmac.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Hash-based_message_authentication_code)</sup>
		- [HMAC-MD5](./libserum/crypto/hmac/hmac-md5.h)
		- [HMAC-SHA2](./libserum/crypto/hmac/hmac-sha2.h)
	- KDF
		- [PBKDF2-\*](./libserum/crypto/kdf/pbkdf2.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/PBKDF2)</sup>
		- [PBKDF2-MD5](./libserum/crypto/kdf/pbkdf2-md5.h)
		- [PBKDF2-SHA2](./libserum/crypto/kdf/pbkdf2-sha2.h)
	- Key exchange
		- [X25519](./libserum/crypto/key_exchange/x25519.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Curve25519)</sup>
	- Padding
		- [ISO 9797-1 methods 1 and 2]()  <sup>[Wikipedia](https://en.wikipedia.org/wiki/ISO/IEC_9797-1#Padding)</sup>
	- Pseudo-random number generation
		- [Reading from a **/dev**ice (or file)](./libserum/crypto/prng/device.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki//dev/random)</sup>
		- [ISAAC](./libserum/crypto/prng/isaac.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/ISAAC_(cipher))</sup>
	- Storage
		- [Keys](./libserum/crypto/storage/key.h)
	- Symmetric
		- Block cipher modes of operation
			- [CBC](./libserum/crypto/symmetric/modes/cbc.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#CBC)</sup>
			- [PCBC](./libserum/crypto/symmetric/modes/cbc.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#PCBC)</sup>
		- [Rijndael](./libserum/crypto/symmetric/rijndael.h)[[-(P)CBC]](./libserum/crypto/symmetric/rijndael-cbc.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Rijndael)</sup>
		- [Salsa20](./libserum/crypto/symmetric/salsa20.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Salsa20)</sup>
		- [XXTEA](./libserum/crypto/symmetric/xxtea.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/XXTEA)</sup>
- Data
	- Conversion
		- [Strings to integers](./libserum/data/conversion/integers.h)
- Debug
	- [Logging](./libserum/debug/log.h)
	- [Memory dump functions](./libserum/debug/memdump.h)
- Math
	- [Curve25519](./libserum/math/curve25519.h)
- Networking
	- [Universal socket](./libserum/networking/socket.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Network_socket)</sup>
	- [Packets](./libserum/networking/packet.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Network_packet)</sup>
- Runtime
	- [Universal mutexes](./libserum/runtime/mutex.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Mutual_exclusion)</sup>
	- [Universal threading](./libserum/runtime/thread.h) <sup>[Wikipedia](https://en.wikipedia.org/wiki/Thread_(computing))</sup>

TODO
===
- Crypto
	- Key exchange
		- *None currently*
	- Block ciphers
		- *None currently*
	- Stream ciphers
		- *None currently*
	- Hash functions (+ HMACs)
		- SHA-3 <sup>low priority</sup> <sup>[Wikipedia](https://en.wikipedia.org/wiki/SHA-3)</sup>
		- RIPEMD-160 <sup>low priority</sup> <sup>[Wikipedia](https://en.wikipedia.org/wiki/RIPEMD)</sup>
	- KDF
		- Argon2 <sup>[Wikipedia](https://en.wikipedia.org/wiki/Argon2)</sup>
		- Lyra2 <sup>[Wikipedia](https://en.wikipedia.org/wiki/Lyra2)</sup>
- Optimizations
	- ```restrict``` pointers where applicable <sup>low priority</sup> <sup>[Wikipedia](https://en.wikipedia.org/wiki/Restrict)</sup>
