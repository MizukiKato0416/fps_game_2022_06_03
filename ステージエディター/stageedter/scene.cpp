//=============================================================================
//
// �I�u�W�F�N�g���� [scene.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "scene.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CScene *CScene::m_pTop[(int)LAYER_TYPE::MAX_LAYER];
CScene *CScene::m_pBottom[(int)LAYER_TYPE::MAX_LAYER];
bool CScene::m_bFirst = false;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CScene::CScene(LAYER_TYPE Layer)
{
	if (m_bFirst == false)
	{
		for (int nCnt = 0; nCnt < (int)LAYER_TYPE::MAX_LAYER; nCnt++)
		{
			m_pTop[nCnt] = NULL;
			m_pBottom[nCnt] = NULL;
		}
		m_bFirst = true;
	}
	m_bDeth = false;
	m_nPriority = (int)Layer;
	// �ŏ��Ȃ�
	if (m_pBottom[m_nPriority] == NULL &&
		m_pTop[m_nPriority] == NULL)
	{
		// ������������
		m_pTop[m_nPriority] = this;
		m_pBottom[m_nPriority] = this;
		m_pPrev = NULL;
		m_pNext = NULL;
	}
	// �ŏ��ӊO
	else
	{
		// �����̑O�͍��̂���
		m_pPrev = m_pBottom[m_nPriority];
		// �����̑O�̎��͎���
		m_pBottom[m_nPriority]->m_pNext = this;
		// ���͎���
		m_pBottom[m_nPriority] = this;
		// ���͂��Ȃ�
		m_pNext = NULL;
	}
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CScene::~CScene()
{
	CScene *pScene;
	pScene = this;

	if (pScene == m_pTop[m_nPriority] &&
		pScene == m_pBottom[m_nPriority])
	{
		m_pTop[m_nPriority] = NULL;
		m_pBottom[m_nPriority] = NULL;
		pScene->m_pNext = NULL;
		pScene->m_pPrev = NULL;
	}
	else if (pScene == m_pTop[m_nPriority])
	{
		pScene->m_pNext->m_pPrev = NULL;
		m_pTop[m_nPriority] = pScene->m_pNext;
	}
	else if (pScene == m_pBottom[m_nPriority])
	{
		pScene->m_pPrev->m_pNext = NULL;
		m_pBottom[m_nPriority] = pScene->m_pPrev;
	}
	else
	{
		pScene->m_pPrev->m_pNext = pScene->m_pNext;
		pScene->m_pNext->m_pPrev = pScene->m_pPrev;
	}
	pScene = NULL;
}

//=============================================================================
// �S���폜
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCnt = 0; nCnt < (int)LAYER_TYPE::MAX_LAYER; nCnt++)
	{
		CScene *pScene;
		pScene = m_pTop[nCnt];
		if (pScene)
		{
			while (pScene)
			{
				CScene *pSaveScene;
				pSaveScene = pScene->m_pNext;
				delete pScene;
				pScene = NULL;
				pScene = pSaveScene;
			}
		}
	}
}

//=============================================================================
// �S���X�V
//=============================================================================
void CScene::UpdateAll(void)
{
	for (int nCnt = 0; nCnt < (int)LAYER_TYPE::MAX_LAYER; nCnt++)
	{
		CScene *pScene;
		pScene = m_pTop[nCnt];
		while (pScene)
		{
			pScene->Update();
			pScene = pScene->m_pNext;
		}
	}
	for (int nCnt = 0; nCnt < (int)LAYER_TYPE::MAX_LAYER; nCnt++)
	{
		CScene *pScene;
		pScene = m_pTop[nCnt];
		while (pScene)
		{
			CScene *pSaveScene = pScene->m_pNext;
			if (pScene->m_bDeth == true)
			{
				delete pScene;
				pScene = NULL;
			}
			pScene = pSaveScene;
		}
	}
}

//=============================================================================
// �S���`��
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCnt = 0; nCnt < (int)LAYER_TYPE::MAX_LAYER; nCnt++)
	{
		CScene *pScene;
		pScene = m_pTop[nCnt];
		while (pScene)
		{
			pScene->Draw();
			pScene = pScene->m_pNext;
		}
	}
}