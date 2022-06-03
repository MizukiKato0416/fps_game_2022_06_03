//===========================================
//メッシュフィールド処理
//Author:加藤瑞葵
//===========================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_
#include "object.h"
#include "main.h"

//================================================
//マクロ定義
//================================================

//================================================
//クラスの定義
//================================================
//クラスの定義
class CMeshField : public CObject
{
public:
	CMeshField(CObject::PRIORITY Priority = CObject::PRIORITY::MESH_FIELD);	//コンストラクタ
	~CMeshField();										//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMeshField *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXVECTOR3 &rot, const int &nLine, const int &nVertical);
	//テクスチャ設定処理
	void BindTexture(const LPDIRECT3DTEXTURE9 &pTexture);
	//当たり判定(第二引数は当たり判定をする半径)
	static bool Collision(CObject *pSubjectObject, const float &fRadius);
	//頂点座標設定処理
	void SetVtxPos(const int &nNumVtx, const D3DXVECTOR3 &pos);
	//頂点座標加算処理
	void AddVtxPos(const int &nNumVtx, const D3DXVECTOR3 &pos);
	//頂点座標取得処理
	D3DXVECTOR3 GetVtxPos(const int &nNumVtx) { return m_bufferPos[nNumVtx]; }
	//インデックスバッファーの頂点位置取得処理
	void GetIndexPos(void);						
	//インデックスバッファーの頂点位置設定処理
	void SetIndexPos(void);

private:
	//メンバ関数

	LPDIRECT3DTEXTURE9 m_pTexture;				//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;			//インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;							//ポリゴンの位置
	std::vector<D3DXVECTOR3> m_indexPos;		//インデックスバッファーの頂点位置
	std::vector<D3DXVECTOR3> m_indexSavePos;	//インデックスバッファーの頂点位置保存用
	std::vector<D3DXVECTOR3> m_indexRot;		//インデックスバッファーの頂点向き
	std::vector<D3DXVECTOR3> m_bufferPos;		//バッファーの頂点位置
	D3DXVECTOR3	m_size;							//サイズ
	D3DXVECTOR3 m_rot;							//向き
	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	std::vector<D3DXMATRIX> m_indexMtxWorld;	//インデックスバッファワールドマトリックス
	int m_nVertical;							//縦のポリゴンの数 
	int m_nLine;								//横のポリゴンの数
};

#endif // !_MESH_FIELD_H_