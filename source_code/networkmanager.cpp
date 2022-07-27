//====================================================
//
// �R�~���j�P�[�V�����f�[�^�̃w�b�_�[�t�@�C��(communicationdata.cpp)
// Author : �H�� ����
//
//====================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "networkmanager.h"
#include "manager.h"
#include "tcp_client.h"
#include <thread>

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CCommunicationData CNetWorkManager::m_player_data;
vector<CCommunicationData> CNetWorkManager::m_enemy_data;
CTcpClient* CNetWorkManager::m_communication;
bool CNetWorkManager::m_all_connect;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CNetWorkManager::CNetWorkManager()
{
	m_communication = nullptr;
	m_enemy_data.clear();
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CNetWorkManager::~CNetWorkManager()
{
	m_communication = nullptr;
	m_enemy_data.clear();
}

//=============================================================================
// ����������
//=============================================================================
void CNetWorkManager::Init(void)
{
	CCommunicationData buf;	// �x�N�^�[�z��ɓ����p

	// vector�z��ɓ����
	for (int cout_player = 0; cout_player < MAX_PLAYER; cout_player++)
	{
		m_enemy_data.push_back(buf);
	}

	// ����������
	CTcpClient::WSASInit();
	if (m_communication == nullptr)
	{
		m_communication = new CTcpClient;
	}
}

//=============================================================================
// �I������
//=============================================================================
void CNetWorkManager::Uninit(void)
{
	// �I������
	if (m_communication != nullptr)
	{
		delete m_communication;
		m_communication = nullptr;
	}
	CTcpClient::WSASUninit();
}

//=============================================================================
// ��M����
//=============================================================================
void CNetWorkManager::Recv(void)
{
	int recv_size = 1;
	char send_data[MAX_COMMU_DATA];

	if (m_communication != nullptr)
	{
		if (m_communication->GetConnect() == true)
		{
			while (recv_size > 0)
			{
				//�ϐ�������
				//m_player_data.Init();

				char recv_data[MAX_COMMU_DATA];

				CCommunicationData::COMMUNICATION_DATA *pDataBuf = new CCommunicationData::COMMUNICATION_DATA;

				recv_size = m_communication->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

				// �������̃R�s�[
				memcpy(pDataBuf, &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

				if (recv_size <= 0)
				{
					break;
				}
				else
				{
					// �v���C���[�̏��Ȃ�
					if (pDataBuf->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER)
					{
						CCommunicationData::COMMUNICATION_DATA *player_data = m_player_data.GetCmmuData();

						// ����U���Ă��Ȃ�������
						if (player_data->Player.nNumber == 0)
						{
							// ��������
							*player_data = *pDataBuf;
							player_data->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY;
							memcpy(&send_data[0], player_data, sizeof(CCommunicationData::COMMUNICATION_DATA));
							m_communication->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
						}
						// ����U���Ă�����
						else if (pDataBuf->Player.nNumber == player_data->Player.nNumber)
						{
							// ��������
							*player_data = *pDataBuf;
						}
					}
					// �G�̏��Ȃ�
					else if (pDataBuf->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY)
					{
						int enemy_size = m_enemy_data.size();	// �T�C�Y���擾

						for (int count_enemy = 0; count_enemy < enemy_size; count_enemy++)
						{
							CCommunicationData::COMMUNICATION_DATA *data = m_enemy_data[count_enemy].GetCmmuData();

							// ����U���Ă��Ȃ�������
							if (data->Player.nNumber == 0)
							{
								// ��������
								*data = *pDataBuf;
							}
							// ����U���Ă�����
							else if (data->Player.nNumber == pDataBuf->Player.nNumber)
							{
								// ��������
								*data = *pDataBuf;
							}
						}
					}
					// �G�̏��Ȃ�
					else if (pDataBuf->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER)
					{
						CCommunicationData::COMMUNICATION_DATA *player_data = m_player_data.GetCmmuData();
						int enemy_size = m_enemy_data.size();	// �T�C�Y���擾

						// ����U���Ă��Ȃ�������
						if (player_data->Player.nNumber == 0)
						{
							// ��������
							*player_data = *pDataBuf;
							player_data->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY;
							memcpy(&send_data[0], player_data, sizeof(CCommunicationData::COMMUNICATION_DATA));
							m_communication->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
						}
						// ����U���Ă�����
						else if (pDataBuf->Player.nNumber == player_data->Player.nNumber)
						{
							// ��������
							*player_data = *pDataBuf;
						}

						for (int count_enemy = 0; count_enemy < enemy_size; count_enemy++)
						{
							recv_size = m_communication->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

							// �������̃R�s�[
							memcpy(pDataBuf, &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
							player_data = m_player_data.GetCmmuData();

							if (pDataBuf->Player.nNumber == 0)
							{
								continue;
							}
							else
							{
								if (player_data->Player.nNumber > pDataBuf->Player.nNumber)
								{
									// ��������
									m_enemy_data[pDataBuf->Player.nNumber - 1].SetCmmuData(*pDataBuf);
								}
								else if (player_data->Player.nNumber < pDataBuf->Player.nNumber)
								{
									// ��������
									m_enemy_data[pDataBuf->Player.nNumber - 2].SetCmmuData(*pDataBuf);
								}
							}

							//// ����U���Ă��Ȃ�������
							//if (data->Player.nNumber == 0)
							//{
							//	// ��������
							//	*data = *pDataBuf;
							//}
							//// ����U���Ă�����
							//else if (data->Player.nNumber == pDataBuf->Player.nNumber)
							//{
							//	// ��������
							//	*data = *pDataBuf;
							//}
						}
					}
				}
			}
			CCommunicationData::COMMUNICATION_DATA *data = m_player_data.GetCmmuData();
			data->bConnect = false;
		}
	}
}

void CNetWorkManager::Reset(void)
{
	m_player_data.Init();
	int enemy_max = m_enemy_data.size();

	for (int count_enemy = 0; count_enemy < enemy_max; count_enemy++)
	{
		m_enemy_data[count_enemy].Init();
	}
}

bool CNetWorkManager::GetAllConnect(void)
{
	vector<bool> all_connect;
	CCommunicationData::COMMUNICATION_DATA *player_data = m_player_data.GetCmmuData();

	if (player_data->bConnect == true)
	{
		all_connect.push_back(true);
	}

	int enemy_size = m_enemy_data.size();	// �T�C�Y���擾

	for (int count_enemy = 0; count_enemy < enemy_size; count_enemy++)
	{
		CCommunicationData::COMMUNICATION_DATA *data = m_enemy_data[count_enemy].GetCmmuData();

		if (data->bConnect == true)
		{
			all_connect.push_back(true);
		}
		else if (data->bConnect == false)
		{
			all_connect.push_back(false);
		}
	}

	int size = all_connect.size();

	for (int count_enemy = 0; count_enemy < size; count_enemy++)
	{
		if (all_connect[count_enemy] == true)
		{
			m_all_connect = true;
		}
		else
		{
			m_all_connect = false;
		}
	}

	return m_all_connect;
}
