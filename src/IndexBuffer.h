#pragma once


class IndexBuffer
{
private:
	unsigned int m_RendererID; // OpenGL�̃o�b�t�@�I�u�W�F�N�g��ID���i�[����ϐ�
	unsigned int m_Count; // �C���f�b�N�X�̐����i�[����ϐ�

public:
	IndexBuffer(const unsigned int* data, unsigned int count );
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline unsigned int GetCount() const { return m_Count; } // �C���f�b�N�X�̐����擾����֐�
};



