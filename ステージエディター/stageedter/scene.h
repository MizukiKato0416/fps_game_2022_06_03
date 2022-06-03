//=============================================================================
//
// �I�u�W�F�N�g���� [scene.h]
// Author : �H������
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "texture.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene
{
public:
	enum class LAYER_TYPE
	{
		LAYER_00 = 0,	// ���C���[(0)
		LAYER_01,		// ���C���[(1)
		LAYER_02,		// ���C���[(2)
		LAYER_03,		// ���C���[(3)
		LAYER_04,		// ���C���[(4)
		LAYER_05,		// ���C���[(5)
		MAX_LAYER,		// ���C���[����
	};
	CScene(LAYER_TYPE Layer = LAYER_TYPE::LAYER_00);	// �f�t�H���g�R���X�g���N�^
	virtual ~CScene();	// �f�t�H���g�f�X�g���N�^
	virtual HRESULT Init(void) = 0;	// �|���S���̏�����
	virtual void Uninit(void) = 0;	// �|���S���̏I��
	virtual void Update(void) = 0;	// �|���S���̍X�V
	virtual void Draw(void) = 0;	// �|���S���̕`��
	static void ReleaseAll(void);	// �I�u�W�F�N�g�̔j��
	static void UpdateAll(void);	// �I�u�W�F�N�g�̍X�V����
	static void DrawAll(void);		// �I�u�W�F�N�g�̕`��
	D3DXVECTOR3 GetPos(void) { return m_Pos; }			// �ʒu�Q�b�_�[
	D3DXVECTOR2 GetVec2Size(void) { return m_Vec2Size; }		// �T�C�Y�Q�b�_�[
	D3DXVECTOR3 GetVec3Size(void) { return m_Vec3Size; }		// �T�C�Y�Q�b�_�[
	CScene *GetTopScene(int nPriority) { return m_pTop[nPriority]; }	// Top�̃Q�b�_�[
	CScene *GetNextScene(CScene *pScene) { return pScene->m_pNext; }	// ������Next�̃Q�b�_�[
	bool GetDeath(void) { return m_bDeth; }	// ���S�t���O�̃Q�b�_�[
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }		// �ʒu�Z�b�^�[
	void SetVec2Size(D3DXVECTOR2 size) { m_Vec2Size = size; }	// �T�C�Y�Z�b�^�[
	void SetVec3Size(D3DXVECTOR3 size) { m_Vec3Size = size; }	// �T�C�Y�Z�b�^�[
private:
	D3DXVECTOR3	m_Pos;		// �|�X
	D3DXVECTOR3 m_Vec3Size;		// �T�C�Y
	D3DXVECTOR2 m_Vec2Size;		// �T�C�Y
	CScene *m_pPrev;	// �O�̃I�u�W�F�N�g
	CScene *m_pNext;	// ���̃I�u�W�F�N�g
	static CScene *m_pTop[(int)LAYER_TYPE::MAX_LAYER];	// �擪
	static CScene *m_pBottom[(int)LAYER_TYPE::MAX_LAYER];	// �Ō��
	static bool m_bFirst;
	int m_nPriority;	// �`�揇
	bool m_bDeth;	// ���S�t���O
protected:
	void Release(void) { m_bDeth = true; }		// ���S�t���O���Ă�
};

#endif