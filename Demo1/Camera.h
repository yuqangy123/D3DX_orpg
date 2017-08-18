#pragma once
#include "common1.h"
#include "Scene.h"
#include "Input.h"

class CScene;
class CCamera
{
public:
	CCamera(IDirect3DDevice9* p);
	~CCamera(void);
	HRESULT InitCamera(CScene *pScene, CInput *pInput, int nWidth, int nHeight);
	void Update();
	void GetViewAndProjMat(D3DXMATRIX &matView, D3DXMATRIX &matProj);
	void GetPosVtr(D3DXVECTOR3 &pos){pos=m_vEye;};
	void GetLookAtVtr(D3DXVECTOR3 &vLook){vLook=m_vLookat;};
	void GetCameraDistance(float &fDist){fDist = m_R;}
	void SetLookAtVtr(D3DXVECTOR3 &vLook){m_vLookat=vLook;}
	void SetCameraDistance(const float &fDist);
	void AdjustCamera();

protected:
	void AdjustAngle(float fX, float fZ);

private:
	CScene		*m_pScene;
	IDirect3DDevice9* m_pDevice;
	CInput		*m_pInput;
	D3DXVECTOR3		m_vEye;
	D3DXVECTOR3		m_vLookat;
	D3DXVECTOR3		m_vUp;
	D3DXMATRIX m_matView, m_matViewTest;//��ͼ�任���󣬴���������任����ͼ����
	D3DXMATRIX m_matProj, m_matProjTest;//ͶӰ����
	float		m_far;
	float		m_R;
	float		m_ScalR;
	float		m_MaxR, m_MinR;
	float		m_fovy;// ��ͷ��ֱ�۲췶Χ, 
	float		m_MoveSpeed;//�ƶ��ٶ�
	float		m_fPitch;//���ӽ�
	float		m_fYaw;//���ӽ�
	float		m_fAspect;//�߿��
	bool		m_bTest;
};
