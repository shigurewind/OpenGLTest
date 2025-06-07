#pragma once
#include <vector>
#include <GL/glew.h> 
#include "Renderer.h" 


struct VertexBufferElement
{
	unsigned int type; // �f�[�^�^
	unsigned int count; // �v�f�̐�
	unsigned char normalized; // ���K���t���O

	static unsigned int GetSizeOfType(unsigned int type) // �f�[�^�^�̃T�C�Y���擾����ÓI���\�b�h
	{
		switch (type)
		{
		case GL_FLOAT:			return sizeof(float);
		case GL_UNSIGNED_INT:	return sizeof(unsigned int);
		case GL_UNSIGNED_BYTE:	return sizeof(unsigned char);
		
		}
		ASSERT(false); // �T�|�[�g����Ă��Ȃ��^�̏ꍇ�̓R���p�C���G���[�𔭐�������
		return 0; // �f�t�H���g�̖߂�l
	}

	VertexBufferElement(unsigned int t, unsigned int c, unsigned char n)
		: type(t), count(c), normalized(n) {
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements; // ���_�o�b�t�@�̗v�f���i�[����x�N�^�[
	unsigned int m_Stride; // �e���_�̃o�C�g���̍��v

public:
	VertexBufferLayout()
		: m_Stride(0) // �X�g���C�h��������
	{
	}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false); // �T�|�[�g����Ă��Ȃ��^�̏ꍇ�̓R���p�C���G���[�𔭐�������
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT); // �X�g���C�h���X�V
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT); // �X�g���C�h���X�V
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE , count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE); // �X�g���C�h���X�V
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

};
