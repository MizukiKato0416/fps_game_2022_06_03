//=============================================================================
//
// 設定画面の処理 [option.h]
// Author : 加藤瑞葵
//
//=============================================================================
#ifndef _OPTION_H_
#define _OPTION_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;
class COptionBar;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OPTION_BAR_NUM		(2)		//バーの数

//*****************************************************************************
// 構造体の定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class COption : public CObject
{
public:
	COption(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// デフォルトコンストラクタ
	~COption();						// デフォルトデストラクタ
	HRESULT Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画
	static COption *Create(void);	// 生成
	//設定が開かれているかどうか取得処理
	bool GetOpen(void) { return m_bOpen; }
	//設定が開かれているかどうか設定処理
	void SetOpen(const bool &bOpen) { m_bOpen = bOpen; }
	void Open(void);			//設定を開いた時の処理
	void Close(void);			//設定を閉じた時の処理

private:
	
	void MouseSensi(void);		//マウス感度設定処理
	void ViewAngle(void);		//視野角設定処理

	COptionBar *m_apOptionBar[OPTION_BAR_NUM];		//バーのポインタ
	CObject2D *m_pOptionFrame;						//設定画面のフレーム
	float m_aNum[OPTION_BAR_NUM];					//バーの値保存用
	bool m_bOpen;									//設定が開かれているかどうか
};

#endif