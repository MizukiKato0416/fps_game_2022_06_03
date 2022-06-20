//---------------------------
// Author:佐藤秀亮
// 平面(plane.cpp)
//---------------------------
#include "plane.h"
#include "manager.h"
#include "renderer.h"
#include "object3D.h"

//マクロ定義

//静的メンバ変数

CPlane::CPlane(PRIORITY priorty) : CObject(priorty)
{

}

CPlane::~CPlane()
{

}

//初期化処理
HRESULT CPlane::Init(/*D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex*/)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する

	SetPos(m_Oliginpos);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);



	//SetVtxMin(-size);

	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_Oliginsize.x, m_Oliginsize.y, m_Oliginsize.z);
	pVtx[1].pos = D3DXVECTOR3(m_Oliginsize.x, m_Oliginsize.y, m_Oliginsize.z);
	pVtx[2].pos = D3DXVECTOR3(-m_Oliginsize.x, -m_Oliginsize.y, -m_Oliginsize.z);
	pVtx[3].pos = D3DXVECTOR3(m_Oliginsize.x, -m_Oliginsize.y, -m_Oliginsize.z);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_OliginTex.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_OliginTex.y);
	pVtx[3].tex = D3DXVECTOR2(m_OliginTex.x, m_OliginTex.y);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	return S_OK;
}

//終了処理
void CPlane::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

//更新処理
void CPlane::Update()
{

}

//描画処理
void CPlane::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld; //計算用マトリックス
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する

	LPDIRECT3DTEXTURE9 buf = CManager::GetInstance()->GetTexture()->GetTexture(m_pTexture);

	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//透明な部分を描画しないようにする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);
	if (buf != NULL)
	{
		pDevice->SetTexture(0, buf);    //テクスチャの設定
	}
	else
	{
		pDevice->SetTexture(0, NULL);    //テクスチャの設定
	}
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数
			//ラインティングを有効にする

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//デフォルトに戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}
//=============================================================================
// テクスチャ破棄
//=============================================================================
void CPlane::UninitTexture()
{

}

//作成
CPlane *CPlane::Create()
{
	CPlane *pPlane;
	pPlane = new CPlane(CObject::PRIORITY::EFFECT3D);
	if (pPlane != NULL)
	{
		pPlane->Init();
	}
	return pPlane;
}

//=============================================================================
// サイズセット
//=============================================================================
void CPlane::SetSize(D3DXVECTOR3 size)
{
	//m_size = size;
	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, size.z);
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y, size.z);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y, -size.z);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y, -size.z);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 色セット
//=============================================================================
void CPlane::ChangeColor(D3DXCOLOR col)
{
	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//カラー変更
//=============================================================================
void CPlane::ColorChange(D3DCOLORVALUE color)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ

				   //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の色
	pVtx[0].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);
	pVtx[1].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);
	pVtx[2].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);
	pVtx[3].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//サイズ変更
//=============================================================================
void CPlane::ChangeSize(D3DXVECTOR3 size)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size.x, size.y	, size.z);
	pVtx[1].pos = D3DXVECTOR3(size.x, size.y	, size.z);
	pVtx[2].pos = D3DXVECTOR3(-size.x, -size.y	, -size.z);
	pVtx[3].pos = D3DXVECTOR3(size.x, -size.y	, -size.z);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//平面エフェクト
//=============================================================================
void CPlane::SetPosField(D3DXVECTOR3 pos, D3DXVECTOR3 Size, float Rotate, float Rotate2)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pos.x + (cosf(Rotate2)) * Size.x, Size.y, pos.z + (sinf(Rotate))* Size.x);
	pVtx[1].pos = D3DXVECTOR3(pos.x + (sinf(Rotate))  * Size.x, Size.y, pos.z - (cosf(Rotate2))* Size.x);
	pVtx[2].pos = D3DXVECTOR3(pos.x - (sinf(Rotate))  * Size.x, Size.y, pos.z + (cosf(Rotate2))* Size.x);
	pVtx[3].pos = D3DXVECTOR3(pos.x - (cosf(Rotate2)) * Size.x, Size.y, pos.z - (sinf(Rotate))* Size.x);


	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//=============================================================================
//サイズ変更
//=============================================================================
void CPlane::BillboardSize(float size)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-size, size, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(size, size, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-size, -size, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(size, -size, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//テクスチャ移動
//=============================================================================
void CPlane::TexturMove(D3DXVECTOR2 MoveTex)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ
	m_TexMove += MoveTex;
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(m_TexMove.x				, m_TexMove.y);
	pVtx[1].tex = D3DXVECTOR2(m_TexNum.x + m_TexMove.x	, m_TexMove.y);
	pVtx[2].tex = D3DXVECTOR2(m_TexMove.x				, m_TexNum.y + m_TexMove.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexNum.x + m_TexMove.x	, m_TexNum.y + m_TexMove.y);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
//テクスチャパターン
//=============================================================================
void CPlane::SetTexAnim(D3DXVECTOR2 TexPattern, D3DXVECTOR2 TexAnimSize)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ

	m_nSplit = TexPattern;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(m_TexNum.x + m_nSplit.x * TexAnimSize.x + m_TexMove.x					, m_TexNum.y + m_nSplit.y * TexAnimSize.y + m_TexMove.y);
	pVtx[1].tex = D3DXVECTOR2(m_TexNum.x + m_nSplit.x * TexAnimSize.x + TexAnimSize.x + m_TexMove.x	, m_TexNum.y + m_nSplit.y * TexAnimSize.y + m_TexMove.y);
	pVtx[2].tex = D3DXVECTOR2(m_TexNum.x + m_nSplit.x * TexAnimSize.x + m_TexMove.x					, m_TexNum.y + m_nSplit.y *  + TexAnimSize.y + TexAnimSize.y + m_TexMove.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexNum.x + m_nSplit.x * TexAnimSize.x + TexAnimSize.x + m_TexMove.x	, m_TexNum.y + m_nSplit.y *  +TexAnimSize.y + TexAnimSize.y + m_TexMove.y);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//=============================================================================
//ビルボード座標いじり
//=============================================================================
void CPlane::SetPosBill(D3DXVECTOR3 pos, D3DXVECTOR3 pos2, D3DXVECTOR3 pos3, D3DXVECTOR3 pos4)
{
	VERTEX_3D*pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = pos;
	pVtx[1].pos = pos2;
	pVtx[2].pos = pos3;
	pVtx[3].pos = pos4;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}