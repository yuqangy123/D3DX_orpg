#include "StdAfx.h"
#include "MyFile.h"
// #include "shellapi.h"
// #include "Sddl.h"
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


CYFile* CYFile::m_pIns=NULL;
CYFile::CYFile()
{
//	m_isStop = FALSE;
}


CYFile::~CYFile(void)
{
}
//
// CString CYFile::ScanFolder(const CString &csFoldername, const CString &csSite, DWORD ndepth)
// {
// 	CString csPath,csTmp;
// 	WIN32_FIND_DATA FindFileData;
// 	HANDLE hFind;
// 	std::queue<CString> dirQueue,dirDepthQueue;
// 	dirQueue.push(csSite);
// 
// 	while( ndepth )
// 	{
// 		while(dirQueue.size())
// 		{
// 			csPath = dirQueue.front();dirQueue.pop();
// 			csPath += L"\\*.*";
// 			hFind=::FindFirstFile(csPath, &FindFileData);
// 			if(INVALID_HANDLE_VALUE == hFind){
// 				FindClose(hFind);
// 				return CString(_T(""));
// 			}
// 
// 			do{
// 				if( m_isStop )
// 				{FindClose(hFind);return CString(_T(""));}
// 
// 				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
// 				{
// 					if(FindFileData.cFileName[0]!=L'.')
// 					{
// 						csTmp.Format(_T("%s\\%s"),csPath.Left(csPath.ReverseFind(L'\\')), FindFileData.cFileName);
// 						dirDepthQueue.push(csTmp);
// 						if(wildcard(csFoldername,FindFileData.cFileName))
// 						{
// 							FindClose(hFind);
// 							return csTmp;
// 						}
// 					}
// 				}
// 			}while(FindNextFile(hFind,&FindFileData));
// 			FindClose(hFind);
// 		}
// 		--ndepth;
// 		dirQueue = dirDepthQueue;
// 	}
// 
// 	return CString(_T(""));
// }
// 
// int CYFile::ScanFolder(const CString &csFoldername, const CString &csSite, std::vector<CString> &folderlist, DWORD ndepth)
// {
// 	folderlist.clear();
// 	CString csPath,csTmp;
// 	WIN32_FIND_DATA FindFileData;
// 	HANDLE hFind;
// 	std::queue<CString> dirQueue,dirDepthQueue;
// 	dirQueue.push(csSite);
// 
// 	while( ndepth )
// 	{
// 		while(dirQueue.size())
// 		{
// 			csPath = dirQueue.front();dirQueue.pop();
// 			csPath += L"\\*.*";
// 			hFind=::FindFirstFile(csPath, &FindFileData);
// 			if(INVALID_HANDLE_VALUE == hFind){
// 				FindClose(hFind);
// 				return -1;
// 			}
// 
// 			do{
// 				if( m_isStop )
// 					{FindClose(hFind);return -1;}
// 
// 				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
// 				{
// 					if(FindFileData.cFileName[0]!=L'.')
// 					{
// 						csTmp.Format(_T("%s\\%s"),csPath.Left(csPath.ReverseFind(L'\\')), FindFileData.cFileName);
// 						dirDepthQueue.push(csTmp);
// 						if(wildcard(csFoldername,FindFileData.cFileName))
// 							folderlist.push_back(csTmp);
// 					}
// 				}
// 			}while(FindNextFile(hFind,&FindFileData));
// 			FindClose(hFind);
// 		}
// 		--ndepth;
// 		dirQueue = dirDepthQueue;
// 	}
// 
// 	return 1;
// }
// 
// bool CYFile::Init()
// {
// 	return GetSysEnvDir();
// }
// 
// void CYFile::GetParentDir(LPCTSTR sFileName, CString &csDir)
// {
// 	csDir = sFileName;
// 	csDir = csDir.Left(csDir.ReverseFind(_T('\\')));
// }
// 
// string_t CYFile::GetSpecialFolderPath( int nFolder )
// {
// 	char_t Buf[MAX_PATH] = {0};
// 
// 	if (SHGetSpecialFolderPath(0, Buf, nFolder, FALSE))
// 		return string_t(Buf);
// 
// 	return string_t(_T(""));
// }
// 
// //绝对路径后面没有跟反斜杠
// BOOL CYFile::GetSysEnvDir()
// {
// 	if (g_SysEnvMap.size())return TRUE;
// 
// 	string_t strDir;
// 	wchar_t *pDir = NULL;
// 	CString csDirectory;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_ADMINTOOLS);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("AdminTools")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_APPDATA);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("userappdata")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_COMMON_APPDATA);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("commonuserappdata")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_COMMON_DESKTOPDIRECTORY);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("commonuserdesktop")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_COMMON_DOCUMENTS);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("commondocs")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_COMMON_FAVORITES);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("commonfavorites")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_COMMON_PROGRAMS);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("commonprograms")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_COMMON_STARTMENU);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("commonstartmenu")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_COMMON_STARTUP);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("commonstartup")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_COMMON_TEMPLATES);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("commontemplates")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_COOKIES);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("usercookies")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_DESKTOPDIRECTORY);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("userdesktop")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_FAVORITES);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("userfavorites")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_HISTORY);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("commonhistory")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_LOCAL_APPDATA);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("userlocalappdata")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_PROGRAM_FILES);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("programfiles")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_PROGRAMS);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("startprograms")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_STARTMENU);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("userstartmenu")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_STARTUP);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("userstartup")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_SYSTEM);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("system32")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_TEMPLATES);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("usertemplates")] = strDir;
// 
// 	strDir = GetSpecialFolderPath(CSIDL_WINDOWS);
// 	if (strDir.size())
// 		g_SysEnvMap[_T("windows")] = strDir;
// 
// 	return TRUE;
// }
// 
// void CYFile::GetFileName( LPCTSTR sFileName, CString &csFile)
// {
// 	csFile = sFileName;
// 	csFile = csFile.Mid(csFile.ReverseFind(_T('\\')) + 1);
// }
// 
// bool CYFile::ScanFileEx(const CString &csFile, std::vector<CString> &Filelist, DWORD mode)
// {
// 	std::vector<CString> sift;
// 	sift.clear();
// 	switch (mode)
// 	{
// 	case QUICK_MODE:
// 		if(false == QuickMode(csFile,sift, Filelist))
// 			return false;
// 		break;
// 
// 	case CUSTOM_MODE:
// 		if(false == CustomMode(csFile, sift, Filelist))
// 			return false;
// 		break;
// 
// 	case OVERALL_MODE:
// 		if(false == OverallMode(csFile, g_IgnoreDIR, Filelist))
// 			return false;
// 		break;
// 
// 	default:return false;
// 	}
// 
// 	return true;
// }
// 
// //GetLogicalDrive函数返回一个DWORD类型的值,第一位表示所对应的驱动器是否存在.一般情况下DWORD的数据长度是32位,在这个DWORD中,每一位对应了逻辑驱动器是否存在.第一位如果是 1 则表示逻辑驱动器 A 存在,第二位如果是 1 则表示 B 存在,第四位如果是 1 则表示 D 存在,以此类推.
// bool CYFile::OverallMode(const wchar_t *file, const std::vector<CString> &sift, std::vector<CString> &Filelist)
// {
// 	if(NULL == file)return false;
// 	std::wstring strsys = g_SysEnvMap[L"windows"];
// 	wchar_t sysChar = strsys[0];
// 	DWORD nDrives;
// 	INT iIndex = 1;
// 	nDrives = GetLogicalDrives();
// 	CString cstr;
// 	wchar_t cChar = L'A';
// 	std::vector<CString> emptyvtr;
// 
// 	for (int i=0;i<32;++i)
// 	{
// 		if(nDrives & iIndex){
// 			cstr.Format(L"%c:",cChar+i);
// 			if(sysChar == cChar+i){
// 				if(FALSE == ScanFile(cstr, file, sift, Filelist))
// 					return FALSE;
// 			}
// 			else{
// 				emptyvtr.clear();
// 				emptyvtr.push_back(cstr+L"\\$RECYCLE.BIN");
// 				if(FALSE == ScanFile(cstr, file, emptyvtr, Filelist))
// 					return FALSE;
// 			}
// 		}
// 		iIndex = iIndex<<1;
// 	}
// 	return true;
// }
// 
// void CYFile::SetCustomScanPath(const CString &cspath)
// {
// 	g_CustomDIR.push_back(cspath);
// }
// 
// bool CYFile::CustomMode(const wchar_t *file, const std::vector<CString> &sift, std::vector<CString> &Filelist)
// {
// 	Filelist.clear();
// 	for (std::vector<CString>::const_iterator itr = g_CustomDIR.begin();itr != g_CustomDIR.end();++itr)
// 		ScanFile(*itr, file, sift, Filelist);
// 
// 	if (Filelist.size())
// 		return true;
// 	return false;
// }
// 
// bool CYFile::QuickMode(const wchar_t *file, const std::vector<CString> &sift, std::vector<CString> &Filelist)
// {
// 	std::wstring Env_DIR = g_SysEnvMap[_T("commonuserdesktop")];
// 	ScanFile(Env_DIR.c_str(), file, sift, Filelist);
// 
// 	Env_DIR = g_SysEnvMap[_T("userdesktop")];
// 	ScanFile(Env_DIR.c_str(), file, sift, Filelist);
// 
// 	Env_DIR = g_SysEnvMap[_T("commonprograms")];
// 	ScanFile(Env_DIR.c_str(), file, sift, Filelist);
// 
// 	if (Filelist.size())
// 		return true;
// 	return false;
// }
// 
// INT64 CYFile::GetFileSizeEx_(LPCTSTR lpszFileName)
// {
// 	WIN32_FIND_DATA data;
// 	HANDLE h = FindFirstFile(lpszFileName, &data);
// 	if (h != INVALID_HANDLE_VALUE)
// 	{
// 		FindClose(h);
// 		return data.nFileSizeLow | (INT64(data.nFileSizeHigh)<<32);
// 	}
// 	else
// 		return -1;
// }
// 
// BOOL CYFile::Lnk2Path(const CString &cslnkfile, CString &csRespath)
// {
// 	HRESULT hres ;
// 	IShellLink  * PShLink ;
// 	WIN32_FIND_DATA wfd ;
// 
// 	TCHAR* lpszPath = new TCHAR[1024];
// 	memset(lpszPath, 0x0, 1024 * sizeof(TCHAR));
// 	CString path;
// 	hres = CoInitialize( NULL ) ;
// 	if ( SUCCEEDED(hres) )
// 	{
// 		hres = CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&PShLink ) ;
// 
// 		if ( SUCCEEDED(hres) )
// 		{
// 			IPersistFile * ppf ;
// 
// 			hres = PShLink->QueryInterface(  IID_IPersistFile, (LPVOID*)&ppf  );
// 			if ( SUCCEEDED(hres) )
// 			{
// 				hres = ppf->Load( cslnkfile, STGM_READ ) ;
// 				if ( SUCCEEDED(hres) )
// 				{
// 					wchar_t szWorkDir[MAX_PATH]={0};
// 					PShLink->GetWorkingDirectory(szWorkDir,MAX_PATH);
// 					hres = PShLink->GetPath( lpszPath, 1024, &wfd, SLGP_RAWPATH ) ;
// 					if( SUCCEEDED(hres) )
// 					{
// 						/*test code
// 						path = lpszPath;
// 
// 						if(-1 == path.Find(szWorkDir))
// 						{
// 							CString csLauncher = CGameInfoManager::Instance()->GetGameInfo(m_gid).launcher;//只扫描游戏派范围内的游戏
// 							if(wcslen(szWorkDir) && L'\\' == szWorkDir[wcslen(szWorkDir)-1])
// 								path.Format(L"%s%s",szWorkDir,csLauncher.Right(csLauncher.GetLength()-csLauncher.ReverseFind(L'\\')-1));
// 							else
// 								path.Format(L"%s\\%s",szWorkDir,csLauncher.Right(csLauncher.GetLength()-csLauncher.ReverseFind(L'\\')-1));
// 						}
// 						*/
// 					}
// 					csRespath = path;
// 				}
// 				ppf->Release( ) ;
// 			}
// 			PShLink->Release(  ) ;
// 		}
// 		CoUninitialize() ;
// 	}
// 	delete[] lpszPath;
// 	return TRUE;
// // 
// // 	// 	FILE *fp=fopen(cslnkfile,"rb");
// // 	// 	if(!fp)
// // 	// 		return FALSE;
// // 	// 
// // 	// 	char filepath[MAX_PATH]={0};
// // 	// 	unsigned int flag,off;
// // 	// 	unsigned short int sflag;
// // 	// 	char uch;
// // 	// 	int i=0;
// // 	// 
// // 	// 
// // 	// 	fread(&flag,4,1,fp);
// // 	// 	if (flag!=0x4c)//不是Lnk文件
// // 	// 	{
// // 	// 		fclose(fp);
// // 	// 		return FALSE;
// // 	// 	}
// // 	// 	fseek(fp,0x14,SEEK_SET);
// // 	// 	fread(&flag,4,1,fp);
// // 	// 	fseek(fp,0x4c,SEEK_SET);
// // 	// 
// // 	// 	if(flag & 0x01)
// // 	// 	{
// // 	// 		fread(&sflag,2,1,fp);
// // 	// 		fseek(fp,sflag,SEEK_CUR);
// // 	// 	}
// // 	// 
// // 	// 	off = ftell(fp);
// // 	// 	fseek(fp,off+0x10,SEEK_SET);
// // 	// 	fread(&flag,4,1,fp);
// // 	// 	fseek(fp,flag+off,SEEK_SET);
// // 	// 
// // 	// 	do
// // 	// 	{
// // 	// 		fread(&uch,1,1,fp);
// // 	// 		filepath[i++]=uch;
// // 	// 	}
// // 	// 	while (uch!='\0' && i < MAX_PATH-1);
// // 	// 	filepath[i]=0;
// // 	// 	fclose(fp);
// // 	// 	csRespath = filepath;
// // 	// 	return TRUE;
// // 
//  }
// 
// 
// bool CYFile::wildcard(const wchar_t *pat,const wchar_t *src)
// {
// 	while(*pat && *src)
// 	{
// 		switch(*pat)
// 		{
// 		case L'*':{
// 			while(L'*' == *pat || L'?' == *pat)++pat;
// 			while(*src){
// 				if(wildcard(pat,src))return true;
// 				++src;
// 			}
// 				  }break;
// 
// 		case L'?':{
// 			if(*(++pat) == *src)continue;
// 			++src;
// 				  }break;
// 		default:{
// 			if(*pat != *src)return false;
// 			++pat;++src; 
// 				}break;
// 		}
// 	}
// 
// 	while(L'*' == *pat || L'?' == *pat )++pat;
// 	return !(*pat) && !(*src);
// }
// 
// CString CYFile::ScanFile( const wchar_t *szFile, const CString &csDir)
// {
// 	if(NULL == szFile)	return CString(_T(""));
// 
// 	//非递归
// 	CString csPath,csTmp;
// 	WIN32_FIND_DATA FindFileData;
// 	HANDLE hFind;
// 	std::queue<CString> dirQueue;
// 
// 	dirQueue.push(csDir);
// 	while(!dirQueue.empty())
// 	{
// 		csPath = dirQueue.front();dirQueue.pop();
// 		csTmp = csPath;
// 		csPath += L"\\*.*";
// 		hFind=::FindFirstFile(csPath, &FindFileData);
// 		csPath = csTmp;
// 
// 		if(INVALID_HANDLE_VALUE == hFind){
// 			FindClose(hFind);
// 			csTmp.Format(L"%s,%d",csPath,GetLastError());
// 			continue;
// 		}
// 
// 		do{
// 			if( TRUE == m_isStop ){FindClose(hFind);return CString(_T(""));}
// 
// 			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
// 			{
// 				if(FindFileData.cFileName[0]!=L'.')
// 				{
// 					csTmp.Format(L"%s\\%s",csPath,FindFileData.cFileName);
// 					dirQueue.push(csTmp);
// 				}
// 			}
// 			else
// 			{
// 				if(wildcard(szFile,FindFileData.cFileName)){
// 					csTmp.Format(L"%s\\%s",csPath,FindFileData.cFileName);
// 					FindClose(hFind);
// 					return csTmp;
// 				}
// 			}
// 		}while(FindNextFile(hFind,&FindFileData));
// 		FindClose(hFind);
// 	}
// 
// 	return CString("");
// }
// 
// BOOL CYFile::ScanFile( const CString &csDir, const wchar_t *szFile, const std::vector<CString> &sift, std::vector<CString> &Filelist)
// {
// 	if(NULL == szFile)	return FALSE;
// 
// 	//非递归
// 	CString csPath,csTmp;
// 	WIN32_FIND_DATA FindFileData;
// 	HANDLE hFind;
// 	std::queue<CString> dirQueue;
// 	enum{NOSIFT,SIFTDIR} sifttype;
// 	std::vector<CString>::const_iterator itr;
// 	if(0 == sift.size())
// 		sifttype = NOSIFT;
// 	else
// 		sifttype = SIFTDIR;
// 
// 	dirQueue.push(csDir);
// 	while(!dirQueue.empty())
// 	{
// 		csPath = dirQueue.front();dirQueue.pop();
// 		csTmp = csPath;
// 		csPath += L"\\*.*";
// 		hFind=::FindFirstFile(csPath, &FindFileData);
// 		csPath = csTmp;
// 
// 		if(INVALID_HANDLE_VALUE == hFind){
// 			FindClose(hFind);
// 			csTmp.Format(L"%s,%d",csPath,GetLastError());
// 			continue;
// 		}
// 
// 		do{
// 			if(TRUE == m_isStop ){FindClose(hFind);return FALSE;}
// 
// 			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
// 			{
// 				if(FindFileData.cFileName[0]!=L'.')
// 				{
// 					csTmp.Format(L"%s\\%s",csPath,FindFileData.cFileName);
// 					switch(sifttype)
// 					{
// 					case NOSIFT:
// 						dirQueue.push(csTmp);
// 						break;
// 
// 					case SIFTDIR:{
// 						for(itr = sift.begin();itr != sift.end();++itr)
// 							if(0 == itr->CompareNoCase(csTmp))
// 								goto CONTINUE;
// 
// 						dirQueue.push(csTmp);
// 								 }
// 					}
// 				}
// 			}
// 			else
// 			{
// 				if(wildcard(szFile,FindFileData.cFileName)){
// 					csTmp.Format(L"%s\\%s",csPath,FindFileData.cFileName);
// 					Filelist.push_back(csTmp);
// 				}
// 			};
// CONTINUE:continue;
// 		}while(FindNextFile(hFind,&FindFileData));
// 		FindClose(hFind);
// 	}
// 	return TRUE;
// }
// 
// 
// UINT64 CYFile::GetFileVer( LPCTSTR sFileName )
// {
// 	UINT64 r = 0;
// 	DWORD dwVerHnd = 0;
// 	DWORD dwVerInfoSize = GetFileVersionInfoSize(sFileName, &dwVerHnd);   
// 	if(dwVerInfoSize)   
// 	{   
// 		LPBYTE lpvMem = new BYTE[dwVerInfoSize];   
// 		memset(lpvMem, 0, dwVerInfoSize);   
// 		GetFileVersionInfo(sFileName, dwVerHnd, dwVerInfoSize, lpvMem);    
// 		LPVOID lszVer = NULL;   
// 		UINT  cchVer = 0;   
// 		if(VerQueryValue(lpvMem, _T("\\"), &lszVer, &cchVer))   
// 		{   
// 			VS_FIXEDFILEINFO *pVerInfo = (VS_FIXEDFILEINFO*)lszVer;   
// 			r = (UINT64)pVerInfo->dwFileVersionMS << 32 | pVerInfo->dwFileVersionLS;
// 		}
// 		delete [] lpvMem;
// 	}
// 	return r;
// }
// 
// WORD CYFile::VerToWord( u_int64 uVer )
// {
// 	return (uVer>>48)%10*1000 + (uVer>>32&0xFFFF)%10*100 + (uVer>>16&0xFFFF)%10 * 10 + (uVer&0xFFFF)%10;
// }
// 
// UINT64 CYFile::StrToVer( LPCTSTR sVer )
// {
// 	CString s = sVer;
// 	UINT64 r = 0;
// 	int k = 0;
// 	for (int i = 0; i < 4; i++)
// 		if (k >= 0)
// 			r = r << 16 | _ttoi(s.Tokenize(_T("."), k));
// 		else
// 			r = r << 16;
// 	return r;
// }
// 
// CString CYFile::VerToStr(u_int64 uVer)
// {
// 	CString s;
// 	if ((uVer&0xFFFFFFFF) == 0)
// 		s.Format(_T("%d.%d"), (u_int16)(uVer>>48), (u_int16)(uVer>>32&0xFFFF));
// 	else if ((uVer&0xFFFF) == 0)
// 		s.Format(_T("%d.%d.%d"), (u_int16)(uVer>>48), (u_int16)(uVer>>32&0xFFFF), (u_int16)(uVer>>16&0xFFFF));
// 	else
// 		s.Format(_T("%d.%d.%d.%d"), (u_int16)(uVer>>48), (u_int16)(uVer>>32&0xFFFF), (u_int16)(uVer>>16&0xFFFF), (u_int16)(uVer&0xFFFF));
// 	return s;
// }
// 
// bool CYFile::FileExists(const char_t *ApFileName)
// {
// 	if(!ApFileName)
// 		return false;
// 	DWORD code = GetFileAttributes( ApFileName );
// 	return ((-1 != code) && !(FILE_ATTRIBUTE_DIRECTORY & code));
// }
// 
// bool CYFile::DirectoryExists(const char_t *ApFileName)
// {
// 	if(!ApFileName)
// 		return false;
// 	DWORD code = GetFileAttributes( ApFileName );
// 	return ((-1 != code) && (FILE_ATTRIBUTE_DIRECTORY & code));
// }
// 
string_t CYFile::ExtractFilePath(const string_t &AFileName)
{
	return Getwstring(AFileName, _T("\\"), true);
}

string_t CYFile::Getwstring(const string_t &s, const string_t &Sing, bool Forwar /*= true*/)
{
	string_t::size_type pos = s.rfind(Sing);
	if (string_t::npos == pos)
	{
		return _T("");
	}
	else
	{
		if (Forwar)
			return s.substr(0, pos + 1);
		else
			return s.substr(pos + 1, s.length());
	}
}

// string_t CYFile::ExtractFileExt(const string_t &AFileName)
// {
// 	return Getwstring(AFileName, _T("."), false);
// }
// 
// 
// bool CYFile::ForceDirectories(string_t &Dir)
// {
// 	if ( Dir == _T("") )
// 		return false;
// 	
// 	Dir = ExcludeTrailingPathDelimiter(Dir);
// 	if ((Dir.length() < 3) || DirectoryExists(Dir.c_str()) || 
// 		ExtractFilePath(Dir) == Dir)
// 	{
// 		return true;
// 	}
// 	return ForceDirectories(ExtractFilePath(Dir)) && CreateDirectory(Dir.c_str(), NULL);
// }
// 
// 
// string_t CYFile::ExcludeTrailingPathDelimiter(const string_t &s)
// {
// 	if ( _T("\\") == s.substr(s.length() - 1, s.length()))
// 	{
// 		return string_t(s.substr(0, s.length() -1 ));
// 	}
// 	else
// 	{
// 		return s;
// 	}
// }
// 
// BOOL CYFile::DeleteDirectiories(const string_t &DelPath)
// {
// 	string_t Path = DelPath;
// 	if(0 == Path.length())
// 		return FALSE;
// 	
// 	SHFILEOPSTRUCT FileOp; 
// 	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));
// 
// 	FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SIMPLEPROGRESS |FOF_ALLOWUNDO /*FOF_SILENT*/; 
// 	FileOp.hNameMappings = NULL; 
// 	FileOp.hwnd = NULL; 
// 	FileOp.lpszProgressTitle = NULL; 
// 	int len = Path.length()+ 1;
// 
// 	char_t* lpszPath = new char_t[(len+1)*sizeof(char_t)];
// 	memset(lpszPath, 0, (len+1)*sizeof(char_t));
// 	memcpy(lpszPath, Path.c_str(), (len - 1) * sizeof(char_t));
// 	
// 	FileOp.pFrom = lpszPath; 
// 
// 	FileOp.pTo = NULL; 
// 	FileOp.wFunc = FO_DELETE; 
// 
// 	if(SHFileOperation(&FileOp))
// 	{
// 		delete[] lpszPath;
// 		return FALSE;
// 	}
// 	delete[] lpszPath;
// 	return TRUE;
// }
// 
// string_t CYFile::GetSysPath()
// {
// 	char_t tmpBuf[MAX_PATH];
// 	if (0 != GetSystemDirectory(tmpBuf, MAX_PATH))
// 	{
// 		return tmpBuf;
// 	}
// 	else
// 	{
// 		return _T("");
// 	}
// }
// 
// string_t CYFile::GetUserPath( void )
// {
// 	CString sUserPath;
// 	if ( !::SHGetSpecialFolderPath(NULL,sUserPath.GetBuffer(sUserPath.GetLength()),CSIDL_LOCAL_APPDATA,FALSE) )
// 	{
// 		//AfxGetMainWnd()->MessageBox( TEXT("GetUserPath failed"), TEXT("error"), MB_OK );
// 		//::ShGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, )
// 		return GetAppFilePath().c_str();
// 	}
// 
// 	string_t str(sUserPath.GetBuffer());
// 	sUserPath.ReleaseBuffer();
// 	return str;
// }
//
string_t CYFile::GetAppFilePath( HMODULE hModule /*= NULL*/ )
{
	return ExtractFilePath( GetAppFileName(hModule) );
}

string_t CYFile::GetAppFileName( HMODULE hModule /*= NULL*/ )
{
	char_t strName[256] = {0};
	char_t strFullName[512] = {0};
	GetModuleFileName( hModule, strName, 256 );
	GetLongPathName( strName, strFullName, 256 );
	return strFullName;
}



// string_t CYFile::GetAbsFilePath(const string_t &strFullPath, const string_t &strPath)
// {
// 	/*
// 	//相对路径：..\img\logo.bmp
// 	//绝对路径：E:\VC\TerryLib\Sub
// 	*/
// 	if ( FileExists(strPath.c_str()) )
// 	{
// 		return strPath;
// 	}
// 	static const string_t CtSign( _T("..\\") );
// 	string_t strResult( ExcludeTrailingPathDelimiter(strFullPath) ), strTmp( strPath );
// 	while ( strTmp.substr(0, 3) == CtSign )
// 	{
// 		strTmp.erase( 0, 3 );
// 		string_t::size_type n = strResult.rfind( '\\' );
// 		if ( n != strResult.npos )
// 			strResult.erase( n, strResult.size() - n );
// 		else
// 			return _T("");
// 	}
// 	strResult += _T('\\') + strTmp;
// 	return strResult;
// }
// 
// string_t CYFile::GetCurWorkDir()
// {
// 	char_t buf[1024 * 2]  = {0};
// 	GetCurrentDirectory( 1024 * 2, buf );
// 	return string_t(buf);
// }
// 
// 
// bool CYFile::CreateOpenDir(const string_t& Path)
// {
// 	if (!Path.size())
// 		return false;
// 	
// 	SECURITY_ATTRIBUTES  sa;
// 	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
// 	sa.bInheritHandle = TRUE;
// 	TCHAR* szSD = _T("D:(A;OICI;GA;;;WD)");
// 	if(ConvertStringSecurityDescriptorToSecurityDescriptor( 
// 		szSD, 
// 		SDDL_REVISION_1, 
// 		&(sa.lpSecurityDescriptor), 
// 		NULL))
// 	{
// 		int res = SHCreateDirectoryEx(NULL, Path.c_str(), &sa);
// 		if(res == ERROR_SUCCESS || res == ERROR_FILE_EXISTS || res == ERROR_ALREADY_EXISTS)
// 			return true;
// 	}
// 	return false;
// }
// 
// #if 0
// bool CYFile::CreateOpenDir(const string_t& Path)
// {
// 	if(!string_t.size())
// 		return false;
// 
// 	SECURITY_ATTRIBUTES sa;   //和文件有关的安全结构
// 	SECURITY_DESCRIPTOR sd;   //声明一个SD
// 
// 	BYTE aclBuffer[1024];
// 	PACL pacl=(PACL)&aclBuffer; //声明一个ACL，长度是1024
// 
// 	BYTE sidBuffer[100];
// 	PSID psid=(PSID) &sidBuffer;   //声明一个SID，长度是100
// 
// 	DWORD sidBufferSize = 100;
// 	DWORD domainBufferSize = 80;
// 	HANDLE file;
// 
// 	//初始化一个SD
// 	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
// 	//初始化一个ACL
// 	InitializeAcl(pacl, 1024, ACL_REVISION);
// 
// 	PSID  pEveryoneSID = NULL;
// 	SID_IDENTIFIER_AUTHORITY SIDAuthWorld =
// 		SECURITY_WORLD_SID_AUTHORITY;
// 	// Create a well-known SID for the Everyone group.
// 	if(!AllocateAndInitializeSid(&SIDAuthWorld, 1,
// 		SECURITY_WORLD_RID,
// 		0, 0, 0, 0, 0, 0, 0,
// 		&pEveryoneSID))
// 	{
// 		printf("AllocateAndInitializeSid Error %u\n", GetLastError());
// 		if (pEveryoneSID) 
// 			FreeSid(pEveryoneSID);
// 		return;
// 	}
// 	//设置该用户的Access-Allowed的ACE，其权限为“所有权限”
// 	AddAccessAllowedAce(pacl, ACL_REVISION, GENERIC_ALL, pEveryoneSID);
// 	//把ACL设置到SD中
// 	SetSecurityDescriptorDacl(&sd, TRUE, pacl, FALSE);
// 
// 	//把SD放到文件安全结构SA中
// 	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
// 	sa.bInheritHandle = FALSE;
// 	sa.lpSecurityDescriptor = &sd;
// 
// 	//创建文件
// 	//file = CreateFile(_T("c:\\testfile"), 
// 	//	0, 0, &sa, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
// 	//CloseHandle(file);
// 	CreateDirectory(_T("C:\\Games\\"), NULL);		
// 
// 	if (pEveryoneSID) 
// 		FreeSid(pEveryoneSID);
// }
// #endif