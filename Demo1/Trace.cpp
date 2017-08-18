#include "StdAfx.h"
#include "Trace.h"

void debug_stdcout()
{

#ifdef _TRACE_BUG
	{
		static int use = 0;
		if (use)
			return;

		AllocConsole();
		int hCrun = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
		FILE *file  = _fdopen(hCrun, "w");  

		// use default stream buffer   
		setvbuf(file, NULL, _IONBF, 0);  
		*stdout = *file; 

		use = 1;
	}

#else 
	{
		return;
	}

#endif
}

void close_debug_stdcout()
{
#ifdef _TRACE_BUG
	FreeConsole();
#endif
}


//setvbuf设置stream的相关缓冲区,这个函数应该在打开流后,立即调用,在任何对该流做输入输出前,_IOFBF满缓冲,_IOLBF行缓冲,_IONBF无缓冲
// _fdopen为指定描述符打开一个stream
//_open_osfhandle从file handle获得文件描述符
//GetStdHandle获得标准设备(input,output,error)
//AllocConsole详见msdn
//AttachConsole关联另一个控制台