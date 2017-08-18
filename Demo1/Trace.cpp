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


//setvbuf����stream����ػ�����,�������Ӧ���ڴ�����,��������,���κζԸ������������ǰ,_IOFBF������,_IOLBF�л���,_IONBF�޻���
// _fdopenΪָ����������һ��stream
//_open_osfhandle��file handle����ļ�������
//GetStdHandle��ñ�׼�豸(input,output,error)
//AllocConsole���msdn
//AttachConsole������һ������̨