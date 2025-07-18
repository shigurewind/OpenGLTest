#include "TestTextureCube.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"
#include <GLFW/glfw3.h>



//数学ライブラリ
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	TestTextureCube::TestTextureCube()
		:
		m_Proj(glm::perspective(45.0f, 960.0f / 640.0f, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f))),
		m_Model(glm::rotate(glm::mat4(1.0f), -55.0f, glm::vec3(1.0f, 0.0f, 0.0f))),
		cubePosition(0, 0, -3.0f)
	{
		//位置とテクスチャのデータ構造を作る
		float positions[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f

		};

		

		//頂点のインデックスを作る
		//unsigned int indices[] = {
		//	0, 1, 3, // 一つ目の三角形
		//	1, 2, 3  // 二つ目の三角形
		//};

		glEnable(GL_BLEND); // ブレンドを有効にする
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ブレンド関数を設定(アルファブレンディングを有効にする)
		
		
		m_VAO = std::make_unique<VertexArray>(); // 頂点配列オブジェクトを作成

		
		//頂点バッファを作成
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 36 * 5 * sizeof(float)); // (4つの頂点、各頂点は4つのfloat値を持つ)

		//頂点バッファのレイアウトを定義
		VertexBufferLayout layout;
		layout.Push<float>(3); // 3つのfloat値を持つ頂点属性を追加(位置データ)
		layout.Push<float>(2); // 2つのfloat値を持つ頂点属性を追加(テクスチャデータ)

		m_VAO->AddBuffer(*m_VertexBuffer, layout); // VAOに頂点バッファとレイアウトを追加
		//--------------------------------------------

		// インデックスバッファを作成
		
		//m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6); // インデックスバッファを作成



		// シェーダーを読み込む
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader"); // シェーダーを読み込む
		m_Shader->Bind(); // シェーダーをバインドして使用可能にする
		m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // シェーダーのuniform変数に色を設定(黄色)

		m_Texture = std::make_unique<Texture> ("res/textures/magicCat.jpg"); // テクスチャを読み込む 
		
		m_Shader->SetUniform1i("u_Texture", 0); // シェーダーのuniform変数にテクスチャユニットを設定(0番目のテクスチャユニットを使用)

		

	}

	TestTextureCube::~TestTextureCube()
	{

	}

	void TestTextureCube::OnUpdate(float deltaTime)
	{

	}

	void TestTextureCube::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Depth Buffer起用

		Renderer renderer; // レンダラーを作成

		m_Texture->Bind(); // テクスチャをバインドして使用可能にする

		
		{
			
			m_Shader->Bind(); // シェーダープログラムをバインドして使用可能にする
			//renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader); // レンダラーを使用してVAOとIBOを描画

			m_VAO->Bind();

			for (GLuint i = 0; i < 10; i++)
			{
				m_View = glm::translate(glm::mat4(1.0f), cubePosition);
				//m_Model = glm::rotate(glm::mat4(1.0f), (GLfloat)glfwGetTime() * 1.0f, cubePositions[i]);

				float angle = 20.0f * i;
				m_Model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
				m_Model = glm::rotate(m_Model, (GLfloat)glfwGetTime() + angle, glm::vec3(1.0f, 0.3f, 0.5f));

				glm::mat4 mvp = m_Proj * m_View * m_Model; // モデルビュー投影行列を計算
				m_Shader->SetUniformMat4f("u_MVP", mvp); // シェーダーのuniform変数に正射影行列を設定

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

		}


	}

	void TestTextureCube::OnImGuiRender()
	{
		//ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Position", &cubePosition.x, -5.0f, 5.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}
}


