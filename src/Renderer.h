#pragma once
#include <GL/glew.h> // OpenGLのヘッダーファイル
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h" 


// OpenGLのエラーをチェックするためのヘッダーファイル
#define ASSERT(x) if (!(x)) __debugbreak(); // アサーションの定義,MSVCのデバッグブレークポイントを使用
//全てののGL関数の呼び出しをラップするマクロを定義
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__ )) // OpenGLのエラーをチェックするマクロ


void GLClearError(); // OpenGLのエラーをクリアする関数
bool GLLogCall(const char* function, const char* file, int line); // OpenGLのエラーをチェックする関数

class Renderer
{
private:

public:
	/*Renderer();
	~Renderer();*/
	void Clear() const; // 画面をクリアする関数
	
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};



