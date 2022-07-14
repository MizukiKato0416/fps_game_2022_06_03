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
CEffect::CEffect(PRIORITY priorty) : CScene2D(priorty)
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
HRESULT CEffect::Init(D3DXVECTOR3 pos,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	D3DXVECTOR2 Size,
	D3DXVECTOR2 MinSize,
	int nLife, string nType,
	int Synthetic,
	D3DXVECTOR2 TexNum,
	D3DXVECTOR2 TexMove,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	ANIMPATTERN AnimPattern)
{
	CScene2D::Init();

	m_pos = pos;
	m_Color = color;
	m_MinColor = Mincolor;

	m_Size = Size;
	m_MinSize = MinSize;
	m_pTexture = nType;
	m_nLife = nLife;
	m_bUninit = false;
	m_nAnimCount = nAnimCounter;
	m_nSetAnimCnt = nAnimCounter;
	m_AnimPattern = AnimPattern;
	m_MaxSplit = nSplit;
	m_TexNum = TexMove;
	m_TexMove = TexNum;
	m_PatternSize = D3DXVECTOR2(1.0f / m_MaxSplit.x, 1.0f / m_MaxSplit.y);

	if (m_MaxSplit.x <= 0)
	{
		m_MaxSplit.x = 1;
	}
	if (m_MaxSplit.y <= 0)
	{
		m_MaxSplit.y = 1;
	}

	float SplitU = float(rand() % (int)m_MaxSplit.x) + 1;
	float SplitV = float(rand() % (int)m_MaxSplit.y) + 1;
	m_nSplit = nSplit;

	if (m_AnimPattern == ANIMPATTERN_RAND)
	{
		m_nSplit.x = SplitU;
		m_nSplit.y = SplitV;
	}

	m_nSynthetic = Synthetic;
	ColorChange(m_Color);

	SetTexAnim(m_nSplit, m_PatternSize);

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
	switch (m_AnimPattern)
	{
	case(ANIMPATTERN_NOMAL):
		//テクスチャアニメーション
		if (m_nAnimCount >= 0)
		{
			m_nAnimCount--;
			if (m_nAnimCount < 0)
			{
				m_nAnimCount = m_nSetAnimCnt;
				m_nSplit.x++;
				m_nSplit.y++;
			}
			if (m_MaxSplit > m_MaxSplit)
			{
				m_nSplit.x = 0;
				m_nSplit.y = 0;
			}
		}
		break;
	case(ANIMPATTERN_RAND):
		////テクスチャアニメーション
		//if (m_nAnimCount >= 0)
		//{
		//	m_nAnimCount--;
		//	if (m_nAnimCount < 0)
		//	{
		//		m_nAnimCount = m_nSetAnimCnt;
		//		m_nSplit.x++;
		//		m_nSplit.y++;
		//	}
		//	if (m_MaxSplit > m_MaxSplit)
		//	{
		//		m_nSplit.x = 0;
		//		m_nSplit.y = 0;
		//	}
		//}

		break;
	default:
		//テクスチャアニメーション
		if (m_nAnimCount >= 0)
		{
			m_nAnimCount--;
			if (m_nAnimCount < 0)
			{
				m_nAnimCount = m_nSetAnimCnt;
				m_nSplit.x++;
				m_nSplit.y++;
			}
			if (m_MaxSplit > m_MaxSplit)
			{
				m_nSplit.x = 0;
				m_nSplit.y = 0;
			}
		}
		break;
	}

	if (m_TexMove.x >= 1.0f)
	{
		m_TexMove.x -= 1.0f;
	}
	else if (m_TexMove.x < 0.0f)
	{
		m_TexMove.x += 1.0f;
	}

	if (m_TexMove.y >= 1.0f)
	{
		m_TexMove.y -= 1.0f;
	}
	else if (m_TexMove.y < 0.0f)
	{
		m_TexMove.y += 1.0f;
	}
	TexMove(m_TexMove);
	SetTexAnim(m_nSplit, m_PatternSize);

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

	if (m_nSynthetic == 0)
	{
		//加算合成関係
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else if (m_nSynthetic == 1)
	{
		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else if (m_nSynthetic == 2)
	{
		//合成無し
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


