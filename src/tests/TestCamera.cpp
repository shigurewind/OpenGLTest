#include "TestCamera.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"
#include <GLFW/glfw3.h>



//���w���C�u����
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

extern bool freeMouse;

namespace test
{
	TestCamera::TestCamera()
		:
		m_Proj(glm::perspective(45.0f, 960.0f / 640.0f, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.0f))),
		m_Model(glm::rotate(glm::mat4(1.0f), -55.0f, glm::vec3(1.0f, 0.0f, 0.0f))),
		cameraPosition(0, 0, 3.0f),cameraTarget(0,0,0),cameraSpeed(0.5f)
	{
		//�ʒu�ƃe�N�X�`���̃f�[�^�\�������
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

		
		//�J��������
		cameraDirection = glm::normalize(cameraPosition - cameraTarget);

		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));

		cameraUp = glm::cross(cameraDirection, cameraRight);

		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

		m_View = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));

		//���_�̃C���f�b�N�X�����
		//unsigned int indices[] = {
		//	0, 1, 3, // ��ڂ̎O�p�`
		//	1, 2, 3  // ��ڂ̎O�p�`
		//};

		glEnable(GL_BLEND); // �u�����h��L���ɂ���
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // �u�����h�֐���ݒ�(�A���t�@�u�����f�B���O��L���ɂ���)
		
		
		m_VAO = std::make_unique<VertexArray>(); // ���_�z��I�u�W�F�N�g���쐬

		
		//���_�o�b�t�@���쐬
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 36 * 5 * sizeof(float)); // (4�̒��_�A�e���_��4��float�l������)

		//���_�o�b�t�@�̃��C�A�E�g���`
		VertexBufferLayout layout;
		layout.Push<float>(3); // 3��float�l�������_������ǉ�(�ʒu�f�[�^)
		layout.Push<float>(2); // 2��float�l�������_������ǉ�(�e�N�X�`���f�[�^)

		m_VAO->AddBuffer(*m_VertexBuffer, layout); // VAO�ɒ��_�o�b�t�@�ƃ��C�A�E�g��ǉ�
		//--------------------------------------------

		// �C���f�b�N�X�o�b�t�@���쐬
		
		//m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6); // �C���f�b�N�X�o�b�t�@���쐬



		// �V�F�[�_�[��ǂݍ���
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader"); // �V�F�[�_�[��ǂݍ���
		m_Shader->Bind(); // �V�F�[�_�[���o�C���h���Ďg�p�\�ɂ���
		m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // �V�F�[�_�[��uniform�ϐ��ɐF��ݒ�(���F)

		m_Texture = std::make_unique<Texture> ("res/textures/magicCat.jpg"); // �e�N�X�`����ǂݍ��� 
		
		m_Shader->SetUniform1i("u_Texture", 0); // �V�F�[�_�[��uniform�ϐ��Ƀe�N�X�`�����j�b�g��ݒ�(0�Ԗڂ̃e�N�X�`�����j�b�g���g�p)

		

	}

	TestCamera::~TestCamera()
	{

	}

	void TestCamera::OnUpdate(float deltaTime)
	{
		
		m_Proj = glm::perspective(glm::radians(fov), 960.0f / 640.0f, 0.1f, 100.0f);


		if (keys[GLFW_KEY_W])
			cameraPosition += cameraSpeed * cameraFront;
		if (keys[GLFW_KEY_S])
			cameraPosition -= cameraSpeed * cameraFront;
		if (keys[GLFW_KEY_A])
			cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (keys[GLFW_KEY_D])
			cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

		m_View = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

	}

	void TestCamera::OnRender()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Depth Buffer�N�p

		Renderer renderer; // �����_���[���쐬

		m_Texture->Bind(); // �e�N�X�`�����o�C���h���Ďg�p�\�ɂ���

		
		{
			
			m_Shader->Bind(); // �V�F�[�_�[�v���O�������o�C���h���Ďg�p�\�ɂ���
			//renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader); // �����_���[���g�p����VAO��IBO��`��

			m_VAO->Bind();

			for (GLuint i = 0; i < 10; i++)
			{
				//m_View = glm::translate(glm::mat4(1.0f), cameraPosition);
				//m_Model = glm::rotate(glm::mat4(1.0f), (GLfloat)glfwGetTime() * 1.0f, cubePositions[i]);

				float angle = 20.0f * i;
				m_Model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
				m_Model = glm::rotate(m_Model, (GLfloat)glfwGetTime() + angle, glm::vec3(1.0f, 0.3f, 0.5f));

				glm::mat4 mvp = m_Proj * m_View * m_Model; // ���f���r���[���e�s����v�Z
				m_Shader->SetUniformMat4f("u_MVP", mvp); // �V�F�[�_�[��uniform�ϐ��ɐ��ˉe�s���ݒ�

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

		}


	}

	void TestCamera::OnImGuiRender()
	{
		
		ImGui::SliderFloat("CameraSpeed", &cameraSpeed, 0.01f, 2.0f);

		ImGui::SliderFloat3("CameraPosition", &cameraPosition.x, -5.0f, 5.0f);

		ImGui::Checkbox("Free Mouse", &freeMouse);
		ImGui::SliderFloat("FOV", &fov, 1.0f, 90.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}


	void TestCamera::OnKeyEvent(int key, int action)
	{
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
				keys[key] = true;
			else if (action == GLFW_RELEASE)
				keys[key] = false;
		}

		
	}

	void TestCamera::OnMouseMove(double xpos, double ypos) {
		if (firstMouse)
		{
			lastX = float(xpos);
			lastY = float(ypos);
			firstMouse = false;
			return;
		}

		float xoffset = float(xpos) - lastX;
		float yoffset = lastY - float(ypos);
		lastX = float(xpos); lastY = float(ypos);

		float sensitivity = 0.1f;
		yaw += xoffset * sensitivity;
		pitch += yoffset * sensitivity;
		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}

	void TestCamera::OnScroll(double /*xoffset*/, double yoffset) {
		if (fov >= 1.0f && fov <= 90.0f)
			fov -= yoffset;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 90.0f)
			fov = 90.0f;
	}

}





