#include "Renderer.h"

#include <iostream> // 標準入出力ライブラリ

void GLClearError() // OpenGLのエラーをクリアする関数
{
	while (glGetError() != GL_NO_ERROR);// エラーがなくなるまでループ
}

bool GLLogCall(const char* function, const char* file, int line) // OpenGLのエラーをチェックする関数
{
	while (GLenum error = glGetError()) // エラーがある限りループ
	{
		std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << ":" << line << std::endl; // エラーメッセージを表示
		return false; // エラーが発生した場合はfalseを返す
	}

	return true; // エラーが発生しなかった場合はtrueを返す
}


//Renderer::Renderer() // コンストラクタ
//{
//	// OpenGLの初期化や設定が必要な場合はここに記述
//}
//
//Renderer::~Renderer() // デストラクタ
//{
//	// OpenGLのリソース解放が必要な場合はここに記述
//	// 例えば、VAOやVBOの削除など
//}

void Renderer::Clear() const // 画面をクリアする関数
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT)); // カラーバッファと深度バッファをクリア
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();// シェーダープログラムをバインドする
	va.Bind();// 頂点配列オブジェクトをバインドする
	ib.Bind();// インデックスバッファをバインドする

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));//IndexBufferを使って描画する(必ずunsigned intを使う)
}



