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
class CLetter;

//*****************************************************************************
// �\���̂̒�`
//*****************************************************************************
typedef struct
{
	int nPlayerNum;		//�v���C���[�ԍ�
	int nKill;			//�L����
	string aName;
} Ranking;

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
	//�w�肵�������L���O�̃L�����ƃv���C���[���\�����鏈��
	void SetRank(const int &nRank) { m_nRank = nRank; }
	//�w�肵�����ʂ̃����L���O�f�[�^�擾����
	Ranking GetRankData(const int &nRank) { return m_ranking[nRank]; }
	//�v���C���[�̔ԍ�����\������f�[�^���Q�Ƃ��邩���Ȃ����ݒ菈��
	void SetPlayerNum(const bool &bSetPlayerNum) { m_bSetPlayerNum = bSetPlayerNum; }
	//�v���C���[�̔ԍ�����\������f�[�^���Q�Ƃ��邩���Ȃ����擾����
	bool GetPlayerNum(void) { return m_bSetPlayerNum; }


private:
	//������������
	void CreateLetter(const int &nRank);

	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_scale;			//�X�P�[��
	int m_nRank;					//�\�����郉���L���O
	CCounter *m_pRankCounter;		//�����L���OUI�\���p
	CCounter *m_pKillCounter;		//�L����UI�\���p
	vector<Ranking> m_ranking;		//�����L���O�f�[�^
	bool m_bSetPlayerNum;			//�v���C���[�̔ԍ�����\������f�[�^���Q�Ƃ��邩���Ȃ���
	CObject2D *m_pRank;				//�����L���O�̐����̌�̉p��
	vector<CLetter*> m_name_font;	// ���O�t�H���g
	//int m_count_letter;				// �e�L�X�g�J�E���g
};

#endif