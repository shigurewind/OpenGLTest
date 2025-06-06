#pragma once


class IndexBuffer
{
private:
	unsigned int m_RendererID; // OpenGLのバッファオブジェクトのIDを格納する変数
	unsigned int m_Count; // インデックスの数を格納する変数

public:
	IndexBuffer(const unsigned int* data, unsigned int count );
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_Count; } // インデックスの数を取得する関数
};



