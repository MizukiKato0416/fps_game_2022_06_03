//=============================================================================
//
// �e�N�X�`������ [texture.h]
// Author : �H������
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTexture
{
public:
	CTexture();	// �f�t�H���g�R���X�g���N�^
	~CTexture();// �f�t�H���g�f�X�g���N�^
	void Init(void);	// �e�N�X�`���̐���
	LPDIRECT3DTEXTURE9 GetTexture(int type) { return m_pTexture[type]; }	// �e�N�X�`���̊��蓖��(�e�N�X�`���̎��) { return �I�񂾃e�N�X�`����Ԃ� }
	int GetTexNum(void) { return m_nTextureMax; }
	char *GetPasName(int nCntPas) { return m_aFilePas[nCntPas]; }
	void Uniinit(void);	// �I��
private:
	LPDIRECT3DTEXTURE9	*m_pTexture;	//�e�N�X�`��
	int m_nTextureMax;	// �ǂݍ��ރe�N�X�`���̍ő吔
	char **m_aFilePas;	// �t�@�C���̃p�X
};

#endif