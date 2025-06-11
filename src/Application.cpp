
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

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"





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

	

		glEnable(GL_BLEND); // ブレンドを有効にする
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ブレンド関数を設定(アルファブレンディングを有効にする)

		

		Renderer renderer; // レンダラーのインスタンスを作成

		//ImGui
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest); // テストメニューのインスタンスを作成
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");// クリアカラーのテストを登録
		testMenu->RegisterTest<test::TestTexture2D>("2D Texture"); // 2Dテクスチャのテストを登録

		
		

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 背景色を設定
			/* Render here */
			renderer.Clear(); // 画面をクリア



			//ImGuiの新しいフレームを開始
			ImGui_ImplGlfwGL3_NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Tests"); // ImGuiのウィンドウを開始

				if (currentTest != testMenu && ImGui::Button("<-"))// テストメニューに戻る
				{
					delete currentTest;
					currentTest = testMenu; 
				}
				currentTest->OnImGuiRender(); // 現在のテストのImGuiレンダリング処理を呼び出す

				ImGui::End(); // ImGuiのウィンドウを終了
			}


			
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		if (currentTest != testMenu)
		{
			delete testMenu;
		}
		delete currentTest; // 現在のテストを削除
		
		
	}

	

	//ImGuiの終了処理
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}