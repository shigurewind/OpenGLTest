#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout; // ‘O•ûéŒ¾

class VertexArray
{
private:
	unsigned int m_RendererID; // OpenGL‚ÌƒŒƒ“ƒ_ƒ‰[ID

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void UnBind() const;
	

};

