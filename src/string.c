#include <common.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

bool CONST str_eq(Str a, Str b) {
  if (a.len != b.len) return false;
  return !strncmp(a.ptr, b.ptr, a.len);
}

