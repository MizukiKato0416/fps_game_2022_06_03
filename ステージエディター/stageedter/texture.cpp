//=============================================================================
//
// �e�N�X�`������ [texture.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "texture.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// �e�N�X�`���̐���
//=============================================================================
void CTexture::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();	//�f�o�C�X���擾����
	FILE *pFile;
	char aFile[64];

	pFile = fopen("data/Txtdata/texturedata.txt", "r");

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "TEX_NUM") == 0) // TEX_NUM�̕��������������
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &m_nTextureMax);
				m_aFilePas = new char*[m_nTextureMax];
				m_pTexture = new LPDIRECT3DTEXTURE9[m_nTextureMax];
				for (int nCnt = 0; nCnt < m_nTextureMax; nCnt++)
				{
					m_aFilePas[nCnt] = new char[PASCHAR_MAX];
				}
			}
			else if (strcmp(&aFile[0], "TEXTURE_PAS") == 0) // TEXTURE_PAS�̕��������������
			{
				for (int nCnt = 0; nCnt < m_nTextureMax; nCnt++)
				{
					fscanf(pFile, "%s", m_aFilePas[nCnt]);
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

	for (int nCnt = 0; nCnt < m_nTextureMax; nCnt++)
	{
		D3DXCreateTextureFromFile(	pDevice,
									m_aFilePas[nCnt],	// �e�N�X�`���p�X
									&m_pTexture[nCnt]);
	}
}	

//=============================================================================
// �I��
//=============================================================================
void CTexture::Uniinit(void)
{
	for (int nCntTexture = 0; nCntTexture < m_nTextureMax; nCntTexture++)
	{
		//�e�N�X�`���̔j��
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
	delete[] m_aFilePas;
}