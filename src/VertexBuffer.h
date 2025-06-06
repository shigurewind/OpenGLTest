#pragma once


class VertexBuffer
{
private:
	unsigned int m_RendererID; // OpenGL�̃o�b�t�@�I�u�W�F�N�g��ID���i�[����ϐ�

public:
	VertexBuffer(const void* data, unsigned int size );
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;


};



