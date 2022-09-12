//=============================================================================
//
// キルレのUI処理 [kill_rate_ui.h]
// Author : 加藤瑞葵
//
//=============================================================================
#ifndef _KILL_RATE_UI_H_
#define _KILL_RATE_UI_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;
class CCounter;
class CLetter;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define KILL_RATE_UI_FRAME_DEFAULT_SIZE_X			(516.0f)		//フレームのデフォルトサイズ
#define KILL_RATE_UI_FRAME_DEFAULT_SIZE_Y			(72.0f)			//フレームのデフォルトサイズ

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
typedef struct
{
	int nPlayerNum;		//プレイヤー番号
	int nKill;			//キル数
	int nDeath;			//デス数
	string aName;
} KillRateRanking;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CKillRateUi : public CObject
{
public:
	CKillRateUi(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// デフォルトコンストラクタ
	~CKillRateUi();	// デフォルトデストラクタ
	HRESULT Init(void);	// ポリゴンの初期化
	void Uninit(void);	// ポリゴンの終了
	void Update(void);	// ポリゴンの更新
	void Draw(void);	// ポリゴンの描画
	static CKillRateUi *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &scale);	// 生成
	//指定したランキングのキル数とプレイヤー名表示する処理
	void SetRank(const int &nRank) { m_nRank = nRank; }
	//指定した順位のランキングデータ取得処理
	KillRateRanking GetRankData(const int &nRank) { return m_ranking[nRank]; }
	//プレイヤーの番号から表示するデータを参照するかしないか設定処理
	void SetPlayerNum(const bool &bSetPlayerNum) { m_bSetPlayerNum = bSetPlayerNum; }
	//プレイヤーの番号から表示するデータを参照するかしないか取得処理
	bool GetPlayerNum(void) { return m_bSetPlayerNum; }


private:
	//文字生成処理
	void CreateLetter(const int &nRank);

	D3DXVECTOR3 m_pos;						//位置
	D3DXVECTOR3 m_scale;					//スケール
	int m_nRank;							//表示するランキング
	CCounter *m_pKillCounter;				//キル数UI表示用
	CCounter *m_pDeathCounter;				//デス数UI表示用
	vector<KillRateRanking> m_ranking;		//ランキングデータ
	bool m_bSetPlayerNum;					//プレイヤーの番号から表示するデータを参照するかしないか
	vector<CLetter*> m_name_font;			// 名前フォント
};

#endif