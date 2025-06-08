
#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

//数学ライブラリ
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"





int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGLのメジャーバージョンを3に設定
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGLのマイナーバージョンを3に設定
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGLのコアプロファイルを使用

    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	glfwSwapInterval(5); // VSyncを有効にする(1はモニターのフレームレート同期する)

    if (glewInit() != GLEW_OK) {
        std::cout << "Error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

	{ // スコープを作成してリソースの管理を行う

	//位置とテクスチャのデータ構造を作る
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,//0
			 0.5f, -0.5f, 1.0f, 0.0f,//1
			 0.5f,  0.5f, 1.0f, 1.0f,//2
			-0.5f,  0.5f, 0.0f, 1.0f,//3

		};

		//頂点のインデックスを作る
		unsigned int indices[] = {
			0, 1, 2, // 一つ目の三角形
			2, 3, 0  // 二つ目の三角形
		};

		glEnable(GL_BLEND); // ブレンドを有効にする
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ブレンド関数を設定(アルファブレンディングを有効にする)

		//vertex array abstractを作成----------------
		VertexArray va;
		//頂点バッファを作成
		VertexBuffer vb(positions, 4 * 4 * sizeof(float)); // (4つの頂点、各頂点は4つのfloat値を持つ)

		//頂点バッファのレイアウトを定義
		VertexBufferLayout layout;
		layout.Push<float>(2); // 2つのfloat値を持つ頂点属性を追加(位置データ)
		layout.Push<float>(2); // 2つのfloat値を持つ頂点属性を追加(テクスチャデータ)
		va.AddBuffer(vb, layout); // VAOに頂点バッファとレイアウトを追加
		//--------------------------------------------

		// インデックスバッファを作成
		IndexBuffer ib(indices, 6); // (6つのインデックスを持つ)


		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f); // 正射影行列を作成


		// シェーダーを読み込む
		Shader shader("res/shaders/Basic.shader"); // シェーダーを読み込む
		shader.Bind(); // シェーダーをバインドして使用可能にする
		shader.SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // シェーダーのuniform変数に色を設定(黄色)
		shader.SetUniformMat4f("u_MVP", proj); // シェーダーのuniform変数に正射影行列を設定

		va.UnBind(); // VAOのバインドを解除
		vb.UnBind(); // 頂点バッファのバインドを解除
		ib.UnBind(); // インデックスバッファのバインドを解除
		shader.UnBind(); // シェーダープログラムの使用を停止

		Renderer renderer; // レンダラーのインスタンスを作成
		

		//色の変数を作る
		float r = 1.0f; // 赤の値
		float increment = 0.05f; // 赤の値の増加量

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear(); // 画面をクリア

			//--------------
			shader.Bind(); // シェーダープログラムをバインドして使用可能にする
			shader.SetUniform4f("u_Color", r, 1.0f, 0.0f, 1.0f); // シェーダーのuniform変数に色を設定
			//--------------ここのコード省略したいならMaterialsを使う必要があります

			Texture texture("res/textures/enemy.png"); // テクスチャを読み込む
			texture.Bind(); // テクスチャをバインドして使用可能にする
			shader.SetUniform1i("u_Texture", 0); // シェーダーのuniform変数にテクスチャユニットを設定(0番目のテクスチャユニットを使用)

			renderer.Draw(va, ib, shader); // レンダラーを使用してVAOとIBOを描画


			//赤色の値を更新
			if (r > 1.0f)
			{
				increment = -0.05f;
			}
			else if (r < 0.0f)
			{
				increment = 0.05f;
			}

			r += increment; // 赤の値を更新


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		
	}

    glfwTerminate();
    return 0;
}