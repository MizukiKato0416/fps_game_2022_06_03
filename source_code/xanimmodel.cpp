//=============================================================================
//
// ���f������ [model.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "xanimmodel.h"
#include "renderer.h"
#include "manager.h"
#include "input_keyboard.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CXanimModel::CXanimModel()
{

}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CXanimModel::~CXanimModel()
{

}

//=============================================================================
// ���f���̏���������
//=============================================================================
HRESULT CXanimModel::Init(void)
{
	int nMaxAnim;

	if (m_anim_controller != NULL)
	{
		nMaxAnim = m_anim_controller->GetMaxNumAnimationSets();
		for (int nCntAnim = 0; nCntAnim < nMaxAnim; nCntAnim++)
		{
			LPD3DXANIMATIONSET Anim;
			m_anim_set.push_back(Anim);
			m_anim_controller->GetAnimationSet(nCntAnim, &m_anim_set[nCntAnim]);
		}
		m_anim_controller->SetTrackAnimationSet(0, m_anim_set[0]);
		m_now_anim = 0;
	}

	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CXanimModel::Uninit(void)
{
	delete m_root_frame;
	delete m_anim_controller;
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CXanimModel::Update(void)
{

}

//=============================================================================
// �S�t���[���̍X�V
//=============================================================================
void CXanimModel::UpdateFrame(LPD3DXFRAME base, LPD3DXMATRIX parent_matrix)
{
	FrameData *frame = (FrameData*)base;

	// �p���s��̍X�V
	if (parent_matrix != NULL)
	{
		D3DXMatrixMultiply(	&frame->CombinedTransformationMatrix,
							&frame->TransformationMatrix,
							parent_matrix);
	}
	else
	{
		frame->CombinedTransformationMatrix = frame->TransformationMatrix;
	}

	// �Z�킪����΍ċA�ŌĂяo��
	if (frame->pFrameSibling != NULL)
	{
		UpdateFrame(frame->pFrameSibling, parent_matrix);
	}

	// �q��������΍ċA�ŌĂяo��
	if (frame->pFrameFirstChild != NULL)
	{
		UpdateFrame(frame->pFrameFirstChild,
					&frame->CombinedTransformationMatrix);
	}
}

//=============================================================================
// �t���[���̕`�揈��
//=============================================================================
void CXanimModel::DrawMatrix(LPD3DXMATRIX matrix)
{
	// �t���[���̍s����X�V
	UpdateFrame(m_root_frame, matrix);
	// �t���[���`��
	DrawFrame(m_root_frame);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CXanimModel::Draw(void)
{
	LPDIRECT3DDEVICE9 device; //�f�o�C�X�̃|�C���^
	device = CManager::GetRenderer()->GetDevice();	//�f�o�C�X���擾����
	D3DXMATRIX mtx_parent, trans_matrix, rot_matrix;

	D3DXMatrixIdentity(&m_mtx_wold);

	D3DXMatrixRotationYawPitchRoll(	&rot_matrix,
									m_rot.y,
									m_rot.x,
									m_rot.z);
	D3DXMatrixMultiply(	&m_mtx_wold,
						&m_mtx_wold,
						&rot_matrix);

	D3DXMatrixTranslation(	&trans_matrix,
							m_pos.x,
							m_pos.y,
							m_pos.z);
	D3DXMatrixMultiply(	&m_mtx_wold,
						&m_mtx_wold,
						&trans_matrix);

	device->GetTransform(D3DTS_WORLD, &mtx_parent);

	//�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃��[���h�}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(	&m_mtx_wold,
						&m_mtx_wold,
						&mtx_parent);

	device->SetTransform(D3DTS_WORLD, &m_mtx_wold);

	DrawMatrix(&m_mtx_wold);
}

//=============================================================================
// �S�t���[���̕`��
//=============================================================================
void CXanimModel::DrawFrame(LPD3DXFRAME frame)
{
	FrameData *frame_data = (FrameData*)frame;
	LPD3DXMESHCONTAINER container_data = frame_data->pMeshContainer;

	// �R���e�i�̐������`�悷��
	while (container_data != NULL)
	{
		DrawMeshContainer(frame, container_data);

		container_data = container_data->pNextMeshContainer;
	}

	// �Z�킪����΍ċA�ŌĂяo��
	if (frame_data->pFrameSibling != NULL)
	{
		DrawFrame(frame_data->pFrameSibling);
	}

	// �q������΍ċA�ŌĂяo��
	if (frame_data->pFrameFirstChild != NULL)
	{
		DrawFrame(frame_data->pFrameFirstChild);
	}
}

//=============================================================================
// �R���e�i�̕`��
//=============================================================================
void CXanimModel::DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	MeshContainer *mesh_container = (MeshContainer*)container;
	pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X���擾����

	// �J�����O
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (mesh_container->pSkinInfo != NULL)
	{
		LPD3DXBONECOMBINATION bone_buffer = (LPD3DXBONECOMBINATION)mesh_container->m_BoneBuffer->GetBufferPointer();	// �{�[���̐��܂킷
		for (DWORD nCntBone = 0; nCntBone < mesh_container->m_BoneNum; nCntBone++)
		{
			// �u�����h����{�[���̐�
			DWORD bone_blend_num = 0;

			// �{�[��ID����u�����h�����������o��
			for (DWORD nCntBlend = 0; nCntBlend < mesh_container->m_BoneWeightNum; nCntBlend++)
			{
				if (bone_buffer[nCntBone].BoneId[nCntBlend] != UINT_MAX)
				{
					bone_blend_num++;
				}
			}

			// ���_�u�����h�̐ݒ�
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, bone_blend_num - 1);

			for (DWORD nCntWeight = 0; nCntWeight < mesh_container->m_BoneWeightNum; nCntWeight++)
			{
				DWORD matrix_index = bone_buffer[nCntBone].BoneId[nCntWeight];
				D3DXMATRIX matrix;

				if (matrix_index != UINT_MAX)
				{
					// �I�t�Z�b�g�s��(m_BoneOffsetMatrix) * �{�[���̍s��(m_BoneMatrix)�ōŐV�̈ʒu������o��
					matrix = mesh_container->m_BoneOffsetMatrix[matrix_index] * (*mesh_container->m_BoneMatrix[matrix_index]);
					pDevice->SetTransform(D3DTS_WORLDMATRIX(nCntWeight), &matrix);
				}
			}
			pDevice->SetMaterial(&mesh_container->pMaterials[bone_buffer[nCntBone].AttribId].MatD3D);
			pDevice->SetTexture(0, mesh_container->m_TextureList[bone_buffer[nCntBone].AttribId]);
			mesh_container->MeshData.pMesh->DrawSubset(nCntBone);
		}
	}
}

//=============================================================================
// �S�{�[���̕`��p�}�g���b�N�X�̕ۑ�
//=============================================================================
HRESULT CXanimModel::AllocateAllBoneMatrix(LPD3DXFRAME frame)
{
	if (frame->pMeshContainer != NULL)
	{
		if (FAILED(AllocateBoneMatrix(frame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}

	// �q�������邩�ǂ���
	if (frame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrix(frame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}

	// �Z�킪���邩�ǂ���
	if (frame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrix(frame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CXanimModel::AllocateBoneMatrix(LPD3DXMESHCONTAINER container)
{
	FrameData *pFrame = NULL;
	DWORD bone_num = 0;

	MeshContainer *original_container = (MeshContainer*)container;
	if (original_container->pSkinInfo == NULL)
	{
		return S_OK;
	}

	bone_num = original_container->pSkinInfo->GetNumBones();

	// �{�[���̃}�g���N�X���̗̈���m�ۂ��ăf�[�^��ۑ�����
	original_container->m_BoneMatrix = new D3DXMATRIX*[bone_num];
	for (DWORD nCntBone = 0; nCntBone < bone_num; nCntBone++)
	{
		pFrame = (FrameData*)D3DXFrameFind(m_root_frame, container->pSkinInfo->GetBoneName(nCntBone));
		if (pFrame == NULL)
		{
			return E_FAIL;
		}
		original_container->m_BoneMatrix[nCntBone] = &pFrame->CombinedTransformationMatrix;
	}

	return S_OK;
}

//=============================================================================
// ���f���̐���
//=============================================================================
CXanimModel *CXanimModel::Create(string type)
{
	CXanimModel *anim_model = nullptr;

	if (anim_model == nullptr)
	{
		anim_model = new CXanimModel;
		if (anim_model != nullptr)
		{
			anim_model->m_load_pas = type;
			anim_model->Load();
			anim_model->Init();
		}
	}

	return anim_model;
}

//=============================================================================
// ���f���̃��[�h
//=============================================================================
void CXanimModel::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X���擾����

	D3DXLoadMeshHierarchyFromX(	m_load_pas.c_str(),
								D3DXMESH_MANAGED,
								pDevice,
								&m_hierarchy_data,
								NULL,
								&m_root_frame,
								&m_anim_controller);
	AllocateAllBoneMatrix(m_root_frame);
}

//=============================================================================
// �A�j���[�V�����ύX
//=============================================================================
void CXanimModel::ChangeAnimation(int anim_num, float speed)
{
	if (m_anim_controller != NULL)
	{
		m_anim_controller->SetTrackSpeed(0, speed);
		m_anim_controller->SetTrackAnimationSet(0, m_anim_set[anim_num]);
		m_anim_controller->SetTrackAnimationSet(1, m_anim_set[m_now_anim]);
		m_anim_controller->SetTrackPosition(0, 0);
		m_anim_controller->AdvanceTime(1.0f, NULL);
		m_now_anim = anim_num;
	}
}

//=============================================================================
// �A�j���[�V�����Đ�
//=============================================================================
void CXanimModel::PlayAnimation(float speed)
{
	if (m_anim_controller != NULL)
	{
		m_anim_controller->SetTrackSpeed(0, speed);
		m_anim_controller->AdvanceTime(1.0f, NULL);
	}
}
