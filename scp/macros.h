#ifndef SCP_MACROS_H
#define SCP_MACROS_H

#define SCP_TO_STRING_NX(x) #x
#define SCP_TO_STRING(x) SCP_TO_STRING_NX(x)

#define SCP_CONCAT_NX(a, b) a ## b
#define SCP_CONCAT(a, b) SCP_CONCAT_NX(a, b)

#define SCP_LAMBDA(return_type, body) ({ return_type _ body _; })

#endif // SCP_MACROS_H