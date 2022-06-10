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

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemy : public CObject
{
public:
	CEnemy(CObject::PRIORITY Priority = CObject::PRIORITY::PLAYER);	// �f�t�H���g�R���X�g���N�^
	~CEnemy();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// �|���S���̏�����
	void Uninit(void);	// �|���S���̏I��
	void Update(void);	// �|���S���̍X�V
	void Draw(void);	// �|���S���̕`��
	CCommunicationData::COMMUNICATION_DATA *GetCommuData(void) { return m_commu_data.GetCmmuData(); }	// �Q�b�_�[
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// ����
	static void Recv(CCommunicationData::COMMUNICATION_DATA *my_data);	// ���V�[�u�X���b�h
private:
	void Attack(void);	// �U��
	void Move(void);	// �ړ�
	CXanimModel *m_model;	// ���f��
	CCommunicationData m_commu_data;	// �ʐM�f�[�^
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	D3DXMATRIX m_mtx_wld;	// ���[���h�}�g���b�N�X
	int m_my_index;	// �����̃v���C���[�ԍ�
};

#endif