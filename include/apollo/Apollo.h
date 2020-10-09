
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

#ifndef APOLLO_APOLLO_H
#define APOLLO_APOLLO_H

#ifndef APOLLO_MACROS_H
// Apollo header files should always begin with this, to provide
// universal override capability regardless of which header files
// a user includes, and which order they include them.
#include "apollo/Macros.h"
#endif

// NOTE: "apollo/Apollo.h" is a convenient single-include for all of Apollo.

#include "apollo/Config.h"
#include "apollo/Env.h"
#include "apollo/Exec.h"
#include "apollo/Logging.h"
#include "apollo/Util.h"
#include "apollo/Trace.h"
#include "apollo/Region.h"
#include "apollo/TimingModel.h"
#include "apollo/PolicyModel.h"
#include "apollo/Perf.h"

// So application codes can simply use:
//
//          Apollo_t *apollo = Apollo::instance();
//
typedef Apollo::Exec Apollo_t;

namespace Apollo
{

    // Return a pointer to the singleton instance of Apollo
    inline Apollo::Exec* instance(void) noexcept {
        return Apollo::Exec::instance();
    }

} //end: Apollo (namespace)
#endif
