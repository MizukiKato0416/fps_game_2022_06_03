//===========================================
//プレイデータ管理処理
//Author:加藤瑞葵
//===========================================
#ifndef _PLAY_DATA_H_
#define _PLAY_DATA_H_
#include "main.h"

//前方宣言
class CGauge;
class CScore;

//================================================
//マクロ定義
//================================================

//================================================
//クラスの定義
//================================================
//クラスの定義
class CPlayData
{
public:
	CPlayData();			//コンストラクタ
	~CPlayData();			//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);

	void SetScore(const int &nScore) { m_nScore = nScore; }		//スコアの設定
	int GetScore(void) { return m_nScore; }						//スコアの取得
	void SetScorePoint(CScore *&pScore) { m_pScore = pScore; }	//スコアのポインタ設定処理
	CScore *GetScorePoint(void) { return m_pScore; }			//スコアのポインタ取得処理
	void SetTime(const int &nTime) { m_nTime = nTime; }			//時間の設定
	int GetTime(void) { return m_nTime; }						//時間の取得
	vector<bool> GetFlag(void) { return m_trophy_flag; }		// セッター
	void SetFlag(vector<bool> frag) { m_trophy_flag = frag; }	// セッター
	string GetPasword(void) { return m_pasword; }		// ゲッダー
	void SetPasword(string pas) { m_pasword = pas; }	// セッター
	int GetPlayNum(void) { return m_play_num; }			// ゲッダー
	void AddPlayNum(int num) { if (m_play_num >= INT_MAX) m_play_num = INT_MAX; m_play_num += num; }	// 加算

private:
	//メンバ変数
	int m_nScore;				//スコア
	CScore *m_pScore;			//スコアのポインタ
	int m_nTime;				//時間
	vector<bool> m_trophy_flag;	// トロフィーのフラグ
	string m_pasword;	// パスワード
	int m_play_num;		// プレイ回数
};

//================================================
//プロトタイプ宣言
//================================================
#endif // !_PLAY_DATA_H_