//===========================================
//�v���C���[����
//Author:��������
//===========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "object.h"
#include "communicationdata.h"

//�O���錾
class CGunPlayer;
class CSparkle;
class CWind;
class CXanimModel;
class CObject2D;
class CModelCollision;
class CShadow;

//================================================
//�}�N����`
//================================================
#define MAX_PLAYER_MODEL					(5)			//���f���̐�
#define PLAYER_LIFE							(100)									//�̗�
#define PLAYER_SIZE							(75.0f)									//�v���C���[�̃T�C�Y�����l

//================================================
//�N���X�̒�`
//================================================
//�N���X�̒�`
class CPlayer : public CObject
{
public:
	//�����o�֐�
	CPlayer(CObject::PRIORITY Priority = CObject::PRIORITY::PLAYER);				//�R���X�g���N�^
	~CPlayer();												//�f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot);

	D3DXVECTOR3 GetRot(void) { return m_rot; }												//�v���C���[�̌����擾����
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }									//�v���C���[�̌����ݒ菈��
	D3DXVECTOR3 GetOffsetPos(void) { return m_offsetPos; }									//�v���C���[�̃I�t�Z�b�g�ʒu�擾����
	void SetOffsetPos(const D3DXVECTOR3 &offsetPos) { m_offsetPos = offsetPos; }			//�v���C���[�̃I�t�Z�b�g�ʒu�ݒ菈��
	void SetMove(const D3DXVECTOR3 &move) { m_move = move; }								//�ړ��ʐݒ菈��
	D3DXVECTOR3 GetMove(void) { return m_move; }											//�ړ��ʎ擾����
	static void CPlayer::Collision(CObject *pSubjectObject, const float &fObjRadius);		//�����蔻�菈��
	bool CPlayer::CollisionOnly(CObject *&pSubjectObject, const float &fObjRadius);			//�Փ˔���݂̂̏���
	D3DXMATRIX *GetMtx(void) { return &m_mtxWorld; }										//���[���h�}�g���b�N�X�擾����
	void SetObjParent(const bool bObjeParent) { m_bObjParent = bObjeParent; }				//�I�u�W�F�N�g�Ƃ̐e�q�֌W�ݒ菈��
	bool GetObjParent(void) { return m_bObjParent; }										//�I�u�W�F�N�g�Ƃ̐e�q�֌W�擾����
	D3DXMATRIX *GetMtxParent(void) { return m_mtxWorldParent; }								//���[���h�}�g���b�N�X�擾����
	void SetMtxParent(D3DXMATRIX *mtx) { m_mtxWorldParent = mtx; }							//���[���h�}�g���b�N�X�ݒ菈��
	CGunPlayer *GetGunPlayer(void);															//�e�擾����
	CXanimModel *GetAnimModel(void) { return m_pAnimModel; }								//�A�j���[�V�������f���擾����
	CModelCollision *GetModelCollision(void) { return m_pCollModel; }						//�����蔻��p���f���擾����
	void SetAnimSpeed(const float &fAnimSpeed) { m_fAnimSpeed = fAnimSpeed; }				//�A�j���[�V�����̃X�s�[�h�ݒ菈��
	float GetAnimSpeed(void) { return m_fAnimSpeed; }										//�A�j���[�V�����̃X�s�[�h�擾����

private:
	//�����o�֐�
	void Move(void);								//�ړ�����
	void Rotate(void);								//��]����
	void Jump(void);								//�W�����v����
	void Shot(void);								//�ˌ�����
	void ADS(void);									//ADS����
	void Chest(void);								//���̏���
	void HitBullet(void);							//��e����
	void Respawn(void);								//���X�|�[������

	//�����o�ϐ�
	D3DXVECTOR3 m_pos;								//�ʒu
	D3DXVECTOR3 m_offsetPos;						//�v���C���[���q�ɂȂ����Ƃ��̃I�t�Z�b�g�ʒu
	D3DXVECTOR3 m_posOld;							//1�t���[���O�̈ʒu
	D3DXVECTOR3 m_move;								//�|���S���̈ړ���
	D3DXVECTOR3	m_size;								//�T�C�Y
	D3DXVECTOR3 m_rot;								//����
	CGunPlayer *m_pGunPlayer;							//�e���f���̃|�C���^
	CGunPlayer *m_pGunPlayerAds;						//ADS�p�̏e���f���̃|�C���^
	D3DXMATRIX m_mtxWorld;							//���[���h�}�g���b�N�X
	float m_fObjectiveRot;							//�ړI�̌���
	float m_fNumRot;								//������ς����
	bool m_bRotate;									//��]���Ă��邩�ǂ���
	D3DXMATRIX *m_mtxWorldParent;					//�e�̃��[���h�}�g���b�N�X
	bool m_bObjParent;								//���f���ȊO�Ƃ̐e�q�֌W�����邩�ǂ���
	bool m_bJump;									//�W�����v���Ă��邩�ǂ���
	CXanimModel *m_pAnimModel;						//���f��
	CXanimModel *m_pDeathModel;						//���񂾂Ƃ��̃��f��
	float m_fAnimSpeed;								//�A�j���[�V�����̃X�s�[�h
	float m_fMoveSpeed;								//�ړ����鑬��
	int m_nCounter;									//���̒e���o��܂ł̃J�E���^�[
	bool m_bAds;									//ADS���Ă��邩�ǂ���
	CObject2D *m_pCloss;							//�N���X�w�A
	int m_nLife;									//�̗�
	CModelCollision *m_pCollModel;					//�����蔻��̃{�b�N�X
	bool m_bShot;									//���������ǂ���
	CShadow *m_pShadow;								//�e�̃|�C���^
	int m_nRespawnCounter;							//���X�|�[������܂ł̃J�E���^�[
	bool m_bDeath;									//����ł��邩�ǂ���
	int m_nInvincibleCounter;						//���G���ԃJ�E���g�p
};

#endif // !_PLAYER_H_