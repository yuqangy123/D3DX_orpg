#pragma once

//#include "shlobj.h"
//#include "CBaseDataStructure.h"
#include "common1.h"


//#pragma comment(lib, "Version.lib")

#define QUICK_MODE		1000
#define CUSTOM_MODE		2000
#define OVERALL_MODE	3000


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


class CYFile
{
public:
	static CYFile* Instance(){NewAndReturn(m_pIns,CYFile)};
	static void CloseInstance(){DeleteAndNull(m_pIns)};
//	bool Init();

// 	//扫描文件
// 	bool ScanFileEx(const CString &csFile, std::vector<CString> &Filelist, DWORD mode);
// 
// 	//设置指定扫描目录
// 	void SetCustomScanPath(const CString &cspath);
// 
// 	//扫描文件夹, 支持文件名含通配符
// 	int ScanFolder(const CString &csFoldername, const CString &csSite, std::vector<CString> &folderlist, DWORD ndepth);
// 
// 	//同上, 扫描单个文件夹
// 	CString ScanFolder(const CString &csFoldername, const CString &csSite, DWORD ndepth);
// 
// 	//扫描文件，文件名支持通配符，sift：不用扫描的文件夹
// 	BOOL ScanFile( const CString &csDir, const wchar_t *szFile, const std::vector<CString> &sift, std::vector<CString> &Filelist);
// 
// 	//无限制，单文件
// 	CString ScanFile( const wchar_t *szFile, const CString &csDir );
// 
// 	//支持通配符字符对比
// 	bool wildcard(const wchar_t *pat,const wchar_t *src);
// 
// 	//获得快捷方式的源路径
// 	BOOL Lnk2Path(const CString &cslnkfile, CString &csRespath);
// 
// 	//快速扫描
// 	bool QuickMode(const wchar_t *file, const std::vector<CString> &sift, std::vector<CString> &Filelist);	
// 
// 	//自定义扫描
// 	bool CustomMode(const wchar_t *file, const std::vector<CString> &sift, std::vector<CString> &Filelist);	
// 
// 	//全盘扫描
// 	bool OverallMode(const wchar_t *file, const std::vector<CString> &sift, std::vector<CString> &Filelist);	
// 
// 	//目录是否存在
// 	bool DirectoryExists(const char_t *ApFileName);
// 
// 	//	获取文件目录，最后不包含 "\"
// 	void GetParentDir(LPCTSTR sFileName, CString &csDir);
// 
// 	//获得文件名
// 	void GetFileName( LPCTSTR sFileName, CString &csFile);
// 
// 	//  获取系统相关目录
// 	string_t GetSpecialFolderPath( int nFolder );
// 
// 	//文件大小
// 	INT64 GetFileSizeEx_(LPCTSTR lpszFileName);
// 	
// 	//文件是否存在
// 	bool CYFile::FileExists(const char_t *ApFileName);
// 
 	/*获得文件的路径,如: E:\VCProjects\FileOperation\1\Debug\1.bsc => E:\VCProjects\FileOperation\1\Debug\ */
	string_t ExtractFilePath(const string_t &AFileName);
// 
// 	/*获得文件扩展名*/
// 	string_t ExtractFileExt(const string_t &AFileName);
// 
// 	/*强制创建目录,一层层创建*/
// 	bool ForceDirectories(string_t &Dir);
// 
// 	//删除目录及其所有文件
// 	BOOL DeleteDirectiories(const string_t &DelPath);
// 
// 	/*得到系统目录*/
// 	string_t GetSysPath();
// 
	/*当前模块所在文件夹*/
	string_t GetAppFilePath(HMODULE hModule = NULL);

	/*当前模块名称*/
	string_t GetAppFileName(HMODULE hModule = NULL);

// 	/*由相对路径得到绝对路径*/
// 	string_t GetAbsFilePath(const string_t &strFullPath, const string_t &strPath);
// 
// 	/*系统用户路径*/
// 	string_t GetUserPath(void);
// 
// 	/*获取当前工作目录*/
// 	string_t GetCurWorkDir();
// 
// 	//创建目录
// 	bool CreateOpenDir(const string_t& Path);
// 
// 	//  获取文件版本
// 	UINT64 GetFileVer(LPCTSTR sFileName);
// 	WORD VerToWord(UINT64 uVer);
// 	UINT64 StrToVer(LPCTSTR sVer);
// 	CString VerToStr(UINT64 uVer);
// 
private:
	CYFile();
	~CYFile(void);
// 	BOOL GetSysEnvDir();//绝对路径后面没有跟反斜杠
 	string_t Getwstring(const string_t &s, const string_t &Sing, bool Forwar = true);
// 	string_t ExcludeTrailingPathDelimiter(const string_t &s);//如果文件最后一个字符为: \,则将它去掉后返回,否则返回原字符串
 	static CYFile *m_pIns;

// 	CAutoCSec	g_Lock;	// 临界区对象	
//  	volatile BOOL m_isStop;
// 
// 	map_t<string_t, string_t>g_SysEnvMap;			//系统环境列表
// 
// 	std::vector<CString> g_IgnoreDIR;							//忽略目录的列表
// 	std::vector<CString> g_CustomDIR;							//指定目录列表
};


