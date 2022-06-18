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
#include "main.h"
#include "tcp_listener.h"
#include "communication.h"
#include "communicationdata.h"

using namespace std;

//------------------------
// マクロ定義
//------------------------
#define MAX_ROOM (5)	// 最大部屋数

//------------------------
// グローバル変数
//------------------------
int g_stop = 0;	// サーバストップ判定用
int g_accept_count = 1;	// アクセプトした数
bool g_accept = false;	// 次のアクセプトにいけるかどうか

//------------------------
// メイン関数
//------------------------
void main(void)
{
#ifdef _DEBUG
	// デバッグ用サイズ確認
	int debug_size = sizeof(CCommunicationData::COMMUNICATION_DATA);
#endif // _DEBUG
	//------------------------
	// WSAの初期化
	//------------------------
	WSADATA  wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		cout << "初期化に失敗しました" << endl;
	}

	// サーバの生成
	vector<CTcpListener*> pListenner;
	pListenner.push_back(new CTcpListener);

	// サーバ停止のキーボード判定用関数のスレッド分け
	thread th(StopOrSurver);

	th.detach();

	while (g_stop != 1)
	{
		// サーバーのサイズを取得
		int size = pListenner.size();
		for (int count_listener = 0; count_listener < size; count_listener++)
		{
			// initしていなかったら
			if (pListenner[count_listener]->GetIsInit() == false)
			{
				// 何か入っててinitが成功したら
				if (pListenner[count_listener] != nullptr && pListenner[count_listener]->Init() == true)
				{
					// 部屋が上限じゃない且アクセプト出来たら
					if (g_accept_count <= MAX_ROOM && g_accept == false)
					{
						// アクセプトスレッド
						thread th(Accept, pListenner[count_listener], g_accept_count);

						// アクセプトを出来なくする
						g_accept = true;

						th.detach();

						// アクセプト数のカウント
						g_accept_count++;

						// サーバの生成
						pListenner.push_back(new CTcpListener);
					}
				}
			}
			// initした後でuninitされていたら
			else if (pListenner[count_listener]->GetIsInit() == true &&
					 pListenner[count_listener]->GetIsUninit() == true)
			{
				// 生成されていて通信が切断されているlistenerを削除
				pListenner.erase(pListenner.begin() + count_listener);
				int re_size = pListenner.size();
				for (int count_listener = 0; count_listener < re_size; count_listener++)
				{
					// 登録されている部屋番号を-1する
					int listener_num = pListenner[count_listener]->GetNumber();
					pListenner[count_listener]->SetNumber(listener_num - 1);
				}
			}
		}
	}
	// サイズを取得
	int size = pListenner.size();
	// 全てのlistenerを削除
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
	// 終了
	//------------------------
	cout << "終了します。"<< endl;
	cout << "何かキーを押してください。"<< endl;
	KeyWait();
	WSACleanup();
}

//------------------------
// 接続待ち
//------------------------
void Accept(CTcpListener *listener, int room_num)
{
	// ホスト用クラス
	CCommunication *communication;

	// 通信待ち
	communication = listener->Accept();

	// 部屋番号の設定
	listener->SetNumber(room_num);

	// 部屋事の処理のスレッド分け
	thread th(CreateRoom, listener, communication);

	th.detach();
}

//------------------------
// 部屋生成
//------------------------
void CreateRoom(CTcpListener *listener, CCommunication *player_01)
{
	fd_set fds, readfds;	// select用変数
	SOCKET maxfd, sock[MAX_PLAYER + 1];	// 監視ソケット
	CCommunication *communication[MAX_PLAYER];	// ホスト以外の通信クラス
	CCommunicationData commu_data[MAX_PLAYER + 1];	// 全員分の通信データクラス
	CCommunicationData::COMMUNICATION_DATA *data[MAX_PLAYER + 1];	// 全員分の通信データ
	char recv_data[MAX_COMMU_DATA];	// レシーブ用
	char send_data[MAX_COMMU_DATA];	// センド用
	int recv = 1;	// 最初のループに入る為の初期化

	// select用変数の初期化
	FD_ZERO(&readfds);

	// リセット
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		commu_data[count_playr].GetCmmuData()->Rest();
		data[count_playr] = commu_data[count_playr].GetCmmuData();
	}

	// プレイヤー番号の振り分け
	for (int count_playr = 0; count_playr < MAX_PLAYER + 1; count_playr++)
	{
		data[count_playr]->Player.nNumber = count_playr + 1;
	}

	// ホストはもう接続しているのでtrueに
	data[0] = commu_data[0].GetCmmuData();
	data[0]->bConnect = true;

	// ホスト以外の通信待ち
	for (int count_player = 0; count_player < MAX_PLAYER; count_player++)
	{
		communication[count_player] = listener->Accept();
		data[count_player + 1]->bConnect = true;
	}

	// 初期化データの送信
	memcpy(&send_data[0], data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	player_01->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[1], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[0]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[2], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[1]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	memcpy(&send_data[0], data[3], sizeof(CCommunicationData::COMMUNICATION_DATA));
	communication[2]->Send(&send_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));

	// ホスト、ホスト以外の通信が確立したのでマルチスレッドで通信ができる状態へ
	g_accept = false;

	// ソケットの入手
	sock[0] = player_01->GetSocket();
	sock[1] = communication[0]->GetSocket();
	sock[2] = communication[1]->GetSocket();
	sock[3] = communication[2]->GetSocket();

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
			recv = player_01->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[0], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[0].SetCmmuData(*data[0]);

			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// プレイヤー2にsendされていたら
		if (FD_ISSET(sock[1], &fds))
		{
			recv = communication[0]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[1], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[1].SetCmmuData(*data[1]);

			player_01->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// プレイヤー3にsendされていたら
		if (FD_ISSET(sock[2], &fds))
		{
			recv = communication[1]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[2], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[2].SetCmmuData(*data[2]);

			player_01->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[2]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
		// プレイヤー4にsendされていたら
		if (FD_ISSET(sock[3], &fds))
		{
			recv = communication[2]->Recv(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(data[3], &recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			commu_data[3].SetCmmuData(*data[3]);

			player_01->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[0]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			communication[1]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
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