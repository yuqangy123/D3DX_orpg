#pragma once
#include "BaseObject.h"
#include "SkinMesh.h"
 #include "AnimInstance.h"
#include "Terrain.h"
#include "Input.h"
#include "Camera.h"
#include "Pet.h"

class CCamera;
class CTerrain;
class CSkinMesh;
class CAnimInstance;
class CInput;
class CPet;
class CMajorObject:public CBaseObject
{
public:
	CMajorObject();
	~CMajorObject(void);
	void Update(double nFpsTime, D3DXVECTOR3 &vDir);
	void Render();
	HRESULT Init();
	D3DXVECTOR3 GetPosOfWorld(){return m_vPos;}
	D3DXVECTOR3 GetDir(){return m_vDir;}

protected:
	void OnUpdateWorldPos();
	void OnUpdateSkinMesh();
	void adjustCamera();
	bool CollisionTest(const D3DXVECTOR3 &vDestPos);

private:
	CPet						*m_pPet;
	CCamera				*m_pCamera;
	CTerrain					*m_pTerrain;
	CInput					*m_pInput;
	IDirect3DDevice9 *m_pDevice;
	CSkinMesh			*m_pSkinMesh;
	CAnimInstance		*m_pAI;
	DOUBLE					m_dElapsedTime;  // 每帧时间
	float						m_fRunMoveSpeed, m_fWalkMoveSpeed;//移动速度
	D3DXVECTOR3		m_vDir;//方向
	D3DXVECTOR3		m_vOrigRot;//模型原始旋转方向，与-z轴为参照，顺时针旋转
	D3DXVECTOR3		m_vWalkPos;
	D3DXVECTOR3		m_vMeshCenterOfWorld;
	
	SkinMeshState		m_MajorState;
	SkinMeshState		m_OldMajorState;
	UOperate				m_UP;	
};
