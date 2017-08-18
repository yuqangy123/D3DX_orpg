#pragma once
#include "common1.h"

#define UP_SLOPE			0.9
#define DOWN_SLOPE	-0.8

// 
// typedef enum {noEvent=0, mouseEvent, keyboardEvent} MoveEventTP;
// 
// typedef enum {no_OP=0, down_W, down_S, down_A, down_D, 
// hold_W, hold_S, hold_A, hold_D}UOperate;
// 
// typedef enum {run=0, wait, attack} SkinMeshState;

typedef enum {wait=0, run_front, run_back, run_left, run_right, walk, attack} SkinMeshState;

typedef enum {no_OP=0, down_W, down_S, down_A, down_D, 
hold_W, hold_S, hold_A, hold_D,
mouse_leftClick,mouse_rightClick}UOperate;

class CBaseObject
{
public:
	CBaseObject(void);
	virtual ~CBaseObject(void);
	virtual HRESULT Init()=0;
	void LoadResFile();
	virtual void Transform();
	void GetBoundMaxV(D3DXVECTOR3 &v){v=m_vMax;};
	void GetBoundMinV(D3DXVECTOR3 &v){v=m_vMin;};
	D3DXMATRIX GetOrigWorldMat(){return m_matOrigWorld;};
	D3DXMATRIX GetWorldMat(){return m_matWorld;};

protected:
	D3DXMATRIX m_matWorld;//����
	D3DXMATRIX m_matOrigWorld;//����
	D3DXVECTOR3 m_vPos;//ƽ��
	D3DXVECTOR3 m_vRot;//��ת
	D3DXVECTOR3 m_vScal;//����

	D3DXVECTOR3 m_vMatWorldPos;//�����������е�ֵ

	D3DXVECTOR3 m_vOrigMax;    // ԭʼ��Χ�н�ֵ
	D3DXVECTOR3 m_vOrigMin;
	D3DXVECTOR3 m_vMax;        // ��Χ�н�ֵ
	D3DXVECTOR3 m_vMin;
	D3DXVECTOR3 m_vCenter;     
	float					m_fRaius;
};
