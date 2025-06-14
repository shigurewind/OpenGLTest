#include "TestDynamicGeometry.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"

#include <array>




//���w���C�u����
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
	float size = 1.0f; // �l�p�`�̃T�C�Y

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
		

		

		//���_�̃C���f�b�N�X�����
		unsigned int indices[] = {
			0, 1, 2,	// ��ڂ̎O�p�`
			2, 3, 0,	// ��ڂ̎O�p�`

			4, 5, 6,	// �O�ڂ̎O�p�`
			6, 7, 4,	// �l�ڂ̎O�p�`
		};

		glEnable(GL_BLEND); // �u�����h��L���ɂ���
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // �u�����h�֐���ݒ�(�A���t�@�u�����f�B���O��L���ɂ���)
		
		
		m_VAO = std::make_unique<VertexArray>(); // ���_�z��I�u�W�F�N�g���쐬

		
		//���_�o�b�t�@���쐬
		m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, sizeof(Vertex) * 1000); // �Ƃ肠����1000�̒��_���i�[�ł���o�b�t�@���쐬

		//���_�o�b�t�@�̃��C�A�E�g���`
		VertexBufferLayout layout;
		layout.Push<float>(3); // 3��float�l�������_������ǉ�(�ʒu�f�[�^)
		layout.Push<float>(4); // 4��float�l�������_������ǉ�(�F)
		layout.Push<float>(2); // 2��float�l�������_������ǉ�(UV���W)
		layout.Push<float>(1); // 1��float�l�������_������ǉ�(TextureID)

		m_VAO->AddBuffer(*m_VertexBuffer, layout); // VAO�ɒ��_�o�b�t�@�ƃ��C�A�E�g��ǉ�
		//--------------------------------------------

		// �C���f�b�N�X�o�b�t�@���쐬
		
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12); // �C���f�b�N�X�o�b�t�@���쐬



		// �V�F�[�_�[��ǂݍ���
		m_Shader = std::make_unique<Shader>("res/shaders/DynamicGeometry.shader"); // �V�F�[�_�[��ǂݍ���
		m_Shader->Bind(); // �V�F�[�_�[���o�C���h���Ďg�p�\�ɂ���
		

		//auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures"); // �V�F�[�_�[��uniform�ϐ��̃��P�[�V�������擾
		int samplers[2] = { 0, 1 }; // �e�N�X�`�����j�b�g�̔z����쐬
		m_Shader->SetUniform1iv("u_Textures",2 , samplers); // �V�F�[�_�[��uniform�ϐ��Ƀe�N�X�`�����j�b�g0��ݒ�
		//glUniform1iv(loc, 2, samplers); // �V�F�[�_�[��uniform�ϐ��Ƀe�N�X�`�����j�b�g0��1��ݒ�


	}

	TestDynamicGeometry::~TestDynamicGeometry()
	{

	}

	void TestDynamicGeometry::OnUpdate(float deltaTime)
	{

		//�ʒu�ƐF�̃f�[�^�\�������
		//float vertices[] = {
		//	//�ʒu(x, y, z)�A�F(r, g, b, a)�AUV���W(u, v)�ATextureID
		//	-1.5f, -0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,	//0
		//	-0.5f, -0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,	//1
		//	-0.5f,  0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,	//2
		//	-1.5f,  0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,	//3

		//	 0.5f, -0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,	//4
		//	 1.5f, -0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,	//5
		//	 1.5f,  0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,	//6
		//	 0.5f,  0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f,	//7

		//};

		auto q0 = CreateQuad(m_QuadPosition[0], m_QuadPosition[1], 0); // �G�̎l�p�`���쐬
		auto q1 = CreateQuad(0.5f, -0.5f, 1); // �e�̎l�p�`���쐬

		Vertex vertices[8]; // 8�̒��_���i�[����z����쐬
		memcpy(vertices, q0.data(), sizeof(Vertex) * q0.size()); 
		memcpy(vertices + q0.size(), q1.data(), sizeof(Vertex) * q1.size());

		//dynamic geometry�̍X�V�����������ɏ���
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer->GetRendererID()); // VAO�̃o�b�t�@���o�C���h
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 8, vertices); // �o�b�t�@�̃f�[�^���X�V(�����ł͏������̂�)


		glBindTextureUnit(0, m_TexEnemy); // �e�N�X�`�����j�b�g0�ɓG�̃e�N�X�`�����o�C���h
		glBindTextureUnit(1, m_TexBullet); // �e�N�X�`�����j�b�g1�ɒe�̃e�N�X�`�����o�C���h

		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(m_camera.x, m_camera.y, m_camera.z)); // �J�����̈ʒu���X�V
	}

	void TestDynamicGeometry::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Renderer renderer; // �����_���[���쐬

		//m_Texture->Bind(); // �e�N�X�`�����o�C���h���Ďg�p�\�ɂ���


		{
			
			m_Shader-> SetUniformMat4f("u_Proj", m_Proj); // �V�F�[�_�[��uniform�ϐ��ɐ��ˉe�s���ݒ�
			m_Shader-> SetUniformMat4f("u_View", m_View); // �V�F�[�_�[��uniform�ϐ��ɐ��ˉe�s���ݒ�
			m_Shader->Bind(); // �V�F�[�_�[�v���O�������o�C���h���Ďg�p�\�ɂ���
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader); // �����_���[���g�p����VAO��IBO��`��
		}


	}

	void TestDynamicGeometry::OnImGuiRender()
	{
		
		ImGui::SliderFloat3("camera", &m_camera.x, -2.0f,2.0f);

		ImGui::DragFloat2("Quad Position", m_QuadPosition, 0.1f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}
}


