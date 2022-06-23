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
	sizeof(COMMUNICATION_DATA);
	m_CommuData.bConnect = false;
	m_CommuData.Player.fMotionSpeed = 0.0f;
	m_CommuData.Player.nNumber = 0;
	m_CommuData.Player.Pos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Player.Rot = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Bullet.nCollEnemy = 0;
	m_CommuData.Bullet.bUse = false;
	m_CommuData.Ballistic.BigenPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Ballistic.EndPos = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Ballistic.Size = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Ballistic.Rot = { 0.0f, 0.0f, 0.0f };
	m_CommuData.Ballistic.fSpeed = 0.0f;
	m_CommuData.Ballistic.sTexPas1.clear();
	m_CommuData.Ballistic.sTexPas2.clear();
}

//-------------------------------
// �f�t�H���g�f�X�g���N�^
//-------------------------------
CCommunicationData::~CCommunicationData()
{

}