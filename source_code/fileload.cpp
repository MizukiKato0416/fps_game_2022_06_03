//=============================================================================
//
// ファイルロード処理 [fileload.cpp]
// Author : 羽鳥太一
//
//=============================================================================
#pragma warning( disable : 4592)
#include <stdio.h>
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
// テクスチャの生成
//=============================================================================
pair<vector<string>, vector<string>> CFileLoad::Load(string load_file)
{
	pair<vector<string>, vector<string>> pas_name_buf;	// パスと名前のバッファ
	vector<string> folder_name;	// フォルダの保存バッファ
	int count_element = 0;			// 要素カウント様

	// パスのファイル名を読み込む
	for (const auto &file : recursive_directory_iterator(load_file))
	{
		// パスの保存
		pas_name_buf.first.push_back(file.path().string());
		pas_name_buf.second.push_back(file.path().string());

		// パスが混ざってたら
		if (pas_name_buf.second[count_element].find(load_file) != string::npos)
		{
			// 拡張子がついていたら
			if (pas_name_buf.second[count_element].find(".") != string::npos)
			{
				// フォルダの名前サイズを取得
				int folder_max = folder_name.size();
				for (int folder_count = 0; folder_count < folder_max; folder_count++)
				{
					// 名前を保存する所にパスが混ざっていたら
					if (pas_name_buf.second[count_element].find(folder_name[folder_count]) != string::npos)
					{
						// フォルダの名前のサイズを取得
						int name_size = folder_name[folder_count].size();
						for (int count_erase = 0; count_erase < name_size + 1; count_erase++)
						{
							// 名前だけを残す
							pas_name_buf.second[count_element].erase(pas_name_buf.second[count_element].begin());
						}
					}
				}
				count_element++;
			}
			// 拡張子が付いていない(フォルダなので消去)
			else
			{
				folder_name.push_back(pas_name_buf.second[count_element]);
				pas_name_buf.second.erase(pas_name_buf.second.begin() + count_element);
				pas_name_buf.first.erase(pas_name_buf.first.begin() + count_element);
			}
		}
	}

	return pas_name_buf;
}