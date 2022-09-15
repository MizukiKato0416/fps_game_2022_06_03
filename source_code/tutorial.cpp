//================================================
//���U���g����
//Author: ��������
//================================================
#include "tutorial.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"
#include "sound.h"
#include "input_keyboard.h"
#include "input_mouse.h"

//================================================
//�X�V�����}�N��
//===============================================

//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CTutorial::CTutorial(CObject::PRIORITY Priority) :CObject(Priority)
{
	
}

//================================================
//�f�X�g���N�^
//================================================
CTutorial::~CTutorial()
{

}

//================================================
//����������
//================================================
HRESULT CTutorial::Init(void)
{
	//����炷
	CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL::TITLE_BGM);

	//�`���[�g���A���摜�̐���
	CObject2D *pTutorialUi = CObject2D::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f },
		                                       (int)CObject::PRIORITY::UI);
	pTutorialUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("tutorial.jpg"));

	return S_OK;
}

//================================================
//�I������
//================================================
void CTutorial::Uninit(void)
{	
	//�����~�߂�
	CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL::TITLE_BGM);

	//�I�u�W�F�N�g�̔j��
	Release();
}

//================================================
//�X�V����
//================================================
void CTutorial::Update(void)
{
	//�L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�}�E�X�擾����
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//�G���^�[�L�[����������܂��͍��N���b�N������
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT))
	{
		//�t�F�[�h�擾����
		CFade *pFade;
		pFade = CManager::GetInstance()->GetFade();

		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			//�^�C�g���ɑJ�ڂ���
			pFade->SetFade(CManager::MODE::TITLE);

			//����炷
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL::DECIDE_SE);
		}
	}
}

//================================================
//�`�揈��
//================================================
void CTutorial::Draw(void)
{
	
}
