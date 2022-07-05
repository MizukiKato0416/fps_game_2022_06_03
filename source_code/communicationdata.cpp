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
	sizeof(COMMUNICATION_DATA);
	m_CommuData.Player.Pos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.Rot = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.CamV = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.CamR = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.HitPos = { 0.0f, 0.0f, 0.0f };
	D3DXMatrixIdentity(&m_CommuData.Player.ModelMatrix);
	m_CommuData.Player.nNumber = 0;
	m_CommuData.Player.nFrameCount = 0;
	m_CommuData.Player.nHitDamage = 0;
	m_CommuData.Player.fMotionSpeed = 0.0f;
	memset(m_CommuData.Player.aMotion, 0, sizeof(m_CommuData.Player.aMotion));
	m_CommuData.Player.bHit = false;
	m_CommuData.Bullet.Pos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.type = HIT_TYPE::NONE;
	m_CommuData.Bullet.nDamage = 0.0f;
	m_CommuData.Bullet.fDiffer = 0.0f;
	m_CommuData.Bullet.bUse = false;
	m_CommuData.Ballistic.BigenPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Ballistic.EndPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Ballistic.Rot = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Ballistic.Size = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Ballistic.fSpeed = 0.0f;
	m_CommuData.SendType = COMMUNICATION_TYPE::SEND_TO_ENEMY;
	m_CommuData.bConnect = false;
}

//-------------------------------
// デフォルトデストラクタ
//-------------------------------
CCommunicationData::~CCommunicationData()
{

}