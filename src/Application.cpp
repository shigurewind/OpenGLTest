
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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"





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
			100.0f, 100.0f, 0.0f, 0.0f,//0
			200.0f, 100.0f, 1.0f, 0.0f,//1
			200.0f, 200.0f, 1.0f, 1.0f,//2
			100.0,  200.0f, 0.0f, 1.0f,//3

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


		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f); // 正射影行列を作成
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0, 0)); // ビュー行列を作成

		// シェーダーを読み込む
		Shader shader("res/shaders/Basic.shader"); // シェーダーを読み込む
		shader.Bind(); // シェーダーをバインドして使用可能にする
		shader.SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // シェーダーのuniform変数に色を設定(黄色)

		va.UnBind(); // VAOのバインドを解除
		vb.UnBind(); // 頂点バッファのバインドを解除
		ib.UnBind(); // インデックスバッファのバインドを解除
		shader.UnBind(); // シェーダープログラムの使用を停止

		Renderer renderer; // レンダラーのインスタンスを作成

		//ImGui
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfwGL3_Init(window, true);

		ImGui::StyleColorsDark();

		glm::vec3 translation(200, 200, 0);
		
		

		//色の変数を作る
		float r = 1.0f; // 赤の値
		float increment = 0.05f; // 赤の値の増加量

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear(); // 画面をクリア

			//ImGuiの新しいフレームを開始
			ImGui_ImplGlfwGL3_NewFrame();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation); // モデル行列

			glm::mat4 mvp = proj * view * model; // モデルビュー投影行列を計算


			//--------------
			shader.Bind(); // シェーダープログラムをバインドして使用可能にする
			shader.SetUniform4f("u_Color", r, 1.0f, 0.0f, 1.0f); // シェーダーのuniform変数に色を設定

			shader.SetUniformMat4f("u_MVP", mvp); // シェーダーのuniform変数に正射影行列を設定

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

			//ImGuiの内容
			{
				ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		
	}

	//ImGuiの終了処理
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}