//===========================================
//�Q�[������
//Author:��������
//===========================================
#ifndef _GAME01_H_
#define _GAME01_H_
#include "object.h"
#include "main.h"
#include "option.h"

//�O���錾
class CPlayer;
class CMeshField;
class CEnemy;
class CObject2D;
class CScoreUi;

//================================================
//�}�N����`
//================================================

//========================================================
//�\���̂̒�`
//========================================================

//================================================
//�N���X�̒�`
//================================================
class CGame01 : public CObject
{
public:
	//�v���C���[���X�|�[���ʒu�̎��
	enum class PlayerRespawnPos
	{
		NONE = 0,
		POS_00,
		POS_01,
		POS_02,
		POS_03,
		POS_04,
		POS_05,
		POS_06,
		POS_07,
		MAX
	};


	CGame01(CObject::PRIORITY Priority = CObject::PRIORITY::NONE);		//�R���X�g���N�^
	~CGame01();															//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool MapLimit(CObject* pObj);		//�}�b�v�̈ړ�����
	bool GetAllConnect(void) { return m_bAllConnect; }
	void FirstContact(void);
	//�G�̃��f���擾����
	vector<CEnemy*> GetEnemy(void);
	//�v���C���[���X�|�[���֐�
	void RespawnPlayer(void);			
	//�ݒ��ʎ擾����
	COption *GetOption(void) { return m_pOption; }

private:
	void LoadModelTxt(const string &Pas);			//���f�����[�h����
	void ScoreUi(void);								//�X�R�AUI�̏���
	void LastSpurt(void);							//���X�g�X�p�[�g����
	void Option(void);								//�ݒ��ʏ���
	void GameOver(void);							//�Q�[���I������
	void Start(void);								//�X�^�[�g�܂ł̏���
		
	//�����o�ϐ�
	CPlayer *m_pPlayer;					//�v���C���[�̃|�C���^
	CObject2D *m_now_loding;
	CMeshField *m_pMeshField;			//���b�V���t�B�[���h�̃|�C���^
	vector<CEnemy*> m_pEnemy;					//�G�̃��f���̃|�C���^
	int m_count_pattern = 0;
	int m_pattern_tex = 0;
	bool m_bAllConnect;
	PlayerRespawnPos m_respawnPos;		//�v���C���[�����X�|�[��������ʒu
	CObject2D *m_pMapBg;				//�}�b�v�w�i
	CScoreUi *m_pScorUiTop;				//��̃X�R�AUI
	CScoreUi *m_pScorUiUnder;			//���̃X�R�AUI
	COption *m_pOption;					//�ݒ���
	bool m_bLastSpurt;					//���X�g�X�p�[�g���邩�ǂ���
	bool m_bGameOver;					//�Q�[�����I���������ǂ���
	int m_nResultCounter;				//���U���g�ɍs���܂ł̃J�E���^�[
	bool m_bStart;						//�X�^�[�g���邩�ǂ���
	int m_nStartCounter;				//�X�^�[�g����܂ł̃J�E���^�[
	int m_nCountDownOld;				//1�t���[���O�̃X�^�[�g�܂ł̃J�E���g�_�E��
	CObject2D *m_pCountDownUi;			//�J�E���g�_�E��UI�̃|�C���^
};	

#endif // !_GAME01_H_