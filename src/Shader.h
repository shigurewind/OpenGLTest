#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <GL/glew.h> 


//�V�F�[�_�[�̃\�[�X�R�[�h���i�[���邽�߂̍\����
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	std::string m_FilePath; // �V�F�[�_�[�t�@�C���̃p�X
	unsigned int m_RendererID; // �V�F�[�_�[�̃����_���[ID

	//caching for uniform locations
	mutable std::unordered_map<std::string, GLint> m_UniformLocationCache; // uniform�ϐ��̈ʒu���L���b�V�����邽�߂̃}�b�v


public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const; // �V�F�[�_�[���o�C���h����
	void UnBind() const; // �V�F�[�_�[�̃o�C���h����������

	unsigned int GetRendererID() const { return m_RendererID; } // �V�F�[�_�[�̃����_���[ID���擾����

	//setUniform�֐�
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string& name, int size, int *value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source); // �V�F�[�_�[���R���p�C������
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	GLint GetUniformLocation(const std::string& name) const; // �V�F�[�_�[��uniform�ϐ��̈ʒu���擾����

	

};



