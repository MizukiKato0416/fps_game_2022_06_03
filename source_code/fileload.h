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
	typedef struct
	{
		string pas;	// テクスチャのパス
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		int coll;	// 当たり判定
	} STAGE_ALLOCATION_DATA;
	typedef struct
	{
		string pas;	// テクスチャのパス
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 size;	// サイズ
		D3DXVECTOR3 stop;	// 停止位置
		int blinking_type;	// 点滅タイプ
		int blinking_speed;	// 点滅スピード
		int move_type;	// 移動タイプ
		float move_speed;	// 移動量
	} TITLE_ALLOCATION_DATA;

	CFileLoad();		//コンストラクタ
	~CFileLoad();		//デストラクタ
	static vector<string> LoadFile(string load_file);	// ファイルロード
	static vector<string> LoadTxt(string load_file);	// テキストロード
	static FILE_PAS_TYPE_DATA CreateFilePasElement(vector<string> all_file_info, string load_file);	// パスと名前の取得
	static vector<STAGE_ALLOCATION_DATA> CreateStageAllocation(vector<string> all_file_info);	// ステージ情報の取得
	static vector<TITLE_ALLOCATION_DATA> CreateTitleAllocation(vector<string> all_file_info);	// タイトル情報の取得
private:
};

#endif