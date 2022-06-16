//=============================================================================
//
// �e�N�X�`������ [texture.h]
// Author : �H������
//
//=============================================================================
#ifndef _GUN_MODEL_H_
#define _GUN_MODEL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGunModel : CObject
{
public:
	CGunModel(CObject::PRIORITY Priority = CObject::PRIORITY::MODEL);	// �f�t�H���g�R���X�g���N�^
	~CGunModel();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// �|���S���̏�����
	void Uninit(void);	// �|���S���̏I��
	void Update(void);	// �|���S���̍X�V
	void Draw(void);	// �|���S���̕`��
	static CGunModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 muzzle_pos, string type);	// ����
	void SetPos(const D3DXVECTOR3 &pos) { m_pos = pos; CObject::SetPos(m_pos); }		//�ʒu�ݒ菈��
	D3DXVECTOR3 GetPos(void) { return m_pos; }											//�ʒu�擾����
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }								//��]�ݒ菈��
	D3DXVECTOR3 GetRot(void) { return m_rot; }											//��]�擾����
	D3DXMATRIX GetMuzzleMtx(void) { return m_mtx_muzzle_world; }
	CModel *GetModel(void) { return m_model; }											//���f���擾����
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_muzzle_pos;
	D3DXMATRIX m_mtx_world;				//���[���h�}�g���b�N�X
	D3DXMATRIX m_mtx_muzzle_world;				//���[���h�}�g���b�N�X
	string m_type;
	CModel *m_model;					//���f���̃|�C���^
};

#endif