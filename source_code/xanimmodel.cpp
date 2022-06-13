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
#include "input_keyboard.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CXanimModel::CXanimModel()
{
	m_anim_controller = nullptr;
	m_size = { 0.0f, 0.0f, 0.0f };
	m_vtx_min = { 100000.0f, 100000.0f, 100000.0f };
	m_vtx_max = { -100000.0f, -100000.0f, -100000.0f };
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
	int nMaxAnim;

	if (m_anim_controller != nullptr)
	{
		nMaxAnim = m_anim_controller->GetMaxNumAnimationSets();
		for (int nCntAnim = 0; nCntAnim < nMaxAnim; nCntAnim++)
		{
			LPD3DXANIMATIONSET Anim;
			pair<string, int> buf;
			m_anim_set.push_back(Anim);
			m_anim_controller->GetAnimationSet(nCntAnim, &m_anim_set[nCntAnim]);
			m_anim_name.push_back(m_anim_set[nCntAnim]->GetName());
			buf.first = m_anim_name[nCntAnim];
			buf.second = nCntAnim;
			m_anim_type.push_back(buf);
		}
		m_anim_controller->SetTrackAnimationSet(0, m_anim_set[0]);
		m_anim_now_type.first = m_anim_name[0];
		m_anim_now_type.second = 0;
	}

	return S_OK;
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CXanimModel::Uninit(void)
{
	delete m_root_frame;
	delete m_anim_controller;
}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CXanimModel::Update(void)
{
	if (m_anim_controller != nullptr)
	{
		m_anim_controller->AdvanceTime(1.0f, NULL);
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
		D3DXMatrixMultiply(	&frame->CombinedTransformationMatrix,
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
	UpdateFrame(m_root_frame, matrix);
	// フレーム描画
	DrawFrame(m_root_frame);
}

//=============================================================================
// 描画処理
//=============================================================================
void CXanimModel::Draw(void)
{
	LPDIRECT3DDEVICE9 device; //デバイスのポインタ
	device = CManager::GetRenderer()->GetDevice();	//デバイスを取得する
	D3DXMATRIX mtx_parent, trans_matrix, rot_matrix;

	D3DXMatrixIdentity(&m_mtx_wold);

	D3DXMatrixRotationYawPitchRoll(	&rot_matrix,
									m_rot.y,
									m_rot.x,
									m_rot.z);
	D3DXMatrixMultiply(	&m_mtx_wold,
						&m_mtx_wold,
						&rot_matrix);

	D3DXMatrixTranslation(	&trans_matrix,
							m_pos.x,
							m_pos.y,
							m_pos.z);
	D3DXMatrixMultiply(	&m_mtx_wold,
						&m_mtx_wold,
						&trans_matrix);

	device->GetTransform(D3DTS_WORLD, &mtx_parent);

	//パーツのワールドマトリックスと親のワールドマトリックスを掛け合わせる
	D3DXMatrixMultiply(	&m_mtx_wold,
						&m_mtx_wold,
						&mtx_parent);

	DrawMatrix(&m_mtx_wold);
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
	MeshContainer *mesh_container = (MeshContainer*)container;
	pDevice = CManager::GetRenderer()->GetDevice();	//デバイスを取得する

	// カリング
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

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
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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
		pFrame = (FrameData*)D3DXFrameFind(m_root_frame, container->pSkinInfo->GetBoneName(nCntBone));
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
CXanimModel *CXanimModel::Create(string type)
{
	CXanimModel *anim_model = nullptr;

	if (anim_model == nullptr)
	{
		anim_model = new CXanimModel;
		if (anim_model != nullptr)
		{
			anim_model->m_load_pas = type;
			anim_model->Load();
			anim_model->Init();
		}
	}

	return anim_model;
}

//=============================================================================
// モデルのロード
//=============================================================================
void CXanimModel::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	//デバイスを取得する

	D3DXLoadMeshHierarchyFromX(	m_load_pas.c_str(),
								D3DXMESH_MANAGED,
								pDevice,
								&m_hierarchy_data,
								NULL,
								&m_root_frame,
								&m_anim_controller);
	AllocateAllBoneMatrix(m_root_frame);
	CheckContainer(m_root_frame);
}

//=============================================================================
// アニメーション変更
//=============================================================================
void CXanimModel::ChangeAnimation(string anim_name, float speed)
{
	if (m_anim_controller != NULL)
	{
		int max_anim = m_anim_type.size();
		for (int count_anim = 0; count_anim < max_anim; count_anim++)
		{
			if (m_anim_type[count_anim].first == anim_name)
			{
				m_anim_controller->SetTrackSpeed(0, speed);
				m_anim_controller->SetTrackAnimationSet(0, m_anim_set[m_anim_type[count_anim].second]);
				m_anim_controller->SetTrackAnimationSet(1, m_anim_set[m_anim_now_type.second]);
				m_anim_controller->SetTrackPosition(0, 0);
				m_anim_now_type = m_anim_type[count_anim];
			}
		}
	}
}

//=============================================================================
// コンテナのチェック
//=============================================================================
void CXanimModel::CheckContainer(LPD3DXFRAME frame, D3DXMATRIX *check_mtx, string name)
{
	FrameData *frame_data = (FrameData*)frame;
	MeshContainer *container_data = (MeshContainer*)frame_data->pMeshContainer;

	// コンテナがあったら
	while (container_data != NULL)
	{
		if (container_data->Name == name)
		{
			LPD3DXBONECOMBINATION bone_buffer = (LPD3DXBONECOMBINATION)container_data->m_BoneBuffer->GetBufferPointer();	// ボーンの数まわす
			for (DWORD nCntBone = 0; nCntBone < container_data->m_BoneNum; nCntBone++)
			{
				for (DWORD nCntWeight = 0; nCntWeight < container_data->m_BoneWeightNum; nCntWeight++)
				{
					DWORD matrix_index = bone_buffer[nCntBone].BoneId[nCntWeight];

					if (matrix_index != UINT_MAX)
					{
						// オフセット行列(m_BoneOffsetMatrix) * ボーンの行列(m_BoneMatrix)で最新の位置を割り出す
						*check_mtx = container_data->m_BoneOffsetMatrix[matrix_index] * (*container_data->m_BoneMatrix[matrix_index]);
						break;
					}
				}
			}
		}
		container_data = (MeshContainer*)container_data->pNextMeshContainer;
	}

	// 兄弟がいれば再帰で呼び出す
	if (frame_data->pFrameSibling != NULL)
	{
		CheckContainer(frame_data->pFrameSibling, check_mtx, name);
	}

	// 子がいれば再帰で呼び出す
	if (frame_data->pFrameFirstChild != NULL)
	{
		CheckContainer(frame_data->pFrameFirstChild, check_mtx, name);
	}
}

void CXanimModel::CheckContainer(LPD3DXFRAME frame)
{
	FrameData *frame_data = (FrameData*)frame;
	MeshContainer *container_data = (MeshContainer*)frame_data->pMeshContainer;

	// コンテナがあったら
	while (container_data != NULL)
	{
		int num_vtx = 0;// 頂点数
		DWORD size_fvf;	// 頂点フォーマットのサイズ
		BYTE *vtx_buf;	// 頂点バッファへのポインタ

		// 通常メッシュ
		if (container_data->MeshData.Type == D3DXMESHTYPE_MESH)
		{
			// 頂点数を取得
			num_vtx = container_data->MeshData.pMesh->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			size_fvf = D3DXGetFVFVertexSize(container_data->MeshData.pMesh->GetFVF());

			// 頂点バッファをロック
			container_data->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtx_buf);

			for (int nCntVtx = 0; nCntVtx < num_vtx; nCntVtx++)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)vtx_buf;	// 頂点座標の代入

				// 全ての頂点を比較して最大値と最小手を抜き出す
				// 最大値
				if (vtx.x >= m_vtx_max.x)
				{
					m_vtx_max.x = vtx.x;
				}
				if (vtx.y >= m_vtx_max.y)
				{
					m_vtx_max.y = vtx.y;
				}
				if (vtx.z >= m_vtx_max.z)
				{
					m_vtx_max.z = vtx.z;
				}
				// 最小値
				if (vtx.x <= m_vtx_min.x)
				{
					m_vtx_min.x = vtx.x;
				}
				if (vtx.y <= m_vtx_min.y)
				{
					m_vtx_min.y = vtx.y;
				}
				if (vtx.z <= m_vtx_min.z)
				{
					m_vtx_min.z = vtx.z;
				}

				vtx_buf += size_fvf;		//頂点フォーマットのサイズ分ポインタを進める
			}

			m_size.x = m_vtx_max.x - m_vtx_min.x;
			m_size.y += m_vtx_max.y - m_vtx_min.y;
			m_size.z = m_vtx_max.z - m_vtx_min.z;
		}

		// プログレッシブメッシュ
		else if (container_data->MeshData.Type == D3DXMESHTYPE_PMESH)
		{
			// 頂点数を取得
			num_vtx = container_data->MeshData.pPMesh->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			size_fvf = D3DXGetFVFVertexSize(container_data->MeshData.pPMesh->GetFVF());

			// 頂点バッファをロック
			container_data->MeshData.pPMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vtx_buf);

			for (int nCntVtx = 0; nCntVtx < num_vtx; nCntVtx++)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)vtx_buf;	// 頂点座標の代入

				// 全ての頂点を比較して最大値と最小手を抜き出す
				// 最大値
				if (vtx.x >= m_vtx_max.x)
				{
					m_vtx_max.x = vtx.x;
				}
				if (vtx.y >= m_vtx_max.y)
				{
					m_vtx_max.y = vtx.y;
				}
				if (vtx.z >= m_vtx_max.z)
				{
					m_vtx_max.z = vtx.z;
				}
				// 最小値
				if (vtx.x <= m_vtx_min.x)
				{
					m_vtx_min.x = vtx.x;
				}
				if (vtx.y <= m_vtx_min.y)
				{
					m_vtx_min.y = vtx.y;
				}
				if (vtx.z <= m_vtx_min.z)
				{
					m_vtx_min.z = vtx.z;
				}

				m_size.x = m_vtx_max.x - m_vtx_min.x;
				m_size.y += m_vtx_max.y - m_vtx_min.y;
				m_size.z = m_vtx_max.z - m_vtx_min.z;

				vtx_buf += size_fvf;		//頂点フォーマットのサイズ分ポインタを進める
			}
		}
		container_data = (MeshContainer*)container_data->pNextMeshContainer;
	}

	// 兄弟がいれば再帰で呼び出す
	if (frame_data->pFrameSibling != NULL)
	{
		CheckContainer(frame_data->pFrameSibling);
	}

	// 子がいれば再帰で呼び出す
	if (frame_data->pFrameFirstChild != NULL)
	{
		CheckContainer(frame_data->pFrameFirstChild);
	}
}

//=============================================================================
// 指定オブジェクトのマトリックス入手
//=============================================================================
D3DXMATRIX *CXanimModel::GetMatrix(string name)
{
	D3DXMATRIX *buf = new D3DXMATRIX;

	CheckContainer(m_root_frame, buf, name);

	return buf;
}