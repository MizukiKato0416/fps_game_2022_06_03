//=============================================================================
//
// �L������UI���� [kill_rate_ui.h]
// Author : ��������
//
//=============================================================================
#ifndef _KILL_RATE_UI_H_
#define _KILL_RATE_UI_H_

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
// �}�N����`
//*****************************************************************************
#define KILL_RATE_UI_FRAME_DEFAULT_SIZE_X			(516.0f)		//�t���[���̃f�t�H���g�T�C�Y
#define KILL_RATE_UI_FRAME_DEFAULT_SIZE_Y			(72.0f)			//�t���[���̃f�t�H���g�T�C�Y

//*****************************************************************************
// �\���̂̒�`
//*****************************************************************************
typedef struct
{
	int nPlayerNum;		//�v���C���[�ԍ�
	int nKill;			//�L����
	int nDeath;			//�f�X��
	string aName;
} KillRateRanking;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CKillRateUi : public CObject
{
public:
	CKillRateUi(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// �f�t�H���g�R���X�g���N�^
	~CKillRateUi();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// �|���S���̏�����
	void Uninit(void);	// �|���S���̏I��
	void Update(void);	// �|���S���̍X�V
	void Draw(void);	// �|���S���̕`��
	static CKillRateUi *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &scale);	// ����
	//�w�肵�������L���O�̃L�����ƃv���C���[���\�����鏈��
	void SetRank(const int &nRank) { m_nRank = nRank; }
	//�w�肵�����ʂ̃����L���O�f�[�^�擾����
	KillRateRanking GetRankData(const int &nRank) { return m_ranking[nRank]; }
	//�v���C���[�̔ԍ�����\������f�[�^���Q�Ƃ��邩���Ȃ����ݒ菈��
	void SetPlayerNum(const bool &bSetPlayerNum) { m_bSetPlayerNum = bSetPlayerNum; }
	//�v���C���[�̔ԍ�����\������f�[�^���Q�Ƃ��邩���Ȃ����擾����
	bool GetPlayerNum(void) { return m_bSetPlayerNum; }


private:
	//������������
	void CreateLetter(const int &nRank);

	D3DXVECTOR3 m_pos;						//�ʒu
	D3DXVECTOR3 m_scale;					//�X�P�[��
	int m_nRank;							//�\�����郉���L���O
	CCounter *m_pKillCounter;				//�L����UI�\���p
	CCounter *m_pDeathCounter;				//�f�X��UI�\���p
	vector<KillRateRanking> m_ranking;		//�����L���O�f�[�^
	bool m_bSetPlayerNum;					//�v���C���[�̔ԍ�����\������f�[�^���Q�Ƃ��邩���Ȃ���
	vector<CLetter*> m_name_font;			// ���O�t�H���g
};

#endif