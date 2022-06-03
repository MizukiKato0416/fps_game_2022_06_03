//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 羽鳥太一
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "edter.h"
#include "block.h"
#include "keyinput.h"
#include "manager.h"
#include "renderer.h"
#include "DxLib.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
vector<CBlock*> CEdter::m_Model;
int CEdter::m_nMaxModel;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEdter::CEdter(LAYER_TYPE Layer) : CScene(Layer)
{
	m_nMove = 0;
	m_nTimer = 0;
	m_bUse = false;
}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CEdter::~CEdter()
{

}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CEdter::Init(void)
{
	ifstream iFile;
	string FileString;
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Rot;
	D3DXVECTOR3 Scale;

	iFile.open("data/Txtdata/stagedata.txt");

	if (iFile)
	{
		int nCnt = 0;
		while (!iFile.eof())
		{
			getline(iFile, FileString);

			if (FileString != "")
			{
				// MODEL_NUMの文字列を見つけたら
				if (FileString.substr(FileString.find_first_of("MODEL_NUM"), FileString.find_last_of("MODEL_NUM") + 1) == "MODEL_NUM")
				{
					sscanf(FileString.c_str(), "%*s%*s%d", &m_nMaxModel);
					m_npModel = new int[m_nMaxModel];
				}
				// SET_MODELの文字列を見つけたら
				else if (FileString.substr(FileString.find_first_of("SET_MODEL"), FileString.find_last_of("SET_MODEL") + 1) == "SET_MODEL")
				{
					while (true)
					{
						getline(iFile, FileString);

						if (FileString != "")
						{
							// MODEL_INDXの文字列を見つけたら
							if (FileString.substr(FileString.find_first_of("MODEL_INDX"), FileString.find_last_of("MODEL_INDX")) == "MODEL_INDX")
							{
								sscanf(FileString.c_str(), "%*s%*s%d", &m_npModel[nCnt]);
							}
							// POSの文字列を見つけたら
							else if (FileString.substr(FileString.find_first_of("POS"), FileString.find_last_of("POS")) == "POS")
							{
								sscanf(FileString.c_str(), "%*s%*s%f%f%f", &Pos.x, &Pos.y, &Pos.z);
							}
							// ROTの文字列を見つけたら
							else if (FileString.substr(FileString.find_first_of("ROT"), FileString.find_last_of("ROT")) == "ROT")
							{
								sscanf(FileString.c_str(), "%*s%*s%f%f%f", &Rot.x, &Rot.y, &Rot.z);
							}
							// SCALEの文字列を見つけたら
							else if (FileString.substr(FileString.find_first_of("RSCALE"), FileString.find_last_of("RSCALE")) == "RSCALE")
							{
								sscanf(FileString.c_str(), "%*s%*s%f%f%f", &Scale.x, &Scale.y, &Scale.z);
							}
							// END_MODELSETの文字列を見つけたら
							else if (FileString.substr(FileString.find_first_of("END_MODELSET"), FileString.find_last_of("END_MODELSET") + 1) == "END_MODELSET")
							{
								m_Model.push_back(CBlock::Create(Pos, Rot, Scale, m_npModel[nCnt]));
								nCnt++;
								break;
							}
						}
					}
				}
			}
		}
	}
	else
	{
		cout << "テキストファイルがひらけませんでした。";
	}

	iFile.close();

	m_Model[0]->SetUse(true);

	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CEdter::Uninit(void)
{
	Release();
}

//=============================================================================
// ポリゴンの更新
//=============================================================================
void CEdter::Update(void)
{
	CInputKeyboard *pKey;
	pKey = CManager::GetInputKeyboard();

	UpdateModel();
	if (pKey->GetTrigger(pKey->F9) == true)
	{
		OutPut();
	}
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CEdter::Draw(void)
{

}

//=============================================================================
// テキストファイルに出力
//=============================================================================
void CEdter::OutPut(void)
{
	ofstream oFile;

	oFile.open("data/Txtdata/outstagedata.txt");

	if (oFile)
	{
		oFile << "MODEL_NUM = " << m_nMaxModel << "\n\n";
		for (int nCnt = 0; nCnt < m_nMaxModel; nCnt++)
		{
			D3DXVECTOR3 Pos = m_Model[nCnt]->GetPos();
			D3DXVECTOR3 Rot = m_Model[nCnt]->GetRot();
			D3DXVECTOR3 Scale = m_Model[nCnt]->GetScale();
			oFile << "SET_MODEL\n";
			oFile << "\tMODEL_INDX = " << m_npModel[nCnt] << "\n";
			oFile << fixed << setprecision(1);
			oFile << "\tPOS = " << Pos.x << " " << Pos.y << " " << Pos.z << "\n";
			oFile << "\tROT = " << Rot.x << " " << Rot.y << " " << Rot.z << "\n";
			oFile << "\tRSCALE = " << Scale.x << " " << Scale.y << " " << Scale.z << "\n";
			oFile << "END_MODELSET\n\n";
		}
	}
	else
	{
		cout << "テキストファイルがひらけませんでした。";
	}
	oFile.close();
}

//=============================================================================
// モデルの移動
//=============================================================================
void CEdter::UpdateModel(void)
{
	CInputKeyboard *pKey;
	pKey = CManager::GetInputKeyboard();

	for (int nCnt = 0; nCnt < m_nMaxModel; nCnt++)
	{
		if (m_Model[nCnt]->GetUse() == true)
		{
			D3DXVECTOR3 Pos = m_Model[nCnt]->GetPos();
			D3DXVECTOR3 Rot = m_Model[nCnt]->GetRot();
			D3DXVECTOR3 Scale = m_Model[nCnt]->GetScale();

			if (pKey->GetPress(pKey->LEFT) == true)
			{
				Pos.x += 1.0f * m_nMove;
				m_nTimer++;
			}
			else if (pKey->GetPress(pKey->RIGHT) == true)
			{
				Pos.x -= 1.0f * m_nMove;
				m_nTimer++;
			}
			if (pKey->GetPress(pKey->UP) == true)
			{
				Pos.z += 1.0f * m_nMove;
				m_nTimer++;
			}
			else if (pKey->GetPress(pKey->DOWN) == true)
			{
				Pos.z -= 1.0f * m_nMove;
				m_nTimer++;
			}
			if (pKey->GetPress(pKey->T) == true)
			{
				Pos.y += 1.0f * m_nMove;
				m_nTimer++;
			}
			else if (pKey->GetPress(pKey->B) == true)
			{
				Pos.y -= 1.0f * m_nMove;
				m_nTimer++;
			}
			if (pKey->GetPress(pKey->C) == true)
			{
				Scale.y += 0.01f * m_nMove;
				m_nTimer++;
			}
			else if (pKey->GetPress(pKey->Z) == true)
			{
				Scale.y -= 0.01f * m_nMove;
				m_nTimer++;
			}
			if (pKey->GetPress(pKey->Y) == true)
			{
				Scale.x += 0.01f * m_nMove;
				m_nTimer++;
			}
			else if (pKey->GetPress(pKey->N) == true)
			{
				Scale.x -= 0.01f * m_nMove;
				m_nTimer++;
			}
			if (pKey->GetPress(pKey->U) == true)
			{
				Scale.z += 0.01f * m_nMove;
				m_nTimer++;
			}
			else if (pKey->GetPress(pKey->M) == true)
			{
				Scale.z -= 0.01f * m_nMove;
				m_nTimer++;
			}
			if (pKey->GetTrigger(pKey->Q) == true)
			{
				if (nCnt != 0)
				{
					m_Model[nCnt]->SetUse(false);
					m_Model[nCnt - 1]->SetUse(true);
				}
				else
				{
					m_Model[nCnt]->SetUse(true);
				}
			}
			else if (pKey->GetTrigger(pKey->E) == true)
			{
				if (nCnt != m_nMaxModel - 1)
				{
					m_Model[nCnt]->SetUse(false);
					m_Model[nCnt + 1]->SetUse(true);
				}
				else
				{
					m_Model[nCnt]->SetUse(true);
				}
			}
			if (pKey->GetAllReleaseKey() == true)
			{
				m_nMove = 1;
			}
			if (m_nTimer >= (FPS / 4))
			{
				m_nMove++;
				m_nTimer = 0;
			}
			m_Model[nCnt]->SetPos(Pos);
			m_Model[nCnt]->SetRot(Rot);
			m_Model[nCnt]->SetScale(Scale);
		}
	}
}

//=============================================================================
// 選択モデルの入手
//=============================================================================
CBlock *CEdter::GetModel(void)
{
	for (int nCnt = 0; nCnt < m_nMaxModel; nCnt++)
	{
		if (m_Model[nCnt]->GetUse() == true)
		{
			return m_Model[nCnt];
		}
	}
	return NULL;
}