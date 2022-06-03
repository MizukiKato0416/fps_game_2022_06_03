//===========================================
//サウンド処理
//Authore:加藤瑞葵
//===========================================
#ifndef _SOUND_H_
#define _SOUND_H_
#include "main.h"
#include <thread>

//================================================
//マクロ定義
//================================================

//========================================================
//構造体の定義
//========================================================

//================================================
//クラスの定義
//================================================
//クラスの定義
class CSound
{
public:
	CSound();				//コンストラクタ
	~CSound();				//デストラクタ

	//サウンドファイル
	enum class SOUND_LABEL
	{
		AIRPLANE_SE = 0,		//飛行機SE
		BANANA_SE,				//バナナSE
		CANSEL_SE,				//戻るのSE
		PAUSE_BUTTON_SE,		//ポーズSE
		DECISION_SE,			//決定SE
		FAN_SE,					//扇風機SE
		GAGE_SE,				//ゲージSE
		GOAL_SE,				//ゴールSE
		REDBULL_SE,				//レッドブルSE
		ROCKET_SE,				//ロケットSE
		STAR_RING_SE,			//starringSE
		THORN_SE,				//とげSE
		TRAMPOLINE_SE,			//トランポリンSE
		BREAK_SE,				//ブレーキSE
		JUMP_SE,				//ジャンプSE
		BAD_SE,					//badSE
		GOOD_SE,				//goodSE
		GREAT_SE,				//greatSE
		PARFECT_SE,				//parfectSE
		ERROR_SE,				//エラー
		KIYO_BREAK_SE,			//キヨ　ブレーキ
		ANGRY_SE,				//起こったとき
		DIALOG_SE,				//ダイアログ
		ENDROOL_BGM,			//エンドロールBGM
		RESULT_BGM,				//リザルトBGM
		GAME_BGM,				//ゲームBGM
		TITLE_BGM,				//タイトルBGM
		TROPHY_BGM,				//トロフィーBGM
		MAX,
	};

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	HRESULT Play(const SOUND_LABEL &label);
	void Stop(const SOUND_LABEL &label);
	void Stop(void);
	void ControllVoice(const SOUND_LABEL &label, const float &fVolume) { m_apSourceVoice[(int)label]->SetVolume(fVolume); }	//音量調整

private:
	//パラメーター構造体
	typedef struct
	{
		char *filename;		// ファイル名
		int nCntLoop;		// ループカウント
	} PARAM;

	//メンバ関数
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunksize, DWORD *pChunkDataPosition);
	HRESULT ReadChunk(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;										// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;					// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[(int)SOUND_LABEL::MAX] = {};			// ソースボイス
	BYTE *m_apDataAudio[(int)SOUND_LABEL::MAX] = {};							// オーディオデータ
	DWORD m_asizeAudio[(int)SOUND_LABEL::MAX] = {};							// オーディオデータサイズ

	static PARAM m_aParam[(int)SOUND_LABEL::MAX];								// 各音素材のパラメータ
};

//================================================
//プロトタイプ宣言
//================================================
#endif // !_SOUND_H_