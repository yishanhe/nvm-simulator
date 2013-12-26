/*
 * =====================================================================================
 *
 *       Filename:  example1.c
 *
 *    Description:  example 1 array assignment:
 *
 *                  Create a NVRegion1
 *                  NVMalloc an int arrays(size=100)(array_shm)
 *                  NVNewRoot sets the array_shm address as the NVRoot.
 *                  Assignment value to this array
 *                  Print out the array value
 *                  Close this region
 *                  Quit.
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
    int shmid;
    int pid;
    int i,j;
    // int child_status;
    NVRDescr * nvrAddr;
#if defined(SHM)
    char name[]="/home/syi/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";
#elif defined(MMAP)
    char name[]="/home/syi/GitRepo/nvm-simulator/nvm.daemon/NVRegion4";
#endif
    char rootname1[]="root1";
    //char name[]="/scratch/syi.scratch/GitRepo/nvm-simulator/nvm.daemon/NVRegion1";


    nvrAddr = NVOpenRegion(name,0,SHM_SIZE);

    // shmid = nvrAddr->ID;

    // printf("The shmid is %d\n",shmid);
    printf("base addr of nvr  %p\n",nvrAddr);

    NVRDescrDump(nvrAddr);
    int array_size = 100;


    // if (mm_init()<0)
    // {
    //     e("MVMalloc initialization error.");
    // }

    int * array_shm = (int*)NVMalloc(nvrAddr,sizeof(int)*array_size);
    printf("the array addr is %p\n", array_shm);
    for (i = 0; i < array_size; ++i)
    {
        *(array_shm+i)=array_size-1-i;
    }
    for (j = 0; j < array_size; ++j)
    {
        printf("%d\n", *(array_shm+j));
    }

    if((NVNewRoot(nvrAddr, (void *)array_shm, rootname1, sizeof(int)))!=0){
        perror("Fail to NVNewRoot");
        exit(EXIT_FAILURE);
    }

    // NVFree(array_shm);


    DEBUG_OUTPUT("NVNewRoot Test Pass");
    NVRootmapDump(nvrAddr);


    if(NVCloseRegion(nvrAddr)!=0){
        perror("Fail to close region");
        exit(EXIT_FAILURE);
    }
    DEBUG_OUTPUT("NVCloseRegion Test Pass");



    // nvrAddr = NVOpenRegion(name,0,SHM_SIZE);

    // NVRootmapItem_t  * nvrmPtrIdx = (NVRootmapItem_t  *)NVFetchRoot(nvrAddr, rootname1);
    // int * array_shm_fetched = (int *) nvrmPtrIdx->location;
    // printf("the array addr is %p\n", array_shm_fetched);

    // for (j = 0; j < array_size; ++j)
    // {
    //     printf("%d\n", *(array_shm_fetched+j));
    // }


    //  BubbleSort(array_shm_fetched,array_size);
    // // QuickSort(array_shm_fetched,0,array_size-1);

    // for (j = 0; j < array_size; ++j)
    // {
    //     printf("%d\n", *(array_shm_fetched+j));
    // }

    // if(NVCloseRegion(nvrAddr)!=0){
    //     perror("Fail to close region");
    //     exit(EXIT_FAILURE);
    // }
    // DEBUG_OUTPUT("NVCloseRegion Test Pass");

    // if(NVDeleteRegion(name)==-1){
    //     perror("Fail to delete region");
    //     exit(EXIT_FAILURE);
    // }
    // DEBUG_OUTPUT("NVDeleteRegion Test Pass");



    exit(EXIT_SUCCESS);
}
