//================================================
//メッシュフィールド処理
//Author:加藤瑞葵
//================================================
#include "renderer.h"
#include "mesh_field.h"
#include "object3D.h"
#include "manager.h"

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CMeshField::CMeshField(CObject::PRIORITY Priority):CObject(Priority)
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
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//位置・サイズ設定処理
	CObject::SetPos(m_pos);
	CObject::SetSize(m_size);

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
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//頂点バッファをロックし、頂点データのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			D3DXVECTOR3 vecPos[2];

			if (nVertical < m_nVertical)
			{
				if (nLine < m_nLine)
				{
					vecPos[0] = m_bufferPos[nNum + 1] - m_bufferPos[nNum];
					vecPos[1] = m_bufferPos[nNum + (m_nLine + 2)] - m_bufferPos[nNum];
				}
				else
				{
					vecPos[0] = m_bufferPos[nNum + (m_nLine + 1)] - m_bufferPos[nNum];
					vecPos[1] = m_bufferPos[nNum - 1] - m_bufferPos[nNum];
				}
			}
			else
			{
				if (nLine < m_nLine)
				{
					vecPos[0] = m_bufferPos[nNum / 2] - m_bufferPos[nNum];
					vecPos[1] = m_bufferPos[nNum + 1] - m_bufferPos[nNum];
				}
				else
				{
					vecPos[0] = m_bufferPos[nNum - (m_nLine + 1)] - m_bufferPos[nNum];
					vecPos[1] = m_bufferPos[nNum - (m_nLine + 1) - 1] - m_bufferPos[nNum];
				}
			}



			//法線を求める
			D3DXVECTOR3 vecNor;
			D3DXVec3Cross(&vecNor, &vecPos[0], &vecPos[1]);
			//正規化する
			D3DXVec3Normalize(&vecNor, &vecNor);
			//vecNor = {0.0f, 1.0f, 0.0f};

			//法線設定
			pVtx[nNum].nor = vecNor;
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
	
	//頂点情報の保存
	for (int nCnt1 = 0; nCnt1 < m_nVertical; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < m_nLine + 1; nCnt2++)
		{
			m_indexPos[(nCnt2 * 2) + 0 + (m_nLine + 2) * 2 * nCnt1] = m_bufferPos[((m_nLine + 1) + nCnt2 + (m_nLine + 1) * nCnt1)];
			m_indexPos[(nCnt2 * 2) + 1 + (m_nLine + 2) * 2 * nCnt1] = m_bufferPos[(0 + nCnt2 + (m_nLine + 1) * nCnt1)];
		}
	}

	for (int nCnt3 = 0; nCnt3 < m_nVertical - 1; nCnt3++)
	{
		m_indexPos[((m_nLine + 1) * 2 + 0) * (nCnt3 + 1) + (2 * nCnt3)] = m_bufferPos[(m_nLine + (m_nLine + 1) * nCnt3)];
		m_indexPos[((m_nLine + 1) * 2 + 1) * (nCnt3 + 1) + (1 * nCnt3)] = m_bufferPos[(m_nLine * 2 + 2 + (m_nLine + 1) * nCnt3)];
	}

	//オブジェクトの種類の設定
	SetObjType(CObject::OBJTYPE::FLOOR);
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
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

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

	
	////カメラのポインタ配列1番目のアドレス取得
	//CCamera** pCameraAddress = CManager::GetInstance()->GetCamera();
	////cameraの取得
	//CCamera* pCamera = &**pCameraAddress;
	//if (pCamera != nullptr)
	//{
	//	for (int nCnt1 = 0; nCnt1 < m_indexPos.size() - 2; nCnt1++)
	//	{
	//		//対象の現在位置取得
	//		D3DXVECTOR3 pos[3];
	//		pos[0] = m_indexPos[nCnt1];
	//		pos[1] = m_indexPos[nCnt1 + 1];
	//		pos[2] = m_indexPos[nCnt1 + 2];

	//		//スクリーン座標に変換
	//		D3DXVECTOR3 screenPos[3];
	//		screenPos[0] = pCamera->WorldPosToScreenPos(pos[0]);
	//		screenPos[1] = pCamera->WorldPosToScreenPos(pos[1]);
	//		screenPos[2] = pCamera->WorldPosToScreenPos(pos[2]);

	//		//スクリーンに映っているとき
	//		if ((screenPos[0].x >= 0.0f && screenPos[0].x <= SCREEN_WIDTH && screenPos[0].y >= 0.0f && screenPos[0].y <= SCREEN_HEIGHT) ||
	//			(screenPos[1].x >= 0.0f && screenPos[1].x <= SCREEN_WIDTH && screenPos[1].y >= 0.0f && screenPos[1].y <= SCREEN_HEIGHT) ||
	//			(screenPos[2].x >= 0.0f && screenPos[2].x <= SCREEN_WIDTH && screenPos[2].y >= 0.0f && screenPos[2].y <= SCREEN_HEIGHT))
	//		{
	//			//頂点バッファをデータストリームに設定
	//			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//			//インデックスバッファをデータストリームに設定
	//			pDevice->SetIndices(m_pIdxBuff);

	//			pDevice->SetFVF(FVF_VERTEX_3D);					//頂点フォーマットの設定

	//			pDevice->SetTexture(0, m_pTexture);				//テクスチャの設定

	//			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
	//				0,
	//				0,
	//				((m_nLine + 1) * (m_nVertical + 1)),				//頂点の数
	//				nCnt1,												//開始する頂点のインデックス
	//				2);	//描画するプリミティブ数
	//		}
	//	}
	//}
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

//================================================
//当たり判定
//================================================
bool CMeshField::Collision(CObject *pSubjectObject, const float &fRadius)
{
	//着地しているかどうか
	bool bLand = false;

	//オブジェクト情報を入れるポインタ
	vector<CObject*> object;

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MESH_FIELD));
	int nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::FLOOR)
		{
			//ポインタをCMeshFieldにキャスト
			CMeshField *pMeshField = nullptr;
			pMeshField = (CMeshField*)object[nCnt];

			for (int nCnt1 = 0; nCnt1 < 2 * pMeshField->m_nLine * pMeshField->m_nVertical + (pMeshField->m_nVertical * 4) - 4; nCnt1++)
			{
				//対象の現在位置取得
				D3DXVECTOR3 pos = pSubjectObject->GetPos();
				D3DXVECTOR3 diff = pos - pMeshField->m_indexPos[nCnt1];

				if (D3DXVec3LengthSq(&diff) > fRadius*fRadius)
				{
					continue;
				}

				//各頂点から各頂点のベクトルを算出
				D3DXVECTOR3 vecVtx[VERTEX_3D_NUM - 1];
				
				//偶数
				if (nCnt1 % 2 == 0)
				{
					vecVtx[0] = pMeshField->m_indexPos[nCnt1 + 1] - pMeshField->m_indexPos[nCnt1];
					vecVtx[1] = pMeshField->m_indexPos[nCnt1 + 2] - pMeshField->m_indexPos[nCnt1 + 1];
					vecVtx[2] = pMeshField->m_indexPos[nCnt1] - pMeshField->m_indexPos[nCnt1 + 2];
				}
				else
				{//奇数
					vecVtx[0] = pMeshField->m_indexPos[nCnt1] - pMeshField->m_indexPos[nCnt1 + 2];
					vecVtx[1] = pMeshField->m_indexPos[nCnt1 + 1] - pMeshField->m_indexPos[nCnt1];
					vecVtx[2] = pMeshField->m_indexPos[nCnt1 + 2] - pMeshField->m_indexPos[nCnt1 + 1];
				}

				//法線保存用
				D3DXVECTOR3 vecNor;

				D3DXVECTOR3 vecVtxInv = -vecVtx[2];

				//ポリゴンの法線を求める
				D3DXVec3Cross(&vecNor, &vecVtx[0], &vecVtxInv);

				//nCntが奇数の時
				if (nCnt1 % 2 == 1)
				{
					vecNor *= -1.0f;
				}

				//ベクトルを正規化する(ベクトルの大きさを1にする)
				D3DXVec3Normalize(&vecNor, &vecNor);

				//各頂点から対象の現在位置までのベクトルを算出
				D3DXVECTOR3 vecObj[VERTEX_3D_NUM - 1];
				vecObj[0] = pos - pMeshField->m_indexPos[nCnt1];
				vecObj[1] = pos - pMeshField->m_indexPos[nCnt1 + 1];
				vecObj[2] = pos - pMeshField->m_indexPos[nCnt1 + 2];
				
				//各頂点から対象の現在位置までのベクトルと各頂点から各頂点のベクトルの外積(2D)を求める
				float fvecCross[VERTEX_3D_NUM - 1];

				for (int nCntVec = 0; nCntVec < VERTEX_3D_NUM - 1; nCntVec++)
				{
					//外積の計算
					fvecCross[nCntVec] = (vecVtx[nCntVec].x * vecObj[nCntVec].z) - (vecObj[nCntVec].x * vecVtx[nCntVec].z);

					//nCntが奇数の時
					if (nCnt1 % 2 == 1)
					{
						fvecCross[nCntVec] *= -1.0f;
					}
				}
				
				//ポリゴンの範囲内にいるかどうか
				if (fvecCross[0] <= 0.0f && fvecCross[1] <= 0.0f && fvecCross[2] <= 0.0f)
				{
					//ポリゴンの位置から対象の現在の位置のベクトルを算出
					D3DXVECTOR3 vecPos;
					vecPos = pos - pMeshField->m_indexPos[nCnt1];

					//算出したベクトルと法線のベクトルの内積を求める
					float fVecDot;
					fVecDot = D3DXVec3Dot(&vecNor, &vecPos);

					//内積の計算結果がマイナスの時
					if (fVecDot < 0.0f)
					{
						//押し出す位置を求める
						D3DXVECTOR3 objectPos = pos;

						objectPos.y = pMeshField->m_indexPos[nCnt1].y - 1.0f / vecNor.y *
							          (vecNor.x * (objectPos.x - pMeshField->m_indexPos[nCnt1].x) +
								       vecNor.z * (objectPos.z - pMeshField->m_indexPos[nCnt1].z));

						pSubjectObject->SetPos(objectPos);
						return true;
					}
				}
			}
		}
	}
	return bLand;
}

//================================================
//当たり判定だけ
//================================================
bool CMeshField::Collision(const D3DXVECTOR3 &HitPos, const float &fDiffer, const D3DXVECTOR3 &beginPos, const D3DXVECTOR3 &endPos)
{
	//始点
	D3DXVECTOR3 posBegin = beginPos;
	//終点
	D3DXVECTOR3 posEnd = endPos;
	//始点から終点までのベクトル
	D3DXVECTOR3 vec = posEnd - posBegin;
	//距離を計算
	float fDifferVec = D3DXVec3Length(&vec);
	float fDifferNear = 100000.0f;

	//当たったかどうか
	bool bHit = false;

	//オブジェクト情報を入れるポインタ
	vector<CObject*> object;

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MESH_FIELD));
	int nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::FLOOR)
		{
			//ポインタをCMeshFieldにキャスト
			CMeshField *pMeshField = nullptr;
			pMeshField = (CMeshField*)object[nCnt];

			for (int nCnt1 = 0; nCnt1 < 2 * pMeshField->m_nLine * pMeshField->m_nVertical + (pMeshField->m_nVertical * 4) - 4; nCnt1++)
			{
				//対象の現在位置取得
				D3DXVECTOR3 diff[VERTEX_3D_NUM - 1];
				diff[0] = posBegin - pMeshField->m_indexPos[nCnt1];
				diff[1] = posBegin - pMeshField->m_indexPos[nCnt1 + 1];
				diff[2] = posBegin - pMeshField->m_indexPos[nCnt1 + 2];

				if (D3DXVec3LengthSq(&diff[0]) > fDifferVec && D3DXVec3LengthSq(&diff[1]) > fDifferVec && D3DXVec3LengthSq(&diff[2]) > fDifferVec)
				{
					continue;
				}

				//各頂点から各頂点のベクトルを算出
				D3DXVECTOR3 vecVtx[VERTEX_3D_NUM - 1];

				//偶数
				if (nCnt1 % 2 == 0)
				{
					vecVtx[0] = pMeshField->m_indexPos[nCnt1 + 1] - pMeshField->m_indexPos[nCnt1];
					vecVtx[1] = pMeshField->m_indexPos[nCnt1 + 2] - pMeshField->m_indexPos[nCnt1 + 1];
					vecVtx[2] = pMeshField->m_indexPos[nCnt1] - pMeshField->m_indexPos[nCnt1 + 2];
				}
				else
				{//奇数
					vecVtx[0] = pMeshField->m_indexPos[nCnt1] - pMeshField->m_indexPos[nCnt1 + 2];
					vecVtx[1] = pMeshField->m_indexPos[nCnt1 + 1] - pMeshField->m_indexPos[nCnt1];
					vecVtx[2] = pMeshField->m_indexPos[nCnt1 + 2] - pMeshField->m_indexPos[nCnt1 + 1];
				}

				//法線保存用
				D3DXVECTOR3 vecNor;

				D3DXVECTOR3 vecVtxInv = -vecVtx[2];

				//ポリゴンの法線を求める
				D3DXVec3Cross(&vecNor, &vecVtx[0], &vecVtxInv);

				//nCntが奇数の時
				if (nCnt1 % 2 == 1)
				{
					vecNor *= -1.0f;
				}

				//ベクトルを正規化する(ベクトルの大きさを1にする)
				D3DXVec3Normalize(&vecNor, &vecNor);

				//ポリゴンの位置から対象の現在の位置のベクトルを算出
				D3DXVECTOR3 vecPosBegin;
				D3DXVECTOR3 vecPosEnd;
				vecPosBegin = posBegin - pMeshField->m_indexPos[nCnt1];
				vecPosEnd = posEnd - pMeshField->m_indexPos[nCnt1];

				//算出したベクトルと法線のベクトルの内積を求める
				float fVecDotBegin;
				float fVecDotEnd;
				fVecDotBegin = D3DXVec3Dot(&vecNor, &vecPosBegin);
				fVecDotEnd = D3DXVec3Dot(&vecNor, &vecPosEnd);

				//内積の計算結果がマイナスの時
				if (fVecDotBegin > 0.0f && fVecDotEnd < 0.0f)
				{
					D3DXVECTOR3 hitPos = (1 - (fVecDotBegin / (fVecDotBegin + fVecDotEnd)))*vecPosBegin + (fVecDotBegin / (fVecDotBegin + fVecDotEnd)) * vecPosEnd;

					hitPos += pMeshField->m_indexPos[nCnt1];
					D3DXVECTOR3 differBegin = hitPos - pMeshField->m_indexPos[nCnt1];
					float fDifferBegin = D3DXVec3Length(&differBegin);

					if (fDifferBegin < fDifferNear)
					{
						fDifferNear = fDifferBegin;
						hitPos = const_cast<D3DXVECTOR3&>(HitPos);
						fDifferBegin = const_cast<float&>(fDiffer);
						bHit = true;
					}
				}
			}
		}
	}
	return bHit;
}
