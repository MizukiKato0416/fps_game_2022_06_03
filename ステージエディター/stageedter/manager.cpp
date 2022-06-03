//=============================================================================
//
// �}�l�[�W���[�Ǘ����� [manager.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "mouseinput.h"
#include "keyinput.h"
#include "camera.h"
#include "model.h"
#include "edter.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CManager::MODE CManager::m_Mode;
CManager::MODE CManager::m_ModeOld;
CRenderer *CManager::m_pRenderer = NULL;
CTexture *CManager::m_pTexture = NULL;
CInputMouse *CManager::m_pMouse = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight[MAX_LIGHT];
CEdter *CManager::m_pEdter = NULL;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CManager::CManager()
{
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		m_pLight[nCnt] = NULL;
	}
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	m_Mode = MODE::MODE_EDTER;

	if (m_pTexture == NULL &&
		m_pRenderer == NULL &&
		m_pInputKeyboard == NULL &&
		m_pMouse == NULL)
	{
		m_pTexture = new CTexture;	// �C���X�^���X����
		m_pRenderer = new CRenderer;// �C���X�^���X����
		m_pInputKeyboard = new CInputKeyboard;	// �C���X�^���X����
		m_pMouse = new CInputMouse;

		// ������
		if (m_pTexture != NULL &&
			m_pRenderer != NULL &&
			m_pInputKeyboard != NULL &&
			m_pMouse != NULL)
		{
			m_pRenderer->Init(hWnd, true);
			CModel::Load();
			m_pTexture->Init();
			m_pMouse->Init(hInstance, hWnd);
			m_pInputKeyboard->Init(hInstance, hWnd);
			m_pInputKeyboard->BindKey(CInputKeyboard::W, DIK_W);
			m_pInputKeyboard->BindKey(CInputKeyboard::A, DIK_A);
			m_pInputKeyboard->BindKey(CInputKeyboard::S, DIK_S);
			m_pInputKeyboard->BindKey(CInputKeyboard::D, DIK_D);
			m_pInputKeyboard->BindKey(CInputKeyboard::LEFT, DIK_RIGHT);
			m_pInputKeyboard->BindKey(CInputKeyboard::RIGHT, DIK_LEFT);
			m_pInputKeyboard->BindKey(CInputKeyboard::UP, DIK_UP);
			m_pInputKeyboard->BindKey(CInputKeyboard::DOWN, DIK_DOWN);
			m_pInputKeyboard->BindKey(CInputKeyboard::T, DIK_T);
			m_pInputKeyboard->BindKey(CInputKeyboard::B, DIK_B);
			m_pInputKeyboard->BindKey(CInputKeyboard::Z, DIK_Z);
			m_pInputKeyboard->BindKey(CInputKeyboard::C, DIK_C);
			m_pInputKeyboard->BindKey(CInputKeyboard::Y, DIK_Y);
			m_pInputKeyboard->BindKey(CInputKeyboard::N, DIK_N);
			m_pInputKeyboard->BindKey(CInputKeyboard::U, DIK_U);
			m_pInputKeyboard->BindKey(CInputKeyboard::M, DIK_M);
			m_pInputKeyboard->BindKey(CInputKeyboard::Q, DIK_Q);
			m_pInputKeyboard->BindKey(CInputKeyboard::E, DIK_E);
			m_pInputKeyboard->BindKey(CInputKeyboard::SPACE, DIK_SPACE);
			m_pInputKeyboard->BindKey(CInputKeyboard::LSHIFT, DIK_LSHIFT);
			m_pInputKeyboard->BindKey(CInputKeyboard::F9, DIK_F9);
		}
	}

	m_pCamera = CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, -50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pLight[0] = CLight::Create(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(-100.0f, 500.0f, 100.0f), D3DXVECTOR3(0.2f, -0.8f, 0.4f));
	m_pLight[1] = CLight::Create(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 500.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f));
	m_pLight[2] = CLight::Create(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(100.0f, -500.0f, -100.0f), D3DXVECTOR3(-0.2f, 0.8f, -0.4f));

	SetMode(m_Mode);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �I�u�W�F�N�g�̑S�j��
	CScene::ReleaseAll();
	if (m_pTexture)
	{
		m_pTexture->Uniinit();
		delete m_pTexture;
		m_pTexture = NULL;
	}
	if (m_pMouse)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = NULL;
	}
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	if (m_pCamera)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (m_pLight[nCnt])
		{
			m_pLight[nCnt]->Uninit();
			delete m_pLight[nCnt];
			m_pLight[nCnt] = NULL;
		}
	}
	if (m_pRenderer)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	SetCursorPos(SCREEN_CENTER_X, SCREEN_CENTER_Y);
	ShowCursor(FALSE);

	if (m_pMouse)
	{
		m_pMouse->Update();
	}
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Update();
	}
	if (m_pCamera)
	{
		m_pCamera->Update();
	}
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (m_pLight[nCnt])
		{
			m_pLight[nCnt]->Update();
		}
	}
	if (m_pRenderer)
	{
		m_pRenderer->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer)
	{
		m_pRenderer->Draw();
	}
	m_pCamera->SetCamera();
}

//=============================================================================
// ���[�h�`�����W
//=============================================================================
void CManager::SetMode(MODE mode)
{
	m_ModeOld = m_Mode;
	CScene::ReleaseAll();
	switch (m_Mode)
	{
	case MODE::MODE_EDTER:
		if (m_pEdter)
		{
			m_pEdter = NULL;
		}
		break;
	default:
		break;
	}

	m_Mode = mode;

	switch (mode)
	{
	case MODE::MODE_EDTER:
		if (m_pEdter == NULL)
		{
			m_pEdter = new CEdter;
			m_pEdter->Init();
		}
		break;
	default:
		break;
	}
}