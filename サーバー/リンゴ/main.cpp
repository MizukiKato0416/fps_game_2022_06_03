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
#define TIME_OUT (50)	// タイムアウト
#define FPS (60)	// fps
#define DISPLAY_ON (400000000 / FPS)	// 表示まで
#define SEND_SOUNTER (10)	// 表示まで

//------------------------
// グローバル変数
//------------------------
int g_room_count;	// 部屋数
int g_display_count;	// 表示するためのカウント
bool g_is_collision;	// 当たったら
string g_stop;	// 終了判定用sting
CTcpListener* g_listenner;	// サーバー

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

	// カウントアップ用
	thread count_up(CountUp);

	// 切り離す
	count_up.detach();

	// サーバーを止めるまでループ
	while (true)
	{
		// 部屋数表示
		cout << "現在の部屋数 : " << g_room_count << endl;

		// 全ての人数分通信待ち
		AllAccept(g_listenner, g_room_count);

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
	CCommunicationData commu_data[MAX_PLAYER + 1];	// 全員分の通信データクラス
	CCommunicationData::COMMUNICATION_DATA *data[MAX_PLAYER + 1];	// 全員分の通信データ
	char recv_data[MAX_COMMU_DATA];	// レシーブ用
	char send_data[MAX_COMMU_DATA];	// センド用
	int recv = 1;	// 最初のループに入る為の初期化

	// select用変数の初期化
	FD_ZERO(&readfds);

	// 情報の取得
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr] = commu_data[count_playr].GetCmmuData();
	}

	// 情報の初期化
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr]->Player.nNumber = count_playr + 1;
		data[count_playr]->bConnect = true;
	}

	// 初期化データの送信
	for (int nCnt = 0; nCnt < MAX_PLAYER + 1; nCnt++)
	{
		memcpy(&send_data[0], data[nCnt], sizeof(CCommunicationData::COMMUNICATION_DATA));
		communication[nCnt]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
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

		// プレイヤー分回す
		for (int nCntRecv = 0; nCntRecv < MAX_PLAYER + 1; nCntRecv++)
		{
			// ソケットにsendされていたら
			if (FD_ISSET(sock[nCntRecv], &fds))
			{
				// レシーブ
				recv = communication[nCntRecv]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				memcpy(data[nCntRecv], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				commu_data[nCntRecv].SetCmmuData(*data[nCntRecv]);
			}
		}

		D3DXVECTOR3 ray_vec_hit;
		D3DXVECTOR3 hit_posV;
		int save_hit_enemy; 		// 当たった敵
		float save_differ = 100000.0f;

		// プレイヤー分回す
		for (int cout_player = 0; cout_player < MAX_PLAYER + 1; cout_player++)
		{
			// 弾を使ってるなら
			if (data[cout_player]->Bullet.bUse == true)
			{
				// プレイヤー分回す
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

						//レイを飛ばす方向を算出
						D3DXVECTOR3 ray_vec = data[cout_enemy]->Player.CamR - data[cout_enemy]->Player.CamV;

						//ベクトルを正規化
						D3DXVec3Normalize(&ray_vec, &ray_vec);

						D3DXVECTOR3 posV = data[cout_enemy]->Player.CamV;

						D3DXVECTOR3 endPos = posV + ray_vec;
						D3DXVec3TransformCoord(&posV, &posV, &modelInvMtx);
						D3DXVec3TransformCoord(&endPos, &endPos, &modelInvMtx);

						D3DXVECTOR3 vec = endPos - posV;

						//レイとメッシュの当たり判定
						if (D3DXIntersect(data[cout_enemy]->Player.Mesh, &posV, &vec, &is_hit, NULL, NULL, NULL, &differ, NULL, NULL) == D3D_OK)
						{
							//当たったとき
							if (is_hit == true)
							{
								if (save_differ > differ)
								{
									//距離を保存
									save_differ = differ;

									//レイの方向を保存
									ray_vec_hit = vec;

									//カメラのローカル座標を保存
									hit_posV = posV;

									//敵の番号保存
									data[cout_player]->Bullet.nCollEnemy  = cout_enemy;
									save_hit_enemy = cout_enemy;

									// 当たった
									g_is_collision = true;
								}
							}
						}
					}
				}
			}
		}

		// 当たってたら
		if (g_is_collision == true)
		{
			D3DXVec3Normalize(&ray_vec_hit, &ray_vec_hit);

			// レイのベクトルを算出した距離の分伸ばす
			ray_vec_hit *= save_differ;

			// カメラの位置から伸ばしたベクトルを足して当たった位置を算出
			D3DXVECTOR3 HitPos = hit_posV + ray_vec_hit;
			D3DXMATRIX hitModelMtx = data[save_hit_enemy]->Player.ModelMatrix;
			D3DXVec3TransformCoord(&HitPos, &HitPos, &hitModelMtx);

			// 終点を設定
			//m_endPos = HitPos;
		}

		// 指定した秒数に一回
		if ((g_display_count % SEND_SOUNTER/*(DISPLAY_ON * SEND_SOUNTER)*/) == 0 ||
			g_is_collision == true)
		{
			// スクリーン消去
			system("cls");

			cout << "=======================================================" << endl;
			cout << "ルーム : " << room_num << endl;
			// プレイヤー分回す
			for (int nCntSend = 0; nCntSend < MAX_PLAYER + 1; nCntSend++)
			{
				cout << "Player : "<< nCntSend << "->pos" << data[nCntSend]->Player.Pos.x << " : " << data[nCntSend]->Player.Pos.y << " : " << data[nCntSend]->Player.Pos.z << endl;
				cout << "Player : " << nCntSend << "->rot" << data[nCntSend]->Player.Rot.x << " : " << data[nCntSend]->Player.Rot.y << " : " << data[nCntSend]->Player.Rot.z << endl;
			}
			cout << "=======================================================" << endl;

			// プレイヤー分回す
			for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
			{

				// 敵分回す
				for (int count_send = 0; count_send < MAX_PLAYER + 1; count_send++)
				{
					// そのプレイヤーじゃなかったら
					if (data[count_send] != data[count_player])
					{
						// メモリのコピー
						memcpy(&recv_data[0], data[count_send], sizeof(CCommunicationData::COMMUNICATION_DATA));

						// sendする
						communication[count_player]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
					}
				}
			}
			g_is_collision = false;
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
// 規定人数分の接続待ち
//------------------------
void AllAccept(CTcpListener* listener, int room_num)
{
	vector<CCommunication*> communication;	// 通信クラス

	while (true)
	{
		// 通信待ち
		communication.push_back(listener->Accept());

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
// カウントアップ用
//------------------------
void CountUp(void)
{
	while (true)
	{
		// カウント加算
		g_display_count++;
	}
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
}

//------------------------
// キー入力待ち
//------------------------
void KeyWait(void)
{
	rewind(stdin);
	getchar();
}