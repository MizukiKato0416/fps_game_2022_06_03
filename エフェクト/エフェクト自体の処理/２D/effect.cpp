//*****************************************************************************
// エフェクト処理 [Effect.cpp]
// Author : 佐藤秀亮
//*****************************************************************************

#include "Effect.h"
#include "manager.h"
#include "Renderer.h"
#include "control.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CEffect::CEffect(int nPriority) : CScene2D(nPriority)
{
	m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_MinColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CEffect::~CEffect()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size, D3DXVECTOR2 MinSize, int nLife, int nType,int Synthetic)
{
	CScene2D::Init(pos);

	m_Color = color;
	m_MinColor = Mincolor;

	m_Size = Size;
	m_MinSize = MinSize;

	m_nLife = nLife;
	m_bUninit = false;
	
	nSynthetic = Synthetic;
	CScene2D::SetTexture(nType);	//選択した番号のテクスチャを貼る
	CScene2D::SetWhidth(m_Size.x);
	CScene2D::SetHight(m_Size.y);

	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CEffect::Uninit()
{
	CScene2D::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CEffect::Update()
{
	D3DXVECTOR3 pos = GetPosition();

	m_Size += m_MinSize;

	//サイズ変更
	SetWhidth(m_Size.x);
	SetHight(m_Size.y);

	m_nLife--;		//ライフ減算

	//画面外処理
	if (pos.y < 0)
	{
		m_bUninit = true;
	}
	else if (pos.y > SCREEN_HEIGHT)
	{
		m_bUninit = true;
	}
	if (pos.x < 0)
	{
		m_bUninit = true;
	}
	else if (pos.x > SCREEN_WIDTH)
	{
		m_bUninit = true;
	}


	//高さか幅が0になったら
	if (m_Size.x < 0 || m_Size.y < 0)
	{
		m_bUninit = true;
	}
	//寿命が0になったら
	if (m_nLife < 0)
	{
		m_bUninit = true;
	}

	//カラー変更
	m_Color.r += m_MinColor.r;
	m_Color.g += m_MinColor.g;
	m_Color.b += m_MinColor.b;
	m_Color.a += m_MinColor.a;

	//カラーが0以下の時
	if (m_Color.r < 0)
	{
		m_Color.r = 0;
	}
	if (m_Color.g < 0)
	{
		m_Color.g = 0;
	}
	if (m_Color.b < 0)
	{
		m_Color.b = 0;
	}
	if (m_Color.a < 0)
	{
		m_Color.a = 0;
	}

	//カラーが255以上の時
	if (m_Color.r > 255)
	{
		m_Color.r = 255;
	}
	if (m_Color.g > 255)
	{
		m_Color.g = 255;
	}
	if (m_Color.b > 255)
	{
		m_Color.b = 255;
	}
	if (m_Color.a > 255)
	{
		m_Color.a = 255;
	}
	ColorChange(m_Color);

	//破棄
	if (m_bUninit == true)
	{
		Uninit();
	}

}

//*****************************************************************************
// 描画
//*****************************************************************************
void CEffect::Draw()
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスの取得

	//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (nSynthetic == 0)
	{
		//加算合成関係
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else if (nSynthetic == 1)
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

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	CScene2D::Draw();

	//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	//通常合成に戻す(加算合成)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


