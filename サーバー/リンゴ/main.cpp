//==================================================
//
// メインCPP(main.cpp)
// Author：羽鳥太一
//
//==================================================
//------------------------
// インクルード
//------------------------
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <thread>
#include <vector>
#include <assert.h>
#include "main.h"
#include "tcp_listener.h"
#include "communication.h"
#include "communicationdata.h"

//------------------------
// マクロ定義
//------------------------
#define MAX_ROOM (5)	// 最大部屋数

//------------------------
// グローバル変数
//------------------------
int g_stop;	// サーバストップ判定用
int g_accept_count;	// アクセプトした数
int g_active_surver;	// アクティブなサーバー
int	g_old_active_surver;	// 前回アクティブだったらサーバー
vector<CTcpListener*> g_listenner;	// サーバー

//------------------------
// メイン関数
//------------------------
void main(void)
{
#ifdef _DEBUG
	// デバッグ用サイズ確認
	int debug_size = sizeof(CCommunicationData::COMMUNICATION_DATA);
#endif // _DEBUG

	g_stop = 0;
	g_accept_count = 1;
	g_active_surver = 0;
	g_old_active_surver = 0;
	g_listenner.push_back(new CTcpListener);

	//------------------------
	// WSAの初期化
	//------------------------
	WSADATA  wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		cout << "初期化に失敗しました" << endl;
	}

	// サーバ停止のキーボード判定用関数のスレッド分け
	thread select_th(StopOrSurver);

	// スレッドを切り離す
	select_th.detach();

	// 最初のサーバーを立てる
	{
		g_listenner[0]->Init();

		// 部屋を作るスレッド
		thread room_create_th(CreateRoom, g_listenner[0], g_accept_count);

		// スレッドを切り離す
		room_create_th.detach();

		// 部屋数を加算
		g_accept_count++;

		// 前のサーバー
		g_old_active_surver = g_active_surver;

		// サーバーを次に
		g_active_surver++;

		// サーバーの生成
		g_listenner.push_back(new CTcpListener);
	}

	while (g_stop != 1)
	{
		// 何か入ってて初期化していなくて部屋数が最大数以下で前のサーバーが部屋を作り終えていたら
		if (g_listenner[g_active_surver] != nullptr &&
			g_listenner[g_active_surver]->GetIsInit() == false &&
			g_accept_count <= MAX_ROOM &&
			g_listenner[g_old_active_surver]->GetCreate() == true)
		{
			// 初期化処理
			g_listenner[g_active_surver]->Init();

			// 部屋を作るスレッド
			thread th(CreateRoom, g_listenner[g_active_surver], g_accept_count);

			// スレッドを切り離す
			th.detach();

			// 部屋数を加算
			g_accept_count++;

			// 前のサーバー
			g_old_active_surver = g_active_surver;

			// サーバーを次に
			g_active_surver++;

			// サーバーの生成
			g_listenner.push_back(new CTcpListener);
		}
		// サイズを取得
		int size = g_listenner.size();
		for (int count_surver = 0; count_surver < size; count_surver++)
		{
			// initしていてuninitしていたら
			if (g_listenner[count_surver]->GetIsInit() == true &&
				g_listenner[count_surver]->GetIsUninit() == true &&
				g_listenner[count_surver]->GetSocket() == INVALID_SOCKET)
			{
				// 消去
				delete g_listenner[count_surver];

				// 配列から消去
				g_listenner.erase(g_listenner.begin() + count_surver);

				// サーバーの生成
				g_listenner.push_back(new CTcpListener);

				// 前のサーバー
				g_old_active_surver = count_surver - 1;

				// サーバーを次に
				g_active_surver = size - 1;
			}
		}
	}
	// サイズを取得
	int size = g_listenner.size();
	// 全てのlistenerを削除
	for (int count_listener = 0; count_listener < size; count_listener++)
	{
		if (g_listenner[count_listener] != nullptr)
		{
			g_listenner[count_listener]->Uninit();
			delete g_listenner[count_listener];
			g_listenner[count_listener] = nullptr;
		}
	}

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
void CreateRoom(CTcpListener *listener, int room_num)
{
	fd_set fds, readfds;	// select用変数
	SOCKET maxfd, sock[MAX_PLAYER + 1];	// 監視ソケット
	CCommunication *communication[MAX_PLAYER + 1];	// 通信クラス
	CCommunicationData commu_data[MAX_PLAYER + 1];	// 全員分の通信データクラス
	CCommunicationData::COMMUNICATION_DATA *data[MAX_PLAYER + 1];	// 全員分の通信データ
	char recv_data[MAX_COMMU_DATA];	// レシーブ用
	char send_data[MAX_COMMU_DATA];	// センド用
	int recv = 1;	// 最初のループに入る為の初期化

	// select用変数の初期化
	FD_ZERO(&readfds);

	// 部屋番号の設定
	listener->SetNumber(room_num);

	// リセット
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr] = commu_data[count_playr].GetCmmuData();
	}

	// プレイヤー番号の振り分け
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr]->Player.nNumber = count_playr + 1;
	}

	// 通信待ち
	for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
	{
		communication[count_player] = listener->Accept();
		data[count_player]->bConnect = true;
	}

	if (communication[0] != nullptr &&
		communication[1] != nullptr &&
		communication[2] != nullptr &&
		communication[3] != nullptr &&
		data[0]->bConnect == true &&
		data[1]->bConnect == true &&
		data[2]->bConnect == true &&
		data[3]->bConnect == true)
	{
		// 部屋を作り終えた
		listener->SetCreate(true);
	}

	// 初期化データの送信
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
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

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

	// ソケットの入手
	sock[0] = communication[0]->GetSocket();
	sock[1] = communication[1]->GetSocket();
	sock[2] = communication[2]->GetSocket();
	sock[3] = communication[3]->GetSocket();

	for (int nCnt = 0; nCnt < MAX_PLAYER + 1; nCnt++)
	{
		// 監視ソケットの登録
		FD_SET(sock[nCnt], &readfds);
	}
	// 最大ソケットの判定
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

	// レシーブでなんも来なかったら
	while (recv > 0)
	{
		memcpy(&fds, &readfds, sizeof(fd_set));

		// ソケットの監視
		select(maxfd + 1, &fds, NULL, NULL, NULL);

		// 終了命令が来たら
		if (g_stop == 1)
		{
			break;
		}

		// プレイヤー1にsendされていたら
		if (FD_ISSET(sock[0], &fds))
		{
			recv = communication[0]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[0], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[0].SetCmmuData(*data[0]);

			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[3]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// プレイヤー2にsendされていたら
		if (FD_ISSET(sock[1], &fds))
		{
			recv = communication[1]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[1], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[1].SetCmmuData(*data[1]);

			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[3]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// プレイヤー3にsendされていたら
		if (FD_ISSET(sock[2], &fds))
		{
			recv = communication[2]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[2], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[2].SetCmmuData(*data[2]);

			communication[3]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// プレイヤー4にsendされていたら
		if (FD_ISSET(sock[3], &fds))
		{
			recv = communication[3]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[3], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[3].SetCmmuData(*data[3]);

			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}

		// スクリーン消去
		system("cls");

		// 情報表示の為の変数
		CCommunicationData::COMMUNICATION_DATA *screen_info[MAX_PLAYER + 1];
		
		// 情報を取得
		for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
		{
			screen_info[count_playr] = commu_data[count_playr].GetCmmuData();
		}
		cout << "=======================================================" << endl;
		cout << "ルーム : " << listener->GetNumber() << endl;
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

	// 終了処理
	for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
	{
		if (communication[count_player] != nullptr)
		{
			communication[count_player]->Uninit();
			delete communication[count_player];
			communication[count_player] = nullptr;
		}
	}
	if (listener != nullptr)
	{
		listener->Uninit();
	}

	// アクセプト数を減らす
	g_accept_count--;
}

//------------------------
// 選択処理
//------------------------
void StopOrSurver(void)
{
	while (g_stop != 1)
	{
		cin >> g_stop;
	}
}

//------------------------
// キー入力待ち
//------------------------
void KeyWait(void)
{
	rewind(stdin);
	getchar();
}