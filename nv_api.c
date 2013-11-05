/*
 * =====================================================================================
 *
 *       Filename:  nv_api.c
 *
 *    Description:  provide api for nvregion operation
 *
 *        Version:  1.0
 *        Created:  10/31/2013 06:14:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Syi
 *   Organization:  CS@WM
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "nv_api.h"



/*-----------------------------------------------------------------------------
 *
 *  this is reasonable since if we are using a share memory as nvregion,
 *  we can return this address and actually we will put the NVRDescr structure
 *  at this position.  the offset + base address if we put all nvregion in a big
 *  chunk of shared memory. So let's do  nvregion=shm first.
 *-----------------------------------------------------------------------------*/

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  NVOpenRegion
 *  Description:
 * =====================================================================================
 */
NVRDescr * NVOpenRegion(char * name,            /* region name */
                        void * startingAddr,    /* starting addr */
                        int size)               /* region sizze */
{

}


}



