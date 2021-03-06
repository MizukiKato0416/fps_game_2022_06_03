//*****************************************************************************
// シーン2D処理 [Scene2D.h]
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define MAX_TEXTURE (16)		//最大で使える2D用の画像数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点フォーマットに合わせた構造体
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点処理
	float rhw;			// 1.0で固定
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャの座標
} VERTEX_2D;

//*****************************************************************************
//オブジェクトクラス
//*****************************************************************************
class CScene2D : public CObject
{
public:
	CScene2D(PRIORITY priorty = PRIORITY::EFFECT2D);
	~CScene2D();

	HRESULT Init();	//初期化
	void Uninit();				//終了
	void Update();				//更新
	void Draw();				//描画

	void SetPosition(D3DXVECTOR3 pos);		//pos更新用
	D3DXVECTOR3 GetPosition() { return m_pos; }				//pos更新するためにゲット

	float GetHight() { return  m_Size.y; }			//pos更新するためにゲット
	float GetWhidth() { return  m_Size.x; }			//pos更新するためにゲッ
	void SetHight(float Hight);		//大きさ更新
	void SetWhidth(float Whidth);	//大きさ更新

	void ColorChange(D3DCOLORVALUE color);	//カラー変更

	void FadeColorChange(int Alpha);		//フェード用α変更

	void SetTex(int Texpos, float TexSize);						//テクスチャパターン番号、パターン１つのサイズ
	void TexMove(float TexMoveU, float TexMoveV);				//テクスチャ座標の更新

	void SetGaugeScele(float Whidth, float Hight);				//ゲージタイプのスケール変更
	void SetRotate(D3DXVECTOR3 pos, float Rotate, float Rotate2, float Vectol);			//回転

protected:
	D3DXVECTOR3 m_pos;
	string m_pTexture;					//種類

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	//頂点バッファへのポインタ

	D3DXVECTOR2 m_Size;				//サイズ


	int m_Texpos;					//テクスチャ座標
	float m_TexSize;				//テクスチャサイズ

	float m_TexMoveU;				//テクスチャ移動値
	float m_TexMoveV;				//テクスチャ移動値

	float m_PosTexU;				//テクスチャU座標
	float m_PosTexV;				//テクスチャV座標

									//カラー値
};
#endif
