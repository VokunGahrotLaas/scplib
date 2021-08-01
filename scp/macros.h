#ifndef SCP_MACROS_H
#define SCP_MACROS_H

#include <stdint.h>

#define SCP_TO_STRING_NX(x) #x
#define SCP_TO_STRING(x) SCP_TO_STRING_NX(x)

#define SCP_CONCAT_NX(a, b) a ## b
#define SCP_CONCAT(a, b) SCP_CONCAT_NX(a, b)

#ifndef SCP_PEDANTIC

#define SCP_LAMBDA(return_type, body) ({ return_type _ body _; })

#endif // SCP_PEDANTIC

typedef void (*scpFunc_copy)(void* data, void* new_data);
typedef void* (*scpFunc_clone)(void* data);
typedef void (*scpFunc_map)(void* data);
typedef void (*scpFunc_map_index)(void* data, size_t index, size_t size);
typedef void (*scpFunc_print)(void* data);
typedef uint64_t (*scpFunc_hash)(void* data);

#endif // SCP_MACROS_H