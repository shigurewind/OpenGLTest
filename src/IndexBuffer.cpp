#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count) // �R���X�g���N�^�̏��������X�g��m_Count��������
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint)); // unsigned int��GLuint�̃T�C�Y�������ł��邱�Ƃ��m�F

	GLCall(glGenBuffers(1, &m_RendererID)); // �o�b�t�@�I�u�W�F�N�g�𐶐�
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // �o�b�t�@���o�C���h
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); // �f�[�^���o�b�t�@�ɓ]��
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID)); // �o�b�t�@�I�u�W�F�N�g���폜
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // �o�b�t�@���o�C���h
}

void IndexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // �o�b�t�@�̃o�C���h������
}