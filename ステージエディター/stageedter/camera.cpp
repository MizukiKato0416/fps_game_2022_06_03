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

	m_Rot.x += pMouse->GetMousePos().lX * 0.01f;
	m_Rot.y += pMouse->GetMousePos().lY * 0.01f;

	if(pKeyboard->GetPress(pKeyboard->W) == true)
	{
		m_PosV.x = m_PosV.x + sinf(m_Rot.x) * 10.0f;
		m_PosV.z = m_PosV.z + cosf(m_Rot.x) * 10.0f;
	}
	else if (pKeyboard->GetPress(pKeyboard->S) == true)
	{
		m_PosV.x = m_PosV.x - sinf(m_Rot.x) * 10.0f;
		m_PosV.z = m_PosV.z - cosf(m_Rot.x) * 10.0f;
	}
	if (pKeyboard->GetPress(pKeyboard->A) == true)
	{
		m_PosV.x = m_PosV.x - cosf(m_Rot.x) * 10.0f;
		m_PosV.z = m_PosV.z + sinf(m_Rot.x) * 10.0f;
	}
	else if (pKeyboard->GetPress(pKeyboard->D) == true)
	{
		m_PosV.x =m_PosV.x + cosf(m_Rot.x) * 10.0f;
		m_PosV.z =m_PosV.z - sinf(m_Rot.x) * 10.0f;
	}
	if (pKeyboard->GetPress(pKeyboard->SPACE) == true)
	{
		m_PosV.y += 10.0f;
	}
	else if (pKeyboard->GetPress(pKeyboard->LSHIFT) == true)
	{
		m_PosV.y -= 10.0f;
	}

	if (m_Rot.y >= D3DX_PI - 0.1f)
	{
		m_Rot.y = D3DX_PI - 0.1f;
	}
	else if (m_Rot.y <= -0.0f + 0.1f)
	{
		m_Rot.y = -0.0f + 0.1f;
	}

	m_PosR.x = m_PosV.x + (sinf(m_Rot.y) * sinf(m_Rot.x)) * 50.0f;
	m_PosR.y = m_PosV.y + cosf(m_Rot.y)				  * 50.0f;
	m_PosR.z = m_PosV.z + (sinf(m_Rot.y) * cosf(m_Rot.x)) * 50.0f;

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