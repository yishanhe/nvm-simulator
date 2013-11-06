/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  11/06/2013 01:19:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "global.h"
#include <stdio.h>

int main(int argc, const char *argv[])
{
    NVRDescr_t * t = (NVRDescr *)malloc(sizeof(NVRDescr));
    printf("sizeof nvrdescr %d\n", sizeof(NVRDescr));
    printf("sizeof name %d\n", sizeof(t->name));
    return 0;
}

