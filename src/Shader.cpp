#include "Shader.h"

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"



Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0) // �V�F�[�_�[�̃t�@�C���p�X��ݒ肵�A�����_���[ID��������
{
	ShaderProgramSource source = ParseShader(filepath); // �V�F�[�_�[�̃\�[�X�R�[�h��ǂݍ���
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);//�V�F�[�_�[���쐬

	 
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID); // �V�F�[�_�[�v���O�������폜
	
}



//�V�F�[�_�[�t�@�C���𕪂��ēǂݍ���Ŏ擾����
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath); // �t�@�C���X�g���[�����J��

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1,
	};

	std::string line; // �s���i�[����ϐ�
	std::stringstream ss[2]; // �V�F�[�_�[�̃\�[�X�R�[�h���i�[���邽�߂�stringstream��2�p��
	ShaderType type = ShaderType::NONE; // �V�F�[�_�[�̃^�C�v��������    
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos) // "#shader"���s�Ɋ܂܂�Ă��邩�`�F�b�N
		{
			if (line.find("vertex") != std::string::npos)
			{
				//vertex shader�ɃZ�b�g����
				type = ShaderType::VERTEX; // �V�F�[�_�[�^�C�v�𒸓_�V�F�[�_�[�ɐݒ�
			}
			else if (line.find("fragment") != std::string::npos)
			{
				//fragment shader�ɃZ�b�g����
				type = ShaderType::FRAGMENT; // �V�F�[�_�[�^�C�v���t���O�����g�V�F�[�_�[�ɐݒ�
			}
		}
		else
		{
			ss[(int)type] << line << "\n"; // ���݂̃V�F�[�_�[�^�C�v�ɑΉ�����stringstream�ɍs��ǉ�
		}
	}

	return { ss[0].str(), ss[1].str() }; // �V�F�[�_�[�̃\�[�X�R�[�h��Ԃ�

}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type); // �V�F�[�_�[��ID���쐬
	const char* src = source.c_str(); // �\�[�X�R�[�h��C�X�^�C��������ɕϊ�
	glShaderSource(id, 1, &src, nullptr); // �V�F�[�_�[�̃\�[�X�R�[�h��ݒ�
	glCompileShader(id); // �V�F�[�_�[���R���p�C��

	//TODO: �G���[�`�F�b�N��ǉ�����
	int result; // �R���p�C�����ʂ��i�[����ϐ�
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); // �R���p�C���X�e�[�^�X���擾
	if (result == GL_FALSE) // �R���p�C���Ɏ��s�����ꍇ
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // �G���[���O�̒������擾
		char* message = (char*)alloca(length * sizeof(char)); // �G���[���b�Z�[�W�p�̃X�e�[�N���������m��
		glGetShaderInfoLog(id, length, &length, message); // �G���[���b�Z�[�W���擾
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl; // �G���[���b�Z�[�W��\��
		glDeleteShader(id); // �V�F�[�_�[��ID���폜
		return 0; // �G���[�����������ꍇ��0��Ԃ�

	}

	return id; // �R���p�C�����ꂽ�V�F�[�_�[��ID��Ԃ�
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{

	unsigned int program = glCreateProgram();//�v���O�������쐬����
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);//���_�V�F�[�_�[���쐬
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);//�t���O�����g�V�F�[�_�[���쐬

	glAttachShader(program, vs); // �v���O�����ɒ��_�V�F�[�_�[���A�^�b�`
	glAttachShader(program, fs); // �v���O�����Ƀt���O�����g�V�F�[�_�[���A�^�b�`
	glLinkProgram(program); // �v���O�����������N
	glValidateProgram(program); // �v���O�����̃o���f�[�V�������s��

	glDeleteShader(vs); // �V�F�[�_�[��ID���폜
	glDeleteShader(fs); // �V�F�[�_�[��ID���폜

	return program; // �쐬���ꂽ�v���O������ID��Ԃ�
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID); // �V�F�[�_�[���o�C���h����
}

void Shader::UnBind() const
{
	glUseProgram(0); // �V�F�[�_�[�̃o�C���h����������
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value); // �V�F�[�_�[��uniform�ϐ��ɒl��ݒ�
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value); // �V�F�[�_�[��uniform�ϐ��ɒl��ݒ�
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3); // �V�F�[�_�[��uniform�ϐ��ɒl��ݒ�
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]); // �V�F�[�_�[��uniform�ϐ��ɍs���ݒ�
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) // �L���b�V���ɑ��݂��邩�`�F�b�N
	{
		return m_UniformLocationCache[name];// �L���b�V������ʒu���擾
	}

	GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // �V�F�[�_�[��uniform�ϐ��̈ʒu���擾
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl; // uniform�ϐ������݂��Ȃ��ꍇ�̌x��
	}
	
	m_UniformLocationCache[name] = location; // �L���b�V���Ɉʒu��ۑ�
	
	return location; // �擾�����ʒu��Ԃ�
}
