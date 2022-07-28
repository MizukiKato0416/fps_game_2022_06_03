//=============================================================================
//
// �t�@�C�����[�h���� [fileload.cpp]
// Author : �H������
//
//=============================================================================
#pragma warning( disable : 4592)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <cstdlib>
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
// �t�@�C���̓ǂݍ���
//=============================================================================
vector<string> CFileLoad::LoadFile(string load_file)
{
	vector<string> all_data;	// �t�@�C���̂��ׂẴf�[�^

	// �p�X�̃t�@�C����ǂݍ���
	for (const auto &file : recursive_directory_iterator(load_file))
	{
		// �ǂݍ��񂾃e�L�X�g�t�@�C���̕ۑ�
		all_data.push_back(file.path().string());
	}

	return all_data;
}

//=============================================================================
// �e�L�X�g�t�@�C���̓ǂݍ���
//=============================================================================
vector<string> CFileLoad::LoadTxt(string load_file)
{
	vector<string> all_data;	// �e�L�X�g�t�@�C���̂��ׂẴf�[�^

	// �e�L�X�g�t�@�C���̓ǂݍ���
	FILE *file;
	file = fopen(load_file.c_str(), "r");

	while (true)
	{
		// ��񂪓����Ă���
		if (file != NULL)
		{
			char buf[1][128];	// ������̃o�b�t�@
			string chek_end;	// �I��肩�̃`�F�b�N

			fscanf(file, "%s", buf[0]);
			chek_end = buf[0];
			all_data.push_back(buf[0]);
			if (chek_end == "END_SCRIPT")
			{
				break;
			}
		}
	}

	// �t�@�C�������
	fclose(file);

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

//=============================================================================
// �X�e�[�W�z�u���
//=============================================================================
vector<CFileLoad::STAGE_ALLOCATION_DATA> CFileLoad::CreateStageAllocation(vector<string> all_file_info)
{
	vector<CFileLoad::STAGE_ALLOCATION_DATA> stage_data;	// �X�e�[�W�̃f�[�^
	int all_element = all_file_info.size();	// �e�L�X�g�t�@�C���̃T�C�Y

	// �e�L�X�g�t�@�C���̃T�C�Y���̃��[�v
	for (int element_count = 0; element_count < all_element; element_count++)
	{
		// SET_MDOEL����������
		if (all_file_info[element_count].find("SET_MODEL") != string::npos)
		{
			CFileLoad::STAGE_ALLOCATION_DATA stage_data_buf; // ���̃o�b�t�@
			int info_count = element_count;	// ���̃J�E���g�A�b�v

			// �������[�v
			while (true)
			{
				// pas����������
				if (all_file_info[info_count].find("pas") != string::npos)
				{
					// �p�X��ۑ�
					stage_data_buf.pas = all_file_info[info_count + 2];
				}
				// pos����������
				if (all_file_info[info_count].find("pos") != string::npos)
				{
					// �������float�ɂ��ĕۑ�
					stage_data_buf.pos.x = atof(all_file_info[info_count + 2].c_str());
					stage_data_buf.pos.y = atof(all_file_info[info_count + 3].c_str());
					stage_data_buf.pos.z = atof(all_file_info[info_count + 4].c_str());
				}
				// rot����������
				if (all_file_info[info_count].find("rot") != string::npos)
				{
					// �������float�ɂ��ĕۑ�
					stage_data_buf.rot.x = atof(all_file_info[info_count + 2].c_str());
					stage_data_buf.rot.y = atof(all_file_info[info_count + 3].c_str());
					stage_data_buf.rot.z = atof(all_file_info[info_count + 4].c_str());
				}
				// coll����������
				if (all_file_info[info_count].find("coll") != string::npos)
				{
					// �������int�ɂ��ĕۑ�
					stage_data_buf.coll = atoi(all_file_info[info_count + 2].c_str());
				}
				// END_SET_MODEL����������
				if (all_file_info[info_count].find("END_SET_MODEL") != string::npos)
				{
					// �o�b�t�@��vector�ɕۑ�
					stage_data.push_back(stage_data_buf);
					element_count = info_count;
					break;
				}
				info_count++;
			}
		}
	}

	return stage_data;
}

//=============================================================================
// �^�C�g���z�u���
//=============================================================================
vector<CFileLoad::TITLE_ALLOCATION_DATA> CFileLoad::CreateTitleAllocation(vector<string> all_file_info)
{
	vector<CFileLoad::TITLE_ALLOCATION_DATA> title_data;	// �^�C�g���̃f�[�^
	int all_element = all_file_info.size();	// �e�L�X�g�t�@�C���̃T�C�Y

	// �e�L�X�g�t�@�C���̃T�C�Y���̃��[�v
	for (int element_count = 0; element_count < all_element; element_count++)
	{
		// SET_UI����������
		if (all_file_info[element_count].find("SET_UI") != string::npos)
		{
			CFileLoad::TITLE_ALLOCATION_DATA title_data_buf; // ���̃o�b�t�@
			int info_count = element_count;	// ���̃J�E���g�A�b�v

			// �������[�v
			while (true)
			{
				// TEX_TYPE����������
				if (all_file_info[info_count].find("TEX_TYPE") != string::npos)
				{
					// �p�X��ۑ�
					title_data_buf.pas = all_file_info[info_count + 2];
				}
				// POS����������
				if (all_file_info[info_count].find("POS") != string::npos)
				{
					// �������float�ɂ��ĕۑ�
					title_data_buf.pos.x = atof(all_file_info[info_count + 2].c_str());
					title_data_buf.pos.y = atof(all_file_info[info_count + 3].c_str());
					title_data_buf.pos.z = atof(all_file_info[info_count + 4].c_str());
				}
				// SIZE����������
				if (all_file_info[info_count].find("SIZE") != string::npos)
				{
					// �������float�ɂ��ĕۑ�
					title_data_buf.size.x = atof(all_file_info[info_count + 2].c_str());
					title_data_buf.size.y = atof(all_file_info[info_count + 3].c_str());
					title_data_buf.size.z = atof(all_file_info[info_count + 4].c_str());
				}
				// BLINKING����������
				if (all_file_info[info_count].find("BLINKING") != string::npos)
				{
					// �������int�ɂ��ĕۑ�
					title_data_buf.blinking_type = atoi(all_file_info[info_count + 2].c_str());
				}
				// BLINKING_SPEED����������
				if (all_file_info[info_count].find("BLINKING_SPEED") != string::npos)
				{
					// �������int�ɂ��ĕۑ�
					title_data_buf.blinking_speed = atoi(all_file_info[info_count + 2].c_str());
				}
				// MOVE����������
				if (all_file_info[info_count].find("MOVE") != string::npos)
				{
					// �������int�ɂ��ĕۑ�
					title_data_buf.move_type = atoi(all_file_info[info_count + 2].c_str());
				}
				// MOVE_SPEED����������
				if (all_file_info[info_count].find("MOVE_SPEED") != string::npos)
				{
					// �������float�ɂ��ĕۑ�
					title_data_buf.move_speed = atof(all_file_info[info_count + 2].c_str());
				}
				// STOP_POINT����������
				if (all_file_info[info_count].find("STOP_POINT") != string::npos)
				{
					// �������float�ɂ��ĕۑ�
					title_data_buf.stop.x = atof(all_file_info[info_count + 2].c_str());
					title_data_buf.stop.y = atof(all_file_info[info_count + 3].c_str());
					title_data_buf.stop.z = atof(all_file_info[info_count + 4].c_str());
				}
				// END_SET_MODEL����������
				if (all_file_info[info_count].find("END_UISET") != string::npos)
				{
					// �o�b�t�@��vector�ɕۑ�
					title_data.push_back(title_data_buf);
					element_count = info_count;
					break;
				}
				info_count++;
			}
		}
	}

	return title_data;
}
