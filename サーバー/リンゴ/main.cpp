//==================================================
//
// ���C��CPP(main.cpp)
// Author�F�H������
//
//==================================================
//------------------------
// �C���N���[�h
//------------------------
#include <thread>
#include <algorithm>
#include <functional>
#include "main.h"
#include "tcp_listener.h"
#include "communication.h"
#include "communicationdata.h"

//------------------------
// �}�N����`
//------------------------
#define TIME_OUT (50)	// �^�C���A�E�g
#define DISOLAY_ON (1)	// �\���܂�

//------------------------
// �O���[�o���ϐ�
//------------------------
int g_room_count;	// ������
int g_display_count;	// �\�����邽�߂̃J�E���g
string g_stop;	// �I������psting
CTcpListener* g_listenner;	// �T�[�o�[

//------------------------
// ���C���֐�
//------------------------
void main(void)
{
	Init();
	//------------------------
	// WSA�̏�����
	//------------------------
	WSADATA  wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		cout << "�������Ɏ��s���܂���" << endl;
	}

	// �T�[�o�[�̐���
	g_listenner = new CTcpListener;

	// init�����s������
	if (g_listenner->Init() == false)
	{
		cout << "�T�[�o�[�̏��������ł��܂���ł����B" << endl;
		return;
	}

	// �J�E���g�A�b�v�p
	thread count_up(CountUp);

	// �؂藣��
	count_up.detach();

	// �T�[�o�[���~�߂�܂Ń��[�v
	while (true)
	{
		// �������\��
		cout << "���݂̕����� : " << g_room_count << endl;

		// �S�Ă̐l�����ʐM�҂�
		AllAccept(g_listenner, g_room_count);

		// ���[���𑝂₷
		g_room_count++;
	}

	// �폜
	g_listenner->Uninit();
	delete g_listenner;
	g_listenner = nullptr;

	//------------------------
	// �I��
	//------------------------
	cout << "�I�����܂��B"<< endl;
	cout << "�����L�[�������Ă��������B"<< endl;
	KeyWait();
	WSACleanup();
}

//------------------------
// ��������
//------------------------
void CreateRoom(vector<CCommunication*> communication, int room_num)
{
	fd_set fds, readfds;	// select�p�ϐ�
	vector<SOCKET> maxfd;	// �Ď��\�P�b�g
	vector<SOCKET> sock;	// �Ď��\�P�b�g
	CCommunicationData commu_data[MAX_PLAYER + 1];	// �S�����̒ʐM�f�[�^�N���X
	CCommunicationData::COMMUNICATION_DATA *data[MAX_PLAYER + 1];	// �S�����̒ʐM�f�[�^
	char recv_data[MAX_COMMU_DATA];	// ���V�[�u�p
	char send_data[MAX_COMMU_DATA];	// �Z���h�p
	int recv = 1;	// �ŏ��̃��[�v�ɓ���ׂ̏�����

	// select�p�ϐ��̏�����
	FD_ZERO(&readfds);

	// ���̎擾
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr] = commu_data[count_playr].GetCmmuData();
	}

	// ���̏�����
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr]->Player.nNumber = count_playr + 1;
		data[count_playr]->bConnect = true;
	}

	// �������f�[�^�̑��M
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[0]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[1], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[0]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[2], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[0]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[3], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[0]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[1], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[1]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[2], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[1]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[3], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[1]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[1]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[2], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[3], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[1], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[3], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[3]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[3]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[1], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[3]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[2], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[3]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	// �\�P�b�g�̓���
	sock.push_back(communication[0]->GetSocket());
	sock.push_back(communication[1]->GetSocket());
	sock.push_back(communication[2]->GetSocket());
	sock.push_back(communication[3]->GetSocket());

	for (int nCnt = 0; nCnt < MAX_PLAYER + 1; nCnt++)
	{
		// �Ď��\�P�b�g�̓o�^
		FD_SET(sock[nCnt], &readfds);
	}

	// �ő�\�P�b�g����ׂ̈ɃR�s�[
	maxfd = sock;

	// �ő�\�P�b�g�̔���
	sort(maxfd.begin(), maxfd.end(), std::greater<SOCKET>{});

	// �ő�\�P�b�g
	SOCKET max_socket = maxfd[0];

	// ���V�[�u�łȂ�����Ȃ�������
	while (recv > 0)
	{
		// �������[�̃R�s�[
		memcpy(&fds, &readfds, sizeof(fd_set));

		// �\�P�b�g�̊Ď�
		select(max_socket + 1, &fds, NULL, NULL, NULL);

		// �I�����߂�������
		if (g_stop == "stop")
		{
			break;
		}

		// �v���C���[1��send����Ă�����
		if (FD_ISSET(sock[0], &fds))
		{
			recv = communication[0]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[0], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[0].SetCmmuData(*data[0]);

			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[3]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[2��send����Ă�����
		if (FD_ISSET(sock[1], &fds))
		{
			recv = communication[1]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[1], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[1].SetCmmuData(*data[1]);

			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[3]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[3��send����Ă�����
		if (FD_ISSET(sock[2], &fds))
		{
			recv = communication[2]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[2], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[2].SetCmmuData(*data[2]);

			communication[3]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// �v���C���[4��send����Ă�����
		if (FD_ISSET(sock[3], &fds))
		{
			recv = communication[3]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[3], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[3].SetCmmuData(*data[3]);

			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}

		// �w�肵���b���Ɉ��
		if ((g_display_count % DISOLAY_ON) == 0)
		{
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
			cout << "���[�� : " << room_num << endl;
			cout << "Player : 1->pos" << screen_info[0]->Player.Pos.x << " : " << screen_info[0]->Player.Pos.y << " : " << screen_info[0]->Player.Pos.z << endl;
			cout << "Player : 1->rot" << screen_info[0]->Player.Rot.x << " : " << screen_info[0]->Player.Rot.y << " : " << screen_info[0]->Player.Rot.z << endl;
			cout << "Player : 2->pos" << screen_info[1]->Player.Pos.x << " : " << screen_info[1]->Player.Pos.y << " : " << screen_info[1]->Player.Pos.z << endl;
			cout << "Player : 2->rot" << screen_info[1]->Player.Rot.x << " : " << screen_info[1]->Player.Rot.y << " : " << screen_info[1]->Player.Rot.z << endl;
			cout << "Player : 3->pos" << screen_info[2]->Player.Pos.x << " : " << screen_info[2]->Player.Pos.y << " : " << screen_info[2]->Player.Pos.z << endl;
			cout << "Player : 3->rot" << screen_info[2]->Player.Rot.x << " : " << screen_info[2]->Player.Rot.y << " : " << screen_info[2]->Player.Rot.z << endl;
			cout << "Player : 4->pos" << screen_info[3]->Player.Pos.x << " : " << screen_info[3]->Player.Pos.y << " : " << screen_info[3]->Player.Pos.z << endl;
			cout << "Player : 4->rot" << screen_info[3]->Player.Rot.x << " : " << screen_info[3]->Player.Rot.y << " : " << screen_info[3]->Player.Rot.z << endl;
			cout << "=======================================================" << endl;
		}
	}

	// �����������炷
	g_room_count--;

	// �I������
	int size = communication.size();
	for (int count_player = 0; count_player < size; count_player++)
	{
		if (communication[count_player] != nullptr)
		{
			communication[count_player]->Uninit();
			delete communication[count_player];
			communication[count_player] = nullptr;
		}
	}
}

//------------------------
// �K��l�����̐ڑ��҂�
//------------------------
void AllAccept(CTcpListener* listener, int room_num)
{
	vector<CCommunication*> communication;	// �ʐM�N���X

	while (true)
	{
		// �ʐM�҂�
		communication.push_back(listener->Accept());

		// �ʐM�҂��𔲂��Ă��ڑ����������ĂȂ����\�P�b�g�������l�Ȃ�
		if (communication.size() >= MAX_PLAYER + 1)
		{
			break;
		}
	}

	// �������̃X���b�h
	thread room_communication_th(CreateRoom, communication, room_num);

	// �؂藣��
	room_communication_th.detach();
}

//------------------------
// �J�E���g�A�b�v�p
//------------------------
void CountUp(void)
{
	while (true)
	{
		// �J�E���g���Z
		g_display_count++;
	}
}

//------------------------
// ����������
//------------------------
void Init(void)
{
#ifdef _DEBUG
	// �f�o�b�O�p�T�C�Y�m�F
	int debug_size = sizeof(CCommunicationData::COMMUNICATION_DATA);
#endif // _DEBUG

	g_stop.clear();
	g_room_count = 1;
	g_listenner = nullptr;
}

//------------------------
// �L�[���͑҂�
//------------------------
void KeyWait(void)
{
	rewind(stdin);
	getchar();
}