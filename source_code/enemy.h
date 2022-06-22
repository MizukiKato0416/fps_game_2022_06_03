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
	static void Recv(void);	// ���V�[�u�X���b�h
	void SetCommuData(CCommunicationData::COMMUNICATION_DATA data, int number) { m_commu_data[number].SetCmmuData(data); }	// �ʐM�f�[�^�ݒ菈��
	CCommunicationData::COMMUNICATION_DATA *GetCommuData(int number) { return m_commu_data[number].GetCmmuData(); }	// �ʐM�f�[�^�擾����
	CXanimModel *GetSelfModel(void) { return m_model; }

private:
	void Attack(void);	// �U��
	void Move(void);	// �ړ�
	static CCommunicationData m_commu_data[MAX_PLAYER];	// �ʐM�f�[�^
	static int m_create_count;	// ����������
	static bool m_create_thread;	// �X���b�h�𕪂�����
	CXanimModel *m_model;	// ���f��
	CModelSingle *m_pGunModel;			// �e���f��
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXMATRIX m_mtx_wld;	// ���[���h�}�g���b�N�X
	int m_my_index;	// �����̃v���C���[�ԍ�
};

#endif