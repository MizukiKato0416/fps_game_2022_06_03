//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_DESTAANCE (200)		//カメラと注視点の距離

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCamera
{
public:
	CCamera();	// デフォルトコンストラクタ
	~CCamera();	// デフォルトデストラクタ
	HRESULT Init(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR, D3DXVECTOR3 Rot);	// カメラの初期化
	void Uninit(void);	// カメラの終了
	void Update(void);	// カメラの更新
	void SetCamera(void);	// カメラの描画
	static CCamera *Create(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR, D3DXVECTOR3 Rot);
	CCamera *GetCamera(void) { return this; }	// カメラの入手
	D3DXMATRIX GetViewMtx(void) { return m_mtxView; }
	D3DXMATRIX GetProjecMtx(void) { return m_mtxProjection; }
	D3DXVECTOR3 *CalcScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj);	// スクリーン座標をワールド座標に変換
	D3DXVECTOR3 *GetMousePos(void) { return &m_MouseWorldPos; }
private:
	void Move(void);			//動き

	D3DXVECTOR3 m_PosV;			//視点
	D3DXVECTOR3 m_PosR;			//注視点
	D3DXVECTOR3 m_Rot;			//カメラの向き
	D3DXVECTOR3 m_VecU;			//上方向ベクトル
	D3DXVECTOR3 m_MouseWorldPos;
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		//ビューマトリックス
	float Long;					//幅
	float m_fDifferVR;
};
#endif