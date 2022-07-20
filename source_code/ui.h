//=============================================================================
//
// タイトル処理 [title.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "object2D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CUi : public CObject2D
{
public:
	enum class BLINKING_TYPE
	{
		NONE = 0,
		BLINKING,
		FADE,
		MAX
	};
	enum class MOVE_TYPE
	{
		NONE = 0,
		LEFT_TO_RIGHT,
		RIGHT_TO_LEFT,
		TOP_TO_BOTTOM,
		BOTTOM_TO_TOP,
		MAX
	};
	CUi(PRIORITY Layer = PRIORITY::UI);	// デフォルトコンストラクタ
	~CUi();	// デフォルトデストラクタ
	HRESULT Init(void);	// ポリゴンの初期化()
	void Uninit(void);	// ポリゴンの終了
	void Update(void);	// ポリゴンの更新
	void Draw(void);	// ポリゴンの描画
	static CUi *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXVECTOR3 &stop, const int &blinkingtype, const int &movetype, const int &blinkingspeed, const int &movespeed, string tex);
	void SetBlinkingType(const BLINKING_TYPE &type) { m_BlinkingType = type; }				// 点滅タイプ設定処理
	BLINKING_TYPE GetBlinkingType(void) { return m_BlinkingType; }							// 点滅タイプ取得処理
	void SetMoveType(const MOVE_TYPE &type) { m_MoveType = type; }							// 移動タイプ設定処理
	MOVE_TYPE GetMoveType(void) { return m_MoveType; }										// 移動タイプ取得処理
	D3DXVECTOR3 GetStopPoint(void) { return m_stopPoint; }									// 停止位置取得処理
	void SetStopPoint(const D3DXVECTOR3 &stop) { m_stopPoint = stop; }						// 停止位置設定処理
	int GetBlinkingSpeed(void) { return m_nBlinkingSpeed; }									// 点滅感覚取得処理
	void SetBlinkingSpeed(const int &blinking) { m_nBlinkingSpeed = blinking; }				// 点滅感覚設定処理
	float GetMoveSpeed(void) { return m_fMoveSpeed; }										// 移動量取得処理
	void SetMoveSpeed(const float &movespeed) { m_fMoveSpeed = movespeed; }					// 移動量設定処理
	string GetTexName(void) { return m_tex; }	// テクスチャ名取得処理
private:
	D3DXVECTOR3 m_pos;						//ポリゴンの位置
	D3DXVECTOR3	m_size;						//サイズ
	D3DXVECTOR3 m_stopPoint;
	string m_tex;
	BLINKING_TYPE m_BlinkingType;
	MOVE_TYPE m_MoveType;
	int m_nTimer;
	int m_nBlinkingSpeed;
	float m_fMoveSpeed;
	bool m_bChange;
};

#endif