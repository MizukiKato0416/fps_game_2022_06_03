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
#define SEND_COUNTER (10)	// 表示まで

//------------------------
// グローバル変数
//------------------------
int g_room_count;	// 部屋数
int g_display_count;	// 表示するためのカウント
bool g_is_collision;	// 当たったら
LPD3DXMESH g_mesh;	// メッシュ
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
	D3DXVECTOR3 ray_vec_hit;	// 当たったレイ
	D3DXVECTOR3 hit_posV;	// 当たったpos
	int save_hit_enemy;	// 当たった敵
	int recv = 1;	// 最初のループに入る為の初期化
	float save_differ = 100000.0f;	// 距離
	char recv_data[MAX_COMMU_DATA];	// レシーブ用
	char send_data[MAX_COMMU_DATA];	// センド用

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
		data[count_playr]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER;
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

		// 初期化
		save_differ = 100000.0f;

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
						D3DXVECTOR3 ray_vec = data[cout_player]->Player.CamR - data[cout_player]->Player.CamV;

						//ベクトルを正規化
						D3DXVec3Normalize(&ray_vec, &ray_vec);

						D3DXVECTOR3 posV = data[cout_player]->Player.CamV;

						D3DXVECTOR3 endPos = posV + ray_vec;
						//D3DXVec3TransformCoord(&posV, &posV, &modelInvMtx);
						//D3DXVec3TransformCoord(&endPos, &endPos, &modelInvMtx);

						D3DXVECTOR3 vec = endPos - posV;
						D3DXVECTOR3 HitPos = { 0.0f, 0.0f, 0.0f };
						D3DXVECTOR3 EndPos = { 0.0f, 0.0f, 0.0f };
						//レイとカプセルの当たり判定
						if (calcRayCapsule(posV.x, posV.y, posV.z, ray_vec.x, ray_vec.x, ray_vec.x, modelMtx._41, modelMtx._42, modelMtx._43,
							               modelMtx._41, modelMtx._42 + 150.0f, modelMtx._43, 100.0f, HitPos.x, HitPos.y, HitPos.z, 
							               EndPos.x, EndPos.y, EndPos.z))
						{
							int nCnt = 0;
						}

						/*//レイとメッシュの当たり判定
						if (D3DXIntersect(data[cout_enemy]->Player.Mesh, &posV, &vec, &is_hit, NULL, NULL, NULL, &differ, NULL, NULL) == D3D_OK)
						{
							//当たったとき
							if (is_hit == TRUE)
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
									data[cout_enemy]->Player.bHit = true;
									data[cout_enemy]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER;
								}
							}
						}*/
					}
				}
			}
			if (data[cout_player]->Player.bHit != true)
			{
				data[cout_player]->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY;
			}
		}

		// 当たってたら
		/*if (g_is_collision == true)
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
		}*/
		// 指定した秒数に一回
		if ((g_display_count % SEND_COUNTER/*(DISPLAY_ON * SEND_SOUNTER)*/) == 0 ||
			g_is_collision == true)
		{
			// プレイヤー分回す
			for (int count_player = 0; count_player < MAX_PLAYER + 1; count_player++)
			{
				// プレイヤーにsendoする
				if (data[count_player]->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_PLAYER)
				{
					// メモリのコピー
					memcpy(&recv_data[0], data[count_player], sizeof(CCommunicationData::COMMUNICATION_DATA));

					// sendする
					communication[count_player]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
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
							memcpy(&recv_data[0], data[countenemy], sizeof(CCommunicationData::COMMUNICATION_DATA));

							// sendする
							communication[count_player]->Send(&recv_data[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
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
				cout << "Player : "<< nCntSend << "->pos" << data[nCntSend]->Player.Pos.x << " : " << data[nCntSend]->Player.Pos.y << " : " << data[nCntSend]->Player.Pos.z << endl;
				cout << "Player : " << nCntSend << "->rot" << data[nCntSend]->Player.Rot.x << " : " << data[nCntSend]->Player.Rot.y << " : " << data[nCntSend]->Player.Rot.z << endl;
			}
			cout << "=======================================================" << endl;

			// 当たってない
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

// ∠P1P2P3の内積を算出
// x1, y1, z1: 点P1
// x2, y2, z2: 点P2
// x3, y3, z3: 点P3
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
	return (x1 - x2) * (x3 - x2) + (y1 - y2) * (y3 - y2) + (z1 - z2) * (z3 - z2);
}

// レイとカプセルの貫通点を算出
// lx, ly, lz : レイの始点
// vx, vy, vz : レイの方向ベクトル
// p1x, p1y, p1z: カプセル軸の端点P1
// p2x, p2y, p2z: カプセル軸の端点P2
// r : カプセルの半径
// q1x, q1y, q1z: 貫通開始点（戻り値）
// q2x, q2y, q2z: 貫通終了点（戻り値）
bool calcRayCapsule(
	float lx, float ly, float lz,
	float vx, float vy, float vz,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z
) {
	bool Q1inP1 = false;
	bool Q1inP2 = false;
	bool Q1inCld = false;

	// Q1の検査
	if (
		calcRaySphere(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q1x, q1y, q1z) <= 0.0f
		) {
		Q1inP1 = true; // Q1は球面P1上にある

	}
	else if (
		calcRaySphere(lx, ly, lz, vx, vy, vz, p2x, p2y, p2z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q1x, q1y, q1z) <= 0.0f
		) {
		Q1inP2 = true; // Q1は球面P2上にある

	}
	else if (
		calcRayInfCilinder(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, p2x, p2y, p2z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q1x, q1y, q1z) > 0.0f &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q1x, q1y, q1z) > 0.0f
		) {
		Q1inCld = true; // Q1は円柱面にある

	}
	else
		return false; // レイは衝突していない

					  // Q2の検査
	float tx, ty, tz; // ダミー
	if (Q1inP1 && checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) <= 0.0f) {
		// Q1、Q2共球P1上にある
		return true;

	}
	else if (Q1inP2 && checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) <= 0.0f) {
		// Q1、Q2共球P2上にある
		return true;

	}
	else if (
		Q1inCld &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) > 0.0f &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) > 0.0f
		) {
		// Q1、Q2共球円柱面にある
		return true;

	}
	else if (
		calcRaySphere(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, r, tx, ty, tz, q2x, q2y, q2z) == true &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) <= 0.0f
		) {
		// Q2は球P1上にある
		return true;

	}
	else if (
		calcRaySphere(lx, ly, lz, vx, vy, vz, p2x, p2y, p2z, r, tx, ty, tz, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) <= 0.0f
		) {
		// Q2は球P2上にある
		return true;

	}

	// Q2が円柱上にある事が確定
	calcRayInfCilinder(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, p2x, p2y, p2z, r, tx, ty, tz, q2x, q2y, q2z);

	return true;
}

// lx, ly, lz : レイの始点
// vx, vy, vz : レイの方向ベクトル
// p1x, p1y, p1z: 円柱軸の1点
// p2x, p2y, p2z: 円柱軸のもう1点
// r : 円柱の半径
// q1x, q1y, q1z: 貫通開始点（戻り値）
// q2x, q2y, q2z: 貫通終了点（戻り値）

bool calcRayInfCilinder(
	float lx, float ly, float lz,
	float vx, float vy, float vz,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z
) {
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

// lx, ly, lz : レイの始点
// vx, vy, vz : レイの方向ベクトル
// px, py, pz : 球の中心点の座標
// r : 球の半径
// q1x, q1y, q1z: 衝突開始点（戻り値）
// q2x, q2y, q2z: 衝突終了点（戻り値）

bool calcRaySphere(
	float lx, float ly, float lz,
	float vx, float vy, float vz,
	float px, float py, float pz,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z
) {
	px = px - lx;
	py = py - ly;
	pz = pz - lz;

	float A = vx * vx + vy * vy + vz * vz;
	float B = vx * px + vy * py + vz * pz;
	float C = px * px + py * py + pz * pz - r * r;

	if (A == 0.0f)
		return false; // レイの長さが0

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // 衝突していない

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
		return false; // レイの反対で衝突

	q1x = lx + a1 * vx;
	q1y = ly + a1 * vy;
	q1z = lz + a1 * vz;
	q2x = lx + a2 * vx;
	q2y = ly + a2 * vy;
	q2z = lz + a2 * vz;

	return true;
}