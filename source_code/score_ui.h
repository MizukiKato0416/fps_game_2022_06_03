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

private:
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_scale;			//スケール
	int m_nRank;					//表示するランキング
	CCounter *m_pRankCounter;		//ランキングUI表示用
	CCounter *m_pKillCounter;		//キル数UI表示用
};

#endif