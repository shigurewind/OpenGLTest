#include "TestTexture2D.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"




//���w���C�u����
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
	TestTexture2D::TestTexture2D()
		:m_TranslationA(200.0f, 200.0f, 0.0f), m_TranslationB(400.0f, 300.0f, 0.0f),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
		
	{
		//�ʒu�ƃe�N�X�`���̃f�[�^�\�������
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,//0
			 50.0f, -50.0f, 1.0f, 0.0f,//1
			 50.0f,  50.0f, 1.0f, 1.0f,//2
			-50.0f,  50.0f, 0.0f, 1.0f,//3

		};

		//���_�̃C���f�b�N�X�����
		unsigned int indices[] = {
			0, 1, 2, // ��ڂ̎O�p�`
			2, 3, 0  // ��ڂ̎O�p�`
		};

		glEnable(GL_BLEND); // �u�����h��L���ɂ���
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // �u�����h�֐���ݒ�(�A���t�@�u�����f�B���O��L���ɂ���)

		
		m_VAO = std::make_unique<VertexArray>(); // ���_�z��I�u�W�F�N�g���쐬

		
		//���_�o�b�t�@���쐬
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float)); // (4�̒��_�A�e���_��4��float�l������)

		//���_�o�b�t�@�̃��C�A�E�g���`
		VertexBufferLayout layout;
		layout.Push<float>(2); // 2��float�l�������_������ǉ�(�ʒu�f�[�^)
		layout.Push<float>(2); // 2��float�l�������_������ǉ�(�e�N�X�`���f�[�^)

		m_VAO->AddBuffer(*m_VertexBuffer, layout); // VAO�ɒ��_�o�b�t�@�ƃ��C�A�E�g��ǉ�
		//--------------------------------------------

		// �C���f�b�N�X�o�b�t�@���쐬
		
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6); // �C���f�b�N�X�o�b�t�@���쐬



		// �V�F�[�_�[��ǂݍ���
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader"); // �V�F�[�_�[��ǂݍ���
		m_Shader->Bind(); // �V�F�[�_�[���o�C���h���Ďg�p�\�ɂ���
		m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // �V�F�[�_�[��uniform�ϐ��ɐF��ݒ�(���F)

		m_Texture = std::make_unique<Texture> ("res/textures/enemy.png"); // �e�N�X�`����ǂݍ��� 
		
		m_Shader->SetUniform1i("u_Texture", 0); // �V�F�[�_�[��uniform�ϐ��Ƀe�N�X�`�����j�b�g��ݒ�(0�Ԗڂ̃e�N�X�`�����j�b�g���g�p)

		

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

		Renderer renderer; // �����_���[���쐬

		m_Texture->Bind(); // �e�N�X�`�����o�C���h���Ďg�p�\�ɂ���

		

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA); // ���f���s��
			glm::mat4 mvp = m_Proj * m_View * model; // ���f���r���[���e�s����v�Z
			m_Shader-> SetUniformMat4f("u_MVP", mvp); // �V�F�[�_�[��uniform�ϐ��ɐ��ˉe�s���ݒ�
			m_Shader->Bind(); // �V�F�[�_�[�v���O�������o�C���h���Ďg�p�\�ɂ���
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader); // �����_���[���g�p����VAO��IBO��`��
		}


		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB); // ���f���s��
			glm::mat4 mvp = m_Proj * m_View * model; // ���f���r���[���e�s����v�Z
			m_Shader->SetUniformMat4f("u_MVP", mvp); // �V�F�[�_�[��uniform�ϐ��ɐ��ˉe�s���ݒ�
			m_Shader->Bind(); // �V�F�[�_�[�v���O�������o�C���h���Ďg�p�\�ɂ���
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader); // �����_���[���g�p����VAO��IBO��`��
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 960.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}
}


