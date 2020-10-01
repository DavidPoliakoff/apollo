
// Copyright (c) 2020, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory
//
// This file is part of Apollo.
// OCEC-17-092
// All rights reserved.
//
// Apollo is currently developed by Chad Wood, wood67@llnl.gov, with the help
// of many collaborators.
//
// Apollo was originally created by David Beckingsale, david@llnl.gov
//
// For details, see https://github.com/LLNL/apollo.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef APOLLO_ENV_H
#define APOLLO_ENV_H


#include "omp.h"


namespace Apollo
{
class Env
{
    public:

        enum class Name {
            UNKNOWN, LSF, SLURM
        };

        void    clear(void);
        Name    detect(void);
        void    load(Name from_env);
        bool    validate(void);
        bool    refresh(void);

        // ---

        bool loaded;
        Name name;

        int numNodes;
        int numCPUsOnNode;
        int numGPUsOnNode;

        int numProcs;
        int numProcsPerNode;

        int numThreadsPerCPUCap;

        omp_sched_t ompDefaultSchedule;
        int         ompDefaultNumThreads;
        int         ompDefaultChunkSize;
        //
        int mpiSize;   // 1 if no MPI
        int mpiRank;   // 0 if no MPI

}; //end: Env (class)

}; //end: Apollo (namespace)


#endif
