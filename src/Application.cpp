
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


//シェーダーのソースコードを格納するための構造体
struct ShaderProgramSource
{
    std::string VertexSource;
	std::string FragmentSource;
};

//シェーダーファイルを分けて読み込んで取得する
static ShaderProgramSource ParseShader(const std::string& filepath)
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


static unsigned int CompileShader(unsigned int type, const std::string& source)
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
		std::cout << "Failed to compile "<<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl; // エラーメッセージを表示
		glDeleteShader(id); // シェーダーのIDを削除
		return 0; // エラーが発生した場合は0を返す

    }

	return id; // コンパイルされたシェーダーのIDを返す
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGLのメジャーバージョンを3に設定
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGLのマイナーバージョンを3に設定
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGLのコアプロファイルを使用

    

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	glfwSwapInterval(1); // VSyncを有効にする(モニターのフレームレート同期する)

    if (glewInit() != GLEW_OK) {
        std::cout << "Error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

	{ // スコープを作成してリソースの管理を行う

	//位置のデータ構造を作る
		float positions[] = {
			-0.5f, -0.5f,//0
			 0.5f, -0.5f,//1
			 0.5f,  0.5f,//2
			-0.5f,  0.5f,//3

		};

		//頂点のインデックスを作る
		unsigned int indices[] = {
			0, 1, 2, // 一つ目の三角形
			2, 3, 0  // 二つ目の三角形
		};

		//vertex array abstractを作成----------------
		VertexArray va;
		//頂点バッファを作成
		VertexBuffer vb(positions, 4 * 2 * sizeof(float)); // (4つの頂点、各頂点は2つのfloat値を持つ)

		//頂点バッファのレイアウトを定義
		VertexBufferLayout layout;
		layout.Push<float>(2); // 2つのfloat値を持つ頂点属性を追加(位置データ)
		va.AddBuffer(vb, layout); // VAOに頂点バッファとレイアウトを追加
		
		//--------------------------------------------

		
		// インデックスバッファを作成
		IndexBuffer ib(indices, 6); // (6つのインデックスを持つ)



		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); // シェーダーのソースコードを読み込む
		std::cout << "Vertex Shader: " << std::endl << source.VertexSource << std::endl;
		std::cout << "Fragment Shader: " << std::endl << source.FragmentSource << std::endl;

		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);//シェーダーを作成
		glUseProgram(shader); // シェーダープログラムを使用

		int location = glGetUniformLocation(shader, "u_Color"); // シェーダーのuniform変数の位置を取得
		ASSERT(location != -1); // uniform変数が正しく取得できたかチェック
		glUniform4f(location, 1.0f, 1.0f, 0.0f, 1.0f);// uniform変数に色を設定(黄色)

		va.UnBind(); // VAOのバインドを解除
		glUseProgram(0); // シェーダープログラムの使用を停止
		glBindBuffer(GL_ARRAY_BUFFER, 0); // バッファのバインドを解除
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // インデックスバッファのバインドを解除

		//色の変数を作る
		float r = 1.0f; // 赤の値
		float increment = 0.05f; // 赤の値の増加量

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			////三角形を描画(古いバージョンのOpenGL)
			//glBegin(GL_TRIANGLES);
			//glVertex2f(-0.5f, -0.5f);
			//glVertex2f(0, 0.5f);
			//glVertex2f(0.5f, -0.5f);
			//glEnd();

			glUseProgram(shader);
			glUniform4f(location, r, 1.0f, 0.0f, 1.0f);//Uniformは毎回描画する時に設置する


			va.Bind();
			ib.Bind(); // インデックスバッファをバインドして有効にする


			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));//Indexを使って描画する(必ずunsigned intを使う)

			//赤色の値を更新
			if (r > 1.0f)
			{
				increment = -0.05f;
			}
			else if (r < 0.0f)
			{
				increment = 0.05f;
			}

			r += increment; // 赤の値を更新


			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		glDeleteProgram(shader); // シェーダープログラムのIDを削除

		//delete vb; // 頂点バッファのメモリを解放
		//delete ib; // インデックスバッファのメモリを解放
	}

    glfwTerminate();
    return 0;
}