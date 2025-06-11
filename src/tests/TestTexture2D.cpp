#include "TestTexture2D.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"




//数学ライブラリ
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	TestTexture2D::TestTexture2D()
		:m_TranslationA(200.0f, 200.0f, 0.0f), m_TranslationB(400.0f, 300.0f, 0.0f),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
		
	{
		//位置とテクスチャのデータ構造を作る
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,//0
			 50.0f, -50.0f, 1.0f, 0.0f,//1
			 50.0f,  50.0f, 1.0f, 1.0f,//2
			-50.0f,  50.0f, 0.0f, 1.0f,//3

		};

		//頂点のインデックスを作る
		unsigned int indices[] = {
			0, 1, 2, // 一つ目の三角形
			2, 3, 0  // 二つ目の三角形
		};

		glEnable(GL_BLEND); // ブレンドを有効にする
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ブレンド関数を設定(アルファブレンディングを有効にする)

		
		m_VAO = std::make_unique<VertexArray>(); // 頂点配列オブジェクトを作成

		
		//頂点バッファを作成
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float)); // (4つの頂点、各頂点は4つのfloat値を持つ)

		//頂点バッファのレイアウトを定義
		VertexBufferLayout layout;
		layout.Push<float>(2); // 2つのfloat値を持つ頂点属性を追加(位置データ)
		layout.Push<float>(2); // 2つのfloat値を持つ頂点属性を追加(テクスチャデータ)

		m_VAO->AddBuffer(*m_VertexBuffer, layout); // VAOに頂点バッファとレイアウトを追加
		//--------------------------------------------

		// インデックスバッファを作成
		
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6); // インデックスバッファを作成



		// シェーダーを読み込む
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader"); // シェーダーを読み込む
		m_Shader->Bind(); // シェーダーをバインドして使用可能にする
		m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // シェーダーのuniform変数に色を設定(黄色)

		m_Texture = std::make_unique<Texture> ("res/textures/enemy.png"); // テクスチャを読み込む 
		
		m_Shader->SetUniform1i("u_Texture", 0); // シェーダーのuniform変数にテクスチャユニットを設定(0番目のテクスチャユニットを使用)

		

	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer renderer; // レンダラーを作成

		m_Texture->Bind(); // テクスチャをバインドして使用可能にする

		

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA); // モデル行列
			glm::mat4 mvp = m_Proj * m_View * model; // モデルビュー投影行列を計算
			m_Shader-> SetUniformMat4f("u_MVP", mvp); // シェーダーのuniform変数に正射影行列を設定
			m_Shader->Bind(); // シェーダープログラムをバインドして使用可能にする
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader); // レンダラーを使用してVAOとIBOを描画
		}


		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB); // モデル行列
			glm::mat4 mvp = m_Proj * m_View * model; // モデルビュー投影行列を計算
			m_Shader->SetUniformMat4f("u_MVP", mvp); // シェーダーのuniform変数に正射影行列を設定
			m_Shader->Bind(); // シェーダープログラムをバインドして使用可能にする
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader); // レンダラーを使用してVAOとIBOを描画
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 960.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}
}


