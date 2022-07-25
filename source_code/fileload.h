//=============================================================================
//
// ファイルロード処理 [fileload.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _FILE_LOAD_H_
#define _FILE_LOAD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFileLoad
{
public:
	typedef struct 
	{
		pair<vector<string>, vector<string>> file_name_pas;	// パスと名前の読み込み
		map<string, int> type;		// 疑似列挙型
	} FILE_PAS_TYPE_DATA;
	CFileLoad();		//コンストラクタ
	~CFileLoad();		//デストラクタ
	static vector<string> Load(string load_file);	// ファイルロード
	static FILE_PAS_TYPE_DATA CreateFilePasElement(vector<string> all_file_info, string load_file);
private:
};

#endif