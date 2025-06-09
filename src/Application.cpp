
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

//���w���C�u����
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL�̃��W���[�o�[�W������3�ɐݒ�
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL�̃}�C�i�[�o�[�W������3�ɐݒ�
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGL�̃R�A�v���t�@�C�����g�p

    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	glfwSwapInterval(5); // VSync��L���ɂ���(1�̓��j�^�[�̃t���[�����[�g��������)

    if (glewInit() != GLEW_OK) {
        std::cout << "Error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

	{ // �X�R�[�v���쐬���ă��\�[�X�̊Ǘ����s��

	//�ʒu�ƃe�N�X�`���̃f�[�^�\�������
		float positions[] = {
			100.0f, 100.0f, 0.0f, 0.0f,//0
			200.0f, 100.0f, 1.0f, 0.0f,//1
			200.0f, 200.0f, 1.0f, 1.0f,//2
			100.0,  200.0f, 0.0f, 1.0f,//3

		};

		//���_�̃C���f�b�N�X�����
		unsigned int indices[] = {
			0, 1, 2, // ��ڂ̎O�p�`
			2, 3, 0  // ��ڂ̎O�p�`
		};

		glEnable(GL_BLEND); // �u�����h��L���ɂ���
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // �u�����h�֐���ݒ�(�A���t�@�u�����f�B���O��L���ɂ���)

		//vertex array abstract���쐬----------------
		VertexArray va;
		//���_�o�b�t�@���쐬
		VertexBuffer vb(positions, 4 * 4 * sizeof(float)); // (4�̒��_�A�e���_��4��float�l������)

		//���_�o�b�t�@�̃��C�A�E�g���`
		VertexBufferLayout layout;
		layout.Push<float>(2); // 2��float�l�������_������ǉ�(�ʒu�f�[�^)
		layout.Push<float>(2); // 2��float�l�������_������ǉ�(�e�N�X�`���f�[�^)
		va.AddBuffer(vb, layout); // VAO�ɒ��_�o�b�t�@�ƃ��C�A�E�g��ǉ�
		//--------------------------------------------

		// �C���f�b�N�X�o�b�t�@���쐬
		IndexBuffer ib(indices, 6); // (6�̃C���f�b�N�X������)


		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f); // ���ˉe�s����쐬
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0, 0)); // �r���[�s����쐬

		// �V�F�[�_�[��ǂݍ���
		Shader shader("res/shaders/Basic.shader"); // �V�F�[�_�[��ǂݍ���
		shader.Bind(); // �V�F�[�_�[���o�C���h���Ďg�p�\�ɂ���
		shader.SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // �V�F�[�_�[��uniform�ϐ��ɐF��ݒ�(���F)

		va.UnBind(); // VAO�̃o�C���h������
		vb.UnBind(); // ���_�o�b�t�@�̃o�C���h������
		ib.UnBind(); // �C���f�b�N�X�o�b�t�@�̃o�C���h������
		shader.UnBind(); // �V�F�[�_�[�v���O�����̎g�p���~

		Renderer renderer; // �����_���[�̃C���X�^���X���쐬

		//ImGui
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfwGL3_Init(window, true);

		ImGui::StyleColorsDark();

		glm::vec3 translation(200, 200, 0);
		
		

		//�F�̕ϐ������
		float r = 1.0f; // �Ԃ̒l
		float increment = 0.05f; // �Ԃ̒l�̑�����

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear(); // ��ʂ��N���A

			//ImGui�̐V�����t���[�����J�n
			ImGui_ImplGlfwGL3_NewFrame();

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation); // ���f���s��

			glm::mat4 mvp = proj * view * model; // ���f���r���[���e�s����v�Z


			//--------------
			shader.Bind(); // �V�F�[�_�[�v���O�������o�C���h���Ďg�p�\�ɂ���
			shader.SetUniform4f("u_Color", r, 1.0f, 0.0f, 1.0f); // �V�F�[�_�[��uniform�ϐ��ɐF��ݒ�

			shader.SetUniformMat4f("u_MVP", mvp); // �V�F�[�_�[��uniform�ϐ��ɐ��ˉe�s���ݒ�

			//--------------�����̃R�[�h�ȗ��������Ȃ�Materials���g���K�v������܂�

			Texture texture("res/textures/enemy.png"); // �e�N�X�`����ǂݍ���
			texture.Bind(); // �e�N�X�`�����o�C���h���Ďg�p�\�ɂ���
			shader.SetUniform1i("u_Texture", 0); // �V�F�[�_�[��uniform�ϐ��Ƀe�N�X�`�����j�b�g��ݒ�(0�Ԗڂ̃e�N�X�`�����j�b�g���g�p)

			renderer.Draw(va, ib, shader); // �����_���[���g�p����VAO��IBO��`��


			//�ԐF�̒l���X�V
			if (r > 1.0f)
			{
				increment = -0.05f;
			}
			else if (r < 0.0f)
			{
				increment = 0.05f;
			}

			r += increment; // �Ԃ̒l���X�V

			//ImGui�̓��e
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

	//ImGui�̏I������
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}