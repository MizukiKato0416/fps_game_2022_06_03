//=============================================================================
//
// �e�N�X�`������ [texture.h]
// Author : �H������
//
//=============================================================================
#include "texture.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTexture::CTexture()
{
	m_file_data.type.clear();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// �e�N�X�`���̐���
//=============================================================================
void CTexture::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// �f�o�C�X�̃|�C���^
	vector<string> folder_name;	// �t�H���_�̕ۑ��o�b�t�@
	int element_max;		// �e�N�X�`���J�E���g�l
	int file_element;	// �e�L�X�g�t�@�C���̕�����T�C�Y
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���擾����

	// �t�@�C����ǂݍ���
	m_all_file = CFileLoad::Load("data\\TEXTURE\\");
	file_element = m_all_file.size();

	// �e�L�X�g�t�@�C���̃T�C�Y���̃��[�v
	for (int element_count = 0; element_count < file_element; element_count++)
	{
		// �p�X�̕ۑ�
		m_file_data.file_name_pas.first.push_back(m_all_file[element_count]);
		m_file_data.file_name_pas.second.push_back(m_all_file[element_count]);
	}

	// �p�X�̗v�f��
	file_element = m_file_data.file_name_pas.second.size();

	// �p�X�̗v�f���̃T�C�Y���̃��[�v
	for (int element_count = 0; element_count < file_element; element_count++)
	{
		// �p�X���������Ă���
		if (m_file_data.file_name_pas.second[element_count].find("data\\TEXTURE\\") != string::npos)
		{
			// �g���q�����Ă�����
			if (m_file_data.file_name_pas.second[element_count].find(".") != string::npos)
			{
				// �t�H���_�̖��O�T�C�Y���擾
				int folder_max = folder_name.size();
				for (int folder_count = 0; folder_count < folder_max; folder_count++)
				{
					// ���O��ۑ����鏊�Ƀp�X���������Ă�����
					if (m_file_data.file_name_pas.second[element_count].find(folder_name[folder_count]) != string::npos)
					{
						// �t�H���_�̖��O�̃T�C�Y���擾
						int name_size = folder_name[folder_count].size();
						for (int count_erase = 0; count_erase < name_size + 1; count_erase++)
						{
							// ���O�������c��
							m_file_data.file_name_pas.second[element_count].erase(m_file_data.file_name_pas.second[element_count].begin());
						}
					}
				}
			}
			// �g���q���t���Ă��Ȃ�(�t�H���_�Ȃ̂ŏ���)
			else
			{
				folder_name.push_back(m_file_data.file_name_pas.second[element_count]);
				m_file_data.file_name_pas.second.erase(m_file_data.file_name_pas.second.begin() + element_count);
				m_file_data.file_name_pas.first.erase(m_file_data.file_name_pas.first.begin() + element_count);
				file_element--;
				element_count--;
			}
		}
	}

	// �p�X�̗v�f��
	element_max = m_file_data.file_name_pas.second.size();

	for (int count_element = 0; count_element < element_max; count_element++)
	{
		// �^���񋓌^�����
		m_file_data.type[m_file_data.file_name_pas.second[count_element]] = count_element;
	}

	// �p�X�̗v�f�����擾
	m_num_tex = m_file_data.file_name_pas.first.size();

	// �T�C�Y����
	for (int nCntTex = 0; nCntTex < m_num_tex; nCntTex++)
	{
		LPDIRECT3DTEXTURE9 pTexBuffer = NULL;	// �e�N�X�`���̃o�b�t�@
		//�e�N�X�`���̐���
		D3DXCreateTextureFromFile(	pDevice,
									m_file_data.file_name_pas.first[nCntTex].c_str(),
									&pTexBuffer);
		// vector�Ɋi�[
		m_texture.push_back(pTexBuffer);
	}
}

//=============================================================================
// �I��
//=============================================================================
void CTexture::Uninit(void)
{
	for (int nCntTexture = 0; nCntTexture < m_num_tex; nCntTexture++)
	{
		// �e�N�X�`���̔j��
		if (m_texture[nCntTexture] != NULL)
		{
			m_texture[nCntTexture]->Release();
			m_texture[nCntTexture] = NULL;
		}
	}
}