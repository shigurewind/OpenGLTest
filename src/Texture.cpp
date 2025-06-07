#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(true); // 画像のY軸を反転して読み込む設定(OpenGLは下から、PNGは上から)
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // 画像を読み込む(4はRGBA形式で読み込む)

	glGenTextures(1, &m_RendererID); // OpenGLのテクスチャオブジェクトを生成
	glBindTexture(GL_TEXTURE_2D, m_RendererID); // テクスチャをバインド

	//defaultのテクスチャパラメータを設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // テクスチャの最小フィルタリングを設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // テクスチャの拡大フィルタリングを設定
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // テクスチャのS軸のラッピングを設定(X)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // テクスチャのT軸のラッピングを設定(Y)

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer); // テクスチャの画像データを設定
	glBindTexture(GL_TEXTURE_2D, 0); // テクスチャのバインドを解除

	if (m_LocalBuffer) {
		stbi_image_free(m_LocalBuffer); // ローカルバッファを解放
		m_LocalBuffer = nullptr; // ポインタをnullptrに設定
	}
	

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID); // OpenGLのテクスチャオブジェクトを削除
}

void Texture::Bind(unsigned int slot/*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot); // アクティブなテクスチャユニットを設定
	glBindTexture(GL_TEXTURE_2D, m_RendererID); // テクスチャをバインド
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0); // テクスチャのバインドを解除
}