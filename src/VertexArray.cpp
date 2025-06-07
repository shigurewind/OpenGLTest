#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Renderer.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID); // VAO�𐶐�
	
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID); // VAO���폜
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();

	vb.Bind(); // VertexBuffer���o�C���h���ėL���ɂ���

	const auto& elements = layout.GetElements(); // ���C�A�E�g����v�f���擾
	unsigned int offset = 0; // �I�t�Z�b�g��������

	for (unsigned int i = 0; i < elements.size(); i++) // �e�v�f�ɑ΂���
	{
		const auto& element = elements[i]; // �v�f���擾

		//Index���N��(layout set)
		GLCall(glEnableVertexAttribArray(i)); // ���_������L���ɂ���(i�͑����̃C���f�b�N�X)
		// OpenGL�ɂǂ�����ăf�[�^�������̂�������
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		// index(index�A��g���A�f�[�^�^�ANormalize���邩�A����_�܂ł�Byte���A�|�C���^�[)
		offset += element.count * element.GetSizeOfType(element.type); // �I�t�Z�b�g���X�V

	}
	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID)); // VAO���o�C���h���ėL���ɂ���
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0)); // VAO�̃o�C���h������
}

