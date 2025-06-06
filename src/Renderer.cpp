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



