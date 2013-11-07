/*
 * =====================================================================================
 *
 *       Filename:  nv_mm.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  11/07/2013 01:21:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Syi,
 *   Organization:  CS@WM
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "global.h"


NVRootmapItem_t * nv_dataregion_init(NVRDescr * nvrAddr){
    NVRootmapItem_t * nvrmPtr;
    nvrmPtr = nvrAddr+nvrAddr->rootMapOffset;
//    nvrmPtr = offset2addr(nvrAddr,nvAddr->rootMapOffset);
//    memset();
}


