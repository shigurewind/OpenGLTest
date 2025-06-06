#pragma once


class VertexBuffer
{
private:
	unsigned int m_RendererID; // OpenGLのバッファオブジェクトのIDを格納する変数

public:
	VertexBuffer(const void* data, unsigned int size );
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;


};



