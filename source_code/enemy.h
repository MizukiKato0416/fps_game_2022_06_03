//=============================================================================
//
// �G���� [enemy.h]
// Author : �H������
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "communicationdata.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CXanimModel;
class CModelSingle;
class CModelCollision;
class CGunModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemy : public CObject
{
public:
	CEnemy(CObject::PRIORITY Priority = CObject::PRIORITY::ENEMY);	// �f�t�H���g�R���X�g���N�^
	~CEnemy();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// �|���S���̏�����
	void Uninit(void);	// �|���S���̏I��
	void Update(void);	// �|���S���̍X�V
	void Draw(void);	// �|���S���̕`��
	static CEnemy *Create(void);	// ����
	static void Recv(CCommunicationData *data);	// ���V�[�u�X���b�h
	void SetCommuData(CCommunicationData::COMMUNICATION_DATA data) { m_commu_data.SetCmmuData(data); }	// �ʐM�f�[�^�ݒ菈��
	CCommunicationData::COMMUNICATION_DATA *GetCommuData(void) { return m_commu_data.GetCmmuData(); }	// �ʐM�f�[�^�擾����
	CXanimModel *GetSelfModel(void) { return m_model; }
	int GetPlayerNumber(void) { CCommunicationData::COMMUNICATION_DATA *data = m_commu_data.GetCmmuData(); return data->Player.nNumber; }
	int GetLife(void) { return m_nLife; }		//���C�t�擾����
	void SetLife(const int nLife) { m_nLife = nLife; }	//���C�t�ݒ菈��
	D3DXMATRIX GetMtx(void) { return m_mtx_wld; }	//���[���h�}�g���b�N�X�擾����
	CModelCollision *GetModelCollision(void) { return m_pCollModel; }	//�����蔻��p���f���擾����

private:
	void Attack(void);	// �U��
	void Move(void);	// �ړ�
	CCommunicationData m_commu_data;	// �ʐM�f�[�^
	CXanimModel *m_model;	// ���f��
	CGunModel *m_pGunModel;			// �e���f��
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_recvPos;	// �󂯎�����ʒu
	D3DXVECTOR3 m_posOld;	// �O�̈ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_recvRot;	// �󂯎��������
	D3DXVECTOR3 m_rotOld;	// �O�̌���
	D3DXVECTOR3 m_size;		// �T�C�Y
	D3DXMATRIX m_mtx_wld;	// ���[���h�}�g���b�N�X
	int m_nLife;			//���C�t
	CModelCollision *m_pCollModel;	//�����蔻��̃{�b�N�X
};

#endif