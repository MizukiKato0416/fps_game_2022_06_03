//=============================================================================
//
// X�t�@�C�����f������ [sceneX.h]
// Author : �H������
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
	void ChangeAnimation(string anim_name, float speed);	// �A�j���[�V�����ύX
	void ChangeSpeed(float speed) { m_anim_controller->SetTrackSpeed(0, speed); }	// �A�j���[�V�����X�s�[�h�ύX
	void CheckContainer(LPD3DXFRAME frame, D3DXMATRIX *check_mtx, string name);	// �R���e�i�̖��O�̃`�F�b�N
	void CheckContainer(LPD3DXFRAME frame);	// �R���e�i�ŏ��ő咸�_�̃`�F�b�N
	void ResetAnimasionPos(void) { m_anim_controller->SetTrackPosition(0, 0); }	// �A�j���[�V�����ʒu�̃��Z�b�g
	void SaveMeshContainer(LPD3DXFRAME frame);	// ���b�V���̐ݒ�
	vector<MeshContainer> GetMesh(void) { return m_mesh; }	// ���b�V���̓���
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// �Z�b�^�[
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }	// �Z�b�^�[
	void SetSize(D3DXVECTOR3 size) { m_size = size; }	// �Z�b�^�[
	D3DXMATRIX *GetMatrix(string name);	// �Q�b�_�[
	void SetMatrix(string name, D3DXMATRIX *mtx);	// �Z�b�^�[
	LPD3DXFRAME GetRootFrame(void) { return m_root_frame; }	// �Q�b�_�[
	D3DXVECTOR3 GetPos(void) { return m_pos; }	// �Q�b�^�[
	D3DXVECTOR3 GetRot(void) { return m_rot; }	// �Q�b�^�[
	D3DXVECTOR3 GetSize(void) { return m_size; }	// �Q�b�^�[
	string GetAnimation(void) { return m_anim_now_type.first; }	// �Q�b�^�[
private:
	HierarchyData m_hierarchy_data;	// �K�w�f�[�^
	LPD3DXFRAME m_root_frame;	// ���[�g�t���[��
	LPD3DXANIMATIONCONTROLLER m_anim_controller;	// �A�j���[�V�����Ǘ�
	vector<LPD3DXANIMATIONSET> m_anim_set;	// �A�j���[�V�����擾
	string m_load_pas; // ���f���̃p�X
	vector<string> m_anim_name;	// �A�j���[�V�����̖��O
	vector<pair<string, int>> m_anim_type;	// �A�j���[�V�����̃^�C�v
	pair<string, int> m_anim_now_type;	// ���݂̃A�j���[�V�����̃^�C�v
	D3DXMATRIX m_mtx_wold;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// ��]
	D3DXVECTOR3 m_size;	// �T�C�Y
	D3DXVECTOR3 m_vtx_min;	// ���f���̍ŏ����_
	D3DXVECTOR3 m_vtx_max;	// ���f���̍ő咸�_
	vector<MeshContainer> m_mesh;		//���b�V��
};
#endif