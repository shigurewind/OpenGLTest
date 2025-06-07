#pragma once
#include <vector>
#include <GL/glew.h> 
#include "Renderer.h" 


struct VertexBufferElement
{
	unsigned int type; // データ型
	unsigned int count; // 要素の数
	unsigned char normalized; // 正規化フラグ

	static unsigned int GetSizeOfType(unsigned int type) // データ型のサイズを取得する静的メソッド
	{
		switch (type)
		{
		case GL_FLOAT:			return sizeof(float);
		case GL_UNSIGNED_INT:	return sizeof(unsigned int);
		case GL_UNSIGNED_BYTE:	return sizeof(unsigned char);
		
		}
		ASSERT(false); // サポートされていない型の場合はコンパイルエラーを発生させる
		return 0; // デフォルトの戻り値
	}

	VertexBufferElement(unsigned int t, unsigned int c, unsigned char n)
		: type(t), count(c), normalized(n) {
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements; // 頂点バッファの要素を格納するベクター
	unsigned int m_Stride; // 各頂点のバイト数の合計

public:
	VertexBufferLayout()
		: m_Stride(0) // ストライドを初期化
	{
	}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false); // サポートされていない型の場合はコンパイルエラーを発生させる
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT); // ストライドを更新
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT); // ストライドを更新
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE , count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE); // ストライドを更新
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

};
