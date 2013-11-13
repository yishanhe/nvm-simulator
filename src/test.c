#include "nv_mm.h"
#include <stdio.h>
#define ALIGNMENT 8
#define ALIGN(size) (((size)+(ALIGNMENT-1))& ~0x7)
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

extern int errno;
extern char *nv_mm_start_brk;
extern char *nv_mm_brk;
extern char *nv_mm_max_addr;


int main(int argc, char const *argv[])
{
	int size=9;

    int newsize = ALIGN(size+SIZE_T_SIZE);

    printf("the new size if %d\n", newsize);

    // void *p = nvmm_sbrk(newsize);
    // if(p==(void *)-1) {
    //     return NULL;
    // }
    // else {
    //     *(size_t *)p = size;
    //     return (void *)((char *)p+SIZE_T_SIZE);
    char * a="123123123";
    printf("%p\n",a);
    printf("%p\n",(size_t *)a );
	return 0;
}