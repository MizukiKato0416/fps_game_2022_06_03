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
class CXanimModel
{
public:
	CXanimModel();	// デフォルトコンストラクタ
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
	static CXanimModel *Create(string type);	// 生成
	void Load(void);	// モデルのロード
	void ChangeAnimation(int anim_num, float speed);	// アニメーション変更
	void PlayAnimation(float speed);	// アニメーション再生
	void CheckContainer(LPD3DXFRAME frame, D3DXMATRIX *check_mtx, string name);	// コンテナの名前のチェック
	void ResetAnimasionPos(void) { m_anim_controller->SetTrackPosition(0, 0); }	// アニメーション位置のリセット
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// セッター
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }	// セッター
	D3DXMATRIX GetMatrix(string name);	// ゲッダー
	LPD3DXFRAME GetRootFrame(void) { return m_root_frame; }	// ゲッダー
	D3DXVECTOR3 GetPos(void) { return m_pos; }	// ゲッター
	D3DXVECTOR3 GetRot(void) { return m_rot; }	// ゲッター
private:
	HierarchyData m_hierarchy_data;	// 階層データ
	LPD3DXFRAME m_root_frame;	// ルートフレーム
	LPD3DXANIMATIONCONTROLLER m_anim_controller;	// アニメーション管理
	vector<LPD3DXANIMATIONSET> m_anim_set;	// アニメーション取得
	string m_load_pas; // モデルのパス
	D3DXMATRIX m_mtx_wold;	// ワールドマトリックス
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 回転
	int m_now_anim;	// 現在のアニメーション
};

#endif
