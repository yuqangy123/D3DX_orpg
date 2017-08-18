// Demo1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DemoGE.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CDemoGE GE;
	if(!FAILED(GE.system_init(TRUE, 800, 600)))
		GE.system_run();
	else
		printf("system_init failed\r\n");
	return 0;
}

