#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <GL/glew.h> 


//シェーダーのソースコードを格納するための構造体
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	std::string m_FilePath; // シェーダーファイルのパス
	unsigned int m_RendererID; // シェーダーのレンダラーID

	//caching for uniform locations
	mutable std::unordered_map<std::string, GLint> m_UniformLocationCache; // uniform変数の位置をキャッシュするためのマップ


public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const; // シェーダーをバインドする
	void UnBind() const; // シェーダーのバインドを解除する

	unsigned int GetRendererID() const { return m_RendererID; } // シェーダーのレンダラーIDを取得する

	//setUniform関数
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string& name, int size, int *value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source); // シェーダーをコンパイルする
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	GLint GetUniformLocation(const std::string& name) const; // シェーダーのuniform変数の位置を取得する

	

};



