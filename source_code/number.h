//=============================================================================
//
// ������ [number.h]
// Author : �H������
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "object2D.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CNumber : public CObject2D
{
public:
	CNumber(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// �f�t�H���g�R���X�g���N�^
	~CNumber();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// �|���S���̏�����(�ʒu�A�T�C�Y�A�e�N�X�`��)
	void Uninit(void);	// �|���S���̏I��
	void Update(void);	// �|���S���̍X�V
	void Draw(void);	// �|���S���̕`��
	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, string type);	// �I�u�W�F�N�g�̐���(�ʒu, �T�C�Y�A�e�N�X�`��)
	void SetScore(int nScore) { CObject2D::SetTex(nScore); };	// �X�R�A
	void SetCol(D3DCOLORVALUE col) { CObject2D::SetCol(col); }	// �Z�b�^�[
private:
	D3DXVECTOR3 m_Pos;	// �ʒu
	D3DXVECTOR3 m_Size;	// �T�C�Y
	string m_Type;	// �e�N�X�`��
};

#endif