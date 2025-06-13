#pragma once
#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID; // テクスチャのレンダラーID
	std::string m_FilePath; // テクスチャのファイルパス
	unsigned char* m_LocalBuffer; // ローカルバッファへのポインタ
	int m_Width, m_Height, m_BPP; // 幅、高さ、ビット数
	
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	unsigned int GetRendererID() const { return m_RendererID; }
	
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};



