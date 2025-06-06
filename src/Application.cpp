
#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

// OpenGL�̃G���[���`�F�b�N���邽�߂̃w�b�_�[�t�@�C��
#define ASSERT(x) if (!(x)) __debugbreak(); // �A�T�[�V�����̒�`,MSVC�̃f�o�b�O�u���[�N�|�C���g���g�p
//�S�Ă̂�GL�֐��̌Ăяo�������b�v����}�N�����`
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__,__LINE__ )) // OpenGL�̃G���[���`�F�b�N����}�N��


static void GLClearError() // OpenGL�̃G���[���N���A����֐�
{
	while (glGetError() != GL_NO_ERROR);// �G���[���Ȃ��Ȃ�܂Ń��[�v
}

static bool GLLogCall(const char* function, const char*file, int line ) // OpenGL�̃G���[���`�F�b�N����֐�
{
	while (GLenum error = glGetError()) // �G���[��������胋�[�v
	{
		std::cout << "[OpenGL Error] (" << error << ") " << function << " " << file << ":" << line << std::endl; // �G���[���b�Z�[�W��\��
		return false; // �G���[�����������ꍇ��false��Ԃ�
	}

	return true; // �G���[���������Ȃ������ꍇ��true��Ԃ�
}


//�V�F�[�_�[�̃\�[�X�R�[�h���i�[���邽�߂̍\����
struct ShaderProgramSource
{
    std::string VertexSource;
	std::string FragmentSource;
};

//�V�F�[�_�[�t�@�C���𕪂��ēǂݍ���Ŏ擾����
static ShaderProgramSource ParseShader(const std::string& filepath)
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


static unsigned int CompileShader(unsigned int type, const std::string& source)
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
		std::cout << "Failed to compile "<<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
		std::cout << message << std::endl; // �G���[���b�Z�[�W��\��
		glDeleteShader(id); // �V�F�[�_�[��ID���폜
		return 0; // �G���[�����������ꍇ��0��Ԃ�

    }

	return id; // �R���p�C�����ꂽ�V�F�[�_�[��ID��Ԃ�
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

	glfwSwapInterval(1); // VSync��L���ɂ���(���j�^�[�̃t���[�����[�g��������)

    if (glewInit() != GLEW_OK) {
        std::cout << "Error" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

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

	unsigned int vao; // Vertex Array Object���쐬���邽�߂̕ϐ�
	glGenVertexArrays(1, &vao); // VAO�𐶐�
	glBindVertexArray(vao); // VAO���o�C���h���ėL���ɂ���

    //OpenGL�Ƀf�[�^��n��
    unsigned int buffer;//buffer�����
    glGenBuffers(1, &buffer);//buffer��ID
    glBindBuffer(GL_ARRAY_BUFFER, buffer);//buffer��Bind����
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);//���_��6��,�ÓI�ɕ`��

    //Index���N��
	GLCall(glEnableVertexAttribArray(0));//���_������L���ɂ���(0�͈ʒu�̑����̃C���f�b�N�X)
    //OpenGL�ɂǂ�����ăf�[�^�������̂�������
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//index(�ʒu����������0�A��g���A�f�[�^�^�ANormalize���邩�A����_�܂ł�Byte���A�|�C���^�[�i�����A�B��̑���������0�j)

    //
	unsigned int ibo;//index buffer object�����
    glGenBuffers(1, &ibo);//buffer��ID
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);//buffer��Bind����
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);//�C���f�b�N�X��6��,�ÓI�ɕ`��


    
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); // �V�F�[�_�[�̃\�[�X�R�[�h��ǂݍ���
	std::cout << "Vertex Shader: " << std::endl << source.VertexSource << std::endl;
	std::cout << "Fragment Shader: " << std::endl << source.FragmentSource << std::endl;

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);//�V�F�[�_�[���쐬
	glUseProgram(shader); // �V�F�[�_�[�v���O�������g�p

	int location = glGetUniformLocation(shader, "u_Color"); // �V�F�[�_�[��uniform�ϐ��̈ʒu���擾
	ASSERT(location != -1); // uniform�ϐ����������擾�ł������`�F�b�N
	glUniform4f(location, 1.0f, 1.0f, 0.0f, 1.0f);// uniform�ϐ��ɐF��ݒ�(���F)

	glBindVertexArray(0); // VAO�̃o�C���h������
	glUseProgram(0); // �V�F�[�_�[�v���O�����̎g�p���~
	glBindBuffer(GL_ARRAY_BUFFER, 0); // �o�b�t�@�̃o�C���h������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // �C���f�b�N�X�o�b�t�@�̃o�C���h������

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

		glUseProgram(shader); 
        glUniform4f(location, r, 1.0f, 0.0f, 1.0f);//Uniform�͖���`�悷�鎞�ɐݒu����

		
		glBindVertexArray(vao); // VAO���o�C���h���ėL���ɂ���
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		
		
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

	glDeleteProgram(shader); // �V�F�[�_�[�v���O������ID���폜

    glfwTerminate();
    return 0;
}