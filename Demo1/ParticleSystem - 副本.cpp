#include "StdAfx.h"
#include "ParticleSystem.h"
#include "DXDevice.h"

CBaseParticleSystem::CBaseParticleSystem(IDirect3DDevice9 *pDevice):
m_pDevice(pDevice)
{

}

CBaseParticleSystem::~CBaseParticleSystem()
{

}

HRESULT CBaseParticleSystem::Init(const string_t &TextFile)
{
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(m_pDevice, TextFile.c_str(), &m_pTexture);
	if(FAILED(hr))
		return E_FAIL;
	
	hr = m_pDevice->CreateVertexBuffer( m_vbSize*sizeof(Particle), 
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,//用法
		PARTICLE_FVF,
		D3DPOOL_DEFAULT,&m_pVB, NULL);
	if(FAILED(hr))
		return E_FAIL;

	//初始化成员变量
	
	return S_OK;
}

void CBaseParticleSystem::reset()
{
	for (list_t<AttributeP>::iterator Itr = m_particles.begin(); Itr != m_particles.end(); ++Itr)
	{
		resetP(*Itr);
	}	
}

void CBaseParticleSystem::addP()
{
	AttributeP PP;
	resetP(PP);
	m_particles.push_back(PP);
}

void CBaseParticleSystem::InitRenderStatus()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pDevice->SetRenderState(D3DRS_POINTSIZE, 2);//表示点精灵的尺寸. 这个值可以任意指定视图空间或屏幕空间的点精灵的尺寸, 取决于D3DRS_POINTSCALEENABLE 状态如何设置
	m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, 0);

	// control the size of the particle relative to distance
	//如何根据距离控制点精灵的尺寸—这个距离是点精灵到照相机的距离
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_A, 0);
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_B, 0);
	m_pDevice->SetRenderState(D3DRS_POINTSCALE_C, 1);

	// use alpha from texture
	//take alpha from alpha channel
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);//D3DTA_DIFFUSE从顶点获得alpha，D3DTA_TEXTURE从纹理获得
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//alpha混合渲染，用于设置透明度
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CBaseParticleSystem::postRenderStatus()
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING,          true);
	m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE,  false);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,  false);
}


void CBaseParticleSystem::Render()
{
	if(!m_particles.empty())
	{
		InitRenderStatus();

		m_pDevice->SetTexture(0, m_pTexture);
		m_pDevice->SetFVF(PARTICLE_FVF);
		m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(Particle));

		if(m_vbOffset >= m_vbSize)//偏移循环
			m_vbOffset=0;

		/*
D3DLOCK_DISCARD——这个参数仅仅会在动态缓存时被使用。它指示硬件丢弃缓存并返回一个指向新分配的缓存的指针。这是很有用，因为当我们存取一个新分配的缓存时它允许硬件继续从丢弃的缓存渲染。这防止了硬件延迟。
D3DLOCK_NOOVERWRITE——这个参数仅仅会在动态缓存时被使用。它声明你将向缓存中添加数据。即，你不能向已经渲染的内存中写数据。这是有好处的因为他允许你在添加新数据到缓存的同时让硬件继续渲染。
用参数D3DLOCK_DISCARD和D3DLOCK_NOOVERWRITE的地址实际上就是缓存的一部分被使用（正在渲染）时它被锁定。假如情况允许这些标记被使用，当在锁定时他们防止渲染停止
		*/
		Particle *v;
		m_pVB->Lock(
			m_vbOffset    * sizeof( Particle ),
			m_vbBatchSize * sizeof( Particle ),
			(void**)&v,
			m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD numParticlesInBatch=0;
		for (list_t<AttributeP>::iterator Itr = m_particles.begin(); Itr != m_particles.end(); ++Itr)
		{
			if(Itr->_isAlive)
			{
				v->_position = Itr->_position;
				v->_color = 0xffffffff;
				v->_size = m_size;
				++v;

				++numParticlesInBatch;
				if(numParticlesInBatch == m_vbBatchSize)
				{
					m_pVB->Unlock();
					m_pDevice->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, m_vbBatchSize);
					m_vbOffset+=m_vbBatchSize;
					if(m_vbOffset >= m_vbSize)
						m_vbOffset=0;
					numParticlesInBatch = 0;

					m_pVB->Lock(
						m_vbOffset    * sizeof( Particle ),
						m_vbBatchSize * sizeof( Particle ),
						(void**)&v,
						m_vbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
				}
			}
		}
		m_pVB->Unlock();
		if(numParticlesInBatch)
			m_pDevice->DrawPrimitive(D3DPT_POINTLIST, m_vbOffset, numParticlesInBatch);
	m_vbOffset+=m_vbBatchSize;
	}

	postRenderStatus();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

CParticleSnow::CParticleSnow(IDirect3DDevice9 *p):
CBaseParticleSystem(p)
{
	
}

CParticleSnow::~CParticleSnow(void)
{
}

void CParticleSnow::resetP(AttributeP &resetAttr)
{
	resetAttr._position = RandomVectorFormBoundBox(m_BoundBox);
	resetAttr._velocity = D3DXVECTOR3(0,1.0f,0);
	ZeroVector3(resetAttr._acceleration);
	resetAttr._vDir=D3DXVECTOR3(0,-1.0f,0);
	resetAttr._lifeTime=.0f;
	resetAttr._age=.0f;
	resetAttr._colorAlive=0xffffff00;
	resetAttr._isAlive=false;


	resetAttr._position.x += -(resetAttr._vDir.x*(abs(m_BoundBox._max.x-m_BoundBox._min.x)));
	resetAttr._position.y += -(resetAttr._vDir.y*(abs(m_BoundBox._max.y-m_BoundBox._min.y)));
	resetAttr._position.z += -(resetAttr._vDir.z*(abs(m_BoundBox._max.z-m_BoundBox._min.z)));
}

void CParticleSnow::Update(float timeDelta)
{
	for (list_t<AttributeP>::iterator Itr = m_particles.begin(); Itr != m_particles.end(); ++Itr)
	{
		Itr->_position.x += (Itr->_velocity.x*Itr->_vDir.x);
		Itr->_position.y += (Itr->_velocity.y*Itr->_vDir.y);
		Itr->_position.z += (Itr->_velocity.z*Itr->_vDir.z);
		if(!m_BoundBox.isPointInside(Itr->_position)){
			Itr->_isAlive=false;
			if(Itr->_position.y <= m_BoundBox._min.y )
			resetP(*Itr);
		}
		else{
			Itr->_isAlive=true;
		}
	}
}

void CParticleSnow::SetDefAttrubute(const AttributeP &DefA)
{
	m_DefAttribute = DefA;
}

HRESULT CParticleSnow::Init(const string_t &TextFile)
{
	HRESULT hr;
	hr = CBaseParticleSystem::Init(TextFile);
	if(FAILED(hr))
		return E_FAIL;

	CDXDevice::Instance()->GetCamera()->GetLookAtVtr(m_vOrigin);
	m_vOrigin.y=CDXDevice::Instance()->GetScene()->GetCurTerrain()->GetTHeight(m_vOrigin.x, m_vOrigin.z);
	
	m_BoundBox._max.x=-200.0f;m_BoundBox._max.z=200.0f;m_BoundBox._max.y=m_vOrigin.y+400.0f;
	m_BoundBox._min.x=200.0f;m_BoundBox._min.z=-200.0f;m_BoundBox._min.y=m_vOrigin.y;

	m_emitRate = 0.5f;
	m_size = 2.0f;	

	m_vbSize = 200;//粒子总数
	m_vbOffset=0;
	m_vbBatchSize=50;
	for (int nP=0; nP < m_vbSize; ++nP)
		addP();

	return S_OK;
}