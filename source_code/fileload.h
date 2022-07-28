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
	typedef struct
	{
		string pas;	// �e�N�X�`���̃p�X
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 rot;	// ����
		int coll;	// �����蔻��
	} STAGE_ALLOCATION_DATA;
	typedef struct
	{
		string pas;	// �e�N�X�`���̃p�X
		D3DXVECTOR3 pos;	// �ʒu
		D3DXVECTOR3 size;	// �T�C�Y
		D3DXVECTOR3 stop;	// ��~�ʒu
		int blinking_type;	// �_�Ń^�C�v
		int blinking_speed;	// �_�ŃX�s�[�h
		int move_type;	// �ړ��^�C�v
		float move_speed;	// �ړ���
	} TITLE_ALLOCATION_DATA;

	CFileLoad();		//�R���X�g���N�^
	~CFileLoad();		//�f�X�g���N�^
	static vector<string> LoadFile(string load_file);	// �t�@�C�����[�h
	static vector<string> LoadTxt(string load_file);	// �e�L�X�g���[�h
	static FILE_PAS_TYPE_DATA CreateFilePasElement(vector<string> all_file_info, string load_file);	// �p�X�Ɩ��O�̎擾
	static vector<STAGE_ALLOCATION_DATA> CreateStageAllocation(vector<string> all_file_info);	// �X�e�[�W���̎擾
	static vector<TITLE_ALLOCATION_DATA> CreateTitleAllocation(vector<string> all_file_info);	// �^�C�g�����̎擾
private:
};

#endif