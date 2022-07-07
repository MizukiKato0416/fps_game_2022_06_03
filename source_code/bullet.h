//===========================================
//弾処理
//Author:加藤瑞葵
//===========================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "object.h"

//================================================
//前方宣言
//================================================
class CObject3D;

//================================================
//マクロ定義
//================================================

//========================================================
//構造体の定義
//========================================================

//================================================
//クラスの定義
//================================================
//クラスの定義
class CBullet : public CObject
{
public:
	CBullet(PRIORITY priorty = PRIORITY::NONE);				//コンストラクタ
	~CBullet();				//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBullet *Create(void);		//生成処理

	//当たった敵のプレイヤー番号取得処理
	int GetHitPlayerNum(void) { return m_nPlayer; }
	//ダメージ取得処理
	int GetDamage(void) { return m_nDamage; }

private:
	D3DXVECTOR3 m_bigenPos;						//弾の始点
	D3DXVECTOR3 m_endPos;						//弾の終点
	D3DXVECTOR3 m_rayVec;						//レイの方向
	float m_fDiffer;							//終点までの距離
	int m_nPlayer;								//プレイヤー番号
	int m_nDamage;								//ダメージ
};
//================================================
//プロトタイプ宣言
//================================================
#endif // !_BULLET_H_