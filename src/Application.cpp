
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
#include "tests/TestVertexColor.h"
#include "tests/TestMultiTexture.h"
#include "tests/TestDynamicGeometry.h"
#include "tests/TestTextureCube.h"
#include "tests/TestCamera.h"



test::Test* currentTest = nullptr;
test::TestMenu* testMenu = nullptr;

bool freeMouse = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* w, double x, double y);
void scroll_callback(GLFWwindow* w, double xo, double yo);

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

		//キーボード入力有効
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// 日本語フォントの指定
		//実際を表示したい日本語表示する前のダブルクォーテーションにu8を入れる（Unicode指定）
		io.Fonts->AddFontFromFileTTF(u8"c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

		
		testMenu = new test::TestMenu(currentTest); // テストメニューのインスタンスを作成
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");// クリアカラーのテストを登録
		testMenu->RegisterTest<test::TestTexture2D>("2D Texture");  // 2Dテクスチャのテストを登録
		testMenu->RegisterTest<test::TestVertexColor>("VertexColor");
		testMenu->RegisterTest<test::TestMultiTexture>("MultiTexture");
		testMenu->RegisterTest<test::TestDynamicGeometry>("DynamicGeometry");
		testMenu->RegisterTest<test::TestTextureCube>("TestTextureCube");
		testMenu->RegisterTest<test::TestCamera>("TestCamera");
		


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
				ImGui::Begin(u8"テスト実行"); // ImGuiのウィンドウを開始

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << "Key event: " << key << ", action: " << action << std::endl;

	//自由にマオス使えるスイッチ
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
		freeMouse = !freeMouse;
		glfwSetInputMode(window, GLFW_CURSOR,
			freeMouse ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	if (currentTest)
		currentTest->OnKeyEvent(key, action);
}


void mouse_callback(GLFWwindow* w, double x, double y) {
	if (!freeMouse && currentTest)
		currentTest->OnMouseMove(x, y);
}
void scroll_callback(GLFWwindow* w, double xo, double yo) {
	if (currentTest)
		currentTest->OnScroll(xo, yo);
}