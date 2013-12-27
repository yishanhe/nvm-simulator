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
- (*)NVOpenRegion ()
- (*)NVCloseRegion ()
- (*)NVDeleteRegion ()
- (*)NVFetchRoot ()
- (*)NVNewRoot ()
- (*)NVMalloc ()
- (*)NVFree ()
- NVRealloc ()
- NVRDescrDump ()
- NVRootmapDump ()



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

