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
	int m_nCount;
};
//================================================
//プロトタイプ宣言
//================================================
#endif // !_RESULT_H_