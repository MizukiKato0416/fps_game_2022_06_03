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
#include "meshfield.h"

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
	//�v���C���[�̐���
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2.0f, 0.0f));


	CMeshField *pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1000.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		                                        100, 100);
	pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("road000.jpg"));

	

	D3DXVECTOR3 pos = pMeshField->GetVtxPos(0);
	pos.y += 400;
	pMeshField->SetVtxPos(0, pos);

	pos = pMeshField->GetVtxPos(2);
	pos.y += 100;
	pMeshField->SetVtxPos(2, pos);

	pos = pMeshField->GetVtxPos(6);
	pos.y += -300;
	pMeshField->SetVtxPos(6, pos);

	pos = pMeshField->GetVtxPos(8);
	pos.y += 400;
	pMeshField->SetVtxPos(8, pos);

	return S_OK;
}

//================================================
//�I������
//================================================
void CGame01::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	Release();
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
}

//================================================
//�`�揈��
//================================================
void CGame01::Draw(void)
{

}