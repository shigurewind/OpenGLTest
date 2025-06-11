
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

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"





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

	

		glEnable(GL_BLEND); // �u�����h��L���ɂ���
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // �u�����h�֐���ݒ�(�A���t�@�u�����f�B���O��L���ɂ���)

		

		Renderer renderer; // �����_���[�̃C���X�^���X���쐬

		//ImGui
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest); // �e�X�g���j���[�̃C���X�^���X���쐬
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");// �N���A�J���[�̃e�X�g��o�^
		testMenu->RegisterTest<test::TestTexture2D>("2D Texture"); // 2D�e�N�X�`���̃e�X�g��o�^

		
		

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // �w�i�F��ݒ�
			/* Render here */
			renderer.Clear(); // ��ʂ��N���A



			//ImGui�̐V�����t���[�����J�n
			ImGui_ImplGlfwGL3_NewFrame();

			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Tests"); // ImGui�̃E�B���h�E���J�n

				if (currentTest != testMenu && ImGui::Button("<-"))// �e�X�g���j���[�ɖ߂�
				{
					delete currentTest;
					currentTest = testMenu; 
				}
				currentTest->OnImGuiRender(); // ���݂̃e�X�g��ImGui�����_�����O�������Ăяo��

				ImGui::End(); // ImGui�̃E�B���h�E���I��
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
		delete currentTest; // ���݂̃e�X�g���폜
		
		
	}

	

	//ImGui�̏I������
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}