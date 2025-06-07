#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Renderer.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID); // VAOを生成
	
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID); // VAOを削除
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();

	vb.Bind(); // VertexBufferをバインドして有効にする

	const auto& elements = layout.GetElements(); // レイアウトから要素を取得
	unsigned int offset = 0; // オフセットを初期化

	for (unsigned int i = 0; i < elements.size(); i++) // 各要素に対して
	{
		const auto& element = elements[i]; // 要素を取得

		//Indexを起動(layout set)
		GLCall(glEnableVertexAttribArray(i)); // 頂点属性を有効にする(iは属性のインデックス)
		// OpenGLにどうやってデータを扱うのを教える
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		// index(index、一組何個、データ型、Normalizeするか、二つ頂点までのByte数、ポインター)
		offset += element.count * element.GetSizeOfType(element.type); // オフセットを更新

	}
	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID)); // VAOをバインドして有効にする
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0)); // VAOのバインドを解除
}

