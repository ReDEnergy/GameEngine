#pragma once

#define CLEAR_USING_COMPUTE_SHADER

#include <include/gl.h>

#ifdef CLEAR_USING_COMPUTE_SHADER
	#include <Utils/OpenGL.h>
	#include <Manager/Manager.h>
	#include <Manager/ShaderManager.h>
#endif

template <class StorageEntry>
class SSBO
{
	public:
		SSBO(unsigned int size)
		{
			data = nullptr;
			this->size = size;
			memorySize = size * sizeof(StorageEntry);

			#ifdef GLEW_ARB_shader_storage_buffer_object
			{
				glGenBuffers(1, &ssbo);
				Bind();
				glBufferData(GL_SHADER_STORAGE_BUFFER, memorySize, NULL, GL_DYNAMIC_DRAW);
				Unbind();
			}
			#endif
		}

		~SSBO()
		{
			glDeleteBuffers(1, &ssbo);
			SAFE_FREE_ARRAY(data);
		};

		void SetBufferData(const StorageEntry *data, GLenum usage = GL_DYNAMIC_DRAW)
		{
			Bind();
			glBufferData(GL_SHADER_STORAGE_BUFFER, memorySize, data, GL_DYNAMIC_DRAW);
			Unbind();
		}

		void SetBufferSubData(const StorageEntry *data, int offset, int size)
		{
			Bind();
			glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size * sizeof(StorageEntry), data);
			Unbind();
		}

		void BindBuffer(GLuint index) const
		{
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ssbo);
		}

		void ReadBuffer()
		{
			if (data == nullptr)
			{
				data = new StorageEntry[size];
			}

			Bind();
			GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
			memcpy(data, p, memorySize);
			CheckOpenGLError();
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
			CheckOpenGLError();
			Unbind();
		}

		const StorageEntry* GetBuffer() const
		{
			return data;
		}

		unsigned int GetSize() const
		{
			return size;
		}

		unsigned int GetBufferSize() const
		{
			return size * sizeof(StorageEntry);
		}

		void ClearBuffer() const
		{
			// Clear Buffer Object using a compute shader
			#ifdef CLEAR_USING_COMPUTE_SHADER
			{
				Shader *S = Manager::GetShader()->GetShader("ClearBuffer");
				S->Use();
				BindBuffer(0);
				OpenGL::DispatchCompute(memorySize, 1, 1, 32);
				glMemoryBarrier(GL_ALL_BARRIER_BITS);
			}
			#else
				Bind();
				uint value = 0;
				glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32UI, GL_RED, GL_UNSIGNED_INT, &value);
				Unbind();
			#endif
			CheckOpenGLError();
		}

	private:
		inline void Bind() const
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
			CheckOpenGLError();
		}

		static inline void Unbind()
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			CheckOpenGLError();
		}

	private:
		unsigned int ssbo;
		unsigned int size;
		unsigned int memorySize;
		StorageEntry *data;
};
