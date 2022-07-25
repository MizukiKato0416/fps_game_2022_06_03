//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "counter.h"
#include "number.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CCounter::CCounter()
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nNumberMax = 0;	// ����
	m_nCounter = 0;	// �X�R�A
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CCounter::~CCounter()
{

}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CCounter::Init(void)
{
	for (int nNumber = 0; nNumber < m_nNumberMax; nNumber++)
	{
		m_pNumber.push_back(CNumber::Create(D3DXVECTOR3(m_Pos.x + m_Size.x * nNumber, m_Pos.y, 0.0f), m_Size, m_Type));
	}
	m_nCounter = 0;

	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CCounter::Uninit(void)
{
	for (int nNumber = 0; nNumber < m_nNumberMax; nNumber++)
	{
		m_pNumber[0]->Uninit();
		m_pNumber[0] = NULL;
		m_pNumber.erase(m_pNumber.begin());
	}
	Release();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CCounter::Update(void)
{
	SetCounter(m_nCounter);
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CCounter::Draw(void)
{
	for (int nNumber = 0; nNumber < m_nNumberMax; nNumber++)
	{
		m_pNumber[nNumber]->Draw();
	}
}

//=============================================================================
// �Q�b�_�[
//=============================================================================
CCounter *CCounter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumbderLength, string type)
{
	CCounter *pCount = NULL;
	if (pCount == NULL)
	{
		pCount = new CCounter;
	}

	pCount->m_Pos = pos;
	pCount->m_Size = size;
	pCount->m_Type = type;
	pCount->m_nNumberMax = nNumbderLength;

	if (pCount != NULL)
	{
		pCount->Init();
	}
	return pCount;
}

//=============================================================================
// �Z�b�^�[
//=============================================================================
void CCounter::SetCounter(int nCounter)
{
	int *aNumber = new int[m_nNumberMax];	// ��
	int nDivide = 10;	// �������
	int nBreak = 1;	// �����

	for (int nCnt = 0; nCnt < m_nNumberMax - 1; nCnt++)
	{
		nDivide *= 10;
		nBreak *= 10;
	}

	for (int nCnt = 0; nCnt < m_nNumberMax; nCnt++)
	{
		aNumber[nCnt] = m_nCounter % nDivide / nBreak;
		nDivide /= 10;
		nBreak /= 10;
	}

	for (int nNumber = 0; nNumber < m_nNumberMax; nNumber++)
	{
		m_pNumber[nNumber]->SetScore(aNumber[nNumber]);
	}
}

void CCounter::SetCol(D3DCOLORVALUE col)
{
	for (int nCntNumber = 0; nCntNumber < m_nNumberMax; nCntNumber++)
	{
		m_pNumber[nCntNumber]->SetCol(col);
	}
}