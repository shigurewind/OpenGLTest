#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout; // �O���錾

class VertexArray
{
private:
	unsigned int m_RendererID; // OpenGL�̃����_���[ID

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void UnBind() const;
	

};

