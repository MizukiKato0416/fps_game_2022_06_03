//=============================================================================
//
// �ݒ��ʂ̏��� [option.cpp]
// Author :��������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "option.h"
#include "option_bar.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "player.h"
#include "input_mouse.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define OPTION_MOUSE_SENSI_MIN			(30.0f)												//�}�E�X�̊��x���f�t�H���g���牽�p�[�Z���g�܂ŉ������悤�ɂ��邩
#define OPTION_MOUSE_SENSI_MAX			(170.0f)											//�}�E�X�̊��x���f�t�H���g���牽�p�[�Z���g�܂ŏグ���悤�ɂ��邩
#define OPTION_MOUSE_SENSI_DEFAULT		(100.0f)											//�}�E�X�̊��x�̍ŏ��̐ݒ���f�t�H���g�̉��p�[�Z���g�ɂ��邩
#define OPTION_BAR_SIZE					(D3DXVECTOR3(600.0f, 5.0f, 0.0f))					//�o�[�̃T�C�Y
#define OPTION_BAR_POS_01				(D3DXVECTOR3(710.0f, 285.0f, 0.0f))					//�o�[�̈ʒu
#define OPTION_BAR_POS_02				(D3DXVECTOR3(710.0f, 355.0f, 0.0f))					//�o�[�̈ʒu
#define OPTION_CIRCLE_SIZE				(D3DXVECTOR3(30.0f, 30.0f, 0.0f ))					//�~�̃T�C�Y
#define OPTION_SELECT_COLOR_ALPHA		(0.5f)												//�I���Ŕ����Ȃ鎞�̃��l
#define OPTION_DEFAULT_UI_POS			(D3DXVECTOR3(300.0f, 650.0f, 0.0f))					//�f�t�H���g�ɖ߂�UI�̈ʒu
#define OPTION_DEFAULT_UI_SIZE			(D3DXVECTOR3(414.0f, 76.0f, 0.0f))					//�f�t�H���g�ɖ߂�UI�̃T�C�Y
#define OPTION_TUTORIAL_UI_POS			(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 650.0f, 0.0f))	//���������\������UI�̈ʒu
#define OPTION_CLOSE_TUTORIAL_UI_POS	(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 50.0f, 0.0f))	//�����������UI�̈ʒu
#define OPTION_CLOSE_TUTORIAL_UI_SIZE	(D3DXVECTOR3(53.0f, 51.0f, 0.0f))					//�����������UI�̃T�C�Y

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
COption::COption(CObject::PRIORITY Priority) : CObject(Priority)
{
	memset(m_apOptionBar, NULL, sizeof(m_apOptionBar[OPTION_BAR_NUM]));
	memset(m_aNum, 0.0f, sizeof(m_aNum[OPTION_BAR_NUM]));
	m_pOptionFrame = nullptr;
	m_bOpen = false;
	m_pDefaultUi = nullptr;
	m_pTutorialUi = nullptr;
	m_pTutorial = nullptr;
	m_pCloseTutorialUi = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
COption::~COption()
{
	
}

//=============================================================================
// ����������
//=============================================================================
HRESULT COption::Init(void)
{
	//�ϐ�������
	m_bOpen = false;
	memset(m_apOptionBar, NULL, sizeof(m_apOptionBar[OPTION_BAR_NUM]));
	m_pOptionFrame = nullptr;
	m_pDefaultUi = nullptr;
	m_aNum[0] = OPTION_MOUSE_SENSI_DEFAULT;
	m_aNum[1] = OPTION_MOUSE_SENSI_DEFAULT;
	m_pTutorialUi = nullptr;
	m_pCloseTutorialUi = nullptr;
	m_pTutorial = nullptr;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void COption::Uninit(void)
{
	//�ݒ������Ƃ��̏���
	Close();

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void COption::Update(void)
{
	//�J���Ă����Ԃ�������
	if (m_bOpen)
	{
		//�`���[�g���A���摜����������Ă��Ȃ�������
		if (m_pTutorial == nullptr && m_pCloseTutorialUi == nullptr)
		{
			//�}�E�X���x�ݒ菈��
			MouseSensi();

			//ADS���x�ݒ菈��
			AdsSensi();

			//�f�t�H���g�ɖ߂�����
			Default();

			//�`���[�g���A���ɍs������
			Tutorial();
		}
		else
		{//��������Ă�����
			//�`���[�g���A������������
			TutorialCancel();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void COption::Draw(void)
{
	
}

//=============================================================================
// ����
//=============================================================================
COption *COption::Create(void)
{
	//�C���X�^���X�̐���
	COption *pOptionBar = nullptr;
	if (pOptionBar == nullptr)
	{
		pOptionBar = new COption();
		if (pOptionBar != nullptr)
		{
			pOptionBar->Init();
		}
	}
	return pOptionBar;
}

//=============================================================================
//�ݒ���J�������̏���
//=============================================================================
void COption::Open(void)
{
	//�J���Ă����Ԃł͂Ȃ�������
	if (!m_bOpen)
	{
		//�J���Ă����Ԃɂ���
		m_bOpen = true;

		//�t���[���̐���
		m_pOptionFrame = CObject2D::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f },
			                               (int)CObject::PRIORITY::OPTION);
		m_pOptionFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("game_option.png"));

		//�o�[�̐���
		m_apOptionBar[0] = COptionBar::Create(OPTION_BAR_POS_01, OPTION_BAR_SIZE, OPTION_CIRCLE_SIZE,
			                                  OPTION_MOUSE_SENSI_MIN, OPTION_MOUSE_SENSI_MAX, OPTION_MOUSE_SENSI_DEFAULT);
		//�����l�̐ݒ�
		m_apOptionBar[0]->SetNum(m_aNum[0]);
		//�l�̕ۑ�
		m_aNum[0] = m_apOptionBar[0]->GetNum();

		//�o�[�̐���
		m_apOptionBar[1] = COptionBar::Create(OPTION_BAR_POS_02, OPTION_BAR_SIZE, OPTION_CIRCLE_SIZE,
			                                  OPTION_MOUSE_SENSI_MIN, OPTION_MOUSE_SENSI_MAX, OPTION_MOUSE_SENSI_DEFAULT);
		//�����l�̐ݒ�
		m_apOptionBar[1]->SetNum(m_aNum[1]);
		//�l�̕ۑ�
		m_aNum[1] = m_apOptionBar[1]->GetNum();

		//�f�t�H���g�ɖ߂�UI�̐���
		m_pDefaultUi = CObject2D::Create(OPTION_DEFAULT_UI_POS, OPTION_DEFAULT_UI_SIZE,
			                             (int)CObject::PRIORITY::OPTION);
		m_pDefaultUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("default_ui.png"));

		//�`���[�g���A���ɍs��UI�̐���
		m_pTutorialUi = CObject2D::Create(OPTION_TUTORIAL_UI_POS, OPTION_DEFAULT_UI_SIZE,
			                              (int)CObject::PRIORITY::OPTION);
		m_pTutorialUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("tutorial_ui.png"));
	}
}

//=============================================================================
//�ݒ��������̏���
//=============================================================================
void COption::Close(void)
{
	//�J���Ă����Ԃ�������
	if (m_bOpen)
	{
		//���Ă����Ԃɂ���
		m_bOpen = false;

		//�ݒ�̃o�[�̍폜
		for (int nCntBar = 0; nCntBar < OPTION_BAR_NUM; nCntBar++)
		{
			if (m_apOptionBar[nCntBar] != nullptr)
			{
				m_apOptionBar[nCntBar]->Uninit();
				m_apOptionBar[nCntBar] = nullptr;
			}
		}

		//�ݒ��ʂ̃t���[���̍폜
		if (m_pOptionFrame != nullptr)
		{
			m_pOptionFrame->Uninit();
			m_pOptionFrame = nullptr;
		}

		//�f�t�H���g�ɖ߂�UI�̍폜
		if (m_pDefaultUi != nullptr)
		{
			m_pDefaultUi->Uninit();
			m_pDefaultUi = nullptr;
		}

		//�`���[�g���A���̍폜
		if (m_pTutorial != nullptr)
		{
			m_pTutorial->Uninit();
			m_pTutorial = nullptr;
		}

		//�`���[�g���A�����ʂ�UI�̍폜
		if (m_pTutorialUi != nullptr)
		{
			m_pTutorialUi->Uninit();
			m_pTutorialUi = nullptr;
		}

		//�`���[�g���A�������UI�̍폜
		if (m_pCloseTutorialUi != nullptr)
		{
			m_pCloseTutorialUi->Uninit();
			m_pCloseTutorialUi = nullptr;
		}
	}
}

//=============================================================================
//�}�E�X���x�ݒ菈��
//=============================================================================
void COption::MouseSensi(void)
{
	if (m_apOptionBar[0] != nullptr)
	{
		//�I�u�W�F�N�g��������|�C���^
		vector<CObject*> object;

		//�擪�̃|�C���^����
		object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
		int nProprty_Size = object.size();

		for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
		{
			//�I�u�W�F�N�g�̎�ނ��v���C���[��������
			if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
			{
				//�v���C���[�ɃL���X�g
				CPlayer *pPlayerObj = nullptr;
				pPlayerObj = (CPlayer*)object[nCnt];

				//���݂̐ݒ�̒l�擾
				float fNum = m_apOptionBar[0]->GetNum();
				D3DXVECTOR2 cameraSpeed = { PLAYER_CAMERA_V_MOUSE_SPEED_XZ, PLAYER_CAMERA_V_MOUSE_SPEED_Y };

				//�l���X�s�[�h�̔��f
				cameraSpeed *= fNum / 100.0f;

				//�ݒ�
				pPlayerObj->SetCameraSpeed(cameraSpeed);

				//�l�̕ۑ�
				m_aNum[0] = m_apOptionBar[0]->GetNum();
			}
		}
	}
}

//=============================================================================
//ADS���x�ݒ菈��
//=============================================================================
void COption::AdsSensi(void)
{
	if (m_apOptionBar[1] != nullptr)
	{
		//�I�u�W�F�N�g��������|�C���^
		vector<CObject*> object;

		//�擪�̃|�C���^����
		object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
		int nProprty_Size = object.size();

		for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
		{
			//�I�u�W�F�N�g�̎�ނ��v���C���[��������
			if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
			{
				//�v���C���[�ɃL���X�g
				CPlayer *pPlayerObj = nullptr;
				pPlayerObj = (CPlayer*)object[nCnt];

				//���݂̐ݒ�̒l�擾
				float fNum = m_apOptionBar[1]->GetNum();
				D3DXVECTOR2 adsCameraSpeed = { PLAYER_ADS_CAMERA_V_MOUSE_SPEED_XZ, PLAYER_ADS_CAMERA_V_MOUSE_SPEED_Y };

				//�l���X�s�[�h�̔��f
				adsCameraSpeed *= fNum / 100.0f;

				//�ݒ�
				pPlayerObj->SetAdsCameraSpeed(adsCameraSpeed);

				//�l�̕ۑ�
				m_aNum[1] = m_apOptionBar[1]->GetNum();
			}
		}
	}
}

//=============================================================================
//�f�t�H���g�ɖ߂�����
//=============================================================================
void COption::Default(void)
{
	//�}�E�X�擾����
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//�}�E�X�J�[�\���̈ʒu�擾
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	HWND hWind = CManager::GetWindowHandle();
	ScreenToClient(hWind, &mouse_pos);
	D3DXVECTOR2 mousePos = D3DXVECTOR2((float)mouse_pos.x, (float)mouse_pos.y);

	//�f�t�H���gUI�̈ʒu�ƃT�C�Y�擾
	D3DXVECTOR3 uiPos = m_pDefaultUi->GetPos();
	D3DXVECTOR3 uiSize = m_pDefaultUi->GetSize();

	//UI�̒����ɂ�������
	if (mousePos.x >= uiPos.x - uiSize.x / 2.0f && mousePos.x <= uiPos.x + uiSize.x / 2.0f &&
		mousePos.y >= uiPos.y - uiSize.y / 2.0f && mousePos.y <= uiPos.y + uiSize.y / 2.0f)
	{
		//�F�擾
		D3DXCOLOR uiCol = m_pDefaultUi->GetCol();
		//��������
		uiCol.a = OPTION_SELECT_COLOR_ALPHA;
		//�F�ݒ�
		m_pDefaultUi->SetCol(uiCol);

		//�N���b�N������
		if (pInputMouse->GetTrigger(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
		{
			//�ݒ�̒l��S�ăf�t�H���g�̏�Ԃɂ���
			for (int nCntBar = 0; nCntBar < OPTION_BAR_NUM; nCntBar++)
			{
				if (m_apOptionBar[nCntBar] != nullptr)
				{
					//�l���f�t�H���g�ɐݒ肷��
					m_apOptionBar[nCntBar]->SetNum(m_apOptionBar[nCntBar]->GetDefaultNum());
					//�l�̕ۑ�
					m_aNum[nCntBar] = m_apOptionBar[nCntBar]->GetNum();
					//�f�t�H���g�ɖ߂���Ԃɐݒ肷��
					m_apOptionBar[nCntBar]->SetDefault(true);
				}
			}
		}
	}
	else
	{
		//�F�擾
		D3DXCOLOR uiCol = m_pDefaultUi->GetCol();
		//�F�����ɖ߂�
		uiCol.a = 1.0f;
		//�F�ݒ�
		m_pDefaultUi->SetCol(uiCol);
	}
}

//=============================================================================
//�`���[�g���A�����ʂ�����
//=============================================================================
void COption::Tutorial(void)
{
	//�}�E�X�擾����
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//�}�E�X�J�[�\���̈ʒu�擾
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	HWND hWind = CManager::GetWindowHandle();
	ScreenToClient(hWind, &mouse_pos);
	D3DXVECTOR2 mousePos = D3DXVECTOR2((float)mouse_pos.x, (float)mouse_pos.y);

	//�f�t�H���gUI�̈ʒu�ƃT�C�Y�擾
	D3DXVECTOR3 uiPos = m_pTutorialUi->GetPos();
	D3DXVECTOR3 uiSize = m_pTutorialUi->GetSize();

	//UI�̒����ɂ�������
	if (mousePos.x >= uiPos.x - uiSize.x / 2.0f && mousePos.x <= uiPos.x + uiSize.x / 2.0f &&
		mousePos.y >= uiPos.y - uiSize.y / 2.0f && mousePos.y <= uiPos.y + uiSize.y / 2.0f)
	{
		//�F�擾
		D3DXCOLOR uiCol = m_pTutorialUi->GetCol();
		//��������
		uiCol.a = OPTION_SELECT_COLOR_ALPHA;
		//�F�ݒ�
		m_pTutorialUi->SetCol(uiCol);

		//�N���b�N������
		if (pInputMouse->GetTrigger(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
		{
			//�`���[�g���A���摜����������ĂȂ��Ȃ�
			if (m_pTutorial == nullptr)
			{
				//�`���[�g���A���摜�𐶐�����
				m_pTutorial = CObject2D::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f },
					                             (int)CObject::PRIORITY::OPTION);
				m_pTutorial->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("tutorial.jpg"));
			}

			//�`���[�g���A���L�����Z��UI����������ĂȂ��Ȃ�
			if (m_pCloseTutorialUi == nullptr)
			{
				//�`���[�g���A���L�����Z��UI�𐶐�����
				m_pCloseTutorialUi = CObject2D::Create(OPTION_CLOSE_TUTORIAL_UI_POS, OPTION_CLOSE_TUTORIAL_UI_SIZE,
					                                   (int)CObject::PRIORITY::OPTION);
				m_pCloseTutorialUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("cancel_ui.png"));
			}
		}
	}
	else
	{
		//�F�擾
		D3DXCOLOR uiCol = m_pTutorialUi->GetCol();
		//�F�����ɖ߂�
		uiCol.a = 1.0f;
		//�F�ݒ�
		m_pTutorialUi->SetCol(uiCol);
	}
}

//=============================================================================
//�`���[�g���A��������UI�̏���
//=============================================================================
void COption::TutorialCancel(void)
{
	//�}�E�X�擾����
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//�}�E�X�J�[�\���̈ʒu�擾
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	HWND hWind = CManager::GetWindowHandle();
	ScreenToClient(hWind, &mouse_pos);
	D3DXVECTOR2 mousePos = D3DXVECTOR2((float)mouse_pos.x, (float)mouse_pos.y);

	//�f�t�H���gUI�̈ʒu�ƃT�C�Y�擾
	D3DXVECTOR3 uiPos = m_pCloseTutorialUi->GetPos();
	D3DXVECTOR3 uiSize = m_pCloseTutorialUi->GetSize();

	//UI�̒����ɂ�������
	if (mousePos.x >= uiPos.x - uiSize.x / 2.0f && mousePos.x <= uiPos.x + uiSize.x / 2.0f &&
		mousePos.y >= uiPos.y - uiSize.y / 2.0f && mousePos.y <= uiPos.y + uiSize.y / 2.0f)
	{
		//�F�擾
		D3DXCOLOR uiCol = m_pCloseTutorialUi->GetCol();
		//��������
		uiCol.a = OPTION_SELECT_COLOR_ALPHA;
		//�F�ݒ�
		m_pCloseTutorialUi->SetCol(uiCol);

		//�N���b�N������
		if (pInputMouse->GetTrigger(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
		{
			//�`���[�g���A���̍폜
			if (m_pTutorial != nullptr)
			{
				m_pTutorial->Uninit();
				m_pTutorial = nullptr;
			}

			//�`���[�g���A�������UI�̍폜
			if (m_pCloseTutorialUi != nullptr)
			{
				m_pCloseTutorialUi->Uninit();
				m_pCloseTutorialUi = nullptr;
			}
		}
	}
	else
	{
		//�F�擾
		D3DXCOLOR uiCol = m_pCloseTutorialUi->GetCol();
		//�F�����ɖ߂�
		uiCol.a = 1.0f;
		//�F�ݒ�
		m_pCloseTutorialUi->SetCol(uiCol);
	}
}
