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
class CXanimModel : CObject
{
public:
	CXanimModel(CObject::PRIORITY Priority = CObject::PRIORITY::MODEL);	// �f�t�H���g�R���X�g���N�^
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
	void Create(string type);	// ����
	void SetAnimation(int nAnim, float AdjustSpeed);	// �A�j���[�V�����ύX
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }	// �Z�b�^�[
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }	// �Z�b�^�[
	D3DXVECTOR3 GetPos(void) { return m_Pos; }	// �Q�b�^�[
	D3DXVECTOR3 GetRot(void) { return m_Rot; }	// �Q�b�^�[
private:
	HierarchyData m_HierarchyData;	// �K�w�f�[�^
	LPD3DXFRAME m_RootFrame;	// ���[�g�t���[��
	LPD3DXANIMATIONCONTROLLER m_AnimController;	// �A�j���[�V�����Ǘ�
	vector<LPD3DXANIMATIONSET> m_AnimSet;	// �A�j���[�V�����擾
	D3DXMATRIX m_MtxWold;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_Pos;	// �ʒu
	D3DXVECTOR3 m_Rot;	// ��]
	int m_nNowAnim;	// ���݂̃A�j���[�V����
};

#endif
