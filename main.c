/*
 * =====================================================================================
 *
 *       Filename:  mainhm.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  11/01/2013 01:31:03 PM
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
#include <string.h>
#include "hashmap.h"
#include "global.h"


void to_string_dump(FILE *output, char *key, void *value) {
    fprintf(output, "int: %d", *((int *)value));
}

void to_string_print(FILE *output, char *key, void *value) {
    fprintf(output, "%s: %d\n", key,*((int*)value));
}


int main(int argc, const char *argv[])
{
    hashmap *x =  hm_create(2,0.1f,13);
    hm_dump(x, stdout, to_string_dump);

    // create a nv region and assert its address to the hashmap
    //
    //
    //
    //
    char * nv1="nv1";

    if (hm_isset(x,nv1)==1) {// this region name exists
        DEBUG_OUTPUT("nv1 existed!");
        // open it return the address
        // this address is stored
    }
    else {
        DEBUG_OUTPUT("nv1 not existed!");
    }



    return 0;
}

