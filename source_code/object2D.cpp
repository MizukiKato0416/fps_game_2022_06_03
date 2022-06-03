//================================================
//2Dポリゴン処理
//Author:加藤瑞葵
//================================================
#include "renderer.h"
#include "object2D.h"
#include "manager.h"

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CObject2D::CObject2D(CObject::PRIORITY Priority):CObject(Priority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_OriginType = ORIGIN_TYPE::CENTER;
}

//================================================
//オーバーロードされたコンストラクタ
//================================================

//================================================
//デストラクタ
//================================================
CObject2D::~CObject2D()
{

}

//================================================
//初期化処理
//================================================
HRESULT CObject2D::Init(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	

	//位置・サイズ設定処理
	CObject::SetPos(m_pos);
	CObject::SetSize(m_size);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_OriginType)
	{
	case ORIGIN_TYPE::CENTER:
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
		break;
	case ORIGIN_TYPE::LEFT:
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y / 2.0f, 0.0f);
		break;
	default:
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
		break;
	}
	
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//================================================
//終了処理
//================================================
void CObject2D::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//オブジェクトの破棄
	Release();
}

//================================================
//更新処理
//================================================
void CObject2D::Update(void)
{
	
}

//================================================
//描画処理
//================================================
void CObject2D::Draw(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);			//頂点フォーマットの設定

	pDevice->SetTexture(0, m_pTexture);	//テクスチャの設定

												//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,					//描画を開始する頂点のインデックス
		2);					//描画するプリミティブ数
}

//================================================
//生成処理
//================================================
CObject2D* CObject2D::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const int &nPriolity)
{
	//インスタンスの生成
	CObject2D *pObject2D = nullptr;
	if (pObject2D == nullptr)
	{
		pObject2D = new CObject2D(static_cast<CObject::PRIORITY>(nPriolity));
		if (pObject2D != nullptr)
		{
			pObject2D->m_pos = pos;
			pObject2D->m_size = size;
			pObject2D->Init();
		}
	}
	return pObject2D;
}

//================================================
//位置取得処理
//================================================
D3DXVECTOR3 CObject2D::GetPos(void)
{
	return m_pos;
}

//================================================
//位置設定処理
//================================================
void CObject2D::SetPos(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size)
{
	m_pos = pos;
	m_size = size;

	//位置・サイズ設定処理
	CObject::SetPos(m_pos);
	CObject::SetSize(m_size);

	VERTEX_2D *pVtx;	// 頂点情報
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_OriginType)
	{
	case ORIGIN_TYPE::CENTER:
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
		break;
	case ORIGIN_TYPE::LEFT:
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_size.y / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x, m_pos.y + m_size.y / 2.0f, 0.0f);
		break;
	default:
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y - m_size.y / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x / 2.0f, m_pos.y + m_size.y / 2.0f, 0.0f);
		break;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//位置設定処理
//================================================
void CObject2D::SetPos(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const float &fRot)
{
	m_pos = pos;
	m_size = size;

	//位置・サイズ設定処理
	CObject::SetPos(m_pos);
	CObject::SetSize(m_size);

	//角度を求める
	float fTanR = atan2f(m_size.y / 2.0f, m_size.x / 2.0f);
	//直径を求める
	float fCircleR = sqrtf((float)pow(m_size.x / 2.0f, 2.0f) + (float)pow(m_size.y / 2.0f, 2.0f));

	VERTEX_2D *pVtx;	// 頂点情報
						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fCircleR * cosf(fTanR - fRot), m_pos.y - fCircleR * sinf(fTanR - fRot), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fCircleR * cosf(fTanR + fRot), m_pos.y - fCircleR * sinf(fTanR + fRot), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fCircleR * cosf(fTanR + fRot), m_pos.y + fCircleR * sinf(fTanR + fRot), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fCircleR * cosf(fTanR - fRot), m_pos.y + fCircleR * sinf(fTanR - fRot), 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//テクスチャ設定処理
//================================================
void CObject2D::SetTex(const int &nPattern, const int &nMaxPattern)
{
	VERTEX_2D *pVtx;	// 頂点情報
						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2((1.0f / nMaxPattern) * nPattern, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / nMaxPattern) + (1.0f / nMaxPattern) * nPattern, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / nMaxPattern) * nPattern, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / nMaxPattern) + (1.0f / nMaxPattern) * nPattern, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//テクスチャ設定処理
//================================================
void CObject2D::SetTex(const float &fMoveTexU, const float &fMoveTexV)
{
	VERTEX_2D *pVtx;	// 頂点情報
						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + fMoveTexU, 0.0f + fMoveTexV);
	pVtx[1].tex = D3DXVECTOR2(1.0f + fMoveTexU, 0.0f + fMoveTexV);
	pVtx[2].tex = D3DXVECTOR2(0.0f + fMoveTexU, 1.0f + fMoveTexV);
	pVtx[3].tex = D3DXVECTOR2(1.0f + fMoveTexU, 1.0f + fMoveTexV);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//カラー設定処理
//================================================
void CObject2D::SetCol(const D3DCOLORVALUE &col)
{
	VERTEX_2D *pVtx;	// 頂点情報
						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DCOLOR_RGBA((int)col.r, (int)col.g, (int)col.b, (int)col.a);
	pVtx[1].col = D3DCOLOR_RGBA((int)col.r, (int)col.g, (int)col.b, (int)col.a);
	pVtx[2].col = D3DCOLOR_RGBA((int)col.r, (int)col.g, (int)col.b, (int)col.a);
	pVtx[3].col = D3DCOLOR_RGBA((int)col.r, (int)col.g, (int)col.b, (int)col.a);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//カラー設定処理
//================================================
void CObject2D::SetCol(const D3DXCOLOR &col)
{
	VERTEX_2D *pVtx;	// 頂点情報
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
//カラー取得処理
//================================================
D3DXCOLOR CObject2D::GetCol(void)
{
	VERTEX_2D *pVtx;	// 頂点情報
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXCOLOR col;
	col = pVtx[0].col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return col;
}

//================================================
//4頂点設定処理
//================================================
void CObject2D::SetVtx(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &vtx0, const D3DXVECTOR3 &vtx1, const D3DXVECTOR3 &vtx2, const D3DXVECTOR3 &vtx3)
{
	m_pos = pos;

	VERTEX_2D *pVtx;	// 頂点情報
						//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = vtx0;
	pVtx[1].pos = vtx1;
	pVtx[2].pos = vtx2;
	pVtx[3].pos = vtx3;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//================================================
//テクスチャ割り当て処理
//================================================
void CObject2D::BindTexture(const LPDIRECT3DTEXTURE9 &pTexture)
{
	m_pTexture = pTexture;
}
