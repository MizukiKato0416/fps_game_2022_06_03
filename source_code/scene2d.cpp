//=============================================================================
// �V�[��2D���� [Scene2D.cpp]
// Author : �����G��
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "Scene2D.h"
#include "manager.h"
#include "Renderer.h"

//=============================================================================
// �}�N��
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(PRIORITY priorty) : CObject(priorty)
{
	m_pVtxBuff = NULL;

	m_Texpos = 0;
	m_TexSize = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// �|���S���̏�����
//=============================================================================
HRESULT CScene2D::Init()
{
	// ���_����ݒ�
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̎擾

	 //���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	VERTEX_2D*pVtx;//���_���ւ̃|�C���^


	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_Size.x / 2), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�̐F
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`����UV���W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();


	return S_OK;
}

//=============================================================================
// �|���S���̏I��
//=============================================================================
void CScene2D::Uninit()
{

	//���_�o�b�t�@���j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g�j��
	Release();
}


//=============================================================================
// �|���S���̍X�V
//=============================================================================
void CScene2D::Update()
{

	VERTEX_2D*pVtx;//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_Size.x / 2), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

}

//=============================================================================
// �|���S���̕`��
//=============================================================================
void CScene2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//�f�o�C�X�̃|�C���^
	LPDIRECT3DTEXTURE9 buf = CManager::GetInstance()->GetTexture()->GetTexture(m_pTexture);

	//�A���t�@�e�X�g�֌W
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	if (buf != NULL)
	{
		pDevice->SetTexture(0, buf);//�e�N�X�`���̐ݒ�
	}
	else
	{
		pDevice->SetTexture(0, NULL);//�e�N�X�`���̐ݒ�
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�A���t�@�e�X�g�֌W
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

}

//=============================================================================
// �e�N�X�`�����Z�b�g(�e�N�X�`���p�^�[���̍X�V)
//=============================================================================
void CScene2D::SetTex(int Texpos, float TexSize)
{
	m_Texpos = Texpos;
	m_TexSize = TexSize;

	//�ꖇ�G�ƕ����邽�߂����ōX�V
	VERTEX_2D*pVtx;//���_���ւ̃|�C���^
				   //���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`����UV���W
	pVtx[0].tex = D3DXVECTOR2(m_Texpos * m_TexSize, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_Texpos * m_TexSize + m_TexSize, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_Texpos * m_TexSize, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_Texpos * m_TexSize + m_TexSize, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

}

//=============================================================================
// �e�N�X�`�����Z�b�g(�w�i�X�N���[��)
//=============================================================================
void CScene2D::TexMove(float TexMoveU, float TexMoveV)
{
	m_TexMoveU = TexMoveU;
	m_TexMoveV = TexMoveV;
	m_PosTexV += m_TexMoveV;

	if (m_PosTexV >= 1.0f)
	{
		m_PosTexV -= 1.0f;
	}
	else if (m_PosTexV < 0.0f)
	{
		m_PosTexV += 1.0f;
	}

	if (m_TexMoveU >= 1.0f)
	{
		m_TexMoveU -= 1.0f;
	}
	else if (m_TexMoveU < 0.0f)
	{
		m_TexMoveU += 1.0f;
	}

	//�ꖇ�G�ƕ����邽�߂����ōX�V
	VERTEX_2D*pVtx;//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`����UV���W
	pVtx[0].tex = D3DXVECTOR2(m_TexMoveU, m_PosTexV);
	pVtx[1].tex = D3DXVECTOR2(m_TexMoveU + 1.0f, m_PosTexV);
	pVtx[2].tex = D3DXVECTOR2(m_TexMoveU, m_PosTexV + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_TexMoveU + 1.0f, m_PosTexV + 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �Q�[�W�^�̃T�C�Y�ύX
//=============================================================================
void CScene2D::SetGaugeScele(float Whidth, float Hight)
{
	VERTEX_2D*pVtx;//���_���ւ̃|�C���^

	m_Size.x = Whidth;
	m_Size.y = Hight;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x * 4), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x * 4), m_pos.y + (m_Size.y / 2), 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

}

//=============================================================================
// �|���S�����Z�b�g(�J���[)
//=============================================================================
void CScene2D::ColorChange(D3DCOLORVALUE color)
{
	VERTEX_2D*pVtx;//���_���ւ̃|�C���^

				   //���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̐F
	pVtx[0].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);
	pVtx[1].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);
	pVtx[2].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);
	pVtx[3].col = D3DCOLOR_RGBA((int)color.r, (int)color.g, (int)color.b, (int)color.a);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

}

//=============================================================================
// �|���S�����Z�b�g(�t�F�[�h�J���[)
//=============================================================================
void CScene2D::FadeColorChange(int Alpha)
{
	VERTEX_2D*pVtx;//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̐F
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, Alpha);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �|���S�����Z�b�g(��])
//=============================================================================
void CScene2D::SetRotate(D3DXVECTOR3 pos, float Rotate, float Rotate2, float Vectol)
{
	VERTEX_2D*pVtx;//���_���ւ̃|�C���^

	m_pos = pos;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�̍��W
	pVtx[0].pos = D3DXVECTOR3(pos.x - (sinf(Rotate)) * Vectol, pos.y - (cosf(Rotate))* Vectol, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - (cosf(-Rotate)) * Vectol, pos.y - (sinf(-Rotate))* Vectol, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + (cosf(-Rotate)) * Vectol, pos.y + (sinf(-Rotate))* Vectol, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + (sinf(Rotate)) * Vectol, pos.y + (cosf(Rotate))* Vectol, 0.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �|���S�����Z�b�g(pos)
//=============================================================================
void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;

	CScene2D::Update();
}
//=============================================================================
// �|���S�����Z�b�g(Hight)
//=============================================================================
void CScene2D::SetHight(float Hight)
{
	m_Size.y = Hight;
	//CScene2D::Update();
}

//=============================================================================
// �|���S�����Z�b�g(Whidth)
//=============================================================================
void CScene2D::SetWhidth(float Whidth)
{
	m_Size.x = Whidth;
	//CScene2D::Update();
}
