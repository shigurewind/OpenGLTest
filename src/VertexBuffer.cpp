#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID)); // �o�b�t�@�I�u�W�F�N�g�𐶐�
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // �o�b�t�@���o�C���h
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // �f�[�^���o�b�t�@�ɓ]��
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID)); // �o�b�t�@�I�u�W�F�N�g���폜
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // �o�b�t�@���o�C���h
}

void VertexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // �o�b�t�@�̃o�C���h������
}