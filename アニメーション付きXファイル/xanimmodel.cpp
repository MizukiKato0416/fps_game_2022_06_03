//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 羽鳥太一
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "xanimmodel.h"
#include "renderer.h"
#include "manager.h"
#include "keyinput.h"

bool g_bAnimUse = false;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CXanimModel::CXanimModel()
{

}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CXanimModel::~CXanimModel()
{

}

//=============================================================================
// モデルの初期化処理
//=============================================================================
HRESULT CXanimModel::Init(void)
{
	return S_OK;
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CXanimModel::Uninit(void)
{
	delete m_RootFrame;
	delete m_AnimController;
}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CXanimModel::Update(void)
{
	CKeyboard *pKey = CManager::GetKeyboard();

	if (pKey->GetTrigger(pKey->SPACE) == true)
	{
		if (g_bAnimUse == false)
		{
			g_bAnimUse = true;
		}
		else if (g_bAnimUse == true)
		{
			g_bAnimUse = false;
		}
	}
	if (pKey->GetTrigger(pKey->NUM_1) == true)
	{
		SetAnimation(0, 144.0f / (4800 * 10));
	}
	if (pKey->GetTrigger(pKey->NUM_2) == true)
	{
		SetAnimation(1, 144.0f / (4800 * 1));
	}

	if (g_bAnimUse == true)
	{
		if (m_AnimController != NULL)
		{
			m_AnimController->AdvanceTime(1.0f, NULL);
		}
	}
}

//=============================================================================
// 全フレームの更新
//=============================================================================
void CXanimModel::UpdateFrame(LPD3DXFRAME base, LPD3DXMATRIX parent_matrix)
{
	FrameData *frame = (FrameData*)base;

	// 姿勢行列の更新
	if (parent_matrix != NULL)
	{
		D3DXMatrixMultiply(&frame->CombinedTransformationMatrix,
			&frame->TransformationMatrix,
			parent_matrix);
	}
	else
	{
		frame->CombinedTransformationMatrix = frame->TransformationMatrix;
	}

	// 兄弟があれば再帰で呼び出す
	if (frame->pFrameSibling != NULL)
	{
		UpdateFrame(frame->pFrameSibling, parent_matrix);
	}

	// 子供がいれば再帰で呼び出す
	if (frame->pFrameFirstChild != NULL)
	{
		UpdateFrame(frame->pFrameFirstChild,
					&frame->CombinedTransformationMatrix);
	}
}

//=============================================================================
// フレームの描画処理
//=============================================================================
void CXanimModel::DrawMatrix(LPD3DXMATRIX matrix)
{
	// フレームの行列を更新
	UpdateFrame(m_RootFrame, matrix);
	// フレーム描画
	DrawFrame(m_RootFrame);
}

//=============================================================================
// 描画処理
//=============================================================================
void CXanimModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	//デバイスを取得する
	D3DXMATRIX mtxParent, trans_matrix, rot_matrix;

	D3DXMatrixIdentity(&m_MtxWold);

	D3DXMatrixRotationYawPitchRoll(	&rot_matrix,
									m_Rot.y,
									m_Rot.x,
									m_Rot.z);
	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&rot_matrix);

	D3DXMatrixTranslation(	&trans_matrix,
							m_Pos.x,
							m_Pos.y,
							m_Pos.z);
	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&trans_matrix);

	pDevice->GetTransform(D3DTS_WORLD, &mtxParent);

	//パーツのワールドマトリックスと親のワールドマトリックスを掛け合わせる
	D3DXMatrixMultiply(	&m_MtxWold,
						&m_MtxWold,
						&mtxParent);

	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWold);

	DrawMatrix(&m_MtxWold);
}

//=============================================================================
// 全フレームの描画
//=============================================================================
void CXanimModel::DrawFrame(LPD3DXFRAME frame)
{
	FrameData *frame_data = (FrameData*)frame;
	LPD3DXMESHCONTAINER container_data = frame_data->pMeshContainer;

	// コンテナの数だけ描画する
	while (container_data != NULL)
	{
		DrawMeshContainer(frame, container_data);

		container_data = container_data->pNextMeshContainer;
	}

	// 兄弟がいれば再帰で呼び出す
	if (frame_data->pFrameSibling != NULL)
	{
		DrawFrame(frame_data->pFrameSibling);
	}

	// 子がいれば再帰で呼び出す
	if (frame_data->pFrameFirstChild != NULL)
	{
		DrawFrame(frame_data->pFrameFirstChild);
	}
}

//=============================================================================
// コンテナの描画
//=============================================================================
void CXanimModel::DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	FrameData *frame_data = (FrameData*)frame;
	MeshContainer *mesh_container = (MeshContainer*)container;
	pDevice = CManager::GetRenderer()->GetDevice();	//デバイスを取得する

	// カリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (mesh_container->pSkinInfo != NULL)
	{
		LPD3DXBONECOMBINATION bone_buffer = (LPD3DXBONECOMBINATION)mesh_container->m_BoneBuffer->GetBufferPointer();	// ボーンの数まわす
		for (DWORD nCntBone = 0; nCntBone < mesh_container->m_BoneNum; nCntBone++)
		{
			// ブレンドするボーンの数
			DWORD bone_blend_num = 0;

			// ボーンIDからブレンドする個数を割り出す
			for (DWORD nCntBlend = 0; nCntBlend < mesh_container->m_BoneWeightNum; nCntBlend++)
			{
				if (bone_buffer[nCntBone].BoneId[nCntBlend] != UINT_MAX)
				{
					bone_blend_num++;
				}
			}

			// 頂点ブレンドの設定
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, bone_blend_num - 1);

			for (DWORD nCntWeight = 0; nCntWeight < mesh_container->m_BoneWeightNum; nCntWeight++)
			{
				DWORD matrix_index = bone_buffer[nCntBone].BoneId[nCntWeight];
				D3DXMATRIX matrix;

				if (matrix_index != UINT_MAX)
				{
					// オフセット行列(m_BoneOffsetMatrix) * ボーンの行列(m_BoneMatrix)で最新の位置を割り出す
					matrix = mesh_container->m_BoneOffsetMatrix[matrix_index] * (*mesh_container->m_BoneMatrix[matrix_index]);
					pDevice->SetTransform(D3DTS_WORLDMATRIX(nCntWeight), &matrix);
				}
			}
			pDevice->SetMaterial(&mesh_container->pMaterials[bone_buffer[nCntBone].AttribId].MatD3D);
			pDevice->SetTexture(0, mesh_container->m_TextureList[bone_buffer[nCntBone].AttribId]);
			mesh_container->MeshData.pMesh->DrawSubset(nCntBone);
		}
	}

	// カリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

//=============================================================================
// 全ボーンの描画用マトリックスの保存
//=============================================================================
HRESULT CXanimModel::AllocateAllBoneMatrix(LPD3DXFRAME frame)
{
	if (frame->pMeshContainer != NULL)
	{
		if (FAILED(AllocateBoneMatrix(frame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}

	// 子供があるかどうか
	if (frame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrix(frame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}

	// 兄弟があるかどうか
	if (frame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrix(frame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CXanimModel::AllocateBoneMatrix(LPD3DXMESHCONTAINER container)
{
	FrameData *pFrame = NULL;
	DWORD bone_num = 0;

	MeshContainer *original_container = (MeshContainer*)container;
	if (original_container->pSkinInfo == NULL)
	{
		return S_OK;
	}

	bone_num = original_container->pSkinInfo->GetNumBones();

	// ボーンのマトリクス分の領域を確保してデータを保存する
	original_container->m_BoneMatrix = new D3DXMATRIX*[bone_num];
	for (DWORD nCntBone = 0; nCntBone < bone_num; nCntBone++)
	{
		pFrame = (FrameData*)D3DXFrameFind(m_RootFrame, container->pSkinInfo->GetBoneName(nCntBone));
		if (pFrame == NULL)
		{
			return E_FAIL;
		}
		original_container->m_BoneMatrix[nCntBone] = &pFrame->CombinedTransformationMatrix;
	}

	return S_OK;
}

//=============================================================================
// モデルの生成
//=============================================================================
void CXanimModel::Create(string type)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	int nMaxAnim;
	FLOAT AdjustSpeed = FPS / (4800 * 10);
	pDevice = CManager::GetRenderer()->GetDevice();	//デバイスを取得する

	D3DXLoadMeshHierarchyFromX(	type.c_str(),
								D3DXMESH_MANAGED,
								pDevice,
								&m_HierarchyData,
								NULL,
								&m_RootFrame,
								&m_AnimController);
	AllocateAllBoneMatrix(m_RootFrame);
	if (m_AnimController != NULL)
	{
		nMaxAnim = m_AnimController->GetMaxNumAnimationSets();
		for (int nCntAnim = 0; nCntAnim < nMaxAnim; nCntAnim++)
		{
			LPD3DXANIMATIONSET Anim;
			m_AnimSet.push_back(Anim);
			m_AnimController->GetAnimationSet(nCntAnim, &m_AnimSet[nCntAnim]);
		}
		m_AnimController->SetTrackAnimationSet(0, m_AnimSet[0]);
		m_AnimController->SetTrackSpeed(0, AdjustSpeed);
		m_nNowAnim = 0;
	}
}

//=============================================================================
// アニメーション変更
//=============================================================================
void CXanimModel::SetAnimation(int nAnim, float AdjustSpeed)
{
	if (m_AnimController != NULL)
	{
		m_AnimController->SetTrackSpeed(0, AdjustSpeed);
		m_AnimController->SetTrackAnimationSet(0, m_AnimSet[nAnim]);
		m_AnimController->SetTrackAnimationSet(1, m_AnimSet[m_nNowAnim]);
		m_AnimController->SetTrackPosition(0, 0);
		m_nNowAnim = nAnim;
	}
}