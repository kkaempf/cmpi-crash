/*****************************************************************************
* Copyright (C) 2008 Novell Inc. All rights reserved.
* Copyright (C) 2008 SUSE Linux Products GmbH. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 
*   - Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
* 
*   - Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
* 
*   - Neither the name of Novell Inc. nor of SUSE Linux Products GmbH nor the
*     names of its contributors may be used to endorse or promote products
*     derived from this software without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL Novell Inc. OR SUSE Linux Products GmbH OR
* THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <pthread.h>

/* Include the required CMPI macros, data types, and API function headers */
#include <cmpi/cmpidt.h>
#include <cmpi/cmpift.h>
#include <cmpi/cmpimacs.h>

// Needed to obtain errno of failed system calls
#include <errno.h>

/* Needed for kill() */
#include <signal.h>

static int cmpi_crash_trace_level = 42; /* increase by setenv CMPI_CRASH_TRACE_LEVEL */

/* A simple stderr logging/tracing facility. */
#ifndef _SBLIM_TRACE
#define _SBLIM_TRACE(tracelevel,args) if (tracelevel <= cmpi_crash_trace_level) { _logstderr args; }
static void _logstderr(char *fmt,...)
{
   va_list ap;
   va_start(ap,fmt);
   vfprintf(stderr,fmt,ap);
   va_end(ap);
   fprintf(stderr,"\n");
}
#endif

#define _CMPI_SETFAIL(msgstr) {if (st != NULL) st->rc = CMPI_RC_ERR_FAILED; st->msg = msgstr; }

/***************************************************************************/

/*
 * InstCleanup
 */

static CMPIStatus
InstCleanup(CMPIInstanceMI * self,      
        const CMPIContext * context,
        CMPIBoolean terminating)
{
    CMPIStatus st = { CMPI_RC_OK, NULL };
    _SBLIM_TRACE(1,("Cleanup() called for crash provider"));
    return st;
}

/*
 * EnumInstanceNames() - return a list of all the instances names (i.e. return their object paths only)
 */
static CMPIStatus
EnumInstanceNames(CMPIInstanceMI * self,      
        const CMPIContext * context,
        const CMPIResult * result,
        const CMPIObjectPath * reference)
{
    CMPIStatus status = {CMPI_RC_OK, NULL};

    _SBLIM_TRACE(1,("EnumInstancesNames() called for crash provider"));

    abort();

    return status;
}


static CMPIInstanceMIFT InstanceMIFT__={ 
    CMPICurrentVersion, 
    CMPICurrentVersion, 
    "instanceCmpi_Crash",  // miName
    InstCleanup, 
    EnumInstanceNames,
    NULL, 
    NULL, 
    NULL, 
    NULL, 
    NULL, 
    NULL, 
}; 


#define SWIG_CMPI_MI_FACTORY(ptype) \
CMPI##ptype##MI* _Generic_Create_##ptype##MI(const CMPIBroker* broker, \
        const CMPIContext* context, const char* miname, CMPIStatus* st)\
{ \
    CMPI##ptype##MI *mi; \
    _SBLIM_TRACE(1, (">>>>> in FACTORY: CMPI"#ptype"MI* _Generic_Create_"#ptype"MI... miname=%s", miname)); \
    mi = (CMPI##ptype##MI*)malloc(sizeof(CMPI##ptype##MI)); \
    if (mi) { \
        mi->hdl = NULL; \
        mi->ft = &ptype##MIFT__; \
    } \
    _SBLIM_TRACE(1, (">>>>> CMPI"#ptype"MI(%s)", miname)); \
    return mi; \
}

SWIG_CMPI_MI_FACTORY(Instance)

#undef _CMPI_SETFAIL
