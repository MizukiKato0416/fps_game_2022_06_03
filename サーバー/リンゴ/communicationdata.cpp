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
	m_CommuData.Player.nNumber = 0;
	m_CommuData.Player.Pos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.Rot = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.BigenPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.EndPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.nCollEnemy = 0;
	m_CommuData.Bullet.bUse = false;
}

//-------------------------------
// �f�t�H���g�f�X�g���N�^
//-------------------------------
CCommunicationData::~CCommunicationData()
{

}