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
	CCommunicationData::COMMUNICATION_DATA *GetCommuData(int index) { return m_commu_data[index].GetCmmuData(); }	// �Q�b�_�[
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// ����
	static void Recv(void);	// ���V�[�u�X���b�h
private:
	void Attack(void);	// �U��
	void Move(void);	// �ړ�
	static CCommunicationData m_commu_data[MAX_PLAYER];	// �ʐM�f�[�^
	CXanimModel *m_model[MAX_PLAYER];	// ���f��
	D3DXVECTOR3 m_pos[MAX_PLAYER];		// �ʒu
	D3DXVECTOR3 m_rot[MAX_PLAYER];		// ����
	D3DXMATRIX m_mtx_wld[MAX_PLAYER];	// ���[���h�}�g���b�N�X
	int m_my_index[MAX_PLAYER];	// �����̃v���C���[�ԍ�
};

#endif