#pragma once
#include <string>
#include <unordered_map>


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
	std::unordered_map<std::string, int> m_UniformLocationCache; // uniform�ϐ��̈ʒu���L���b�V�����邽�߂̃}�b�v


public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const; // �V�F�[�_�[���o�C���h����
	void UnBind() const; // �V�F�[�_�[�̃o�C���h����������

	//setUniform�֐�
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source); // �V�F�[�_�[���R���p�C������
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name); // �V�F�[�_�[��uniform�ϐ��̈ʒu���擾����

};



