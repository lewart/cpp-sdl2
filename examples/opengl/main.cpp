#include <iostream>
#include <string>
// To use the GL wrapper, define the following
#include <glbinding/gl/gl.h>
// Const definition from X.h conflicting with CallbackMask.h,
// which is included through Binding.h
//#ifdef None
//#undef None
//#endif
#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
// sdl.h SHOULD be included after glbinding in Linux
// because of conflicting const None from X.h.
#include <sdl.hpp>

using namespace gl;

// The following arrays are the geometry data for one triangle with vertex
// colors

// clang-format off
GLfloat tri_vertex_buffer[]
{   /*X   Y   Z    R  G  B*/
	  0,  .5, 0,   1, 0, 0, //Vertex 2
	 .5, -.5, 0,   0, 1, 0, //Vertex 1
	-.5, -.5, 0,   0, 0, 1, //Vertex 0
};
// clang-format on

GLuint tri_index_buffer[]{0, 1, 2};

// The following strings are a shader program that just bland the above data to
// the screen
const GLchar vert_shader_source[] = R"_(
#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_rgb;

out vec4 color_output;

void main()
{
	gl_Position = vec4(in_pos, 1);
	color_output = vec4(in_rgb, 1);
})_";

const GLchar frag_shader_source[] = R"_(
#version 330 core

in vec4 color_output;
out vec4 frag_color;

void main()
{
	frag_color = color_output;
}
)_";

// This function print context version information to a console
void print_gl_version();

// This function build a shader program from shader source code
GLuint build_shader_program(
	const GLchar* vert_source, const GLchar* frag_source);

int main(int argc, char* argv[])
{
	(void)argc, (void)argv;
	// Create an SDL window, with the SDL_WINDOW_OPENGL flags
	auto window = sdl::Window("OpenGL", {800, 600}, SDL_WINDOW_OPENGL);

	// Before creating a context, set the flag for the version you want to get,
	// here we want Core OpenGL 3.3
	sdl::Window::gl_set_attribute(
		SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	sdl::Window::gl_set_attribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	sdl::Window::gl_set_attribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Create your context
	auto context = window.create_context();

	// You are done, now you can use OpenGL!

	// Call whatever function loader you want, in this example we use glbinding2.
	glbinding::Binding::initialize(false);

	// We generated a really small version of glad for Core OpenGL 3.3.
	print_gl_version();
	GLuint shader_program =
		build_shader_program(vert_shader_source, frag_shader_source);

	// We build a VertexArrayObject to reference our triangle geometry in the
	// GPU
	//
	// Our geometry data is expressed as one VertexBufferObject and one
	// ElementBufferObject.
	//
	// The VertexBufferObject contains 6 floats per vertex. 3 of them is a 3D
	// vector for position, the 3 other are an RGB color value in linear
	// colorspace.
	//
	// Our shader want position data in location 0 and color data in location 1
	GLuint tri_vao, tri_vbo, tri_ebo;
	glGenVertexArrays(1, &tri_vao);
	glGenBuffers(1, &tri_vbo);
	glGenBuffers(1, &tri_ebo);

	glBindVertexArray(tri_vao);

	// Build VBO
	glBindBuffer(GL_ARRAY_BUFFER, tri_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		18 * sizeof(GLfloat),
		tri_vertex_buffer,
		GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(GLfloat),
		(void*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		6 * sizeof(GLfloat),
		(void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Build EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_ebo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		3 * sizeof(tri_index_buffer),
		tri_index_buffer,
		GL_STATIC_DRAW);

	// Clean behind yourself
	glBindVertexArray(0);

	// Render loop
	bool running = true;
	while (running)
	{
		sdl::Event e;
		while (e.poll())
		{
			if (e.type == SDL_QUIT) running = false;
		}

		glClearColor(.1f, .2f, .3f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(tri_vao);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		window.gl_swap();
	}

	glUseProgram(0);
	glDeleteProgram(shader_program);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &tri_vao);
	glDeleteBuffers(1, &tri_vbo);
	glDeleteBuffers(1, &tri_ebo);

	return 0;
}

void print_gl_version()
{
	std::cout << "\n"
		<< "Vendor          :\t" << glbinding::ContextInfo::vendor() << "\n"
		<< "Renderer Device :\t" << glbinding::ContextInfo::renderer() << "\n"
		<< "OpenGL Version  :\t" <<  glbinding::ContextInfo::version() << "\n"
		<< "Context Version :\t" << glGetString(GL_VERSION) << "\n"
		<< "Shading Language:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

GLuint build_shader_program(
	const GLchar* vert_source, const GLchar* frag_source)
{
	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint program	   = glCreateProgram();

	glShaderSource(vert_shader, 1, &vert_source, nullptr);
	glShaderSource(frag_shader, 1, &frag_source, nullptr);
	glCompileShader(vert_shader);
	glCompileShader(frag_shader);

	GLint  success = 0;
	GLchar info_log[512];

	if (glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success); !success)
	{
		glGetShaderInfoLog(vert_shader, sizeof info_log, nullptr, info_log);
		std::cerr << info_log << std::endl;
		abort();
	}
	if (glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success); !success)
	{
		glGetShaderInfoLog(frag_shader, sizeof info_log, nullptr, info_log);
		std::cerr << info_log << std::endl;
		abort();
	}

	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);
	if (glGetProgramiv(program, GL_LINK_STATUS, &success); !success)
	{
		glGetProgramInfoLog(program, sizeof info_log, nullptr, info_log);
		std::cerr << info_log << std::endl;
		abort();
	}

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
	return program;
}
