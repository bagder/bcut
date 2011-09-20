#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BCUT_VERSION "0.1"

int main(int argc, char **argv)
{
  int fd;
  off_t req_offset;
  off_t ask_offset;
  size_t length;
  size_t extra;
  unsigned char *contents;
  long pagesize;

  if(argc < 4) {
    fprintf(stderr, "bcut version %s\nUsage: bcut <offset> <length> <file>\n",
            BCUT_VERSION);
    return 1;
  }

  req_offset = (off_t)strtoll(argv[1], NULL, 0);
  length = (size_t)strtol(argv[2], NULL, 0);

  fd = open(argv[3], O_RDONLY);
  if(fd == -1)
    return 3;

  pagesize = sysconf(_SC_PAGE_SIZE);
  if(pagesize < 1)
    return 4;

  /* make the ask offset aligned to the page size */
  ask_offset = (req_offset / pagesize);
  ask_offset *= pagesize;

  extra = req_offset - ask_offset;

  contents = mmap(NULL, length + extra, PROT_READ, MAP_SHARED, fd, ask_offset);
  if(MAP_FAILED == contents)
    return 2;

  write(1, &contents[extra], length);

  close(fd);

  return 0;
}
