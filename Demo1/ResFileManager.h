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
	string_t GetMeshFilePath();//模型资源路径
	string_t GetTextureFilePath();//纹理资源路径
	string_t GetTerrainTextureFilePath();//地形纹理
	string_t GetTerrainHeightMapFilePath();//地形深度图
	void GetTerrainAttribute(int &nRow, int &nCol, int &nCellSpace, float &nHeightScale);//地形属性值
	void GetLods(float &fLod);
	
	astring_t GetMajorSkinMeshFile();//猪脚mesh&texture&属性
	astring_t GetMajorTextureFile();//
	void GetMajorAttribute(D3DXVECTOR3 &vPos, D3DXVECTOR3 &vRot, D3DXVECTOR3 &vScal,
		double &dElapsedTime,  float &fRunMoveSpeed, float &fWalkMoveSpeed);

	astring_t GetPetSkinMeshFile();//宠物mesh&texture&attribute
	astring_t GetPetTextureFile();
	void GetPetAttribute(D3DXVECTOR3 &vPos, D3DXVECTOR3 &vRot, D3DXVECTOR3 &vScal,
		double &dElapsedTime, float &fMoveSpeed, float &fDistanceWidthMajor);
	

	void GetCameraAttribute(float &fPitch, float &fYaw, float &fOvy, 
		float &fFar, float &fR, float &fMaxR, float &fMinR, float &fScalSpeed,
		D3DXVECTOR3 &vUp, D3DXVECTOR3 &vLookat);//摄像头属性

	astring_t GetSkyMeshFile();//获取天空的mesh&texture&属性
	astring_t GetSkyTextureFile();//
	void GetSkyAttribute(D3DXVECTOR3 &vPos, D3DXVECTOR3 &vRot, D3DXVECTOR3 &vScal);

	//粒子：雪
	string_t GetSnowTextureFile();

private:
	static CResFileManager		*m_pMyself;
};
