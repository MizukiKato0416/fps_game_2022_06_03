//===========================================
//ゲーム処理
//Author:加藤瑞葵
//===========================================
#ifndef _GAME01_H_
#define _GAME01_H_
#include "object.h"
#include "main.h"
#include "option.h"

//前方宣言
class CPlayer;
class CMeshField;
class CEnemy;
class CObject2D;
class CScoreUi;

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
	~CGame01();															//デストラクタ

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
	//プレイヤーリスポーン関数
	void RespawnPlayer(void);			
	//設定画面取得処理
	COption *GetOption(void) { return m_pOption; }

private:
	void LoadModelTxt(const string &Pas);			//モデルロード処理
	void ScoreUi(void);								//スコアUIの処理
	void LastSpurt(void);							//ラストスパート処理
	void Option(void);								//設定画面処理
	void GameOver(void);							//ゲーム終了処理
	void Start(void);								//スタートまでの処理
		
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
	CScoreUi *m_pScorUiTop;				//上のスコアUI
	CScoreUi *m_pScorUiUnder;			//下のスコアUI
	COption *m_pOption;					//設定画面
	bool m_bLastSpurt;					//ラストスパートするかどうか
	bool m_bGameOver;					//ゲームが終了したかどうか
	int m_nResultCounter;				//リザルトに行くまでのカウンター
	bool m_bStart;						//スタートするかどうか
	int m_nStartCounter;				//スタートするまでのカウンター
	int m_nCountDownOld;				//1フレーム前のスタートまでのカウントダウン
	CObject2D *m_pCountDownUi;			//カウントダウンUIのポインタ
};	

#endif // !_GAME01_H_