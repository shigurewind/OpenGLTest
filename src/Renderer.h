#pragma once
#include <GL/glew.h> // OpenGL�̃w�b�_�[�t�@�C��
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h" 


// OpenGL�̃G���[���`�F�b�N���邽�߂̃w�b�_�[�t�@�C��
#define ASSERT(x) if (!(x)) __debugbreak(); // �A�T�[�V�����̒�`,MSVC�̃f�o�b�O�u���[�N�|�C���g���g�p
//�S�Ă̂�GL�֐��̌Ăяo�������b�v����}�N�����`
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__ )) // OpenGL�̃G���[���`�F�b�N����}�N��


void GLClearError(); // OpenGL�̃G���[���N���A����֐�
bool GLLogCall(const char* function, const char* file, int line); // OpenGL�̃G���[���`�F�b�N����֐�

class Renderer
{
private:

public:
	/*Renderer();
	~Renderer();*/
	void Clear() const; // ��ʂ��N���A����֐�

	//void SetClearColor(float r, float g, float b, float a) const; // �N���A�J���[��ݒ肷��֐�
	
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};



