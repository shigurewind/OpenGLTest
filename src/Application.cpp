
#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"





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

	//位置のデータ構造を作る
		float positions[] = {
			-0.5f, -0.5f,//0
			 0.5f, -0.5f,//1
			 0.5f,  0.5f,//2
			-0.5f,  0.5f,//3

		};

		//頂点のインデックスを作る
		unsigned int indices[] = {
			0, 1, 2, // 一つ目の三角形
			2, 3, 0  // 二つ目の三角形
		};

		//vertex array abstractを作成----------------
		VertexArray va;
		//頂点バッファを作成
		VertexBuffer vb(positions, 4 * 2 * sizeof(float)); // (4つの頂点、各頂点は2つのfloat値を持つ)

		//頂点バッファのレイアウトを定義
		VertexBufferLayout layout;
		layout.Push<float>(2); // 2つのfloat値を持つ頂点属性を追加(位置データ)
		va.AddBuffer(vb, layout); // VAOに頂点バッファとレイアウトを追加
		//--------------------------------------------

		// インデックスバッファを作成
		IndexBuffer ib(indices, 6); // (6つのインデックスを持つ)

		// シェーダーを読み込む
		Shader shader("res/shaders/Basic.shader"); // シェーダーを読み込む
		shader.Bind(); // シェーダーをバインドして使用可能にする
		shader.SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // シェーダーのuniform変数に色を設定(黄色)

		va.UnBind(); // VAOのバインドを解除
		vb.UnBind(); // 頂点バッファのバインドを解除
		ib.UnBind(); // インデックスバッファのバインドを解除
		shader.UnBind(); // シェーダープログラムの使用を停止
		

		//色の変数を作る
		float r = 1.0f; // 赤の値
		float increment = 0.05f; // 赤の値の増加量

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			////三角形を描画(古いバージョンのOpenGL)
			//glBegin(GL_TRIANGLES);
			//glVertex2f(-0.5f, -0.5f);
			//glVertex2f(0, 0.5f);
			//glVertex2f(0.5f, -0.5f);
			//glEnd();

			shader.Bind(); // シェーダープログラムをバインドして使用可能にする
			shader.SetUniform4f("u_Color", r, 1.0f, 0.0f, 1.0f); // シェーダーのuniform変数に色を設定
			


			va.Bind();
			ib.Bind(); // インデックスバッファをバインドして有効にする


			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));//Indexを使って描画する(必ずunsigned intを使う)

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