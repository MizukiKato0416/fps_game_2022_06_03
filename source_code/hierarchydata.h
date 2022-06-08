//====================================================
//
// �R�~���j�P�[�V�����f�[�^�̃w�b�_�[�t�@�C��(communicationdata.h)
// Author : �H�� ����
//
//====================================================
#ifndef HIERARCHY_DATA_H_
#define HIERARCHY_DATA_H_
#include "main.h"

//====================================================
// ����t���[���f�[�^
//====================================================
struct FrameData : public D3DXFRAME
{
	// �����s��
    D3DXMATRIXA16 CombinedTransformationMatrix;
};

//====================================================
// ���상�b�V���R���e�i
//====================================================
struct MeshContainer : public D3DXMESHCONTAINER
{
	vector<LPDIRECT3DTEXTURE9> m_TextureList;	// �}�e���A���Ŏg�p����e�N�X�`��
	DWORD m_BoneWeightNum;	// �E�F�C�g�̐�
	DWORD m_BoneNum;	// �{�[���̐�
	LPD3DXBUFFER m_BoneBuffer;	// �{�[�����̃o�b�t�@
	D3DXMATRIX **m_BoneMatrix;	// �{�[���̃}�g���b�N�X�̃|�C���^�̔z��
	D3DXMATRIX *m_BoneOffsetMatrix;	// �{�[���̃I�t�Z�b�g�s��
};

//====================================================
// �K�w�N���X(ID3DXAllocateHierarchy�Ŏg�����z�֐��̂ݒ�`)
//====================================================
class HierarchyData : public ID3DXAllocateHierarchy
{
public:
	HierarchyData() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR name, LPD3DXFRAME *new_frame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR name, CONST D3DXMESHDATA *mesh_data, CONST D3DXMATERIAL *material_data, CONST D3DXEFFECTINSTANCE *effect_instance, DWORD material_num, CONST DWORD *adjacency, LPD3DXSKININFO skin_info, LPD3DXMESHCONTAINER *new_mesh_container);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME frame);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER base);
private:
	map<string, LPDIRECT3DTEXTURE9> m_TextureList;
};

#endif
