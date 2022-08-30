//===========================================
//リザルト処理
//Author:加藤瑞葵
//===========================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "object.h"

//================================================
//前方宣言
//================================================
class CObject2D;
class CCounter;
class CLetter;

//================================================
//クラスの定義
//================================================
//クラスの定義
class CResult : public CObject
{
public:
	CResult(PRIORITY priorty = PRIORITY::NONE);				//コンストラクタ
	~CResult();				//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	vector<CObject2D*> m_Ui;
	vector<CCounter*> m_Countor;
	vector<CLetter*> m_name_font;// 名前フォント
	vector<wstring> m_key_name;	// 名前1文字ずつ
	vector<string> m_letter_single;	// 名前
	int m_count_letter;	// テキストカウント
	int m_letter_limitl;	// 文字制限
	int m_nCount;
};
//================================================
//プロトタイプ宣言
//================================================
#endif // !_RESULT_H_