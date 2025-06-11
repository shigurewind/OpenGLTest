#include "Renderer.h"

#include <iostream> // �W�����o�̓��C�u����

void GLClearError() // OpenGL�̃G���[���N���A����֐�
{
	while (glGetError() != GL_NO_ERROR);// �G���[���Ȃ��Ȃ�܂Ń��[�v
}

bool GLLogCall(const char* function, const char* file, int line) // OpenGL�̃G���[���`�F�b�N����֐�
{
	while (GLenum error = glGetError()) // �G���[��������胋�[�v
	{
		std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << ":" << line << std::endl; // �G���[���b�Z�[�W��\��
		return false; // �G���[�����������ꍇ��false��Ԃ�
	}

	return true; // �G���[���������Ȃ������ꍇ��true��Ԃ�
}


//Renderer::Renderer() // �R���X�g���N�^
//{
//	// OpenGL�̏�������ݒ肪�K�v�ȏꍇ�͂����ɋL�q
//}
//
//Renderer::~Renderer() // �f�X�g���N�^
//{
//	// OpenGL�̃��\�[�X������K�v�ȏꍇ�͂����ɋL�q
//	// �Ⴆ�΁AVAO��VBO�̍폜�Ȃ�
//}

void Renderer::Clear() const // ��ʂ��N���A����֐�
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT)); // �J���[�o�b�t�@�Ɛ[�x�o�b�t�@���N���A
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();// �V�F�[�_�[�v���O�������o�C���h����
	va.Bind();// ���_�z��I�u�W�F�N�g���o�C���h����
	ib.Bind();// �C���f�b�N�X�o�b�t�@���o�C���h����

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));//IndexBuffer���g���ĕ`�悷��(�K��unsigned int���g��)
}



