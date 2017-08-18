#include "StdAfx.h"
#include "ResFileManager.h"
//#include "MyFile.h"

CResFileManager* CResFileManager::m_pMyself=NULL;
CResFileManager::CResFileManager(void)
{
}

CResFileManager::~CResFileManager(void)
{
}

std::vector<string_t> CResFileManager::GetMeshFile()
{
	return std::vector<string_t>();
}

string_t CResFileManager::GetMeshFilePath()
{
	string_t filepath=CYFile::Instance()->GetAppFilePath();
	filepath.append(_T("mesh"));
	return filepath;
}

string_t CResFileManager::GetTextureFilePath()
{
	string_t AppFilePath = CYFile::Instance()->GetAppFilePath();
	AppFilePath.append(_T("texture"));
	return AppFilePath;
}

string_t CResFileManager::GetTerrainTextureFilePath()//地形纹理资源
{
	string_t AppFilePath = CYFile::Instance()->GetAppFilePath();
	AppFilePath.append(_T("scene\\terrain\\terrain_texture.jpg"));
	return AppFilePath;	
}

string_t CResFileManager::GetTerrainHeightMapFilePath()//地形深度图
{
	string_t AppFilePath = CYFile::Instance()->GetAppFilePath();
	AppFilePath.append(_T("scene\\terrain\\terrain_heightmap.bmp"));
	return AppFilePath;		
}

void CResFileManager::GetTerrainAttribute(int &nRow, int &nCol, int &nCellSpace, float &nHeightScale)//地形属性值
{
	nRow=128;//行方格个数
	nCol=128;
	nCellSpace=75;
	nHeightScale=3.0;
}

void CResFileManager::GetLods(float &fLod)
{
	fLod = 0.0005;
}

astring_t CResFileManager::GetMajorSkinMeshFile()
{
	return "mesh\\skinmesh\\major_mesh.x";
}

astring_t CResFileManager::GetMajorTextureFile()
{
	return "texture\\";
}

void CResFileManager::GetMajorAttribute(D3DXVECTOR3 &vPos, D3DXVECTOR3 &vRot, D3DXVECTOR3 &vScal,  double &dElapsedTime,float &fRunMoveSpeed, float &fWalkMoveSpeed)
{
	vPos=D3DXVECTOR3(0,0,0);
	vRot=D3DXVECTOR3(0,0,0);
	vScal=D3DXVECTOR3(1,1,1);
	dElapsedTime=0.006;//越小越慢
	fRunMoveSpeed=200.0f;
	fWalkMoveSpeed=100.0f;
};

void CResFileManager::GetCameraAttribute(float &fPitch, float &fYaw, float &fOvy, 
						float &fFar, float &fR, float &fMaxR, float &fMinR, float &fScalSpeed,
						D3DXVECTOR3 &vUp, D3DXVECTOR3 &vLookat)
{
	fPitch = D3DX_PI/4;
	fYaw = 0.0f;
	fOvy = D3DX_PI/4.0f;
	fFar = 7000.0f;
	fR = 300.0f;
	fMaxR = 500.0f;
	fMinR = 50.0f;
	fScalSpeed = 20.0f;
	vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vLookat = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

astring_t CResFileManager::GetSkyMeshFile()
{
	astring_t strMeshFile = "mesh\\sky_mesh.x";
	return strMeshFile;
}

astring_t CResFileManager::GetSkyTextureFile()
{
	return "texture\\";
}

void CResFileManager::GetSkyAttribute(D3DXVECTOR3 &vPos, D3DXVECTOR3 &vRot, D3DXVECTOR3 &vScal)
{
	vPos=D3DXVECTOR3(0,0,0);
	vRot=D3DXVECTOR3(0,0,0);
	vScal=D3DXVECTOR3(20,20,20);//覆盖面积
}

string_t CResFileManager::GetSnowTextureFile()
{
	string_t AppFilePath = CYFile::Instance()->GetAppFilePath();
	AppFilePath.append(_T("texture\\snowflake.dds"));
	return AppFilePath;		
}

astring_t CResFileManager::GetPetSkinMeshFile()
{
	return "mesh\\skinmesh\\pet_mesh.x";
}

astring_t CResFileManager::GetPetTextureFile()
{
	return "texture\\";
}

void CResFileManager::GetPetAttribute(D3DXVECTOR3 &vPos, D3DXVECTOR3 &vRot, D3DXVECTOR3 &vScal, double &dElapsedTime, float &fMoveSpeed, float &fDistanceWidthMajor)
{
	vPos=D3DXVECTOR3(0,0,0);
	vRot=D3DXVECTOR3(-1.57f,1.57f,0.0f);
	vScal=D3DXVECTOR3(10,10,10);
	dElapsedTime=0.006;//越小越慢
	fMoveSpeed=90.f;
	fDistanceWidthMajor = 30.0f;
};
