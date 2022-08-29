//=============================================================================
//
// 設定画面のバーの処理 [option_bar.h]
// Author : 加藤瑞葵
//
//=============================================================================
#ifndef _OPTION_BAR_H_
#define _OPTION_BAR_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;

//*****************************************************************************
// 構造体の定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class COptionBar : public CObject
{
public:
	COptionBar(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// デフォルトコンストラクタ
	~COptionBar();		// デフォルトデストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	//生成処理
	static COptionBar *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &barSize, const D3DXVECTOR3 &circleSize,
		                      const float &fMin, const float &fMax, const float &fDefault);	// 生成
	//現在の値取得処理
	float GetNum(void) { return m_fNum; }

private:
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_barSize;			//バーのサイズ
	D3DXVECTOR3 m_circleSize;		//円のサイズ
	float m_fMin;					//最小値
	float m_fMax;					//最大値
	float m_fDefault;				//デフォルト値
	float m_fNum;					//現在の値
	CObject2D *m_pCircle;			//動かせる円
	CObject2D *m_pBar;				//バー
	bool m_bClick;					//クリックしたかどうか
};

#endif