//===========================================
//ナンバー処理
//Author:KatoMizuki
//===========================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//================================================
//マクロ定義
//================================================
#define FVF_VERTEX_2D_NUMBER (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//頂点フォーマット
#define VERTEX_NUM_NUMBER (4)												//頂点数

//========================================================
//構造体の定義
//========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	float rhw;				//1.0fで固定
	D3DCOLOR col;			//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
} VERTEX_2D_NUMBER;

//================================================
//クラスの定義
//================================================
//クラスの定義
class CNumber
{
public:
	CNumber();			//コンストラクタ
	~CNumber();							//デストラクタ

	//メンバ関数
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(int nNumber);
	void SetCol(D3DXCOLOR col);

	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);				//テクスチャ割り当て

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;						//ポリゴンの位置
	D3DXVECTOR3	m_size;						// サイズ
};

//================================================
//プロトタイプ宣言
//================================================
#endif // !_NUMBER_H_