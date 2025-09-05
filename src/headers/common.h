#ifndef INCLUDE_COMMON
#define INCLUDE_COMMON

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  const char *ptr;
  uint32_t len;
} Str;

#define STR_NONE ((Str){0})
#define STRINGIFY_INNER(x) #x
#define STRINGIFY(x) STRINGIFY_INNER(x) 
#define CSTR_LEN(str) (sizeof(str) - 1)
#define STR(str) ((Str){ (str), CSTR_LEN(str) })

#define PACKED __attribute__((packed))
#define ALIGNED(n) __attribute__((aligned(n)))
#define UNREACHABLE __builtin_unreachable();
#define NORETURN __attribute__((noreturn))
// NOTE: https://stackoverflow.com/questions/29117836/attribute-const-vs-attribute-pure-in-gnu-c
#define PURE __attribute__((pure))
#define CONST __attribute__((const))

#define ANSI_RESET "\x1b[0m"
#define ANSI_BOLD "\x1b[1m"
#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define UPPER_BOUND(a, b) MIN(a, b)
#define LOWER_BOUND(a, b) MAX(a, b)

#define DEBUGD(var) printf(STRINGIFY(var) "=%d\n", var)
#define DEBUGC(var) printf(STRINGIFY(var) "='%c'\n", var)
#define DEBUGS(var) printf(STRINGIFY(var) "=\"%s\"\n", var)
#define DEBUGX(var) printf(STRINGIFY(var) "=0x%x\n", var)

#define LOG(fmt, ...) \
  printf("[LOG] %s " __FILE__ ":" STRINGIFY(__LINE__) " " fmt, __func__, ##__VA_ARGS__)

typedef struct {
  Str left;
  Str right;
} StrPair;

// string.c
bool CONST str_eq(Str a, Str b);

// fs.c
Str map_file_readonly(const char *filename);

#endif
