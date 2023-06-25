#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4096

int main()
{
	int fd;
	char *buf;
	char *msg = "Test message smiley:))";

	char *name = "mmap_test";


	fd = shm_open(shm_name, O_RDWR, 0);
	ftruncate(fd, PAGE_SIZE);

	buf = 
}
