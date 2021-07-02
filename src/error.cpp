#include "error.h"
#include <glad/glad.h>

namespace gl {
	
    void IsError(const std::string& file, int line)
    {
        auto error_code = glGetError();
        if (error_code != GL_NO_ERROR)
        {
            switch (error_code)
            {
            case GL_INVALID_ENUM:
                std::cerr
                    << error_code
                    << " in file: " << file
                    << " at line: " << line
                    << " OpenGL: GL_INVALID_ENUM "
                    << "\n";
                break;
            case GL_INVALID_VALUE:
                std::cerr
                    << error_code
                    << " in file: " << file
                    << " at line: " << line
                    << " OpenGL: GL_INVALID_VALUE "
                    << "\n";
                break;
            case GL_INVALID_OPERATION:
                std::cerr
                    << error_code
                    << " in file: " << file
                    << " at line: " << line
                    << " OpenGL: GL_INVALID_OPERATION "
                    << "\n";
                break;
            case GL_STACK_OVERFLOW:
                std::cerr
                    << error_code
                    << " in file: " << file
                    << " at line: " << line
                    << " OpenGL: GL_STACK_OVERFLOW "
                    << "\n";
                break;
            case GL_STACK_UNDERFLOW:
                std::cerr
                    << error_code
                    << " in file: " << file
                    << " at line: " << line
                    << " OpenGL: OpenGL: GL_STACK_UNDERFLOW "
                    << "\n";
                break;
            case GL_OUT_OF_MEMORY:
                std::cerr
                    << error_code
                    << " in file: " << file
                    << " at line: " << line
                    << " OpenGL: OpenGL: GL_OUT_OF_MEMORY "
                    << "\n";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cerr
                    << error_code
                    << " in file: " << file
                    << " at line: " << line
                    << " OpenGL: GL_INVALID_FRAMEBUFFER_OPERATION "
                    << "\n";
                break;
            case GL_CONTEXT_LOST:
                std::cerr
                    << error_code
                    << " in file: " << file
                    << " at line: " << line
                    << " OpenGL: GL_CONTEXT_LOST "
                    << "\n";
                break;
            default:
                std::cerr
                    << error_code
                    << " in file: " << file
                    << " at line: " << line
                    << "\n";
                break;
            }
        }
    }
	
} // namespace gl
