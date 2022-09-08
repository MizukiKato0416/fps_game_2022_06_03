//==================================================
//
// メインCPP(main.cpp)
// Author：羽鳥太一
//
//==================================================
//------------------------
// インクルード
//------------------------
#include <thread>
#include <algorithm>
#include <functional>
#include "main.h"
#include "tcp_listener.h"
#include "communication.h"
#include "communicationdata.h"

//------------------------
// マクロ定義
//------------------------
#define TIME_OUT (50)					// タイムアウト
#define PLAYER_COL_SIZE_Y (90.0f)		//当たり判定サイズ調整値
#define PLAYER_COL_RADIUS (28.0f)		//当たり判定半径
#define START_COUNTER		(100)		//カウントダウン用カウンター

//------------------------
// グローバル変数
//------------------------
int g_room_count;	// 部屋数
int g_display_count;	// 表示するためのカウント
bool g_is_collision;	// 当たったら
LPD3DXMESH g_mesh;	// メッシュ
vector<int> g_save_display_count[MAX_PLAYER + 1];	// フレームの保存
string g_stop;	// 終了判定用sting
CTcpListener* g_listenner;	// サーバー
int g_nCountStart;			//スタートするまでのカウンター
bool g_bStart;				//スタートしているかどうか

//------------------------
// メイン関数
//------------------------
void main(void)
{
	Init();
	//------------------------
	// WSAの初期化
	//------------------------
	WSADATA  wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		cout << "初期化に失敗しました" << endl;
	}

	// サーバーの生成
	g_listenner = new CTcpListener;

	// initが失敗したら
	if (g_listenner->Init() == false)
	{
		cout << "サーバーの初期化ができませんでした。" << endl;
		return;
	}

	// サーバーを止めるまでループ
	while (true)
	{
		// 部屋数表示
		cout << "現在の部屋数 : " << g_room_count << endl;

		// 全ての人数分通信待ち
		AllAcceptInit(g_listenner, g_room_count);

		/*
		DWORD dwCurrentTime;
		DWORD dwExecLastTime;
		// フレームカウント初期化
		dwCurrentTime = 0;
		dwExecLastTime = 0;

		// サーバーを止めるまでループ
		while (true)
		{
			// 部屋数表示
			cout << "現在の部屋数 : " << g_room_count << endl;

			dwCurrentTime = timeGetTime();	// 現在の時間を取得

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

												// 全ての人数分通信待ち
				AllAcceptInit(g_listenner, g_room_count);
			}

			// ルームを増やす
			g_room_count++;
		}
		*/

		// ルームを増やす
		g_room_count++;
	}

	// 削除
	g_listenner->Uninit();
	delete g_listenner;
	g_listenner = nullptr;

	//------------------------
	// 終了
	//------------------------
	cout << "終了します。"<< endl;
	cout << "何かキーを押してください。"<< endl;
	KeyWait();
	WSACleanup();
}

//------------------------
// 部屋生成
//------------------------
void CreateRoom(vector<CCommunication*> communication, int room_num)
{
	fd_set fds, readfds;	// select用変数
	vector<SOCKET> maxfd;	// 監視ソケット
	vector<SOCKET> sock;	// 監視ソケット
	vector<CCommunicationData::COMMUNICATION_DATA> frame_lag[MAX_PLAYER + 1];	// フレームずれ分の情報
	CCommunicationData commu_data[MAX_PLAYER + 1];	// 全員分の通信データクラス
	CCommunicationData::COMMUNICATION_DATA *data[MAX_PLAYER + 1];	// 全員分の通信データ
	D3DXVECTOR3 ray_vec_hit;	// 当たったレイ
	D3DXVECTOR3 hit_posV;	// 当たったpos
	int save_hit_enemy;	// 当たった敵
	int recv = 1;	// 最初のループに入る為の初期化
	float save_differ = 100000.0f;	// 距離
	char recv_data[MAX_COMMU_DATA];	// レシーブ用
	char send_data[MAX_COMMU_DATA];	// センド用
	bool hit = false;	// あったっか

	// select用変数の初期化
	FD_ZERO(&readfds);

	// 情報の取得
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr] = commu_data[count_playr].GetCmmuData();
	}

	// ソケットの入手
	for (int nCnt = 0; nCnt < MAX_PLAYER + 1; nCnt++)
	{
		sock.push_back(communication[nCnt]->GetSocket());
	}

	for (int nCnt = 0; nCnt < MAX_PLAYER + 1; nCnt++)
	{
		// 監視ソケットの登録
		FD_SET(sock[nCnt], &readfds);
	}

	// 最大ソケット判定の為にコピー
	maxfd = sock;

	// 最大ソケットの判定
	sort(maxfd.begin(), maxfd.end(), std::greater<SOCKET>{});

	// 最大ソケット
	SOCKET max_socket = maxfd[0];

	// レシーブでなんも来なかったら
	while (recv > 0)
	{
		// フレームカウント
		g_display_count++;

		// メモリーのコピー
		memcpy(&fds, &readfds, sizeof(fd_set));

		// ソケットの監視
		select(max_socket + 1, &fds, NULL, NULL, NULL);

		// 終了命令が来たら
		if (g_stop == "stop")
		{
			break;
		}

		//保存用
		int aCountDown[MAX_PLAYER] = { 0 };

		//スタートしていなったら
		if (!g_bStart)
		{
			//スタートまでの処理
			//カウンターを加算
			g_nCountStart++;
		}
		// プレイヤー分回す
		for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
		{
			//カウントダウンが0より大きかったら
			if (data[count_player]->Player.nStartCountDown > 0)
			{
				//指定フレームに一回
				if (g_nCountStart % START_COUNTER == 0)
				{
					//カウントダウンする
					data[count_player]->Player.nStartCountDown--;
					//0にする
					g_nCountStart = 0;

					//カウントダウンが0だったら
					if (data[count_player]->Player.nStartCountDown == 0)
					{
						//スタートしている状態にする
						g_bStart = true;
					}
				}
			}
			//保存
			aCountDown[count_player] = data[count_player]->Player.nStartCountDown;
		}
		

		// プレイヤー分回す
		for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
		{
			// ソケットにsendされていたら
			if (FD_ISSET(sock[count_player], &fds))
			{
				//変数初期化
				//commu_data[count_player].Init();

				// レシーブ
				recv = communication[count_player]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				memcpy(data[count_player], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				commu_data[count_player].SetCmmuData(*data[count_player]);

				//上書き
				data[count_player]->Player.nStartCountDown = aCountDown[count_player];

				// 弾を使ってたら
				if (data[count_player]->Bullet.bUse == true)
				{
					// フレーム数の保存
					//g_save_display_count[count_player].push_back(data[count_player]->Player.nFrameCount);
					g_save_display_count[count_player].push_back(g_display_count - 1);
					//弾が当たったオブジェクトを保存
					data[count_player]->Player.type[data[count_player]->Player.nNumShot] = data[count_player]->Bullet.type;
					//弾が当たった場所を保存
					data[count_player]->Player.HitPos[data[count_player]->Player.nNumShot] = data[count_player]->Bullet.hitPos;
					//撃った数を1増やす
					data[count_player]->Player.nNumShot++;
				}
			}

			frame_lag[count_player].push_back(*data[count_player]);
		}

		// 指定した秒数に一回
		if ((g_display_count % SEND_COUNTER) == 0)
		{
			// 初期化
			D3DXVECTOR3 HitPos = { 0.0f, 0.0f, 0.0f };
			D3DXVECTOR3 EndPos = { 0.0f, 0.0f, 0.0f };
			save_differ = 100000.0f;
			g_display_count = 0;
			save_hit_enemy = -1;
			hit = false;

			// プレイヤー分回す
			for (int cout_player = 0; cout_player < MAX_PLAYER + 1; cout_player++)
			{
				//死んだ情報が送られていたら
				if (data[cout_player]->Player.bDeath &&
					data[cout_player]->Bullet.nGiveDamagePlayerNum > 0 && data[cout_player]->Bullet.nGiveDamagePlayerNum <= MAX_PLAYER + 1)
				{
					//殺した敵のキル数を1増やす
					data[data[cout_player]->Bullet.nGiveDamagePlayerNum - 1]->Player.nKill++;
					data[cout_player]->Player.bDeath = false;
					//キル数が既定の数だったら
					if (data[data[cout_player]->Bullet.nGiveDamagePlayerNum - 1]->Player.nKill >= WIN_COUNTER)
					{
						//勝ちじゃなかったら
						if (!data[data[cout_player]->Bullet.nGiveDamagePlayerNum - 1]->Player.bWin)
						{
							//そのプレイヤーを勝ちにする
							data[data[cout_player]->Bullet.nGiveDamagePlayerNum - 1]->Player.bWin = true;
						}
					}	
				}

				// プレイヤーの撃った弾の数
				int cout_bullet = g_save_display_count[cout_player].size();

				//何にもあたっていない弾の数
				int nCntBullet = 0;

				// 弾の数分のループ
				for (int count_bullet = 0; count_bullet < cout_bullet; count_bullet++)
				{
					//距離を取得
					save_differ = frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Bullet.fDiffer;
					// プレイヤー分回す
					for (int cout_enemy = 0; cout_enemy < MAX_PLAYER + 1; cout_enemy++)
					{
						// サイズを取得
						int frame_lag_size = frame_lag[cout_enemy].size();

						// サイズが0より大きかったら
						/*if (frame_lag_size > 0)
						{
							
						}*/

						// プレイヤーじゃなかったら且つ敵が無敵状態でなかったら
						if (cout_player != cout_enemy && data[cout_enemy]->Player.bInvincible == false)
						{
							D3DXMATRIX modelMtx = frame_lag[cout_enemy][g_save_display_count[cout_player][count_bullet]].Player.ModelMatrix;
							float differ = 0.0f;

							// レイを飛ばす方向を算出
							D3DXVECTOR3 ray_vec = frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Player.CamR - frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Player.CamV;

							// ベクトルを正規化
							D3DXVec3Normalize(&ray_vec, &ray_vec);

							D3DXVECTOR3 posV = frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Player.CamV;
							D3DXVECTOR3 posPlayer = frame_lag[cout_player][g_save_display_count[cout_player][count_bullet]].Player.Pos;

							// レイとカプセルの当たり判定
							if (calcRayCapsule(posV.x, posV.y, posV.z,
								ray_vec.x, ray_vec.y, ray_vec.z,
								modelMtx._41, modelMtx._42, modelMtx._43,
								modelMtx._41, modelMtx._42 + PLAYER_COL_SIZE_Y, modelMtx._43,
								PLAYER_COL_RADIUS,
								HitPos.x, HitPos.y, HitPos.z,
								EndPos.x, EndPos.y, EndPos.z))
							{
								D3DXVECTOR3 hitVec = HitPos - posV;
								// ベクトルを正規化
								D3DXVec3Normalize(&hitVec, &hitVec);


								if ((ray_vec.x > 0.0f && hitVec.x < 0.0f || ray_vec.x < 0.0f && hitVec.x > 0.0f) &&
									(ray_vec.y > 0.0f && hitVec.y < 0.0f || ray_vec.y < 0.0f && hitVec.y > 0.0f) &&
									(ray_vec.z > 0.0f && hitVec.z < 0.0f || ray_vec.z < 0.0f && hitVec.z > 0.0f))
								{
									//当たっていない
								}
								else
								{//当たってる
								 // 当たった場所までの距離を算出
									D3DXVECTOR3 differVec = HitPos - posV;
									differ = D3DXVec3Length(&differVec);

									if (save_differ > differ)
									{
										// 距離を保存
										save_differ = differ;

										//ダメージを保存
										data[cout_enemy]->Player.nHitDamage += data[cout_player]->Bullet.nDamage;
										// 敵の番号保存
										save_hit_enemy = cout_enemy;

										// 当たった
										hit = true;

										//当たった場所を保存
										data[cout_player]->Player.HitPos[count_bullet] = HitPos;
										//プレイヤーにデータを送信する状態にする
										data[cout_player]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER;
										//プレイヤーに当たった状態にする
										data[cout_enemy]->Player.bHit = true;
										//当たったオブジェクトを敵に設定する
										data[cout_player]->Player.type[count_bullet] = CCommunicationData::HIT_TYPE::ENEMY;
										//当たった敵をプレイヤーにデータを送信する状態にする
										data[cout_enemy]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER;
										//このプレイヤーがダメージを与えた敵がもつダメージを与えたプレイヤーの番号をこのプレイヤーに設定
										data[cout_enemy]->Bullet.nGiveDamagePlayerNum = data[cout_player]->Player.nNumber;
										//このプレイヤーがダメージを与えた敵がもつ撃ってきた位置をこのプレイヤーの位置に設定する
										data[cout_enemy]->Bullet.hitPlayerPos = data[cout_player]->Player.Pos;
									}
								}
							}
						}

					}
					//何にもあたっていなかったら
					if (data[cout_player]->Player.type[count_bullet] == CCommunicationData::HIT_TYPE::NONE)
					{
						//当たっていない弾の数を＋する
						nCntBullet++;
					}
				}

				// 当たってなかつたら
				if (data[cout_player]->Player.bHit != true && nCntBullet == cout_bullet)
				{
					data[cout_player]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER;
				}
				else
				{
					//プレイヤーにデータを送信する状態にする
					data[cout_player]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER;
					//弾の数を保存
					data[cout_player]->Player.nNumShot = cout_bullet;
				}
			}

			// プレイヤー分回す
			for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
			{
				// プレイヤーにsendoする
				if (data[count_player]->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER)
				{
					// メモリのコピー
					memcpy(&send_data[0], data[count_player], sizeof(CCommunicationData::COMMUNICATION_DATA));

					// sendする
					communication[count_player]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				}
				// 敵にsendoする
				else if (data[count_player]->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY)
				{
					// 敵分回す
					for (int countenemy = 0; countenemy < MAX_PLAYER + 1; countenemy++)
					{
						// そのプレイヤーじゃなかったら
						if (countenemy != count_player)
						{
							// メモリのコピー
							memcpy(&send_data[0], data[countenemy], sizeof(CCommunicationData::COMMUNICATION_DATA));

							// sendする
							communication[count_player]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
						}
					}
				}
				// 敵にsendoする
				else if (data[count_player]->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER)
				{
					// メモリのコピー
					memcpy(&send_data[0], data[count_player], sizeof(CCommunicationData::COMMUNICATION_DATA));

					// sendする
					communication[count_player]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

					// 敵分回す
					for (int countenemy = 0; countenemy < MAX_PLAYER + 1; countenemy++)
					{
						// そのプレイヤーじゃなかったら
						if (countenemy != count_player)
						{
							// メモリのコピー
							memcpy(&send_data[0], data[countenemy], sizeof(CCommunicationData::COMMUNICATION_DATA));

							// sendする
							communication[count_player]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
						}
					}
				}
			}

			// スクリーン消去
			system("cls");

			cout << "=======================================================" << endl;
			cout << "ルーム : " << room_num << endl;
			// プレイヤー分回す
			for (int nCntSend = 0; nCntSend < MAX_PLAYER + 1; nCntSend++)
			{
				cout << "Player : " << nCntSend << "->プレイヤーの番号" << data[nCntSend]->Player.nNumber << endl;
				cout << "Player : " << nCntSend << "->プレイヤーの位置" << data[nCntSend]->Player.Pos.x << " : " << data[nCntSend]->Player.Pos.y << " : " << data[nCntSend]->Player.Pos.z << endl;
				cout << "Player : " << nCntSend << "->プレイヤーの回転" << data[nCntSend]->Player.Rot.x << " : " << data[nCntSend]->Player.Rot.y << " : " << data[nCntSend]->Player.Rot.z << endl;
				cout << "Player : " << nCntSend << "->プレイヤー当たり判定フラグ" << data[nCntSend]->Player.bHit << endl;
				cout << "Player : " << nCntSend << "->プレイヤーへのダメージ" << data[nCntSend]->Player.nHitDamage << endl;
				cout << "Player : " << nCntSend << "->当たった物への距離" << data[nCntSend]->Bullet.fDiffer << endl;
				cout << "Player : " << nCntSend << "->どれに当たったか" << (int)data[nCntSend]->Bullet.type << endl;
				cout << "Player : " << nCntSend << "->弾を使ってるか" << data[nCntSend]->Bullet.bUse << endl;
				cout << "Player : " << nCntSend << "->キル数" << data[nCntSend]->Player.nKill << endl;
				cout << "Player : " << nCntSend << "->デス数" << data[nCntSend]->Player.nDeath << endl;
				cout << "Player : " << nCntSend << "->サーバーからクライアントへのsendタイプ" << (int)data[nCntSend]->SendType << endl;
				cout << "Player : " << nCntSend << "->通信が確立されてるか" << data[nCntSend]->bConnect << endl;
			}
			cout << "=======================================================" << endl;

			// 当たってない
			g_is_collision = false;

			for (int cout_player = 0; cout_player < MAX_PLAYER + 1; cout_player++)
			{
				frame_lag[cout_player].clear();
				g_save_display_count[cout_player].clear();
			}
		}
	}

	// 部屋数を減らす
	g_room_count--;

	// 終了処理
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
// 規定人数分の接続待ちとば番号の初期化
//------------------------
void AllAcceptInit(CTcpListener* listener, int room_num)
{
	vector<CCommunication*> communication;	// 通信クラス
	CCommunicationData::COMMUNICATION_DATA data = {};	// 割り振るためのデータ
	int count_player = 0;	// カウント
	char recv_data[MAX_COMMU_DATA];	// レシーブ用

	while (true)
	{
		// 通信待ち
		communication.push_back(listener->Accept());

		// 初期化
		data.Player.nNumber = count_player + 1;
		data.SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER;
		data.bConnect = true;
		

		// メモリのコピー
		memcpy(&recv_data[0], &data, sizeof(CCommunicationData::COMMUNICATION_DATA));

		// semd
		communication[count_player]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

		// カウントアップ
		count_player++;

		// 通信待ちを抜けても接続数が増えてないかソケットが初期値なら
		if (communication.size() >= MAX_PLAYER + 1)
		{
			break;
		}
	}

	// 部屋毎のスレッド
	thread room_communication_th(CreateRoom, communication, room_num);

	// 切り離す
	room_communication_th.detach();
}

//------------------------
// 初期化処理
//------------------------
void Init(void)
{
#ifdef _DEBUG
	// デバッグ用サイズ確認
	int debug_size = sizeof(CCommunicationData::COMMUNICATION_DATA);
#endif // _DEBUG

	g_stop.clear();
	g_room_count = 1;
	g_listenner = nullptr;
	g_nCountStart = 0;
	g_bStart = false;
}

//------------------------
// キー入力待ち
//------------------------
void KeyWait(void)
{
	rewind(stdin);
	getchar();
}

//------------------------
// ∠P1P2P3の内積を算出
//------------------------
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	return (x1 - x2) * (x3 - x2) + (y1 - y2) * (y3 - y2) + (z1 - z2) * (z3 - z2);
}

//------------------------
// レイとカプセルの貫通点を算出
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

	// Q1の検査
	if (calcRaySphere(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q1x, q1y, q1z) <= 0.0f) 
	{
		Q1inP1 = true; // Q1は球面P1上にある
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p2x, p2y, p2z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q1x, q1y, q1z) <= 0.0f) 
	{
		Q1inP2 = true; // Q1は球面P2上にある
	}
	else if (calcRayInfCilinder(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, p2x, p2y, p2z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q1x, q1y, q1z) > 0.0f &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q1x, q1y, q1z) > 0.0f)
	{
		Q1inCld = true; // Q1は円柱面にある
	}
	else
	{
		return false; // レイは衝突していない
	}

	  // Q2の検査
	float tx, ty, tz; // ダミー
	if (Q1inP1 && checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q1、Q2共球P1上にある
		return true;
	}
	else if (Q1inP2 && checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q1、Q2共球P2上にある
		return true;
	}
	else if (Q1inCld &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) > 0.0f &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) > 0.0f)
	{
		// Q1、Q2共球円柱面にある
		return true;
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, r, tx, ty, tz, q2x, q2y, q2z) == true &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q2は球P1上にある
		return true;
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p2x, p2y, p2z, r, tx, ty, tz, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q2は球P2上にある
		return true;
	}

	// Q2が円柱上にある事が確定
	calcRayInfCilinder(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, p2x, p2y, p2z, r, tx, ty, tz, q2x, q2y, q2z);

	return true;
}

//------------------------
// レイと円柱の貫通点を算出
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

	// 各種内積値
	float Dvv = vx * vx + vy * vy + vz * vz;
	float Dsv = sx * vx + sy * vy + sz * vz;
	float Dpv = px * vx + py * vy + pz * vz;
	float Dss = sx * sx + sy * sy + sz * sz;
	float Dps = px * sx + py * sy + pz * sz;
	float Dpp = px * px + py * py + pz * pz;
	float rr = r * r;

	if (Dss == 0.0f)
		return false; // 円柱が定義されない

	float A = Dvv - Dsv * Dsv / Dss;
	float B = Dpv - Dps * Dsv / Dss;
	float C = Dpp - Dps * Dps / Dss - rr;

	if (A == 0.0f)
		return false;

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // レイが円柱と衝突していない
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
// lx, ly, lz : レイの始点
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
		return false; // レイの長さが0
	}

	float s = B * B - A * C;
	if (s < 0.0f)
	{
		return false; // 衝突していない
	}

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
	{
		return false; // レイの反対で衝突
	}

	q1x = lx + a1 * vx;
	q1y = ly + a1 * vy;
	q1z = lz + a1 * vz;
	q2x = lx + a2 * vx;
	q2y = ly + a2 * vy;
	q2z = lz + a2 * vz;

	return true;
}