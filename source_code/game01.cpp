//================================================
//�Q�[��01����
//Author: ��������
//================================================
#include "game01.h"
#include "manager.h"
#include "input_keyboard.h"
#include "player.h"
#include "fade.h"
#include "floor.h"
#include "mesh_field.h"
#include "xanimmodel.h"

//================================================
//�}�N����`
//================================================

//================================================
//�ÓI�����o�ϐ��錾
//================================================

//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CGame01::CGame01(CObject::PRIORITY Priority):CObject(Priority)
{
	m_AnimSpeed = 1.0f;
	m_pPlayer = nullptr;
}

//================================================
//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
//================================================


//================================================
//�f�X�g���N�^
//================================================
CGame01::~CGame01()
{
	
}

//================================================
//����������
//================================================
HRESULT CGame01::Init(void)
{
	pXanim = CXanimModel::Create("data/test.x");

	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2.0f, 0.0f));

	//���b�V���t�B�[���h����
	CMeshField *pMeshField = CMeshField::CreateLoadText("data/mesh_field.txt");
	pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("grass001.png"));

	return S_OK;
}

//================================================
//�I������
//================================================
void CGame01::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	Release();
	pXanim->Uninit();
}

//================================================
//�X�V����
//================================================
void CGame01::Update(void)
{
#ifdef _DEBUG
	//�L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//Enter�L�[����������
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		//�t�F�[�h�擾����
		CFade *pFade;
		pFade = CManager::GetInstance()->GetFade();

		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			pFade->SetFade(CManager::MODE::RESULT);
		}
	}
#endif // !_DEBUG
	if (pInputKeyboard->GetPress(DIK_LEFT) == true)
	{
		m_AnimSpeed -= 0.01f;
	}
	else if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{
		m_AnimSpeed += 0.01f;
	}

	pXanim->Update();
	pXanim->PlayAnimation((160.0f * m_AnimSpeed) / 4800);

	D3DXMATRIX test = pXanim->GetMatrix("Cube_001");
}

//================================================
//�`�揈��
//================================================
void CGame01::Draw(void)
{
	pXanim->Draw();
}