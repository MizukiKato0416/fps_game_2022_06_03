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
class CMeshField;
class CEnemy;

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
	bool MapLimit(CObject* pObj);		//マップの移動制限
	void FirstContact(void);
	static void ConnectLading(bool *bConnect);
	//敵のモデル取得処理
	vector<CEnemy*> GetEnemy(void);

private:
	void LoadModelTxt(const string &Pas);			//モデルロード処理
		
	//メンバ変数
	CPlayer *m_pPlayer;					//プレイヤーのポインタ
	CMeshField *m_pMeshField;			//メッシュフィールドのポインタ
	vector<CEnemy*> m_pEnemy;					//敵のモデルのポインタ
	bool m_bAllConnect;
};	

#endif // !_GAME01_H_