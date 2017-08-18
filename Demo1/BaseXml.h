#pragma once
#include "CommonAPI.h"
#include "Encoder.h"
#include "tinyxml/inc/tinyxml.h"
#include "tinyxml/inc/tinystr.h"

class CBaseXml
{
public:
	CBaseXml(void);
	virtual ~CBaseXml(void);

	//��ȡxml�ļ����ú��������LoadIntoMem
	BOOL LoadXmlFile(CString &csXmlFilePath);

	//������Ϣ
	CString GetLastErrorString();

protected:
	//��Ҫ����ʵ�֣��ú�����xml���ݶ�ȡ���ڴ�
	virtual BOOL LoadIntoMem(CString csXmlFilePath)=0;

	//	���ַ�ת�������ص��ڴ����Ҫ�ֶ�free
	inline char * UnicodeToUTF8( const wchar_t *str );

	//�½ڵ�
	TiXmlElement * NewXmlNode(const char *ndName);
	TiXmlElement * NewXmlNode(const wchar_t *ndName);

	//���ýڵ�ֵ
	BOOL SetXmlNodeValue(TiXmlElement * nd,const char *chValue);
	BOOL SetXmlNodeValue(TiXmlElement * nd,const wchar_t *chValue);
	BOOL SetXmlNodeValue(TiXmlElement * nd,const int &chValue);

	//��ӡ��滻�ڵ�����
	BOOL SetXmlNodeAttribute(TiXmlElement * nd,const char *szAttName, const int &chValue);
	BOOL SetXmlNodeAttribute(TiXmlElement * nd,const char *szAttName, const wchar_t *chValue);
	BOOL SetXmlNodeAttribute(TiXmlElement * nd,const char *szAttName, const char *chValue);

	BOOL GetNodeAttribute2Int(TiXmlElement * nd,const char *szAttName,int & nValue);
	BOOL GetNodeAttribute2CString(TiXmlElement * nd,const char *szAttName,CString & csValue);

	BOOL DeleteNode(CString &csXmlFilePath,const wchar_t *csNodeName,const wchar_t *csAttName,const wchar_t *csAttValue,BOOL bIsKey);
	BOOL DeleteNode(CString &csXmlFilePath,const wchar_t *csNodeName,const wchar_t *csAttName,const UINT &nAttValue,BOOL bIsKey);

	bool DocLoadFile(TiXmlDocument &doc,const char *szName);
	bool DocLoadFile(TiXmlDocument &doc,const wchar_t *szName);
	bool DocSaveFile(TiXmlDocument &doc,const char *szName);
	bool DocSaveFile(TiXmlDocument &doc,const wchar_t *szName);

	//	TiXmlElement* FindXmlNode(const char *szNdName,const char *szValue);

private:
	CString m_strError;
	CString m_csXmlFilePath;//xml��ַ

	TiXmlDocument m_doc;
	TiXmlElement *m_pRootElement;
	TiXmlElement *m_newElement;
	TiXmlText *m_newElementValue;
};
