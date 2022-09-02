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
	memset(m_CommuData.Player.HitPos, NULL, sizeof(m_CommuData.Player.HitPos[SEND_COUNTER]));
	memset(m_CommuData.Player.type, NULL, sizeof(m_CommuData.Player.type[SEND_COUNTER]));
	D3DXMatrixIdentity(&m_CommuData.Player.ModelMatrix);
	m_CommuData.Player.nNumber = 0;
	m_CommuData.Player.nFrameCount = 0;
	m_CommuData.Player.nHitDamage = 0;
	m_CommuData.Player.nKill = 0;
	m_CommuData.Player.nDeath = 0;
	m_CommuData.Player.fMotionSpeed = 0.0f;
	memset(m_CommuData.Player.aMotion, 0, sizeof(m_CommuData.Player.aMotion));
	m_CommuData.Player.bHit = false;
	m_CommuData.Player.bWin = false;
	m_CommuData.Player.bDeath = false;
	m_CommuData.Player.bInvincible = false;
	m_CommuData.Player.bRespawn = false;
	m_CommuData.Player.nNumShot = 0;
	m_CommuData.Player.nRespawnPos = 0;
	m_CommuData.Bullet.hitPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.hitPlayerPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.type = CCommunicationData::HIT_TYPE::NONE;
	m_CommuData.Bullet.nDamage = 0.0f;
	m_CommuData.Bullet.nGiveDamagePlayerNum = 0;
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

//-------------------------------
//初期化
//-------------------------------
void CCommunicationData::Init(void)
{
	m_CommuData.Player.Pos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.Rot = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.CamV = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.CamR = { 0.0f, 0.0f, 0.0f };
	memset(m_CommuData.Player.HitPos, NULL, sizeof(m_CommuData.Player.HitPos[SEND_COUNTER]));
	memset(m_CommuData.Player.type, NULL, sizeof(m_CommuData.Player.type[SEND_COUNTER]));
	D3DXMatrixIdentity(&m_CommuData.Player.ModelMatrix);
	m_CommuData.Player.nNumber = 0;
	m_CommuData.Player.nFrameCount = 0;
	m_CommuData.Player.nHitDamage = 0;
	m_CommuData.Player.nKill = 0;
	m_CommuData.Player.nDeath = 0;
	m_CommuData.Player.bInvincible = false;
	m_CommuData.Player.bRespawn = false;
	m_CommuData.Player.fMotionSpeed = 0.0f;
	memset(m_CommuData.Player.aMotion, 0, sizeof(m_CommuData.Player.aMotion));
	m_CommuData.Player.bHit = false;
	m_CommuData.Player.bWin = false;
	m_CommuData.Player.bDeath = false;
	m_CommuData.Player.nNumShot = 0;
	m_CommuData.Player.nRespawnPos = 0;
	m_CommuData.Bullet.hitPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.hitPlayerPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.type = CCommunicationData::HIT_TYPE::NONE;
	m_CommuData.Bullet.nDamage = 0.0f;
	m_CommuData.Bullet.nGiveDamagePlayerNum = 0;
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