//=============================================================================
//
// �t�@�C�����[�h���� [fileload.cpp]
// Author : �H������
//
//=============================================================================
#pragma warning( disable : 4592)
#include <stdio.h>
#include "fileload.h"

namespace file = experimental::filesystem;
using file::recursive_directory_iterator;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFileLoad::CFileLoad()
{
	
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFileLoad::~CFileLoad()
{

}

//=============================================================================
// �e�L�X�g�t�@�C���̓ǂݍ���
//=============================================================================
vector<string> CFileLoad::Load(string load_file)
{
	vector<string> all_data;	// �e�L�X�g�t�@�C���̂��ׂẴf�[�^

	// �p�X�̃t�@�C������ǂݍ���
	for (const auto &file : recursive_directory_iterator(load_file))
	{
		// �ǂݍ��񂾃e�L�X�g�t�@�C���̕ۑ�
		all_data.push_back(file.path().string());
	}

	return all_data;
}

//=============================================================================
// �p�X�Ɩ��O�̓ǂݍ���
//=============================================================================
CFileLoad::FILE_PAS_TYPE_DATA CFileLoad::CreateFilePasElement(vector<string> all_file_info, string load_file)
{
	vector<string> folder_name;	// �t�H���_�̕ۑ��o�b�t�@
	CFileLoad::FILE_PAS_TYPE_DATA file_data_buf;	// ���o������
	int file_element;	// �e�L�X�g�t�@�C���̕�����T�C�Y
	file_element = all_file_info.size();

	// �e�L�X�g�t�@�C���̃T�C�Y���̃��[�v
	for (int element_count = 0; element_count < file_element; element_count++)
	{
		// �p�X�̕ۑ�
		file_data_buf.file_name_pas.first.push_back(all_file_info[element_count]);
		file_data_buf.file_name_pas.second.push_back(all_file_info[element_count]);
	}

	// �p�X�̗v�f��
	file_element = file_data_buf.file_name_pas.second.size();

	// �p�X�̗v�f���̃T�C�Y���̃��[�v
	for (int element_count = 0; element_count < file_element; element_count++)
	{
		// �p�X���������Ă���
		if (file_data_buf.file_name_pas.second[element_count].find(load_file) != string::npos)
		{
			// �g���q�����Ă�����
			if (file_data_buf.file_name_pas.second[element_count].find(".") != string::npos)
			{
				// �t�H���_�̖��O�T�C�Y���擾
				int folder_max = folder_name.size();
				for (int folder_count = 0; folder_count < folder_max; folder_count++)
				{
					// ���O��ۑ����鏊�Ƀp�X���������Ă�����
					if (file_data_buf.file_name_pas.second[element_count].find(folder_name[folder_count]) != string::npos)
					{
						// �t�H���_�̖��O�̃T�C�Y���擾
						int name_size = folder_name[folder_count].size();
						for (int count_erase = 0; count_erase < name_size + 1; count_erase++)
						{
							// ���O�������c��
							file_data_buf.file_name_pas.second[element_count].erase(file_data_buf.file_name_pas.second[element_count].begin());
						}
					}
				}
			}
			// �g���q���t���Ă��Ȃ�(�t�H���_�Ȃ̂ŏ���)
			else
			{
				folder_name.push_back(file_data_buf.file_name_pas.second[element_count]);
				file_data_buf.file_name_pas.second.erase(file_data_buf.file_name_pas.second.begin() + element_count);
				file_data_buf.file_name_pas.first.erase(file_data_buf.file_name_pas.first.begin() + element_count);
				file_element--;
				element_count--;
			}
		}
	}
	return file_data_buf;
}
