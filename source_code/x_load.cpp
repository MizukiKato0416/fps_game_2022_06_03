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
	vector<string> folder_name;	// フォルダの保存バッファ
	int pas_element;	// パスの要素数
	int file_element;	// テキストファイルの文字列サイズ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスを取得する

	// ファイルを読み込む
	m_all_file = CFileLoad::Load("data\\MODEL\\");
	file_element = m_all_file.size();

	//サイズ分のループ
	for (int element_count = 0; element_count < file_element; element_count++)
	{
		// パスの保存
		m_file_data.file_name_pas.first.push_back(m_all_file[element_count]);
		m_file_data.file_name_pas.second.push_back(m_all_file[element_count]);
	}

	// パスの要素数
	file_element = m_file_data.file_name_pas.second.size();

	// パスの要素数のサイズ分のループ
	for (int element_count = 0; element_count < file_element; element_count++)
	{
		// パスが混ざってたら
		if (m_file_data.file_name_pas.second[element_count].find("data\\MODEL\\") != string::npos)
		{
			// 拡張子がついていたら
			if (m_file_data.file_name_pas.second[element_count].find(".") != string::npos)
			{
				// フォルダの名前サイズを取得
				int folder_max = folder_name.size();
				for (int folder_count = 0; folder_count < folder_max; folder_count++)
				{
					// 名前を保存する所にパスが混ざっていたら
					if (m_file_data.file_name_pas.second[element_count].find(folder_name[folder_count]) != string::npos)
					{
						// フォルダの名前のサイズを取得
						int name_size = folder_name[folder_count].size();
						for (int count_erase = 0; count_erase < name_size + 1; count_erase++)
						{
							// 名前だけを残す
							m_file_data.file_name_pas.second[element_count].erase(m_file_data.file_name_pas.second[element_count].begin());
						}
					}
				}
			}
			// 拡張子が付いていない(フォルダなので消去)
			else
			{
				folder_name.push_back(m_file_data.file_name_pas.second[element_count]);
				m_file_data.file_name_pas.second.erase(m_file_data.file_name_pas.second.begin() + element_count);
				m_file_data.file_name_pas.first.erase(m_file_data.file_name_pas.first.begin() + element_count);
				element_count--;
				file_element--;
			}
		}
	}

	// パスの要素数
	pas_element = m_file_data.file_name_pas.second.size();

	for (int count_element = 0; count_element < pas_element; count_element++)
	{
		// 疑似列挙型を作る
		m_file_data.type[m_file_data.file_name_pas.second[count_element]] = count_element;
	}

	// パスの要素数を取得
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
