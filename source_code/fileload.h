//=============================================================================
//
// �t�@�C�����[�h���� [fileload.h]
// Author : �H������
//
//=============================================================================
#ifndef _FILE_LOAD_H_
#define _FILE_LOAD_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFileLoad
{
public:
	typedef struct 
	{
		pair<vector<string>, vector<string>> file_name_pas;	// �p�X�Ɩ��O�̓ǂݍ���
		map<string, int> type;		// �^���񋓌^
	} FILE_PAS_TYPE_DATA;
	CFileLoad();		//�R���X�g���N�^
	~CFileLoad();		//�f�X�g���N�^
	static vector<string> Load(string load_file);	// �t�@�C�����[�h
	static FILE_PAS_TYPE_DATA CreateFilePasElement(vector<string> all_file_info, string load_file);
private:
};

#endif