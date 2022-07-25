//=============================================================================
//
// テクスチャ処理 [texture.h]
// Author : 羽鳥太一
//
//=============================================================================
#include "texture.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CTexture::CTexture()
{
	m_file_data.type.clear();
}

//=============================================================================
// デストラクタ
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// テクスチャの生成
//=============================================================================
void CTexture::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスのポインタ
	vector<string> folder_name;	// フォルダの保存バッファ
	int element_max;		// テクスチャカウント様
	int file_element;	// テキストファイルの文字列サイズ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスを取得する

	// ファイルを読み込む
	m_all_file = CFileLoad::Load("data\\TEXTURE\\");
	file_element = m_all_file.size();

	// テキストファイルのサイズ分のループ
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
		if (m_file_data.file_name_pas.second[element_count].find("data\\TEXTURE\\") != string::npos)
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
				file_element--;
				element_count--;
			}
		}
	}

	// パスの要素数
	element_max = m_file_data.file_name_pas.second.size();

	for (int count_element = 0; count_element < element_max; count_element++)
	{
		// 疑似列挙型を作る
		m_file_data.type[m_file_data.file_name_pas.second[count_element]] = count_element;
	}

	// パスの要素数を取得
	m_num_tex = m_file_data.file_name_pas.first.size();

	// サイズ分回す
	for (int nCntTex = 0; nCntTex < m_num_tex; nCntTex++)
	{
		LPDIRECT3DTEXTURE9 pTexBuffer = NULL;	// テクスチャのバッファ
		//テクスチャの生成
		D3DXCreateTextureFromFile(	pDevice,
									m_file_data.file_name_pas.first[nCntTex].c_str(),
									&pTexBuffer);
		// vectorに格納
		m_texture.push_back(pTexBuffer);
	}
}

//=============================================================================
// 終了
//=============================================================================
void CTexture::Uninit(void)
{
	for (int nCntTexture = 0; nCntTexture < m_num_tex; nCntTexture++)
	{
		// テクスチャの破棄
		if (m_texture[nCntTexture] != NULL)
		{
			m_texture[nCntTexture]->Release();
			m_texture[nCntTexture] = NULL;
		}
	}
}