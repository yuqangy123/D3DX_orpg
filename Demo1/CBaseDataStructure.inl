#include "CBaseDataStructure.h"

template<class type>
CMemoryPool<type>::CMemoryPool()
{
}

template<class type>
bool CMemoryPool<type>::init(const DWORD &nNodeCount)
{
	SIZEOF_DWORD = sizeof(DWORD);
	m_nNodeSize = sizeof(type);

	m_nBufSize = (m_nNodeSize)*nNodeCount;
	m_pBuf = (type*)malloc(m_nBufSize);
	m_pRecoPointer = (DWORD *)malloc((nNodeCount+1)*SIZEOF_DWORD);

	if(!m_pBuf || !m_pRecoPointer)
		return false;

	memset(m_pBuf,0,m_nBufSize);
	m_pFreePointer = m_pBuf;
	memset(m_pRecoPointer,0x0,(nNodeCount+1)*SIZEOF_DWORD);
	m_pBaseRecoPointer = m_pRecoPointer;

	m_nNodeCounter = 0;
	m_nMaxNodes = nNodeCount;
	return true;
}

template<class type>
CMemoryPool<type>::~CMemoryPool()
{
	if(m_pBuf)
		free(m_pBuf);
	if(m_pBaseRecoPointer)
		free(m_pBaseRecoPointer);
};

template<class type>
type * CMemoryPool<type>::NewNode_(unsigned int nCount)
{
	if((m_nNodeCounter+nCount) > m_nMaxNodes)
		return 0;

	type *p=0, *pTest=0;//test code
	if(0 == *m_pRecoPointer)//空闲区找
	{
		p=m_pFreePointer;
		m_pFreePointer+=nCount;
	}
	if(*m_pRecoPointer)//回收处有节点
	{
		DWORD *pRecoTmp = m_pRecoPointer;
		DWORD dwSz=0;
		int nDie = 1;
		while (pRecoTmp != m_pBaseRecoPointer)//寻找在回收处的可用大小内存
		{
			if(nDie >= nCount)
				break;

			dwSz = *pRecoTmp;
			pTest = (type*)*pRecoTmp;//test code
			if(abs(dwSz-*(--pRecoTmp)) == m_nNodeSize)
				++nDie;
			else
				nDie = 1;
		}
		if(nDie >= nCount)//找到连续内存
		{
			p=(type*)*pRecoTmp;
			DWORD *pRecoTmp1 = pRecoTmp;
			pRecoTmp1+=nCount;//??
			pTest = (type*)*pRecoTmp1;
			while (pRecoTmp1 <= m_pRecoPointer){//移动回收指针
				pTest = (type*)*pRecoTmp;
				pTest = (type*)*pRecoTmp1;
				*pRecoTmp++ = *pRecoTmp1++;
			}
			pTest = (type*)*pRecoTmp;
			m_pRecoPointer = (pRecoTmp == m_pBaseRecoPointer ? (*m_pBaseRecoPointer=0, m_pBaseRecoPointer) : --pRecoTmp);
			pTest = (type*)*m_pRecoPointer;
			pTest = (type*)*pRecoTmp;
		}
		else//在空闲区找
		{
			p=m_pFreePointer;
			m_pFreePointer+=nCount;
		}
	}

	m_nNodeCounter+=nCount;
	return p;
}

template<class type>
void CMemoryPool<type>::Clear()
{
	m_Lock.Lock();

	memset(m_pBuf,0,m_nBufSize);
	m_pFreePointer = m_pBuf;

	memset(m_pBaseRecoPointer,0x0,(m_nMaxNodes+1)*SIZEOF_DWORD);
	m_pRecoPointer = m_pBaseRecoPointer;
	
	m_nNodeCounter = 0;

	m_Lock.UnLock();
}

template<class type>
type * CMemoryPool<type>::NewNode(unsigned int nCount /*= 1*/)
{
	m_Lock.Lock();
	type * nd = NewNode_(nCount);
	m_Lock.UnLock();
	return nd;
}

template<class type>
void CMemoryPool<type>::TestPrint()
{
	static int nfile=0;
	char cfilename[32]={0};
	sprintf_s(cfilename,"c:\\recoData_%d.txt", nfile++);
	CMyTrace logfile(cfilename,"w");
	logfile.on();
	
	int nRecoCount=0, nIdleCount=0, nUsingCount=0;
	type *pNodeTemp=m_pBuf;
	DWORD dwBeginAddr=0, dwEndAddr=0;
	int nprintIndex = zero;
	const char* chState[5]={"idle","zero","reco","","unsing"};

	debug("\r\n-----------------------------------------------\r\n");
	for (int n=0; n<m_nMaxNodes; ++n)
	{
		switch(pNodeTemp[n].cState)
		{
		case unsing:{
			if(unsing != nprintIndex)
			{
				debug("%s:%d-%d |||| ",chState[nprintIndex], dwBeginAddr, n-1);
				nprintIndex = unsing;
				dwBeginAddr=n;
			}
			++nUsingCount;
					}break;
		case reco:{
			if(reco != nprintIndex)
			{
				debug("%s:%d-%d |||| ",chState[nprintIndex], dwBeginAddr, n-1);
					nprintIndex = reco;
				dwBeginAddr=n;
			}
			++nRecoCount;
				  }break;
		case idle:{
			if(idle != nprintIndex)
			{
				debug("%s:%d-%d |||| ",chState[nprintIndex], dwBeginAddr, n-1);
					nprintIndex = idle;
				dwBeginAddr=n;
			}
			++nIdleCount;
				  }break;
		}
	}
	debug("\r\n-----------------------------------------------\r\n");
	DWORD *pRecoTmp = m_pRecoPointer;
	if(*m_pBaseRecoPointer){
		for (;pRecoTmp >= m_pBaseRecoPointer; --pRecoTmp)
		{
			type* pData = (type*)*pRecoTmp;
			char cprint[128]={0};
			sprintf_s(cprint, "pcgame=%d, count=%d, state=%s \r\n", pData->dwPCGame, pData->dwPCIDCount, chState[pData->cState]);
			logfile.print(cprint);
		}
	}

	
	debug("\r\n-----------------------------------------------\r\n");
	debug("unsingcount=%d, recocount=%d, idlecount=%d", nUsingCount, nRecoCount, nIdleCount);


	
		
// 	debug("reco node:\r\n");
// 	DWORD *RecoTemp = m_pRecoPointer;//回收指针,存放地址
// 	while (RecoTemp != m_pBaseRecoPointer)
// 	{
// 		pNodeTemp = (type *)*RecoTemp;
// 		pNodeTemp->Testprint();
// 		++nRecoCount;
// 		--RecoTemp;
// 	}
// 	debug("\r\n-----------------------------------------------\r\n");

	
}

template<class type>
bool CMemoryPool<type>::DelNode_(const type* p, unsigned int nCount )
{
	if(0 == m_nNodeCounter)//当前无使用节点
		return false;

	if(p > (m_pBuf+m_nMaxNodes) || p < m_pBuf)
		return false;

	DWORD *nORecoPointer = m_pBaseRecoPointer;
	int n=0;
	for (; n<m_nMaxNodes; ++n)
	{
		if(*nORecoPointer == (DWORD)p)
			break;
		if(nORecoPointer++ == m_pRecoPointer)//回收指针的指针地址自加
			break;
	}
	if(n >= m_nMaxNodes)
		return false;

	type* pTest=0;//test code
	for (n=0;n<nCount;++n){
		(0 == *m_pRecoPointer ? *m_pRecoPointer:*(++m_pRecoPointer)) = (DWORD)p++;
		pTest = (type*)*m_pRecoPointer;
	}
	
	m_nNodeCounter-=nCount;

	return true;
}

template<class type>
bool CMemoryPool<type>::DelNode(const type* p, unsigned int nCount /*= 1*/)
{
	m_Lock.Lock();
	bool bRet = DelNode_(p, nCount);
	m_Lock.UnLock();
	return bRet;
}

/////////////////////////////////////////////////////////////////////////////////////////////


template<class type>
CBaseLink<type>::CBaseLink():
m_dwVersion(0),m_dwNodeSize(sizeof(type)),m_dwMaxNodes(0),m_dwNodeCounter(0),SIZEOF_DWORD(sizeof(DWORD)),
m_pbuf(0),m_pHeadPointer(0),m_pEndPointer(0),m_pFreePointer(0),m_pRecoPointer(0),m_pBaseRecoPointer(0)
{
}

template<class type>
bool CBaseLink<type>::init(const DWORD &ndCount,const DWORD &version)
{
	m_dwVersion=version;
	m_dwMaxNodes=ndCount;

	if(m_dwMaxNodes)
	{
		m_pbuf = (char *)malloc(m_dwMaxNodes*(m_dwNodeSize+SIZEOF_DWORD));
		if(!m_pbuf)
			return false;	
		memset(m_pbuf ,0x0, m_dwMaxNodes*(m_dwNodeSize+SIZEOF_DWORD));

		m_pHeadPointer = (DWORD)m_pbuf;
		m_pEndPointer = (DWORD)m_pHeadPointer;
		m_pFreePointer = (DWORD)m_pbuf;

		m_pRecoPointer = (DWORD*)malloc(m_dwMaxNodes*SIZEOF_DWORD);
		if(m_pRecoPointer)
			return false;
		memset(m_pRecoPointer, 0x0, m_dwMaxNodes*SIZEOF_DWORD);
		m_pBaseRecoPointer = m_pRecoPointer;
		return true;
	}
	return false;
}


template<class type>
CBaseLink<type>::~CBaseLink()
{
	if(m_pbuf)
		free(m_pbuf);

	if(m_pBaseRecoPointer)
		free(m_pBaseRecoPointer);
}

template<class type>
bool CBaseLink<type>::push(const type* const nd)
{
	bool bReturn = false;
	m_Lock.WLock();

	if (m_dwNodeCounter >= m_dwMaxNodes || !nd)
		goto EXIT;

	DWORD dwNext = 0;
	if (*m_pRecoPointer)
	{
		/*if(m_pHeadPointer == m_pEndPointer)*/
		memcpy_s((void*)(*m_pRecoPointer),m_dwNodeSize, nd,m_dwNodeSize);

		if(m_pEndPointer == (DWORD)m_pRecoPointer)//如果尾指针和回收指针处于同一位置
			dwNext = 0;
		else
			dwNext = (DWORD)m_pRecoPointer;

		memcpy_s((void *)(m_pEndPointer+m_dwNodeSize), SIZEOF_DWORD,&dwNext, SIZEOF_DWORD);//尾指针的next指向新的地址
		m_pEndPointer = (DWORD)m_pRecoPointer;//尾指针向后移一位

		dwNext=0;
		memcpy_s((void *)(m_pEndPointer+m_dwNodeSize), SIZEOF_DWORD, &dwNext, SIZEOF_DWORD);//新尾指针的next指向0

		if(m_pBaseRecoPointer == m_pRecoPointer)
			*m_pRecoPointer = 0;//回收指针只有一个节点
		else
			--m_pRecoPointer;
		
	}
	else
	{
		memcpy_s((void*)m_pFreePointer,m_dwNodeSize, nd,m_dwNodeSize);
		
		if(m_pFreePointer != m_pEndPointer)
			dwNext = m_pFreePointer;
		else
			dwNext = 0;//当尾指针和空闲指针式同一地址时，next指向0

		memcpy_s((void *)(m_pEndPointer+m_dwNodeSize), SIZEOF_DWORD, &dwNext,SIZEOF_DWORD);//尾指针的next指向新的位置

		m_pEndPointer = m_pFreePointer;//尾指针向后移一位

		dwNext=0;
		memcpy_s((void *)(m_pEndPointer+m_dwNodeSize), SIZEOF_DWORD, &dwNext, SIZEOF_DWORD);//新尾指针的next指向0

		m_pFreePointer += (m_dwNodeSize+SIZEOF_DWORD);//空闲指针向后移一位
	}
	++m_dwNodeCounter;
	bReturn = true;


EXIT:
	m_Lock.WUnLock();
	return bReturn;
}

template<class type>
bool CBaseLink<type>::pop()
{
	if (0 == m_dwNodeCounter)
		return false;

	//回收指针是否为空
	(0 == *m_pRecoPointer ? *(m_pRecoPointer):*(++m_pRecoPointer)) = m_pHeadPointer;

	//如果头尾指针不是同一个 头指针向后移一位
	if (m_pHeadPointer != m_pEndPointer)
	{
		DWORD dwNext = 0;
		memcpy_s(&dwNext,SIZEOF_DWORD,(void *)(m_pHeadPointer+m_dwNodeSize),SIZEOF_DWORD);
		if(dwNext)
			m_pHeadPointer = dwNext;
	}

	--m_dwNodeCounter;

	return true;
}

template<class type>
type* CBaseLink<type>::top()
{
	type * nd = 0;
	if(0 < m_dwNodeCounter)
	{
		m_Lock.RLock();
		nd = (type*)m_pHeadPointer;
		m_Lock.RUnLock();
	}

	return nd;
}

template<class type>
DWORD CBaseLink<type>::size()
{
	if(m_pbuf && m_pBaseRecoPointer)
		return m_dwMaxNodes*(m_dwNodeSize+2*SIZEOF_DWORD);
	else 
		return 0;
}


template<class type>
DWORD CBaseLink<type>::GetLength()
{
	if(m_pbuf && m_pBaseRecoPointer)
	{
		m_Lock.RLock();
		DWORD dwLength = m_dwNodeCounter;
		m_Lock.RUnLock();
		return dwLength;
	}
	else 
		return 0;
}

template<class type>
void CBaseLink<type>::Clear()
{
	m_Lock.Lock();

	memset(m_pbuf ,0x0, m_dwMaxNodes*(m_dwNodeSize+SIZEOF_DWORD));
	m_pHeadPointer = (DWORD)m_pbuf;
	m_pEndPointer = (DWORD)m_pHeadPointer;
	m_pFreePointer = (DWORD)m_pbuf;
	memset(m_pBaseRecoPointer, 0x0, m_dwMaxNodes*SIZEOF_DWORD);
	m_pRecoPointer = m_pBaseRecoPointer;
	m_dwNodeCounter = 0;

	m_Lock.UnLock();
}

template<class type>
bool CBaseLink<type>::Delete_(const type &nd)
{
	if(0 >= m_dwNodeCounter)
		return false;

	DWORD phead = m_pHeadPointer,dwNext = 0;
	DWORD pnd,pfrontnd;
	pnd = phead;
	pfrontnd = pnd;
	do 
	{
		if(*((type*)pnd) == nd)//这里需要节点数据结构重载==操作符
		{
			if (pfrontnd == m_pHeadPointer)
				return pop();

			memcpy_s(&dwNext,SIZEOF_DWORD,(void*)(pnd+m_dwNodeSize),SIZEOF_DWORD);
			memcpy_s((void*)(pfrontnd+m_dwNodeSize),SIZEOF_DWORD,&dwNext,SIZEOF_DWORD);
			
			*( m_pRecoPointer += (0 == *m_pRecoPointer)?0:1 ) = pnd;//回收指针没有节点
			--m_dwNodeCounter;
			return true;
		}

		if (phead == m_pEndPointer)
			return false;

		memcpy_s(&dwNext,SIZEOF_DWORD,(void *)(phead+m_dwNodeSize),SIZEOF_DWORD);
		phead = dwNext;
	} while (true);
	

	return false;
}

template<class type>
bool CBaseLink<type>::Delete(const type &nd)
{
	m_Lock.WLock();
	bool bReturn = Delete_(nd);
	m_Lock.WUnLock();
	return bReturn;
}

template<class type>
type* const CBaseLink<type>::Next(const type * const nd)
{
	if (NULL == nd)
		return 0;

	DWORD nNext = 0;
	m_Lock.Lock();
	memcpy_s(&nNext, SIZEOF_DWORD,(void *)((DWORD)nd + m_dwNodeSize),SIZEOF_DWORD);
	m_Lock.UnLock();

	return (type*)nNext;
}

//////////////////////////////////////////////////////////////////////////////

template<class type>
CHashTable<type>::CHashTable():
m_pbuf(0), m_dwVersion(0), m_TableLength(0),m_mempool(0),
SIZEOF_DWORD(sizeof(DWORD)),SIZEOF_NODE(sizeof(type)), m_nNodeCounter(0)
{
}


template<class type>
bool CHashTable<type>::init(DWORD dwTableLen, DWORD dwVersion)
{
	m_dwVersion=dwVersion;
	m_TableLength=dwTableLen;

	if(m_TableLength)
	{
		m_pbuf = new DWORD[m_TableLength];
		m_mempool = new CMemoryPool<HashNode>(m_TableLength);
		if(!m_pbuf || !m_mempool)
			return false;
		
		memset(m_pbuf,0x0,m_TableLength*SIZEOF_DWORD);
		return true;
	}

	return false;
}

template<class type>
CHashTable<type>::~CHashTable()
{
	if(m_pbuf)
		delete m_pbuf;

	if(m_mempool)
		delete m_mempool;

}


template<class type>
void CHashTable<type>::Clear()
{
	m_Lock.Lock();
	m_mempool->Clear();
	memset(m_pbuf,0x0,m_TableLength*SIZEOF_DWORD);
	m_nNodeCounter = 0;
	m_Lock.UnLock();
}

template<class type>
DWORD CHashTable<type>::size()
{
	if (m_mempool && m_pbuf)
		return ((m_TableLength*4) + (m_TableLength*SIZEOF_NODE));

	return 0;
}

template<class type>
DWORD CHashTable<type>::GetItemCount()
{
	return m_nNodeCounter;//bug
}

template <class type>
void CHashTable<type>::CreateIndex(LPVOID szdata, const DWORD &nDataLen,DWORD &nIndex)
{
	DWORD r = 0;
	byte* p = (byte*)szdata;
	for (DWORD i = 0; i < nDataLen; i++)
		r = ((r << 2) | (r >> 30)) ^ p[i];

	nIndex = r%m_TableLength;
}

template <class type>
void CHashTable<type>::CreateIndex(const DWORD &szdata, DWORD &nIndex)
{
	//Thomas Wang
	nIndex = szdata;
	nIndex += ~(nIndex<<15);
	nIndex ^= (nIndex >> 10);
	nIndex += (nIndex << 3);
	nIndex ^= (nIndex >> 6);
	nIndex += (nIndex << 11);
	nIndex ^= (nIndex >> 16);
	nIndex = (nIndex%m_TableLength);
}

template <class type>
bool CHashTable<type>::Add(LPVOID szdata, const DWORD &nDataLen,const type &nd)
{
	if(!szdata)
		return false;

	DWORD dwIndex = 0;
	CreateIndex(szdata,nDataLen,dwIndex);
	
	m_Lock.Lock();
	HashNode *NewNd = m_mempool->NewNode();
	HashNode *nextNd=0;

	if(NewNd)
	{
		memcpy_s(&NewNd->node,SIZEOF_NODE,&nd,SIZEOF_NODE);
		NewNd->next=0;

		nextNd = (HashNode*)m_pbuf[dwIndex];
		if(!nextNd)
			m_pbuf[dwIndex] = (DWORD)NewNd;
		else
		{
			while (nextNd->next)
				nextNd = nextNd->next;
			nextNd->next = NewNd;
		}
		++m_nNodeCounter;
		m_Lock.UnLock();
		return true;
	}

	m_Lock.UnLock();
	return false;
}

template <class type>
bool CHashTable<type>::Add(const DWORD &szdata, const type &nd)
{
	DWORD dwIndex = 0;
	CreateIndex(szdata,dwIndex);

	m_Lock.Lock();
	HashNode *NewNd = m_mempool->NewNode();
	HashNode *nextNd=0;

	if(NewNd)
	{
		memcpy_s(&(NewNd->node),SIZEOF_NODE,&nd,SIZEOF_NODE);
		NewNd->next=0;

		nextNd = (HashNode*)m_pbuf[dwIndex];
		if(!nextNd)
			m_pbuf[dwIndex] = (DWORD)NewNd;
		else
		{
			while (nextNd->next)
				nextNd = nextNd->next;
			nextNd->next = NewNd;
		}
		++m_nNodeCounter;
		m_Lock.UnLock();
		return true;
	}

	m_Lock.UnLock();
	return false;
}

template <class type>
bool CHashTable<type>::get(LPVOID szdata, const DWORD &nDataLen,type *nd)
{
	if(!nd || !szdata)
		return false;

	DWORD dwIndex = 0;
	CreateIndex(szdata,nDataLen,dwIndex);

	m_Lock.Lock();
	HashNode *tnd=(HashNode *)m_pbuf[dwIndex];
	
	while (tnd)
	{
		if(tnd->node == nd)
		{
			memcpy_s(nd,SIZEOF_NODE,&(tnd->node),SIZEOF_NODE);
			m_Lock.UnLock();
			return true;
		}
		tnd = tnd->next;
	}

	m_Lock.UnLock();
	return false;
}


template <class type>
bool CHashTable<type>::get(const DWORD &szdata, type *nd)
{
	if(!nd)
		return false;

	DWORD dwIndex = 0;
	CreateIndex(szdata,dwIndex);

	m_Lock.Lock();
	HashNode *tnd=(HashNode *)m_pbuf[dwIndex];
	
	while (tnd)
	{
		if(tnd->node == nd)
		{
			memcpy_s(nd,SIZEOF_NODE,&(tnd->node),SIZEOF_NODE);
			m_Lock.UnLock();
			return true;
		}
		tnd = tnd->next;
	}
	m_Lock.UnLock();
	return false;
}

template<class type>
bool CHashTable<type>::Find(const DWORD &szdata, const type &nd)
{
	DWORD dwIndex = 0;
	CreateIndex(szdata,dwIndex);

	m_Lock.Lock();
	HashNode *tnd=(HashNode *)m_pbuf[dwIndex];
	
	while (tnd)
	{
		if(tnd->node == nd){
			m_Lock.UnLock();
			return true;
		}
		tnd = tnd->next;
	}

	m_Lock.UnLock();
	return false;
}

template<class type>
bool CHashTable<type>::Find(LPVOID szdata, const DWORD &nDataLen,const type &nd)
{
	if(!szdata)
		return false;

	DWORD dwIndex = 0;
	CreateIndex(szdata,nDataLen,dwIndex);

	m_Lock.Lock();
	HashNode *tnd=(HashNode *)m_pbuf[dwIndex];

	while (tnd)
	{
		if(tnd->node == nd){
			m_Lock.UnLock();
			return true;
		}
		tnd = tnd->next;
	}

	m_Lock.UnLock();
	return false;
}
