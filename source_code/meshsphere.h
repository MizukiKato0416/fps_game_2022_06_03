//===========================================
//メッシュスフィア処理
//Author:加藤瑞葵
//===========================================
#ifndef _MESH_SPHERE_H_
#define _MESH_SPHERE_H_
#include "object.h"
#include "main.h"

//================================================
//マクロ定義
//================================================

//================================================
//クラスの定義
//================================================
//クラスの定義
class CMeshSphere : public CObject
{
public:
	CMeshSphere(CObject::PRIORITY Priority = CObject::PRIORITY::NONE);	//コンストラクタ
	~CMeshSphere();								//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMeshSphere *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXVECTOR3 &rot, const int &nLine, const int &nVertical);
	//テクスチャ設定処理
	void BindTexture(const LPDIRECT3DTEXTURE9 &pTexture);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;				//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;			//インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;							//ポリゴンの位置
	D3DXVECTOR3	m_size;							//サイズ
	D3DXVECTOR3 m_rot;							//向き
	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	int m_nVertical;							//縦のポリゴンの数 
	int m_nLine;								//横のポリゴンの数
};

#endif // !_MESH_SPHERE_H_