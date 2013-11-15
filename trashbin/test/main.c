/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  11/03/2013 09:14:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

union mem_word {
    void *mw_vp;
    void (*mw_fp)(void);
    char *mw_cp;
    long mw_l;
    double mw_d;
};

int main(int argc, const char *argv[])
{
    union  mem_word  abc;
    printf("%ld\n",sizeof(abc));
    printf("%ld\n",sizeof(double));
    printf("%ld\n",sizeof(long));
    printf("%ld\n",sizeof(int));
    printf("%ld\n",sizeof(char));
    printf("%ld\n",sizeof(long long));
    return 0;
}

