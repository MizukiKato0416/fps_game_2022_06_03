//=============================================================================
//xファイルロード処理
//Author:加藤瑞葵
//=============================================================================
#ifndef _X_LOAD_H_
#define _X_LOAD_H_

//=============================================================================
// ヘッダファイルのインクルード
//=============================================================================
#include "main.h"
#include "fileload.h"

//=============================================================================
// クラス定義
//=============================================================================
class CXload
{
public:
	CXload();									//コンストラクタ
	~CXload();									//デストラクタ

	void Init(void);
	void Uninit(void);											// 終了
	LPD3DXMESH GetMesh(const string &sPas);					// メッシュの割り当て
	LPD3DXBUFFER GetBuffMat(const string &sPas);				// マテリアルの割り当て
	DWORD GetNumMat(const string &sPas);						// マテリアル数の割り当て
	string GetType(const int &nType) { return m_file_data.file_name_pas.second[nType]; }	// タイプ取得処理
	int GetNum(const string &sPas) { return m_file_data.type[sPas]; }						//何番目のモデルか取得処理
	

private:
	vector<LPD3DXMESH>  m_apMesh;			//メッシュ(頂点情報)へのポインタ
	vector<LPD3DXBUFFER> m_apBuffMat;		//マテリアル(材質情報)へのポインタ
	vector<DWORD> m_aNumMat;				//マテリアル数
	CFileLoad::FILE_LOAD_DATA m_file_data;
	int m_nNum;								//総数
};

#endif