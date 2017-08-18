#pragma once
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include "windows.h"

//���Կ���, �Ƿ���ʾ����̨���Դ���
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

void debug_stdcout();//���øú��������̽�����һ������̨���Դ��ڣ���std::cout/printf���������Ϣ
void close_debug_stdcout();

#ifdef _TRACE_BUG
	#define debug(msg, ...)printf(msg,__VA_ARGS__)
#else 
	#define debug(msg, ...) 
#endif