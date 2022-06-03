//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : 羽鳥太一
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "scene.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CScene *CScene::m_pTop[(int)LAYER_TYPE::MAX_LAYER];
CScene *CScene::m_pBottom[(int)LAYER_TYPE::MAX_LAYER];
bool CScene::m_bFirst = false;

//=============================================================================
// デフォルトコンストラクタ
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
	// 最初なら
	if (m_pBottom[m_nPriority] == NULL &&
		m_pTop[m_nPriority] == NULL)
	{
		// けつも頭も自分
		m_pTop[m_nPriority] = this;
		m_pBottom[m_nPriority] = this;
		m_pPrev = NULL;
		m_pNext = NULL;
	}
	// 最初意外
	else
	{
		// 自分の前は今のけつ
		m_pPrev = m_pBottom[m_nPriority];
		// 自分の前の次は自分
		m_pBottom[m_nPriority]->m_pNext = this;
		// けつは自分
		m_pBottom[m_nPriority] = this;
		// 次はいない
		m_pNext = NULL;
	}
}

//=============================================================================
// デフォルトデストラクタ
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
// 全部削除
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
// 全部更新
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
// 全部描画
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