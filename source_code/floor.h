//===========================================
//3Dポリゴンの床処理
//Author:加藤瑞葵
//===========================================
#ifndef _FLOOR_H_
#define _FLOOR_H_
#include "object3D.h"

//================================================
//マクロ定義
//================================================
#define FLOOR_SIZE		(D3DXVECTOR3(2000.0f, 0.0f, 100.0f))	//床のサイズ

//================================================
//クラスの定義
//================================================
//クラスの定義
class CFloor : public CObject3D
{
public:
	CFloor(CObject::PRIORITY Priority = CObject::PRIORITY::POLYGON_3D);	//コンストラクタ
	~CFloor();										//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CFloor *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXVECTOR3 &rot);
	static bool Collision(CObject *pObject, const float &fRadius);			//当たり判定
	static bool CollisionShadow(CObject *pObject);		//影との当たり判定

private:
};

#endif // !_FLOOR_H_