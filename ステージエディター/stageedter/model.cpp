//=============================================================================
//
// Xファイル読み込み処理 [sceneX.cpp]
// Author : 羽鳥太一&鶴間俊樹v
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "model.h"
#include "renderer.h"
#include "manager.h"
#include "block.h"
#include "camera.h"
#include "texture.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CModel::ModelData *CModel::m_aModelData;
int CModel::m_nModelMax;	// 読み込むテクスチャの最大数
char **CModel::m_aFilePas;	// ファイルのパス

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CModel::CModel()
{
	
}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// モデルの初期化処理
//=============================================================================
HRESULT CModel::Init(int type)
{
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	LPDIRECT3DDEVICE9 pDevice; // デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	// デバイスを取得する
	m_nType = type;

	//---------------------------
	//テクスチャの抜き出し
	//---------------------------
	D3DXMATERIAL *pMat;	//マテリアルのポインタ
	int nTexNum = CManager::GetTexture()->GetTexNum();

	//マテリアル情報のポインタ
	pMat = (D3DXMATERIAL*)m_aModelData[m_nType].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aModelData[m_nType].nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			for (int nCnt = 0; nCnt < nTexNum; nCnt++)
			{
				char *apPas = CManager::GetTexture()->GetPasName(nCnt);
				if (strcmp(pMat[nCntMat].pTextureFilename, apPas) == 0)
				{
					m_pTexture[nCntMat] = CManager::GetTexture()->GetTexture(nCnt);
				}
			}
		}
	}

	//---------------------------
	//頂点座標の抜き出し
	//---------------------------
	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;	//頂点バッファのポインタ

	//頂点数を取得
	nNumVtx = m_aModelData[(int)type].pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_aModelData[(int)type].pMesh->GetFVF());

	//頂点バッファのロック
	m_aModelData[(int)type].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

		//頂点数を比較して最小値、最大値を抜く
		//X
		if (m_vtxMax.x <= vtx.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (m_vtxMin.x >= vtx.x)
		{
			m_vtxMin.x = vtx.x;
		}
		//Y
		if (m_vtxMax.y <= vtx.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (m_vtxMin.y >= vtx.y)
		{
			m_vtxMin.y = vtx.y;
		}
		//Z
		if (m_vtxMax.z <= vtx.z)
		{
			m_vtxMax.z = vtx.z;
		}
		if (m_vtxMin.z >= vtx.z)
		{
			m_vtxMin.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	//頂点を進める
	}
	//頂点バッファをアンロック
	m_aModelData[(int)type].pMesh->UnlockVertexBuffer();

	return S_OK;
}

HRESULT CModel::Init(void)
{
	return S_OK;
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CModel::Uninit(void)
{

}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CModel::Update(void)
{
	
}

//=============================================================================
// モデルの描画処理
//=============================================================================
void CModel::Draw(bool bUse)
{
	LPDIRECT3DDEVICE9 pDevice; // デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	// デバイスを取得する

	//--------------------------------------
	//プレイヤー(原点)のマトリックスの設定
	//--------------------------------------
	D3DXMATRIX mtxRot, mtxScale, mtxTrans, mtxParent;	//計算用マトリックス

	D3DXMatrixIdentity(&m_MtxWold);	//マトリックス初期化

	//スケールの設定
	D3DXMatrixScaling(	&mtxScale,
						m_Scale.x,
						m_Scale.y,
						m_Scale.z);

	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&mtxScale);

	//向きの設定
	D3DXMatrixRotationYawPitchRoll(	&mtxRot,
									m_Rot.y,
									m_Rot.x,
									m_Rot.z);

	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&mtxRot);
	//位置
	D3DXMatrixTranslation(	&mtxTrans,
							m_Pos.x,
							m_Pos.y,
							m_Pos.z);

	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&mtxTrans);

	//パーツの親のマトリックスを設定
	//もし親がいたら
	if (m_pPrent != NULL)
	{
		mtxParent = m_pPrent->GetMatrix();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//パーツのワールドマトリックスと親のワールドマトリックスを掛け合わせる
	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&mtxParent);

	//マトリックスの設定
	pDevice->SetTransform(	D3DTS_WORLD,
							&m_MtxWold);

	//---------------------------
	//描画処理
	//---------------------------
	D3DMATERIAL9 matDef;	//マテリアルの一時保存
	D3DXMATERIAL *pMat;		//マテリアルデータのポインタ

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータのポインタ
	pMat = (D3DXMATERIAL*)m_aModelData[m_nType].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aModelData[m_nType].nNumMat; nCntMat++)
	{
		if (bUse == true)
		{
			pMat[nCntMat].MatD3D.Diffuse.a = 0.5;
		}
		else if (bUse == false)
		{
			pMat[nCntMat].MatD3D.Diffuse.a = 1.0;
		}

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(	0,
								m_pTexture[nCntMat]);

		//モデルパーツの描画
		m_aModelData[m_nType].pMesh->DrawSubset(nCntMat);
	}
	//一時保存してたやつをもどす
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//モデルデータの読み込み
//=============================================================================
HRESULT CModel::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	//デバイスを取得する
	FILE *pFile;
	char aFile[PASCHAR_MAX * 2];

	pFile = fopen("data/Txtdata/modeldata.txt", "r");

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "PASNUM") == 0) // PASNUMの文字列を見つけたら
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &m_nModelMax);
				m_aFilePas = new char*[m_nModelMax];
				for (int nCnt = 0; nCnt < m_nModelMax; nCnt++)
				{
					m_aFilePas[nCnt] = new char[PASCHAR_MAX];
				}
				m_aModelData = new ModelData[m_nModelMax];
			}
			else if (strcmp(&aFile[0], "MODEL_FILENAME") == 0) // MODEL_FILENAMEの文字列を見つけたら
			{
				for (int nCnt = 0; nCnt < m_nModelMax; nCnt++)
				{
					fscanf(pFile, "%s", m_aFilePas[nCnt]);
					fscanf(pFile, "%s%s", &aFile[0], &aFile[1]);
				}
			}
			else if (strcmp(&aFile[0], "END") == 0) //ENDの文字列を見つけたら
			{
				break;
			}
		}
	}
	else
	{
		printf("テクスチャデータが読み取れませんでした。");
	}

	fclose(pFile);

	//-------------------------------------
	//モデルの読み込み
	//-------------------------------------
	for (int nCnt = 0; nCnt < m_nModelMax; nCnt++)
	{
		D3DXLoadMeshFromX(	m_aFilePas[nCnt],
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,		//ここまで固定
							&m_aModelData[nCnt].pBuffMat,
							NULL,
							&m_aModelData[nCnt].nNumMat,
							&m_aModelData[nCnt].pMesh);
	}

	return S_OK;
}

//=============================================================================
// モデルの生成
//=============================================================================
CModel *CModel::Create(int type)
{
	CModel *pModel = NULL;
	if (pModel == NULL)
	{
		pModel = new CModel;

	}
	if (pModel)
	{
		pModel->Init(type);
	}
	return pModel;
}