//===========================================
//プレイヤーのモーション処理
//Author:加藤瑞葵
//===========================================
#ifndef _MOTION_PLAYER_H_
#define _MOTION_PLAYER_H_
#include "main.h"
#include "player.h"
#include "motion_road.h"
#include "manager.h"

//================================================
//マクロ定義
//================================================

//================================================
//構造体の定義
//================================================

//================================================
//クラスの定義
//================================================
//クラスの定義
class CMotionPlayer
{
public:

	CMotionPlayer();			//コンストラクタ
	~CMotionPlayer();			//デストラクタ

	//メンバ関数
	HRESULT Init(CPlayer *&pPlayer);
	void Uninit(void);
	void Update(CPlayer *pPlayer);

	static CMotionPlayer *Create(CPlayer *pPlayer);									//生成処理
	void SetMotion(const CMotionRoad::MOTION_PLAYER_TYPE &type, CPlayer *&pPlayer);			//モーション設定処理
	void SetMotion(const int &nType, CPlayer *pPlayer);									//モーション設定処理
	CMotionRoad::MOTION_PLAYER_TYPE GetMotion(void);								//モーション取得処理
	bool GetConnect(void);															//モーションをつなげている最中かどうか取得処理
	int GetKey(void) { return m_nKey; }												//キー取得処理
	int GetFrame(void) { return (int)m_fCounter; }									//モーションカウンター取得処理
	void SetFrameMax(const int &nFrame) { m_fConnectMaxFrame = (float)nFrame; }			//モーションカウンター最大値設定処理
	//キー最大数取得処理
	int GetKeyMax(void) { return CManager::GetInstance()->GetMotionRoad()->GetMotionInfo(CMotionRoad::MOTION_HAVE_TYPE_PLAYER, m_type).nNumKey; }
	//モーションカウンター最大数取得処理
	int GetFrameMax(void) { return CManager::GetInstance()->GetMotionRoad()->GetMotionInfo(CMotionRoad::MOTION_HAVE_TYPE_PLAYER, m_type).aKeyInfo[m_nKey].nFrame; }
	

private:
	std::vector<MOTION_INFO> m_aInfo;					//モーション情報
	CMotionRoad::MOTION_PLAYER_TYPE m_type;				//現在のモーションタイプ
	CMotionRoad::MOTION_PLAYER_TYPE m_typeNext;			//次のモーションタイプ
	CMotionRoad::MOTION_PLAYER_TYPE m_typeOld;			//前のモーションタイプ
	int m_nKey;											//キーNo,(現在のキー)
	float m_fCounter;									//モーションのカウンター
	bool m_bConnect;									//モーションをつなげているかどうか
	float m_fConnectMaxFrame;							//モーションをつなげるときのフレーム数最大値
	std::vector<D3DXVECTOR3> m_posOld;					//モーションをつなげるときの前のモデルの位置
	std::vector<D3DXVECTOR3> m_rotOld;					//モーションをつなげるときの前のモデルの向き
};

//================================================
//プロトタイプ宣言
//================================================
#endif // !_MOTION_PLAYER_H_