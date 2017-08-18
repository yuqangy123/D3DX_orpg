#pragma once
#include <vector>
#include "common1.h"
#include "MyFile.h"
#include "base_type.h"

class CResFileManager
{
public:
	CResFileManager(void);
	~CResFileManager(void);
	static CResFileManager* Instance(){NewAndReturn(m_pMyself, CResFileManager)};
	static void DestoryInstance(){DeleteAndNull(m_pMyself)};
	
	std::vector<string_t> GetMeshFile();
	string_t GetMeshFilePath();//ģ����Դ·��
	string_t GetTextureFilePath();//������Դ·��
	string_t GetTerrainTextureFilePath();//��������
	string_t GetTerrainHeightMapFilePath();//�������ͼ
	void GetTerrainAttribute(int &nRow, int &nCol, int &nCellSpace, float &nHeightScale);//��������ֵ
	void GetLods(float &fLod);
	
	astring_t GetMajorSkinMeshFile();//���mesh&texture&����
	astring_t GetMajorTextureFile();//
	void GetMajorAttribute(D3DXVECTOR3 &vPos, D3DXVECTOR3 &vRot, D3DXVECTOR3 &vScal,
		double &dElapsedTime,  float &fRunMoveSpeed, float &fWalkMoveSpeed);

	astring_t GetPetSkinMeshFile();//����mesh&texture&attribute
	astring_t GetPetTextureFile();
	void GetPetAttribute(D3DXVECTOR3 &vPos, D3DXVECTOR3 &vRot, D3DXVECTOR3 &vScal,
		double &dElapsedTime, float &fMoveSpeed, float &fDistanceWidthMajor);
	

	void GetCameraAttribute(float &fPitch, float &fYaw, float &fOvy, 
		float &fFar, float &fR, float &fMaxR, float &fMinR, float &fScalSpeed,
		D3DXVECTOR3 &vUp, D3DXVECTOR3 &vLookat);//����ͷ����

	astring_t GetSkyMeshFile();//��ȡ��յ�mesh&texture&����
	astring_t GetSkyTextureFile();//
	void GetSkyAttribute(D3DXVECTOR3 &vPos, D3DXVECTOR3 &vRot, D3DXVECTOR3 &vScal);

	//���ӣ�ѩ
	string_t GetSnowTextureFile();

private:
	static CResFileManager		*m_pMyself;
};
