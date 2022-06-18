//====================================================
//
// �R�~���j�P�[�V�����f�[�^�̃w�b�_�[�t�@�C��(communicationdata.cpp)
// Author : �H�� ����
//
//====================================================
//-------------------------------
// �C���N���[�h
//-------------------------------
#include "communicationdata.h"

//-------------------------------
// �f�t�H���g�R���X�g���N�^
//-------------------------------
CCommunicationData::CCommunicationData()
{
	m_CommuData.bConnect = false;
	m_CommuData.Player.fMotionSpeed = 0.0f;
	m_CommuData.Player.Motion = " ";
	m_CommuData.Player.nNumber = 0;
	m_CommuData.Player.Pos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.Rot = { 0.0f, 0.0f, 0.0f };
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		m_CommuData.Bullet[nCnt].bEnemyHit = false;
		m_CommuData.Bullet[nCnt].BigenPos = { 0.0f, 0.0f, 0.0f };
		m_CommuData.Bullet[nCnt].bObjectHit = false;
		m_CommuData.Bullet[nCnt].EndPos = { 0.0f, 0.0f, 0.0f };
		m_CommuData.Bullet[nCnt].nCollEnemy = 0;
	}
}

//-------------------------------
// �f�t�H���g�f�X�g���N�^
//-------------------------------
CCommunicationData::~CCommunicationData()
{
	m_CommuData.bConnect = false;
	m_CommuData.Player.fMotionSpeed = 0.0f;
	m_CommuData.Player.Motion = " ";
	m_CommuData.Player.nNumber = 0;
	m_CommuData.Player.Pos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.Rot = { 0.0f, 0.0f, 0.0f };
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		m_CommuData.Bullet[nCnt].bEnemyHit = false;
		m_CommuData.Bullet[nCnt].BigenPos = { 0.0f, 0.0f, 0.0f };
		m_CommuData.Bullet[nCnt].bObjectHit = false;
		m_CommuData.Bullet[nCnt].EndPos = { 0.0f, 0.0f, 0.0f };
		m_CommuData.Bullet[nCnt].nCollEnemy = 0;
	}
}

//-------------------------------
// ���Z�b�g
//-------------------------------
void CCommunicationData::COMMUNICATION_DATA::Rest(void)
{
	bConnect = false;
	Player.fMotionSpeed = 0.0f;
	Player.Motion = " ";
	Player.nNumber = 0;
	Player.Pos = { 0.0f, 0.0f, 0.0f };
	Player.Rot = { 0.0f, 0.0f, 0.0f };
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		Bullet[nCnt].bEnemyHit = false;
		Bullet[nCnt].BigenPos = { 0.0f, 0.0f, 0.0f };
		Bullet[nCnt].bObjectHit = false;
		Bullet[nCnt].EndPos = { 0.0f, 0.0f, 0.0f };
		Bullet[nCnt].nCollEnemy = 0;
	}
}
