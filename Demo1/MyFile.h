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

// 	//ɨ���ļ�
// 	bool ScanFileEx(const CString &csFile, std::vector<CString> &Filelist, DWORD mode);
// 
// 	//����ָ��ɨ��Ŀ¼
// 	void SetCustomScanPath(const CString &cspath);
// 
// 	//ɨ���ļ���, ֧���ļ�����ͨ���
// 	int ScanFolder(const CString &csFoldername, const CString &csSite, std::vector<CString> &folderlist, DWORD ndepth);
// 
// 	//ͬ��, ɨ�赥���ļ���
// 	CString ScanFolder(const CString &csFoldername, const CString &csSite, DWORD ndepth);
// 
// 	//ɨ���ļ����ļ���֧��ͨ�����sift������ɨ����ļ���
// 	BOOL ScanFile( const CString &csDir, const wchar_t *szFile, const std::vector<CString> &sift, std::vector<CString> &Filelist);
// 
// 	//�����ƣ����ļ�
// 	CString ScanFile( const wchar_t *szFile, const CString &csDir );
// 
// 	//֧��ͨ����ַ��Ա�
// 	bool wildcard(const wchar_t *pat,const wchar_t *src);
// 
// 	//��ÿ�ݷ�ʽ��Դ·��
// 	BOOL Lnk2Path(const CString &cslnkfile, CString &csRespath);
// 
// 	//����ɨ��
// 	bool QuickMode(const wchar_t *file, const std::vector<CString> &sift, std::vector<CString> &Filelist);	
// 
// 	//�Զ���ɨ��
// 	bool CustomMode(const wchar_t *file, const std::vector<CString> &sift, std::vector<CString> &Filelist);	
// 
// 	//ȫ��ɨ��
// 	bool OverallMode(const wchar_t *file, const std::vector<CString> &sift, std::vector<CString> &Filelist);	
// 
// 	//Ŀ¼�Ƿ����
// 	bool DirectoryExists(const char_t *ApFileName);
// 
// 	//	��ȡ�ļ�Ŀ¼����󲻰��� "\"
// 	void GetParentDir(LPCTSTR sFileName, CString &csDir);
// 
// 	//����ļ���
// 	void GetFileName( LPCTSTR sFileName, CString &csFile);
// 
// 	//  ��ȡϵͳ���Ŀ¼
// 	string_t GetSpecialFolderPath( int nFolder );
// 
// 	//�ļ���С
// 	INT64 GetFileSizeEx_(LPCTSTR lpszFileName);
// 	
// 	//�ļ��Ƿ����
// 	bool CYFile::FileExists(const char_t *ApFileName);
// 
 	/*����ļ���·��,��: E:\VCProjects\FileOperation\1\Debug\1.bsc => E:\VCProjects\FileOperation\1\Debug\ */
	string_t ExtractFilePath(const string_t &AFileName);
// 
// 	/*����ļ���չ��*/
// 	string_t ExtractFileExt(const string_t &AFileName);
// 
// 	/*ǿ�ƴ���Ŀ¼,һ��㴴��*/
// 	bool ForceDirectories(string_t &Dir);
// 
// 	//ɾ��Ŀ¼���������ļ�
// 	BOOL DeleteDirectiories(const string_t &DelPath);
// 
// 	/*�õ�ϵͳĿ¼*/
// 	string_t GetSysPath();
// 
	/*��ǰģ�������ļ���*/
	string_t GetAppFilePath(HMODULE hModule = NULL);

	/*��ǰģ������*/
	string_t GetAppFileName(HMODULE hModule = NULL);

// 	/*�����·���õ�����·��*/
// 	string_t GetAbsFilePath(const string_t &strFullPath, const string_t &strPath);
// 
// 	/*ϵͳ�û�·��*/
// 	string_t GetUserPath(void);
// 
// 	/*��ȡ��ǰ����Ŀ¼*/
// 	string_t GetCurWorkDir();
// 
// 	//����Ŀ¼
// 	bool CreateOpenDir(const string_t& Path);
// 
// 	//  ��ȡ�ļ��汾
// 	UINT64 GetFileVer(LPCTSTR sFileName);
// 	WORD VerToWord(UINT64 uVer);
// 	UINT64 StrToVer(LPCTSTR sVer);
// 	CString VerToStr(UINT64 uVer);
// 
private:
	CYFile();
	~CYFile(void);
// 	BOOL GetSysEnvDir();//����·������û�и���б��
 	string_t Getwstring(const string_t &s, const string_t &Sing, bool Forwar = true);
// 	string_t ExcludeTrailingPathDelimiter(const string_t &s);//����ļ����һ���ַ�Ϊ: \,����ȥ���󷵻�,���򷵻�ԭ�ַ���
 	static CYFile *m_pIns;

// 	CAutoCSec	g_Lock;	// �ٽ�������	
//  	volatile BOOL m_isStop;
// 
// 	map_t<string_t, string_t>g_SysEnvMap;			//ϵͳ�����б�
// 
// 	std::vector<CString> g_IgnoreDIR;							//����Ŀ¼���б�
// 	std::vector<CString> g_CustomDIR;							//ָ��Ŀ¼�б�
};


