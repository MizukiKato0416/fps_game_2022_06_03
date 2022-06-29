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
		if (m_communication != nullptr)
		{
			m_communication->Init();
		}
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
		m_communication->Uninit();
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

	if (m_communication != nullptr)
	{
		if (m_communication->GetConnect() == true)
		{
			while (recv_size > 0)
			{
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
						CCommunicationData::COMMUNICATION_DATA *data = m_player_data.GetCmmuData();

						// ��������
						*data = *pDataBuf;
					}
					// �G�̏��Ȃ�
					else if (pDataBuf->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY)
					{
						int enemy_size = m_enemy_data.size();

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
				}
			}
			CCommunicationData::COMMUNICATION_DATA *data = m_player_data.GetCmmuData();
			data->bConnect = false;
		}
	}
}