//================================================
//弾処理
//Author: 加藤瑞葵
//================================================
#include "ballistic.h"
#include "manager.h"
#include "Object3D.h"

//================================================
//マクロ定義
//================================================

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CBallistic::CBallistic(CObject::PRIORITY Priority) :CObject(Priority)
{
	m_bigenPos = { 0.0f, 0.0f, 0.0f };
	m_endPos = { 0.0f, 0.0f, 0.0f };
	m_size = { 0.0f, 0.0f, 0.0f };
	m_rot = {0.0f, 0.0f, 0.0f};
	m_fSpeed = 0.0f;
	m_sTexPas1.clear();
	m_sTexPas2.clear();
	memset(m_apOrbit, NULL, sizeof(m_apOrbit[BALLISTIC_MAX_ORBIT]));
}

//================================================
//オーバーロードされたコンストラクタ
//================================================


//================================================
//デストラクタ
//================================================
CBallistic::~CBallistic()
{

}

//================================================
//初期化処理
//================================================
HRESULT CBallistic::Init(void)
{
	//オブジェクトの種類を設定
	SetObjType(CObject::OBJTYPE::BALLISTIC);

	//弾の軌道エフェクトを生成
	m_apOrbit[0] = CObject3D::Create(m_bigenPos, { m_size.x, m_size.y, 0.0f }, { 0.0f, m_rot.y + D3DX_PI / 2.0f, m_rot.x + D3DX_PI / 2.0f });
	m_apOrbit[0]->BindTexture(CManager::GetTexture()->GetTexture(m_sTexPas1));
	m_apOrbit[0]->SetOriginType(CObject3D::ORIGIN_TYPE::LEFT);
	m_apOrbit[0]->SetPos(m_bigenPos, { m_size.x, m_size.y, 0.0f });

	m_apOrbit[1] = CObject3D::Create(m_bigenPos, { m_size.y, m_size.x, 0.0f }, { m_rot.x, m_rot.y, 0.0f });
	m_apOrbit[1]->BindTexture(CManager::GetTexture()->GetTexture(m_sTexPas2));
	m_apOrbit[1]->SetOriginType(CObject3D::ORIGIN_TYPE::LOWER);
	m_apOrbit[1]->SetPos(m_bigenPos, { m_size.y, m_size.x, 0.0f });

	//カリングをオフにする
	m_apOrbit[0]->SetCulling(false);
	m_apOrbit[1]->SetCulling(false);
	//ライティングオフにする
	m_apOrbit[0]->SetLighting(false);
	m_apOrbit[1]->SetLighting(false);
	//ライティングオフにする
	m_apOrbit[0]->SetAlphaTest(true);
	m_apOrbit[1]->SetAlphaTest(true);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CBallistic::Uninit(void)
{	
	//オブジェクトの破棄
	Release();
}

//================================================
//更新処理
//================================================
void CBallistic::Update(void)
{
	for (int nCntOrbit = 0; nCntOrbit < BALLISTIC_MAX_ORBIT; nCntOrbit++)
	{
		if (m_apOrbit[nCntOrbit] != nullptr)
		{
			//位置とサイズを取得
			D3DXVECTOR3 pos = m_apOrbit[nCntOrbit]->GetPos();

			//移動した位置から終点までのベクトルを算出
			D3DXVECTOR3 differVec = m_endPos - pos;
			float fDiffer = D3DXVec3Length(&differVec);

			if (fDiffer < m_size.x)
			{
				//消す
				m_apOrbit[nCntOrbit]->Uninit();
				m_apOrbit[nCntOrbit] = nullptr;
				break;
			}

			D3DXVECTOR3 size = m_apOrbit[nCntOrbit]->GetSize();

			//飛ばす方向を設定
			D3DXVECTOR3 vec = m_endPos - m_bigenPos;
			//正規化
			D3DXVec3Normalize(&vec, &vec);
			//移動量を設定
			D3DXVECTOR3 move = vec * m_fSpeed;
			pos += move;

			//移動した位置から終点までのベクトルを算出
			differVec = m_endPos - pos;
			fDiffer = D3DXVec3Length(&differVec);

			if (fDiffer < size.x)
			{
				//消す
				m_apOrbit[nCntOrbit]->Uninit();
				m_apOrbit[nCntOrbit] = nullptr;
			}
			else
			{
				//位置を設定
				m_apOrbit[nCntOrbit]->SetPos(pos, size);
			}
		}
	}
}

//================================================
//描画処理
//================================================
void CBallistic::Draw(void)
{
	
}

//================================================
//生成処理
//================================================
CBallistic *CBallistic::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 rot, const D3DXVECTOR3 endPos,
	                     const float fSpeed, const string sTexPas1, const string sTexPas2)
{
	//インスタンスの生成
	CBallistic *pBallistic = nullptr;
	if (pBallistic == nullptr)
	{
		pBallistic = new CBallistic;
		if (pBallistic != nullptr)
		{
			pBallistic->m_bigenPos = pos;
			pBallistic->m_size = size;
			pBallistic->m_rot = rot;
			pBallistic->m_endPos = endPos;
			pBallistic->m_fSpeed = fSpeed;
			pBallistic->m_sTexPas1 = sTexPas1;
			pBallistic->m_sTexPas2 = sTexPas2;
			pBallistic->Init();
		}
	}
	return pBallistic;
}