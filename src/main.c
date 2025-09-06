#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "common.h"
#include "emu.h"

#include "fs.c"
#include "emu.c"

int main(int argc, char *argv[]) {
  assert(argc == 2);
  const char *filename = argv[1];

  Str file = map_file_readonly(filename);
  assert(file.len < 64 * 1024);

  Emu *e = malloc(sizeof *e);
  Emu_reset(e, file);
  Emu_run(e, UINT32_MAX);

  return 0;
}
