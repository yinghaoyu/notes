#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	int fd;
	int pid;
	unsigned long pte;
	unsigned long addr;
	unsigned long phy_addr;
	char procbuf[64] = {0};
	pid = atoi(argv[1]);
	addr = atol(argv[2]);
	sprintf(procbuf, "/proc/%d/pagemap", pid);
	fd = open(procbuf, O_RDONLY);
	size_t offset = (addr/4096) * sizeof(unsigned long);
	lseek(fd, offset, SEEK_SET);
	read(fd, &pte, sizeof(unsigned long));
	phy_addr = (pte & ((((unsigned long)1) << 55) - 1))*4096 + addr%4096;
	printf("phy addr:%lu\n", phy_addr);
	return 0;
}
