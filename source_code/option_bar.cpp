//=============================================================================
//
// �ݒ��ʂ̃o�[�̏��� [option_bar.cpp]
// Author :��������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "option_bar.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "input_mouse.h"
#include "counter.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define OPTION_BAR_COUNETR_POS		(25.0f)									//���lUI�ʒu���ߒl
#define OPTION_BAR_COUNETR_SIZE		(D3DXVECTOR3(20.0f, 30.0f, 0.0f))		//���lUI�T�C�Y
#define OPTION_BAR_COUNETR_NUM		(3)										//���lUI����

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
COptionBar::COptionBar(CObject::PRIORITY Priority) : CObject(Priority)
{
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_barSize = { 0.0f, 0.0f, 0.0f };
	m_circleSize = { 0.0f, 0.0f, 0.0f };
	m_fMin = 0.0f;
	m_fMax = 0.0f;
	m_fDefault = 0.0f;
	m_fNum = 0.0f;
	m_pCircle = nullptr;
	m_pBar = nullptr;
	m_bClick = false;
	m_pCounter = nullptr;
	m_bDefault = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
COptionBar::~COptionBar()
{
	
}

//=============================================================================
// ����������
//=============================================================================
HRESULT COptionBar::Init(void)
{
	//�ϐ�������
	m_bClick = false;
	m_fNum = m_fDefault;
	m_bDefault = false;

	SetObjType(CObject::OBJTYPE::UI);

	//�o�[�̐���
	m_pBar = CObject2D::Create(m_pos, m_barSize, (int)CObject::PRIORITY::OPTION);

	//���l�̐���
	m_pCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + m_barSize.x / 2.0f + OPTION_BAR_COUNETR_POS, m_pos.y , 0.0f),
		                          OPTION_BAR_COUNETR_SIZE,
		                          OPTION_BAR_COUNETR_NUM, "Number.png");
	//���l�̐ݒ�
	m_pCounter->SetCounterNum(int(m_fDefault));
	m_pCounter->SetPriority((int)CObject::PRIORITY::OPTION);


	//�~�̐���
	m_pCircle = CObject2D::Create(m_pos, m_circleSize, (int)CObject::PRIORITY::OPTION);
	m_pCircle->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("circle.png"));

	//�~�̈ʒu�ۑ��p
	D3DXVECTOR3 circlePos = m_pos;

	//�ő�l�ƍŏ��l�̍��������߂�
	float fDiffer = m_fMax - m_fMin;
	//�f�t�H���g�̉~�̈ʒu���ǂ��ɂ��邩�����߂邽�߂Ƀo�[�ɑ΂��Ăǂ��ɂ���̂��䗦�����߂�
	float fDefaultRatio = (m_fDefault - m_fMin) * 100.0f / fDiffer;
	//�o�����䗦�ŏꏊ�����߂�
	circlePos.x = (m_pos.x - m_barSize.x / 2.0f) + (m_barSize.x * (fDefaultRatio / 100.0f));
	//�~�̈ʒu��ݒ�
	m_pCircle->SetPos(circlePos, m_pCircle->GetSize());

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void COptionBar::Uninit(void)
{
	//�~�̍폜
	if (m_pCircle != nullptr)
	{
		m_pCircle->Uninit();
		m_pCircle = nullptr;
	}

	//�o�[�̍폜
	if (m_pBar != nullptr)
	{
		m_pBar->Uninit();
		m_pBar = nullptr;
	}

	//���l�̍폜
	if (m_pCounter != nullptr)
	{
		m_pCounter->Uninit();
		m_pCounter = nullptr;
	}

	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void COptionBar::Update(void)
{
	//�}�E�X�擾����
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//�N���b�N������
	if (pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
	{
		//�}�E�X�J�[�\���̈ʒu�擾
		POINT mouse_pos;
		GetCursorPos(&mouse_pos);
		
		HWND hWind = CManager::GetWindowHandle();

		//RECT lprc = {};
		//if (GetClientRect(hWind, &lprc))
		//{
		//	SetWindowPos(
		//	// �E�B���h�E�n���h��
		//	hWind,
		//	// �z�u�����̃n���h��(NULL�ł悵)
		//	NULL,
		//	// �\�����WX
		//	CW_USEDEFAULT,
		//	// �\�����WY
		//	CW_USEDEFAULT,
		//	// ���T�C�Y����		
		//	lprc.right,
		//	// ���T�C�Y�c��
		//	lprc.bottom,
		//	// SWP_NOMOVE => �ʒu�ύX�Ȃ�
		//	SWP_NOMOVE);
		//}

		ScreenToClient(hWind, &mouse_pos);
		D3DXVECTOR2 mousePos = D3DXVECTOR2((float)mouse_pos.x, (float)mouse_pos.y);

		//�~�̈ʒu�ƃT�C�Y�擾
		D3DXVECTOR3 circlePos = m_pCircle->GetPos();
		D3DXVECTOR3 circleSize = m_pCircle->GetSize();

		//�N���b�N������
		if (pInputMouse->GetTrigger(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
		{
			//�~�̒����N���b�N���Ă�����
			if (mousePos.x >= circlePos.x - circleSize.x / 2.0f && mousePos.x <= circlePos.x + circleSize.x / 2.0f &&
				mousePos.y >= circlePos.y - circleSize.y / 2.0f && mousePos.y <= circlePos.y + circleSize.y / 2.0f)
			{
				//�N���b�N���Ă��Ȃ���ԂȂ�
				if (!m_bClick)
				{
					//�N���b�N������Ԃɂ���
					m_bClick = true;
				}
			}
		}

		//�N���b�N���Ă����ԂȂ�
		if (m_bClick)
		{
			//�~��X�ʒu���}�E�X��X�ʒu�ɂ���
			circlePos.x = mousePos.x;

			//�o�[�̈ʒu�A�T�C�Y���擾
			D3DXVECTOR3 barPos = m_pBar->GetPos();
			D3DXVECTOR3 barSize = m_pBar->GetSize();

			//�~�̈ʒu���o�[�̍��[�����������Ȃ�����
			if (circlePos.x < barPos.x - barSize.x / 2.0f)
			{
				//�~�̈ʒu���o�[�̍��[�ɂ���
				circlePos.x = barPos.x - barSize.x / 2.0f;
			}
			else if (circlePos.x > barPos.x + barSize.x / 2.0f)
			{//�~�̈ʒu���o�[�̉E�[�����������Ȃ�����
				//�~�̈ʒu���o�[�̉E�[�ɂ���
				circlePos.x = barPos.x + barSize.x / 2.0f;
			}
			
			//�o�[�̑傫���ɑ΂��錻�݂̉~�̈ʒu�̔䗦�����߂�
			float fRatio = (circlePos.x - (barPos.x - barSize.x / 2.0f)) /
				           ((barPos.x + barSize.x / 2.0f) - (barPos.x - barSize.x / 2.0f));

			//�䗦���猻�݂̒l�����߂�
			m_fNum = (m_fMax - m_fMin) * fRatio + m_fMin;

			//�~�̈ʒu��ݒ�
			m_pCircle->SetPos(circlePos, m_pCircle->GetSize());
		}
	}

	//��������
	if (pInputMouse->GetRelease(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
	{
		//�N���b�N���Ă����ԂȂ�
		if (m_bClick)
		{
			//�N���b�N���Ă��Ȃ���Ԃɂ���
			m_bClick = false;
		}
	}

	//���lUI����������Ă�����
	if (m_pCounter != nullptr)
	{
		//���l�̐ݒ�
		m_pCounter->SetCounterNum(int(m_fNum));
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void COptionBar::Draw(void)
{
	
}

//=============================================================================
// ����
//=============================================================================
COptionBar *COptionBar::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &barSize, const D3DXVECTOR3 &circleSize,
		                       const float &fMin, const float &fMax, const float &fDefault)
{
	//�C���X�^���X�̐���
	COptionBar *pOptionBar = nullptr;
	if (pOptionBar == nullptr)
	{
		pOptionBar = new COptionBar();
		if (pOptionBar != nullptr)
		{
			pOptionBar->m_pos = pos;
			pOptionBar->m_barSize = barSize;
			pOptionBar->m_circleSize = circleSize;
			pOptionBar->m_fMin = fMin;
			pOptionBar->m_fMax = fMax;
			pOptionBar->m_fDefault = fDefault;
			pOptionBar->Init();
		}
	}
	return pOptionBar;
}

//=============================================================================
//���݂̒l�ݒ菈��
//=============================================================================
void COptionBar::SetNum(const float & fNum)
{
	m_fNum = fNum;

	//�ő�l�ƍŏ��l�̍��������߂�
	float fDiffer = m_fMax - m_fMin;
	//�f�t�H���g�̉~�̈ʒu���ǂ��ɂ��邩�����߂邽�߂Ƀo�[�ɑ΂��Ăǂ��ɂ���̂��䗦�����߂�
	float fDefaultRatio = (m_fNum - m_fMin) * 100.0f / fDiffer;
	//�~�̈ʒu�擾
	D3DXVECTOR3 circlePos = m_pCircle->GetPos();
	//�o�����䗦�ŏꏊ�����߂�
	circlePos.x = (m_pos.x - m_barSize.x / 2.0f) + (m_barSize.x * (fDefaultRatio / 100.0f));
	//�~�̈ʒu��ݒ�
	m_pCircle->SetPos(circlePos, m_pCircle->GetSize());
}
