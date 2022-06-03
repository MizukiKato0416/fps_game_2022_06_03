//=============================================================================
//Xファイル読み込み処理
//Author:加藤瑞葵
//=============================================================================
#include "x_load.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
//コンストラクタ
//=============================================================================
CXload::CXload()
{
	m_file_data.type.clear();
}

//=============================================================================
//デストラクタ
//=============================================================================
CXload::~CXload()
{

}

//=============================================================================
//初期化
//=============================================================================
void CXload::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスを取得する

	int element_max;		// テクスチャカウント様

	// ファイルを読み込む
	m_file_data.file_name_pas = CFileLoad::Load("data\\MODEL\\");
	element_max = m_file_data.file_name_pas.second.size();

	for (int count_element = 0; count_element < element_max; count_element++)
	{
		// 疑似列挙型を作る
		m_file_data.type[m_file_data.file_name_pas.second[count_element]] = count_element;
	}
	// パスとサイズを保存
	m_file_data.pas = m_file_data.file_name_pas.first;
	m_nNum = m_file_data.file_name_pas.first.size();

	// サイズ分回す
	for (int nCntTex = 0; nCntTex < m_nNum; nCntTex++)
	{
		LPD3DXBUFFER buf;
		DWORD num_buf;
		LPD3DXMESH mesh_buf;

		//プレイヤーの体
		D3DXLoadMeshFromX(m_file_data.file_name_pas.first[nCntTex].c_str(), D3DXMESH_SYSTEMMEM, pDevice, NULL,
			              &buf, NULL, &num_buf, &mesh_buf);
		m_aNumMat.push_back(num_buf);
		m_apBuffMat.push_back(buf);
		m_apMesh.push_back(mesh_buf);
	}

	//Xファイル読み込み
}	

//=============================================================================
//終了
//=============================================================================
void CXload::Uninit(void)
{
	for (int nCntXload = 0; nCntXload < m_nNum; nCntXload++)
	{
		//メッシュの破棄
		if (m_apMesh[nCntXload] != NULL)
		{
			m_apMesh[nCntXload]->Release();
			m_apMesh[nCntXload] = NULL;
		}

		//マテリアルの破棄
		if (m_apBuffMat[nCntXload] != NULL)
		{
			m_apBuffMat[nCntXload]->Release();
			m_apBuffMat[nCntXload] = NULL;
		}
	}
}

//=============================================================================
//メッシュの割り当て
//=============================================================================
LPD3DXMESH CXload::GetMesh(const string &sPas)
{
	return m_apMesh[m_file_data.type[sPas]];
}

//=============================================================================
//マテリアル割り当て
//=============================================================================
LPD3DXBUFFER CXload::GetBuffMat(const string &sPas)
{
	return m_apBuffMat[m_file_data.type[sPas]];
}

//=============================================================================
//マテリアル数割り当て
//=============================================================================
DWORD CXload::GetNumMat(const string &sPas)
{
	return m_aNumMat[m_file_data.type[sPas]];
}
