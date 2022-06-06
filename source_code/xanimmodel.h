//=============================================================================
//
// X�t�@�C�����f������ [sceneX.h]
// Author : �H������&�ߊԏr��
//
//=============================================================================
#ifndef _XANIMMODEL_H_
#define _XANIMMODEL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "hierarchydata.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CXanimModel
{
public:
	CXanimModel();	// �f�t�H���g�R���X�g���N�^
	~CXanimModel();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void UpdateFrame(LPD3DXFRAME base, LPD3DXMATRIX parent_matrix);	// �t���[���̃}�g���b�N�X�̍X�V
	void Draw(void);	// �`��
	void DrawMatrix(LPD3DXMATRIX matrix);	// �t���[���̕`��
	void DrawFrame(LPD3DXFRAME frame);	// �t���[���̕`��(�ċA����)
	void DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container);	// ���b�V���R���e�i�̕`��(�ċA����)
	HRESULT AllocateAllBoneMatrix(LPD3DXFRAME frame);	// �S�{�[���̕`��p�}�g���b�N�X�̕ۑ�(�ċA����)
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER vontainer);	// �S�{�[���̕`��p�}�g���b�N�X�̕ۑ�(�ċA����)
	static CXanimModel *Create(string type);	// ����
	void Load(void);	// ���f���̃��[�h
	void ChangeAnimation(int anim_num, float speed);	// �A�j���[�V�����ύX
	void PlayAnimation(float speed);	// �A�j���[�V�����Đ�
	void CheckContainer(LPD3DXFRAME frame, D3DXMATRIX *check_mtx, string name);	// �R���e�i�̖��O�̃`�F�b�N
	void ResetAnimasionPos(void) { m_anim_controller->SetTrackPosition(0, 0); }	// �A�j���[�V�����ʒu�̃��Z�b�g
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// �Z�b�^�[
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }	// �Z�b�^�[
	D3DXMATRIX GetMatrix(string name);	// �Q�b�_�[
	LPD3DXFRAME GetRootFrame(void) { return m_root_frame; }	// �Q�b�_�[
	D3DXVECTOR3 GetPos(void) { return m_pos; }	// �Q�b�^�[
	D3DXVECTOR3 GetRot(void) { return m_rot; }	// �Q�b�^�[
private:
	HierarchyData m_hierarchy_data;	// �K�w�f�[�^
	LPD3DXFRAME m_root_frame;	// ���[�g�t���[��
	LPD3DXANIMATIONCONTROLLER m_anim_controller;	// �A�j���[�V�����Ǘ�
	vector<LPD3DXANIMATIONSET> m_anim_set;	// �A�j���[�V�����擾
	string m_load_pas; // ���f���̃p�X
	D3DXMATRIX m_mtx_wold;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// ��]
	int m_now_anim;	// ���݂̃A�j���[�V����
};

#endif
