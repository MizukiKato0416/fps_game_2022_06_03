//=============================================================================
//
// X�t�@�C�����f������ [sceneX.h]
// Author : �H������&�ߊԏr��
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXT_FILE_NAME_MODEL "data/TEXT/load_model.txt"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CModel
{
public:
	typedef struct
	{
		LPD3DXMESH pMesh;	//���b�V���i���_���j�ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;	//�}�e���A���ւ̃|�C���^
		DWORD nNumMat;	//�}�e���A���̐�
	}ModelData;
	CModel();	// �f�t�H���g�R���X�g���N�^
	~CModel();	// �f�t�H���g�f�X�g���N�^
	virtual HRESULT Init(int type);	// �|���S���̏�����
	virtual HRESULT Init(void);	// �|���S���̏�����
	virtual void Uninit(void);	// �|���S���̏I��
	virtual void Update(void);	// �|���S���̍X�V
	virtual void Draw(bool bUse);	// �|���S���̕`��
	static HRESULT Load(void);	//���f���f�[�^�̓ǂݍ���
	static CModel *Create(int type);	// ����
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }	//�ʒu���W�̐ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }	//�p�x�̐ݒ�
	void SetScale(D3DXVECTOR3 scale) { m_Scale = scale; }	//�p�x�̐ݒ�
	void SetMatrix(D3DXMATRIX mtx) { m_MtxWold = mtx; }	// ���[���h�}�g���b�N�X�̃Q�b�_�[
	void SetPrent(CModel *pModel) { m_pPrent = pModel; }	// �e�̃Z�b�^�[
	D3DXVECTOR3 GetPos(void) { return m_Pos; }		//�ʒu���W�̎擾
	D3DXVECTOR3 GetRot(void) { return m_Rot; }		//�p�x�̎擾
	D3DXVECTOR3 GetScale(void) { return m_Scale; }
	D3DXMATRIX GetMatrix(void) { return m_MtxWold; }	// ���[���h�}�g���b�N�X�̃Q�b�_�[
	D3DXVECTOR3 GetVMin(void) { return m_vtxMin; }
	D3DXVECTOR3 GetVMax(void) { return m_vtxMax; }
private:
	static ModelData *m_aModelData;	//�S���f���̏��
	static int m_nModelMax;	// �ǂݍ��ރe�N�X�`���̍ő吔
	static char **m_aFilePas;	// �t�@�C���̃p�X
	int m_nType;	//���f���̎��
	CModel *m_pPrent;	// �e�̃��f��
	D3DXMATRIX m_MtxWold;	//���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 m_pTexture[64];	// �e�N�X�`��
	D3DXVECTOR3 m_vtxMin, m_vtxMax;	//���_�̒[���
	D3DXVECTOR3 m_Pos;		//�ʒu
	D3DXVECTOR3 m_Rot;		//����
	D3DXVECTOR3 m_Scale;
};

#endif
