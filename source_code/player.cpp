//================================================
//�v���C���[����
//Author:��������
//================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "input_pad_d.h"
#include "input_mouse.h"
#include "player.h"
#include "x_load.h"
#include "model_single.h"
#include "texture.h"
#include "camera.h"
#include "shadow.h"
#include "floor.h"
#include "mesh_field.h"
#include "xanimmodel.h"
#include "model.h"
#include "tcp_client.h"
#include "communicationdata.h"
#include "game01.h"
#include <thread>
#include "PresetSetEffect.h"
#include "bullet.h"
#include "gunmodel.h"
#include "object2D.h"

//================================================
//�}�N����`
//================================================
#define PLAYER_JUMP							(16.0f)									//�W�����v��
#define PLAYER_GRAVITY						(1.2f)									//�d�͂̑傫��
#define PLAYER_WALK_SPEED					(3.5f)									//�����ړ��̈ړ���
#define PLAYER_RUN_SPEED					(6.0f)									//����ړ��̈ړ���
#define PLAYER_ADS_WALK_SPEED				(2.0f)									//ADS���̈ړ����x
#define PLAYER_SIZE							(75.0f)									//�v���C���[�̃T�C�Y�����l
#define PLAYER_SHOT_COUNTER					(5)										//���̒e���o��܂ł̃J�E���^�[
#define PLAYER_ADS_GUN_OFFSET				(D3DXVECTOR3(0.0f, -3.0f, 2.0f))		//ADS�����Ƃ��̏e�̃I�t�Z�b�g
#define PLAYER_ADS_CAMERA_ADD_RADIUS		(10.0f)									//ADS�����Ƃ��̉�p���Z��
#define PLAYER_ADS_CAMERA_RADIUS			(65.0f)									//ADS�����Ƃ��̉�p

//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CPlayer::CPlayer(CObject::PRIORITY Priority):CObject(Priority)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_offsetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pGunModel = nullptr;
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_bRotate = false;
	m_bJump = false;
	m_fMoveSpeed = 0.0f;
	m_nCounter = 0;
	m_bAds = false;
	m_pCloss = nullptr;
}

//================================================
//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
//================================================


//================================================
//�f�X�g���N�^
//================================================
CPlayer::~CPlayer()
{

}

//================================================
//����������
//================================================
HRESULT CPlayer::Init(void)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_offsetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_bRotate = false;
	m_bJump = false;
	m_fMoveSpeed = 0.0f;
	m_nCounter = 0;
	m_bAds = false;

	//�e���f���̐���
	m_pGunModel = CGunModel::Create({0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f}, { 0.0f, 1.6f, 12.0f }, "asult_gun_inv.x");

	//�ʒu�̐ݒ�
	SetPos(m_pos);
	SetPosOld(m_posOld);

	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	SetObjType(CObject::OBJTYPE::PLAYER);

	// �A�j���[�V�����t��X�t�@�C���̐���
	m_pAnimModel = CXanimModel::Create("data/armmotion.x");
	//�j���[�g�������[�V�����ɂ���
	m_pAnimModel->ChangeAnimation("neutral", 60.0f / 4800.0f);

	//�T�C�Y���擾
	D3DXVECTOR3 modelSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	modelSize = m_pAnimModel->GetSize();

	//�T�C�Y��X��Z���ׂđ傫���ق���X��Z���ꂼ��Ɋ��蓖�Ă�
	if (modelSize.x >= modelSize.z)
	{
		m_size = D3DXVECTOR3(modelSize.x + PLAYER_SIZE, modelSize.y, modelSize.x + PLAYER_SIZE);
	}
	else
	{
		m_size = D3DXVECTOR3(modelSize.z + PLAYER_SIZE, modelSize.y, modelSize.z + PLAYER_SIZE);
	}

	//�T�C�Y�̐ݒ�
	SetSize(m_size);

	//�e�̐ݒ�
	CShadow::Create(D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z), D3DXVECTOR3(m_size.x, 0.0f, m_size.z), this);

	m_pCloss = CObject2D::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, {SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f}, (int)CObject::PRIORITY::UI);
	m_pCloss->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("closs.png"));

	return S_OK;
}

//================================================
//�I������
//================================================
void CPlayer::Uninit(void)
{
	//�}�g���b�N�X���擾
	D3DXMATRIX *handR = nullptr;
	handR = m_pAnimModel->GetMatrix("handR");

	//new�����̂ŏ���
	delete handR;
	handR = nullptr;

	m_pAnimModel->Uninit();
	Release();
}

//================================================
//�X�V����
//================================================
void CPlayer::Update(void)
{
	CSound *sound;
	sound = CManager::GetInstance()->GetSound();
	CTcpClient *pTcp = CManager::GetInstance()->GetCommunication();
	CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data.GetCmmuData();

	//�ʒu�擾
	D3DXVECTOR3 pos = GetPos();

	m_pos = pos;
	m_posOld = pos;

	//1�t���[���O�̈ʒu�ݒ�
	SetPosOld(m_posOld);

	//���̏���
	Chest();

	//�ړ�����
	Move();

	//�d��
	m_move.y -= PLAYER_GRAVITY;

	m_pos += m_move;		//�ړ��ʔ��f

	//��]�̊���
	Rotate();

	//�ʒu���f
	SetPos(m_pos);

	//���Ƃ̓����蔻��
	if (CFloor::Collision(this, m_size.x) == true)
	{
		//�d�͂�0�ɂ���
		m_move.y = 0.0f;

		//�W�����v�����Ă��Ȃ���Ԃɂ���
		m_bJump = false;

		//�W�����v����
		Jump();

		//�ʒu�擾
		pos = GetPos();
		m_pos = pos;
	}

	//���b�V���t�B�[���h�Ƃ̓����蔻��
	if (CMeshField::Collision(this, m_size.x * 20.0f) == true)
	{
		//�d�͂�0�ɂ���
		m_move.y = 0.0f;

		//�W�����v�����Ă��Ȃ���Ԃɂ���
		m_bJump = false;

		//�W�����v����
		Jump();

		//�ʒu�擾
		pos = GetPos();
		m_pos = pos;
	}

	//���f���Ƃ̓����蔻��
	int nHit = CModelSingle::Collision(this, m_size.x / 2.0f, 150.0f);
	//�ォ�炠�������Ƃ�
	if (nHit == 1)
	{
		//�d�͂�0�ɂ���
		m_move.y = 0.0f;

		//�W�����v�����Ă��Ȃ���Ԃɂ���
		m_bJump = false;

		//�W�����v����
		Jump();
	}
	else if (nHit == 2)
	{//�����炠�������Ƃ�
		//�d�͂�0�ɂ���
		m_move.y = 0.0f;
	}

	//�ʒu�擾
	pos = GetPos();
	m_pos = pos;

	m_pAnimModel->Update();

	pData->Player.Pos = m_pos;
	pData->Player.Rot = m_rot;
	pData->Player.fMotionSpeed = m_fAnimSpeed;
	pTcp->Send((char*)pData, sizeof(CCommunicationData::COMMUNICATION_DATA));

	//�ˌ�����
	Shot();

	//ADS����
	ADS();
}

//================================================
//�`�揈��
//================================================
void CPlayer::Draw(void)
{
	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	D3DXMatrixIdentity(&m_mtxWorld);		//�v���C���[�̃��[���h�}�g���b�N�X�̏�����

	D3DXVECTOR3 pos, rot;

	//�e�q�֌W�������Ă�����
	if (m_bObjParent == true)
	{
		pos = m_offsetPos;
		rot = m_rot;
	}
	else
	{//�����Ă��Ȃ�������
		pos = m_pos;
		rot = m_rot;
	}

	D3DXMATRIX *cameraMtx = nullptr;
	//camera�̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//camera�̎擾
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//camera�̌����擾
			rotCamera = pCamera->GetRotV();

			rotCamera.x -= D3DX_PI / 2.0f;

			//�v���C���[�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

			cameraMtx = pCamera->GetMtxPoint();
		}
	}

	//�v���C���[�̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_bObjParent == true)
	{
		D3DXMATRIX mtxParent = *m_mtxWorldParent;

		//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃��[���h�}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//m_pAnimModel->Draw();

	if (!m_bAds)
	{
		//�}�g���b�N�X���擾
		D3DXMATRIX *handR = nullptr;
		handR = m_pAnimModel->GetMatrix("handR");
		m_pGunModel->SetMtxParent(m_pGunModel->GetModel()->GetModel()->GetMtxPoint());
		//�e�Ɛe�q�֌W������
		m_pGunModel->GetModel()->GetModel()->SetMtxParent(handR);
		m_pGunModel->GetModel()->GetModel()->SetObjParent(true);
		m_pGunModel->GetModel()->GetModel()->SetRot({ 0.0f, D3DX_PI / 2.0f, 0.0f });
		m_pGunModel->GetModel()->GetModel()->SetPos({ 0.0f, 0.0f, 0.0f });
	}
	else
	{
		//�e�Ɛe�q�֌W������
		m_pGunModel->GetModel()->GetModel()->SetMtxParent(cameraMtx);
		m_pGunModel->GetModel()->GetModel()->SetObjParent(true);
		m_pGunModel->GetModel()->GetModel()->SetRot({ 0.0f, 0.0f, 0.0f });
		m_pGunModel->GetModel()->GetModel()->SetPos(PLAYER_ADS_GUN_OFFSET);
	}
}

//================================================
//��������
//================================================
CPlayer *CPlayer::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot)
{
	//�C���X�^���X�̐���
	CPlayer *pPlayer = nullptr;
	if (pPlayer == nullptr)
	{
		pPlayer = new CPlayer;
		if (pPlayer != nullptr)
		{
			pPlayer->m_pos = pos;
			pPlayer->m_posOld = pos;
			pPlayer->m_rot = rot;
			pPlayer->Init();
		}
	}
	return pPlayer;
}

//================================================
//���f���̓�������
//================================================
void CPlayer::Move(void)
{
	//�p�b�hD�擾����
	CInputPadD *pInputPadD = CManager::GetInstance()->GetInputPadD();
	DIJOYSTATE2 JoyStick = pInputPadD->GetGamepad();

	//�L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�p�b�hX�擾����
	CInputPadX *pInputPadX;
	pInputPadX = CManager::GetInstance()->GetInputPadX();

	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//camera�̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//camera�̎擾
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			//camera�̌����擾
			rotCamera = pCamera->GetRotV();
		}
	}

	//�ړ��ʐݒ�p�ϐ�
	m_fMoveSpeed = PLAYER_WALK_SPEED;

	//�X�e�B�b�N�̌X������������܂���WASD����������
	if ((float)JoyStick.lX != 0.0f || (float)JoyStick.lY != 0.0f || 
		pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true ||
		pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{
		//�������[�V�����łȂ�������
		if (m_pAnimModel->GetAnimation() != "walk")
		{
			//�������[�V�����ɂ���
			m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
			m_pAnimModel->ChangeAnimation("walk", m_fAnimSpeed);
		}

		//�ړI�̌�����ݒ�
		if ((float)JoyStick.lX != 0.0f || (float)JoyStick.lY != 0.0f)
		{
			m_fObjectiveRot = rotCamera.y + atan2f((float)JoyStick.lY, (float)JoyStick.lX);
		}
		else if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			//SHIFT���������瑁������
			if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
			{
				m_fMoveSpeed = PLAYER_RUN_SPEED;

				//���胂�[�V�����ɂ���
				m_fAnimSpeed = (20.0f * 5.0f) / 4800.0f;
				m_pAnimModel->ChangeSpeed(m_fAnimSpeed);
			}
			else
			{
				//�������[�V�����ɂ���
				m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
				m_pAnimModel->ChangeSpeed(m_fAnimSpeed);
			}

			if (pInputKeyboard->GetPress(DIK_A) == true)
			{
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 4.0f;
			}
			else if (pInputKeyboard->GetPress(DIK_D) == true)
			{
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 4.0f;
			}
			else
			{
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			if (pInputKeyboard->GetPress(DIK_A) == true)
			{
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 4.0f * 3.0f;
			}
			else if (pInputKeyboard->GetPress(DIK_D) == true)
			{
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 4.0f * 3.0f;
			}
			else
			{
				//�ړI�̌�����ݒ�
				m_fObjectiveRot = rotCamera.y + D3DX_PI;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			//�ړI�̌�����ݒ�
			m_fObjectiveRot = rotCamera.y - D3DX_PI / 2.0f;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			//�ړI�̌�����ݒ�
			m_fObjectiveRot = rotCamera.y + D3DX_PI / 2.0f;
		}

		if (m_bAds)
		{
			m_fMoveSpeed = PLAYER_ADS_WALK_SPEED;
		}

		//�ړ��ʉ��Z
		m_move.x = -sinf(m_fObjectiveRot + D3DX_PI) * m_fMoveSpeed;
		m_move.z = -cosf(m_fObjectiveRot + D3DX_PI) * m_fMoveSpeed;
		//��]��������
		m_bRotate = true;
	}
	else
	{//�X�e�B�b�N�ɌX�����Ȃ�������

		//�ړ��ʂ��[���ɂ���
		m_move.x = 0.0f;
		m_move.z = 0.0f;

		//�j���[�g�������[�V�����łȂ������犎�����[�V��������Ȃ�������
		if (m_pAnimModel->GetAnimation() != "neutral" && m_pAnimModel->GetAnimation() != "fireneutral")
		{
			//�j���[�g�������[�V�����ɂ���
			m_pAnimModel->ChangeAnimation("neutral", 60.0f / 4800.0f);
			m_fAnimSpeed = 60.0f / 4800.0f;
		}
	}
}

//================================================
//��]����
//================================================
void CPlayer::Rotate(void)
{
	//camera�̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//camera�̎擾
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)

		{
			//camera�̌����擾
			m_rot.y = pCamera->GetRotV().y + D3DX_PI;
		}
	}

	//�΂��傫���Ȃ�����-2�΂���
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{	//-�΂�菬�����Ȃ�����+2�΂���
		m_rot.y += D3DX_PI * 2.0f;
	}
}

//================================================
//�W�����v����
//================================================
void CPlayer::Jump(void)
{
	//�L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)	//SPACE�{�^�����������Ƃ��̏���
	{
		//�ړ��ʂ��W�����v�����Z
		m_move.y = PLAYER_JUMP;
		m_bJump = true;
	}
}

//================================================
//�ˌ�����
//================================================
void CPlayer::Shot(void)
{
	//�}�E�X�擾����
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	if (pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
	{
		//���A�j���[�V�����łȂ�������
		if (m_pAnimModel->GetAnimation() != "fireneutral")
		{
			//�����[�V�����ɂ���
			m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
			m_pAnimModel->ChangeAnimation("fireneutral", m_fAnimSpeed);
		}

		//�J�E���^�[�����Z
		m_nCounter--;

		//0��菬�����Ȃ�����
		if (m_nCounter < 0)
		{
			//����̒l�ɂ���
			m_nCounter = PLAYER_SHOT_COUNTER;

			//�I�t�Z�b�g�ʒu�ݒ�
			D3DXVECTOR3 pos = { m_pGunModel->GetMuzzleMtx()._41, m_pGunModel->GetMuzzleMtx()._42, m_pGunModel->GetMuzzleMtx()._43};
			pos += m_posOld - m_pos;

			//�}�Y���t���b�V���G�t�F�N�g�̐���
			CPresetEffect::SetEffect3D(0, pos, {}, {});
			CPresetEffect::SetEffect3D(1, pos, {}, {});

			//�e�̐���
			CBullet::Create();
		}
	}
	else
	{
		//���A�j���[�V������������
		if (m_pAnimModel->GetAnimation() == "fireneutral")
		{
			//�j���[�g�������[�V�����ɂ���
			m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
			m_pAnimModel->ChangeAnimation("neutral", m_fAnimSpeed);
		}

		//����̒l��菬����������
		if (m_nCounter > 0 && m_nCounter <= PLAYER_SHOT_COUNTER)
		{
			//�J�E���^�[�����Z
			m_nCounter++;

			//����̒l���傫���Ȃ�����
			if (m_nCounter > PLAYER_SHOT_COUNTER)
			{
				//0�ɂ���
				m_nCounter = 0;
			}
		}
	}
}

//================================================
//ADS�̏���
//================================================
void CPlayer::ADS(void)
{
	//�}�E�X�擾����
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//�E�N���b�N��������
	if (pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_RIGHT) == true)
	{
		//ADS��ԂłȂ��Ȃ�
		if (!m_bAds)
		{
			//ADS��Ԃɂ���
			m_bAds = true;
		}
		//camera�̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
		CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

		for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
		{
			//camera�̎擾
			CCamera *pCamera = &**pCameraAddress;
			if (pCamera != nullptr)
			{
				//camera�̉�p�擾
				float fRadius = pCamera->GetRadius();
				//������傫���Ƃ�
				if (fRadius > PLAYER_ADS_CAMERA_RADIUS)
				{
					//���炷
					fRadius -= PLAYER_ADS_CAMERA_ADD_RADIUS;

					//�����菬�����Ȃ�����
					if (fRadius < PLAYER_ADS_CAMERA_RADIUS)
					{
						//����̒l�ɂ���
						fRadius = PLAYER_ADS_CAMERA_RADIUS;
					}
					//��p��ݒ�
					pCamera->SetRadius(fRadius);
				}
			}
		}
	}
	else
	{
		//ADS��ԂȂ�
		if (m_bAds)
		{
			//ADS��Ԃ���߂�
			m_bAds = false;

			//camera�̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
			CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

			for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
			{
				//camera�̎擾
				CCamera *pCamera = &**pCameraAddress;
				if (pCamera != nullptr)
				{
					//camera�̉�p�擾
					float fRadius = pCamera->GetRadius();
					//�����菬�����Ƃ�
					if (fRadius < CAMERA_RADIUS)
					{
						//����̒l�ɂ���
						fRadius = CAMERA_RADIUS;
						//��p��ݒ�
						pCamera->SetRadius(fRadius);
					}
				}
			}
		}
	}
}

//================================================
//���̏���
//================================================
void CPlayer::Chest(void)
{
	//camera�̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//camera�̎擾
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//camera�̌����擾
			rotCamera = pCamera->GetRotV();

			D3DXMATRIX cameraMtx;
			D3DXMatrixIdentity(&cameraMtx);
			D3DXMatrixRotationYawPitchRoll(&cameraMtx, 0.0f, rotCamera.x, 0.0f);

			//�}�g���b�N�X���擾
			D3DXMATRIX *handR = nullptr;
			handR = m_pAnimModel->GetMatrix("handR");

			handR->_11 = 10050.0f;

			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &cameraMtx);

			//m_pAnimModel->SetMatrix("handR", handR);
		}
	}
}

//================================================
//�v���C���[�Ƃ̓����蔻��
//================================================
void CPlayer::Collision(CObject *&pSubjectObject, const float &fObjRadius)
{
	//�I�u�W�F�N�g��������|�C���^
	vector<CObject*> object;

	//�擪�̃|�C���^����
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
	int nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//�I�u�W�F�N�g�̎�ނ��v���C���[��������
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
		{
			//�v���C���[�ɃL���X�g
			CPlayer *pPlayerObj = nullptr;
			pPlayerObj = (CPlayer*)object[nCnt];

			//�I�u�W�F�N�g�̈ʒu���擾
			D3DXVECTOR3 posObj = pSubjectObject->GetPos();

			//�v���C���[����Ώۂ̃I�u�W�F�N�g�܂ł̋��������߂�
			float fRadius = sqrtf((posObj.x - pPlayerObj->m_pos.x) * (posObj.x - pPlayerObj->m_pos.x) +
				(posObj.z - pPlayerObj->m_pos.z) * (posObj.z - pPlayerObj->m_pos.z));
			//�v���C���[�ƑΏۂ̃I�u�W�F�N�g�̊p�x�����߂�
			float fRot = atan2((posObj.x - pPlayerObj->m_pos.x), (posObj.z - pPlayerObj->m_pos.z)) - D3DX_PI;

			//���߂��������v���C���[�̔��a�ƑΏۂ̃I�u�W�F�N�g�̔��a�𑫂������l�ȉ���������
			if (fRadius <= pPlayerObj->m_size.x / 2.0f + fObjRadius)
			{
				//�v���C���[�̈ʒu�������o��
				pPlayerObj->m_pos.x = posObj.x + (sinf(fRot) * (pPlayerObj->m_size.x / 2.0f + fObjRadius));
				pPlayerObj->m_pos.z = posObj.z + (cosf(fRot) * (pPlayerObj->m_size.x / 2.0f + fObjRadius));

				//�ʒu�ݒ�
				pPlayerObj->SetPos(pPlayerObj->m_pos);

				//���f���Ƃ̓����蔻��
				CModelSingle::Collision(pPlayerObj);
			}
		}
	}
}

//================================================
//�v���C���[�Ƃ̏Փ˔���݂̂̏���
//================================================
bool CPlayer::CollisionOnly(CObject *&pSubjectObject, const float &fObjRadius)
{
	//�I�u�W�F�N�g�̈ʒu���擾
	D3DXVECTOR3 posObj = pSubjectObject->GetPos();

	//�v���C���[����Ώۂ̃I�u�W�F�N�g�܂ł̋��������߂�
	float fRadius = sqrtf((posObj.x - m_pos.x) * (posObj.x - m_pos.x) +
		(posObj.z - m_pos.z) * (posObj.z - m_pos.z));

	//���߂��������v���C���[�̔��a�ƑΏۂ̃I�u�W�F�N�g�̔��a�𑫂������l�ȉ���������
	if (fRadius <= m_size.x / 2.0f + fObjRadius)
	{
		//�������Ă���
		return true;
	}

	return false;
}