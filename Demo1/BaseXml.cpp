#include "StdAfx.h"
#include "BaseXml.h"

CBaseXml::CBaseXml(void)
{
}

CBaseXml::~CBaseXml(void)
{
}


// BOOL CBaseXml::LoadXmlFile(CString &csXmlFilePath)
// {
// 	FILE *file;
// 	m_csXmlFilePath = csXmlFilePath;
// 	char *chxmlname = NULL;
// 	chxmlname = this->UnicodeToUTF8( (LPCTSTR)csXmlFilePath);
// 	BOOL bNewXml = FALSE;
// 
// 	if (NULL == (file = fopen(chxmlname,"r")))
// 	{
// 		bNewXml = TRUE;
// 	}
// 	else
// 	{	
// 		fclose(file);
// 		file  = NULL;
// 		TiXmlDocument doc;
// 		if(FALSE == DocLoadFile(doc,csXmlFilePath) )
// 			bNewXml = TRUE;
// 	}
// 
// 	if(bNewXml)//生成新的dat
// 	{
// 		if (NULL == (file = fopen(chxmlname,"w")))
// 		{
// 			m_strError.Format(L"create XmlFile error");
// 			return FALSE;
// 		}
// 		fclose(file);
// 		file  = NULL;
// 
// 		TiXmlDocument *doc = new TiXmlDocument();
// 		if (NULL == doc)
// 		{
// 			m_strError.Format(L"create TiXmlDocument error");
// 			FreeAndNull(chxmlname);
// 			return FALSE;
// 		}
// 
// 		//XML版本
// 		TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","utf-8","yes");
// 		doc->LinkEndChild(decl);
// 		DocSaveFile(*doc,chxmlname);
// 
// 		doc->LoadFile(chxmlname);
// 		m_pRootElement = new TiXmlElement("MANAGER");
// 		if (NULL == m_pRootElement)
// 		{
// 			m_strError.Format(L"create xml error");
// 			FreeAndNull(chxmlname);
// 			return FALSE;
// 		}
// 
// 		doc->LinkEndChild(m_pRootElement);
// 		DocSaveFile(*doc,chxmlname);
// 	}
// 
// 	if (!bNewXml)//读取XML
// 	{
// 		if (FALSE == LoadIntoMem(csXmlFilePath))
// 		{
// 			FreeAndNull(chxmlname);
// 			m_strError.Format(L"LoadIntoMem error");
// 			return FALSE;
// 		}
// 	}
// 
// 	FreeAndNull(chxmlname);
// 	return TRUE;
// }

BOOL CBaseXml::LoadXmlFile(CString &csXmlFilePath)
{
	BOOL bReturn = FALSE;
	char *chxmlname = NULL;
	do 
	{
		FILE *file;
		m_csXmlFilePath = csXmlFilePath;
		chxmlname = this->UnicodeToUTF8( (LPCTSTR)csXmlFilePath);
		BOOL bNewXml = FALSE;

		if (NULL == (file = fopen(chxmlname,"r")))
		{
			bNewXml = TRUE;
		}
		else
		{	
			fclose(file);
			file  = NULL;
			TiXmlDocument doc;
			if(FALSE == DocLoadFile(doc,csXmlFilePath) )
				bNewXml = TRUE;
		}

		if(bNewXml)//生成新的dat
		{
			if (NULL == (file = fopen(chxmlname,"w")))
			{
				m_strError.Format(L"create XmlFile error");
				break;
			}
			fclose(file);
			file  = NULL;

			TiXmlDocument *doc = new TiXmlDocument();
			if (NULL == doc)
			{
				m_strError.Format(L"create TiXmlDocument error");
				break;
			}

			//XML版本
			TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","utf-8","yes");
			doc->LinkEndChild(decl);
			DocSaveFile(*doc,chxmlname);

			doc->LoadFile(chxmlname);
			m_pRootElement = new TiXmlElement("MANAGER");
			if (NULL == m_pRootElement)
			{
				m_strError.Format(L"create xml error");
				break;
			}

			doc->LinkEndChild(m_pRootElement);
			DocSaveFile(*doc,chxmlname);
		}

		if (!bNewXml)//读取XML
		{
			if (FALSE == LoadIntoMem(csXmlFilePath))
			{
				m_strError.Format(L"LoadIntoMem error");
				break;
			}
		}
		bReturn = TRUE;
	} while (0);

	FreeAndNull(chxmlname);
	return bReturn;
}

CString CBaseXml::GetLastErrorString()
{
	CString strtmp(m_strError);
	m_strError.Empty();
	return strtmp;
}

char * CBaseXml::UnicodeToUTF8( const wchar_t *str )
{
	if (NULL == str)
		return NULL;

	char * result;
	int textlen;
	// wide char to multi char
	textlen = WideCharToMultiByte( CP_UTF8,    0,    str,    -1,    NULL, 0, NULL, NULL );
	result =(char *)malloc((textlen+1)*sizeof(char));
	memset( result, 0, sizeof(char) * ( textlen + 1 ) );
	WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL );
	return result;
}

TiXmlElement * CBaseXml::NewXmlNode(const char *ndName)
{
	if (NULL == ndName)
		return NULL;

	TiXmlElement *newItem = new TiXmlElement(ndName);

	return newItem;
}

TiXmlElement * CBaseXml::NewXmlNode(const wchar_t *ndName)
{
	if (NULL == ndName)
		return NULL;

	char *ctmp = UnicodeToUTF8(ndName);
	TiXmlElement *newItem = new TiXmlElement(ctmp);
	FreeAndNull(ctmp);

	return newItem;
}

BOOL CBaseXml::SetXmlNodeValue(TiXmlElement * nd,const char *chValue)
{
	if (NULL == nd || NULL == chValue)
		return FALSE;

	TiXmlNode *m_newElementValue = nd->FirstChild();
	if (NULL == m_newElementValue)
	{
		m_newElementValue = new TiXmlText(chValue);
		nd->LinkEndChild(m_newElementValue);
		return TRUE;
	}

	m_newElementValue->SetValue(chValue);
	return TRUE;
}

BOOL CBaseXml::SetXmlNodeValue(TiXmlElement * nd,const wchar_t *chValue)
{
	if (NULL == nd || NULL == chValue)
		return FALSE;


	char *ctmp = UnicodeToUTF8(chValue);

	TiXmlNode *m_newElementValue = nd->FirstChild();
	if (NULL == m_newElementValue)
	{
		m_newElementValue = new TiXmlText(ctmp);
		nd->LinkEndChild(m_newElementValue);
		FreeAndNull(ctmp);
		return TRUE;
	}

	m_newElementValue->SetValue(ctmp);
	FreeAndNull(ctmp);
	return TRUE;
}

BOOL CBaseXml::SetXmlNodeValue(TiXmlElement * nd,const int &chValue)
{
	if (NULL == nd)
		return FALSE;

	char * szBuffer = new char[64];
	memset(szBuffer,0,64);
	itoa(chValue,szBuffer,10);
	if (0 == strlen(szBuffer))
		return FALSE;

	TiXmlNode *m_newElementValue = nd->FirstChild();
	if (NULL == m_newElementValue)
	{
		m_newElementValue = new TiXmlText(szBuffer);
		nd->LinkEndChild(m_newElementValue);
		return TRUE;
	}

	m_newElementValue->SetValue(szBuffer);
	return TRUE;
} 

BOOL CBaseXml::SetXmlNodeAttribute(TiXmlElement * nd,const char *szAttName, const int &chValue)
{
	if (NULL == nd || NULL == szAttName)
	{
		m_strError = L"node or attname is null";
		return FALSE;
	}
	char szbuf[256] = {0};
	itoa(chValue,szbuf,10);
	nd->SetAttribute(szAttName,szbuf);//这里返回值没有判断
	return TRUE;
}

BOOL CBaseXml::SetXmlNodeAttribute(TiXmlElement * nd,const char *szAttName, const wchar_t *chValue)
{
	if (NULL == nd || NULL == szAttName || NULL == chValue)
	{
		m_strError = L"node or attname or value is null";
		return FALSE;
	}
	if (0 == wcslen(chValue))
	{
		nd->SetAttribute(szAttName,"");
		return TRUE;
	}

	char *szUtf8 = NULL;
	szUtf8 = UnicodeToUTF8(chValue);
	if (NULL == szUtf8)
		return FALSE;

	nd->SetAttribute(szAttName,szUtf8);

	FreeAndNull(szUtf8);
	return TRUE;
}

BOOL CBaseXml::SetXmlNodeAttribute(TiXmlElement * nd,const char *szAttName, const char *chValue)
{
	if (NULL == nd || NULL == szAttName || NULL == chValue)
	{
		m_strError = L"node or attname or value is null";
		return FALSE;
	}
	if (0 == strlen(chValue))
	{
		nd->SetAttribute(szAttName,"");
		return TRUE;
	}
	nd->SetAttribute(szAttName,chValue);
	return TRUE;
}

bool CBaseXml::DocLoadFile(TiXmlDocument &doc,const char *szName)
{
	if (NULL == szName)
		return FALSE;
	return doc.LoadFile(szName);
}

bool CBaseXml::DocLoadFile(TiXmlDocument &doc,const wchar_t *szName)
{
	if (NULL == szName)
		return FALSE;

	char *szUtf8 = NULL;
	szUtf8 = UnicodeToUTF8(szName);

	bool bReturn  = doc.LoadFile(szUtf8);

	FreeAndNull(szUtf8);

	return bReturn;
}

bool CBaseXml::DocSaveFile(TiXmlDocument &doc,const char *szName)
{
	if (NULL == szName)
		return FALSE;
	return doc.SaveFile(szName);
}

bool CBaseXml::DocSaveFile(TiXmlDocument &doc,const wchar_t *szName)
{
	if (NULL == szName)
		return FALSE;

	char *szUtf8 = NULL;
	szUtf8 = UnicodeToUTF8(szName);

	bool bReturn  = doc.SaveFile(szUtf8);

	FreeAndNull(szUtf8);

	return bReturn;
}

BOOL CBaseXml::GetNodeAttribute2Int(TiXmlElement * nd,const char *szAttName,int & nValue)
{
	nValue = 0;
	if (NULL == szAttName || NULL == nd)
		return FALSE;

	const char * szAttrValue = nd->Attribute(szAttName);
	if (NULL == szAttrValue)
		return FALSE;

	nValue = atoi(szAttrValue);
	return TRUE;
}

BOOL CBaseXml::GetNodeAttribute2CString(TiXmlElement * nd,const char *szAttName,CString &csValue)
{
	csValue.Empty();
	if (NULL == szAttName || NULL == nd)
		return FALSE;

	const char * szAttrValue = nd->Attribute(szAttName);
	if (NULL == szAttrValue)
		return FALSE;

	// 	int nsite=0,nlen = strlen(szAttrValue),ntmp;
	// 	char szTmpValue[MAX_PATH]={0};
	// 	strcpy_s(szTmpValue,szAttrValue);
	// 	while(nsite < nlen){
	// 		if('\\' == szTmpValue[nsite] && '\\' == szTmpValue[nsite+1]){
	// 			ntmp = nsite;
	// 			while(nsite < nlen){
	// 				szTmpValue[nsite]=szTmpValue[nsite+1];++nsite;
	// 			}--nlen;
	// 			nsite = ntmp+1;
	// 		}
	// 	}

	CEncoder e;
	e.SetMString(szAttrValue, CP_UTF8);//转码
	csValue = e.GetWString();

	return TRUE;
}

BOOL CBaseXml::DeleteNode(CString &csXmlFilePath,const wchar_t *csNodeName,const wchar_t *csAttName,const wchar_t *csAttValue,BOOL bIsKey)
{
	//更新XML
	TiXmlDocument doc;
	if (DocLoadFile(doc,csXmlFilePath))
	{
		TiXmlElement *m_pRootElement = doc.RootElement();
		if( m_pRootElement)
		{
			TiXmlElement * porperty_element = m_pRootElement->FirstChildElement();
			char *szAttName = UnicodeToUTF8(csAttName);
			char *szAttValue = UnicodeToUTF8(csAttValue);
			char *szNodeName = UnicodeToUTF8(csNodeName);
			while( porperty_element )
			{	

				if (0 == strcmp(szAttValue, porperty_element->Attribute(szAttName)))
				{
					TiXmlNode *nd = porperty_element->ToElement();
					porperty_element = porperty_element->NextSiblingElement();
					m_pRootElement->RemoveChild(nd);
					if (bIsKey)
						break;
				}
				porperty_element = porperty_element->NextSiblingElement(szNodeName);
			}

			DocSaveFile(doc,csXmlFilePath);
			FreeAndNull(szAttName);
			FreeAndNull(szAttValue);
			FreeAndNull(szNodeName);
		}
	}
	return TRUE;
}

BOOL CBaseXml::DeleteNode(CString &csXmlFilePath,const wchar_t *csNodeName,const wchar_t *csAttName,const UINT &nAttValue,BOOL bIsKey)
{
	//更新XML
	TiXmlDocument doc;
	if (csAttName && DocLoadFile(doc,csXmlFilePath))
	{
		TiXmlElement *m_pRootElement = doc.RootElement();
		if( m_pRootElement)
		{
			TiXmlElement * porperty_element = m_pRootElement->FirstChildElement();
			char *szAttName = UnicodeToUTF8(csAttName);
			char *szNodeName = UnicodeToUTF8(csNodeName);
			while( porperty_element )
			{	
				const char * szAttValue = porperty_element->Attribute(szAttName);
				if (NULL != szAttValue && nAttValue == atoi(szAttValue))
				{
					TiXmlNode *nd = porperty_element->ToElement();
					porperty_element = porperty_element->NextSiblingElement();
					m_pRootElement->RemoveChild(nd);
					if (bIsKey)
						break;
				}
				porperty_element = porperty_element->NextSiblingElement(szNodeName);
			}

			DocSaveFile(doc,csXmlFilePath);
			FreeAndNull(szAttName);
			FreeAndNull(szNodeName);
		}
	}
	return TRUE;	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////