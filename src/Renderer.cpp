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



