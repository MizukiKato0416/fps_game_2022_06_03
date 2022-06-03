//================================================
//影の処理
//Author:加藤瑞葵
//================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "floor.h"

//================================================
//マクロ定義
//================================================
#define SHADOW_ADD_SIZE					(4.0f)		//地面との距離の何分の一増やすか
#define SHADOW_ADD_ALPHA				(200.0f)	//地面との距離の何分の一薄くするか

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CShadow::CShadow(CObject::PRIORITY Priority) :CObject3D(Priority)
{
	m_pObject = nullptr;
	m_bDraw = false;
	m_size = { 0.0f, 0.0f, 0.0f };
}

//================================================
//オーバーロードされたコンストラクタ
//================================================

//================================================
//デストラクタ
//================================================
CShadow::~CShadow()
{

}

//================================================
//初期化処理
//================================================
HRESULT CShadow::Init(void)
{
	//変数初期化
	m_bDraw = true;

	CObject3D::Init();

	//オブジェクトの種類を設定
	SetObjType(CObject::OBJTYPE::SHADOW);
	//ライティングを無効にする
	SetLighting(false);
	//αテストを有効にする
	SetAlphaTest(true);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CShadow::Uninit(void)
{
	CObject3D::Uninit();
}

//================================================
//更新処理
//================================================
void CShadow::Update(void)
{
	//位置とサイズを取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 size = GetSize();

	//カラーを取得
	D3DXCOLOR col = GetCol();

	//対象のオブジェクトの位置を取得
	D3DXVECTOR3 posObj = m_pObject->GetPos();

	//高さの差を求める
	float fDiffer = posObj.y - pos.y;

	//差によって薄さを変える
	col.a = 1.0f - fDiffer / SHADOW_ADD_ALPHA;

	//見えなくなったら
	if (col.a <= 0.0f)
	{
		//描画されている状態なら
		if (m_bDraw == true)
		{
			//描画しないようにする
			m_bDraw = false;
		}
	}
	else
	{
		//描画されていない状態なら
		if (m_bDraw == false)
		{
			//描画する
			m_bDraw = true;
		}
	}

	//差によって大きさを変える
	size.x = m_size.x + fDiffer / SHADOW_ADD_SIZE;
	size.z = m_size.z + fDiffer / SHADOW_ADD_SIZE;

	//位置、サイズ、カラーを設定
	SetPos(D3DXVECTOR3(posObj.x, 0.01f, posObj.z), size);
	SetCol(col);

	//当たり判定
	Collision();
}

//================================================
//描画処理
//================================================
void CShadow::Draw(void)
{
	//描画するなら
	if (m_bDraw == true)
	{
		//デバイスのポインタ
		LPDIRECT3DDEVICE9 pDevice;
		//デバイスの取得
		pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		CObject3D::Draw();

		//減算合成から通常の合成に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//================================================
//生成処理
//================================================
CShadow* CShadow::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, CObject *pObject)
{
	//インスタンスの生成
	CShadow *pShadow = nullptr;
	if (pShadow == nullptr)
	{
		pShadow = new CShadow();
		if (pShadow != nullptr)
		{
			pShadow->m_pos = pos;
			pShadow->m_size = size;
			pShadow->m_pObject = pObject;
			pShadow->Init();
			pShadow->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("effect000.jpg"));
		}
	}
	return pShadow;
}

//================================================
//当たり判定
//================================================
void CShadow::Collision(void)
{
	//床との当たり判定
	if (CFloor::CollisionShadow(this) == false)
	{
		//描画されないようにする
		m_bDraw = false;
	}
}