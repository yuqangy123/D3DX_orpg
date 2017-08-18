#include "StdAfx.h"
#include "Sound.h"

CWaveSoundRead::CWaveSoundRead()
{
	m_pWfx=NULL;
	memset(&m_ckIn, 0x0, sizeof(m_ckIn));
}

CWaveSoundRead::~CWaveSoundRead()
{
	Close();
	DeleteAndNull(m_pWfx);
}

//open a wave file for reading
HRESULT CWaveSoundRead::Open(LPWSTR pFileName)
{
	DeleteAndNull(m_pWfx);
	HRESULT hr;
	hr = WaveOpenFile(pFileName, &m_hMmioIn, &m_pWfx, &m_ckInRiff);
	if(FAILED(hr))
		return E_FAIL;

	hr = Reset();
	if(FAILED(hr))
		return E_FAIL;

	return S_OK;
}

//resets the internal m_ckIn pointer so reading starts from the beginning of the file again
HRESULT CWaveSoundRead::Reset()
{
	return WaveStartDataRead(&m_hMmioIn, &m_ckIn, &m_ckInRiff);
}

//reads a wave file into a pointer and returns how much read using m_ckIn to determine where to start reading from
HRESULT CWaveSoundRead::Read(UINT nSizeToRead, BYTE *pData, UINT *pnSizeRead)
{
	return WaveReadFile(m_hMmioIn, nSizeToRead, pData, &m_ckIn, pnSizeRead);
}

HRESULT CWaveSoundRead::Close()
{
	mmioClose(m_hMmioIn,0);
	return S_OK;
}

//read wave data from the wave file. make sure we're   into the data chunk before calling this function
HRESULT CWaveSoundRead::WaveReadFile(HMMIO hMmioIn, UINT nRead, BYTE *pDest, MMCKINFO *pckIn, UINT *pActualRead)
{
	MMIOINFO	mmioInfoIn;
	*pActualRead=0;
	if( S_OK != mmioGetInfo(hMmioIn, &mmioInfoIn, 0) )
		return E_FAIL;
	
	UINT nDataIn = nRead;
	if(nDataIn > pckIn->cksize)
		nDataIn = pckIn->cksize;

	for (DWORD dwT = 0; dwT < nDataIn; ++dwT)
	{
		//copy the bytes from the io to the buffer
		if(mmioInfoIn.pchNext == mmioInfoIn.pchEndRead)
		{
			if( S_OK != mmioAdvance(hMmioIn, &mmioInfoIn, MMIO_READ))
				return E_FAIL;

			if(mmioInfoIn.pchNext == mmioInfoIn.pchEndRead)
				return E_FAIL;
		}
		//actual copy
		*((BYTE*)pDest+dwT) = *((BYTE*)mmioInfoIn.pchNext);
		++mmioInfoIn.pchNext;
	}
	if (S_OK != mmioSetInfo(hMmioIn, &mmioInfoIn, 0))
		return E_FAIL;

	*pActualRead = nDataIn;
	return S_OK;
}

//before wavereadfile called
HRESULT CWaveSoundRead::WaveStartDataRead(HMMIO *phMmioIn, MMCKINFO *pckIn, MMCKINFO *pckInRIFF)
{
	//seek to the data
	if(-1 == mmioSeek(*phMmioIn, pckInRIFF->dwDataOffset + sizeof(FOURCC), SEEK_SET))
		return E_FAIL;

	//search the input file for the 'data' chunk, data
//	memcpy(pckIn, pckInRIFF, sizeof(MMCKINFO));
	pckIn->ckid = mmioFOURCC('d', 'a', 't', 'a');
	
	if(S_OK != mmioDescend(*phMmioIn, pckIn, pckInRIFF, MMIO_FINDCHUNK))
		return E_FAIL;

	return S_OK;
}

//this func will open wave input file and prepare it for reading
HRESULT CWaveSoundRead::WaveOpenFile(LPWSTR pFileName, HMMIO *phMmioIn, WAVEFORMATEX **ppWfxInfo, MMCKINFO *pckInRIFF)
{
	HRESULT hr;
	HMMIO hmmioin = NULL;

	if(NULL == (hmmioin = mmioOpen(pFileName, NULL, MMIO_ALLOCBUF | MMIO_READ)))
		return E_FAIL;

	if(FAILED(hr = ReadMMIO(hmmioin, pckInRIFF, ppWfxInfo)))
	{
		mmioClose(hmmioin, 0);
		return hr;
	}

	*phMmioIn = hmmioin;
	return S_OK;
}

//support func for reading from a multimedia i/o stream
HRESULT CWaveSoundRead::ReadMMIO(HMMIO hMmioIn, MMCKINFO *pckInRIFF, WAVEFORMATEX **ppWfxInfo)
{
	MMCKINFO ckin;
	PCMWAVEFORMAT pcmwaveformat;
	*ppWfxInfo=NULL;

	if(( S_OK != mmioDescend(hMmioIn, pckInRIFF, NULL, 0)))
		return E_FAIL;

	if ((pckInRIFF->ckid != FOURCC_RIFF) ||
		(pckInRIFF->fccType != mmioFOURCC('W', 'A', 'V', 'E')))
		return E_FAIL;

	//search the input file for the 'fmt' chunk
	ckin.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if(S_OK != mmioDescend(hMmioIn, &ckin, pckInRIFF, MMIO_FINDCHUNK))
		return E_FAIL;

	//expect the 'fmt' chunk to be at least as large sa <PCMWAVEFORMAT>
	//if hte are extra parmeters at hte end, we'll ignore them
	if (ckin.cksize < (LONG)sizeof(PCMWAVEFORMAT))
		return E_FAIL;

	//read the 'fmt' chunk into <pcmwaveformat>
	if(mmioRead(hMmioIn, (HPSTR)&pcmwaveformat, sizeof(pcmwaveformat)) != sizeof(pcmwaveformat))
		return E_FAIL;

	if(WAVE_FORMAT_PCM == pcmwaveformat.wf.wFormatTag)
	{
		if(NULL == (*ppWfxInfo = new WAVEFORMATEX))
			return E_FAIL;

		memcpy(*ppWfxInfo, &pcmwaveformat, sizeof(pcmwaveformat));
		(*ppWfxInfo)->cbSize = 0;
	}
	else
	{
		WORD cbExtraBytes = 0L;
		if (mmioRead(hMmioIn, (char*)&cbExtraBytes, sizeof(WORD)) != sizeof(WORD))
			return E_FAIL;

		*ppWfxInfo = (WAVEFORMATEX*)new char[sizeof(WAVEFORMATEX)+cbExtraBytes];
		if(!ppWfxInfo)
			return E_FAIL;
		memcpy(*ppWfxInfo, &pcmwaveformat, sizeof(pcmwaveformat));
		(*ppWfxInfo)->cbSize = cbExtraBytes;

		if(cbExtraBytes != mmioRead(hMmioIn, (char*)((BYTE*)&((*ppWfxInfo)->cbSize) + sizeof(WORD)), cbExtraBytes))
		{
			DeleteAndNull(ppWfxInfo);
			*ppWfxInfo = NULL;
			return E_FAIL;
		}
	}

	if(S_OK != mmioAscend(hMmioIn, &ckin, 0))
	{
		DeleteAndNull(*ppWfxInfo);
		return E_FAIL;
	}
	return S_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


CSoundLayer* CSoundLayer::m_pOwner = NULL;
CSoundLayer::CSoundLayer(HWND hWnd):m_bEnable(false)
{
	m_pDSound=NULL;
	m_pPrimary=NULL;

	HRESULT hr;
	LPDIRECTSOUNDBUFFER pDSBPrimary=NULL;
	
	hr = DirectSoundCreate8(NULL, &m_pDSound, NULL);
	if(FAILED(hr))
		return ;

	hr=m_pDSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (FAILED(hr))
		return ;

	sAutoZero<DSBUFFERDESC> dsbd;
	dsbd.dwFlags=DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes=0;
	dsbd.lpwfxFormat=NULL;


	hr = m_pDSound->CreateSoundBuffer(&dsbd, &pDSBPrimary, NULL);
	if (FAILED(hr))
	{
		return ;
	}

	//基本格式设置为22khz, 16bit立体声的代码
	WAVEFORMATEX wfx;
	memset(&wfx, 0x0, sizeof(WAVEFORMATEX));
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 2;
	wfx.nSamplesPerSec = 11025*2;//22khz
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = wfx.wBitsPerSample/8*wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec*wfx.nBlockAlign;


	hr = pDSBPrimary->SetFormat(&wfx);
	if (FAILED(hr))
		return ;

	SafeRelease(pDSBPrimary);
	m_bEnable=true;
}

CSoundLayer::~CSoundLayer()
{
	SafeRelease(m_pPrimary);
	SafeRelease(m_pDSound);
	m_pOwner = NULL;
}

CSoundLayer* CSoundLayer::Inistance()
{
	return m_pOwner;
}

void CSoundLayer::CloseInstance()
{
	DeleteAndNull(m_pOwner);
}

HRESULT CSoundLayer::CreateInstance(HWND hWnd)
{
	if(m_pOwner)
		return E_FAIL;

	m_pOwner = new CSoundLayer(hWnd);
	if(!m_pOwner->m_bEnable){
		DeleteAndNull(m_pOwner);
		return E_FAIL;
	}

	return S_OK;
}

LPDIRECTSOUND8 CSoundLayer::GetDSound()
{
	return m_pDSound;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


map_t<CWaveSoundRead*, int>			CSound::m_WaveMap;
CSound::CSound(LPWSTR pFilename)
{
	m_pBuf=NULL;
	m_pWaveSoundRead = new CWaveSoundRead;
	m_WaveMap[m_pWaveSoundRead]=1;
	if (FAILED(m_pWaveSoundRead->Open(pFilename)))
	{
		m_WaveMap[m_pWaveSoundRead]=0;
		DeleteAndNull(m_pWaveSoundRead);
		return ;
	}

	init();
	Fill();
}

CSound::CSound(const CSound& inS)
{
	m_pWaveSoundRead = inS.m_pWaveSoundRead;
	++m_WaveMap[m_pWaveSoundRead];
	init();
	Fill();
}

CSound::~CSound()
{
	int nCount = m_WaveMap[m_pWaveSoundRead];
	if (1 == nCount)
	{
		DeleteAndNull(m_pWaveSoundRead);
	}
	else
	{

		m_WaveMap[m_pWaveSoundRead] = m_WaveMap[m_pWaveSoundRead]-1;
	}

}

void CSound::Restore()
{
	HRESULT hr;
	if(NULL == m_pBuf)
		return;

	DWORD dwStatus;
	hr = m_pBuf->GetStatus(&dwStatus);
	if(FAILED(hr))
		return;

	if (dwStatus & DSBSTATUS_BUFFERLOST)
	{
		do 
		{
			hr = m_pBuf->Restore();
			if (hr == DSERR_BUFFERLOST)
			{
				Sleep(10);
			}
		} while (hr == m_pBuf->Restore());

		Fill();
	}
}

void CSound::Fill()
{
	HRESULT hr;
	UCHAR *pWaveData;
	UINT nWaveSize;
	void *pData = NULL;
	void *pData2 = NULL;
	ULONG dwLength;
	ULONG dwLength2;
	UINT nWaveFileSize;
	
	nWaveFileSize = m_pWaveSoundRead->m_ckIn.cksize;
	pWaveData = new UCHAR[nWaveFileSize];
	
	hr = m_pWaveSoundRead->Read(nWaveFileSize, pWaveData, &nWaveSize);
	if (FAILED(hr)){
		DeleteAndNull(pWaveData);
		return ;
	}
	
	m_pWaveSoundRead->Reset();
	hr = m_pBuf->Lock(0, m_nBufsize, &pData, &dwLength, &pData2, &dwLength2, 0);
	if (FAILED(hr)){
		DeleteAndNull(pWaveData);
		return;
	}

	//copy data over, unlock after
	memcpy(pData, pWaveData, m_nBufsize);
	m_pBuf->Unlock(pData, m_nBufsize, NULL, 0);
	
	DeleteAndNull(pWaveData);
}

void CSound::init()
{
	HRESULT hr;
	sAutoZero<DSBUFFERDESC> dsbd;
	dsbd.dwFlags = DSBCAPS_STATIC;//静态缓冲和流缓冲
	dsbd.dwBufferBytes = m_pWaveSoundRead->m_ckIn.cksize;
	dsbd.lpwfxFormat = m_pWaveSoundRead->m_pWfx;

	LPDIRECTSOUNDBUFFER pTempBuffer = 0;
	hr = CSoundLayer::Inistance()->GetDSound()->CreateSoundBuffer(&dsbd, &pTempBuffer, NULL);
	if (FAILED(hr))
	{
		return;
	}

	pTempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pBuf);
	pTempBuffer->Release();
	m_nBufsize = dsbd.dwBufferBytes;
}

bool CSound::IsPlay()
{
	DWORD dwStatus;
	m_pBuf->GetStatus(&dwStatus);
	
	return dwStatus&DSBSTATUS_PLAYING ? true:false;
}

bool CSound::Play(bool bLoop/* =false */)
{
	if(NULL == m_pBuf)
		return false;

	DWORD dwLooped = bLoop?DSBPLAY_LOOPING:0;
	if (FAILED(m_pBuf->Play(0, 0, dwLooped)))
	{
		return false;
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Type>
sAutoZero<Type>::sAutoZero()
{
	memset(this, 0x0, sizeof(Type));
	this->dwSize=sizeof(Type);
}