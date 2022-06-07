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
#define GAME01_MAX_ROAD		(2)		//道の最大数
#define GAME01_MAX_BG		(3)		//背景の最大数

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
	
	//終了設定処理
	void SetFinish(bool bFinish) { m_bFinish = bFinish; }
	//終了取得処理
	bool GetFinish(void) { return m_bFinish; }
	//ポーズ設定処理
	void SetPause(bool bPause) { m_bPause = bPause; }
	//ポーズ取得処理
	bool GetPause(void) { return m_bPause; }
	//セリフを出しているかどうか設定処理
	void SetDialog(bool bDialog) { m_bDialog = bDialog; }
	//セリフを出しているかどうか取得処理
	bool GetDialog(void) { return m_bDialog; }

private:
	void Camera(void);											//カメラの処理
	void Road(void);											//道の処理
	void Gauge(void);											//ゲージ処理
	void UninitGauge(void);										//ゲージを消す処理
	void UninitArrow(void);										//矢印を消す処理
	void Shot(void);											//発射処理
	void ShotUi(void);											//発射UI処理
	void Rocket(void);											//ロケットの処理
	void Finish(void);											//ゴール処理
	void FlyingDistanceFlag(void);								//トロフィーの飛距離のフラグ処理
	void Mask(void);											//マスク処理
	void Click(void);											//クリック処理
		
	//メンバ変数
	CPlayer *m_pPlayer;					//プレイヤーのポインタ
	CFloor *m_pFloor;					//床のポインタ
	CModelSingle *m_pStart;				//スタート床のポインタ
	CRoad *m_apRoad[GAME01_MAX_ROAD];	//道のポインタ
	D3DXVECTOR3 m_mouseTriggerPos;		//マウスをクリックした瞬間の位置
	CGauge *m_pGauge;					//発射ゲージ
	CObject2D *m_pGaugeFrame;			//ゲージのフレーム
	int m_nGaugeCounter;				//ゲージが満タンの時間を設定するためのカウンター
	CBg *m_pBg[GAME01_MAX_BG];			//背景のポインタ
	CRocket *m_pRocket;					//ロケットのポインタ
	int m_nShotUiCounter;				//発射した際のUI用カウンター
	CObject2D *m_pShotUi;				//発射した際のUI
	D3DXVECTOR3 m_shotMoveVec;			//発射するときの移動量ベクトル
	bool m_bReleaseMouse;				//マウスを離したかどうか
	CObject2D *m_pArrow;				//矢印UIのポインタ
	bool m_bFinish;						//終了フラグ
	int m_nFinishCounter;				//終了時のカウンター
	bool m_bPause;						//ポーズのフラグ
	CObject2D *m_pDengerMask;			//マスク
	bool m_bAddCol;						//カラー加算の切り替え
	int m_nClick;						//クリックした回数
	bool m_bDialog;						//セリフを出しているかどうか	
	CPause *m_pPause;					//のポインタ
	float m_AnimSpeed;
};	

#endif // !_GAME01_H_