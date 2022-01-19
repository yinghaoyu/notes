#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
int main()
{
    char* addr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    strcpy(addr, "Zhejiang wenzhou pixie shi");
    printf("addr: %lu   pid:%d\n", addr, getpid());
    printf("before:%s  \n", addr);
	getchar();
    printf("after:%s\n", addr);
    return 0;
}
