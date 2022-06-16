//=============================================================================
// 弾痕(仮称)処理 [BulletHoll.cpp]
// Author : 佐藤秀亮
//=============================================================================
#include "BulletHoll.h"
#include "renderer.h"
#include "manager.h"


//=============================================================================
// コンストラクタ
//=============================================================================
CBulletHoll::CBulletHoll(int nPriority) : CBillEffect(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBulletHoll::~CBulletHoll()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CBulletHoll::Init(D3DXVECTOR3 size,
	D3DXVECTOR3 Addsize,
	D3DXVECTOR3 pos,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	int nLife,
	int nTex,
	D3DXVECTOR2 TexMove,
	D3DXVECTOR2 TexNum,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	D3DXVECTOR3 rot,
	int Synthetic,
	ANIMPATTERN AnimPattern)
{
	CBillEffect::Init(size, Addsize, color, Mincolor, nTex, nLife, TexNum, TexMove, nAnimCounter, nSplit, AnimPattern);
	m_nSynthenic = Synthetic;
	m_Size = size;
	m_Rot = rot;

	//m_pos1 = D3DXVECTOR3(
	//	pos.x + size.x * (cosf(m_Rot.y)),
	//	pos.y /*+ size.x / 2*/,
	//	pos.z + size.x * (sinf(-m_Rot.y)));
	//m_pos2 = D3DXVECTOR3(
	//	pos.x + (sinf(-m_Rot.y))  * size.x,
	//	pos.y /*- size.x / 2*/,
	//	pos.z - (cosf(m_Rot.y))* size.x);
	//m_pos3 = D3DXVECTOR3(
	//	pos.x - (sinf(-m_Rot.y))  * size.x,
	//	pos.y /*- size.x / 2*/,
	//	pos.z + (cosf(m_Rot.y))* size.x);
	//m_pos4 = D3DXVECTOR3(
	//	pos.x - (cosf(m_Rot.y)) * size.x,
	//	pos.y /*- size.x / 2*/,
	//	pos.z - (sinf(-m_Rot.y))* size.x);

	m_pos1 = D3DXVECTOR3(
		pos.x - (cosf(-m_Rot.y) * size.x / 2),
		pos.y + size.x / 2,
		pos.z - (sinf(-m_Rot.y) * size.x / 2));
	m_pos2 = D3DXVECTOR3(
		pos.x + (cosf(-m_Rot.y)) * size.x / 2,
		pos.y + size.x / 2,
		pos.z + (sinf(-m_Rot.y)) * size.x / 2);
	m_pos3 = D3DXVECTOR3(
		pos.x - (cosf(-m_Rot.y) * size.x / 2),
		pos.y - size.x / 2,
		pos.z - (sinf(-m_Rot.y) * size.x / 2));
	m_pos4 = D3DXVECTOR3(
		pos.x + (cosf(-m_Rot.y)) * size.x / 2,
		pos.y - size.x / 2,
		pos.z + (sinf(-m_Rot.y)) * size.x / 2);

	//m_pos1 = D3DXVECTOR3(
	//	pos.x + size.x,
	//	pos.y /*+ size.x / 2*/,
	//	pos.z + size.x);
	//m_pos2 = D3DXVECTOR3(
	//	pos.x + size.x,
	//	pos.y /*- size.x / 2*/,
	//	pos.z - size.x);
	//m_pos3 = D3DXVECTOR3(
	//	pos.x - size.x,
	//	pos.y /*- size.x / 2*/,
	//	pos.z + size.x);
	//m_pos4 = D3DXVECTOR3(
	//	pos.x - size.x,
	//	pos.y /*- size.x / 2*/,
	//	pos.z - size.x);
	//CScene3d::SetRot(m_Rot);


	CPlane::SetPosBill(m_pos1, m_pos2, m_pos3, m_pos4);
	m_nSynthenic = Synthetic;
	return S_OK;

}

//=============================================================================
// 終了
//=============================================================================
void CBulletHoll::Uninit()
{
	CBillEffect::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CBulletHoll::Update()
{
	CPlane::SetPosBill(m_pos1, m_pos2, m_pos3, m_pos4);
	CBillEffect::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CBulletHoll::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する

														//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

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
	//カリングオフ
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	CPlane::Draw();
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
CBulletHoll *CBulletHoll::Create(D3DXVECTOR3 size,
	D3DXVECTOR3 Addsize,
	D3DXVECTOR3 pos,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	int nLife,
	int nTex,
	D3DXVECTOR2 TexMove,
	D3DXVECTOR2 TexNum,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	D3DXVECTOR3 rot,
	int Synthetic,
	ANIMPATTERN AnimPattern)
{
	CBulletHoll *pBulletHoll = new CBulletHoll(CManager::PRIORITY_EFFECT);

	if (pBulletHoll != NULL)
	{
		pBulletHoll->Init(size, Addsize, pos, color, Mincolor, nLife, nTex, TexMove, TexNum, nAnimCounter, nSplit,
			rot, Synthetic, AnimPattern);
	}

	return pBulletHoll;

}