//=============================================================================
//
// �u���b�N���� [block.h]
// Author : �H������
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"
#include "model.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBlock : public CScene
{
public:
	CBlock(LAYER_TYPE Layer = LAYER_TYPE::LAYER_00);	// �f�t�H���g�R���X�g���N�^
	~CBlock();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int type);
	void Uninit(void);	// �|���S���̏I��
	void Update(void);	// �|���S���̍X�V
	void Draw(void);	// �|���S���̕`��
	void SetUse(bool bUse) { m_bUse = bUse; }
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }	// �ʒu�̃Z�b�^�[
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }	// �����̃Z�b�^�[
	void SetMatrix(D3DXMATRIX mtx) { m_pModel->SetMatrix(mtx); }	// ���[���h�}�g���b�N�X�̃Q�b�_�[
	void SetScale(D3DXVECTOR3 scale) { m_pModel->SetScale(scale); }	// ���[���h�}�g���b�N�X�̃Q�b�_�[
	bool GetUse(void) { return m_bUse; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }	// �ʒu�̃Z�b�^�[
	D3DXVECTOR3 GetRot(void) { return m_Rot; }	// �����̃Z�b�^�[
	D3DXMATRIX GetMatrix(void) { return m_pModel->GetMatrix(); }	// ���[���h�}�g���b�N�X�̃Q�b�_�[
	D3DXVECTOR3 GetScale(void) { return m_pModel->GetScale(); }
	D3DXVECTOR3 GetVMin(void) { return m_pModel->GetVMin(); }
	D3DXVECTOR3 GetVMax(void) { return m_pModel->GetVMax(); }
	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int type);
private:
	D3DXMATRIX m_MtxWold;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_Pos;		//�ʒu
	D3DXVECTOR3 m_Rot;		//����
	D3DXVECTOR3 m_Scale;		//����
	CModel *m_pModel;	// ���f��
	bool m_bUse;
};

#endif