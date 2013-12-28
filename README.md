nvm-simulator
=============

cs652 course project

Simulate a non-volatile memory using a chunk of main memory.
Generally, process never share resourcces unless the shared memory. 
This project is using this mechanism to implement an NVM simulator.


#### Project Requirements
- NVRegion related APIs
- DataRegion realted APIs


#### Assumptions, requirements and limitations
- System V Shared Memory support
- All NVRegions are attached at the same starting address of the process address space.
- The process won't crash and there is no power outage.
- Currently, there is only file-level lock granularity (The programmer should be responsible for this).
- NVMalloc, NVRealloc and NVFree is not thread safe. 
- NVSet can not span one or more NVRegion

#### File Manifest
- Makefile
   - Makefile-test
   - Makefile-example1
   - Makefile-example2
- test.c
- example1.c
- example2.c
- global.h
- memlib.h
- memlib.c
- mm.h
- mm.c
- GeneralHashFunctions.h
- GeneralHashFunctions.c
- mm.h
- mm.c
- nv_api.h
- nv_api.c

#### Implemented APIs(* means required)
- (*)NVOpenRegion (char * name, void * startingAddr, int size) // create a region with given size and name, at the given starting address.
- (*)NVCloseRegion (NVRDescr * addr) // close region at the given address
- (*)NVDeleteRegion (char * name) // delete region of the given name
- (*)NVFetchRoot (NVRDescr * addr, char * name) // fetch given name root at the given region address
- (*)NVNewRoot (NVRDescr * addr, void *p, char * name, size_t size) // create a new root with pointer p and its size as given name in given region
- (*)NVMalloc (NVRDescr * addr, int size) // malloc a block of given size in given region
- (*)NVFree (void * addr) // free the data object in given address
- NVRealloc (NVRDescr * addr, void * ptr, int size) // realloc the given data object with new size in the given region
- NVRDescrDump (NVRDescr *nvrAddr) // print out the NVRDescr info of given region
- NVRootmapDump (NVRDescr *nvrAddr) // print out the Rootmap of given region



#### Configuration Instruction
All the configuratoin settings are in the file global.h.
Those paramemter can be configurated:
- define SHM or MMAP // choose how to implement share memory
- define FIXMEMBASE // fixed startingAddr if defined 
- define DEBUG // debug info ouput if defined
- define NV_MAXPATH // maximum file name length
- define NVROOT_MAXPATH // maximum root name length
- define SHM_MODE // read write mode
- define SHM_SIZE // shared memory size
- define PAGESIZE // page size
- define membase // default starting address of shm


#### Installation Instruction
To install this nvm-simulator, you only need to copy out files according to the File Manifest to your working folder. Then you can write you own application using APIs. Remember to modify the makefiles to ensure compilation.

#### Operating Instruction
To write your own application using nvm-simulator, if the MMAP is defined, you do not need to create the backing file pre-hand. If you define the SHM, you do need to create the backing file pre-hand. The backing file should have the corresponding pathname as the input. Generally, the first operation should always be the NVOpenRegion. If the region exists, it will open it otherwise it will create it. After that, the data region will be initializaed, you can do operation in the data region by using the memory-realted APIs such as NVMalloc, NVFree and NVRealloc. If you want the data in the region is persistent, you can just close the region. If you want to destroy the data, you can use NVDeleteRegion to delete this region. The operation of NVRootmap is for the root in nvregion. You can use NVNewRoot to set new root. After that you can NVFetchRoot to fetch given name root. For better visulization , you can call NVRDescrDump and NVRootmapDump to print information. 

#### How to run Test
For example, to complie and run the test.c, you can do the following:
- >: make test
- >: ./test
- >: make clean-test
Then there will be output information in stdout.
Make will go into the Makefile then go into the Makefile-test

#### How to run Example
- >: make example1
- >: make example2
- >: ./example1
- >: ./example2
- >: make clean-example1
- >: make clean-example2

If you want to complie all at once. just run >: make. This will automatically compile all test and examples. To clean those files you can run >: make clean.


#### Credits
- CSAPP course project malloc lab implementation.
- General Purpose Hash Function Algorithms Library by Arash Partow

#### Copyright (BSD-style)

Copyright (c) 2013 Shanhe Yi

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

