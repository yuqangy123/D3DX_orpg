#pragma once
//#include "Trace.h"

//�ٽ���(�̰߳�ȫ)
class CAutoCSec
{
	CRITICAL_SECTION   m_cs;
public:
	CAutoCSec()	{InitializeCriticalSection (&m_cs) ;}
	virtual ~CAutoCSec() {DeleteCriticalSection (&m_cs) ;}
	void Lock() {EnterCriticalSection(&m_cs);}
	void UnLock() {LeaveCriticalSection(&m_cs);}
	BOOL TryLock(){return TryEnterCriticalSection(&m_cs);}
};

//��д��(ĳЩ�Ͱ汾windowsϵͳ��֧��)
class CSRWLock
{
	SRWLOCK   m_cs;
public:
	CSRWLock()	{InitializeSRWLock(&m_cs) ;}
	void WLock(){AcquireSRWLockExclusive(&m_cs);};
	void WUnLock(){ReleaseSRWLockExclusive(&m_cs);};

	void RLock(){AcquireSRWLockShared(&m_cs);};
	void RUnLock(){ReleaseSRWLockShared(&m_cs);};
};

template<class type>
class CMemoryPool
{
public:
	CMemoryPool();
	~CMemoryPool();
	bool init(const DWORD &nNodeCount);

	void Clear();
	type * NewNode(unsigned int nCount = 1);
	bool DelNode(const type* p, unsigned int nCount=1);
	void NodeCount(DWORD &n){n = m_nMaxNodes};
	void FreeNodeCount(DWORD &n){n = m_nMaxNodes-m_nNodeCounter;};
	void UsingNodeCount(DWORD &n){n = m_nNodeCounter;};
	void TestPrint();
	const type* const getBufBegin(){return m_pBuf;};

private:
	type * NewNode_(unsigned int nCount);
	bool DelNode_(const type* p, unsigned int nCount);
	
private:
	type *m_pBuf;//�����׵�ַ
	type *m_pFreePointer;//����ָ��

	DWORD *m_pRecoPointer;//����ָ��,��ŵ�ַ
	DWORD *m_pBaseRecoPointer;//����ָ���׵�ַ

	volatile DWORD m_nNodeCounter;//�ڵ�ʹ�ü�����
	DWORD m_nBufSize;//buf size

	DWORD m_nMaxNodes;
	DWORD m_nNodeSize;//���ڵ��С

	WORD SIZEOF_DWORD;
	CAutoCSec m_Lock;

};

template<class type>
class CBaseLink
{
protected:
	DWORD m_dwVersion;	//�汾
	DWORD m_dwNodeSize;	//�ڵ��С
	DWORD m_dwMaxNodes;	//�ڵ�������
	DWORD m_dwNodeCounter;//�ڵ������

	/*void *m_pbuf;*/
	char *m_pbuf;
	DWORD m_pHeadPointer;	//ͷָ��
	DWORD m_pEndPointer;	//βָ��
	DWORD m_pFreePointer;	//����
	DWORD *m_pRecoPointer;	//����
	DWORD *m_pBaseRecoPointer;//����ָ���׵�ַ

	CHAR SIZEOF_DWORD;
	CAutoCSec m_Lock;

public:
	CBaseLink();
	bool init(const DWORD &ndCount,const DWORD &version);
	bool push(const type* const nd);
	type* top();
	bool pop();
	DWORD size();
	DWORD GetLength();
	void Clear();

	bool Delete(const type &nd);
	bool Find(const type &nd);

	type* const Next(const type * const nd);
	~CBaseLink();

private:
	bool Delete_(const type &nd);
};

template<class type>
class CHashTable
{
protected:
	typedef struct HashNode_{
		type node;
		HashNode_ *next;
		}HashNode;

protected:
	DWORD m_dwVersion;	//�汾

	DWORD m_TableLength;//hash����
	DWORD m_NodeLinkLength;//���ڵ�������

	DWORD SIZEOF_DWORD;
	DWORD SIZEOF_NODE;
	DWORD *m_pbuf;//hash�����׵�ַ
	CAutoCSec m_Lock;
	DWORD m_nNodeCounter;
	
	CMemoryPool<HashNode> *m_mempool;
	inline void CreateIndex(LPVOID szdata, const DWORD &nDataLen,DWORD &nIndex);
	inline void CreateIndex(const DWORD &szdata, DWORD &nIndex);

public:
	CHashTable();
	~CHashTable();
	bool init(DWORD dwTableLen, DWORD dwVersion);
	DWORD size();
	DWORD GetItemCount();
	bool Find(LPVOID szdata, const DWORD &nDataLen,const type &nd);
	bool Find(const DWORD &szdata, const type &nd);
	bool Add(LPVOID szdata, const DWORD &nDataLen,const type &nd);
	bool Add(const DWORD &szdata, const type &nd);
	bool get(LPVOID szdata, const DWORD &nDataLen,type *nd);
	bool get(const DWORD &szdata, type *nd);
	void Clear();
};


#include "CBaseDataStructure.inl"