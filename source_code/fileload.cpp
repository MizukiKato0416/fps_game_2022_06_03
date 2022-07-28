//=============================================================================
//
// ファイルロード処理 [fileload.cpp]
// Author : 羽鳥太一
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
// コンストラクタ
//=============================================================================
CFileLoad::CFileLoad()
{
	
}

//=============================================================================
// デストラクタ
//=============================================================================
CFileLoad::~CFileLoad()
{

}

//=============================================================================
// ファイルの読み込み
//=============================================================================
vector<string> CFileLoad::LoadFile(string load_file)
{
	vector<string> all_data;	// ファイルのすべてのデータ

	// パスのファイルを読み込む
	for (const auto &file : recursive_directory_iterator(load_file))
	{
		// 読み込んだテキストファイルの保存
		all_data.push_back(file.path().string());
	}

	return all_data;
}

//=============================================================================
// テキストファイルの読み込み
//=============================================================================
vector<string> CFileLoad::LoadTxt(string load_file)
{
	vector<string> all_data;	// テキストファイルのすべてのデータ

	// テキストファイルの読み込み
	FILE *file;
	file = fopen(load_file.c_str(), "r");

	while (true)
	{
		// 情報が入ってたら
		if (file != NULL)
		{
			char buf[1][128];	// 文字列のバッファ
			string chek_end;	// 終わりかのチェック

			fscanf(file, "%s", buf[0]);
			chek_end = buf[0];
			all_data.push_back(buf[0]);
			if (chek_end == "END_SCRIPT")
			{
				break;
			}
		}
	}

	// ファイルを閉じる
	fclose(file);

	return all_data;
}

//=============================================================================
// パスと名前の読み込み
//=============================================================================
CFileLoad::FILE_PAS_TYPE_DATA CFileLoad::CreateFilePasElement(vector<string> all_file_info, string load_file)
{
	vector<string> folder_name;	// フォルダの保存バッファ
	CFileLoad::FILE_PAS_TYPE_DATA file_data_buf;	// 抽出する情報
	int file_element;	// テキストファイルの文字列サイズ
	file_element = all_file_info.size();

	// テキストファイルのサイズ分のループ
	for (int element_count = 0; element_count < file_element; element_count++)
	{
		// パスの保存
		file_data_buf.file_name_pas.first.push_back(all_file_info[element_count]);
		file_data_buf.file_name_pas.second.push_back(all_file_info[element_count]);
	}

	// パスの要素数
	file_element = file_data_buf.file_name_pas.second.size();

	// パスの要素数のサイズ分のループ
	for (int element_count = 0; element_count < file_element; element_count++)
	{
		// パスが混ざってたら
		if (file_data_buf.file_name_pas.second[element_count].find(load_file) != string::npos)
		{
			// 拡張子がついていたら
			if (file_data_buf.file_name_pas.second[element_count].find(".") != string::npos)
			{
				// フォルダの名前サイズを取得
				int folder_max = folder_name.size();
				for (int folder_count = 0; folder_count < folder_max; folder_count++)
				{
					// 名前を保存する所にパスが混ざっていたら
					if (file_data_buf.file_name_pas.second[element_count].find(folder_name[folder_count]) != string::npos)
					{
						// フォルダの名前のサイズを取得
						int name_size = folder_name[folder_count].size();
						for (int count_erase = 0; count_erase < name_size + 1; count_erase++)
						{
							// 名前だけを残す
							file_data_buf.file_name_pas.second[element_count].erase(file_data_buf.file_name_pas.second[element_count].begin());
						}
					}
				}
			}
			// 拡張子が付いていない(フォルダなので消去)
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
// ステージ配置情報
//=============================================================================
vector<CFileLoad::STAGE_ALLOCATION_DATA> CFileLoad::CreateStageAllocation(vector<string> all_file_info)
{
	vector<CFileLoad::STAGE_ALLOCATION_DATA> stage_data;	// ステージのデータ
	int all_element = all_file_info.size();	// テキストファイルのサイズ

	// テキストファイルのサイズ分のループ
	for (int element_count = 0; element_count < all_element; element_count++)
	{
		// SET_MDOELがあったら
		if (all_file_info[element_count].find("SET_MODEL") != string::npos)
		{
			CFileLoad::STAGE_ALLOCATION_DATA stage_data_buf; // 情報のバッファ
			int info_count = element_count;	// 情報のカウントアップ

			// 無限ループ
			while (true)
			{
				// pasがあったら
				if (all_file_info[info_count].find("pas") != string::npos)
				{
					// パスを保存
					stage_data_buf.pas = all_file_info[info_count + 2];
				}
				// posがあったら
				if (all_file_info[info_count].find("pos") != string::npos)
				{
					// 文字列をfloatにして保存
					stage_data_buf.pos.x = atof(all_file_info[info_count + 2].c_str());
					stage_data_buf.pos.y = atof(all_file_info[info_count + 3].c_str());
					stage_data_buf.pos.z = atof(all_file_info[info_count + 4].c_str());
				}
				// rotがあったら
				if (all_file_info[info_count].find("rot") != string::npos)
				{
					// 文字列をfloatにして保存
					stage_data_buf.rot.x = atof(all_file_info[info_count + 2].c_str());
					stage_data_buf.rot.y = atof(all_file_info[info_count + 3].c_str());
					stage_data_buf.rot.z = atof(all_file_info[info_count + 4].c_str());
				}
				// collがあったら
				if (all_file_info[info_count].find("coll") != string::npos)
				{
					// 文字列をintにして保存
					stage_data_buf.coll = atoi(all_file_info[info_count + 2].c_str());
				}
				// END_SET_MODELがあったら
				if (all_file_info[info_count].find("END_SET_MODEL") != string::npos)
				{
					// バッファをvectorに保存
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
// タイトル配置情報
//=============================================================================
vector<CFileLoad::TITLE_ALLOCATION_DATA> CFileLoad::CreateTitleAllocation(vector<string> all_file_info)
{
	vector<CFileLoad::TITLE_ALLOCATION_DATA> title_data;	// タイトルのデータ
	int all_element = all_file_info.size();	// テキストファイルのサイズ

	// テキストファイルのサイズ分のループ
	for (int element_count = 0; element_count < all_element; element_count++)
	{
		// SET_UIがあったら
		if (all_file_info[element_count].find("SET_UI") != string::npos)
		{
			CFileLoad::TITLE_ALLOCATION_DATA title_data_buf; // 情報のバッファ
			int info_count = element_count;	// 情報のカウントアップ

			// 無限ループ
			while (true)
			{
				// TEX_TYPEがあったら
				if (all_file_info[info_count].find("TEX_TYPE") != string::npos)
				{
					// パスを保存
					title_data_buf.pas = all_file_info[info_count + 2];
				}
				// POSがあったら
				if (all_file_info[info_count].find("POS") != string::npos)
				{
					// 文字列をfloatにして保存
					title_data_buf.pos.x = atof(all_file_info[info_count + 2].c_str());
					title_data_buf.pos.y = atof(all_file_info[info_count + 3].c_str());
					title_data_buf.pos.z = atof(all_file_info[info_count + 4].c_str());
				}
				// SIZEがあったら
				if (all_file_info[info_count].find("SIZE") != string::npos)
				{
					// 文字列をfloatにして保存
					title_data_buf.size.x = atof(all_file_info[info_count + 2].c_str());
					title_data_buf.size.y = atof(all_file_info[info_count + 3].c_str());
					title_data_buf.size.z = atof(all_file_info[info_count + 4].c_str());
				}
				// BLINKINGがあったら
				if (all_file_info[info_count].find("BLINKING") != string::npos)
				{
					// 文字列をintにして保存
					title_data_buf.blinking_type = atoi(all_file_info[info_count + 2].c_str());
				}
				// BLINKING_SPEEDがあったら
				if (all_file_info[info_count].find("BLINKING_SPEED") != string::npos)
				{
					// 文字列をintにして保存
					title_data_buf.blinking_speed = atoi(all_file_info[info_count + 2].c_str());
				}
				// MOVEがあったら
				if (all_file_info[info_count].find("MOVE") != string::npos)
				{
					// 文字列をintにして保存
					title_data_buf.move_type = atoi(all_file_info[info_count + 2].c_str());
				}
				// MOVE_SPEEDがあったら
				if (all_file_info[info_count].find("MOVE_SPEED") != string::npos)
				{
					// 文字列をfloatにして保存
					title_data_buf.move_speed = atof(all_file_info[info_count + 2].c_str());
				}
				// STOP_POINTがあったら
				if (all_file_info[info_count].find("STOP_POINT") != string::npos)
				{
					// 文字列をfloatにして保存
					title_data_buf.stop.x = atof(all_file_info[info_count + 2].c_str());
					title_data_buf.stop.y = atof(all_file_info[info_count + 3].c_str());
					title_data_buf.stop.z = atof(all_file_info[info_count + 4].c_str());
				}
				// END_SET_MODELがあったら
				if (all_file_info[info_count].find("END_UISET") != string::npos)
				{
					// バッファをvectorに保存
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
