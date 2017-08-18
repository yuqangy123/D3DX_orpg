#pragma once
#include "common1.h"

class CBaseParticleSystem
{
public:
	CBaseParticleSystem(IDirect3DDevice9 *);
	virtual ~CBaseParticleSystem();
	virtual HRESULT Init(const string_t &TextFile);
	virtual void reset();
	virtual void resetP(AttributeP &resetAttr)=0;
	virtual void addP();
	virtual void Update(float timeDelta)=0;
	virtual void InitRenderStatus();//��ʼ����Ⱦ״̬
	virtual void Render();
	virtual void postRenderStatus();
	bool isEmpty(){return false;};
	bool isDead(){return false;};

protected:
//	virtual void removeDeadP();

protected:
	IDirect3DDevice9			*m_pDevice;
	D3DXVECTOR3				m_vOrigin;
	BoundingBox				m_BoundBox;//���Χ
	float							   m_emitRate;//�ٶ�
	float						       m_size;//�������Ӵ�С
	IDirect3DTexture9*    m_pTexture;
	IDirect3DVertexBuffer9* m_pVB;
	list_t<AttributeP>		    m_particles;//
//	int								   m_maxParticles;//������ʱ���ڣ�ϵͳ����������������

	DWORD m_vbSize;//������ʱ���ڶ��㻺�����ܹ���������ӵ�����
	DWORD m_vbOffset;//�ڶ��㻺���е�ƫ�ƣ��ڶ��㻺�������ǽ�������ʼCOPY��һ������
	DWORD m_vbBatchSize;//������һ�������е���������
};

class CParticleSnow : public CBaseParticleSystem
{
public:
	CParticleSnow(IDirect3DDevice9 *);
	~CParticleSnow(void);
	virtual void resetP(AttributeP &resetAttr);//������������
	virtual void Update(float timeDelta);
	void SetDefAttrubute(const AttributeP &DefA);
	HRESULT Init(const string_t &TextFile);

private:
	AttributeP			m_DefAttribute;
};
