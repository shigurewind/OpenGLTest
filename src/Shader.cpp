#include "Shader.h"

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"



Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0) // シェーダーのファイルパスを設定し、レンダラーIDを初期化
{
	ShaderProgramSource source = ParseShader(filepath); // シェーダーのソースコードを読み込む
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);//シェーダーを作成

	 
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID); // シェーダープログラムを削除
	
}



//シェーダーファイルを分けて読み込んで取得する
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath); // ファイルストリームを開く

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1,
	};

	std::string line; // 行を格納する変数
	std::stringstream ss[2]; // シェーダーのソースコードを格納するためのstringstreamを2つ用意
	ShaderType type = ShaderType::NONE; // シェーダーのタイプを初期化    
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos) // "#shader"が行に含まれているかチェック
		{
			if (line.find("vertex") != std::string::npos)
			{
				//vertex shaderにセットする
				type = ShaderType::VERTEX; // シェーダータイプを頂点シェーダーに設定
			}
			else if (line.find("fragment") != std::string::npos)
			{
				//fragment shaderにセットする
				type = ShaderType::FRAGMENT; // シェーダータイプをフラグメントシェーダーに設定
			}
		}
		else
		{
			ss[(int)type] << line << "\n"; // 現在のシェーダータイプに対応するstringstreamに行を追加
		}
	}

	return { ss[0].str(), ss[1].str() }; // シェーダーのソースコードを返す

}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type); // シェーダーのIDを作成
	const char* src = source.c_str(); // ソースコードをCスタイル文字列に変換
	glShaderSource(id, 1, &src, nullptr); // シェーダーのソースコードを設定
	glCompileShader(id); // シェーダーをコンパイル

	//TODO: エラーチェックを追加する
	int result; // コンパイル結果を格納する変数
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); // コンパイルステータスを取得
	if (result == GL_FALSE) // コンパイルに失敗した場合
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // エラーログの長さを取得
		char* message = (char*)alloca(length * sizeof(char)); // エラーメッセージ用のステークメモリを確保
		glGetShaderInfoLog(id, length, &length, message); // エラーメッセージを取得
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl; // エラーメッセージを表示
		glDeleteShader(id); // シェーダーのIDを削除
		return 0; // エラーが発生した場合は0を返す

	}

	return id; // コンパイルされたシェーダーのIDを返す
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{

	unsigned int program = glCreateProgram();//プログラムを作成する
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);//頂点シェーダーを作成
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);//フラグメントシェーダーを作成

	glAttachShader(program, vs); // プログラムに頂点シェーダーをアタッチ
	glAttachShader(program, fs); // プログラムにフラグメントシェーダーをアタッチ
	glLinkProgram(program); // プログラムをリンク
	glValidateProgram(program); // プログラムのバリデーションを行う

	glDeleteShader(vs); // シェーダーのIDを削除
	glDeleteShader(fs); // シェーダーのIDを削除

	return program; // 作成されたプログラムのIDを返す
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID); // シェーダーをバインドする
}

void Shader::UnBind() const
{
	glUseProgram(0); // シェーダーのバインドを解除する
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value); // シェーダーのuniform変数に値を設定
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value); // シェーダーのuniform変数に値を設定
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3); // シェーダーのuniform変数に値を設定
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]); // シェーダーのuniform変数に行列を設定
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) // キャッシュに存在するかチェック
	{
		return m_UniformLocationCache[name];// キャッシュから位置を取得
	}

	GLint location = glGetUniformLocation(m_RendererID, name.c_str()); // シェーダーのuniform変数の位置を取得
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl; // uniform変数が存在しない場合の警告
	}
	
	m_UniformLocationCache[name] = location; // キャッシュに位置を保存
	
	return location; // 取得した位置を返す
}
