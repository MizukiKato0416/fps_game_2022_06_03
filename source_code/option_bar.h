//=============================================================================
//
// �ݒ��ʂ̃o�[�̏��� [option_bar.h]
// Author : ��������
//
//=============================================================================
#ifndef _OPTION_BAR_H_
#define _OPTION_BAR_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject2D;
class CCounter;

//*****************************************************************************
// �\���̂̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class COptionBar : public CObject
{
public:
	COptionBar(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// �f�t�H���g�R���X�g���N�^
	~COptionBar();		// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	//��������
	static COptionBar *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &barSize, const D3DXVECTOR3 &circleSize,
		                      const float &fMin, const float &fMax, const float &fDefault);	// ����
	//���݂̒l�擾����
	float GetNum(void) { return m_fNum; }
	//���݂̒l�ݒ菈��
	void SetNum(const float &fNum);
	//�f�t�H���g�l�擾����
	float GetDefaultNum(void) { return m_fDefault; }
	//�f�t�H���g�ɂ��邩�ǂ����擾����
	bool GetDefault(void) { return m_bDefault; }
	//�f�t�H���g�ɂ��邩�ǂ����ݒ菈��
	void SetDefault(const bool &bDefault) { m_bDefault = bDefault; }

private:
	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_barSize;			//�o�[�̃T�C�Y
	D3DXVECTOR3 m_circleSize;		//�~�̃T�C�Y
	float m_fMin;					//�ŏ��l
	float m_fMax;					//�ő�l
	float m_fDefault;				//�f�t�H���g�l
	float m_fNum;					//���݂̒l
	CObject2D *m_pCircle;			//��������~
	CObject2D *m_pBar;				//�o�[
	bool m_bClick;					//�N���b�N�������ǂ���
	CCounter *m_pCounter;			//�J�E���^�[�̃|�C���^
	bool m_bDefault;				//�f�t�H���g��Ԃɖ߂����ǂ���
};

#endif