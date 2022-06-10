//===========================================
//ゲーム処理
//Author:加藤瑞葵
//===========================================
#ifndef _GAME01_H_
#define _GAME01_H_
#include "object.h"
#include "main.h"

//前方宣言
class CPlayer;
class CFloor;
class CRoad;
class CGauge;
class CBg;
class CObject2D;
class CRocket;
class CModelSingle;
class CLetter;
class CNextDialogUI;
class CDialog;
class CPause;

//================================================
//マクロ定義
//================================================

//========================================================
//構造体の定義
//========================================================

//================================================
//クラスの定義
//================================================
class CGame01 : public CObject
{
public:
	CGame01(CObject::PRIORITY Priority = CObject::PRIORITY::NONE);		//コンストラクタ
	~CGame01();									//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void LoadModelTxt(const string &Pas);			//モデルロード処理
		
	//メンバ変数
	CPlayer *m_pPlayer;					//プレイヤーのポインタ
};	

#endif // !_GAME01_H_