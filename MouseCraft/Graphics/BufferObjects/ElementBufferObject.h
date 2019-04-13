#pragma once
#include "../../GL/glad.h"
#include <vector>

/// <summary>
/// EBO (Element Buffer Object)
/// An OpenGL buffer which can pass a list of vertex indices to the GPU
/// Used for the VAO typically
/// Acts as a wrapper around the OpenGL EBO
/// </summary>
class ElementBufferObject {
public:
	/// <summary>
	/// Initializes the EBO object. This creates it in OpenGL using glCreateBuffers
	/// </summary>
	ElementBufferObject();

	/// <summary>
	///	Destructs the EBO object. This destroys it in OpenGL using glDeleteBuffers
	/// </summary>
	~ElementBufferObject();

	/// <summary>
	/// Get the ID of the EBO
	/// This is the ID used by OpenGL
	/// </summary>
	/// <returns>The EBO ID</returns>
	GLuint getID();

	/// <summary>
	/// Send an array of vertex indices to OpenGL.
	/// This automatically binds the EBO if it is not bound.
	/// </summary>
	/// <param name="elements">The array of vertex indices</param>
	void buffer(std::vector<GLuint>& elements);
	
	/// <summary>
	/// Bind the EBO in OpenGL using glBindBuffer
	/// </summary>
	void bind();

	/// <summary>
	/// Unbind the EBO in OpenGL.
	/// This simply calls glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)
	/// In other words if you had another buffer bound, it would unbind that one.
	/// </summary>
	void unbind();
private:
	/// <summary>
	/// The ID used by OpenGL to identify the EBO
	/// </summary>
	GLuint _id;
};