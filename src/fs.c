#include "common.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <assert.h>

// Returns NULL on failure
Str map_file_readonly(const char *filename) {
  int fd = open(filename, O_RDONLY);
  if (fd < 0) return STR_NONE;

  struct stat st;
  if (fstat(fd, &st) < 0) return STR_NONE;
  if (st.st_size > UINT32_MAX) return STR_NONE;

  const char *file = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (file == MAP_FAILED) return STR_NONE;

  close(fd);
  return (Str){ file, st.st_size };
}
