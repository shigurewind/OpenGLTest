#pragma once
#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID; // �e�N�X�`���̃����_���[ID
	std::string m_FilePath; // �e�N�X�`���̃t�@�C���p�X
	unsigned char* m_LocalBuffer; // ���[�J���o�b�t�@�ւ̃|�C���^
	int m_Width, m_Height, m_BPP; // ���A�����A�r�b�g��
	
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	unsigned int GetRendererID() const { return m_RendererID; }
	
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};



