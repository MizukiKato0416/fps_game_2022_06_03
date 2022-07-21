//=============================================================================
//
// �^�C�g������
// Author : ��������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "title.h"
#include "manager.h"
#include "fade.h"
#include "ui.h"
#include "object2D.h"
#include "input_mouse.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTitle::CTitle(CObject::PRIORITY Priority):CObject(Priority)
{
	
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	FILE *pFile;
	string FileString;
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Size;
	D3DXVECTOR3 Stop;
	int nBlinkingType = 0;
	int nBlinkingSpeed = 0;
	int nMoveType = 0;
	float fMoveSpeed;
	char aFile[1][128];

	pFile = fopen("data/titledata.txt", "r");

	if (pFile != NULL)
	{
		int nCnt = 0;
		while (true)
		{
			fscanf(pFile, "%s", aFile[0]);
			if (strcmp(aFile[0], "SET_UI") == 0) // SET_UI�̕��������������
			{
				while (true)
				{
					fscanf(pFile, "%s", aFile[0]);
					// TEX_TYPE�̕��������������
					if (strcmp(aFile[0], "TEX_TYPE") == 0) // TEX_TYPE�̕��������������
					{
						char aTypeBuf[1][64];
						string Type;
						fscanf(pFile, "%*s%s", aTypeBuf[0]);
						Type = aTypeBuf[0];
						m_type.push_back(Type);
					}
					// POS�̕��������������
					else if (strcmp(aFile[0], "POS") == 0)
					{
						fscanf(pFile, "%*s%f%*s%f%*s%f", &Pos.x, &Pos.y, &Pos.z);
					}
					// SIZE�̕��������������
					else if (strcmp(aFile[0], "SIZE") == 0)
					{
						fscanf(pFile, "%*s%f%*s%f%*s%f", &Size.x, &Size.y, &Size.z);
					}
					// BLINKING�̕��������������
					else if (strcmp(aFile[0], "BLINKING") == 0)
					{
						fscanf(pFile, "%*s%d", &nBlinkingType);
					}
					// BLINKING_SPEED�̕��������������
					else if (strcmp(aFile[0], "BLINKING_SPEED") == 0)
					{
						fscanf(pFile, "%*s%d", &nBlinkingSpeed);
					}
					// MOVE�̕��������������
					else if (strcmp(aFile[0], "MOVE") == 0)
					{
						fscanf(pFile, "%*s%d", &nMoveType);
					}
					// MOVE_SPEED�̕��������������
					else if (strcmp(aFile[0], "MOVE_SPEED") == 0)
					{
						fscanf(pFile, "%*s%f", &fMoveSpeed);
					}
					// STOP_POINT�̕��������������
					else if (strcmp(aFile[0], "STOP_POINT") == 0)
					{
						fscanf(pFile, "%*s%f%*s%f%*s%f", &Stop.x, &Stop.y, &Stop.z);
					}
					// END_UISET�̕��������������
					else if (strcmp(aFile[0], "END_UISET") == 0)
					{
						m_ui.push_back(CUi::Create(Pos, Size, Stop, nBlinkingType, nMoveType, nBlinkingSpeed, fMoveSpeed, m_type[nCnt]));
						nCnt++;
						break;
					}
				}
			}
			else if (strcmp(aFile[0], "END") == 0)
			{
				break;
			}
		}
		fclose(pFile);
	}

	m_pointor = CObject2D::Create({ 0.0f, 0.0f, 0.0f }, { 83.0f * 0.9f, 74.0f * 0.9f, 0.0f }, (int)CObject::PRIORITY::UI);
	m_pointor->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("pointer.png"));
	//ShowCursor(FALSE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	ScreenToClient(CManager::GetWindowHandle(), &mouse_pos);
	CInputMouse *mouse = CManager::GetInstance()->GetInputMouse();
	D3DXVECTOR3 pointor_size = m_pointor->GetSize();
	D3DXVECTOR3 pointor_pos = { (float)mouse_pos.x, (float)mouse_pos.y, 0.0f };
	int ui_size = m_ui.size();

	m_pointor->SetPos(pointor_pos, pointor_size);

	for (int count_ui = 0; count_ui < ui_size; count_ui++)
	{
		if (m_ui[count_ui]->GetTexName() == "signboard.png")
		{
			D3DXCOLOR col = m_ui[count_ui]->GetCol();
			if ((pointor_pos.x + pointor_size.x) >= (m_ui[count_ui]->GetPos().x - (m_ui[count_ui]->GetSize().x / 2)) &&
				(pointor_pos.x - pointor_size.x) <= (m_ui[count_ui]->GetPos().x + (m_ui[count_ui]->GetSize().x / 2)) &&
				(pointor_pos.y + pointor_size.y) >= (m_ui[count_ui]->GetPos().y - (m_ui[count_ui]->GetSize().y / 2)) &&
				(pointor_pos.y - pointor_size.y) <= (m_ui[count_ui]->GetPos().y + (m_ui[count_ui]->GetSize().y / 2)))
			{
				col.a = 0.5f;
				if (mouse->GetPress(mouse->MOUSE_TYPE_LEFT) == true)
				{
					CFade *fade = CManager::GetInstance()->GetFade();

					if (fade->GetFade() == CFade::FADE_NONE)
					{
						fade->SetFade(CManager::MODE::GAME01);
					}
				}
			}
			else
			{
				col.a = 1.0f;
			}
			m_ui[count_ui]->SetCol(col);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{
	
}