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

	//读取xml文件，该函数会调用LoadIntoMem
	BOOL LoadXmlFile(CString &csXmlFilePath);

	//错误信息
	CString GetLastErrorString();

protected:
	//需要子类实现，该函数把xml数据读取到内存
	virtual BOOL LoadIntoMem(CString csXmlFilePath)=0;

	//	宽字符转换，返回的内存块需要手动free
	inline char * UnicodeToUTF8( const wchar_t *str );

	//新节点
	TiXmlElement * NewXmlNode(const char *ndName);
	TiXmlElement * NewXmlNode(const wchar_t *ndName);

	//设置节点值
	BOOL SetXmlNodeValue(TiXmlElement * nd,const char *chValue);
	BOOL SetXmlNodeValue(TiXmlElement * nd,const wchar_t *chValue);
	BOOL SetXmlNodeValue(TiXmlElement * nd,const int &chValue);

	//添加、替换节点属性
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
	CString m_csXmlFilePath;//xml地址

	TiXmlDocument m_doc;
	TiXmlElement *m_pRootElement;
	TiXmlElement *m_newElement;
	TiXmlText *m_newElementValue;
};
