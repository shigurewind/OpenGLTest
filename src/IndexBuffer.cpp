#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count) // コンストラクタの初期化リストでm_Countを初期化
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint)); // unsigned intとGLuintのサイズが同じであることを確認

	GLCall(glGenBuffers(1, &m_RendererID)); // バッファオブジェクトを生成
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // バッファをバインド
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); // データをバッファに転送
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID)); // バッファオブジェクトを削除
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // バッファをバインド
}

void IndexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // バッファのバインドを解除
}