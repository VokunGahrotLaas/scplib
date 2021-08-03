# scplib
C multipurpose library. \
This library only contains headers, no `.c` to compile. \
Compatible with stdc version c99 to c2X with `-Wall -Wextra -Wconversion -Werror`. \
For `-Wpedantic` you must declare `SCP_PEPANTIC` (`-D` or `#define`) to disable some functionalities like lambdas. \
(if someone knows how to detect pedantic with the preprocessor directly, dm me please)

## Credits
(future)

## Makefile

### Tests
Tests are located in the `tests/` folder and can be runned from the makefile with `make run_test_<name>`. \
(remove `run_` to just compile without running) \
To enable pedantic mode use `make run_strict_test_<name>`. \
To run all tests (not advised) use `make run_tests` or `make run_strict_tests` \
To clean all compiled tests use `make mrproper`.

### Installation
Run `make install` as root, it will copy the `scp/` folder in your `/usr/includes/`. \
As said in the intro no `.c` to compile, this is a header only library. \
You can include the lib directly as `<scp/containers/hashmap.h>`.

## Docs (short)
* __utils.h__: everithing that is inside the `utils/` folder
* __utils/__:
	* __macros.h__: utility macros
		* `__attribute__((<attribute>))` as `scpAttribute_<attribute>`
		* `scpMacro_noreturn` is definied as `_Noreturn` normally or `scpAttribute_noreturn` in c99.
		* `#X` as `SCP_TO_STRING_NX(X)` and `SCP_TO_STRING(X)`
		* `a##b` as `SCP_CONCAT_NX(a, b)` and `SCP_CONCAT(a, b)`
		* lambda functions with `SCP_LAMBDA(return_type, body)` (not pedantic, disable with `-DSCP_PEDANTIC`)
		* swaping contents of variables with `SCP_SWAP(type, a, b)`
		* function typedefs as `scpFunc_<function>`
	* __print.h__: functions of type `scpFunc_print`
	* __copy.h__: functions of type `scpFunc_copy`
	* __clone.h__: functions of type `scpFunc_clone`
	* __hash.h__: functions of type `scpFunc_hash`
	* __cmp.h__: functions of type `scpFunc_cmp`
* __maths/__:
	* __binpow.h__: integers binary exponanciation
		* `scpMaths_binpow_(i|il|ill|u|ul|ull)` for individual function
		* `scpMaths_binpow` `_Generic` with type deduced from the first argument
	* __primes.h__: prime numbers generation
		* `scpPrimes_array`: scpArray in which the prime numbers are stored
		* `scpPrimes_gen(n)`: generates prime numbers until n
		* `scpPrimes_next(n)`: returns the first prime number after n
* __containers/__:
	* __array.h__: wrapper for an array as `struct scpArray`
    	* `.type`: pointer to virtual functions
		* `.data`: pointer to the raw array
		* `.count`: number of elements in the array
		* `.size`: size of one element (as given by `sizeof`)
	* __vector.h__: wrapper for a dynamic array as `struct scpVector`
    	* `.type`: pointer to virtual functions
		* `.data`: pointer to the raw array
		* `.count`: number of elements in the array
		* `.size`: size of one element (as given by `sizeof`)
		* `.reserved`: number of reserved elements
	* __hashmap.h__: wrapper for a hash table, mapping key-value pair as `scpHashMap`
    	* `.type`: pointer to virtual functions
		* `.base_size`: desired size for items
		* `.size`: reserved space for items (next prime to `.base_size`)
		* `.count`: number of items
		* `.items`: pointer to raw items
		* `.hash_a`: first key hashing function (scpFunc_hash)
		* `.hash_b`: second key hashing function (scpFunc_hash)
		* `.cmp`: key comparing function (scpFunc_cmp)
	* __hashset.h__: wrapper for a hash table, mapping single key as `scpHashSet`
    	* `.type`: pointer to virtual functions
		* `.base_size`: desired size for items
		* `.size`: reserved space for items (next prime to `.base_size`)
		* `.count`: pointer to raw items
		* `.items`: raw pointer to item buckets
		* `.hash_a`: first key hashing function (scpFunc_hash)
		* `.hash_b`: second key hashing function (scpFunc_hash)
		* `.cmp`: key comparing function (scpFunc_cmp)
* __exceptions.h__:
	* `scpException_<name>`: individual exception functions
	* `SCP_EXCEPTION(exception, ...)`: calls `exeption` with `__FILE__`, `__func__` and `__LINE__` automatically
* __int128.h__: wrapper for GNU C `__int128` (not pedantic)
	* typedefs and printing functions for signed and unsigned int128
* __io.h__: generic stream for files, strings, etc... (in dev)
* __time.h__: function timing utility
