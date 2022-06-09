//=============================================================================
// 球状処理 [Sphere.cpp]
// Author : 佐藤秀亮
//=============================================================================
#include "Sphere.h"
#include "manager.h"
#include "renderer.h"
#include "control.h"

LPDIRECT3DTEXTURE9 CSphere::m_pTexture[MAX_TEXTURE] = {};
int CSphere::m_nMaxTex = 0;

//=============================================================================
// マクロ
//=============================================================================
#define TEXTURE_FILENAME_SPHERE "data/Tex3DNameRead.txt"

//=============================================================================
// コンストラクタ
//=============================================================================
CSphere::CSphere(int nPriority) : CScene3D::CScene3D(nPriority)
{
	m_mtxWorld = {};
	m_Pos = {};
	m_Rot = {};
	m_nVertical = {};
	m_nLine = {};
	m_nTex = {};
	m_fRadius = {};
}

//=============================================================================
// デストラクタ
//=============================================================================
CSphere::~CSphere(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CSphere::Init(D3DXVECTOR3 pos, float rot, float fSize, int Vertical, int Line, int nTex, int Synthetic, D3DXVECTOR2 TexNum)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する

	m_Pos = pos;
	m_Rot = rot;
	m_fRadius = fSize;
	m_nVertical = Vertical;
	m_nLine = Line;
	m_nTex = nTex;
	m_TexUV = TexNum;

	m_Synthetic = Synthetic;

	m_nVtx = ((m_nLine + 1) * (m_nVertical + 1));	//頂点数
	m_nIdx = ((2 * m_nLine * (m_nVertical * 2)));	//インデックス
	m_Primithive = (2 * m_nLine * m_nVertical + (m_nVertical * 4) - 4);	//プリミティブ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	int nCntVtx = 0;	//頂点カウント

	//縦の頂点の座標設定
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		//横の頂点の座標設定
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nCntVtx++)
		{
			//頂点座標角度
			float fphi = (D3DX_PI * 2) / m_nVertical * nVertical;
			float ftheta = (D3DX_PI * 2) / m_nVertical * nLine;

			//頂点座標
			pVtx[nCntVtx].pos = D3DXVECTOR3(
				m_Pos.x + m_fRadius * sinf(ftheta) * cosf(fphi),	//X
				m_Pos.y + m_fRadius * sinf(ftheta + D3DX_PI / 2),	//ｚ(ｙ)
				m_Pos.z + m_fRadius * sinf(ftheta) * sinf(fphi));	//z

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//テクスチャ座標
			pVtx[nCntVtx].tex = D3DXVECTOR2((m_TexUV.x / m_nLine * nLine), (m_TexUV.y / m_nVertical * nVertical));

		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx; //インデックス情報へのポインタ
	//インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void **)&pIdx, 0);

	//インデックス設定
	for (int nVertical = 0; nVertical < m_nVertical; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++)
		{
			pIdx[(m_nLine * 2 + 4) * nVertical + 0 + (nLine * 2)] = (nLine + (m_nLine + 1) + (m_nLine + 1) * nVertical);
			pIdx[(m_nLine * 2 + 4) * nVertical + 1 + (nLine * 2)] = (nLine + 0 + (m_nLine + 1) * nVertical);
		}
	}
	//重複インデックス
	for (int nDup = 0; nDup < m_nVertical - 1; nDup++)
	{
		pIdx[(m_nLine * 2 + 2) + 0 + nDup * (m_nLine * 2 + 4)] = (m_nLine + (m_nLine + 1) * nDup);
		pIdx[(m_nLine * 2 + 2) + 1 + nDup * (m_nLine * 2 + 4)] = ((m_nLine * 2 + 2)+ (m_nLine + 1) * nDup);
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CSphere::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	Release();
}

//=============================================================================
// 更新
//=============================================================================
void CSphere::Update()
{

	m_nLife--;
	if (m_nLife <= 0)
	{
		m_bUninit = true;
	}

	if (m_bUninit == true)
	{
		Uninit();
	}
}

//=============================================================================
// 描画
//=============================================================================
void CSphere::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する

	//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	if (m_Synthetic == 0)
	{
		//加算合成関係
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else if (m_Synthetic == 1)
	{
		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	//それ以外の数値は加算合成に
	else
	{
		//加算合成関係
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//カリングオフ
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetTexture(0, m_pTexture[m_nTex]);    //テクスチャの設定
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nVtx, //頂点の数
		0,
		m_Primithive); //描画するプリミティブ数

	//カリングオン
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	//通常合成に戻す(加算合成)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// 色セット
//=============================================================================
void CSphere::SetColor(D3DCOLORVALUE Color)
{
	VERTEX_3D *pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	//頂点の色
	for (int Vtx = 0; Vtx < m_nVtx; Vtx++)
	{
		pVtx[Vtx].col = D3DCOLOR_RGBA((int)Color.r, (int)Color.g, (int)Color.b, (int)Color.a);
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//=============================================================================
// サイズセット
//=============================================================================
void CSphere::SetSize(D3DXVECTOR3 pos, float Size)
{
	m_fRadius = Size;
	m_Pos = pos;

	VERTEX_3D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	int nCntVtx = 0;	//頂点カウント

	//縦の頂点の座標設定
	for (int Vertical = 0; Vertical < m_nVertical + 1; Vertical++)
	{
		//横の頂点の座標設定
		for (int Line = 0; Line < m_nLine + 1; Line++, nCntVtx++)
		{
			//頂点座標角度
			float fphi = (D3DX_PI * 2) / m_nVertical * Vertical;
			float ftheta = (D3DX_PI * 2) / m_nVertical * Line;

			//頂点座標
			pVtx[nCntVtx].pos = D3DXVECTOR3(
				m_Pos.x + m_fRadius * sinf(ftheta) * cosf(fphi),	//X
				m_Pos.y + m_fRadius * sinf(ftheta + D3DX_PI / 2),	//ｚ(ｙ)
				m_Pos.z + m_fRadius * sinf(ftheta) * sinf(fphi));	//z
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}

//=============================================================================
// テクスチャUV座標セット
//=============================================================================
void CSphere::SetTexUV(D3DXVECTOR2 TexUV)
{
	VERTEX_3D *pVtx; //頂点情報へのポインタ

	m_TexMoveUV += TexUV;
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	int nCntVtx = 0;	//頂点カウント

						//縦の頂点の座標設定
	for (int Vertical = 0; Vertical < m_nVertical + 1; Vertical++)
	{
		//横の頂点の座標設定
		for (int Line = 0; Line < m_nLine + 1; Line++, nCntVtx++)
		{
			//テクスチャ座標
			pVtx[nCntVtx].tex = D3DXVECTOR2(
				(m_TexUV.x / m_nLine * Line) + m_TexMoveUV.x,
				(m_TexUV.y / m_nVertical * Vertical)+ m_TexMoveUV.y);
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}


//=============================================================================
// テクスチャUV座標セット
//=============================================================================
void CSphere::SetAnimTexUV( D3DXVECTOR2 TexPattern)
{
	VERTEX_3D *pVtx; //頂点情報へのポインタ

	m_PatternUV = TexPattern;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	int nCntVtx = 0;	//頂点カウント

						//縦の頂点の座標設定
	for (int Vertical = 0; Vertical < m_nVertical + 1; Vertical++)
	{
		//横の頂点の座標設定
		for (int Line = 0; Line < m_nLine + 1; Line++, nCntVtx++)
		{
			//テクスチャ座標
			pVtx[nCntVtx].tex = D3DXVECTOR2(
				(m_TexUV.x / m_nLine * Line) + m_PatternUV.x + m_TexMoveUV.x,
				(m_TexUV.y / m_nVertical * Vertical) + m_PatternUV.y + m_TexMoveUV.y);
		}
	}
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}


//=============================================================================
// テクスチャ生成
//=============================================================================
void CSphere::CreateTextureSphere(void)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスの取得

	//ファイル読み込み
	char aFile[256];
	FILE *pFile = fopen(TEXTURE_FILENAME_SPHERE, "r");

	int nCntTex = 1;

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);

			if (strcmp(&aFile[0], "NUM_TEXTURE") == 0)	//NUM_TEXTUREの文字列
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &m_nMaxTex);//使用するテクスチャ数を読み込む
			}

			if (strcmp(&aFile[0], "TEXTURE_FILENAME") == 0) //TEXTURE_FILENAMEの文字列
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				D3DXCreateTextureFromFile(pDevice, &aFile[0], &m_pTexture[nCntTex]);
				nCntTex++;
			}

			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPTの文字列を見つけたら
			{
				break;
			}

		}
	}
	fclose(pFile);
}

//=============================================================================
// テクスチャ破棄
//=============================================================================
void CSphere::UninitTextureSphere()
{
	//テクスチャ破棄
	for (int nCnt = 0; nCnt < m_nMaxTex; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}

}
