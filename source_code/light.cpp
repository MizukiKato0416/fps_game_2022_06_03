//=======================================================================
//ライトに関する処理
//Aouthor: 加藤　瑞葵
//=======================================================================
#include "main.h"
#include "light.h"
#include "manager.h"
#include "renderer.h"

//================================================
//静的メンバ変数宣言
//================================================
int CLight::m_nNum = 0;

//================================================
//デフォルトコンストラクタ
//================================================
CLight::CLight()
{
	//ライトをクリアする
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	m_nCnt = m_nNum;
	m_nNum++;
}

//================================================
//オーバーロードされたコンストラクタ
//================================================


//================================================
//デストラクタ
//================================================
CLight::~CLight()
{

}

//================================================
//初期化処理
//================================================
HRESULT CLight::Init(D3DXVECTOR3 vecDir, const D3DXVECTOR3 &pos)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//ライトをクリアする
	//ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	//ライトの種類を設定
	m_light.Type = D3DLIGHT_DIRECTIONAL;		//平行光源

	//ライトの拡散光を設定
	m_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//位置設定
	m_light.Position = pos;

	//正規化する(ベクトルの大きさを1にする)
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_light.Direction = vecDir;

	//ライトを設定する
	pDevice->SetLight(m_nCnt, &m_light);

	//ライトを有効にする
	pDevice->LightEnable(m_nCnt, TRUE);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CLight::Uninit(void)
{
	m_nNum--;
}

//================================================
//更新処理
//================================================
void CLight::Update(void)
{
	
}

//================================================
//生成処理
//================================================
CLight *CLight::Create(const D3DXVECTOR3 &vecDir, const D3DXVECTOR3 &pos)
{
	//インスタンスの生成
	CLight *pLight = nullptr;
	if (pLight == nullptr)
	{
		pLight = new CLight;
		if (pLight != nullptr)
		{
			pLight->Init(vecDir, pos);
		}
	}
	return pLight;
}