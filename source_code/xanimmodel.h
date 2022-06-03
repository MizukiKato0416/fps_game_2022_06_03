//=============================================================================
//
// Xファイルモデル処理 [sceneX.h]
// Author : 羽鳥太一&鶴間俊樹
//
//=============================================================================
#ifndef _XANIMMODEL_H_
#define _XANIMMODEL_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "hierarchydata.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CXanimModel : CObject
{
public:
	CXanimModel(CObject::PRIORITY Priority = CObject::PRIORITY::MODEL);	// デフォルトコンストラクタ
	~CXanimModel();	// デフォルトデストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void UpdateFrame(LPD3DXFRAME base, LPD3DXMATRIX parent_matrix);	// フレームのマトリックスの更新
	void Draw(void);	// 描画
	void DrawMatrix(LPD3DXMATRIX matrix);	// フレームの描画
	void DrawFrame(LPD3DXFRAME frame);	// フレームの描画(再帰処理)
	void DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container);	// メッシュコンテナの描画(再帰処理)
	HRESULT AllocateAllBoneMatrix(LPD3DXFRAME frame);	// 全ボーンの描画用マトリックスの保存(再帰処理)
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER vontainer);	// 全ボーンの描画用マトリックスの保存(再帰処理)
	void Create(string type);	// 生成
	void SetAnimation(int nAnim, float AdjustSpeed);	// アニメーション変更
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }	// セッター
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }	// セッター
	D3DXVECTOR3 GetPos(void) { return m_Pos; }	// ゲッター
	D3DXVECTOR3 GetRot(void) { return m_Rot; }	// ゲッター
private:
	HierarchyData m_HierarchyData;	// 階層データ
	LPD3DXFRAME m_RootFrame;	// ルートフレーム
	LPD3DXANIMATIONCONTROLLER m_AnimController;	// アニメーション管理
	vector<LPD3DXANIMATIONSET> m_AnimSet;	// アニメーション取得
	D3DXMATRIX m_MtxWold;	// ワールドマトリックス
	D3DXVECTOR3 m_Pos;	// 位置
	D3DXVECTOR3 m_Rot;	// 回転
	int m_nNowAnim;	// 現在のアニメーション
};

#endif
