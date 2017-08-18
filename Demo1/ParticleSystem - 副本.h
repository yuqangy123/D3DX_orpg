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
	virtual void InitRenderStatus();//初始化渲染状态
	virtual void Render();
	virtual void postRenderStatus();
	bool isEmpty(){return false;};
	bool isDead(){return false;};

protected:
//	virtual void removeDeadP();

protected:
	IDirect3DDevice9			*m_pDevice;
	D3DXVECTOR3				m_vOrigin;
	BoundingBox				m_BoundBox;//活动范围
	float							   m_emitRate;//速度
	float						       m_size;//所以粒子大小
	IDirect3DTexture9*    m_pTexture;
	IDirect3DVertexBuffer9* m_pVB;
	list_t<AttributeP>		    m_particles;//
//	int								   m_maxParticles;//给定的时间内，系统中允许的粒子最大数

	DWORD m_vbSize;//给定的时间内顶点缓存中能够保存的粒子的数量
	DWORD m_vbOffset;//在顶点缓存中的偏移，在顶点缓存里我们将用它开始COPY下一批粒子
	DWORD m_vbBatchSize;//定义在一批缓存中的粒子数量
};

class CParticleSnow : public CBaseParticleSystem
{
public:
	CParticleSnow(IDirect3DDevice9 *);
	~CParticleSnow(void);
	virtual void resetP(AttributeP &resetAttr);//重新设置粒子
	virtual void Update(float timeDelta);
	void SetDefAttrubute(const AttributeP &DefA);
	HRESULT Init(const string_t &TextFile);

private:
	AttributeP			m_DefAttribute;
};
