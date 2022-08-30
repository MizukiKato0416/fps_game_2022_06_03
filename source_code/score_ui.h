//=============================================================================
//
// スコアのUI処理 [score_ui.h]
// Author : 加藤瑞葵
//
//=============================================================================
#ifndef _SCORE_UI_H_
#define _SCORE_UI_H_

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
// 構造体の定義
//*****************************************************************************
typedef struct
{
	int nPlayerNum;		//プレイヤー番号
	int nKill;			//キル数
	string aName;
} Ranking;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScoreUi : public CObject
{
public:
	CScoreUi(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// デフォルトコンストラクタ
	~CScoreUi();	// デフォルトデストラクタ
	HRESULT Init(void);	// ポリゴンの初期化
	void Uninit(void);	// ポリゴンの終了
	void Update(void);	// ポリゴンの更新
	void Draw(void);	// ポリゴンの描画
	static CScoreUi *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &scale);	// 生成
	//指定したランキングのキル数とプレイヤー名表示する処理
	void SetRank(const int &nRank) { m_nRank = nRank; }
	//指定した順位のランキングデータ取得処理
	Ranking GetRankData(const int &nRank) { return m_ranking[nRank]; }
	//プレイヤーの番号から表示するデータを参照するかしないか設定処理
	void SetPlayerNum(const bool &bSetPlayerNum) { m_bSetPlayerNum = bSetPlayerNum; }
	//プレイヤーの番号から表示するデータを参照するかしないか取得処理
	bool GetPlayerNum(void) { return m_bSetPlayerNum; }


private:
	//文字生成処理
	void CreateLetter(const int &nRank);

	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_scale;			//スケール
	int m_nRank;					//表示するランキング
	CCounter *m_pRankCounter;		//ランキングUI表示用
	CCounter *m_pKillCounter;		//キル数UI表示用
	vector<Ranking> m_ranking;		//ランキングデータ
	bool m_bSetPlayerNum;			//プレイヤーの番号から表示するデータを参照するかしないか
	CObject2D *m_pRank;				//ランキングの数字の後の英語
	vector<CLetter*> m_name_font;	// 名前フォント
	//int m_count_letter;				// テキストカウント
};

#endif