/*
 * =====================================================================================
 *
 *       Filename:  example2.c
 *
 *    Description:  example 2 Bubble Sort
 *
 *        Version:  1.0
 *        Created:  12/05/2013 01:45:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Syi,
 *   Organization:  CS@WM
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "global.h"
#include "nv_api.h"
#include "memlib.h"
#include "mm.h"


void BubbleSort(int *a, int n);

void BubbleSort(int *a, int n) {
    int tmp,i,j; 
    for (i=0;i<n;i++) {
        for (j=0;j<n-i;j++){
            if(*(a+j) > *(a+j+1)){
                tmp = *(a+j);
                *(a+j) = *(a+j+1);
                *(a+j+1) = tmp;
            }
        }
    }
}





int Partition(int *array, int i, int j);
void QuickSort(int *array, int low, int high);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  Partition
 *  Description:  quicksort sorting 
 * =====================================================================================
 */
        int
Partition ( int * array, int i, int j )
{
        int t = * (array + i);
        while ( i < j ) {
                while ( i < j && *(array + j ) >= t )
                        j--;
                if ( i < j) {
                        * ( array + i ) = * ( array + j );
                        * ( array + j) = t ;
                        i++;
                }
                while ( i < j && *(array + i ) <= t )
                        i++;
                if ( i < j) {
                        * ( array + j ) = * ( array + i );
                        * ( array + i) = t ;
                        j--;

                }
        }
        * ( array + i) = t;
        return i;
}                /* -----  end of function Partition  ----- */


void QuickSort(int *array, int low, int high ) {
        int mid;
        if ( low < high ) {
                mid = Partition(array,low,high);
                QuickSort(array,low,mid-1);
                QuickSort(array,mid+1,high);
        }
}


int main(int argc, const char *argv[])
{
    int shmid,j;

    char rootname1[]="root1";
   // char name[]="/scratch/syi.scratch/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";
    char name[]="/home/syi/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";

    // open existed region 
    NVRDescr * nvrAddr = NVOpenRegion(name,0,SHM_SIZE);
    if (nvrAddr==NULL) {
        e("NVOpenRegion fails");
    }
    printf("base addr of nvr  %p\n",nvrAddr);
    DEBUG_OUTPUT("NVOpenRegion Re-open Existed Region Test Pass");
    
    NVRDescrDump(nvrAddr);
    NVRootmapDump(nvrAddr);

    // shmid = nvrAddr->ID;
    // printf("The shmid is %d\n",shmid);
    
    // void *NVFetchRoot(NVRDescr * addr, char *name);
    NVRootmapItem_t  * nvrmPtrIdx = (NVRootmapItem_t  *)NVFetchRoot(nvrAddr, rootname1);


 //   int array_size = (int)nvrmPtrIdx->type;
    int array_size = 100;
    printf("the array size is %d\n", array_size);
 
    // get the array address
    int * array_shm = (int *) nvrmPtrIdx->location;
    printf("the array addr is %p\n", array_shm);
  //  int * array = (int*)malloc(sizeof(int)*array_size);


    // if (mm_init()<0)
    // {
    //     e("MVMalloc initialization error.");
    // }



    // for (j = 0; j < array_size; ++j)
    // {
    //     printf("%d\n", *(array_shm+j));
    // }

    BubbleSort(array_shm,array_size);

    for (j = 0; j < array_size; j++)
    {
        printf("%d\n", *(array_shm+j));
    } 

    // NVFree(array_shm); // need to invalidate the correspoding nvroot


    // NVRDescrDump(nvrAddr);
    // NVRootmapDump(nvrAddr);


    if(NVCloseRegion(nvrAddr)!=0){
        perror("Fail to close region");
        exit(EXIT_FAILURE);
    }
    DEBUG_OUTPUT("NVCloseRegion Test Pass");
    //kill(pid,SIGUSR1)


    if(NVDeleteRegion(name)==-1){
        perror("Fail to delete region");
        exit(EXIT_FAILURE);
    }
    DEBUG_OUTPUT("NVDeleteRegion Test Pass");


    exit(EXIT_SUCCESS);
}


