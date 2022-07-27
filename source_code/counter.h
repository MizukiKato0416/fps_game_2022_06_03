//=============================================================================
//
// ������ [number.h]
// Author : �H������
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CNumber;

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "object.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCounter : public CObject
{
public:
	CCounter(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// �f�t�H���g�R���X�g���N�^
	~CCounter();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void AddCounter(int nValue) { m_nCounter += nValue; }	// �X�R�A���Z { �X�R�A += ����}
	int GetCounter(void) { return m_nCounter; }				// �Q�b�^�[
	void SetCounter(int nCounter);	// �Z�b�^�[
	void SetCol(D3DXCOLOR col);	// �Z�b�^�[
	void SetCounterNum(int nCounter) { m_nCounter = nCounter; }	// �Z�b�^�[
	int GetCounterNum(void) { return m_nCounter; }	// �Q�b�_�[
	static CCounter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nNumbderLength, string type);	// �I�u�W�F�N�g�̐���
private:
	vector<CNumber*> m_pNumber;	// ��
	D3DXVECTOR3 m_Pos;	// �ʒu
	D3DXVECTOR3 m_Size;	// �T�C�Y
	string m_Type;	// �e�N�X�`��
	int m_nNumberMax;	// ����
	int m_nCounter;	// �X�R�A
};

#endif