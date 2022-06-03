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
	string GetPasWord(void) { return m_pasword; }	// ゲッダー

private:
	void SeletMode(void);	// 選択処理
	void Tutorial(void);	// チュートリアル処理
	void ColUpdate(void);	// カラーの更新処理
	void ResultTimer(void);	// リザルトへの遷移
	void PasWord(void);	// パスワード入力処理
	CObject2D *m_click;	// CLICKロゴ
	CObject2D *m_pas_word;// パスワード入力欄
	vector<CObject2D*> m_button;// ボタン
	vector<CObject2D*> m_tutorial;// チュートリアル
	vector<CLetter*> m_pas_font;// パスワード
	vector<wstring> m_key_name;	// パスワード1文字ずつ
	vector<string> m_letter_single;	// パスワード
	string m_pasword;	// パスワード
	int m_count_letter;	// テキストカウント
	int m_result_timer;	// リザルトへのタイマー
	int m_letter_limitl;	// 文字制限
	bool m_fade_flag;	// フェードフラグ
	bool m_tutorial_flag;	// チュートリアルか
	bool m_pas_drop;	// パスワード打ち込み
};

#endif // !_TITLE_H_