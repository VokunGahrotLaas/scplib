#ifndef SCP_MACROS_H
#define SCP_MACROS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>

#ifndef __GNUC__
#define  __attribute__(x) /*NOTHING*/
#endif

#define scpMacro_deprecated __attribute__((deprecated))
#define scpMacro_used __attribute__((used))
#define scpMacro_unused __attribute__((unused))
#define scpMacro_noinline __attribute__((noinline))
#define scpMacro_always_inline __attribute__((always_inline))
#define scpMacro_constructor __attribute__((constructor))
#define scpMacro_destructor __attribute__((destructor))
#define scpMacro_pure __attribute__((pure))
#define scpMacro_const __attribute__((const))
#define scpMacro_malloc __attribute__((malloc))
#define scpMacro_format_printf(m, n) __attribute__((format(printf, m, n)))
#define scpMacro_format_scanf(m, n) __attribute__((format(scanf, m, n)))

#define SCP_TO_STRING_NX(x) #x
#define SCP_TO_STRING(x) SCP_TO_STRING_NX(x)

#define SCP_CONCAT_NX(a, b) a##b
#define SCP_CONCAT(a, b) SCP_CONCAT_NX(a, b)

#ifndef SCP_PEDANTIC
#define SCP_LAMBDA(return_type, body) ({ return_type _ body _; })
#endif // SCP_PEDANTIC

#define SCP_SWAP(type, a, b) { type tmp = a; a = b; b = tmp; }

typedef void (*scpFunc_copy)(const void* data, void* new_data);
typedef void* (*scpFunc_clone)(const void* data);
typedef void (*scpFunc_map)(void* data);
typedef void (*scpFunc_map_index)(void* data, size_t index, size_t size);
typedef void (*scpFunc_print)(const void* data);
typedef uint64_t (*scpFunc_hash)(const void* data);
typedef int (*scpFunc_cmp)(const void* a, const void* b);

#endif // SCP_MACROS_H