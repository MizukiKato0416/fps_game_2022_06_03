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
#define DISOLAY_ON (1)	// 表示まで

//------------------------
// グローバル変数
//------------------------
int g_room_count;	// 部屋数
int g_display_count;	// 表示するためのカウント
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

	// ソケットの入手
	sock.push_back(communication[0]->GetSocket());
	sock.push_back(communication[1]->GetSocket());
	sock.push_back(communication[2]->GetSocket());
	sock.push_back(communication[3]->GetSocket());

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
		// メモリーのコピー
		memcpy(&fds, &readfds, sizeof(fd_set));

		// ソケットの監視
		select(max_socket + 1, &fds, NULL, NULL, NULL);

		// 終了命令が来たら
		if (g_stop == "stop")
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

		// 指定した秒数に一回
		if ((g_display_count % DISOLAY_ON) == 0)
		{
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
			cout << "ルーム : " << room_num << endl;
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