//================================================
//モーション読み込み処理
//Author:加藤瑞葵
//================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "motion_road.h"
#include "player.h"

//================================================
//マクロ定義
//================================================
#define MOTION_MAX_STRING	(128)	//文字列の最大値

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CMotionRoad::CMotionRoad()
{
	//モーション最大数保存用変数
	int nMotionTypeMax = 0;
	//モデルの数保存用変数
	int nModelMax = 0;

	//モーションを持っているオブジェクトの種類の数だけ回す
	for (int nCntMotionHaveType = 0; nCntMotionHaveType < MOTION_HAVE_TYPE_MAX; nCntMotionHaveType++)
	{
		//モーションを持っているオブジェクトの種類によってモーション最大数とモデル最大数を変える
		switch (nCntMotionHaveType)
		{
		case MOTION_HAVE_TYPE_PLAYER:		//プレイヤーの場合
			nMotionTypeMax = MOTION_PLAYER_TYPE_MAX;
			nModelMax = MAX_PLAYER_MODEL;
			break;
		default:
			break;
		}

		//モーションの数だけ回す
		for (int nCntMotionRoad = 0; nCntMotionRoad < nMotionTypeMax; nCntMotionRoad++)
		{
			//モーションの情報を入れる変数を動的に確保
			m_aInfo[nCntMotionHaveType].push_back({});
			//初期化
			m_aInfo[nCntMotionHaveType][nCntMotionRoad].bLoop = false;
			m_aInfo[nCntMotionHaveType][nCntMotionRoad].nNumKey = 0;

			//キーの数分回す
			for (int nCntKey = 0; nCntKey < MOTION_MAX_KEY_INFO; nCntKey++)
			{
				//初期化
				m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].nFrame = 0;

				//モデルの数分回す
				for (int nCntModel = 0; nCntModel < nModelMax; nCntModel++)
				{
					//モデルの数分必要な変数を動的確保
					m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].aKey.push_back({});

					//初期化
					m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].aKey[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].aKey[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
		}
	}
}

//================================================
//デストラクタ
//================================================
CMotionRoad::~CMotionRoad()
{

}

//================================================
//初期化処理
//================================================
HRESULT CMotionRoad::Init(void)
{
	//textファイル読み込みよう変数
	FILE *pfile = nullptr;
	//文字列保存用変数
	char cString[MOTION_MAX_STRING];
	//モーション最大数保存用変数
	int nMotionTypeMax = 0;
	//モデルの数保存用変数
	int nModelMax = 0;

	//モーションを持っているオブジェクトの種類の数だけ回す
	for (int nCntMotionHaveType = 0; nCntMotionHaveType < MOTION_HAVE_TYPE_MAX; nCntMotionHaveType++)
	{
		//モーションを持っているオブジェクトの種類によってモーション最大数、モデル最大数、読み込むファイルを変える
		switch (nCntMotionHaveType)
		{
		case MOTION_HAVE_TYPE_PLAYER:		//プレイヤーの場合
			nMotionTypeMax = MOTION_PLAYER_TYPE_MAX;
			nModelMax = MAX_PLAYER_MODEL;
			pfile = fopen("data/MOTION/motion_player.txt", "r");
		default:
			break;
		}

		//モーションの読み込み
		for (int nCntMotionRoad = 0; nCntMotionRoad < nMotionTypeMax; nCntMotionRoad++)
		{
			//一行ずつ保存
			while (fgets(cString, MOTION_MAX_STRING, pfile) != nullptr)
			{
				//文字列を保存
				fscanf(pfile, "%s", cString);
				//文字列の中にMOTIONSETがあったら
				if (strncmp("MOTIONSET", cString, 10) == 0)
				{
					//ループ情報の取得
					//キー数の取得
					fscanf(pfile, "%s%*s%d%*s%*s%*s%*s", cString, (int*)&m_aInfo[nCntMotionHaveType][nCntMotionRoad].bLoop);
					fscanf(pfile, "%s%*s%d", cString, &m_aInfo[nCntMotionHaveType][nCntMotionRoad].nNumKey);
					break;
				}
			}

			for (int nCntKey = 0; nCntKey < m_aInfo[nCntMotionHaveType][nCntMotionRoad].nNumKey; nCntKey++)
			{
				while (fgets(cString, MOTION_MAX_STRING, pfile) != nullptr)
				{
					//文字列を保存
					fscanf(pfile, "%s", cString);
					//文字列の中にFRAMEがあったら
					if (strncmp("FRAME", cString, 6) == 0)
					{
						//フレーム数の取得
						fscanf(pfile, "%s%d", cString, &m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].nFrame);
						break;
					}
				}

				for (int nCntModel = 0; nCntModel < nModelMax; nCntModel++)
				{
					while (fgets(cString, MOTION_MAX_STRING, pfile) != nullptr)
					{
						//文字列を保存
						fscanf(pfile, "%s", cString);
						//文字列の中にPOSがあったら
						if (strncmp("POS", cString, 4) == 0)
						{
							//位置、回転情報の取得
							fscanf(pfile, "%s%f%f%f", cString, &m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].aKey[nCntModel].pos.x,
								&m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].aKey[nCntModel].pos.y,
								&m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].aKey[nCntModel].pos.z);
							fscanf(pfile, "%s%*s%f%f%f%*s", cString, &m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].aKey[nCntModel].rot.x,
								&m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].aKey[nCntModel].rot.y,
								&m_aInfo[nCntMotionHaveType][nCntMotionRoad].aKeyInfo[nCntKey].aKey[nCntModel].rot.z);
							break;
						}
					}
				}

				while (fgets(cString, MOTION_MAX_STRING, pfile) != nullptr)
				{
					//文字列を保存
					fscanf(pfile, "%s", cString);
					//文字列の中にEND_KEYSETがあったら
					if (strncmp("END_KEYSET", cString, 11) == 0)
					{
						break;
					}
				}
			}

			while (fgets(cString, MOTION_MAX_STRING, pfile) != nullptr)
			{
				//文字列を保存
				fscanf(pfile, "%s", cString);
				//文字列の中にEND_MOTIONSETがあったら
				if (strncmp("END_MOTIONSET", cString, 14) == 0)
				{
					break;
				}
			}
		}
		fclose(pfile);
	}
	return S_OK;
}

//================================================
//終了処理
//================================================
void CMotionRoad::Uninit(void)
{
	
}

//================================================
//生成処理
//================================================
CMotionRoad *CMotionRoad::Create(void)
{
	//インスタンスの生成
	CMotionRoad *pMotionRoad = nullptr;
	if (pMotionRoad == nullptr)
	{
		pMotionRoad = new CMotionRoad;
		if (pMotionRoad != nullptr)
		{
			pMotionRoad->Init();
		}
	}
	return pMotionRoad;
}

//================================================
//モーション情報取得処理
//================================================
MOTION_INFO CMotionRoad::GetMotionInfo(const MOTION_HAVE_TYPE &motionHaveType, const int &nMotionType)
{
	return m_aInfo[motionHaveType][nMotionType];
}

//================================================
//モーション情報->キー情報->再生フレーム取得処理
//================================================
int CMotionRoad::GetMotionKeyInfoFrame(const MOTION_HAVE_TYPE &motionHaveType, const int &nMotionType, const int &nCntKey)
{
	return m_aInfo[motionHaveType][nMotionType].aKeyInfo[nCntKey].nFrame;
}

//================================================
//モーション情報->キー情報->位置取得処理
//================================================
D3DXVECTOR3 CMotionRoad::GetMotionKeyInfoPos(const MOTION_HAVE_TYPE &motionHaveType, const int &nMotionType, const int &nCntKey, const int &nCntModel)
{
	return m_aInfo[motionHaveType][nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].pos;
}

//================================================
//モーション情報->キー情報->回転取得処理
//================================================
D3DXVECTOR3 CMotionRoad::GetMotionKeyInfoRot(const MOTION_HAVE_TYPE &motionHaveType, const int &nMotionType, const int &nCntKey, const int &nCntModel)
{
	return m_aInfo[motionHaveType][nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].rot;
}

//================================================
//モーション情報->キー情報->位置と回転設定処理
//================================================
void CMotionRoad::SetMotionKeyInfoPosRot(const MOTION_HAVE_TYPE &motionHaveType, const int &nMotionType, const int &nCntKey, const int &nCntModel, const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot)
{
	m_aInfo[motionHaveType][nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].pos = pos;
	m_aInfo[motionHaveType][nMotionType].aKeyInfo[nCntKey].aKey[nCntModel].rot = rot;
}
