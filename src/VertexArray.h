#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout; // 前方宣言

class VertexArray
{
private:
	unsigned int m_RendererID; // OpenGLのレンダラーID

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void UnBind() const;
	

};

