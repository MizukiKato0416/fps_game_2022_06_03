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
#define TIME_OUT (50)					// �^�C���A�E�g
#define PLAYER_COL_SIZE_Y (90.0f)		//�����蔻��T�C�Y�����l
#define PLAYER_COL_RADIUS (28.0f)		//�����蔻�蔼�a
#define START_COUNTER		(100)		//�J�E���g�_�E���p�J�E���^�[

//------------------------
// �O���[�o���ϐ�
//------------------------
int g_room_count;	// ������
int g_display_count;	// �\�����邽�߂̃J�E���g
bool g_is_collision;	// ����������
LPD3DXMESH g_mesh;	// ���b�V��
vector<int> g_save_display_count[MAX_PLAYER + 1];	// �t���[���̕ۑ�
string g_stop;	// �I������psting
CTcpListener* g_listenner;	// �T�[�o�[
int g_nCountStart;			//�X�^�[�g����܂ł̃J�E���^�[
bool g_bStart;				//�X�^�[�g���Ă��邩�ǂ���

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

	// �T�[�o�[���~�߂�܂Ń��[�v
	while (true)
	{
		// �������\��
		cout << "���݂̕����� : " << g_room_count << endl;

		// �S�Ă̐l�����ʐM�҂�
		AllAcceptInit(g_listenner, g_room_count);

		/*
		DWORD dwCurrentTime;
		DWORD dwExecLastTime;
		// �t���[���J�E���g������
		dwCurrentTime = 0;
		dwExecLastTime = 0;

		// �T�[�o�[���~�߂�܂Ń��[�v
		while (true)
		{
			// �������\��
			cout << "���݂̕����� : " << g_room_count << endl;

			dwCurrentTime = timeGetTime();	// ���݂̎��Ԃ��擾

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60�b�o��
				dwExecLastTime = dwCurrentTime;	// ���݂̎��Ԃ�ۑ�

												// �S�Ă̐l�����ʐM�҂�
				AllAcceptInit(g_listenner, g_room_count);
			}

			// ���[���𑝂₷
			g_room_count++;
		}
		*/

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
	vector<CCommunicationData::COMMUNICATION_DATA> frame_lag[MAX_PLAYER + 1];	// �t���[�����ꕪ�̏��
	CCommunicationData commu_data[MAX_PLAYER + 1];	// �S�����̒ʐM�f�[�^�N���X
	CCommunicationData::COMMUNICATION_DATA *data[MAX_PLAYER + 1];	// �S�����̒ʐM�f�[�^
	D3DXVECTOR3 ray_vec_hit;	// �����������C
	D3DXVECTOR3 hit_posV;	// ��������pos
	int save_hit_enemy;	// ���������G
	int recv = 1;	// �ŏ��̃��[�v�ɓ���ׂ̏�����
	float save_differ = 100000.0f;	// ����
	char recv_data[MAX_COMMU_DATA];	// ���V�[�u�p
	char send_data[MAX_COMMU_DATA];	// �Z���h�p
	bool hit = false;	// ����������

	// select�p�ϐ��̏�����
	FD_ZERO(&readfds);

	// ���̎擾
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr] = commu_data[count_playr].GetCmmuData();
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

		//�ۑ��p
		int aCountDown[MAX_PLAYER] = { 0 };

		//�X�^�[�g���Ă��Ȃ�����
		if (!g_bStart)
		{
			//�X�^�[�g�܂ł̏���
			//�J�E���^�[�����Z
			g_nCountStart++;
		}
		// �v���C���[����
		for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
		{
			//�J�E���g�_�E����0���傫��������
			if (data[count_player]->Player.nStartCountDown > 0)
			{
				//�w��t���[���Ɉ��
				if (g_nCountStart % START_COUNTER == 0)
				{
					//�J�E���g�_�E������
					data[count_player]->Player.nStartCountDown--;
					//0�ɂ���
					g_nCountStart = 0;

					//�J�E���g�_�E����0��������
					if (data[count_player]->Player.nStartCountDown == 0)
					{
						//�X�^�[�g���Ă����Ԃɂ���
						g_bStart = true;
					}
				}
			}
			//�ۑ�
			aCountDown[count_player] = data[count_player]->Player.nStartCountDown;
		}
		

		// �v���C���[����
		for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
		{
			// �\�P�b�g��send����Ă�����
			if (FD_ISSET(sock[count_player], &fds))
			{
				//�ϐ�������
				//commu_data[count_player].Init();

				// ���V�[�u
				recv = communication[count_player]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				memcpy(data[count_player], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				commu_data[count_player].SetCmmuData(*data[count_player]);

				//�㏑��
				data[count_player]->Player.nStartCountDown = aCountDown[count_player];

				// �e���g���Ă���
				if (data[count_player]->Bullet.bUse == true)
				{
					// �t���[�����̕ۑ�
					//g_save_display_count[count_player].push_back(data[count_player]->Player.nFrameCount);
					g_save_display_count[count_player].push_back(g_display_count - 1);
					//�e�����������I�u�W�F�N�g��ۑ�
					data[count_player]->Player.type[data[count_player]->Player.nNumShot] = data[count_player]->Bullet.type;
					//�e�����������ꏊ��ۑ�
					data[count_player]->Player.HitPos[data[count_player]->Player.nNumShot] = data[count_player]->Bullet.hitPos;
					//����������1���₷
					data[count_player]->Player.nNumShot++;
				}
			}

			frame_lag[count_player].push_back(*data[count_player]);
		}

		// �w�肵���b���Ɉ��
		if ((g_display_count % SEND_COUNTER) == 0)
		{
			// ������
			D3DXVECTOR3 HitPos = { 0.0f, 0.0f, 0.0f };
			D3DXVECTOR3 EndPos = { 0.0f, 0.0f, 0.0f };
			save_differ = 100000.0f;
			g_display_count = 0;
			save_hit_enemy = -1;
			hit = false;

			// �v���C���[����
			for (int cout_player = 0; cout_player < MAX_PLAYER + 1; cout_player++)
			{
				//���񂾏�񂪑����Ă�����
				if (data[cout_player]->Player.bDeath &&
					data[cout_player]->Bullet.nGiveDamagePlayerNum > 0 && data[cout_player]->Bullet.nGiveDamagePlayerNum <= MAX_PLAYER + 1)
				{
					//�E�����G�̃L������1���₷
					data[data[cout_player]->Bullet.nGiveDamagePlayerNum - 1]->Player.nKill++;
					data[cout_player]->Player.bDeath = false;
					//�L����������̐���������
					if (data[data[cout_player]->Bullet.nGiveDamagePlayerNum - 1]->Player.nKill >= WIN_COUNTER)
					{
						//��������Ȃ�������
						if (!data[data[cout_player]->Bullet.nGiveDamagePlayerNum - 1]->Player.bWin)
						{
							//���̃v���C���[�������ɂ���
							data[data[cout_player]->Bullet.nGiveDamagePlayerNum - 1]->Player.bWin = true;
						}
					}	
				}

				// �v���C���[�̌������e�̐�
				int cout_bullet = g_save_display_count[cout_player].size();

				//���ɂ��������Ă��Ȃ��e�̐�
				int nCntBullet = 0;

				// �e�̐����̃��[�v
				for (int count_bullet = 0; count_bullet < cout_bullet; count_bullet++)
				{
					//�������擾
					save_differ = frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Bullet.fDiffer;
					// �v���C���[����
					for (int cout_enemy = 0; cout_enemy < MAX_PLAYER + 1; cout_enemy++)
					{
						// �T�C�Y���擾
						int frame_lag_size = frame_lag[cout_enemy].size();

						// �T�C�Y��0���傫��������
						/*if (frame_lag_size > 0)
						{
							
						}*/

						// �v���C���[����Ȃ������犎�G�����G��ԂłȂ�������
						if (cout_player != cout_enemy && data[cout_enemy]->Player.bInvincible == false)
						{
							D3DXMATRIX modelMtx = frame_lag[cout_enemy][g_save_display_count[cout_player][count_bullet]].Player.ModelMatrix;
							float differ = 0.0f;

							// ���C���΂��������Z�o
							D3DXVECTOR3 ray_vec = frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Player.CamR - frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Player.CamV;

							// �x�N�g���𐳋K��
							D3DXVec3Normalize(&ray_vec, &ray_vec);

							D3DXVECTOR3 posV = frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Player.CamV;
							D3DXVECTOR3 posPlayer = frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Player.Pos;

							// ���C�ƃJ�v�Z���̓����蔻��
							if (calcRayCapsule(posV.x, posV.y, posV.z,
								ray_vec.x, ray_vec.y, ray_vec.z,
								modelMtx._41, modelMtx._42, modelMtx._43,
								modelMtx._41, modelMtx._42 + PLAYER_COL_SIZE_Y, modelMtx._43,
								PLAYER_COL_RADIUS,
								HitPos.x, HitPos.y, HitPos.z,
								EndPos.x, EndPos.y, EndPos.z))
							{
								D3DXVECTOR3 hitVec = HitPos - posV;
								// �x�N�g���𐳋K��
								D3DXVec3Normalize(&hitVec, &hitVec);


								if ((ray_vec.x > 0.0f && hitVec.x < 0.0f || ray_vec.x < 0.0f && hitVec.x > 0.0f) &&
									(ray_vec.y > 0.0f && hitVec.y < 0.0f || ray_vec.y < 0.0f && hitVec.y > 0.0f) &&
									(ray_vec.z > 0.0f && hitVec.z < 0.0f || ray_vec.z < 0.0f && hitVec.z > 0.0f))
								{
									//�������Ă��Ȃ�
								}
								else
								{//�������Ă�
								 // ���������ꏊ�܂ł̋������Z�o
									D3DXVECTOR3 differVec = HitPos - posV;
									differ = D3DXVec3Length(&differVec);

									if (save_differ > differ)
									{
										// ������ۑ�
										save_differ = differ;

										//�_���[�W��ۑ�
										data[cout_enemy]->Player.nHitDamage += data[cout_player]->Bullet.nDamage;
										// �G�̔ԍ��ۑ�
										save_hit_enemy = cout_enemy;

										// ��������
										hit = true;

										//���������ꏊ��ۑ�
										data[cout_player]->Player.HitPos[count_bullet] = HitPos;
										//�v���C���[�Ƀf�[�^�𑗐M�����Ԃɂ���
										data[cout_player]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER;
										//�v���C���[�ɓ���������Ԃɂ���
										data[cout_enemy]->Player.bHit = true;
										//���������I�u�W�F�N�g��G�ɐݒ肷��
										data[cout_player]->Player.type[count_bullet] = CCommunicationData::HIT_TYPE::ENEMY;
										//���������G���v���C���[�Ƀf�[�^�𑗐M�����Ԃɂ���
										data[cout_enemy]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER;
										//���̃v���C���[���_���[�W��^�����G�����_���[�W��^�����v���C���[�̔ԍ������̃v���C���[�ɐݒ�
										data[cout_enemy]->Bullet.nGiveDamagePlayerNum = data[cout_player]->Player.nNumber;
										//���̃v���C���[���_���[�W��^�����G���������Ă����ʒu�����̃v���C���[�̈ʒu�ɐݒ肷��
										data[cout_enemy]->Bullet.hitPlayerPos = data[cout_player]->Player.Pos;
									}
								}
							}
						}

					}
					//���ɂ��������Ă��Ȃ�������
					if (data[cout_player]->Player.type[count_bullet] == CCommunicationData::HIT_TYPE::NONE)
					{
						//�������Ă��Ȃ��e�̐����{����
						nCntBullet++;
					}
				}

				// �������ĂȂ�����
				if (data[cout_player]->Player.bHit != true && nCntBullet == cout_bullet)
				{
					data[cout_player]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER;
				}
				else
				{
					//�v���C���[�Ƀf�[�^�𑗐M�����Ԃɂ���
					data[cout_player]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER;
					//�e�̐���ۑ�
					data[cout_player]->Player.nNumShot = cout_bullet;
				}
			}

			// �v���C���[����
			for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
			{
				// �v���C���[��sendo����
				if (data[count_player]->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER)
				{
					// �������̃R�s�[
					memcpy(&send_data[0], data[count_player], sizeof(CCommunicationData::COMMUNICATION_DATA));

					// send����
					communication[count_player]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				}
				// �G��sendo����
				else if (data[count_player]->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY)
				{
					// �G����
					for (int countenemy = 0; countenemy < MAX_PLAYER + 1; countenemy++)
					{
						// ���̃v���C���[����Ȃ�������
						if (countenemy != count_player)
						{
							// �������̃R�s�[
							memcpy(&send_data[0], data[countenemy], sizeof(CCommunicationData::COMMUNICATION_DATA));

							// send����
							communication[count_player]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
						}
					}
				}
				// �G��sendo����
				else if (data[count_player]->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER)
				{
					// �������̃R�s�[
					memcpy(&send_data[0], data[count_player], sizeof(CCommunicationData::COMMUNICATION_DATA));

					// send����
					communication[count_player]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

					// �G����
					for (int countenemy = 0; countenemy < MAX_PLAYER + 1; countenemy++)
					{
						// ���̃v���C���[����Ȃ�������
						if (countenemy != count_player)
						{
							// �������̃R�s�[
							memcpy(&send_data[0], data[countenemy], sizeof(CCommunicationData::COMMUNICATION_DATA));

							// send����
							communication[count_player]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
						}
					}
				}
			}

			// �X�N���[������
			system("cls");

			cout << "=======================================================" << endl;
			cout << "���[�� : " << room_num << endl;
			// �v���C���[����
			for (int nCntSend = 0; nCntSend < MAX_PLAYER + 1; nCntSend++)
			{
				cout << "Player : " << nCntSend << "->�v���C���[�̔ԍ�" << data[nCntSend]->Player.nNumber << endl;
				cout << "Player : " << nCntSend << "->�v���C���[�̈ʒu" << data[nCntSend]->Player.Pos.x << " : " << data[nCntSend]->Player.Pos.y << " : " << data[nCntSend]->Player.Pos.z << endl;
				cout << "Player : " << nCntSend << "->�v���C���[�̉�]" << data[nCntSend]->Player.Rot.x << " : " << data[nCntSend]->Player.Rot.y << " : " << data[nCntSend]->Player.Rot.z << endl;
				cout << "Player : " << nCntSend << "->�v���C���[�����蔻��t���O" << data[nCntSend]->Player.bHit << endl;
				cout << "Player : " << nCntSend << "->�v���C���[�ւ̃_���[�W" << data[nCntSend]->Player.nHitDamage << endl;
				cout << "Player : " << nCntSend << "->�����������ւ̋���" << data[nCntSend]->Bullet.fDiffer << endl;
				cout << "Player : " << nCntSend << "->�ǂ�ɓ���������" << (int)data[nCntSend]->Bullet.type << endl;
				cout << "Player : " << nCntSend << "->�e���g���Ă邩" << data[nCntSend]->Bullet.bUse << endl;
				cout << "Player : " << nCntSend << "->�L����" << data[nCntSend]->Player.nKill << endl;
				cout << "Player : " << nCntSend << "->�f�X��" << data[nCntSend]->Player.nDeath << endl;
				cout << "Player : " << nCntSend << "->�T�[�o�[����N���C�A���g�ւ�send�^�C�v" << (int)data[nCntSend]->SendType << endl;
				cout << "Player : " << nCntSend << "->�ʐM���m������Ă邩" << data[nCntSend]->bConnect << endl;
			}
			cout << "=======================================================" << endl;

			// �������ĂȂ�
			g_is_collision = false;

			for (int cout_player = 0; cout_player < MAX_PLAYER + 1; cout_player++)
			{
				frame_lag[cout_player].clear();
				g_save_display_count[cout_player].clear();
			}
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
// �K��l�����̐ڑ��҂��ƂΔԍ��̏�����
//------------------------
void AllAcceptInit(CTcpListener* listener, int room_num)
{
	vector<CCommunication*> communication;	// �ʐM�N���X
	CCommunicationData::COMMUNICATION_DATA data = {};	// ����U�邽�߂̃f�[�^
	int count_player = 0;	// �J�E���g
	char recv_data[MAX_COMMU_DATA];	// ���V�[�u�p

	while (true)
	{
		// �ʐM�҂�
		communication.push_back(listener->Accept());

		// ������
		data.Player.nNumber = count_player + 1;
		data.SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER;
		data.bConnect = true;
		

		// �������̃R�s�[
		memcpy(&recv_data[0], &data, sizeof(CCommunicationData::COMMUNICATION_DATA));

		// semd
		communication[count_player]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

		// �J�E���g�A�b�v
		count_player++;

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
	g_nCountStart = 0;
	g_bStart = false;
}

//------------------------
// �L�[���͑҂�
//------------------------
void KeyWait(void)
{
	rewind(stdin);
	getchar();
}

//------------------------
// ��P1P2P3�̓��ς��Z�o
//------------------------
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	return (x1 - x2) * (x3 - x2) + (y1 - y2) * (y3 - y2) + (z1 - z2) * (z3 - z2);
}

//------------------------
// ���C�ƃJ�v�Z���̊ђʓ_���Z�o
//------------------------
bool calcRayCapsule(float lx, float ly, float lz,
					float vx, float vy, float vz,
					float p1x, float p1y, float p1z,
					float p2x, float p2y, float p2z,
					float r,
					float &q1x, float &q1y, float &q1z,
					float &q2x, float &q2y, float &q2z)
{
	bool Q1inP1 = false;
	bool Q1inP2 = false;
	bool Q1inCld = false;

	// Q1�̌���
	if (calcRaySphere(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q1x, q1y, q1z) <= 0.0f) 
	{
		Q1inP1 = true; // Q1�͋���P1��ɂ���
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p2x, p2y, p2z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q1x, q1y, q1z) <= 0.0f) 
	{
		Q1inP2 = true; // Q1�͋���P2��ɂ���
	}
	else if (calcRayInfCilinder(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, p2x, p2y, p2z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q1x, q1y, q1z) > 0.0f &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q1x, q1y, q1z) > 0.0f)
	{
		Q1inCld = true; // Q1�͉~���ʂɂ���
	}
	else
	{
		return false; // ���C�͏Փ˂��Ă��Ȃ�
	}

	  // Q2�̌���
	float tx, ty, tz; // �_�~�[
	if (Q1inP1 && checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q1�AQ2����P1��ɂ���
		return true;
	}
	else if (Q1inP2 && checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q1�AQ2����P2��ɂ���
		return true;
	}
	else if (Q1inCld &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) > 0.0f &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) > 0.0f)
	{
		// Q1�AQ2�����~���ʂɂ���
		return true;
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, r, tx, ty, tz, q2x, q2y, q2z) == true &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q2�͋�P1��ɂ���
		return true;
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p2x, p2y, p2z, r, tx, ty, tz, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q2�͋�P2��ɂ���
		return true;
	}

	// Q2���~����ɂ��鎖���m��
	calcRayInfCilinder(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, p2x, p2y, p2z, r, tx, ty, tz, q2x, q2y, q2z);

	return true;
}

//------------------------
// ���C�Ɖ~���̊ђʓ_���Z�o
//------------------------
bool calcRayInfCilinder(float lx, float ly, float lz,
						float vx, float vy, float vz,
						float p1x, float p1y, float p1z,
						float p2x, float p2y, float p2z,
						float r,
						float &q1x, float &q1y, float &q1z,
						float &q2x, float &q2y, float &q2z) 
{
	float px = p1x - lx;
	float py = p1y - ly;
	float pz = p1z - lz;
	p2x = p2x - lx;
	p2y = p2y - ly;
	p2z = p2z - lz;
	float sx = p2x - px;
	float sy = p2y - py;
	float sz = p2z - pz;

	// �e����ϒl
	float Dvv = vx * vx + vy * vy + vz * vz;
	float Dsv = sx * vx + sy * vy + sz * vz;
	float Dpv = px * vx + py * vy + pz * vz;
	float Dss = sx * sx + sy * sy + sz * sz;
	float Dps = px * sx + py * sy + pz * sz;
	float Dpp = px * px + py * py + pz * pz;
	float rr = r * r;

	if (Dss == 0.0f)
		return false; // �~������`����Ȃ�

	float A = Dvv - Dsv * Dsv / Dss;
	float B = Dpv - Dps * Dsv / Dss;
	float C = Dpp - Dps * Dps / Dss - rr;

	if (A == 0.0f)
		return false;

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // ���C���~���ƏՓ˂��Ă��Ȃ�
	s = sqrtf(s);

	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	q1x = lx + a1 * vx;
	q1y = ly + a1 * vy;
	q1z = lz + a1 * vz;
	q2x = lx + a2 * vx;
	q2y = ly + a2 * vy;
	q2z = lz + a2 * vz;

	return true;
}

//------------------------
// lx, ly, lz : ���C�̎n�_
//------------------------
bool calcRaySphere(	float lx, float ly, float lz,
					float vx, float vy, float vz,
					float px, float py, float pz,
					float r,
					float &q1x, float &q1y, float &q1z,
					float &q2x, float &q2y, float &q2z)
{
	px = px - lx;
	py = py - ly;
	pz = pz - lz;

	float A = vx * vx + vy * vy + vz * vz;
	float B = vx * px + vy * py + vz * pz;
	float C = px * px + py * py + pz * pz - r * r;

	if (A == 0.0f)
	{
		return false; // ���C�̒�����0
	}

	float s = B * B - A * C;
	if (s < 0.0f)
	{
		return false; // �Փ˂��Ă��Ȃ�
	}

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
	{
		return false; // ���C�̔��΂ŏՓ�
	}

	q1x = lx + a1 * vx;
	q1y = ly + a1 * vy;
	q1z = lz + a1 * vz;
	q2x = lx + a2 * vx;
	q2y = ly + a2 * vy;
	q2z = lz + a2 * vz;

	return true;
}