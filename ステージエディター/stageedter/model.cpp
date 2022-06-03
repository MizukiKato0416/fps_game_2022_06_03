//=============================================================================
//
// X�t�@�C���ǂݍ��ݏ��� [sceneX.cpp]
// Author : �H������&�ߊԏr��v
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "block.h"
#include "camera.h"
#include "texture.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CModel::ModelData *CModel::m_aModelData;
int CModel::m_nModelMax;	// �ǂݍ��ރe�N�X�`���̍ő吔
char **CModel::m_aFilePas;	// �t�@�C���̃p�X

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// ���f���̏���������
//=============================================================================
HRESULT CModel::Init(int type)
{
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	LPDIRECT3DDEVICE9 pDevice; // �f�o�C�X�̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X���擾����
	m_nType = type;

	//---------------------------
	//�e�N�X�`���̔����o��
	//---------------------------
	D3DXMATERIAL *pMat;	//�}�e���A���̃|�C���^
	int nTexNum = CManager::GetTexture()->GetTexNum();

	//�}�e���A�����̃|�C���^
	pMat = (D3DXMATERIAL*)m_aModelData[m_nType].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aModelData[m_nType].nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			for (int nCnt = 0; nCnt < nTexNum; nCnt++)
			{
				char *apPas = CManager::GetTexture()->GetPasName(nCnt);
				if (strcmp(pMat[nCntMat].pTextureFilename, apPas) == 0)
				{
					m_pTexture[nCntMat] = CManager::GetTexture()->GetTexture(nCnt);
				}
			}
		}
	}

	//---------------------------
	//���_���W�̔����o��
	//---------------------------
	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;	//���_�o�b�t�@�̃|�C���^

	//���_�����擾
	nNumVtx = m_aModelData[(int)type].pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_aModelData[(int)type].pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_aModelData[(int)type].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

		//���_�����r���čŏ��l�A�ő�l�𔲂�
		//X
		if (m_vtxMax.x <= vtx.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (m_vtxMin.x >= vtx.x)
		{
			m_vtxMin.x = vtx.x;
		}
		//Y
		if (m_vtxMax.y <= vtx.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (m_vtxMin.y >= vtx.y)
		{
			m_vtxMin.y = vtx.y;
		}
		//Z
		if (m_vtxMax.z <= vtx.z)
		{
			m_vtxMax.z = vtx.z;
		}
		if (m_vtxMin.z >= vtx.z)
		{
			m_vtxMin.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	//���_��i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	m_aModelData[(int)type].pMesh->UnlockVertexBuffer();

	return S_OK;
}

HRESULT CModel::Init(void)
{
	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CModel::Uninit(void)
{

}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CModel::Update(void)
{
	
}

//=============================================================================
// ���f���̕`�揈��
//=============================================================================
void CModel::Draw(bool bUse)
{
	LPDIRECT3DDEVICE9 pDevice; // �f�o�C�X�̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X���擾����

	//--------------------------------------
	//�v���C���[(���_)�̃}�g���b�N�X�̐ݒ�
	//--------------------------------------
	D3DXMATRIX mtxRot, mtxScale, mtxTrans, mtxParent;	//�v�Z�p�}�g���b�N�X

	D3DXMatrixIdentity(&m_MtxWold);	//�}�g���b�N�X������

	//�X�P�[���̐ݒ�
	D3DXMatrixScaling(	&mtxScale,
						m_Scale.x,
						m_Scale.y,
						m_Scale.z);

	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&mtxScale);

	//�����̐ݒ�
	D3DXMatrixRotationYawPitchRoll(	&mtxRot,
									m_Rot.y,
									m_Rot.x,
									m_Rot.z);

	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&mtxRot);
	//�ʒu
	D3DXMatrixTranslation(	&mtxTrans,
							m_Pos.x,
							m_Pos.y,
							m_Pos.z);

	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&mtxTrans);

	//�p�[�c�̐e�̃}�g���b�N�X��ݒ�
	//�����e��������
	if (m_pPrent != NULL)
	{
		mtxParent = m_pPrent->GetMatrix();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃��[���h�}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&mtxParent);

	//�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(	D3DTS_WORLD,
							&m_MtxWold);

	//---------------------------
	//�`�揈��
	//---------------------------
	D3DMATERIAL9 matDef;	//�}�e���A���̈ꎞ�ۑ�
	D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�̃|�C���^

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�̃|�C���^
	pMat = (D3DXMATERIAL*)m_aModelData[m_nType].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aModelData[m_nType].nNumMat; nCntMat++)
	{
		if (bUse == true)
		{
			pMat[nCntMat].MatD3D.Diffuse.a = 0.5;
		}
		else if (bUse == false)
		{
			pMat[nCntMat].MatD3D.Diffuse.a = 1.0;
		}

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(	0,
								m_pTexture[nCntMat]);

		//���f���p�[�c�̕`��
		m_aModelData[m_nType].pMesh->DrawSubset(nCntMat);
	}
	//�ꎞ�ۑ����Ă�������ǂ�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//���f���f�[�^�̓ǂݍ���
//=============================================================================
HRESULT CModel::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X���擾����
	FILE *pFile;
	char aFile[PASCHAR_MAX * 2];

	pFile = fopen("data/Txtdata/modeldata.txt", "r");

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "PASNUM") == 0) // PASNUM�̕��������������
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &m_nModelMax);
				m_aFilePas = new char*[m_nModelMax];
				for (int nCnt = 0; nCnt < m_nModelMax; nCnt++)
				{
					m_aFilePas[nCnt] = new char[PASCHAR_MAX];
				}
				m_aModelData = new ModelData[m_nModelMax];
			}
			else if (strcmp(&aFile[0], "MODEL_FILENAME") == 0) // MODEL_FILENAME�̕��������������
			{
				for (int nCnt = 0; nCnt < m_nModelMax; nCnt++)
				{
					fscanf(pFile, "%s", m_aFilePas[nCnt]);
					fscanf(pFile, "%s%s", &aFile[0], &aFile[1]);
				}
			}
			else if (strcmp(&aFile[0], "END") == 0) //END�̕��������������
			{
				break;
			}
		}
	}
	else
	{
		printf("�e�N�X�`���f�[�^���ǂݎ��܂���ł����B");
	}

	fclose(pFile);

	//-------------------------------------
	//���f���̓ǂݍ���
	//-------------------------------------
	for (int nCnt = 0; nCnt < m_nModelMax; nCnt++)
	{
		D3DXLoadMeshFromX(	m_aFilePas[nCnt],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,		//�����܂ŌŒ�
							&m_aModelData[nCnt].pBuffMat,
							NULL,
							&m_aModelData[nCnt].nNumMat,
							&m_aModelData[nCnt].pMesh);
	}

	return S_OK;
}

//=============================================================================
// ���f���̐���
//=============================================================================
CModel *CModel::Create(int type)
{
	CModel *pModel = NULL;
	if (pModel == NULL)
	{
		pModel = new CModel;

	}
	if (pModel)
	{
		pModel->Init(type);
	}
	return pModel;
}