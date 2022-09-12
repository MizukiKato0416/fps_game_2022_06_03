//=============================================================================
//
// �^�C�g������
// Author : ��������
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "object.h"
#include "input_keyboard.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CUi;
class CObject2D;
class CLetter;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTitle : public CObject
{
public:
	CTitle(PRIORITY priorty = PRIORITY::NONE);	// �f�t�H���g�R���X�g���N�^
	~CTitle();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void Draw(void);	// �`��
	void PasWord(void);

private:
	void Logo(void);	//���S�̏���

	vector<CUi*> m_ui;	// UI
	vector<string> m_type;	// �e�N�X�`����
	vector<CLetter*> m_name_font;// ���O�t�H���g
	vector<wstring> m_key_name;	// ���O1��������
	vector<string> m_letter_single;	// ���O
	CObject2D *m_pointor;	// �}�E�X�|�C���^�[
	CObject2D *m_name_box;	// �p�X���[�h���͗�
	string m_name;	// ���O
	int m_count_letter;	// �e�L�X�g�J�E���g
	int m_letter_limitl;	// ��������
	bool m_name_drop;	// ���O�ł�����
	int m_nCounter;			//�o�߃t���[���J�E���g�p
	CObject2D *m_pTitleLogo;	//�^�C�g�����S�̃|�C���^
};

#endif // !_TITLE_H_