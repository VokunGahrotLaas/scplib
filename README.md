# scplib
C multipurpose library. \
This library only contains headers, no `.c` to compile. \
Compatible with stdc versions from c99 to c2X with flags like `-Wall -Wextra -Wconversion -Werror`. \
For `-Wpedantic` you must define `SCP_PEPANTIC` (`-D` or `#define`) to disable some functionalities like lambdas. \
(if someone knows how to detect pedantic with the preprocessor directly, dm me please)

## Credits
* [Write A Hash Table](https://github.com/jamesroutley/write-a-hash-table) by [jamesroutley](https://github.com/jamesroutley/)

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
For this reason you must define SCP_IMPLEMENTATION in __one and only one__ of the translation units (usualy the main.c file). \
You can thus include the lib directly as follows:
```c
#define SCP_IMPLEMENTATION
#include <scp/containers/hashmap.h>
```
Or add -DSCP_IMPLEMENTATION to the compiler arguments.

## Docs (short)
* __utils.h__: everithing that is inside the `utils/` folder
* __utils/__:
	* __macros.h__: utility macros
		* `__attribute__((<attribute>))` as `scpAttribute_<attribute>`
		* `scpNoreturn` is definied as `_Noreturn` normally or `scpAttribute_noreturn` with c99 and under
		* `scpBool`, `scpTrue` and `scpFalse`, macros for `_Bool`, `1` and `0`
		* `#X` as `SCP_TO_STRING_NX(X)` and `SCP_TO_STRING(X)`
		* `a##b` as `SCP_CONCAT_NX(a, b)` and `SCP_CONCAT(a, b)`
		* lambda functions with `SCP_LAMBDA(return_type, body)` (not pedantic, disable with `-DSCP_PEDANTIC`)
		* swaping contents of variables with `SCP_SWAP(type, a, b)`
		* function typedefs as `scpFunc_<function>`
	* __string.h__: utility functions for strings
		* `char* scpString_to_lower(char* str)`: change str to lowercase, returns str
		* `char* scpString_to_upper(char* str)`: change str to uppercase, returns str
		* `char* scpString_new_lower(const char* str)`: change str to lowercase, returns a new string (must be free'd)
		* `char* scpString_new_upper(const char* str)`: change str to uppercase, returns a new string (must be free'd)
	* __io.h__: utility functions for input/output
		* `scpBool scpIO_flush_buffer(FILE* stream, char* buffer)`: utility for fgets, will check if the used inputed a line bigger than the cappacity of the buffer and if not it will replace the newline character '\n' with a null character '\0', otherwise it will empty the buffer until a newline is uncountered, returns `scpTrue` if it emptied the buffer, `scpFalse` otherwise.
	* __print.h__: functions of type `scpFunc_print`
	* __fprint.h__: functions of type `scpFunc_fprint`
	* __copy.h__: functions of type `scpFunc_copy`
	* __clone.h__: functions of type `scpFunc_clone`
	* __hash.h__: functions of type `scpFunc_hash`
	* __cmp.h__: functions of type `scpFunc_cmp`
* __maths.h__: everithing that is inside the `maths/` folder
* __maths/__: general maths utility functions
	* __binpow.h__: integers binary exponanciation
		* `scpMaths_binpow_(i|il|ill|u|ul|ull)` for individual function
		* `scpMaths_binpow` `_Generic` with type deduced from the first argument
	* __primes.h__: prime numbers generation
		* `scpPrimes_array`: scpArray in which the prime numbers are stored
		* `scpPrimes_gen(n)`: generates prime numbers until n
		* `scpPrimes_next(n)`: returns the first prime number after n
* __containers/__: OOP wrappers for data structures
	* __array.h__: OOP wrapper for an array as `scpArray`
		* `struct scpArray`: array's struct
			* `struct scpArrayType* .type`: pointer to virtual functions
			* `void* .data`: pointer to the raw array
			* `size_t .count`: number of elements in the array
			* `size_t .size`: size of one element (as given by `sizeof`)
		* `scpArray`: `struct scpArrayType` instance, virtual table of array
			* `struct scpArray* .new(size_t count, size_t size)`: creates a new instance of `struct scpArray`, cf. `.count` and `.size` for info on parameters
			* `void .delete(struct scpArray* array)`: deletes the array
			* `void .resize(struct scpArray* array, size_t count)`: assigns a new count to the array
			* `void* .at(struct scpArray* array, size_t index)`: returns a pointer to the index-th element of the array
			* for `copy`, `fcopy`, `clone`, `fclone`, `map`, `map_index`, `print` and `fprint` cf. __utils.h__
	* __vector.h__: OOP wrapper for a dynamic array as `scpVector`
		* `struct scpVector`: vector's struct
			* `struct scpVectorType* .type`: pointer to virtual functions
			* `void* .data`: pointer to the raw vector
			* `size_t .count`: number of elements in the vector
			* `size_t .size`: size of one element (as given by `sizeof`)
			* `size_t .reserved`: number of element the vector can contain before needing to resize
		* `scpVector`: `struct scpVectorType` instance, virtual table of vector
			* `struct scpVector* .new(size_t count, size_t size)`: creates a new instance of `struct scpVector`, cf. `.count` and `.size` for info on parameters
			* `void .delete(struct scpVector* vector)`: deletes the vector
			* `void .resize(struct scpVector* vector, size_t count)`: assigns a new count to the vector
			* `void .reserve(struct scpVector* vector, size_t count)`: assigns a new reserved to the vector
			* `void* .at(struct scpVector* vector, size_t index)`: returns a pointer to the index-th element of the array
			* `void* .append(struct scpVector* vector)`: adds an element to the vector and returns a pointer to said element
			* `void .pop(struct scpVector* vector)`: removes the last element, throws an exception if the vector is empty
			* for `copy`, `fcopy`, `clone`, `fclone`, `map`, `map_index`, `print` and `fprint` cf. __utils.h__
	* __hashmap.h__: OOP wrapper for a hash table, mapping key-value pair as `scpHashMap`
		* `struct scpHashMap`: hashmap's struct
			* `.type`: pointer to virtual functions
			* `.base_size`: desired size for items
			* `.size`: reserved space for items (next prime to `.base_size`)
			* `.count`: number of items
			* `.items`: pointer to raw items
			* `.hash_a`: first key hashing function (scpFunc_hash)
			* `.hash_b`: second key hashing function (scpFunc_hash)
			* `.cmp`: key comparing function (scpFunc_cmp)
		* `scpHashMap`: `struct scpHashMapType` instance, virtual table of hashmap
			* `struct scpHashMap* .new(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp)`: creates a new instance of `struct scpHashMap`, cf. `.hash_a`, `.hash_b` and `.cmp` for info on parameters
			* `void .delete(struct scpHashMap* hashmap)`: deletes the hashmap
			* `bool .insert(struct scpHashMap* hashmap, const void* key, void* value)`: inserts or replaces a key value pair in the hashmap, returns true if the key was already in the hashtable
			* `bool .remove(struct scpHashMap* hashmap, const void* key)`: removes a key value pair of the hashmap, returns true if the key was in the hashtable
			* `void* .search(struct scpHashMap* hashmap, const void* key)`: returns the value associated to the given key
	* __hashset.h__: OOP wrapper for a hash table, mapping single key as `scpHashSet`
		* `struct scpHashSet`: hashset's struct
			* `.type`: pointer to virtual functions
			* `.base_size`: desired size for items
			* `.size`: reserved space for items (next prime to `.base_size`)
			* `.count`: number of items
			* `.items`: pointer to raw items
			* `.hash_a`: first key hashing function (scpFunc_hash)
			* `.hash_b`: second key hashing function (scpFunc_hash)
			* `.cmp`: key comparing function (scpFunc_cmp)
		* `scpHashSet`: `struct scpHashSetType` instance, virtual table of hashset
			* `struct scpHashSet* .new(scpFunc_hash hash_a, scpFunc_hash hash_b, scpFunc_cmp cmp)`: creates a new instance of `struct scpHashSet`, cf. `.hash_a`, `.hash_b` and `.cmp` for info on parameters
			* `void .delete(struct scpHashSet* hashset)`: deletes the hashset
			* `bool .insert(struct scpHashSet* hashset, const void* key)`: inserts or replaces a key in the hashset, returns true if the key was already in the hashtable
			* `bool .remove(struct scpHashSet* hashset, const void* key)`: removes a key of the hashset, returns true if the key was in the hashtable
			* `bool .search(struct scpHashSet* hashset, const void* key)`: returns true if the key was in the hashtable
* __exceptions.h__:
	* `scpException_<name>`: individual exception functions
	* `SCP_EXCEPTION(exception, ...)`: calls `exeption` with `__FILE__`, `__func__` and `__LINE__` automatically
* __int128.h__: wrapper for GNU C `__int128` (not pedantic)
	* typedefs and printing functions for signed and unsigned int128
* __stream.h__: generic OOP stream for files, strings, etc...
* __time.h__: function timing utility
