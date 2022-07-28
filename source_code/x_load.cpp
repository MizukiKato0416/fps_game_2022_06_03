//=============================================================================
//X�t�@�C���ǂݍ��ݏ���
//Author:��������
//=============================================================================
#include "x_load.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CXload::CXload()
{
	m_file_data.type.clear();
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CXload::~CXload()
{

}

//=============================================================================
//������
//=============================================================================
void CXload::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	vector<string> folder_name;	// �t�H���_�̕ۑ��o�b�t�@
	int pas_element;	// �p�X�̗v�f��
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X���擾����

	// �t�@�C����ǂݍ���
	m_all_file = CFileLoad::LoadFile("data\\MODEL\\");
	// �p�X�Ɩ��O���擾
	m_file_data = CFileLoad::CreateFilePasElement(m_all_file, "data\\MODEL\\");

	// �p�X�̗v�f��
	pas_element = m_file_data.file_name_pas.second.size();

	for (int count_element = 0; count_element < pas_element; count_element++)
	{
		// �^���񋓌^�����
		m_file_data.type[m_file_data.file_name_pas.second[count_element]] = count_element;
	}

	// �p�X�̗v�f�����擾
	m_nNum = m_file_data.file_name_pas.first.size();

	// �T�C�Y����
	for (int nCntTex = 0; nCntTex < m_nNum; nCntTex++)
	{
		LPD3DXBUFFER buf;
		DWORD num_buf;
		LPD3DXMESH mesh_buf;

		//�v���C���[�̑�
		D3DXLoadMeshFromX(m_file_data.file_name_pas.first[nCntTex].c_str(), D3DXMESH_SYSTEMMEM, pDevice, NULL,
			              &buf, NULL, &num_buf, &mesh_buf);
		m_aNumMat.push_back(num_buf);
		m_apBuffMat.push_back(buf);
		m_apMesh.push_back(mesh_buf);
	}

	//X�t�@�C���ǂݍ���
}	

//=============================================================================
//�I��
//=============================================================================
void CXload::Uninit(void)
{
	for (int nCntXload = 0; nCntXload < m_nNum; nCntXload++)
	{
		//���b�V���̔j��
		if (m_apMesh[nCntXload] != NULL)
		{
			m_apMesh[nCntXload]->Release();
			m_apMesh[nCntXload] = NULL;
		}

		//�}�e���A���̔j��
		if (m_apBuffMat[nCntXload] != NULL)
		{
			m_apBuffMat[nCntXload]->Release();
			m_apBuffMat[nCntXload] = NULL;
		}
	}
}

//=============================================================================
//���b�V���̊��蓖��
//=============================================================================
LPD3DXMESH CXload::GetMesh(const string &sPas)
{
	return m_apMesh[m_file_data.type[sPas]];
}

//=============================================================================
//�}�e���A�����蓖��
//=============================================================================
LPD3DXBUFFER CXload::GetBuffMat(const string &sPas)
{
	return m_apBuffMat[m_file_data.type[sPas]];
}

//=============================================================================
//�}�e���A�������蓖��
//=============================================================================
DWORD CXload::GetNumMat(const string &sPas)
{
	return m_aNumMat[m_file_data.type[sPas]];
}
