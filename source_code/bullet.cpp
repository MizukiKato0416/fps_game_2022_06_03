//================================================
//弾処理
//Author: 加藤瑞葵
//================================================
#include "bullet.h"
#include "manager.h"
#include "fade.h"
#include "model.h"
#include "model_single.h"
#include "camera.h"
#include "PresetSetEffect.h"
#include "Object3D.h"
#include "renderer.h"

//================================================
//マクロ定義
//================================================

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CBullet::CBullet(CObject::PRIORITY Priority) :CObject(Priority)
{
	
}

//================================================
//オーバーロードされたコンストラクタ
//================================================


//================================================
//デストラクタ
//================================================
CBullet::~CBullet()
{

}

//================================================
//初期化処理
//================================================
HRESULT CBullet::Init(void)
{
	// 頂点宣言
	const D3DVERTEXELEMENT9 vertexDecl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		D3DDECL_END()
	};

	//カメラの位置保存用
	D3DXVECTOR3 posCameraV = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 posCameraR = { 0.0f, 0.0f, 0.0f };
	//レイを飛ばす方向
	D3DXVECTOR3 rayVec = { 0.0f, 0.0f, 0.0f };
	//カメラの向き保存用
	D3DXVECTOR3 rotCamera = { 0.0f, 0.0f, 0.0f };
	//レイの当たり判定結果保存用
	BOOL bHit = false;
	//当たった場所までの距離保存用
	float fDiffer = 0.0f;

	//cameraのポインタ配列1番目のアドレス取得
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//cameraの取得
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			//カメラの位置取得
			posCameraV = pCamera->GetPosV();
			posCameraR = pCamera->GetPosR();

			//レイを飛ばす方向を算出
			rayVec = posCameraR - posCameraV;
			//ベクトルを正規化
			D3DXVec3Normalize(&rayVec, &rayVec);
		}
	}

	//オブジェクト情報を入れるポインタ
	vector<CObject*> object;

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MODEL));
	int nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//オブジェクトの種類がモデルだったら
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::MODEL)
		{
			//modelSingleにキャスト
			CModelSingle *pModel = (CModelSingle*)object[nCnt];

			D3DXMATRIX modelInvMtx;
			D3DXMatrixIdentity(&modelInvMtx);
			D3DXMatrixInverse(&modelInvMtx, NULL, &pModel->GetModel()->GetMtx());

			D3DXVECTOR3 posV = posCameraV;

			D3DXVECTOR3 endPos = posV + rayVec;
			D3DXVec3TransformCoord(&posV, &posV, &modelInvMtx);
			D3DXVec3TransformCoord(&endPos, &endPos, &modelInvMtx);

			D3DXVECTOR3 vec = endPos - posV;

			//レイとメッシュの当たり判定
			if (D3DXIntersect(pModel->GetModel()->GetMesh(), &posV, &vec, &bHit, NULL, NULL, NULL, &fDiffer, NULL, NULL) == D3D_OK)
			{
				//当たったとき
				if (bHit)
				{
					D3DXVec3Normalize(&vec, &vec);
					//レイのベクトルを算出した距離の分伸ばす
					vec *= fDiffer;

					//カメラの位置から伸ばしたベクトルを足して当たった位置を算出
					D3DXVECTOR3 HitPos = posV + vec;
					D3DXVec3TransformCoord(&HitPos, &HitPos, &pModel->GetModel()->GetMtx());

					//当たった位置にエフェクトを出す
					CPresetEffect::SetEffect3D(2, HitPos, {});
					CPresetEffect::SetEffect3D(3, HitPos, {});
				}
			}
		}
	}

	return S_OK;
}

//================================================
//終了処理
//================================================
void CBullet::Uninit(void)
{	
	//オブジェクトの破棄
	Release();
}

//================================================
//更新処理
//================================================
void CBullet::Update(void)
{
	
}

//================================================
//描画処理
//================================================
void CBullet::Draw(void)
{
	
}

//================================================
//生成処理
//================================================
CBullet *CBullet::Create(void)
{
	//インスタンスの生成
	CBullet *pBullet = nullptr;
	if (pBullet == nullptr)
	{
		pBullet = new CBullet;
		if (pBullet != nullptr)
		{
			pBullet->Init();
		}
	}
	return pBullet;
}