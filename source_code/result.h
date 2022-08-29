//===========================================
//���U���g����
//Author:��������
//===========================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "object.h"

//================================================
//�O���錾
//================================================
class CObject2D;
class CCounter;
class CLetter;

//================================================
//�N���X�̒�`
//================================================
//�N���X�̒�`
class CResult : public CObject
{
public:
	CResult(PRIORITY priorty = PRIORITY::NONE);				//�R���X�g���N�^
	~CResult();				//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	vector<CObject2D*> m_Ui;
	vector<CCounter*> m_Countor;
	vector<CLetter*> m_name_font;// ���O�t�H���g
	vector<wstring> m_key_name;	// ���O1��������
	vector<string> m_letter_single;	// ���O
	int m_count_letter;	// �e�L�X�g�J�E���g
	int m_letter_limitl;	// ��������
	int m_nCount;
};
//================================================
//�v���g�^�C�v�錾
//================================================
#endif // !_RESULT_H_