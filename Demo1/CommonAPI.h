#pragma once
#include "atlstr.h"//CString
#include <process.h>
#include "base_type.h"

#define DeleteAndNull(p) if (p){delete p; p = NULL;}
#define DeleteArrayAndNull(p) if (p){delete []p; p = NULL;}
#define FreeAndNull(p) if (p){free(p); p = NULL;}
#define DeleteObjectAndNull(p) if (p){DeleteObject(p); p = 0;}
#define NewAndReturn(p, t) if (!p) p = new t; return p;
#define ARRSIZE(x)	(sizeof(x)/sizeof(x[0]))
#define SafeRelease(x) {if(x)x->Release();x=NULL;}

#if defined(WIN32)
typedef unsigned( __stdcall *PTHREAD_START) (void*);

#define chBEGINTHREADEX( psa, cbStack, pfnStartAddr,\
	pvParam, fdwCreate, pdwThreadID) \
	( (HANDLE) _beginthreadex(       \
	(void*) (psa),           \
	(unsigned) (cbStack),    \
	(PTHREAD_START) (pfnStartAddr),\
	(void*) (pvParam),       \
	(unsigned) (fdwCreate),  \
	(unsigned*) (pdwThreadID)) )
#else
#endif

/*
bool Is_Unicode_()
{
#if defined(UNICODE) || defined(_UNICODE)
	return true;
#else
	return false;
#endif
	return false;
}
*/