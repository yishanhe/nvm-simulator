/*
 * =====================================================================================
 *
 *  Adapted from CSAPP of CMU
 *
 * =====================================================================================
 */
 
#ifndef  mm_INC
#define mm_INC

#include <stdio.h>

extern int mm_init (void);
extern void *mm_malloc (size_t size);
extern void mm_free (void *ptr);
extern void *mm_realloc(void *ptr, size_t size);
static void examine_heap();


#endif
