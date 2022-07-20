//=======================================================================
//カメラに関する処理
//Aouthor: 加藤　瑞葵
//=======================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "main.h"

//========================================================
//マクロ定義
//========================================================
#define MAX_CAMERA					(1)									//カメラの最大数
#define MAX_MAIN_CAMERA				(1)									//メインカメラの最大数
#define CAMERA_DISTANCE				(500.0f)							//視点と注視点の距離
#define CAMERA_DISTANCE_DEFAULT		(700.0f)							//デフォルトの視点と注視点の距離
#define CAMERA_INIT_ROT_X			(1.5f)								//カメラの向きX
#define CAMERA_INIT_POS				(D3DXVECTOR3(0.0f, 50.0f, 0.0f))	//カメラの初期位置
#define CAMERA_DEFAULT_POS			(D3DXVECTOR3(0.0f, 200.0f, 0.0f))	//カメラのデフォルト位置
#define CAMERA_MAX_POS				(D3DXVECTOR3(0.0f, 350.0f, 0.0f))	//カメラの最大位置
#define CAMERA_RADIUS				(90.0f)								//カメラの画角

//================================================
//クラスの定義
//================================================
//クラスの定義
class CCamera
{
public:
	//メンバ関数
	CCamera();				//コンストラクタ
	~CCamera();				//デストラクタ

	HRESULT Init(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const float &viewportX, const float &viewportY, const float &viewportWidth, const float &viewporHeight);
	void Uninit(void);
	void Update(void);
	static CCamera *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const float &viewportX, const float &viewportY, const float &viewportWidth, const float &viewporHeight);
	void Set(void);

	D3DXVECTOR3 GetPosV(void);					//視点取得処理
	D3DXVECTOR3 GetPosR(void);					//注視点取得処理
	void SetPosV(const D3DXVECTOR3 &pos);		//視点設定処理
	void SetPosR(const D3DXVECTOR3 &pos);		//注視点設定処理
	D3DXVECTOR3 GetRotV(void);					//視点の向き取得処理
	void SetRotV(const D3DXVECTOR3 &rot);		//視点の向き設定処理
	void SetNum(const int &nNum);				//cameraの個体識別番号設定処理
	int GetNum(void);							//cameraの個体識別番号取得処理
	//ビューポート取得処理
	D3DVIEWPORT9 *GetViewPort(void) { return &m_viewport; }
	//ビューマトリックス取得処理
	D3DXMATRIX GetViewMtx(void) { return m_mtxView; }
	//マトリックス取得処理
	D3DXMATRIX *GetMtxPoint(void) { return &m_mtxWorldCamera; }
	//視点と注視点の距離加算処理
	void AddDiffer(const float fAddDiffer) { m_fDifferVR += fAddDiffer; }
	//視点と注視点の距離設定処理
	void SetDiffer(const float fDiffer) { m_fDifferVR = fDiffer; }
	//視点と注視点の距離取得処理
	float GetDiffer(void) { return m_fDifferVR; }
	//目的の向き設定処理
	void SetRotAsk(const D3DXVECTOR3 rotAsk) { m_rotAsk = rotAsk; }
	//カメラをX軸回転させるかどうか設定処理
	void SetRotateX(const bool bRotateX) { m_bRotateX = bRotateX; }
	//カメラをY軸回転させるかどうか設定処理
	void SetRotateY(const bool bRotateY) { m_bRotateY = bRotateY; }
	//ワールド座標からスクリーン座標への変換
	D3DXVECTOR3 WorldPosToScreenPos(D3DXVECTOR3 pos);
	//画角取得処理
	float GetRadius(void) { return m_fRadius; }
	//画角設定処理
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }
	//視点固定しているかどうか設定処理
	void SetLockPosV(const bool bLockPosV) { m_bLockPosV = bLockPosV; }

private:
	//メンバ関数
	void Rotate(void);						//回転処理
	void MainCameraUpdate(void);			//メインカメラの更新処理
	void SetWorldMtx(void);					//ワールドマトリックス設定処理

	D3DXVECTOR3 m_posV;						//現在の視点
	D3DXVECTOR3 m_posVDest;					//目的の視点
	D3DXVECTOR3 m_posR;						//現在の注視点
	D3DXVECTOR3 m_posRDest;					//目的の注視点
	D3DXVECTOR3 m_vecU;						//上方向ベクトル
	D3DXVECTOR3 m_rot;						//視点の向き
	D3DXVECTOR3 m_rotAsk;					//目的の向き
	D3DXVECTOR3 m_numRot;					//向きを変える量
	float m_rotDiffer;						//目的の向きとの差
	D3DXMATRIX m_mtxProjection;				//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;					//ビューマトリックス
	D3DVIEWPORT9 m_viewport;				//ビューポート
	D3DXMATRIX m_mtxWorldCamera;			//ワールドマトリックス
	bool m_bRotateX;						//カメラX軸が回転中かどうか
	bool m_bRotateY;						//カメラY軸が回転中かどうか
	int m_nNum;								//cameraの個体識別番号
	float m_fDifferVR;						//視点と注視点の距離
	float m_fRadius;						//画角
	bool m_bLockPosV;						//視点の位置を固定するかどうか
};

#endif //!_CAMERA_H_

