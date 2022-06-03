//================================================
//メッシュフィールド処理
//Author:加藤瑞葵
//================================================
#include "renderer.h"
#include "meshfield.h"
#include "manager.h"
#include "mouseinput.h"

//================================================
//マクロ定義
//================================================
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)		//頂点フォーマット3D
#define VERTEX_3D_NUM (4)														//頂点数

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CMeshField::CMeshField(LAYER_TYPE Layer) : CScene(Layer)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_indexPos.clear();
	m_indexSavePos.clear();
	m_bufferPos.clear();
	m_indexRot.clear();
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLine = 0;
	m_nVertical = 0;
	m_indexMtxWorld.clear();
}

//================================================
//オーバーロードされたコンストラクタ
//================================================

//================================================
//デストラクタ
//================================================
CMeshField::~CMeshField()
{

}

//================================================
//初期化処理
//================================================
HRESULT CMeshField::Init(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//位置・サイズ設定処理
	CScene::SetPos(m_pos);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nVertical + 1) * (m_nLine + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);


	VERTEX_3D *pVtx;
	//頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nNum = 0;
	if (m_bufferPos.size() == 0)
	{
		for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
		{
			for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
			{
					m_bufferPos.push_back(D3DXVECTOR3(-m_size.x / 2.0f + (m_size.x / m_nLine) * nLine,
			                                          0.0f,
			                                          m_size.z / 2.0f - (m_size.z / m_nVertical) * nVertical));
			}
		}
	}

	nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			//要素を追加
			m_bufferSavePos.push_back({0.0f, 0.0f, 0.0f});

			//頂点座標
			pVtx[nNum].pos = m_bufferPos[nNum];
			//カラー
			pVtx[nNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//テクスチャ
			pVtx[nNum].tex = D3DXVECTOR2(0.0f + (1.0f * nLine), 0.0f + (1.0f * nVertical));
			//法線
			pVtx[nNum].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((m_nVertical - 1) * (m_nLine + 3) + (m_nLine + 1) * (m_nVertical + 1)),
								D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, nullptr);

	WORD *pIdx;		//インデックス情報へのポインタ
	//インデックスバッファをロックし番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt1 = 0; nCnt1 < m_nVertical; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < m_nLine + 1; nCnt2++)
		{
			//番号データの設定
			pIdx[(nCnt2 * 2) + 0 + (m_nLine + 2) * 2 * nCnt1] = static_cast<WORD>((m_nLine + 1) + nCnt2 + (m_nLine + 1) * nCnt1);
			pIdx[(nCnt2 * 2) + 1 + (m_nLine + 2) * 2 * nCnt1] = static_cast<WORD>(0 + nCnt2 + (m_nLine + 1) * nCnt1);
			//要素を追加
			m_indexPos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_indexPos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_indexSavePos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_indexSavePos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			D3DXMATRIX temp;
			m_indexMtxWorld.push_back(temp);
			m_indexMtxWorld.push_back(temp);
			m_indexRot.push_back(m_rot);
			m_indexRot.push_back(m_rot);
		}
	}

	for (int nCnt3 = 0; nCnt3 < m_nVertical - 1; nCnt3++)
	{
		pIdx[((m_nLine + 1) * 2 + 0) * (nCnt3 + 1) + (2 * nCnt3)] = static_cast<WORD>(m_nLine + (m_nLine + 1) * nCnt3);
		pIdx[((m_nLine + 1) * 2 + 1) * (nCnt3 + 1) + (1 * nCnt3)] = static_cast<WORD>(m_nLine * 2 + 2 + (m_nLine + 1) * nCnt3);
		//要素を追加
		m_indexPos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_indexPos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_indexSavePos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_indexSavePos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		D3DXMATRIX temp;
		m_indexMtxWorld.push_back(temp);
		m_indexMtxWorld.push_back(temp);
		m_indexRot.push_back(m_rot);
		m_indexRot.push_back(m_rot);
	}
	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
	return S_OK;
}

//================================================
//終了処理
//================================================
void CMeshField::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//インデックスバッファの破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	//オブジェクトの破棄
	Release();
}

//================================================
//更新処理
//================================================
void CMeshField::Update(void)
{
}

//================================================
//描画処理
//================================================
void CMeshField::Draw(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMatrixIdentity(&m_mtxWorld);		//ワールドマトリックスの初期化

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	pDevice->SetFVF(FVF_VERTEX_3D);					//頂点フォーマットの設定

	pDevice->SetTexture(0, m_pTexture);				//テクスチャの設定

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
									0,
									0,
									((m_nLine + 1) * (m_nVertical + 1)),				//頂点の数
									0,													//開始する頂点のインデックス
									2 * m_nLine * m_nVertical + (m_nVertical * 4) - 4);	//描画するプリミティブ数
}

//================================================
//生成処理
//================================================
CMeshField* CMeshField::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXVECTOR3 &rot, const int &nLine, const int &nVertical)
{
	//インスタンスの生成
	CMeshField *pMeshField = nullptr;
	if (pMeshField == nullptr)
	{
		pMeshField = new CMeshField;
		if (pMeshField != nullptr)
		{
			//変数初期化
			pMeshField->m_rot = rot;
			pMeshField->m_nLine = nLine;
			pMeshField->m_nVertical = nVertical;
			pMeshField->m_pos = pos;
			pMeshField->m_size = size;
			//初期化処理
			pMeshField->Init();
		}
	}
	return pMeshField;
}

//================================================
//テキストファイルからの生成処理
//================================================
CMeshField * CMeshField::CreateLoadText(string sPas)
{
	//インスタンスの生成
	CMeshField *pMeshField = nullptr;
	if (pMeshField == nullptr)
	{
		pMeshField = new CMeshField;
		if (pMeshField != nullptr)
		{
			//ファイルの読み込み
			FILE *pFile;
			pFile = fopen(sPas.c_str(), "r");
			if (pFile != NULL)
			{
				char cStr[128];
				while (fgets(cStr, 128, pFile) != nullptr)
				{
					if (strncmp("SET_MESHFIELD\n", cStr, 15) == 0)
					{
						fscanf(pFile, "%*s%*s%d", &pMeshField->m_nVertical);
						fscanf(pFile, "%*s%*s%d", &pMeshField->m_nLine);
						fscanf(pFile, "%*s%*s%f", &pMeshField->m_size.x);
						fscanf(pFile, "%*s%*s%f", &pMeshField->m_size.z);

						int nCntVtx = 0;
						for (int nVertical = 0; nVertical < pMeshField->m_nVertical + 1; nVertical++)
						{
							for (int nLine = 0; nLine < pMeshField->m_nLine + 1; nLine++, nCntVtx++)
							{
								pMeshField->m_bufferPos.push_back({ 0.0f, 0.0f, 0.0f });
								fscanf(pFile, "%f %f %f\n", &pMeshField->m_bufferPos[nCntVtx].x,
									                        &pMeshField->m_bufferPos[nCntVtx].y,
									                        &pMeshField->m_bufferPos[nCntVtx].z);
							}
						}
					}
				}
			}
			else
			{
				printf("ファイルが開けませんでした\n");
			}
			fclose(pFile);

			//初期化処理
			pMeshField->Init();
		}
	}
	return pMeshField;
}

//================================================
//テクスチャ割り当て処理
//================================================
void CMeshField::BindTexture(const LPDIRECT3DTEXTURE9 &pTexture)
{
	m_pTexture = pTexture;
}