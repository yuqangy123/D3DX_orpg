#pragma once
#include "common1.h"
#include "dsound.h"
#pragma comment(lib, "dsound.lib")

class CSound;
class CWaveSoundRead
{	
public:
	CWaveSoundRead();
	~CWaveSoundRead();
	
	HRESULT Open(LPWSTR pFileName);
	HRESULT Reset();
	HRESULT Read(UINT nSizeToRead, BYTE *pData, UINT *pnSizeRead);
	HRESULT Close();

protected:
	HRESULT ReadMMIO(HMMIO hMmioIn, MMCKINFO *pckInRIFF, WAVEFORMATEX **ppWfxInfo);
	HRESULT WaveOpenFile(LPWSTR pFileName, HMMIO *phMmioIn, WAVEFORMATEX **ppWfxInfo, MMCKINFO *pckInRIFF);
	HRESULT WaveStartDataRead(HMMIO *phMmioIn, MMCKINFO *pckIn, MMCKINFO *pckInRIFF);
	HRESULT WaveReadFile(HMMIO hMmioIn, UINT nRead, BYTE *pDest, MMCKINFO *pckIn, UINT *pActualRead);

private:
	WAVEFORMATEX			*m_pWfx;
	HMMIO							m_hMmioIn;
	MMCKINFO					m_ckIn;
	MMCKINFO					m_ckInRiff;

protected:
	friend class CSound;
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CSoundLayer
{
public:
	static CSoundLayer* Inistance();
	static void CloseInstance();
	static HRESULT CreateInstance(HWND hWnd);
	LPDIRECTSOUND8 GetDSound();

private:
	CSoundLayer(HWND);
	~CSoundLayer();
	LPDIRECTSOUND8					m_pDSound;
	LPDIRECTSOUNDBUFFER8		m_pPrimary;
	HWND								m_hWnd;
	static CSoundLayer			*m_pOwner;
	bool									m_bEnable;
	
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CSound
{
public:
	CSound(LPWSTR pFilename);
	CSound(const CSound&);
	virtual ~CSound();
	void Restore();
	bool Play(bool bLoop=false);
	bool IsPlay();

private:
	void init();
	void Fill();
	CWaveSoundRead			*m_pWaveSoundRead;
	LPDIRECTSOUNDBUFFER8			m_pBuf;
	int			m_nBufsize;
	static		map_t<CWaveSoundRead*, int>			m_WaveMap;
};

template<typename Type>
class sAutoZero:public Type
{
public:
	sAutoZero();
};