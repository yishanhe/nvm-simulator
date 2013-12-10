nvm-simulator
=============

cs652 course project

Simulate a non-volative memory using a chunk of main memory.
Generally, process never share resourcces unless the shared memory. 
This project is using this mechanism to implement a NVM simulator.


#### Project Requirements
- NVRegion related APIs
- DataRegion realted APIs



#### Assumptions, requirements and limitations
- System V Shared Memory support
- All NVRegions are attached at the same starting address of the process address space.
- The process won't crash and there is no power outage.
- Currently, there is only file-level lock granularity (The programmer should be responsible for this).
- NVMalloc and NVFree is not thread safe. 


#### Credits
- CSAPP course project malloc lab implementation.
- General Purpose Hash Function Algorithms Library by Arash Partow





### Copyright (BSD-style)

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

