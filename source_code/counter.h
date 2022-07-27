//=============================================================================
//
// 数処理 [number.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCounter : public CObject
{
public:
	CCounter(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// デフォルトコンストラクタ
	~CCounter();	// デフォルトデストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void AddCounter(int nValue) { m_nCounter += nValue; }	// スコア加算 { スコア += 引数}
	int GetCounter(void) { return m_nCounter; }				// ゲッター
	void SetCounter(int nCounter);	// セッター
	void SetCol(D3DXCOLOR col);	// セッター
	void SetCounterNum(int nCounter) { m_nCounter = nCounter; }	// セッター
	int GetCounterNum(void) { return m_nCounter; }	// ゲッダー
	static CCounter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumbderLength, string type);	// オブジェクトの生成
private:
	vector<CNumber*> m_pNumber;	// 桁
	D3DXVECTOR3 m_Pos;	// 位置
	D3DXVECTOR3 m_Size;	// サイズ
	string m_Type;	// テクスチャ
	int m_nNumberMax;	// 桁数
	int m_nCounter;	// スコア
};

#endif