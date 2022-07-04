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

	// ���̏�����
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr]->Player.nNumber = count_playr + 1;
		data[count_playr]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER;
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
		for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
		{
			// �\�P�b�g��send����Ă�����
			if (FD_ISSET(sock[count_player], &fds))
			{
				// ���V�[�u
				recv = communication[count_player]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				memcpy(data[count_player], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				commu_data[count_player].SetCmmuData(*data[count_player]);
				frame_lag[count_player].push_back(*data[count_player]);

				// �e���g���Ă���
				if (data[count_player]->Bullet.bUse == true)
				{
					// �t���[�����̕ۑ�
					g_save_display_count[count_player].push_back(data[count_player]->Player.nFrameCount);
				}
			}
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
				// �v���C���[�̌������e�̐�
				int cout_bullet = g_save_display_count[cout_player].size();

				// �e�̐����̃��[�v
				for (int count_bullet = 0; count_bullet < cout_bullet; count_bullet++)
				{
					// �v���C���[����
					for (int cout_enemy = 0; cout_enemy < MAX_PLAYER + 1; cout_enemy++)
					{
						// �v���C���[����Ȃ�������
						if (cout_player != cout_enemy)
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
							if (calcRayCapsule(	posV.x, posV.y, posV.z,
												ray_vec.x, ray_vec.y, ray_vec.z,
												modelMtx._41, modelMtx._42, modelMtx._43,
												modelMtx._41, modelMtx._42 + 90.0f, modelMtx._43,
												28.0f,
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

										// �G�̔ԍ��ۑ�
										frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Bullet.nCollEnemy = cout_enemy + 1;
										save_hit_enemy = cout_enemy;

										// ��������
										hit = true;
									}
								}
							}
						}
					}
					// �N�����瓖�ĂĂ���
					if (hit == true)
					{
						data[save_hit_enemy]->Bullet.HitPos = HitPos;
						data[save_hit_enemy]->Player.bHit = true;
						data[save_hit_enemy]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER;
						hit = false;
					}
				}
				// �������ĂȂ�����
				if (data[cout_player]->Player.bHit != true)
				{
					data[cout_player]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY;
				}
			}

			// �v���C���[����
			for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
			{
				// �v���C���[��sendo����
				if (data[count_player]->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER)
				{
					// �������̃R�s�[
					memcpy(&recv_data[0], data[count_player], sizeof(CCommunicationData::COMMUNICATION_DATA));

					// send����
					communication[count_player]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
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
							memcpy(&recv_data[0], data[countenemy], sizeof(CCommunicationData::COMMUNICATION_DATA));

							// send����
							communication[count_player]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
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
				cout << "Player : "<< nCntSend << "->pos" << data[nCntSend]->Player.Pos.x << " : " << data[nCntSend]->Player.Pos.y << " : " << data[nCntSend]->Player.Pos.z << endl;
				cout << "Player : " << nCntSend << "->rot" << data[nCntSend]->Player.Rot.x << " : " << data[nCntSend]->Player.Rot.y << " : " << data[nCntSend]->Player.Rot.z << endl;
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