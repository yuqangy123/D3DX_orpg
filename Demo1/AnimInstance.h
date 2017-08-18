#pragma once
#include "common1.h"
#include "SkinMesh.h"

class CAnimInstance
{
private:
	ID3DXAnimationController*   m_pAC;                    // ����������
	DWORD                       m_dwCurrentTrack;         // ��ǰ������
	char                        m_strCurrentAnim[32];     // ��ǰ��������
	char                        m_strNextAnim[32];        // ��һ����������
	DWORD                       m_dwAnimIndex;            // ��������
	DOUBLE                      m_dTimeMax;               // ��󶯻�ʱ��
	DOUBLE                      m_dTimeCurrent;           // ��ǰ����ʱ��
	DOUBLE						m_timeDelta;              // ÿ�ζ����ƽ�ʱ��
	bool						m_bMoving;                // �������ſ���

public:
	CAnimInstance(void);
	~CAnimInstance(void);

	void Init(CSkinMesh* pSkinMesh);
	void Render();
	// �����л�
	void ChangeTrack(bool bResetPosition);
	// ���ö���������
	void SetAnimIndex(DWORD Index) {m_dwAnimIndex = Index;}
	// ���ö���������
	void SetAnimName(char* strName);
	// ���ö���
	void SetAnimTrack();
	// ���ö����ƽ�ʱ��
	void SetTimeDelta(float fTimeDelta) {m_timeDelta = fTimeDelta;}
	void SetMoving(bool bMoving) {m_bMoving = bMoving;}
	void SetTimeMax(double dTimeMax) {m_dTimeMax = dTimeMax;}

	char* GetAnimName() {return m_strCurrentAnim;}
	DOUBLE GetTimeMax() {return m_dTimeMax;}
	DOUBLE GetTimeDelta() {return m_timeDelta;}
	bool GetMoving() {return m_bMoving;}
};
