//===========================================
//弾道処理
//Author:加藤瑞葵
//===========================================
#ifndef _BALLISTIC_H_
#define _BALLISTIC_H_

#include "object.h"

//================================================
//前方宣言
//================================================
class CObject3D;

//================================================
//マクロ定義
//================================================
#define BALLISTIC_MAX_ORBIT		(2)		//弾の軌道エフェクトの数

//========================================================
//構造体の定義
//========================================================

//================================================
//クラスの定義
//================================================
//クラスの定義
class CBallistic : public CObject
{
public:
	CBallistic(PRIORITY priorty = PRIORITY::BULLET);				//コンストラクタ
	~CBallistic();				//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成処理
	static CBallistic *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 rot, const D3DXVECTOR3 endPos,
		                   const float fSpeed, const string sTexPas1, const string sTexPas2);

private:
	D3DXVECTOR3 m_bigenPos;						//弾の始点
	D3DXVECTOR3 m_endPos;						//弾の終点
	D3DXVECTOR3 m_size;							//弾のサイズ
	D3DXVECTOR3 m_rot;							//弾の向き
	float m_fSpeed;								//弾の速さ
	string m_sTexPas1;							//弾のテクスチャ
	string m_sTexPas2;							//弾のテクスチャ
	CObject3D *m_apOrbit[BALLISTIC_MAX_ORBIT];	//軌道エフェクトのポインタ
	bool m_bMove;								//移動し始めるかどうか
};

//================================================
//プロトタイプ宣言
//================================================
#endif // !_BALLISTIC_H_