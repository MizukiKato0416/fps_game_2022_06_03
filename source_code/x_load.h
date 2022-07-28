//=============================================================================
//x�t�@�C�����[�h����
//Author:��������
//=============================================================================
#ifndef _X_LOAD_H_
#define _X_LOAD_H_

//=============================================================================
// �w�b�_�t�@�C���̃C���N���[�h
//=============================================================================
#include "main.h"
#include "fileload.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CXload
{
public:
	CXload();									//�R���X�g���N�^
	~CXload();									//�f�X�g���N�^

	void Init(void);
	void Uninit(void);											// �I��
	LPD3DXMESH GetMesh(const string &sPas);					// ���b�V���̊��蓖��
	LPD3DXBUFFER GetBuffMat(const string &sPas);				// �}�e���A���̊��蓖��
	DWORD GetNumMat(const string &sPas);						// �}�e���A�����̊��蓖��
	string GetType(const int &nType) { return m_file_data.file_name_pas.second[nType]; }	// �^�C�v�擾����
	int GetNum(const string &sPas) { return m_file_data.type[sPas]; }						//���Ԗڂ̃��f�����擾����
	

private:
	vector<string> m_all_file;	//�e�L�X�g�t�@�C���̑S�Ă̕�����
	vector<LPD3DXMESH>  m_apMesh;			//���b�V��(���_���)�ւ̃|�C���^
	vector<LPD3DXBUFFER> m_apBuffMat;		//�}�e���A��(�ގ����)�ւ̃|�C���^
	vector<DWORD> m_aNumMat;				//�}�e���A����
	CFileLoad::FILE_PAS_TYPE_DATA m_file_data;	//�ǂݍ��݂ɕK�v�ȏ��
	int m_nNum;								//����
};

#endif