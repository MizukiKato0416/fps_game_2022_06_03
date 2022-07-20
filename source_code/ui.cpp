//=============================================================================
//
// タイトル処理
// Author : 加藤瑞葵
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "ui.h"
#include "manager.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CUi::CUi(CObject::PRIORITY Priority) :CObject2D(Priority)
{

}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CUi::~CUi()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUi::Init(void)
{
	CObject2D::Init();
	CObject2D::SetPos(m_pos, m_size);
	CObject2D::SetSize(m_size);
	CObject2D::BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(m_tex));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUi::Uninit(void)
{
	//オブジェクトの破棄
	CObject2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CUi::Update(void)
{
	D3DXVECTOR3 Pos = CObject2D::GetPos();
	D3DXVECTOR3 Size = CObject2D::GetSize();
	D3DXCOLOR col = CObject2D::GetCol();

	if (m_BlinkingType != BLINKING_TYPE::NONE)
	{
		m_nTimer++;
		if (m_nTimer >= m_nBlinkingSpeed)
		{
			switch (m_BlinkingType)
			{
			case BLINKING_TYPE::BLINKING:
				if (col.a >= 1.0f)
				{
					col.a = 0.0f;
				}
				else if (col.a <= 0.0f)
				{
					col.a = 255.0f;
				}
				break;
			case BLINKING_TYPE::FADE:
				if (col.a <= 0.0f)
				{
					m_bChange = true;
				}
				else if (col.a >= 1.0f)
				{
					m_bChange = false;
				}
				if (m_bChange == true)
				{
					col.a += 0.1f;
				}
				else if (m_bChange == false)
				{
					col.a -= 0.1f;
				}
				break;
			default:
				break;
			}
			m_nTimer = 0;
		}
	}
	if (m_MoveType != MOVE_TYPE::NONE)
	{
		switch (m_MoveType)
		{
		case MOVE_TYPE::LEFT_TO_RIGHT:
			if (Pos.x <= m_stopPoint.x)
			{
				Pos.x += m_fMoveSpeed;
			}
			break;
		case MOVE_TYPE::RIGHT_TO_LEFT:
			if (Pos.x >= m_stopPoint.x)
			{
				Pos.x -= m_fMoveSpeed;
			}
			break;
		case MOVE_TYPE::TOP_TO_BOTTOM:
			if (Pos.y <= m_stopPoint.y)
			{
				Pos.y += m_fMoveSpeed;
			}
			break;
		case MOVE_TYPE::BOTTOM_TO_TOP:
			if (Pos.y >= m_stopPoint.y)
			{
				Pos.y -= m_fMoveSpeed;
			}
			break;
		default:
			break;
		}
	}

	CObject2D::SetPos(Pos, Size);
	CObject2D::SetCol(col);
	CObject2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CUi::Draw(void)
{
	CObject2D::Draw();
}

CUi *CUi::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXVECTOR3 &stop, const int &blinkingtype, const int &movetype, const int &blinkingspeed, const int &movespeed, string tex)
{
	CUi *pUi = nullptr;
	if (pUi == nullptr)
	{
		pUi = new CUi;
		if (pUi != nullptr)
		{
			pUi->m_pos = pos;
			pUi->m_size = size;
			pUi->m_stopPoint = stop;
			pUi->m_tex = tex;
			pUi->m_BlinkingType = (BLINKING_TYPE)blinkingtype;
			pUi->m_MoveType = (MOVE_TYPE)movetype;
			pUi->m_nBlinkingSpeed = blinkingspeed;
			pUi->m_fMoveSpeed = movespeed;
			pUi->Init();
		}
	}
	return pUi;
}