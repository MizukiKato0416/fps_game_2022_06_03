//================================================
//モデル単体を出す処理
//Author:KatoMizuki
//================================================
#include "object2D.h"
#include "manager.h"
#include "input_keyboard.h"
#include "input_pad_x.h"
#include "model_single.h"
#include "renderer.h"
#include "model.h"
#include "x_load.h"
#include "object3D.h"

//================================================
//マクロ定義
//================================================
#define MODEL_SINGLE_SUBTRACTION	(1.0f)		//誤差

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CModelSingle::CModelSingle(CObject::PRIORITY Priority) :CObject(Priority)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pParent = nullptr;
	m_pModel = nullptr;
	m_bCollision = false;
}

//================================================
//オーバーロードされたコンストラクタ
//================================================


//================================================
//デストラクタ
//================================================
CModelSingle::~CModelSingle()
{

}

//================================================
//初期化処理
//================================================
HRESULT CModelSingle::Init(void)
{
	//親の設定
	m_pModel->SetParent(m_pParent);


	//オブジェクトの種類の設定
	SetObjType(CObject::OBJTYPE::MODEL);

	//サイズを取得
	m_size = m_pModel->GetSize();
	SetSize(m_size);
	SetPos(m_pos);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CModelSingle::Uninit(void)
{
	Release();
}

//================================================
//更新処理
//================================================
void CModelSingle::Update(void)
{
	
}

//================================================
//描画処理
//================================================
void CModelSingle::Draw(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	D3DXMatrixIdentity(&m_mtxWorld);		//モデルのワールドマトリックスの初期化

	//モデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//モデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデルの描画
	m_pModel->Draw();
}

//================================================
//生成処理
//================================================
CModelSingle *CModelSingle::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const string &sPas, CModel *pParent, const bool &Collision)
{
	//インスタンスの生成
	CModelSingle *pModelSingle = nullptr;
	if (pModelSingle == nullptr)
	{
		pModelSingle = new CModelSingle;
		if (pModelSingle != nullptr)
		{
			pModelSingle->m_bCollision = Collision;
			pModelSingle->m_rot = rot;
			pModelSingle->m_pParent = pParent;
			pModelSingle->m_pos = pos;
			//モデルの生成
			pModelSingle->m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), sPas);
			pModelSingle->Init();
		}
	}
	return pModelSingle;
}

//================================================
//当たり判定処理
//================================================
bool CModelSingle::SimpleCollision(CObject *pObject)
{
	bool bLand = false;	//着地しているかどうか

	//オブジェクト情報を入れるポインタ
	vector<CObject*> object;

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MODEL));
	int object_size = object.size();

	for (int count_object = 0; count_object < object_size; count_object++)
	{
		if (object[count_object]->GetObjType() == CObject::OBJTYPE::MODEL)
		{
			//モデルシングルにキャスト
			CModelSingle *pModelSingle = nullptr;
			pModelSingle = (CModelSingle*)object[count_object];

			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//対象の位置
			D3DXVECTOR3 posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//モデルの位置
			D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//対象のサイズ
			D3DXVECTOR3 sizeModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//モデルのサイズ
			D3DXVECTOR3 posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//対象の1フレーム前の位置

			//モデルの位置・サイズ取得
			posModel = pModelSingle->GetPos();
			sizeModel = pModelSingle->GetSize();

			//対象の位置・サイズ取得
			pos = pObject->GetPos();
			posOld = pObject->GetPosOld();
			size = pObject->GetSize();

			if (pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.y <= posModel.y + sizeModel.y &&
				posOld.y >= posModel.y + sizeModel.y)
			{
				pos.y = posModel.y + sizeModel.y;
				//位置を設定
				pObject->SetPos(pos);

				bLand = true;
			}
			else if (pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
					 pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
					 pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
					 pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
					 pos.y + size.y >= posModel.y &&
					 posOld.y + size.y <= posModel.y)
			{
				pos.y = posModel.y - size.y;
				//位置を設定
				pObject->SetPos(pos);
			}
			if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
				pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
				pos.x + size.x / 2.0f >= posModel.x - sizeModel.x / 2.0f &&
				posOld.x + size.x / 2.0f <= posModel.x - sizeModel.x / 2.0f)
			{
				pos.x = posModel.x - sizeModel.x / 2.0f - size.x / 2.0f;
				//位置を設定
				pObject->SetPos(pos);
			}
			else if (pos.y + size.y > posModel.y &&
					 pos.y < posModel.y + sizeModel.y &&
					 pos.z + size.x / 2.0f > posModel.z - sizeModel.z / 2.0f &&
					 pos.z - size.x / 2.0f < posModel.z + sizeModel.z / 2.0f &&
					 pos.x - size.x / 2.0f <= posModel.x + sizeModel.x / 2.0f &&
					 posOld.x - size.x / 2.0f >= posModel.x + sizeModel.x / 2.0f)
			{
				pos.x = posModel.x + sizeModel.x / 2.0f + size.x / 2.0f;
				//位置を設定
				pObject->SetPos(pos);
			}
			if (pos.y + size.y > posModel.y &&
				pos.y < posModel.y + sizeModel.y &&
				pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
				pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
				pos.z + size.x / 2.0f >= posModel.z - sizeModel.z / 2.0f &&
				posOld.z + size.x / 2.0f <= posModel.z - sizeModel.z / 2.0f)
			{
				pos.z = posModel.z - sizeModel.z / 2.0f - size.x / 2.0f;
				//位置を設定
				pObject->SetPos(pos);
			}
			else if (pos.y + size.y > posModel.y &&
					 pos.y < posModel.y + sizeModel.y &&
					 pos.x + size.x / 2.0f > posModel.x - sizeModel.x / 2.0f &&
					 pos.x - size.x / 2.0f < posModel.x + sizeModel.x / 2.0f &&
					 pos.z - size.x / 2.0f <= posModel.z + sizeModel.z / 2.0f &&
					 posOld.z - size.x / 2.0f >= posModel.z + sizeModel.z / 2.0f)
			{
				pos.z = posModel.z + sizeModel.z / 2.0f + size.x / 2.0f;
				//位置を設定
				pObject->SetPos(pos);
			}
		}
	}
	return bLand;
}

//================================================
//回転させていい当たり判定処理
//================================================
int CModelSingle::Collision(CObject *pObject)
{
	//着地しているかどうか
	int nHit = 0;

	//オブジェクト情報を入れるポインタ
	vector<CObject*> object;

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MODEL));
	int object_size = object.size();

	for (int count_object = 0; count_object < object_size; count_object++)
	{
		if (object[count_object]->GetObjType() == CObject::OBJTYPE::MODEL)
		{
			//pObjectをCModelSingleにキャスト
			CModelSingle *pModelSingle = (CModelSingle*)object[count_object];

			//当たり判定をする設定なら
			if (pModelSingle->m_bCollision == true)
			{
				//8頂点の取得
				D3DXVECTOR3 vtxPos[MODEL_VTX];
				for (int nCntVtx = 0; nCntVtx < MODEL_VTX; nCntVtx++)
				{
					vtxPos[nCntVtx] = pModelSingle->m_pModel->GetVtxPos(nCntVtx);
				}

				//プレイヤーのサイズ取得
				D3DXVECTOR3 objectSize = pObject->GetSize();

				//対象の幅の分大きくする
				vtxPos[0].x -= objectSize.x / 2.0f;
				vtxPos[1].x += objectSize.x / 2.0f;
				vtxPos[2].x -= objectSize.x / 2.0f;
				vtxPos[3].x += objectSize.x / 2.0f;
				vtxPos[4].x -= objectSize.x / 2.0f;
				vtxPos[5].x += objectSize.x / 2.0f;
				vtxPos[6].x -= objectSize.x / 2.0f;
				vtxPos[7].x += objectSize.x / 2.0f;
				vtxPos[0].z += objectSize.x / 2.0f;
				vtxPos[1].z += objectSize.x / 2.0f;
				vtxPos[2].z += objectSize.x / 2.0f;
				vtxPos[3].z += objectSize.x / 2.0f;
				vtxPos[4].z -= objectSize.x / 2.0f;
				vtxPos[5].z -= objectSize.x / 2.0f;
				vtxPos[6].z -= objectSize.x / 2.0f;
				vtxPos[7].z -= objectSize.x / 2.0f;
				vtxPos[2].y -= objectSize.y;
				vtxPos[3].y -= objectSize.y;
				vtxPos[6].y -= objectSize.y;
				vtxPos[7].y -= objectSize.y;

				//8頂点のワールドマトリックスを取得
				D3DXMATRIX *pVtxMtxWorld = pModelSingle->m_pModel->GetVtxMtxWorld();
				//8頂点の設定
				for (int nCntVtx = 0; nCntVtx < MODEL_VTX; nCntVtx++, pVtxMtxWorld++)
				{
					pModelSingle->m_pModel->SetVtxMtxWorld(vtxPos[nCntVtx], nCntVtx);
					vtxPos[nCntVtx] = D3DXVECTOR3(pVtxMtxWorld->_41, pVtxMtxWorld->_42, pVtxMtxWorld->_43);
				}

				//頂点から頂点までのベクトル算出用
				D3DXVECTOR3 vtxVec[6][2];
				//頂点から頂点までのベクトル算出
				//奥の面
				vtxVec[0][0] = vtxPos[0] - vtxPos[1];
				vtxVec[0][1] = vtxPos[3] - vtxPos[1];
				//右の面
				vtxVec[1][0] = vtxPos[1] - vtxPos[5];
				vtxVec[1][1] = vtxPos[7] - vtxPos[5];
				//前の面
				vtxVec[2][0] = vtxPos[5] - vtxPos[4];
				vtxVec[2][1] = vtxPos[6] - vtxPos[4];
				//左の面
				vtxVec[3][0] = vtxPos[4] - vtxPos[0];
				vtxVec[3][1] = vtxPos[2] - vtxPos[0];
				//上の面
				vtxVec[4][0] = vtxPos[1] - vtxPos[0];
				vtxVec[4][1] = vtxPos[4] - vtxPos[0];
				//下の面
				vtxVec[5][0] = vtxPos[7] - vtxPos[6];
				vtxVec[5][1] = vtxPos[2] - vtxPos[6];

				//法線保存用変数
				D3DXVECTOR3 vecNor[6];
				for (int nCntSurfase = 0; nCntSurfase < 6; nCntSurfase++)
				{
					//各面の法線を求める
					D3DXVec3Cross(&vecNor[nCntSurfase], &vtxVec[nCntSurfase][0], &vtxVec[nCntSurfase][1]);

					//ベクトルを正規化する(ベクトルの大きさを1にする)
					D3DXVec3Normalize(&vecNor[nCntSurfase], &vecNor[nCntSurfase]);
				}

				//対象の現在位置取得
				D3DXVECTOR3 pos = pObject->GetPos();

				//対象の1フレーム前の位置取得
				D3DXVECTOR3 posOld = pObject->GetPosOld();

				//頂点の位置から対象の現在の位置のベクトルを算出
				D3DXVECTOR3 vecPos[6];
				//奥の面
				vecPos[0] = pos - vtxPos[0];
				//右の面
				vecPos[1] = pos - vtxPos[1];
				//前の面
				vecPos[2] = pos - vtxPos[5];
				//左の面
				vecPos[3] = pos - vtxPos[4];
				//上の面
				vecPos[4] = pos - vtxPos[1];
				//下の面
				vecPos[5] = pos - vtxPos[7];

				//算出したベクトルと法線のベクトルの内積を求める
				float fVecDot[6];
				for (int nCntSurfase = 0; nCntSurfase < 6; nCntSurfase++)
				{
					fVecDot[nCntSurfase] = D3DXVec3Dot(&vecNor[nCntSurfase], &vecPos[nCntSurfase]);
				}

				//ポリゴンの頂点から対象の1フレーム前の位置のベクトルを算出
				D3DXVECTOR3 vecPosOld[6];
				//奥の面
				vecPosOld[0] = posOld - vtxPos[0];
				//右の面
				vecPosOld[1] = posOld - vtxPos[1];
				//前の面
				vecPosOld[2] = posOld - vtxPos[5];
				//左の面
				vecPosOld[3] = posOld - vtxPos[4];
				//上の面
				vecPosOld[4] = posOld - vtxPos[1];
				//下の面
				vecPosOld[5] = posOld - vtxPos[7];

				//算出したベクトルと法線のベクトルの内積を求める
				float fVecDotOld[6];
				for (int nCntSurfase = 0; nCntSurfase < 6; nCntSurfase++)
				{
					fVecDotOld[nCntSurfase] = D3DXVec3Dot(&vecNor[nCntSurfase], &vecPosOld[nCntSurfase]);
				}

				//内積の計算結果がマイナスの時
				if (fVecDot[0] <= 0.0f && fVecDot[1] <= 0.0f && fVecDot[2] <= 0.0f && fVecDot[3] <= 0.0f && fVecDot[4] <= 0.0f &&
					fVecDotOld[4] >= -MODEL_SINGLE_SUBTRACTION)
				{//上の面
					//押し出す位置を求める
					D3DXVECTOR3 objectPos = pos;
					objectPos.y = ((vecNor[4].x * vecPos[4].x) - (vecNor[4].z * vecPos[4].z)) / vecNor[4].y;

					objectPos.y += pModelSingle->m_pos.y + pModelSingle->m_size.y;

					pObject->SetPos(objectPos);
					nHit = 1;
				}
				else if (fVecDot[0] <= 0.0f && fVecDot[1] <= 0.0f && fVecDot[2] <= 0.0f && fVecDot[3] <= 0.0f && fVecDot[5] <= 0.0f &&
					fVecDotOld[5] > -MODEL_SINGLE_SUBTRACTION)
				{//下の面
					D3DXVECTOR3 objectPos = pos;
					objectPos.y = ((vecNor[5].x * vecPos[5].x) - (vecNor[5].z * vecPos[5].z)) / vecNor[5].y;

					objectPos.y += pModelSingle->m_pos.y - pModelSingle->m_size.y;

					nHit = 2;
					pObject->SetPos(objectPos);
				}
				if (fVecDot[0] <= 0.0f && fVecDot[1] <= 0.0f && fVecDot[3] <= 0.0f && fVecDot[4] <= 0.0f && fVecDot[5] <= 0.0f &&
					fVecDotOld[0] > -MODEL_SINGLE_SUBTRACTION)
				{//奥の面
					//ムーブベクトルを求める
					D3DXVECTOR3 moveVec = pos - posOld;

					//ムーブベクトルと法線の内積を求める
					float fDotMoveVec = -D3DXVec3Dot(&moveVec, &vecNor[0]);

					//法線の方向に内積分伸ばしたベクトルを求める
					D3DXVECTOR3 norInVec = D3DXVECTOR3(vecNor[0].x * fDotMoveVec, vecNor[0].y * fDotMoveVec, vecNor[0].z * fDotMoveVec);

					//対象の押出先を求める
					D3DXVECTOR3 objectPos;
					objectPos = pos + norInVec;

					pObject->SetPos(objectPos);
				}
				else if (fVecDot[1] <= 0.0f && fVecDot[2] <= 0.0f && fVecDot[3] <= 0.0f && fVecDot[4] <= 0.0f && fVecDot[5] <= 0.0f &&
						 fVecDotOld[2] > -MODEL_SINGLE_SUBTRACTION)
				{//前の面
					//ムーブベクトルを求める
					D3DXVECTOR3 moveVec = pos - posOld;

					//ムーブベクトルと法線の内積を求める
					float fDotMoveVec = -D3DXVec3Dot(&moveVec, &vecNor[2]);

					//法線の方向に内積分伸ばしたベクトルを求める
					D3DXVECTOR3 norInVec = D3DXVECTOR3(vecNor[2].x * fDotMoveVec, vecNor[2].y * fDotMoveVec, vecNor[2].z * fDotMoveVec);

					//対象の押出先を求める
					D3DXVECTOR3 objectPos;
					objectPos = pos + norInVec;

					pObject->SetPos(objectPos);
				}
				if (fVecDot[0] <= 0.0f && fVecDot[1] <= 0.0f && fVecDot[2] <= 0.0f && fVecDot[4] <= 0.0f && fVecDot[5] <= 0.0f &&
					fVecDotOld[1] > -MODEL_SINGLE_SUBTRACTION)
				{//右の面
					//ムーブベクトルを求める
					D3DXVECTOR3 moveVec = pos - posOld;

					//ムーブベクトルと法線の内積を求める
					float fDotMoveVec = -D3DXVec3Dot(&moveVec, &vecNor[1]);

					//法線の方向に内積分伸ばしたベクトルを求める
					D3DXVECTOR3 norInVec = D3DXVECTOR3(vecNor[1].x * fDotMoveVec, vecNor[1].y * fDotMoveVec, vecNor[1].z * fDotMoveVec);

					//対象の押出先を求める
					D3DXVECTOR3 objectPos;
					objectPos = pos + norInVec;

					pObject->SetPos(objectPos);
				}
				else if (fVecDot[0] <= 0.0f && fVecDot[2] <= 0.0f && fVecDot[3] <= 0.0f && fVecDot[4] <= 0.0f && fVecDot[5] <= 0.0f &&
						 fVecDotOld[3] > -MODEL_SINGLE_SUBTRACTION)
				{//左の面
					//ムーブベクトルを求める
					D3DXVECTOR3 moveVec = pos - posOld;

					//ムーブベクトルと法線の内積を求める
					float fDotMoveVec = -D3DXVec3Dot(&moveVec, &vecNor[3]);

					//法線の方向に内積分伸ばしたベクトルを求める
					D3DXVECTOR3 norInVec = D3DXVECTOR3(vecNor[3].x * fDotMoveVec, vecNor[3].y * fDotMoveVec, vecNor[3].z * fDotMoveVec);

					//対象の押出先を求める
					D3DXVECTOR3 objectPos;
					objectPos = pos + norInVec;

					pObject->SetPos(objectPos);
				}
			}
		}
	}
	return nHit;
}

//================================================
//回転させていい当たり判定処理
//================================================
int CModelSingle::Collision(CObject * pObject, const float &fRadius, const float &fHeight)
{
	//どこからあたったか
	int nHit = 0;

	//オブジェクト情報を入れるポインタ
	vector<CObject*> object;

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MODEL));
	int object_size = object.size();

	for (int count_object = 0; count_object < object_size; count_object++)
	{
		if (object[count_object]->GetObjType() == CObject::OBJTYPE::MODEL)
		{
			//pObjectをCModelSingleにキャスト
			CModelSingle *pModelSingle = (CModelSingle*)object[count_object];

			//当たり判定をする設定なら
			if (pModelSingle->m_bCollision == true)
			{
				//8頂点の取得
				D3DXVECTOR3 vtxPos[MODEL_VTX];
				for (int nCntVtx = 0; nCntVtx < MODEL_VTX; nCntVtx++)
				{
					vtxPos[nCntVtx] = pModelSingle->m_pModel->GetVtxPos(nCntVtx);
				}

				//対象の幅の分大きくする
				vtxPos[0].x -= fRadius;
				vtxPos[1].x += fRadius;
				vtxPos[2].x -= fRadius;
				vtxPos[3].x += fRadius;
				vtxPos[4].x -= fRadius;
				vtxPos[5].x += fRadius;
				vtxPos[6].x -= fRadius;
				vtxPos[7].x += fRadius;
				vtxPos[0].z += fRadius;
				vtxPos[1].z += fRadius;
				vtxPos[2].z += fRadius;
				vtxPos[3].z += fRadius;
				vtxPos[4].z -= fRadius;
				vtxPos[5].z -= fRadius;
				vtxPos[6].z -= fRadius;
				vtxPos[7].z -= fRadius;
				vtxPos[2].y -= fHeight;
				vtxPos[3].y -= fHeight;
				vtxPos[6].y -= fHeight;
				vtxPos[7].y -= fHeight;

				//8頂点のワールドマトリックスを取得
				D3DXMATRIX *pVtxMtxWorld = pModelSingle->m_pModel->GetVtxMtxWorld();
				//8頂点の設定
				for (int nCntVtx = 0; nCntVtx < MODEL_VTX; nCntVtx++, pVtxMtxWorld++)
				{
					pModelSingle->m_pModel->SetVtxMtxWorld(vtxPos[nCntVtx], nCntVtx);
					vtxPos[nCntVtx] = D3DXVECTOR3(pVtxMtxWorld->_41, pVtxMtxWorld->_42, pVtxMtxWorld->_43);
				}

				//頂点から頂点までのベクトル算出用
				D3DXVECTOR3 vtxVec[6][2];
				//頂点から頂点までのベクトル算出
				//奥の面
				vtxVec[0][0] = vtxPos[0] - vtxPos[1];
				vtxVec[0][1] = vtxPos[3] - vtxPos[1];
				//右の面
				vtxVec[1][0] = vtxPos[1] - vtxPos[5];
				vtxVec[1][1] = vtxPos[7] - vtxPos[5];
				//前の面
				vtxVec[2][0] = vtxPos[5] - vtxPos[4];
				vtxVec[2][1] = vtxPos[6] - vtxPos[4];
				//左の面
				vtxVec[3][0] = vtxPos[4] - vtxPos[0];
				vtxVec[3][1] = vtxPos[2] - vtxPos[0];
				//上の面
				vtxVec[4][0] = vtxPos[1] - vtxPos[0];
				vtxVec[4][1] = vtxPos[4] - vtxPos[0];
				//下の面
				vtxVec[5][0] = vtxPos[7] - vtxPos[6];
				vtxVec[5][1] = vtxPos[2] - vtxPos[6];

				//法線保存用変数
				D3DXVECTOR3 vecNor[6];
				for (int nCntSurfase = 0; nCntSurfase < 6; nCntSurfase++)
				{
					//各面の法線を求める
					D3DXVec3Cross(&vecNor[nCntSurfase], &vtxVec[nCntSurfase][0], &vtxVec[nCntSurfase][1]);

					//ベクトルを正規化する(ベクトルの大きさを1にする)
					D3DXVec3Normalize(&vecNor[nCntSurfase], &vecNor[nCntSurfase]);
				}

				//対象の現在位置取得
				D3DXVECTOR3 pos = pObject->GetPos();

				//対象の1フレーム前の位置取得
				D3DXVECTOR3 posOld = pObject->GetPosOld();

				//頂点の位置から対象の現在の位置のベクトルを算出
				D3DXVECTOR3 vecPos[6];
				//奥の面
				vecPos[0] = pos - vtxPos[0];
				//右の面
				vecPos[1] = pos - vtxPos[1];
				//前の面
				vecPos[2] = pos - vtxPos[5];
				//左の面
				vecPos[3] = pos - vtxPos[4];
				//上の面
				vecPos[4] = pos - vtxPos[1];
				//下の面
				vecPos[5] = pos - vtxPos[7];

				//算出したベクトルと法線のベクトルの内積を求める
				float fVecDot[6];
				for (int nCntSurfase = 0; nCntSurfase < 6; nCntSurfase++)
				{
					fVecDot[nCntSurfase] = D3DXVec3Dot(&vecNor[nCntSurfase], &vecPos[nCntSurfase]);
				}

				//ポリゴンの頂点から対象の1フレーム前の位置のベクトルを算出
				D3DXVECTOR3 vecPosOld[6];
				//奥の面
				vecPosOld[0] = posOld - vtxPos[0];
				//右の面
				vecPosOld[1] = posOld - vtxPos[1];
				//前の面
				vecPosOld[2] = posOld - vtxPos[5];
				//左の面
				vecPosOld[3] = posOld - vtxPos[4];
				//上の面
				vecPosOld[4] = posOld - vtxPos[1];
				//下の面
				vecPosOld[5] = posOld - vtxPos[7];

				//算出したベクトルと法線のベクトルの内積を求める
				float fVecDotOld[6];
				for (int nCntSurfase = 0; nCntSurfase < 6; nCntSurfase++)
				{
					fVecDotOld[nCntSurfase] = D3DXVec3Dot(&vecNor[nCntSurfase], &vecPosOld[nCntSurfase]);
				}

				//内積の計算結果がマイナスの時
				if (fVecDot[0] <= 0.0f && fVecDot[1] <= 0.0f && fVecDot[2] <= 0.0f && fVecDot[3] <= 0.0f && fVecDot[4] <= 0.0f &&
					fVecDotOld[4] >= -MODEL_SINGLE_SUBTRACTION)
				{//上の面
					//押し出す位置を求める
					D3DXVECTOR3 objectPos = pos;
					objectPos.y = ((vecNor[4].x * vecPos[4].x) - (vecNor[4].z * vecPos[4].z)) / vecNor[4].y;

					objectPos.y += vtxPos[4].y;

					pObject->SetPos(objectPos);
					nHit = 1;
				}
				else if (fVecDot[0] <= 0.0f && fVecDot[1] <= 0.0f && fVecDot[2] <= 0.0f && fVecDot[3] <= 0.0f && fVecDot[5] <= 0.0f &&
					fVecDotOld[5] > -MODEL_SINGLE_SUBTRACTION)
				{//下の面
					D3DXVECTOR3 objectPos = pos;
					objectPos.y = ((vecNor[5].x * vecPos[5].x) - (vecNor[5].z * vecPos[5].z)) / vecNor[5].y;

					objectPos.y += vecPos[5].y + fHeight;

					nHit = 2;
					pObject->SetPos(objectPos);
				}
				if (fVecDot[0] <= 0.0f && fVecDot[1] <= 0.0f && fVecDot[3] <= 0.0f && fVecDot[4] <= 0.0f && fVecDot[5] <= 0.0f &&
					fVecDotOld[0] > -MODEL_SINGLE_SUBTRACTION)
				{//奥の面
				 //ムーブベクトルを求める
					D3DXVECTOR3 moveVec = pos - posOld;

					//ムーブベクトルと法線の内積を求める
					float fDotMoveVec = -D3DXVec3Dot(&moveVec, &vecNor[0]);

					//法線の方向に内積分伸ばしたベクトルを求める
					D3DXVECTOR3 norInVec = D3DXVECTOR3(vecNor[0].x * fDotMoveVec, vecNor[0].y * fDotMoveVec, vecNor[0].z * fDotMoveVec);

					//対象の押出先を求める
					D3DXVECTOR3 objectPos;
					objectPos = pos + norInVec;

					pObject->SetPos(objectPos);
				}
				else if (fVecDot[1] <= 0.0f && fVecDot[2] <= 0.0f && fVecDot[3] <= 0.0f && fVecDot[4] <= 0.0f && fVecDot[5] <= 0.0f &&
					fVecDotOld[2] > -MODEL_SINGLE_SUBTRACTION)
				{//前の面
				 //ムーブベクトルを求める
					D3DXVECTOR3 moveVec = pos - posOld;

					//ムーブベクトルと法線の内積を求める
					float fDotMoveVec = -D3DXVec3Dot(&moveVec, &vecNor[2]);

					//法線の方向に内積分伸ばしたベクトルを求める
					D3DXVECTOR3 norInVec = D3DXVECTOR3(vecNor[2].x * fDotMoveVec, vecNor[2].y * fDotMoveVec, vecNor[2].z * fDotMoveVec);

					//対象の押出先を求める
					D3DXVECTOR3 objectPos;
					objectPos = pos + norInVec;

					pObject->SetPos(objectPos);
				}
				if (fVecDot[0] <= 0.0f && fVecDot[1] <= 0.0f && fVecDot[2] <= 0.0f && fVecDot[4] <= 0.0f && fVecDot[5] <= 0.0f &&
					fVecDotOld[1] > -MODEL_SINGLE_SUBTRACTION)
				{//右の面
				 //ムーブベクトルを求める
					D3DXVECTOR3 moveVec = pos - posOld;

					//ムーブベクトルと法線の内積を求める
					float fDotMoveVec = -D3DXVec3Dot(&moveVec, &vecNor[1]);

					//法線の方向に内積分伸ばしたベクトルを求める
					D3DXVECTOR3 norInVec = D3DXVECTOR3(vecNor[1].x * fDotMoveVec, vecNor[1].y * fDotMoveVec, vecNor[1].z * fDotMoveVec);

					//対象の押出先を求める
					D3DXVECTOR3 objectPos;
					objectPos = pos + norInVec;

					pObject->SetPos(objectPos);
				}
				else if (fVecDot[0] <= 0.0f && fVecDot[2] <= 0.0f && fVecDot[3] <= 0.0f && fVecDot[4] <= 0.0f && fVecDot[5] <= 0.0f &&
					fVecDotOld[3] > -MODEL_SINGLE_SUBTRACTION)
				{//左の面
				 //ムーブベクトルを求める
					D3DXVECTOR3 moveVec = pos - posOld;

					//ムーブベクトルと法線の内積を求める
					float fDotMoveVec = -D3DXVec3Dot(&moveVec, &vecNor[3]);

					//法線の方向に内積分伸ばしたベクトルを求める
					D3DXVECTOR3 norInVec = D3DXVECTOR3(vecNor[3].x * fDotMoveVec, vecNor[3].y * fDotMoveVec, vecNor[3].z * fDotMoveVec);

					//対象の押出先を求める
					D3DXVECTOR3 objectPos;
					objectPos = pos + norInVec;

					pObject->SetPos(objectPos);
				}
			}
		}
	}
	return nHit;
}

//================================================
//ただの衝突判定
//================================================
int CModelSingle::CollisionAny(CObject *pObject)
{
	//オブジェクト情報を入れるポインタ
	vector<CObject*> object;

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MODEL));
	int object_size = object.size();

	for (int count_object = 0; count_object < object_size; count_object++)
	{
		if (object[count_object]->GetObjType() == CObject::OBJTYPE::MODEL)
		{
			//pObjectをCModelSingleにキャスト
			CModelSingle *pModelSingle = (CModelSingle*)object[count_object];

			//当たり判定をする設定なら
			if (pModelSingle->m_bCollision == true)
			{
				//8頂点の取得
				D3DXVECTOR3 vtxPos[MODEL_VTX];
				for (int nCntVtx = 0; nCntVtx < MODEL_VTX; nCntVtx++)
				{
					vtxPos[nCntVtx] = pModelSingle->m_pModel->GetVtxPos(nCntVtx);
				}

				//8頂点のワールドマトリックスを取得
				D3DXMATRIX *pVtxMtxWorld = pModelSingle->m_pModel->GetVtxMtxWorld();
				//8頂点の設定
				for (int nCntVtx = 0; nCntVtx < MODEL_VTX; nCntVtx++, pVtxMtxWorld++)
				{
					//ワールドマトリックス設定
					pModelSingle->SetMtxWorld();
					//モデルの設定
					//pModelSingle->m_pModel->SetVtxMtxWorld(vtxPos[nCntVtx], nCntVtx);
					vtxPos[nCntVtx] = D3DXVECTOR3(pVtxMtxWorld->_41, pVtxMtxWorld->_42, pVtxMtxWorld->_43);
				}


				//頂点から頂点までのベクトル算出用
				D3DXVECTOR3 vtxVec[6][2];
				//頂点から頂点までのベクトル算出
				//奥の面
				vtxVec[0][0] = vtxPos[0] - vtxPos[1];
				vtxVec[0][1] = vtxPos[3] - vtxPos[1];
				//右の面
				vtxVec[1][0] = vtxPos[1] - vtxPos[5];
				vtxVec[1][1] = vtxPos[7] - vtxPos[5];
				//前の面
				vtxVec[2][0] = vtxPos[5] - vtxPos[4];
				vtxVec[2][1] = vtxPos[6] - vtxPos[4];
				//左の面
				vtxVec[3][0] = vtxPos[4] - vtxPos[0];
				vtxVec[3][1] = vtxPos[2] - vtxPos[0];
				//上の面
				vtxVec[4][0] = vtxPos[1] - vtxPos[0];
				vtxVec[4][1] = vtxPos[4] - vtxPos[0];
				//下の面
				vtxVec[5][0] = vtxPos[7] - vtxPos[6];
				vtxVec[5][1] = vtxPos[2] - vtxPos[6];

				//法線保存用変数
				D3DXVECTOR3 vecNor[6];
				for (int nCntSurfase = 0; nCntSurfase < 6; nCntSurfase++)
				{
					//各面の法線を求める
					D3DXVec3Cross(&vecNor[nCntSurfase], &vtxVec[nCntSurfase][0], &vtxVec[nCntSurfase][1]);

					//ベクトルを正規化する(ベクトルの大きさを1にする)
					D3DXVec3Normalize(&vecNor[nCntSurfase], &vecNor[nCntSurfase]);
				}

				//対象の現在位置取得
				D3DXVECTOR3 pos = pObject->GetPos();

				//頂点の位置から対象の現在の位置のベクトルを算出
				D3DXVECTOR3 vecPos[6];
				//奥の面
				vecPos[0] = pos - vtxPos[0];
				//右の面
				vecPos[1] = pos - vtxPos[1];
				//前の面
				vecPos[2] = pos - vtxPos[5];
				//左の面
				vecPos[3] = pos - vtxPos[4];
				//上の面
				vecPos[4] = pos - vtxPos[1];
				//下の面
				vecPos[5] = pos - vtxPos[7];

				//算出したベクトルと法線のベクトルの内積を求める
				float fVecDot[6];
				for (int nCntSurfase = 0; nCntSurfase < 6; nCntSurfase++)
				{
					fVecDot[nCntSurfase] = D3DXVec3Dot(&vecNor[nCntSurfase], &vecPos[nCntSurfase]);
				}

				//全ての内積の計算結果がマイナスの時
				if (fVecDot[0] <= 0.0f && fVecDot[1] <= 0.0f && fVecDot[2] <= 0.0f &&
					fVecDot[3] <= 0.0f && fVecDot[4] <= 0.0f && fVecDot[5] <= 0.0f)
				{
					
				}
			}
		}
	}
	return 0;
}

//================================================
//ワールドマトリックス設定処理
//================================================
void CModelSingle::SetMtxWorld(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	D3DXMatrixIdentity(&m_mtxWorld);		//モデルのワールドマトリックスの初期化

											//モデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//モデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデルのワールドマトリックス設定
	m_pModel->SetMtxWorldPos();
}
