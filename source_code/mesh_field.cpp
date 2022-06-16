//================================================
//���b�V���t�B�[���h����
//Author:��������
//================================================
#include "renderer.h"
#include "mesh_field.h"
#include "object3D.h"
#include "manager.h"

//================================================
//�ÓI�����o�ϐ��錾
//================================================

//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CMeshField::CMeshField(CObject::PRIORITY Priority):CObject(Priority)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_indexPos.clear();
	m_indexSavePos.clear();
	m_bufferPos.clear();
	m_indexRot.clear();
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLine = 0;
	m_nVertical = 0;
	m_indexMtxWorld.clear();
}

//================================================
//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
//================================================

//================================================
//�f�X�g���N�^
//================================================
CMeshField::~CMeshField()
{

}

//================================================
//����������
//================================================
HRESULT CMeshField::Init(void)
{
	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//�ʒu�E�T�C�Y�ݒ菈��
	CObject::SetPos(m_pos);
	CObject::SetSize(m_size);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nVertical + 1) * (m_nLine + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);


	VERTEX_3D *pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nNum = 0;
	if (m_bufferPos.size() == 0)
	{
		for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
		{
			for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
			{
					m_bufferPos.push_back(D3DXVECTOR3(-m_size.x / 2.0f + (m_size.x / m_nLine) * nLine,
			                                          0.0f,
			                                          m_size.z / 2.0f - (m_size.z / m_nVertical) * nVertical));
			}
		}
	}

	nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			//�v�f��ǉ�
			m_bufferSavePos.push_back({0.0f, 0.0f, 0.0f});

			//���_���W
			pVtx[nNum].pos = m_bufferPos[nNum];
			//�J���[
			pVtx[nNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//�e�N�X�`��
			pVtx[nNum].tex = D3DXVECTOR2(0.0f + (1.0f * nLine), 0.0f + (1.0f * nVertical));
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			D3DXVECTOR3 vecPos[2];

			if (nVertical < m_nVertical)
			{
				if (nLine < m_nLine)
				{
					vecPos[0] = m_bufferPos[nNum + 1] - m_bufferPos[nNum];
					vecPos[1] = m_bufferPos[nNum + (m_nLine + 2)] - m_bufferPos[nNum];
				}
				else
				{
					vecPos[0] = m_bufferPos[nNum + (m_nLine + 1)] - m_bufferPos[nNum];
					vecPos[1] = m_bufferPos[nNum - 1] - m_bufferPos[nNum];
				}
			}
			else
			{
				if (nLine < m_nLine)
				{
					vecPos[0] = m_bufferPos[nNum / 2] - m_bufferPos[nNum];
					vecPos[1] = m_bufferPos[nNum + 1] - m_bufferPos[nNum];
				}
				else
				{
					vecPos[0] = m_bufferPos[nNum - (m_nLine + 1)] - m_bufferPos[nNum];
					vecPos[1] = m_bufferPos[nNum - (m_nLine + 1) - 1] - m_bufferPos[nNum];
				}
			}



			//�@�������߂�
			D3DXVECTOR3 vecNor;
			D3DXVec3Cross(&vecNor, &vecPos[0], &vecPos[1]);
			//���K������
			D3DXVec3Normalize(&vecNor, &vecNor);
			//vecNor = {0.0f, 1.0f, 0.0f};

			//�@���ݒ�
			pVtx[nNum].nor = vecNor;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((m_nVertical - 1) * (m_nLine + 3) + (m_nLine + 1) * (m_nVertical + 1)),
								D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, nullptr);

	WORD *pIdx;		//�C���f�b�N�X���ւ̃|�C���^
	//�C���f�b�N�X�o�b�t�@�����b�N���ԍ��f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt1 = 0; nCnt1 < m_nVertical; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < m_nLine + 1; nCnt2++)
		{
			//�ԍ��f�[�^�̐ݒ�
			pIdx[(nCnt2 * 2) + 0 + (m_nLine + 2) * 2 * nCnt1] = static_cast<WORD>((m_nLine + 1) + nCnt2 + (m_nLine + 1) * nCnt1);
			pIdx[(nCnt2 * 2) + 1 + (m_nLine + 2) * 2 * nCnt1] = static_cast<WORD>(0 + nCnt2 + (m_nLine + 1) * nCnt1);
			//�v�f��ǉ�
			m_indexPos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_indexPos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_indexSavePos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_indexSavePos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			D3DXMATRIX temp;
			m_indexMtxWorld.push_back(temp);
			m_indexMtxWorld.push_back(temp);
			m_indexRot.push_back(m_rot);
			m_indexRot.push_back(m_rot);
		}
	}

	for (int nCnt3 = 0; nCnt3 < m_nVertical - 1; nCnt3++)
	{
		pIdx[((m_nLine + 1) * 2 + 0) * (nCnt3 + 1) + (2 * nCnt3)] = static_cast<WORD>(m_nLine + (m_nLine + 1) * nCnt3);
		pIdx[((m_nLine + 1) * 2 + 1) * (nCnt3 + 1) + (1 * nCnt3)] = static_cast<WORD>(m_nLine * 2 + 2 + (m_nLine + 1) * nCnt3);
		//�v�f��ǉ�
		m_indexPos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_indexPos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_indexSavePos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_indexSavePos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		D3DXMATRIX temp;
		m_indexMtxWorld.push_back(temp);
		m_indexMtxWorld.push_back(temp);
		m_indexRot.push_back(m_rot);
		m_indexRot.push_back(m_rot);
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
	
	//���_���̕ۑ�
	for (int nCnt1 = 0; nCnt1 < m_nVertical; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < m_nLine + 1; nCnt2++)
		{
			m_indexPos[(nCnt2 * 2) + 0 + (m_nLine + 2) * 2 * nCnt1] = m_bufferPos[((m_nLine + 1) + nCnt2 + (m_nLine + 1) * nCnt1)];
			m_indexPos[(nCnt2 * 2) + 1 + (m_nLine + 2) * 2 * nCnt1] = m_bufferPos[(0 + nCnt2 + (m_nLine + 1) * nCnt1)];
		}
	}

	for (int nCnt3 = 0; nCnt3 < m_nVertical - 1; nCnt3++)
	{
		m_indexPos[((m_nLine + 1) * 2 + 0) * (nCnt3 + 1) + (2 * nCnt3)] = m_bufferPos[(m_nLine + (m_nLine + 1) * nCnt3)];
		m_indexPos[((m_nLine + 1) * 2 + 1) * (nCnt3 + 1) + (1 * nCnt3)] = m_bufferPos[(m_nLine * 2 + 2 + (m_nLine + 1) * nCnt3)];
	}

	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CObject::OBJTYPE::FLOOR);
	return S_OK;
}

//================================================
//�I������
//================================================
void CMeshField::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//================================================
//�X�V����
//================================================
void CMeshField::Update(void)
{
}

//================================================
//�`�揈��
//================================================
void CMeshField::Draw(void)
{
	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);		//���[���h�}�g���b�N�X�̏�����

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	pDevice->SetFVF(FVF_VERTEX_3D);					//���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0, m_pTexture);				//�e�N�X�`���̐ݒ�

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
									0,
									0,
									((m_nLine + 1) * (m_nVertical + 1)),				//���_�̐�
									0,													//�J�n���钸�_�̃C���f�b�N�X
									2 * m_nLine * m_nVertical + (m_nVertical * 4) - 4);	//�`�悷��v���~�e�B�u��

	
	////�J�����̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
	//CCamera** pCameraAddress = CManager::GetInstance()->GetCamera();
	////camera�̎擾
	//CCamera* pCamera = &**pCameraAddress;
	//if (pCamera != nullptr)
	//{
	//	for (int nCnt1 = 0; nCnt1 < m_indexPos.size() - 2; nCnt1++)
	//	{
	//		//�Ώۂ̌��݈ʒu�擾
	//		D3DXVECTOR3 pos[3];
	//		pos[0] = m_indexPos[nCnt1];
	//		pos[1] = m_indexPos[nCnt1 + 1];
	//		pos[2] = m_indexPos[nCnt1 + 2];

	//		//�X�N���[�����W�ɕϊ�
	//		D3DXVECTOR3 screenPos[3];
	//		screenPos[0] = pCamera->WorldPosToScreenPos(pos[0]);
	//		screenPos[1] = pCamera->WorldPosToScreenPos(pos[1]);
	//		screenPos[2] = pCamera->WorldPosToScreenPos(pos[2]);

	//		//�X�N���[���ɉf���Ă���Ƃ�
	//		if ((screenPos[0].x >= 0.0f && screenPos[0].x <= SCREEN_WIDTH && screenPos[0].y >= 0.0f && screenPos[0].y <= SCREEN_HEIGHT) ||
	//			(screenPos[1].x >= 0.0f && screenPos[1].x <= SCREEN_WIDTH && screenPos[1].y >= 0.0f && screenPos[1].y <= SCREEN_HEIGHT) ||
	//			(screenPos[2].x >= 0.0f && screenPos[2].x <= SCREEN_WIDTH && screenPos[2].y >= 0.0f && screenPos[2].y <= SCREEN_HEIGHT))
	//		{
	//			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//			pDevice->SetIndices(m_pIdxBuff);

	//			pDevice->SetFVF(FVF_VERTEX_3D);					//���_�t�H�[�}�b�g�̐ݒ�

	//			pDevice->SetTexture(0, m_pTexture);				//�e�N�X�`���̐ݒ�

	//			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
	//				0,
	//				0,
	//				((m_nLine + 1) * (m_nVertical + 1)),				//���_�̐�
	//				nCnt1,												//�J�n���钸�_�̃C���f�b�N�X
	//				2);	//�`�悷��v���~�e�B�u��
	//		}
	//	}
	//}
}

//================================================
//��������
//================================================
CMeshField* CMeshField::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXVECTOR3 &rot, const int &nLine, const int &nVertical)
{
	//�C���X�^���X�̐���
	CMeshField *pMeshField = nullptr;
	if (pMeshField == nullptr)
	{
		pMeshField = new CMeshField;
		if (pMeshField != nullptr)
		{
			//�ϐ�������
			pMeshField->m_rot = rot;
			pMeshField->m_nLine = nLine;
			pMeshField->m_nVertical = nVertical;
			pMeshField->m_pos = pos;
			pMeshField->m_size = size;
			//����������
			pMeshField->Init();
		}
	}
	return pMeshField;
}

//================================================
//�e�L�X�g�t�@�C������̐�������
//================================================
CMeshField * CMeshField::CreateLoadText(string sPas)
{
	//�C���X�^���X�̐���
	CMeshField *pMeshField = nullptr;
	if (pMeshField == nullptr)
	{
		pMeshField = new CMeshField;
		if (pMeshField != nullptr)
		{
			//�t�@�C���̓ǂݍ���
			FILE *pFile;
			pFile = fopen(sPas.c_str(), "r");
			if (pFile != NULL)
			{
				char cStr[128];
				while (fgets(cStr, 128, pFile) != nullptr)
				{
					if (strncmp("SET_MESHFIELD\n", cStr, 15) == 0)
					{
						fscanf(pFile, "%*s%*s%d", &pMeshField->m_nVertical);
						fscanf(pFile, "%*s%*s%d", &pMeshField->m_nLine);
						fscanf(pFile, "%*s%*s%f", &pMeshField->m_size.x);
						fscanf(pFile, "%*s%*s%f", &pMeshField->m_size.z);

						int nCntVtx = 0;
						for (int nVertical = 0; nVertical < pMeshField->m_nVertical + 1; nVertical++)
						{
							for (int nLine = 0; nLine < pMeshField->m_nLine + 1; nLine++, nCntVtx++)
							{
								pMeshField->m_bufferPos.push_back({ 0.0f, 0.0f, 0.0f });
								fscanf(pFile, "%f %f %f\n", &pMeshField->m_bufferPos[nCntVtx].x,
									                        &pMeshField->m_bufferPos[nCntVtx].y,
									                        &pMeshField->m_bufferPos[nCntVtx].z);
							}
						}
					}
				}
			}
			else
			{
				printf("�t�@�C�����J���܂���ł���\n");
			}
			fclose(pFile);

			//����������
			pMeshField->Init();
		}
	}
	return pMeshField;
}

//================================================
//�e�N�X�`�����蓖�ď���
//================================================
void CMeshField::BindTexture(const LPDIRECT3DTEXTURE9 &pTexture)
{
	m_pTexture = pTexture;
}

//================================================
//�����蔻��
//================================================
bool CMeshField::Collision(CObject *pSubjectObject, const float &fRadius)
{
	//���n���Ă��邩�ǂ���
	bool bLand = false;

	//�I�u�W�F�N�g��������|�C���^
	vector<CObject*> object;

	//�擪�̃|�C���^����
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MESH_FIELD));
	int nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::FLOOR)
		{
			//�|�C���^��CMeshField�ɃL���X�g
			CMeshField *pMeshField = nullptr;
			pMeshField = (CMeshField*)object[nCnt];

			for (int nCnt1 = 0; nCnt1 < 2 * pMeshField->m_nLine * pMeshField->m_nVertical + (pMeshField->m_nVertical * 4) - 4; nCnt1++)
			{
				//�Ώۂ̌��݈ʒu�擾
				D3DXVECTOR3 pos = pSubjectObject->GetPos();
				D3DXVECTOR3 diff = pos - pMeshField->m_indexPos[nCnt1];

				if (D3DXVec3LengthSq(&diff) > fRadius*fRadius)
				{
					continue;
				}

				//�e���_����e���_�̃x�N�g�����Z�o
				D3DXVECTOR3 vecVtx[VERTEX_3D_NUM - 1];
				
				//����
				if (nCnt1 % 2 == 0)
				{
					vecVtx[0] = pMeshField->m_indexPos[nCnt1 + 1] - pMeshField->m_indexPos[nCnt1];
					vecVtx[1] = pMeshField->m_indexPos[nCnt1 + 2] - pMeshField->m_indexPos[nCnt1 + 1];
					vecVtx[2] = pMeshField->m_indexPos[nCnt1] - pMeshField->m_indexPos[nCnt1 + 2];
				}
				else
				{//�
					vecVtx[0] = pMeshField->m_indexPos[nCnt1] - pMeshField->m_indexPos[nCnt1 + 2];
					vecVtx[1] = pMeshField->m_indexPos[nCnt1 + 1] - pMeshField->m_indexPos[nCnt1];
					vecVtx[2] = pMeshField->m_indexPos[nCnt1 + 2] - pMeshField->m_indexPos[nCnt1 + 1];
				}

				//�@���ۑ��p
				D3DXVECTOR3 vecNor;

				D3DXVECTOR3 vecVtxInv = -vecVtx[2];

				//�|���S���̖@�������߂�
				D3DXVec3Cross(&vecNor, &vecVtx[0], &vecVtxInv);

				//nCnt����̎�
				if (nCnt1 % 2 == 1)
				{
					vecNor *= -1.0f;
				}

				//�x�N�g���𐳋K������(�x�N�g���̑傫����1�ɂ���)
				D3DXVec3Normalize(&vecNor, &vecNor);

				//�e���_����Ώۂ̌��݈ʒu�܂ł̃x�N�g�����Z�o
				D3DXVECTOR3 vecObj[VERTEX_3D_NUM - 1];
				vecObj[0] = pos - pMeshField->m_indexPos[nCnt1];
				vecObj[1] = pos - pMeshField->m_indexPos[nCnt1 + 1];
				vecObj[2] = pos - pMeshField->m_indexPos[nCnt1 + 2];
				
				//�e���_����Ώۂ̌��݈ʒu�܂ł̃x�N�g���Ɗe���_����e���_�̃x�N�g���̊O��(2D)�����߂�
				float fvecCross[VERTEX_3D_NUM - 1];

				for (int nCntVec = 0; nCntVec < VERTEX_3D_NUM - 1; nCntVec++)
				{
					//�O�ς̌v�Z
					fvecCross[nCntVec] = (vecVtx[nCntVec].x * vecObj[nCntVec].z) - (vecObj[nCntVec].x * vecVtx[nCntVec].z);

					//nCnt����̎�
					if (nCnt1 % 2 == 1)
					{
						fvecCross[nCntVec] *= -1.0f;
					}
				}
				
				//�|���S���͈͓̔��ɂ��邩�ǂ���
				if (fvecCross[0] <= 0.0f && fvecCross[1] <= 0.0f && fvecCross[2] <= 0.0f)
				{
					//�|���S���̈ʒu����Ώۂ̌��݂̈ʒu�̃x�N�g�����Z�o
					D3DXVECTOR3 vecPos;
					vecPos = pos - pMeshField->m_indexPos[nCnt1];

					//�Z�o�����x�N�g���Ɩ@���̃x�N�g���̓��ς����߂�
					float fVecDot;
					fVecDot = D3DXVec3Dot(&vecNor, &vecPos);

					//���ς̌v�Z���ʂ��}�C�i�X�̎�
					if (fVecDot < 0.0f)
					{
						//�����o���ʒu�����߂�
						D3DXVECTOR3 objectPos = pos;

						objectPos.y = pMeshField->m_indexPos[nCnt1].y - 1.0f / vecNor.y *
							          (vecNor.x * (objectPos.x - pMeshField->m_indexPos[nCnt1].x) +
								       vecNor.z * (objectPos.z - pMeshField->m_indexPos[nCnt1].z));

						pSubjectObject->SetPos(objectPos);
						return true;
					}
				}
			}
		}
	}
	return bLand;
}

//================================================
//�����蔻�肾��
//================================================
bool CMeshField::Collision(const D3DXVECTOR3 &HitPos, const float &fDiffer, const D3DXVECTOR3 &beginPos, const D3DXVECTOR3 &endPos)
{
	//�n�_
	D3DXVECTOR3 posBegin = beginPos;
	//�I�_
	D3DXVECTOR3 posEnd = endPos;
	//�n�_����I�_�܂ł̃x�N�g��
	D3DXVECTOR3 vec = posEnd - posBegin;
	//�������v�Z
	float fDifferVec = D3DXVec3Length(&vec);
	float fDifferNear = 100000.0f;

	//�����������ǂ���
	bool bHit = false;

	//�I�u�W�F�N�g��������|�C���^
	vector<CObject*> object;

	//�擪�̃|�C���^����
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MESH_FIELD));
	int nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::FLOOR)
		{
			//�|�C���^��CMeshField�ɃL���X�g
			CMeshField *pMeshField = nullptr;
			pMeshField = (CMeshField*)object[nCnt];

			for (int nCnt1 = 0; nCnt1 < 2 * pMeshField->m_nLine * pMeshField->m_nVertical + (pMeshField->m_nVertical * 4) - 4; nCnt1++)
			{
				//�Ώۂ̌��݈ʒu�擾
				D3DXVECTOR3 diff[VERTEX_3D_NUM - 1];
				diff[0] = posBegin - pMeshField->m_indexPos[nCnt1];
				diff[1] = posBegin - pMeshField->m_indexPos[nCnt1 + 1];
				diff[2] = posBegin - pMeshField->m_indexPos[nCnt1 + 2];

				if (D3DXVec3LengthSq(&diff[0]) > fDifferVec && D3DXVec3LengthSq(&diff[1]) > fDifferVec && D3DXVec3LengthSq(&diff[2]) > fDifferVec)
				{
					continue;
				}

				//�e���_����e���_�̃x�N�g�����Z�o
				D3DXVECTOR3 vecVtx[VERTEX_3D_NUM - 1];

				//����
				if (nCnt1 % 2 == 0)
				{
					vecVtx[0] = pMeshField->m_indexPos[nCnt1 + 1] - pMeshField->m_indexPos[nCnt1];
					vecVtx[1] = pMeshField->m_indexPos[nCnt1 + 2] - pMeshField->m_indexPos[nCnt1 + 1];
					vecVtx[2] = pMeshField->m_indexPos[nCnt1] - pMeshField->m_indexPos[nCnt1 + 2];
				}
				else
				{//�
					vecVtx[0] = pMeshField->m_indexPos[nCnt1] - pMeshField->m_indexPos[nCnt1 + 2];
					vecVtx[1] = pMeshField->m_indexPos[nCnt1 + 1] - pMeshField->m_indexPos[nCnt1];
					vecVtx[2] = pMeshField->m_indexPos[nCnt1 + 2] - pMeshField->m_indexPos[nCnt1 + 1];
				}

				//�@���ۑ��p
				D3DXVECTOR3 vecNor;

				D3DXVECTOR3 vecVtxInv = -vecVtx[2];

				//�|���S���̖@�������߂�
				D3DXVec3Cross(&vecNor, &vecVtx[0], &vecVtxInv);

				//nCnt����̎�
				if (nCnt1 % 2 == 1)
				{
					vecNor *= -1.0f;
				}

				//�x�N�g���𐳋K������(�x�N�g���̑傫����1�ɂ���)
				D3DXVec3Normalize(&vecNor, &vecNor);

				//�|���S���̈ʒu����Ώۂ̌��݂̈ʒu�̃x�N�g�����Z�o
				D3DXVECTOR3 vecPosBegin;
				D3DXVECTOR3 vecPosEnd;
				vecPosBegin = posBegin - pMeshField->m_indexPos[nCnt1];
				vecPosEnd = posEnd - pMeshField->m_indexPos[nCnt1];

				//�Z�o�����x�N�g���Ɩ@���̃x�N�g���̓��ς����߂�
				float fVecDotBegin;
				float fVecDotEnd;
				fVecDotBegin = D3DXVec3Dot(&vecNor, &vecPosBegin);
				fVecDotEnd = D3DXVec3Dot(&vecNor, &vecPosEnd);

				//���ς̌v�Z���ʂ��}�C�i�X�̎�
				if (fVecDotBegin > 0.0f && fVecDotEnd < 0.0f)
				{
					D3DXVECTOR3 hitPos = (1 - (fVecDotBegin / (fVecDotBegin + fVecDotEnd)))*vecPosBegin + (fVecDotBegin / (fVecDotBegin + fVecDotEnd)) * vecPosEnd;

					hitPos += pMeshField->m_indexPos[nCnt1];
					D3DXVECTOR3 differBegin = hitPos - pMeshField->m_indexPos[nCnt1];
					float fDifferBegin = D3DXVec3Length(&differBegin);

					if (fDifferBegin < fDifferNear)
					{
						fDifferNear = fDifferBegin;
						hitPos = const_cast<D3DXVECTOR3&>(HitPos);
						fDifferBegin = const_cast<float&>(fDiffer);
						bHit = true;
					}
				}
			}
		}
	}
	return bHit;
}
