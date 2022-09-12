//=============================================================================
//
// タイトル処理
// Author : 加藤瑞葵
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "object.h"
#include "input_keyboard.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CUi;
class CObject2D;
class CLetter;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitle : public CObject
{
public:
	CTitle(PRIORITY priorty = PRIORITY::NONE);	// デフォルトコンストラクタ
	~CTitle();	// デフォルトデストラクタ
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画
	void PasWord(void);

private:
	void Logo(void);	//ロゴの処理

	vector<CUi*> m_ui;	// UI
	vector<string> m_type;	// テクスチャ名
	vector<CLetter*> m_name_font;// 名前フォント
	vector<wstring> m_key_name;	// 名前1文字ずつ
	vector<string> m_letter_single;	// 名前
	CObject2D *m_pointor;	// マウスポインター
	CObject2D *m_name_box;	// パスワード入力欄
	string m_name;	// 名前
	int m_count_letter;	// テキストカウント
	int m_letter_limitl;	// 文字制限
	bool m_name_drop;	// 名前打ち込み
	int m_nCounter;			//経過フレームカウント用
	CObject2D *m_pTitleLogo;	//タイトルロゴのポインタ
};

#endif // !_TITLE_H_