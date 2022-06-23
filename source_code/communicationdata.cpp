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
	m_CommuData.bConnect = false;
	m_CommuData.Player.fMotionSpeed = 0.0f;
	m_CommuData.Player.nNumber = 0;
	m_CommuData.Player.Pos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.Rot = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.BigenPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.EndPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.nCollEnemy = 0;
	m_CommuData.Bullet.bUse = false;
}

//-------------------------------
// デフォルトデストラクタ
//-------------------------------
CCommunicationData::~CCommunicationData()
{

}