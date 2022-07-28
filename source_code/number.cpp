//================================================
//ナンバー処理
//Author:KatoMizuki
//================================================
#include "renderer.h"
#include "number.h"
#include "manager.h"
#include "object.h"
#include "texture.h"

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CNumber::CNumber()
{
	m_pTexture = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//================================================
//オーバーロードされたコンストラクタ
//================================================

//================================================
//デストラクタ
//================================================
CNumber::~CNumber()
{

}

//================================================
//初期化処理
//================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//変数初期化
	m_pos = pos;
	m_size = size;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D_NUMBER) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D_NUMBER,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	VERTEX_2D_NUMBER *pVtx;
	//頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f / 10.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / 10.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//================================================
//終了処理
//================================================
void CNumber::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//================================================
//更新処理
//================================================
void CNumber::Update(void)
{
	
}

//================================================
//描画処理
//================================================
void CNumber::Draw(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D_NUMBER));
	pDevice->SetFVF(FVF_VERTEX_2D_NUMBER);			//頂点フォーマットの設定

	pDevice->SetTexture(0, m_pTexture);	//テクスチャの設定

												//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,					//描画を開始する頂点のインデックス
		2);					//描画するプリミティブ数
}

//================================================
//生成処理
//================================================
CNumber* CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	//インスタンスの生成
	CNumber *pNumber = NULL;
	if (pNumber == NULL)
	{
		pNumber = new CNumber;
		if (pNumber != NULL)
		{
			pNumber->Init(pos, size);
		}
	}
	return pNumber;
}

//================================================
//テクスチャ設定処理
//================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D_NUMBER *pVtx;
	//頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + ((1.0f / 10.0f) * nNumber), 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / 10.0f) + ((1.0f / 10.0f) * nNumber), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + ((1.0f / 10.0f) * nNumber), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / 10.0f) + ((1.0f / 10.0f) * nNumber), 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//カラー設定処理
//================================================
void CNumber::SetCol(D3DXCOLOR col)
{
	VERTEX_2D_NUMBER *pVtx;	// 頂点情報
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//テクスチャ割り当て処理
//================================================
void CNumber::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}