//===========================================
//�v���C���[����
//Author:��������
//===========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "object.h"
#include "communicationdata.h"

//�O���錾
class CGunModel;
class CSparkle;
class CWind;
class CXanimModel;

//================================================
//�}�N����`
//================================================
#define MAX_PLAYER_MODEL					(5)			//���f���̐�

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
	static void CPlayer::Collision(CObject *&pSubjectObject, const float &fObjRadius);		//�����蔻�菈��
	bool CPlayer::CollisionOnly(CObject *&pSubjectObject, const float &fObjRadius);			//�Փ˔���݂̂̏���
	D3DXMATRIX *GetMtx(void) { return &m_mtxWorld; }										//���[���h�}�g���b�N�X�擾����
	void SetObjParent(const bool bObjeParent) { m_bObjParent = bObjeParent; }				//�I�u�W�F�N�g�Ƃ̐e�q�֌W�ݒ菈��
	bool GetObjParent(void) { return m_bObjParent; }										//�I�u�W�F�N�g�Ƃ̐e�q�֌W�擾����
	D3DXMATRIX *GetMtxParent(void) { return m_mtxWorldParent; }								//���[���h�}�g���b�N�X�擾����
	void SetMtxParent(D3DXMATRIX *mtx) { m_mtxWorldParent = mtx; }							//���[���h�}�g���b�N�X�ݒ菈��
	CGunModel *GetGunModel(void) { return m_pGunModel; }									//�e�擾����

private:
	//�����o�֐�
	void Move(void);								//�ړ�����
	void Rotate(void);								//��]����
	void Jump(void);								//�W�����v����
	void Shoot(void);								//�ˌ�����
	void Chest(void);								//���̏���

	//�����o�ϐ�
	CCommunicationData m_commu_data;	// �ʐM�f�[�^
	D3DXVECTOR3 m_pos;								//�ʒu
	D3DXVECTOR3 m_offsetPos;						//�v���C���[���q�ɂȂ����Ƃ��̃I�t�Z�b�g�ʒu
	D3DXVECTOR3 m_posOld;							//1�t���[���O�̈ʒu
	D3DXVECTOR3 m_move;								//�|���S���̈ړ���
	D3DXVECTOR3	m_size;								//�T�C�Y
	D3DXVECTOR3 m_rot;								//����
	CGunModel *m_pGunModel;						//�e���f���̃|�C���^
	D3DXMATRIX m_mtxWorld;							//���[���h�}�g���b�N�X
	float m_fObjectiveRot;							//�ړI�̌���
	float m_fNumRot;								//������ς����
	bool m_bRotate;									//��]���Ă��邩�ǂ���
	D3DXMATRIX *m_mtxWorldParent;					//�e�̃��[���h�}�g���b�N�X
	bool m_bObjParent;								//���f���ȊO�Ƃ̐e�q�֌W�����邩�ǂ���
	bool m_bJump;									//�W�����v���Ă��邩�ǂ���
	CXanimModel *m_pAnimModel;						//���f��
	float m_fAnimSpeed;								//�A�j���[�V�����̃X�s�[�h
	float m_fMoveSpeed;								//�ړ����鑬��
	int m_nCounter;									//���̒e���o��܂ł̃J�E���^�[
};

#endif // !_PLAYER_H_