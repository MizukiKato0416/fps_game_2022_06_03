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
#define FPS (60)	// fps
#define DISPLAY_ON (400000000 / FPS)	// �\���܂�
#define SEND_SOUNTER (10)	// �\���܂�

//------------------------
// �O���[�o���ϐ�
//------------------------
int g_room_count;	// ������
int g_display_count;	// �\�����邽�߂̃J�E���g
bool g_is_collision;	// ����������
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
	for (int nCnt = 0; nCnt < MAX_PLAYER + 1; nCnt++)
	{
		memcpy(&send_data[0], data[nCnt], sizeof(CCommunicationData::COMMUNICATION_DATA));
		communication[nCnt]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	}

	// �\�P�b�g�̓���
	for (int nCnt = 0; nCnt < MAX_PLAYER + 1; nCnt++)
	{
		sock.push_back(communication[nCnt]->GetSocket());
	}

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
		// �t���[���J�E���g
		g_display_count++;

		// �������[�̃R�s�[
		memcpy(&fds, &readfds, sizeof(fd_set));

		// �\�P�b�g�̊Ď�
		select(max_socket + 1, &fds, NULL, NULL, NULL);

		// �I�����߂�������
		if (g_stop == "stop")
		{
			break;
		}

		// �v���C���[����
		for (int nCntRecv = 0; nCntRecv < MAX_PLAYER + 1; nCntRecv++)
		{
			// �\�P�b�g��send����Ă�����
			if (FD_ISSET(sock[nCntRecv], &fds))
			{
				// ���V�[�u
				recv = communication[nCntRecv]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				memcpy(data[nCntRecv], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				commu_data[nCntRecv].SetCmmuData(*data[nCntRecv]);
			}
		}

		D3DXVECTOR3 ray_vec_hit;
		D3DXVECTOR3 hit_posV;
		int save_hit_enemy; 		// ���������G
		float save_differ = 100000.0f;

		// �v���C���[����
		for (int cout_player = 0; cout_player < MAX_PLAYER + 1; cout_player++)
		{
			// �e���g���Ă�Ȃ�
			if (data[cout_player]->Bullet.bUse == true)
			{
				// �v���C���[����
				for (int cout_enemy = 0; cout_enemy < MAX_PLAYER + 1; cout_enemy++)
				{
					if (cout_player != cout_enemy)
					{
						D3DXMATRIX modelInvMtx;
						D3DXMATRIX modelMtx = data[cout_enemy]->Player.ModelMatrix;
						BOOL is_hit = false;
						float differ = 0.0f;

						D3DXMatrixIdentity(&modelInvMtx);
						D3DXMatrixInverse(&modelInvMtx, NULL, &modelMtx);

						//���C���΂��������Z�o
						D3DXVECTOR3 ray_vec = data[cout_enemy]->Player.CamR - data[cout_enemy]->Player.CamV;

						//�x�N�g���𐳋K��
						D3DXVec3Normalize(&ray_vec, &ray_vec);

						D3DXVECTOR3 posV = data[cout_enemy]->Player.CamV;

						D3DXVECTOR3 endPos = posV + ray_vec;
						D3DXVec3TransformCoord(&posV, &posV, &modelInvMtx);
						D3DXVec3TransformCoord(&endPos, &endPos, &modelInvMtx);

						D3DXVECTOR3 vec = endPos - posV;

						//���C�ƃ��b�V���̓����蔻��
						if (D3DXIntersect(data[cout_enemy]->Player.Mesh, &posV, &vec, &is_hit, NULL, NULL, NULL, &differ, NULL, NULL) == D3D_OK)
						{
							//���������Ƃ�
							if (is_hit == true)
							{
								if (save_differ > differ)
								{
									//������ۑ�
									save_differ = differ;

									//���C�̕�����ۑ�
									ray_vec_hit = vec;

									//�J�����̃��[�J�����W��ۑ�
									hit_posV = posV;

									//�G�̔ԍ��ۑ�
									data[cout_player]->Bullet.nCollEnemy  = cout_enemy;
									save_hit_enemy = cout_enemy;

									// ��������
									g_is_collision = true;
								}
							}
						}
					}
				}
			}
		}

		// �������Ă���
		if (g_is_collision == true)
		{
			D3DXVec3Normalize(&ray_vec_hit, &ray_vec_hit);

			// ���C�̃x�N�g�����Z�o���������̕��L�΂�
			ray_vec_hit *= save_differ;

			// �J�����̈ʒu����L�΂����x�N�g���𑫂��ē��������ʒu���Z�o
			D3DXVECTOR3 HitPos = hit_posV + ray_vec_hit;
			D3DXMATRIX hitModelMtx = data[save_hit_enemy]->Player.ModelMatrix;
			D3DXVec3TransformCoord(&HitPos, &HitPos, &hitModelMtx);

			// �I�_��ݒ�
			//m_endPos = HitPos;
		}

		// �w�肵���b���Ɉ��
		if ((g_display_count % SEND_SOUNTER/*(DISPLAY_ON * SEND_SOUNTER)*/) == 0 ||
			g_is_collision == true)
		{
			// �X�N���[������
			system("cls");

			cout << "=======================================================" << endl;
			cout << "���[�� : " << room_num << endl;
			// �v���C���[����
			for (int nCntSend = 0; nCntSend < MAX_PLAYER + 1; nCntSend++)
			{
				cout << "Player : "<< nCntSend << "->pos" << data[nCntSend]->Player.Pos.x << " : " << data[nCntSend]->Player.Pos.y << " : " << data[nCntSend]->Player.Pos.z << endl;
				cout << "Player : " << nCntSend << "->rot" << data[nCntSend]->Player.Rot.x << " : " << data[nCntSend]->Player.Rot.y << " : " << data[nCntSend]->Player.Rot.z << endl;
			}
			cout << "=======================================================" << endl;

			// �v���C���[����
			for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
			{

				// �G����
				for (int count_send = 0; count_send < MAX_PLAYER + 1; count_send++)
				{
					// ���̃v���C���[����Ȃ�������
					if (data[count_send] != data[count_player])
					{
						// �������̃R�s�[
						memcpy(&recv_data[0], data[count_send], sizeof(CCommunicationData::COMMUNICATION_DATA));

						// send����
						communication[count_player]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
					}
				}
			}
			g_is_collision = false;
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