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

//=============================================================================
// �}�N����`
//=============================================================================
#define OPTION_MOUSE_SENSI_MIN		(30.0f)								//�}�E�X�̊��x���f�t�H���g���牽�p�[�Z���g�܂ŉ������悤�ɂ��邩
#define OPTION_MOUSE_SENSI_MAX		(170.0f)							//�}�E�X�̊��x���f�t�H���g���牽�p�[�Z���g�܂ŏグ���悤�ɂ��邩
#define OPTION_MOUSE_SENSI_DEFAULT	(100.0f)							//�}�E�X�̊��x�̍ŏ��̐ݒ���f�t�H���g�̉��p�[�Z���g�ɂ��邩
#define OPTION_BAR_SIZE				(D3DXVECTOR3(600.0f, 5.0f, 0.0f))	//�o�[�̃T�C�Y
#define OPTION_CIRCLE_SIZE			(D3DXVECTOR3(30.0f, 30.0f, 0.0f ))	//�~�̃T�C�Y

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
	m_aNum[0] = OPTION_MOUSE_SENSI_DEFAULT;
	m_aNum[1] = 100.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void COption::Uninit(void)
{
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
		//�}�E�X���x�ݒ菈��
		MouseSensi();

		//����p�ݒ菈��
		ViewAngle();
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
			                               (int)CObject::PRIORITY::UI);
		m_pOptionFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("game_option.png"));

		//�o�[�̐���
		m_apOptionBar[0] = COptionBar::Create({ 710.0f, 285.0f, 0.0f }, OPTION_BAR_SIZE, OPTION_CIRCLE_SIZE,
			                                  OPTION_MOUSE_SENSI_MIN, OPTION_MOUSE_SENSI_MAX, m_aNum[0]);
		//�l�̕ۑ�
		m_aNum[0] = m_apOptionBar[0]->GetNum();

		m_apOptionBar[1] = COptionBar::Create({ 710.0f, 355.0f, 0.0f }, OPTION_BAR_SIZE, OPTION_CIRCLE_SIZE,
			                                  50.0f, 150.0f, m_aNum[1]);
		//�l�̕ۑ�
		m_aNum[1] = m_apOptionBar[1]->GetNum();
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
				D3DXVECTOR2 cameraSpeed = { PLAYER_CAMERA_V__MOUSE_SPEED_XZ, PLAYER_CAMERA_V__MOUSE_SPEED_Y };

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
//����p�ݒ菈��
//=============================================================================
void COption::ViewAngle(void)
{

}
