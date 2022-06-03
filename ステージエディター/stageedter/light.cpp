//---------------------------
//�C���N���[�h�t�@�C��
//---------------------------
#include "light.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int	CLight::m_nMaxNumber = 0;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CLight::CLight()
{
	m_nMyNumber = m_nMaxNumber;
	m_nMaxNumber++;
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CLight::~CLight()
{
	m_nMaxNumber--;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CLight::Init(D3DLIGHTTYPE Type, D3DCOLORVALUE Diffuse, D3DVECTOR Pos, D3DVECTOR Dir)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�̃|�C���^
	D3DXVECTOR3 VecDir;		//�x�N�g��

	pDevice = CManager::GetRenderer()->GetDevice();		//�f�o�C�X���擾����
	//---------------------------
	//���C�g�̐ݒ�
	//---------------------------
	//���C�g�̃N���A
	ZeroMemory(	&m_Light,
				sizeof(D3DLIGHT9));

	//���C�g�̎��
	m_Light.Type = Type;	//���

	//���C�g�̊g�U��
	m_Light.Diffuse = Diffuse;	//�F

	//���C�g�̏ꏊ
	m_Light.Position = Pos;

	//���C�g�̕���
	VecDir = Dir;

	//���K������
	D3DXVec3Normalize(	&VecDir,
						&VecDir);

	//���C�g�̕���������
	m_Light.Direction = VecDir;

	//���C�g�̐ݒ�
	pDevice->SetLight(m_nMyNumber, &m_Light);	//n�Ԗڂ̃��C�g�ɐݒ肵���ݒ������

	//���C�g��L���ɂ���
	pDevice->LightEnable(m_nMyNumber, TRUE);	//n�Ԗڂ̃��C�g��t����
	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CLight::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CLight::Update(void)
{

}

//=============================================================================
// ����
//=============================================================================
CLight *CLight::Create(D3DLIGHTTYPE Type, D3DCOLORVALUE Diffuse, D3DVECTOR Pos, D3DVECTOR Dir)
{
	CLight *pLight = NULL;
	if (pLight == NULL)
	{
		pLight = new CLight;
	}

	if (pLight)
	{
		pLight->Init(Type, Diffuse, Pos, Dir);
	}
	return pLight;
}