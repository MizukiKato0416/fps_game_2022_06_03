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
	int m_nCount;
};
//================================================
//�v���g�^�C�v�錾
//================================================
#endif // !_RESULT_H_