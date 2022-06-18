//==================================================
//
// ���C��CPP(main.cpp)
// Author�F�H������
//
//==================================================
//------------------------
// �C���N���[�h
//------------------------
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <thread>
#include <vector>
#include "main.h"
#include "tcp_listener.h"
#include "communication.h"
#include "communicationdata.h"

using namespace std;

//------------------------
// �}�N����`
//------------------------
#define MAX_ROOM (5)	// �ő啔����

//------------------------
// �O���[�o���ϐ�
//------------------------
int g_stop = 0;	// �T�[�o�X�g�b�v����p
int g_accept_count = 1;	// �A�N�Z�v�g������
bool g_accept = false;	// ���̃A�N�Z�v�g�ɂ����邩�ǂ���

//------------------------
// ���C���֐�
//------------------------
void main(void)
{
#ifdef _DEBUG
	// �f�o�b�O�p�T�C�Y�m�F
	int debug_size = sizeof(CCommunicationData::COMMUNICATION_DATA);
#endif // _DEBUG
	//------------------------
	// WSA�̏�����
	//------------------------
	WSADATA  wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		cout << "�������Ɏ��s���܂���" << endl;
	}

	// �T�[�o�̐���
	vector<CTcpListener*> pListenner;
	pListenner.push_back(new CTcpListener);

	// �T�[�o��~�̃L�[�{�[�h����p�֐��̃X���b�h����
	thread th(StopOrSurver);

	th.detach();

	while (g_stop != 1)
	{
		// �T�[�o�[�̃T�C�Y���擾
		int size = pListenner.size();
		for (int count_listener = 0; count_listener < size; count_listener++)
		{
			// init���Ă��Ȃ�������
			if (pListenner[count_listener]->GetIsInit() == false)
			{
				// ���������Ă�init������������
				if (pListenner[count_listener] != nullptr && pListenner[count_listener]->Init() == true)
				{
					// �������������Ȃ����A�N�Z�v�g�o������
					if (g_accept_count <= MAX_ROOM && g_accept == false)
					{
						// �A�N�Z�v�g�X���b�h
						thread th(Accept, pListenner[count_listener], g_accept_count);

						// �A�N�Z�v�g���o���Ȃ�����
						g_accept = true;

						th.detach();

						// �A�N�Z�v�g���̃J�E���g
						g_accept_count++;

						// �T�[�o�̐���
						pListenner.push_back(new CTcpListener);
					}
				}
			}
			// init�������uninit����Ă�����
			else if (pListenner[count_listener]->GetIsInit() == true &&
					 pListenner[count_listener]->GetIsUninit() == true)
			{
				// ��������Ă��ĒʐM���ؒf����Ă���listener���폜
				pListenner.erase(pListenner.begin() + count_listener);
				int re_size = pListenner.size();
				for (int count_listener = 0; count_listener < re_size; count_listener++)
				{
					// �o�^����Ă��镔���ԍ���-1����
					int listener_num = pListenner[count_listener]->GetNumber();
					pListenner[count_listener]->SetNumber(listener_num - 1);
				}
			}
		}
	}
	// �T�C�Y���擾
	int size = pListenner.size();
	// �S�Ă�listener���폜
	for (int count_listener = 0; count_listener < size; count_listener++)
	{
		if (pListenner[count_listener] != nullptr)
		{
			pListenner[count_listener]->Uninit();
			delete pListenner[count_listener];
			pListenner[count_listener] = nullptr;
		}
	}

	//------------------------
	// �I��
	//------------------------
	cout << "�I�����܂��B"<< endl;
	cout << "�����L�[�������Ă��������B"<< endl;
	KeyWait();
	WSACleanup();
}

//------------------------
// �ڑ��҂�
//------------------------
void Accept(CTcpListener *listener, int room_num)
{
	// �z�X�g�p�N���X
	CCommunication *communication;

	// �ʐM�҂�
	communication = listener->Accept();

	// �����ԍ��̐ݒ�
	listener->SetNumber(room_num);

	// �������̏����̃X���b�h����
	thread th(CreateRoom, listener, communication);

	th.detach();
}

//------------------------
// ��������
//------------------------
void CreateRoom(CTcpListener *listener, CCommunication *player_01)
{
	fd_set fds, readfds;	// select�p�ϐ�
	SOCKET maxfd, sock[MAX_PLAYER + 1];	// �Ď��\�P�b�g
	CCommunication *communication[MAX_PLAYER];	// �z�X�g�ȊO�̒ʐM�N���X
	CCommunicationData commu_data[MAX_PLAYER + 1];	// �S�����̒ʐM�f�[�^�N���X
	CCommunicationData::COMMUNICATION_DATA *data[MAX_PLAYER + 1];	// �S�����̒ʐM�f�[�^
	char recv_data[MAX_COMMU_DATA];	// ���V�[�u�p
	char send_data[MAX_COMMU_DATA];	// �Z���h�p
	int recv = 1;	// �ŏ��̃��[�v�ɓ���ׂ̏�����

	// select�p�ϐ��̏�����
	FD_ZERO(&readfds);

	// ���Z�b�g
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		commu_data[count_playr].GetCmmuData()->Rest();
		data[count_playr] = commu_data[count_playr].GetCmmuData();
	}

	// �v���C���[�ԍ��̐U�蕪��
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr]->Player.nNumber = count_playr + 1;
	}

	// �z�X�g�͂����ڑ����Ă���̂�true��
	data[0] = commu_data[0].GetCmmuData();
	data[0]->bConnect = true;

	// �z�X�g�ȊO�̒ʐM�҂�
	for (int count_player = 0; count_player < MAX_PLAYER; count_player++)
	{
		communication[count_player] = listener->Accept();
		data[count_player + 1]->bConnect = true;
	}

	// �������f�[�^�̑��M
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	player_01->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[1], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[0]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[2], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[1]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[3], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	// �z�X�g�A�z�X�g�ȊO�̒ʐM���m�������̂Ń}���`�X���b�h�ŒʐM���ł����Ԃ�
	g_accept = false;

	// �\�P�b�g�̓���
	sock[0] = player_01->GetSocket();
	sock[1] = communication[0]->GetSocket();
	sock[2] = communication[1]->GetSocket();
	sock[3] = communication[2]->GetSocket();

	for (int nCnt = 0; nCnt < MAX_PLAYER + 1; nCnt++)
	{
		// �Ď��\�P�b�g�̓o�^
		FD_SET(sock[nCnt], &readfds);
	}
	// �ő�\�P�b�g�̔���
	if (sock[0] > sock[1] &&
		sock[0] > sock[2] &&
		sock[0] > sock[3])
	{
		maxfd = sock[0];
	}
	else if (sock[1] > sock[0] &&
			 sock[1] > sock[2] &&
			 sock[1] > sock[3])
	{
		maxfd = sock[1];
	}
	else if (sock[2] > sock[0] &&
			 sock[2] > sock[1] &&
			 sock[2] > sock[3])
	{
		maxfd = sock[2];
	}
	else if (sock[3] > sock[0] &&
			 sock[3] > sock[1] &&
			 sock[3] > sock[2])
	{
		maxfd = sock[3];
	}

	// ���V�[�u�łȂ�����Ȃ�������
	while (recv > 0)
	{
		memcpy(&fds, &readfds, sizeof(fd_set));

		// �\�P�b�g�̊Ď�
		select(maxfd + 1, &fds, NULL, NULL, NULL);

		// �I�����߂�������
		if (g_stop == 1)
		{
			break;
		}

		// �v���C���[1��send����Ă�����
		if (FD_ISSET(sock[0], &fds))
		{
			recv = player_01->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[0], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[0].SetCmmuData(*data[0]);

			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[2��send����Ă�����
		if (FD_ISSET(sock[1], &fds))
		{
			recv = communication[0]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[1], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[1].SetCmmuData(*data[1]);

			player_01->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[3��send����Ă�����
		if (FD_ISSET(sock[2], &fds))
		{
			recv = communication[1]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[2], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[2].SetCmmuData(*data[2]);

			player_01->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[4��send����Ă�����
		if (FD_ISSET(sock[3], &fds))
		{
			recv = communication[2]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[3], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[3].SetCmmuData(*data[3]);

			player_01->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}

		// �X�N���[������
		system("cls");

		// ���\���ׂ̈̕ϐ�
		CCommunicationData::COMMUNICATION_DATA *screen_info[MAX_PLAYER + 1];
		
		// �����擾
		for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
		{
			screen_info[count_playr] = commu_data[count_playr].GetCmmuData();
		}
		cout << "=======================================================" << endl;
		cout << "���[�� : " << listener->GetNumber() << endl;
		cout << "Player : 1->pos" << screen_info[0]->Player.Pos.x <<" : " << screen_info[0]->Player.Pos.y <<" : " << screen_info[0]->Player.Pos.z << endl;
		cout << "Player : 1->rot" << screen_info[0]->Player.Rot.x <<" : " << screen_info[0]->Player.Rot.y <<" : " << screen_info[0]->Player.Rot.z << endl;
		cout << "Player : 2->pos" << screen_info[1]->Player.Pos.x <<" : " << screen_info[1]->Player.Pos.y <<" : " << screen_info[1]->Player.Pos.z << endl;
		cout << "Player : 2->rot" << screen_info[1]->Player.Rot.x <<" : " << screen_info[1]->Player.Rot.y <<" : " << screen_info[1]->Player.Rot.z << endl;
		cout << "Player : 3->pos" << screen_info[2]->Player.Pos.x <<" : " << screen_info[2]->Player.Pos.y <<" : " << screen_info[2]->Player.Pos.z << endl;
		cout << "Player : 3->rot" << screen_info[2]->Player.Rot.x <<" : " << screen_info[2]->Player.Rot.y <<" : " << screen_info[2]->Player.Rot.z << endl;
		cout << "Player : 4->pos" << screen_info[3]->Player.Pos.x <<" : " << screen_info[3]->Player.Pos.y <<" : " << screen_info[3]->Player.Pos.z << endl;
		cout << "Player : 4->rot" << screen_info[3]->Player.Rot.x <<" : " << screen_info[3]->Player.Rot.y <<" : " << screen_info[3]->Player.Rot.z << endl;
		cout << "=======================================================" << endl;
	}

	// �I������
	if (player_01 != nullptr)
	{
		player_01->Uninit();
		delete player_01;
	}
	for (int count_player = 0; count_player < MAX_PLAYER; count_player++)
	{
		if (communication[count_player] != nullptr)
		{
			communication[count_player]->Uninit();
			delete communication[count_player];
		}
	}

	// �A�N�Z�v�g�������炷
	g_accept_count--;
}

//------------------------
// �I������
//------------------------
void StopOrSurver(void)
{
	while (g_stop != 1)
	{
		cin >> g_stop;
	}
}

//------------------------
// �L�[���͑҂�
//------------------------
void KeyWait(void)
{
	rewind(stdin);
	getchar();
}