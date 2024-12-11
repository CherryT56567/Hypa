#include "OpenGL.h"

namespace Hypa {
	OpenGL::OpenGL(std::shared_ptr<Window> window, std::shared_ptr<EventSystem> Events) {
		name = "OpenGL";
		pWindow = window;
		pEvents = Events;
	}

	void OpenGL::OnAttach() {
		pWindow->UseGLFW();

		if (!gladLoadGL()) {
			fprintf(stderr, "Failed to initialize GLAD\n");
			exit(-1);
		}

        CreateShader("Default", "vertex.glsl", "fragment.glsl");
		ChangeShader("Default");
	}

	void OpenGL::OnDetach() {
		pWindow->UseNoClientApi();
	}

	void OpenGL::Render() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void OpenGL::CreateShader(std::string name, std::string VertShaderPath, std::string FragShaderPath) {
        // Read vertex shader
        std::ifstream vertexFile(VertShaderPath);
        std::stringstream vertexStream;
        if (vertexFile.is_open()) {
            vertexStream << vertexFile.rdbuf();
            vertexFile.close();
        }
        else {
            std::cerr << "ERROR::SHADER::VERTEX::FILE_NOT_FOUND: " << VertShaderPath << std::endl;
            return;
        }
        std::string vertexCode = vertexStream.str();
        const char* vertexShaderSource = vertexCode.c_str();

        // Read fragment shader
        std::ifstream fragmentFile(FragShaderPath);
        std::stringstream fragmentStream;
        if (fragmentFile.is_open()) {
            fragmentStream << fragmentFile.rdbuf();
            fragmentFile.close();
        }
        else {
            std::cerr << "ERROR::SHADER::FRAGMENT::FILE_NOT_FOUND: " << FragShaderPath << std::endl;
            return;
        }
        std::string fragmentCode = fragmentStream.str();
        const char* fragmentShaderSource = fragmentCode.c_str();

        // Compile vertex shader
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            return;
        }

        // Compile fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            return;
        }

        // Link shaders into a program
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            return;
        }

        // Cleanup shaders as they are no longer needed
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Initialize UBO
        UniformBufferObject ubo = {}; // Initialize as required

        // Store shaders in the map
        Shaders[name] = std::make_tuple(shaderProgram, shaderProgram, ubo);
	}

	void OpenGL::RemoveShader(std::string name) {

	}

	void OpenGL::ChangeShader(std::string name) {
        currentShader = name;
	}

	std::string OpenGL::GetCurrentShaderName() {
		return currentShader;
	}

	void OpenGL::DrawVerts(std::vector<Vertex> vertices, std::vector<uint16_t> indices) {
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the Vertex Array Object
        glBindVertexArray(VAO);

        // Bind and set vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // Bind and set element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Set vertex attribute pointers
        // Attribute 0: Position (vec3)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
        glEnableVertexAttribArray(0);

        // Attribute 1: Color (vec3)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        // Unbind the VBO (not EBO)
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Unbind the VAO (optional)
        glBindVertexArray(0);
	}

	void OpenGL::AddUniform(std::string name, UniformBufferObject& ubo) {

	}
}