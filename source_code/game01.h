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
class CObject2D;

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
	//プレイヤーリスポーン位置の種類
	enum class PlayerRespawnPos
	{
		NONE = 0,
		POS_00,
		POS_01,
		POS_02,
		POS_03,
		POS_04,
		POS_05,
		POS_06,
		POS_07,
		MAX
	};


	CGame01(CObject::PRIORITY Priority = CObject::PRIORITY::NONE);		//コンストラクタ
	~CGame01();									//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool MapLimit(CObject* pObj);		//マップの移動制限
	bool GetAllConnect(void) { return m_bAllConnect; }
	void FirstContact(void);
	//敵のモデル取得処理
	vector<CEnemy*> GetEnemy(void);
	void RespawnPlayer(void);			//プレイヤーリスポーン関数

private:
	void LoadModelTxt(const string &Pas);			//モデルロード処理
		
	//メンバ変数
	CPlayer *m_pPlayer;					//プレイヤーのポインタ
	CObject2D *m_now_loding;
	CMeshField *m_pMeshField;			//メッシュフィールドのポインタ
	vector<CEnemy*> m_pEnemy;					//敵のモデルのポインタ
	int m_count_pattern = 0;
	int m_pattern_tex = 0;
	bool m_bAllConnect;
	PlayerRespawnPos m_respawnPos;		//プレイヤーをリスポーンさせる位置
	CObject2D *m_pMapBg;				//マップ背景
};	

#endif // !_GAME01_H_