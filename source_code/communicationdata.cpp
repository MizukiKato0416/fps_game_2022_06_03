//====================================================
//
// コミュニケーションデータのヘッダーファイル(communicationdata.cpp)
// Author : 羽鳥 太一
//
//====================================================
//-------------------------------
// インクルード
//-------------------------------
#include "communicationdata.h"

//-------------------------------
// デフォルトコンストラクタ
//-------------------------------
CCommunicationData::CCommunicationData()
{
	m_CommuData.Player.nNumber = 0;
	m_CommuData.Player.Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		m_CommuData.Bullet[nCnt].Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//-------------------------------
// デフォルトデストラクタ
//-------------------------------
CCommunicationData::~CCommunicationData()
{

}