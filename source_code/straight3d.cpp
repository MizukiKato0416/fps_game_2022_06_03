//=============================================================================
// (����)3D�G�t�F�N�g���� [Straight3D.cpp]
// Author : �����G��
//=============================================================================
#include "Straight3D.h"

#include "renderer.h"
#include "manager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CStraight3D::CStraight3D(PRIORITY priorty) : CBillEffect(priorty)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CStraight3D::~CStraight3D()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CStraight3D::Init(D3DXVECTOR3 pos,
	D3DXVECTOR3 Size,
	D3DXVECTOR3 MinSize,
	D3DXVECTOR3 move,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	int nType,
	int nLife,
	CStraight3D::MOVE_PATTERN Pattrn,
	D3DXVECTOR3 Target,
	int Synsetic,
	float Destance,
	CStraight3D::RAND_PATTEN RandPattern,
	CStraight3D::POS_PATTERN PosPattern,
	D3DXVECTOR2 TexMove,
	D3DXVECTOR2 TexNum,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	ANIMPATTERN AnimPattern)
{
	CBillEffect::Init(Size, MinSize, color, Mincolor, nType, nLife, TexNum, TexMove, nAnimCounter, nSplit, AnimPattern, pos);

	m_nSynthenic = Synsetic;
	m_Pattern = Pattrn;
	m_Target = Target;
	m_MinSize = MinSize;
	m_Size = Size;

	float fRandAngle = CIRCLE2;
	float fRandAngle2 = CIRCLE2;

	//�ړ�
	switch (RandPattern)
	{
	case(RANDNONE):
		m_move = move;
		break;
	case(RANDPLUS):
		if (move.x <= 0.0f)
		{
			move.x = 1.0f;
		}
		if (move.y <= 0.0f)
		{
			move.y = 1.0f;
		}
		if (move.z <= 0.0f)
		{
			move.z = 1.0f;
		}

		m_move.x = (float(rand() % (int)move.x))/* - (float(rand() % (int)Randmove.x))*/;
		m_move.y = (float(rand() % (int)move.y))/* - (float(rand() % (int)Randmove.y))*/;
		m_move.z = (float(rand() % (int)move.z))/* - (float(rand() % (int)Randmove.z))*/;
		break;
	case(RANDMIN):
		if (move.x <= 0.0f)
		{
			move.x = 1.0f;
		}
		if (move.y <= 0.0f)
		{
			move.y = 1.0f;
		}
		if (move.z <= 0.0f)
		{
			move.z = 1.0f;
		}

		m_move.x = 0.0f - (float(rand() % (int)move.x));
		m_move.y = 0.0f - (float(rand() % (int)move.y));
		m_move.z = 0.0f - (float(rand() % (int)move.z));
		break;
	case(RANDBOTH):
		if (move.x <= 0.0f)
		{
			move.x = 1.0f;
		}
		if (move.y <= 0.0f)
		{
			move.y = 1.0f;
		}
		if (move.z <= 0.0f)
		{
			move.z = 1.0f;
		}

		m_move.x = (float(rand() % (int)move.x)) - (float(rand() % (int)move.x));
		m_move.y = (float(rand() % (int)move.y)) - (float(rand() % (int)move.y));
		m_move.z = (float(rand() % (int)move.z)) - (float(rand() % (int)move.z));
		break;
	case(RANDSMOKE):
		if (move.x <= 0.0f)
		{
			move.x = 1.0f;
		}
		if (move.y <= 0.0f)
		{
			move.y = 1.0f;
		}
		if (move.z <= 0.0f)
		{
			move.z = 1.0f;
		}

		m_move.x = (float(rand() % (int)move.x)) - (float(rand() % (int)move.x));
		m_move.z /= 10;
		m_move.y = (float(rand() % (int)move.y));
		m_move.z = (float(rand() % (int)move.z)) - (float(rand() % (int)move.z));
		m_move.z /= 10;
		break;
	}

	//�o���ʒu
	switch (PosPattern)
	{
	case(FIELD):
		m_pos = D3DXVECTOR3(
			pos.x + Destance * sinf(fRandAngle) * cosf(fRandAngle2),
			pos.y,
			pos.z + Destance * sinf(fRandAngle) * sinf(fRandAngle2));
		break;
	case(WALL):
		//******************************
		//��
		m_pos = D3DXVECTOR3(
			pos.x + Destance * sinf(fRandAngle) * cosf(fRandAngle2),
			pos.y,
			pos.z + Destance * sinf(fRandAngle) * sinf(fRandAngle2));
		//******************************
		break;
	case(SPHERE):
		m_pos = D3DXVECTOR3(
			pos.x + Destance * sinf(fRandAngle) * cosf(fRandAngle2),
			pos.y + Destance * cosf(fRandAngle),
			pos.z + Destance * sinf(fRandAngle) * sinf(fRandAngle2));
		break;
	case(OTHERS):
		m_pos = pos;
		break;
	}

	SetPos(m_pos);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CStraight3D::Uninit()
{
	CBillEffect::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CStraight3D::Update()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 v;	//�v�Z
	float r;	//��������

	m_Size += m_MinSize;		//�T�C�Y�ύX
	//�T�C�Y��0������肻��
	if (m_Size.x <= 0 ||
		m_Size.y <= 0)
	{
		m_bUninit = true;
	}

	if (m_Pattern == STRAIGHT)
	{
		pos += m_move;
	}
	else if(m_Pattern == TARGET)
	{
		v = pos - m_Target;
		r = sqrtf(v.x * v.x + v.z * v.z);

		m_XZr = (float)atan2(v.x, v.z);		//�p�x����
		m_Yr = (float)atan2(v.x, v.y);		//�p�x��

		pos += D3DXVECTOR3(
			sinf(m_XZr) * -m_move.x,	//X
			-m_move.x * sinf(m_Yr + D3DX_PI / 2),	//��(��)
			cosf(m_XZr) * -m_move.x);	//z

		if (r < 5)
		{
			m_bUninit = true;
		}
	}

	SetPos(pos);
	ChangeSize(m_Size);
	CBillEffect::Update();
}

//=============================================================================
// �`��
//=============================================================================
void CStraight3D::Draw()
{
	CBillEffect::Draw();
}

//=============================================================================
// �쐬
//=============================================================================
CStraight3D *CStraight3D::Create(D3DXVECTOR3 pos,
	D3DXVECTOR3 Size,
	D3DXVECTOR3 MinSize,
	D3DXVECTOR3 move,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	int nType,
	int nLife,
	CStraight3D::MOVE_PATTERN Pattrn,
	D3DXVECTOR3 Target,
	int Synsetic,
	float Destance,
	CStraight3D::RAND_PATTEN RandPattern,
	CStraight3D::POS_PATTERN PosPattern,
	D3DXVECTOR2 TexMove,
	D3DXVECTOR2 TexNum,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	ANIMPATTERN AnimPattern)
{
	CStraight3D *pStraight3D = new CStraight3D(CObject::PRIORITY::EFFECT3D);

	if (pStraight3D != NULL)
	{
		pStraight3D->Init(pos, Size, MinSize, move, color, Mincolor, nType, nLife, Pattrn, Target, Synsetic, Destance, RandPattern, PosPattern, TexMove,
			TexNum, nAnimCounter, nSplit, AnimPattern);
	}

	return pStraight3D;
}