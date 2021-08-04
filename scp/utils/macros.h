#ifndef SCP_MACROS_H
#define SCP_MACROS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef __GNUC__
#define  __attribute__(x) /**/
#endif

#define scpAttribute_noreturn __attribute__((noreturn))
#define scpAttribute_deprecated __attribute__((deprecated))
#define scpAttribute_used __attribute__((used))
#define scpAttribute_unused __attribute__((unused))
#define scpAttribute_noinline __attribute__((noinline))
#define scpAttribute_always_inline __attribute__((always_inline))
#define scpAttribute_constructor __attribute__((constructor))
#define scpAttribute_destructor __attribute__((destructor))
#define scpAttribute_pure __attribute__((pure))
#define scpAttribute_const __attribute__((const))
#define scpAttribute_malloc __attribute__((malloc))
#define scpAttribute_format_printf(m, n) __attribute__((format(printf, m, n)))
#define scpAttribib_format_scanf(m, n) __attribute__((format(scanf, m, n)))

#ifdef __STDC__
#ifndef __STDC_VERSION__
#error "ANSI C is not supported"
#define SCP_STDC_PRE11
#define SCP_STDC_PRE99
#define SCP_STDC_90
#define SCP_STDC_89
#elif __STDC_VERSION__ >= 202000L
#define SCP_STDC2X
#elif __STDC_VERSION__ >= 201710L
#define SCP_STDC18
#define SCP_STDC17
#elif __STDC_VERSION__ >= 201112L
#define SCP_STDC11
#define SCP_STDC1X
#elif __STDC_VERSION__ >= 199901L
#define SCP_STDC_PRE11
#define SCP_STDC99
#elif __STDC_VERSION__ >= 199409L
#error "c95 is not suported"
#define SCP_STDC_PRE11
#define SCP_STDC_PRE99
#define SCP_STDC95
#define SCP_STDC94
#endif
#else
#error "K&R C is not supported"
#define SCP_STDC_PRE11
#define SCP_STDC_PRE99
#define SCP_STDC72
#endif

#ifdef SCP_STDC_PRE11
#define scpNoreturn scpAttribute_noreturn
#else
#define scpNoreturn _Noreturn
#endif

#define SCP_TO_STRING_NX(x) #x
#define SCP_TO_STRING(x) SCP_TO_STRING_NX(x)

#define SCP_CONCAT_NX(a, b) a##b
#define SCP_CONCAT(a, b) SCP_CONCAT_NX(a, b)

#ifndef SCP_PEDANTIC
#define SCP_LAMBDA(return_type, body) ({ return_type _ body _; })
#endif /* SCP_PEDANTIC */

#define SCP_SWAP(type, a, b) { type tmp = a; a = b; b = tmp; }

typedef void (*scpFunc_copy)(const void* data, void* new_data);
typedef void* (*scpFunc_clone)(const void* data);
typedef void (*scpFunc_map)(void* data);
typedef void (*scpFunc_map_index)(void* data, size_t index, size_t size);
typedef void (*scpFunc_print)(const void* data);
typedef uint64_t (*scpFunc_hash)(const void* data);
typedef long (*scpFunc_cmp)(const void* a, const void* b);

#endif /* SCP_MACROS_H */