//===========================================
//パーティクル処理
//Author:加藤瑞葵
//===========================================
#ifndef _EFFECT2D_H_
#define _EFFECT2D_H_
#include "object2D.h"

//前方宣言

//================================================
//マクロ定義
//================================================

//================================================
//クラスの定義
//================================================
//クラスの定義
class CEffect2D : public CObject2D
{
public:
	//エフェクトの種類
	enum class EFFECT2D_TYPE
	{
		NONE = 0,
		MAX
	};

	CEffect2D(CObject::PRIORITY Priority = CObject::PRIORITY::EFFECT2D);		//コンストラクタ
	~CEffect2D();										//デストラクタ

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect2D *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXCOLOR &col, const EFFECT2D_TYPE &type);

	//減算合成設定処理
	void SetAlphaBlendSubtraction(const bool &bAlphaBlendSubtraction) { m_bAlphaBlendSubtraction = bAlphaBlendSubtraction; }

private:
	EFFECT2D_TYPE m_type;			//エフェクトの種類
	bool m_bAlphaBlendSubtraction;	//減算合成するかどうか
};

#endif // !_EFFECT3D_H_