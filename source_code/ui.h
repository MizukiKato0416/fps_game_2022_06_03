//=============================================================================
//
// �^�C�g������ [title.h]
// Author : �H������
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "object2D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CUi : public CObject2D
{
public:
	enum class BLINKING_TYPE
	{
		NONE = 0,
		BLINKING,
		FADE,
		MAX
	};
	enum class MOVE_TYPE
	{
		NONE = 0,
		LEFT_TO_RIGHT,
		RIGHT_TO_LEFT,
		TOP_TO_BOTTOM,
		BOTTOM_TO_TOP,
		MAX
	};
	CUi(PRIORITY Layer = PRIORITY::UI);	// �f�t�H���g�R���X�g���N�^
	~CUi();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// �|���S���̏�����()
	void Uninit(void);	// �|���S���̏I��
	void Update(void);	// �|���S���̍X�V
	void Draw(void);	// �|���S���̕`��
	static CUi *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXVECTOR3 &stop, const int &blinkingtype, const int &movetype, const int &blinkingspeed, const int &movespeed, string tex);
	void SetBlinkingType(const BLINKING_TYPE &type) { m_BlinkingType = type; }				// �_�Ń^�C�v�ݒ菈��
	BLINKING_TYPE GetBlinkingType(void) { return m_BlinkingType; }							// �_�Ń^�C�v�擾����
	void SetMoveType(const MOVE_TYPE &type) { m_MoveType = type; }							// �ړ��^�C�v�ݒ菈��
	MOVE_TYPE GetMoveType(void) { return m_MoveType; }										// �ړ��^�C�v�擾����
	D3DXVECTOR3 GetStopPoint(void) { return m_stopPoint; }									// ��~�ʒu�擾����
	void SetStopPoint(const D3DXVECTOR3 &stop) { m_stopPoint = stop; }						// ��~�ʒu�ݒ菈��
	int GetBlinkingSpeed(void) { return m_nBlinkingSpeed; }									// �_�Ŋ��o�擾����
	void SetBlinkingSpeed(const int &blinking) { m_nBlinkingSpeed = blinking; }				// �_�Ŋ��o�ݒ菈��
	float GetMoveSpeed(void) { return m_fMoveSpeed; }										// �ړ��ʎ擾����
	void SetMoveSpeed(const float &movespeed) { m_fMoveSpeed = movespeed; }					// �ړ��ʐݒ菈��
	string GetTexName(void) { return m_tex; }	// �e�N�X�`�����擾����
private:
	D3DXVECTOR3 m_pos;						//�|���S���̈ʒu
	D3DXVECTOR3	m_size;						//�T�C�Y
	D3DXVECTOR3 m_stopPoint;
	string m_tex;
	BLINKING_TYPE m_BlinkingType;
	MOVE_TYPE m_MoveType;
	int m_nTimer;
	int m_nBlinkingSpeed;
	float m_fMoveSpeed;
	bool m_bChange;
};

#endif