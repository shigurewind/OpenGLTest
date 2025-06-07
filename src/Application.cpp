
#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"





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

	//�ʒu�̃f�[�^�\�������
		float positions[] = {
			-0.5f, -0.5f,//0
			 0.5f, -0.5f,//1
			 0.5f,  0.5f,//2
			-0.5f,  0.5f,//3

		};

		//���_�̃C���f�b�N�X�����
		unsigned int indices[] = {
			0, 1, 2, // ��ڂ̎O�p�`
			2, 3, 0  // ��ڂ̎O�p�`
		};

		//vertex array abstract���쐬----------------
		VertexArray va;
		//���_�o�b�t�@���쐬
		VertexBuffer vb(positions, 4 * 2 * sizeof(float)); // (4�̒��_�A�e���_��2��float�l������)

		//���_�o�b�t�@�̃��C�A�E�g���`
		VertexBufferLayout layout;
		layout.Push<float>(2); // 2��float�l�������_������ǉ�(�ʒu�f�[�^)
		va.AddBuffer(vb, layout); // VAO�ɒ��_�o�b�t�@�ƃ��C�A�E�g��ǉ�
		//--------------------------------------------

		// �C���f�b�N�X�o�b�t�@���쐬
		IndexBuffer ib(indices, 6); // (6�̃C���f�b�N�X������)

		// �V�F�[�_�[��ǂݍ���
		Shader shader("res/shaders/Basic.shader"); // �V�F�[�_�[��ǂݍ���
		shader.Bind(); // �V�F�[�_�[���o�C���h���Ďg�p�\�ɂ���
		shader.SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // �V�F�[�_�[��uniform�ϐ��ɐF��ݒ�(���F)

		va.UnBind(); // VAO�̃o�C���h������
		vb.UnBind(); // ���_�o�b�t�@�̃o�C���h������
		ib.UnBind(); // �C���f�b�N�X�o�b�t�@�̃o�C���h������
		shader.UnBind(); // �V�F�[�_�[�v���O�����̎g�p���~
		

		//�F�̕ϐ������
		float r = 1.0f; // �Ԃ̒l
		float increment = 0.05f; // �Ԃ̒l�̑�����

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			////�O�p�`��`��(�Â��o�[�W������OpenGL)
			//glBegin(GL_TRIANGLES);
			//glVertex2f(-0.5f, -0.5f);
			//glVertex2f(0, 0.5f);
			//glVertex2f(0.5f, -0.5f);
			//glEnd();

			shader.Bind(); // �V�F�[�_�[�v���O�������o�C���h���Ďg�p�\�ɂ���
			shader.SetUniform4f("u_Color", r, 1.0f, 0.0f, 1.0f); // �V�F�[�_�[��uniform�ϐ��ɐF��ݒ�
			


			va.Bind();
			ib.Bind(); // �C���f�b�N�X�o�b�t�@���o�C���h���ėL���ɂ���


			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));//Index���g���ĕ`�悷��(�K��unsigned int���g��)

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


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		
	}

    glfwTerminate();
    return 0;
}