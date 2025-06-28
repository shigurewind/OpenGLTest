#include "TestMultiTexture.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"




//���w���C�u����
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"




namespace test
{
	TestMultiTexture::TestMultiTexture()
		:m_Proj(glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_camera(0, 0, 0)
		
	{

		glDisable(GL_DEPTH_TEST);//2D�����_�����O��p

		m_Texture1 = std::make_unique<Texture>("res/textures/whale.png");
		m_Texture2 = std::make_unique<Texture>("res/textures/deer.png");

		m_TexEnemy = m_Texture1->GetRendererID();
		m_TexBullet = m_Texture2->GetRendererID();
		

		//�ʒu�ƐF�̃f�[�^�\�������
		float vertices[] = {
			//�ʒu(x, y, z)�A�F(r, g, b, a)�AUV���W(u, v)�ATextureID
			-1.5f, -0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,	//0
			-0.5f, -0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,	//1
			-0.5f,  0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,	//2
			-1.5f,  0.5f, 0.0f,	0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,	//3
														  			  
			 0.5f, -0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,	//4
			 1.5f, -0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,	//5
			 1.5f,  0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,	//6
			 0.5f,  0.5f, 0.0f,	1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f,	//7

		};

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
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 8 * 10 * sizeof(float)); // (8�̒��_�A�e���_��7��float�l������)

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
		m_Shader = std::make_unique<Shader>("res/shaders/MultiTexture.shader"); // �V�F�[�_�[��ǂݍ���
		m_Shader->Bind(); // �V�F�[�_�[���o�C���h���Ďg�p�\�ɂ���
		

		//auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures"); // �V�F�[�_�[��uniform�ϐ��̃��P�[�V�������擾
		int samplers[2] = { 0, 1 }; // �e�N�X�`�����j�b�g�̔z����쐬
		m_Shader->SetUniform1iv("u_Textures",2 , samplers); // �V�F�[�_�[��uniform�ϐ��Ƀe�N�X�`�����j�b�g0��ݒ�
		//glUniform1iv(loc, 2, samplers); // �V�F�[�_�[��uniform�ϐ��Ƀe�N�X�`�����j�b�g0��1��ݒ�


	}

	TestMultiTexture::~TestMultiTexture()
	{

	}

	void TestMultiTexture::OnUpdate(float deltaTime)
	{

		glBindTextureUnit(0, m_TexEnemy); // �e�N�X�`�����j�b�g0�ɓG�̃e�N�X�`�����o�C���h
		glBindTextureUnit(1, m_TexBullet); // �e�N�X�`�����j�b�g1�ɒe�̃e�N�X�`�����o�C���h

		m_View = glm::translate(glm::mat4(1.0f), glm::vec3(m_camera.x, m_camera.y, m_camera.z)); // �J�����̈ʒu���X�V
	}

	void TestMultiTexture::OnRender()
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

	void TestMultiTexture::OnImGuiRender()
	{
		
		ImGui::SliderFloat3("camera", &m_camera.x, -2.0f,2.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}
}


