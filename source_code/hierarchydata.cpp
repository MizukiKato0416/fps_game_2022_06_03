//---------------------------
//�C���N���[�h�t�@�C��
//---------------------------
#include "hierarchydata.h"
#include "texture.h"
#include "manager.h"

HRESULT HierarchyData::CreateFrame(THIS_ LPCSTR name, LPD3DXFRAME *new_frame)
{
	// ����t���[���̍쐬
	FrameData *frame = new FrameData();
	ZeroMemory(frame, sizeof(FrameData));

	// ���O�̃R�s�[
	frame->Name = new char[strlen(name) + 1];
	strcpy(frame->Name, name);

	*new_frame = frame;

	return S_OK;
}

HRESULT HierarchyData::CreateMeshContainer(THIS_ LPCSTR name, CONST D3DXMESHDATA *mesh_data, CONST D3DXMATERIAL *material_data, CONST D3DXEFFECTINSTANCE *effect_instance, DWORD material_num, CONST DWORD *adjacency, LPD3DXSKININFO skin_info, LPD3DXMESHCONTAINER *new_mesh_container)
{
	// ���b�V���̎擾
	LPD3DXMESH mesh = mesh_data->pMesh;
	// ���상�b�V���R���e�i�̍쐬
	MeshContainer *container = new MeshContainer;
	// �f�o�C�X
	LPDIRECT3DDEVICE9 device = NULL;
	// �|���S���̐�
	int porigon_num = 0;

	if (container == NULL)
	{
		return E_FAIL;
	}
	ZeroMemory(container, sizeof(MeshContainer));

	// ���O�̃R�s�[
	container->Name = new char[lstrlenA(name) + 1];
	strcpy_s(container->Name, lstrlenA(name) + 1, name);

	// �|���S�����̎擾
	porigon_num = mesh->GetNumFaces();

	// �f�o�C�X�̎擾
	mesh->GetDevice(&device);

	// ���b�V���̃R�s�[
	container->MeshData.Type = mesh_data->Type;

	// �ʏ탁�b�V��
	if (mesh_data->Type == D3DXMESHTYPE_MESH) 
	{
		container->MeshData.pMesh = mesh_data->pMesh;
		container->MeshData.pMesh->AddRef();
	}

	// �v���O���b�V�u���b�V��
	else if (mesh_data->Type == D3DXMESHTYPE_PMESH) 
	{
		container->MeshData.pPMesh = mesh_data->pPMesh;
		container->MeshData.pPMesh->AddRef();
	}

	// �p�b�`���b�V��
	else 
	{
		container->MeshData.pPatchMesh = mesh_data->pPatchMesh;
		container->MeshData.pPatchMesh->AddRef();
	}

	// �אڃ|���S���ԍ��̃R�s�[
	container->pAdjacency = new DWORD[porigon_num * 3];
	memcpy(container->pAdjacency, adjacency, sizeof(DWORD) * porigon_num * 3);

	// �}�e���A���̃R�s�[
	container->NumMaterials = material_num;
	if (material_num > 0)
	{
		container->pMaterials = new D3DXMATERIAL[material_num];

		// �t�@�C���ǂݍ���
		for (int nCntMat = 0; nCntMat < (int)material_num; nCntMat++)
		{
			container->pMaterials[nCntMat].MatD3D = material_data[nCntMat].MatD3D;
			container->pMaterials[nCntMat].MatD3D.Diffuse.a = 1.0f;
			container->pMaterials[nCntMat].MatD3D.Ambient = container->pMaterials[nCntMat].MatD3D.Diffuse;
			container->pMaterials[nCntMat].MatD3D.Specular = container->pMaterials[nCntMat].MatD3D.Diffuse;
			container->pMaterials[nCntMat].MatD3D.Emissive = container->pMaterials[nCntMat].MatD3D.Diffuse;
			container->pMaterials[nCntMat].pTextureFilename = material_data[nCntMat].pTextureFilename;

			if (container->pMaterials[nCntMat].pTextureFilename != NULL)
			{
				/*string *Pas = CManager::GetInstance()->GetTexture()->GetPas();
				string TexPas = "data/Texture/Model/";
				int nTexNum = CTexture::GetTexNum();
				TexPas += container->pMaterials[nCntMat].pTextureFilename;
				for (int nCnt = 0; nCnt < nTexNum; nCnt++, Pas++)
				{
					if (strcmp(TexPas.c_str(), Pas->c_str()) == 0)
					{
						string Enum = CTexture::GetEnumName(nCnt);
						m_TextureList[TexPas.c_str()] = (CTexture::GetTexture(Enum));
						container->m_TextureList.push_back(m_TextureList[TexPas.c_str()]);
						break;
					}
				}*/
				container->m_TextureList.push_back(NULL);
			}
			else 
			{
				container->m_TextureList.push_back(NULL);
			}
		}
	}
	else 
	{
		// �}�e���A���̐���0�������玞�̉��̃}�e���A���ǉ�
		container->NumMaterials = 1;
		container->pMaterials = new D3DXMATERIAL[container->NumMaterials];
		container->m_TextureList.push_back(NULL);
		container->pMaterials[0].pTextureFilename = NULL;
		memset(&container->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		container->pMaterials[0].MatD3D.Diffuse.r = 1.0f;
		container->pMaterials[0].MatD3D.Diffuse.g = 1.0f;
		container->pMaterials[0].MatD3D.Diffuse.b = 1.0f;
		container->pMaterials[0].MatD3D.Diffuse.a = 1.0f;
		container->pMaterials[0].MatD3D.Ambient = container->pMaterials[0].MatD3D.Diffuse;
	}

	container->pSkinInfo = skin_info;
	if (container->pSkinInfo != NULL)
	{
		container->pSkinInfo->AddRef();

		// �{�[���̐����擾
		DWORD bone_num = container->m_BoneNum = skin_info->GetNumBones();
		container->m_BoneOffsetMatrix = new D3DXMATRIX[bone_num];

		for (DWORD nCntBone = 0; nCntBone < bone_num; nCntBone++)
		{
			// �I�t�Z�b�g�s����R�s�[����
			memcpy(&container->m_BoneOffsetMatrix[nCntBone], container->pSkinInfo->GetBoneOffsetMatrix(nCntBone), sizeof(D3DXMATRIX));
		}

		// �{�[���̍œK��
		if (container->pSkinInfo->ConvertToBlendedMesh(
			mesh,				// ���̓��b�V��
			NULL,				// ���g�p
			container->pAdjacency,		// ���b�V���̗אڃf�[�^(����)
			NULL,				// ���b�V���̗אڃf�[�^(�o��)
			NULL,				// �|���S���̐V�K�C���f�b�N�X�̃o�b�t�@
			NULL,				// ���_�̐V�K�C���f�b�N�X�̃o�b�t�@
			&container->m_BoneWeightNum,	// �P�̒��_�ɉe�����y�ڂ��d�݂̐�
			&container->m_BoneNum,		// �{�[���̐�
			&container->m_BoneBuffer,	// �{�[���f�[�^���i�[���ꂽ�o�b�t�@
			&container->MeshData.pMesh))	// �ϊ����
		{
			return E_FAIL;
		}
	}
	*new_mesh_container = container;

	return S_OK;
}

HRESULT HierarchyData::DestroyFrame(THIS_ LPD3DXFRAME frame)
{
	if (frame != NULL)
	{
		if (frame->Name != NULL)
		{
			delete[](frame->Name);
		}
		delete(frame);
	}
	return S_OK;
}

HRESULT HierarchyData::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER base)
{
	MeshContainer *container = (MeshContainer*)base;

	if (container->Name != NULL)
	{
		delete[] (container->Name);
	}

	if (container->pAdjacency != NULL)
	{
		delete[](container->pAdjacency);
	}

	if (container->pMaterials != NULL)
	{
		delete[](container->pMaterials);
	}

	if (container->MeshData.pMesh != NULL)
	{
		container->MeshData.pMesh->Release();
	}

	delete(container);

	return S_OK;
}

