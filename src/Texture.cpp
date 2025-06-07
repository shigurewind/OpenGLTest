#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(true); // �摜��Y���𔽓]���ēǂݍ��ސݒ�(OpenGL�͉�����APNG�͏ォ��)
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // �摜��ǂݍ���(4��RGBA�`���œǂݍ���)

	glGenTextures(1, &m_RendererID); // OpenGL�̃e�N�X�`���I�u�W�F�N�g�𐶐�
	glBindTexture(GL_TEXTURE_2D, m_RendererID); // �e�N�X�`�����o�C���h

	//default�̃e�N�X�`���p�����[�^��ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // �e�N�X�`���̍ŏ��t�B���^�����O��ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // �e�N�X�`���̊g��t�B���^�����O��ݒ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // �e�N�X�`����S���̃��b�s���O��ݒ�(X)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // �e�N�X�`����T���̃��b�s���O��ݒ�(Y)

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer); // �e�N�X�`���̉摜�f�[�^��ݒ�
	glBindTexture(GL_TEXTURE_2D, 0); // �e�N�X�`���̃o�C���h������

	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer); // ���[�J���o�b�t�@�����
		m_LocalBuffer = nullptr; // �|�C���^��nullptr�ɐݒ�
	}
	

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID); // OpenGL�̃e�N�X�`���I�u�W�F�N�g���폜
}

void Texture::Bind(unsigned int slot/*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot); // �A�N�e�B�u�ȃe�N�X�`�����j�b�g��ݒ�
	glBindTexture(GL_TEXTURE_2D, m_RendererID); // �e�N�X�`�����o�C���h
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0); // �e�N�X�`���̃o�C���h������
}