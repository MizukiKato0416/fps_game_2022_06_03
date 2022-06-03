//=============================================================================
//
// �J�������� [camera.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "mouseinput.h"
#include "keyinput.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	m_fDifferVR = 0.0f;
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCamera::Init(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR, D3DXVECTOR3 Rot)
{
	m_PosV = PosV;
	m_PosR = PosR;
	m_Rot = Rot;
	m_VecU = (D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_fDifferVR = 1000.0f;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void)
{
	
}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
	CInputKeyboard *pKeyboard;
	CInputMouse *pMouse;
	POINT Mouse;
	pMouse = CManager::GetMouse();
	pKeyboard = CManager::GetInputKeyboard();
	GetCursorPos(&Mouse);

	D3DXVECTOR2 mouseVelocity = D3DXVECTOR2(pMouse->GetMousePos().lX, pMouse->GetMousePos().lY);

	if (pKeyboard->GetPress(pKeyboard->SPACE) == true)
	{
		m_PosV.y += 10.0f;
	}
	else if (pKeyboard->GetPress(pKeyboard->LSHIFT) == true)
	{
		m_PosV.y -= 10.0f;
	}

	//================================================
	//�}�E�X�ɂ�鎋�_�ړ�����
	//================================================
	if (pMouse->GetPress(CInputMouse::RIGHT_BOTTON))
	{


		if (mouseVelocity.x != 0.0f)
		{
			m_Rot.y += mouseVelocity.x * 0.003f;
		}
		if (mouseVelocity.y != 0.0f)
		{
			m_Rot.x += mouseVelocity.y * -0.001;
		}

		//�ړ�����
		Move();
	}



	//�z�C�[���ɂ�钍���_����x�X�܂ł̋�����L�΂�����
	if (pMouse->GetMousePos().lZ >= 120)
	{
		m_fDifferVR -= 50.0f;
	}
	else if (pMouse->GetMousePos().lZ <= -120)
	{
		m_fDifferVR += 50.0f;
	}

	if (m_Rot.y >= D3DX_PI)
	{
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_Rot.y <= -D3DX_PI)
	{
		m_Rot.y += D3DX_PI * 2.0f;
	}
	if (m_Rot.x >= D3DX_PI / 8.0f * 7.0f)
	{
		m_Rot.x = D3DX_PI / 8.0f * 7.0f;
	}
	else if (m_Rot.x <= D3DX_PI / 8.0f)
	{
		m_Rot.x = D3DX_PI / 8.0f;
	}

	//���_�̏ꏊ�𒍎��_�����Ɉړ�
	m_PosV.x = m_PosR.x + m_fDifferVR * sinf(m_Rot.x) * sinf(m_Rot.y);
	m_PosV.z = m_PosR.z + m_fDifferVR * sinf(m_Rot.x) * cosf(m_Rot.y);
	m_PosV.y = m_PosR.y + m_fDifferVR * cosf(m_Rot.x);

	CalcScreenToWorld(&m_MouseWorldPos, Mouse.x, Mouse.y, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, &m_mtxView, &m_mtxProjection);
}

//=============================================================================
// �Z�b�g����
//=============================================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();		//�f�o�C�X���擾����
	//---------------------------
	//�}�g���b�N�X�̐ݒ�
	//---------------------------
	D3DXMatrixIdentity(&m_mtxProjection);	//�}�g���b�N�X������

	//�}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(	&m_mtxProjection,
								D3DXToRadian(90.0f),	//����p
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,		//�J�����̍ŏ��`�拗��
								30000.0f);	//�J�����̍ő�`�拗��

	//�v���W�F�N�V�����}�g���b�N�X�ݒ�
	pDevice->SetTransform(	D3DTS_PROJECTION,
							&m_mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(	&m_mtxView,
						&m_PosV,
						&m_PosR,
						&m_VecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(	D3DTS_VIEW,
							&m_mtxView);
}

D3DXVECTOR3 *CCamera::CalcScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return pout;
}

//=============================================================================
// ��������
//=============================================================================
CCamera *CCamera::Create(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR, D3DXVECTOR3 Rot)
{
	CCamera *pCamera = NULL;
	if (pCamera == NULL)
	{
		pCamera = new CCamera;
	}

	if (pCamera)
	{
		pCamera->Init(PosV, PosR, Rot);
	}
	return pCamera;
}


//================================================
//��������
//================================================
void CCamera::Move(void)
{
	//�L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//WASD����������
	if (pInputKeyboard->GetPress(pInputKeyboard->W) == true || pInputKeyboard->GetPress(pInputKeyboard->A) == true ||
		pInputKeyboard->GetPress(pInputKeyboard->S) == true || pInputKeyboard->GetPress(pInputKeyboard->D) == true)
	{
		float fObjectiveRot = 0.0f;

		if (pInputKeyboard->GetPress(pInputKeyboard->W) == true)
		{
			if (pInputKeyboard->GetPress(pInputKeyboard->A) == true)
			{
				//�ړI�̌�����ݒ�
				fObjectiveRot = m_Rot.y - D3DX_PI / 4.0f;
			}
			else if (pInputKeyboard->GetPress(pInputKeyboard->D) == true)
			{
				//�ړI�̌�����ݒ�
				fObjectiveRot = m_Rot.y + D3DX_PI / 4.0f;
			}
			else
			{
				//�ړI�̌�����ݒ�
				fObjectiveRot = m_Rot.y;
			}
		}
		else if (pInputKeyboard->GetPress(pInputKeyboard->S) == true)
		{
			if (pInputKeyboard->GetPress(pInputKeyboard->A) == true)
			{
				//�ړI�̌�����ݒ�
				fObjectiveRot = m_Rot.y - D3DX_PI / 4.0f * 3.0f;
			}
			else if (pInputKeyboard->GetPress(pInputKeyboard->D) == true)
			{
				//�ړI�̌�����ݒ�
				fObjectiveRot = m_Rot.y + D3DX_PI / 4.0f * 3.0f;
			}
			else
			{
				//�ړI�̌�����ݒ�
				fObjectiveRot = m_Rot.y + D3DX_PI;
			}
		}
		else if (pInputKeyboard->GetPress(pInputKeyboard->A) == true)
		{
			//�ړI�̌�����ݒ�
			fObjectiveRot = m_Rot.y - D3DX_PI / 2.0f;
		}
		else if (pInputKeyboard->GetPress(pInputKeyboard->D) == true)
		{
			//�ړI�̌�����ݒ�
			fObjectiveRot = m_Rot.y + D3DX_PI / 2.0f;
		}

		//�ړ��ʉ��Z
		m_PosR.x += sinf(fObjectiveRot + D3DX_PI) * 20.0f;
		m_PosR.z += cosf(fObjectiveRot + D3DX_PI) * 20.0f;
	}
}
