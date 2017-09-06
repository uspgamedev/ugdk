/**
* Copyright (c) 2006-2012 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#include "vertexbuffer.h"

#include <ugdk/system/exceptions.h>

#include <cstdlib>
#include <cstring>

namespace ugdk {
namespace graphic {
	// VertexBuffer

	VertexBuffer *VertexBuffer::Create(size_t size, GLenum target, GLenum usage)
	{
#ifdef UGDK_OPENGL_USE_VBO
		try
		{
			// Try to create a VBO.
			return new VBO(size, target, usage);
		}
		catch (const system::BaseException &)
#endif
		{
			// VBO not supported ... create regular array.
			return new VertexArray(size, target, usage);
		}
	}

    std::shared_ptr<const VertexBuffer> VertexBuffer::CreateDefaultShared() {
        static std::weak_ptr<const VertexBuffer> buffer;
        if (auto ptr = buffer.lock())
            return ptr;

        GLfloat buffer_data[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f
        };
        std::shared_ptr<VertexBuffer> ptr(VertexBuffer::Create(sizeof(buffer_data), GL_ARRAY_BUFFER, GL_STATIC_DRAW));
        {
            VertexBuffer::Bind bind(*ptr);
            VertexBuffer::Mapper mapper(*ptr);

            GLfloat *indices = static_cast<GLfloat*>(mapper.get());
            if (indices)
                memcpy(indices, buffer_data, sizeof(buffer_data));
        }
        buffer = ptr;
        return ptr;
    }

    const VertexBuffer* VertexBuffer::CreateDefault() {
        static std::shared_ptr<const VertexBuffer> buffer = CreateDefaultShared();
        return buffer.get();
    }


	VertexBuffer::VertexBuffer(size_t size, GLenum target, GLenum usage)
		: size(size)
		, target(target)
		, usage(usage)
	{
	}

	VertexBuffer::~VertexBuffer()
	{
	}

	// VertexArray

	VertexArray::VertexArray(size_t size, GLenum target, GLenum usage)
		: VertexBuffer(size, target, usage)
		, buf(new char[size])
	{
	}

	VertexArray::~VertexArray()
	{
		delete [] buf;
	}

    void *VertexArray::map(bool read_from_buffer)
	{
		return buf;
	}

	void VertexArray::unmap()
	{
	}

	void VertexArray::bind() const
	{
	}

	void VertexArray::unbind() const
	{
	}

	void VertexArray::fill(size_t offset, size_t size, const void *data)
	{
		memcpy(buf + offset, data, size);
	}

	const void *VertexArray::getPointer(size_t offset) const
	{
		return buf + offset;
	}

#ifndef UGDK_USING_GLES
	// VBO

	VBO::VBO(size_t size, GLenum target, GLenum usage)
		: VertexBuffer(size, target, usage)
		, vbo(0)
		, buffer_copy(nullptr)
		, mapped(nullptr)
	{
		if (!GLEW_VERSION_1_5)
			throw system::BaseException("Not supported");

		bool ok = load(false);

		if (!ok)
            throw system::BaseException("Could not load VBO.");
	}

	VBO::~VBO()
	{
		if (vbo != 0)
			unload(false);
	}

    void *VBO::map(bool read_from_buffer)
	{
		// mapping twice could result in memory leaks
		if (mapped)
            throw system::BaseException("VBO is already mapped!");

		mapped = malloc(getSize());
		if (!mapped)
            throw system::BaseException("Out of memory (oh the humanity!)");
    
        if (read_from_buffer) {
            glGetBufferSubData(getTarget(), 0, getSize(), mapped);
            ugdk::internal::AssertNoOpenGLError();
        }

		return mapped;
	}

	void VBO::unmap()
	{
		glBufferSubData(getTarget(), 0, getSize(), mapped);
		free(mapped);
		mapped = nullptr;
	}

	void VBO::bind() const
	{
		glBindBuffer(getTarget(), vbo);
	}

	void VBO::unbind() const
	{
		glBindBuffer(getTarget(), 0);
	}

	void VBO::fill(size_t offset, size_t size, const void *data)
	{
		if (mapped)
			memcpy(static_cast<char*>(mapped) + offset, data, size);
		else
			glBufferSubData(getTarget(), offset, size, data);
	}

	const void *VBO::getPointer(size_t offset) const
	{
		return reinterpret_cast<const void*>(offset);
	}

	bool VBO::loadVolatile()
	{
		return load(true);
	}

	void VBO::unloadVolatile()
	{
		unload(true);
	}

	bool VBO::load(bool restore)
	{
		glGenBuffers(1, &vbo);

		VertexBuffer::Bind bind(*this);

		// Copy the old buffer only if 'restore' was requested.
		const GLvoid *src = restore ? buffer_copy : nullptr;

		while (GL_NO_ERROR != glGetError())
			/* clear error messages */;

		// Note that if 'src' is '0', no data will be copied.
		glBufferData(getTarget(), getSize(), src, getUsage());
		GLenum err = glGetError();

		// Clean up buffer_copy, if it exists.
		delete[] buffer_copy;
		buffer_copy = nullptr;

		return (GL_NO_ERROR == err);
	}

	void VBO::unload(bool save)
	{
		// Clean up buffer_copy, if it exists.
		delete[] buffer_copy;
		buffer_copy = nullptr;

		// Save data before unloading.
		if (save)
		{
			VertexBuffer::Bind bind(*this);

			GLint size;
			glGetBufferParameteriv(getTarget(), GL_BUFFER_SIZE, &size);

			const char *src = static_cast<char *>(map(true));

			if (src)
			{
				buffer_copy = new char[size];
				memcpy(buffer_copy, src, size);
				unmap();
			}
		}

		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}
#endif // UGDK_USING_GLES

} // namespace graphic
} // namespace ugdk
