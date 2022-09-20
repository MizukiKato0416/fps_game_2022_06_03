//================================================
//サウンド処理
//Authore:加藤瑞葵
//================================================
#include "sound.h"

//================================================
//静的メンバ変数宣言
//================================================
CSound::PARAM CSound::m_aParam[(int)SOUND_LABEL::MAX] =
{
	{ "data/SOUND/BGM/bgm_game.wav", -1 },				//ゲームBGM
	{ "data/SOUND/BGM/bgm_game_lastspurt.wav", -1 },	//ラストスパートBGM
	{ "data/SOUND/BGM/bgm_result.wav", -1 },			//リザルトBGM
	{ "data/SOUND/BGM/bgm_title.wav", -1 },				//タイトルBGM
	{ "data/SOUND/SE/game_reload.wav", 0 },				//リロードSE
	{ "data/SOUND/SE/game_reloadover.wav", 0 },			//リロード完了SE
	{ "data/SOUND/SE/se_game_hit.wav", 0 },				//ヒット時SE
	{ "data/SOUND/SE/se_game_kill.wav", 0 },			//キルSE
	{ "data/SOUND/SE/se_game_over.wav", 0 },			//ゲームオーバーSE
	{ "data/SOUND/SE/se_game_shot.wav", 0 },			//ショットSE
	{ "data/SOUND/SE/se_title_start.wav", 0 },			//スタート時のバキューンSE
	{ "data/SOUND/SE/se_title_wind.wav", 0 },			//タイトルの風SE
	{ "data/SOUND/SE/se_count_down.wav", 0 },			//カウントダウンSE
	{ "data/SOUND/SE/se_finish.wav", 0 },				//フィニッシュSE
	{ "data/SOUND/SE/se_start.wav", 0 },				//スタートSE
	{ "data/SOUND/SE/se_decide.wav", 0 },				//決定SE
};

//================================================
//デフォルトコンストラクタ
//================================================
CSound::CSound()
{
	
}

//================================================
//デストラクタ
//================================================
CSound::~CSound()
{

}

//================================================
//初期化処理
//================================================
HRESULT CSound::Init(void)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		if (m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
		}

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}
/*
	// サブミックスボイスの生成
	hr = m_pXAudio2->CreateSubmixVoice(&m_pSubmixVoice, 1, 44100, 0, 0, 0, 0);
	if (FAILED(hr))
	{
		if (m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
		}

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// サブミックス音声への参照
	XAUDIO2_SEND_DESCRIPTOR Send = { 0, m_pSubmixVoice };
	XAUDIO2_VOICE_SENDS *pSendList = new XAUDIO2_VOICE_SENDS;
	pSendList->pSends = &Send;
	pSendList->SendCount = 1;
*/
	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < (int)SOUND_LABEL::MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunksize = 0;
		DWORD dwChunkPosition = 0;
		DWORD filetype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aParam[nCntSound].filename, GENERIC_READ,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunksize, &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		hr = ReadChunk(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		if (filetype != 'EVAW')
		{
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunksize, &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		hr = ReadChunk(hFile, &wfx, dwChunksize, dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_asizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_asizeAudio[nCntSound]);
		hr = ReadChunk(hFile, m_apDataAudio[nCntSound], m_asizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}

		//m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format), XAUDIO2_VOICE_USEFILTER, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, pSendList, NULL);
		m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));

		// バッファの設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_asizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_aParam[nCntSound].nCntLoop;

		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	//音量調整
	//m_pSubmixVoice->SetVolume(0.1f);
	ControllVoice(SOUND_LABEL::GAME_BGM,			0.6f);
	ControllVoice(SOUND_LABEL::GAME_LAST_SPURT_BGM, 0.6f);
	ControllVoice(SOUND_LABEL::RESULT_BGM,			0.8f);
	ControllVoice(SOUND_LABEL::TITLE_BGM,			1.6f);
	ControllVoice(SOUND_LABEL::RELOAD_SE,			8.0f);
	ControllVoice(SOUND_LABEL::RELOAD_FIN_SE,		5.0f);
	ControllVoice(SOUND_LABEL::HIT_SE,				3.0f);
	ControllVoice(SOUND_LABEL::KILL_SE,				2.0f);
	ControllVoice(SOUND_LABEL::GAME_OVER_SE,		0.6f);
	ControllVoice(SOUND_LABEL::SHOT_SE,				4.0f);
	ControllVoice(SOUND_LABEL::START_SHOT_SE,		0.2f);
	ControllVoice(SOUND_LABEL::TITLE_WIND_SE,		1.0f);
/*	
	// Detailisの取得
	XAUDIO2_VOICE_DETAILS Detailis;
	m_pMasteringVoice->GetVoiceDetails(&Detailis);

	// X3DAudioの初期化
	X3DAudioInitialize(Detailis.InputChannels, X3DAUDIO_SPEED_OF_SOUND, m_X3dInstance);

	// エミッター
	m_Emitter.ChannelCount = 1;
	m_Emitter.CurveDistanceScaler = m_Emitter.DopplerScaler = 1.0f;

	// DSPSettingの初期化
	FLOAT32 *matrix = new FLOAT32[Detailis.InputChannels];
	m_DspSettings.SrcChannelCount = 1;
	m_DspSettings.DstChannelCount = Detailis.InputChannels;
	m_DspSettings.pMatrixCoefficients = matrix;

	m_Emitter.OrientFront.x = -0.0f;
	m_Emitter.OrientFront.y = -1.0f;
	m_Emitter.OrientFront.z = 0.0f;
	m_Emitter.OrientTop.x = 0.0f;
	m_Emitter.OrientTop.y = 1.0f;
	m_Emitter.OrientTop.z = 0.0f;
	m_Emitter.Position.x = 100.0f;
	m_Emitter.Position.y = 0.0f;
	m_Emitter.Position.z = 0.0f;
	m_Emitter.Velocity.x = 0.0f;
	m_Emitter.Velocity.y = 0.0f;
	m_Emitter.Velocity.z = 0.0f;

	m_Listener.OrientFront.x = 0.0f;
	m_Listener.OrientFront.y = 0.0f;
	m_Listener.OrientFront.z = 0.0f;
	m_Listener.OrientTop.x = 0.0f;
	m_Listener.OrientTop.y = 1.0f;
	m_Listener.OrientTop.z = 0.0f;
	m_Listener.Position.x = 0.0f;
	m_Listener.Position.y = 0.0f;
	m_Listener.Position.z = 0.0f;
	m_Listener.Velocity.x = 0.0f;
	m_Listener.Velocity.y = 0.0f;
	m_Listener.Velocity.z = 0.0f;
*/

	return S_OK;
}
/*
//================================================
//更新処理
//================================================
void CSound::UpdateVoice(SOUND_LABEL label)
{

	// Detailisの取得
	XAUDIO2_VOICE_DETAILS Detailis;
	m_pMasteringVoice->GetVoiceDetails(&Detailis);

	// 正規化
	D3DXVec3Normalize(static_cast<D3DXVECTOR3*>(&m_Emitter.OrientFront), static_cast<D3DXVECTOR3*>(&m_Emitter.OrientFront));

	// パラメーターの設定
	X3DAudioCalculate(m_X3dInstance, &m_Listener, &m_Emitter,
					  X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB,
					  &m_DspSettings);

	// ソースに適応
	m_apSourceVoice[static_cast<int>(label)]->SetOutputMatrix(m_pMasteringVoice, 1, Detailis.InputChannels, m_DspSettings.pMatrixCoefficients);
	m_apSourceVoice[static_cast<int>(label)]->SetFrequencyRatio(m_DspSettings.DopplerFactor);

	// サブミックス音声に適用
	m_apSourceVoice[static_cast<int>(label)]->SetOutputMatrix(m_pSubmixVoice, 1, 1, &m_DspSettings.ReverbLevel);

	// フィルターを適応
	XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * m_DspSettings.LPFDirectCoefficient), 1.0f };
	m_apSourceVoice[static_cast<int>(label)]->SetFilterParameters(&FilterParameters);
}
*/
//================================================
//終了処理
//================================================
void CSound::Uninit(void)
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < (int)SOUND_LABEL::MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();

			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
		}
	}

	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();

	if (m_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(停止)
//=============================================================================
HRESULT CSound::Play(const SOUND_LABEL &label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER       buffer;

	// バッファの設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_asizeAudio[(int)label];
	buffer.pAudioData = m_apDataAudio[(int)label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aParam[(int)label].nCntLoop;

	// 状態取得
	m_apSourceVoice[(int)label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_apSourceVoice[(int)label]->Stop(0);

		// クリア
		m_apSourceVoice[(int)label]->FlushSourceBuffers();
	}

	// 登録
	m_apSourceVoice[(int)label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[(int)label]->Start(0);

	return S_OK;
}

//=============================================================================
// セグメント停止
//=============================================================================
void CSound::Stop(const SOUND_LABEL &label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[(int)label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
	 // 一時停止
		m_apSourceVoice[(int)label]->Stop(0);

		// クリア
		m_apSourceVoice[(int)label]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止
//=============================================================================
void CSound::Stop(void)
{
	for (int nCntSound = 0; nCntSound < (int)SOUND_LABEL::MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// クリア
			m_apSourceVoice[nCntSound]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunksize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDatasize;
	DWORD dwRIFFDatasize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDatasize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDatasize = dwChunkDatasize;
			dwChunkDatasize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDatasize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunksize = dwChunkDatasize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDatasize;
		if (dwBytesRead >= dwRIFFDatasize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT CSound::ReadChunk(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}