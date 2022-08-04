//=============================================================================
//
// �X�R�A��UI���� [score_ui.h]
// Author : ��������
//
//=============================================================================
#ifndef _SCORE_UI_H_
#define _SCORE_UI_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject2D;
class CCounter;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScoreUi : public CObject
{
public:
	CScoreUi(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// �f�t�H���g�R���X�g���N�^
	~CScoreUi();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// �|���S���̏�����
	void Uninit(void);	// �|���S���̏I��
	void Update(void);	// �|���S���̍X�V
	void Draw(void);	// �|���S���̕`��
	static CScoreUi *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &scale);	// ����

private:
	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_scale;			//�X�P�[��
	int m_nRank;					//�\�����郉���L���O
	CCounter *m_pRankCounter;		//�����L���OUI�\���p
	CCounter *m_pKillCounter;		//�L����UI�\���p
};

#endif