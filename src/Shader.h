#pragma once
#include <string>
#include <unordered_map>


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
	std::unordered_map<std::string, int> m_UniformLocationCache; // uniform変数の位置をキャッシュするためのマップ


public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const; // シェーダーをバインドする
	void UnBind() const; // シェーダーのバインドを解除する

	//setUniform関数
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source); // シェーダーをコンパイルする
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name); // シェーダーのuniform変数の位置を取得する

};



