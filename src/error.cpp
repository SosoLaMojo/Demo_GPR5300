#include "error.h"
#include <glad/glad.h>

namespace gl {
	
    void IsError(const std::string& file, int line)
    {
	    auto error_code = glGetError();
	    if (error_code != GL_NO_ERROR)
	    {
		    std::cerr
			    << error_code
			    << " in file: " << file
			    << " at line: " << line
			    << "\n";
	    }
    }
	
} // namespace gl
