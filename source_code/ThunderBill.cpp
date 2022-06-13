//=============================================================================
// (雷（仮称）)エフェクト処理 [ThunderBill.cpp]
// Author : 佐藤秀亮
//=============================================================================
#include "ThunderBill.h"
#include "renderer.h"
#include "manager.h"
#include "object3D.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CThunderBill::CThunderBill(PRIORITY priorty) : CBillEffect(priorty)
{
	m_orizinSize = {};
}

//=============================================================================
// デストラクタ
//=============================================================================
CThunderBill::~CThunderBill()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CThunderBill::Init(D3DXVECTOR3 pos,
	D3DXVECTOR3 Size,
	D3DXVECTOR3 MinSize,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	int nTex,
	int nLife,
	float Destance,
	D3DXVECTOR2 TexMove,
	D3DXVECTOR2 TexNum,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	float fHigth,
	D3DXVECTOR3 orizinSize,
	int Synthetic,
	ANIMPATTERN AnimPattern)
{
	CBillEffect::Init(Size, MinSize, color, Mincolor, nTex, nLife, TexNum, TexMove, nAnimCounter, nSplit, AnimPattern);
	int fDistance = (int)Destance;
	float fRandAngle = CIRCLE2;
	float fRandAngle2 = CIRCLE2;
	float fRandDistance = float(rand() % fDistance) - float(rand() % fDistance);
	m_orizinSize = orizinSize;

	m_nSynthenic = Synthetic;
	m_Size = Size;
	m_posorizin1 = D3DXVECTOR3(
		pos.x * sinf(fRandAngle)  * cosf(fRandAngle2) + m_orizinSize.x,
		0.0f,
		{});
	m_posorizin2 = D3DXVECTOR3(
		pos.x * sinf(fRandAngle) * cosf(fRandAngle2) - m_orizinSize.x,
		0.0f,
		{});
	m_posHigth1 = D3DXVECTOR3(
		pos.x + fRandDistance * sinf(fRandAngle) * cosf(fRandAngle2) + m_Size.x,
		fHigth,
		{});
	m_posHigth2 = D3DXVECTOR3(
		pos.x + fRandDistance * sinf(fRandAngle) * cosf(fRandAngle2) - m_Size.x,
		fHigth,
		{});

	CPlane::SetPosBill(m_posHigth1, m_posHigth2, m_posorizin1, m_posorizin2);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CThunderBill::Uninit()
{
	CBillEffect::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CThunderBill::Update()
{
	CPlane::SetPosBill(m_posHigth1, m_posHigth2, m_posorizin1, m_posorizin2);
	CBillEffect::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CThunderBill::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans, mtxWorld; //計算用マトリックス
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する

	D3DXMatrixIdentity(&mtxWorld);

	//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//カリングオフ
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (m_nSynthenic == 0)
	{
		//加算合成関係
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else if (m_nSynthenic == 1)
	{
		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	//それ以外の数値は加算合成に
	else
	{
		//加算合成関係
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	////ポリゴンをカメラに対して正面に向ける
	//D3DXMatrixInverse(&mtxWorld, NULL, &mtxView); //逆行列を求める

	//x軸以外のビルボード化
	mtxWorld._11 = mtxView._11;
	mtxWorld._12 = mtxView._21;
	mtxWorld._13 = mtxView._31;
	//mtxWorld._21 = mtxView._12;
	mtxWorld._22 = mtxView._22;
	//mtxWorld._23 = mtxView._32;
	mtxWorld._31 = mtxView._13;
	mtxWorld._32 = mtxView._23;
	mtxWorld._33 = mtxView._33;

	//座標
	mtxWorld._41 = 0.0f;
	mtxWorld._42 = 0.0f;
	mtxWorld._43 = 0.0f;

	D3DXVECTOR3 pos = GetPos();

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, GetVtx(), 0, sizeof(VERTEX_3D));

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, m_pTexture[m_nTexType]);    //テクスチャの設定

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数
			//カリングオン
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	//通常合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//位置によっては映らないようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// 作成
//=============================================================================
CThunderBill *CThunderBill::Create(D3DXVECTOR3 pos,
	D3DXVECTOR3 Size,
	D3DXVECTOR3 MinSize,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	int nTex,
	int nLife,
	float Destance,
	D3DXVECTOR2 TexMove,
	D3DXVECTOR2 TexNum,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	float fHigth,
	D3DXVECTOR3 orizinSize,
	int Synthetic,
	ANIMPATTERN AnimPattern)
{
	CThunderBill *pThunderBill = new CThunderBill(CObject::PRIORITY::EFFECT3D);

	if (pThunderBill != NULL)
	{
		pThunderBill->Init(pos, Size, MinSize, color, Mincolor, nTex, nLife, Destance, TexMove,
			TexNum, nAnimCounter, nSplit, fHigth,
			orizinSize,
			Synthetic, AnimPattern);
	}

	return pThunderBill;
}
