//===========================================
//�Q�[������
//Author:��������
//===========================================
#ifndef _GAME01_H_
#define _GAME01_H_
#include "object.h"
#include "main.h"

//�O���錾
class CPlayer;
class CMeshField;
class CEnemy;
class CObject2D;

//================================================
//�}�N����`
//================================================

//========================================================
//�\���̂̒�`
//========================================================

//================================================
//�N���X�̒�`
//================================================
class CGame01 : public CObject
{
public:
	CGame01(CObject::PRIORITY Priority = CObject::PRIORITY::NONE);		//�R���X�g���N�^
	~CGame01();									//�f�X�g���N�^

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool MapLimit(CObject* pObj);		//�}�b�v�̈ړ�����
	bool GetAllConnect(void) { return m_bAllConnect; }
	void FirstContact(void);
	//�G�̃��f���擾����
	vector<CEnemy*> GetEnemy(void);

private:
	void LoadModelTxt(const string &Pas);			//���f�����[�h����
		
	//�����o�ϐ�
	CPlayer *m_pPlayer;					//�v���C���[�̃|�C���^
	CObject2D *m_now_loding;
	CMeshField *m_pMeshField;			//���b�V���t�B�[���h�̃|�C���^
	vector<CEnemy*> m_pEnemy;					//�G�̃��f���̃|�C���^
	int m_count_pattern = 0;
	int m_pattern_tex = 0;
	bool m_bAllConnect;
};	

#endif // !_GAME01_H_