#include "TestDynamicGeometry.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"

#include <array>




//数学ライブラリ
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


struct Vec2
{
	float x, y;
};

struct Vec3
{
	float x, y, z;
};

struct Vec4
{
	float x, y, z, w;
};

struct Vertex
{
	Vec3 Position;
	Vec4 Color;
	Vec2 TexCoords;
	float TexID;
};

static std::array<Vertex, 4> CreateQuad(float x, float y, float textureID)
{
	float size = 1.0f; // 四角形のサイズ

	Vertex v0;
	v0.Position = { x, y, 0.0f };
	v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };  
	v0.TexCoords = { 0.0f, 0.0f };
	v0.TexID = textureID;

	Vertex v1;
	v1.Position = { x + size, y, 0.0f };
	v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f }; 
	v1.TexCoords = { 1.0f, 0.0f };
	v1.TexID = textureID;

	Vertex v2;
	v2.Position = { x + size, y + size, 0.0f };
	v2.Color = { 0.18f, 0.6f, 0.96f, 1.0f }; 
	v2.TexCoords = { 1.0f, 1.0f };
	v2.TexID = textureID;

	Vertex v3;
	v3.Position = { x, y + size, 0.0f };
	v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f }; 
	v3.TexCoords = { 0.0f, 1.0f };
	v3.TexID = textureID;

	return { v0, v1, v2, v3 };


}




namespace test
{
	TestDynamicGeometry::TestDynamicGeometry()
		:m_Proj(glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_camera(0, 0, 0)
		
	{

		m_Texture1 = std::make_unique<Texture>("res/textures/enemy.png");
		m_Texture2 = std::make_unique<Texture>("res/textures/bullet.png");

		m_TexEnemy = m_Texture1->GetRendererID();
		m_TexBullet = m_Texture2->GetRendererID();
		

		

		//頂点のインデックスを作る
		unsigned int indices[] = {
			0, 1, 2,	// 一つ目の三角形
			2, 3, 0,	// 二つ目の三角形

			4, 5, 6,	// 三つ目の三角形
			6, 7, 4,	// 四つ目の三角形
		};

		glEnable(GL_BLEND); // ブレンドを有効にする
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ブレンド関数を設定(アルファブレンディングを有効にする)
		
		
		m_VAO = std::make_unique<VertexArray>(); // 頂点配列オブジェクトを作成

		
		//頂点バッファを作成
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 1000); // とりあえず1000個の頂点を格納できるバッファを作成

		//頂点バッファのレイアウトを定義
		VertexBufferLayout layout;
		layout.Push<float>(3); // 3つのfloat値を持つ頂点属性を追加(位置データ)
		layout.Push<float>(4); // 4つのfloat値を持つ頂点属性を追加(色)
		layout.Push<float>(2); // 2つのfloat値を持つ頂点属性を追加(UV座標)
		layout.Push<float>(1); // 1つのfloat値を持つ頂点属性を追加(TextureID)

		m_VAO->AddBuffer(*m_VertexBuffer, layout); // VAOに頂点バッファとレイアウトを追加
		//--------------------------------------------

		// インデックスバッファを作成
		
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12); // インデックスバッファを作成



		// シェーダーを読み込む
		m_Shader = std::make_unique<Shader>("res/shaders/DynamicGeometry.shader"); // シェーダーを読み込む
		m_Shader->Bind(); // シェーダーをバインドして使用可能にする
		

		//auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures"); // シェーダーのuniform変数のロケーションを取得
		int samplers[2] = { 0, 1 }; // テクスチャユニットの配列を作成
		m_Shader->SetUniform1iv("u_Textures",2 , samplers); // シェーダーのuniform変数にテクスチャユニット0を設定
		//glUniform1iv(loc, 2, samplers); // シェーダーのuniform変数にテクスチャユニット0と1を設定


	}

	TestDynamicGeometry::~TestDynamicGeometry()
	{

	}

	void TestDynamicGeometry::OnUpdate(float deltaTime)
	{

		//位置と色のデータ構造を作る
		//float vertices[] = {
		//	//位置(x, y, z)、色(r, g, b, a)、UV座標(u, v)、TextureID
		//	-1.5f, -0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,	//0
		//	-0.5f, -0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,	//1
		//	-0.5f,  0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,	//2
		//	-1.5f,  0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,	//3

		//	 0.5f, -0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,	//4
		//	 1.5f, -0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,	//5
		//	 1.5f,  0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,	//6
		//	 0.5f,  0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f,	//7

		//};

		auto q0 = CreateQuad(m_QuadPosition[0], m_QuadPosition[1], 0); // 敵の四角形を作成
		auto q1 = CreateQuad(0.5f, -0.5f, 1); // 弾の四角形を作成

		Vertex vertices[8]; // 8つの頂点を格納する配列を作成
		memcpy(vertices, q0.data(), sizeof(Vertex) * q0.size()); 
		memcpy(vertices + q0.size(), q1.data(), sizeof(Vertex) * q1.size());

		//dynamic geometryの更新処理をここに書く
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer->GetRendererID()); // VAOのバッファをバインド
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 8, vertices); // バッファのデータを更新(ここでは初期化のみ)


		glBindTextureUnit(0, m_TexEnemy); // テクスチャユニット0に敵のテクスチャをバインド
		glBindTextureUnit(1, m_TexBullet); // テクスチャユニット1に弾のテクスチャをバインド

		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(m_camera.x, m_camera.y, m_camera.z)); // カメラの位置を更新
	}

	void TestDynamicGeometry::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer renderer; // レンダラーを作成

		//m_Texture->Bind(); // テクスチャをバインドして使用可能にする


		{
			
			m_Shader-> SetUniformMat4f("u_Proj", m_Proj); // シェーダーのuniform変数に正射影行列を設定
			m_Shader-> SetUniformMat4f("u_View", m_View); // シェーダーのuniform変数に正射影行列を設定
			m_Shader->Bind(); // シェーダープログラムをバインドして使用可能にする
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader); // レンダラーを使用してVAOとIBOを描画
		}


	}

	void TestDynamicGeometry::OnImGuiRender()
	{
		
		ImGui::SliderFloat3("camera", &m_camera.x, -2.0f,2.0f);

		ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.1f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}
}


