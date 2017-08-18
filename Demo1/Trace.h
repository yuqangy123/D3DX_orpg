#pragma once
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include "windows.h"

//调试开关, 是否显示控制台调试窗口
#define _TRACE_BUG


class CMyTrace
{
public:
	CMyTrace (){noisy=0;f=stdout;}
	CMyTrace (const char *cfilename, const char *cflag){noisy=0;if(NULL != (f = fopen(cfilename,cflag)))noisy=1;}
	~CMyTrace(){if(f)fclose(f);}
	void print(const char *s){if(noisy && s){fprintf(f,"%s",s);fflush(f);};}
	void on(){noisy=1;}
	void off(){noisy=0;}
private:
	int noisy;
	FILE *f;
};

void debug_stdcout();//调用该函数本进程将关联一个控制台调试窗口，用std::cout/printf输出调试信息
void close_debug_stdcout();

#ifdef _TRACE_BUG
	#define debug(msg, ...)printf(msg,__VA_ARGS__)
#else 
	#define debug(msg, ...) 
#endif