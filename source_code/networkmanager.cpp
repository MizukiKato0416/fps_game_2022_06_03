//====================================================
//
// コミュニケーションデータのヘッダーファイル(communicationdata.cpp)
// Author : 羽鳥 太一
//
//====================================================
//=============================================================================
// インクルード
//=============================================================================
#include "networkmanager.h"
#include "manager.h"
#include "tcp_client.h"
#include <thread>

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CCommunicationData CNetWorkManager::m_player_data;
vector<CCommunicationData> CNetWorkManager::m_enemy_data;
CTcpClient* CNetWorkManager::m_communication;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CNetWorkManager::CNetWorkManager()
{
	m_communication = nullptr;
	m_enemy_data.clear();
}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CNetWorkManager::~CNetWorkManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
void CNetWorkManager::Init(void)
{
	CCommunicationData buf;	// ベクター配列に入れる用

	// vector配列に入れる
	for (int cout_player = 0; cout_player < MAX_PLAYER; cout_player++)
	{
		m_enemy_data.push_back(buf);
	}

	// 初期化処理
	CTcpClient::WSASInit();
	if (m_communication == nullptr)
	{
		m_communication = new CTcpClient;
		if (m_communication != nullptr)
		{
			m_communication->Init();
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void CNetWorkManager::Uninit(void)
{
	// 終了処理
	if (m_communication != nullptr)
	{
		m_communication->Uninit();
		delete m_communication;
		m_communication = nullptr;
	}
	CTcpClient::WSASUninit();
}

//=============================================================================
// 受信処理
//=============================================================================
void CNetWorkManager::Recv(void)
{
	int recv_size = 1;

	if (m_communication != nullptr)
	{
		if (m_communication->GetConnect() == true)
		{
			while (recv_size > 0)
			{
				char recv_data[MAX_COMMU_DATA];

				CCommunicationData::COMMUNICATION_DATA *pDataBuf = new CCommunicationData::COMMUNICATION_DATA;

				recv_size = m_communication->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

				// メモリのコピー
				memcpy(pDataBuf, &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

				if (recv_size <= 0)
				{
					break;
				}
				else
				{
					// プレイヤーの情報なら
					if (pDataBuf->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER)
					{
						CCommunicationData::COMMUNICATION_DATA *data = m_player_data.GetCmmuData();

						// 情報を入れる
						*data = *pDataBuf;
					}
					// 敵の情報なら
					else if (pDataBuf->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY)
					{
						int enemy_size = m_enemy_data.size();

						for (int count_enemy = 0; count_enemy < enemy_size; count_enemy++)
						{
							CCommunicationData::COMMUNICATION_DATA *data = m_enemy_data[count_enemy].GetCmmuData();

							// 割り振られていなかったら
							if (data->Player.nNumber == 0)
							{
								// 情報を入れる
								*data = *pDataBuf;
							}
							// 割り振られていたら
							else if (data->Player.nNumber == pDataBuf->Player.nNumber)
							{
								// 情報を入れる
								*data = *pDataBuf;
							}
						}
					}
				}
			}
			CCommunicationData::COMMUNICATION_DATA *data = m_player_data.GetCmmuData();
			data->bConnect = false;
		}
	}
}