#pragma once
#include "../../GL/glad.h"
#include "../GLTexture.h"
#include <vector>
/// <summary>
/// FBO (Framebuffer Object)
/// An OpenGL frame buffer, used for defining an alternate render target
/// This acts as a wrapper around framebuffer functions in OpenGL
/// This also contains textures to render to, and by default generates a depth buffer automatically.
/// </summary>
class FrameBufferObject {
public:
	/// <summary>
	/// Initializes the FBO with necessary information, preparing it in OpenGL
	/// This will automatically clear and attach the textures and call glDrawBuffers()
	/// </summary>
	/// <param name="width">The width of the FBO</param>
	/// <param name="height">The height of the FBO</param>
	/// <param name="textures">The color attachments to put in the FBO</param>
	FrameBufferObject(int width, int height, std::vector<GLTexture*>& textures);
	
	/// <summary>
	/// Destructs the FBO, destroying it in OpenGL
	/// </summary>
	~FrameBufferObject();

	/// <summary>
	/// Bind the FBO in OpenGL
	/// </summary>
	void bind(GLuint type = GL_FRAMEBUFFER);

	/// <summary>
	/// Unbind the FBO in OpenGL
	/// This effectively just calls glBindFrameBuffer(GL_FRAMEBUFFER, 0)
	/// So it will unbind any FBO attached regardless of whether it is this one
	/// </summary>
	void unbind(GLuint type = GL_FRAMEBUFFER);

	/// <summary>
	/// Attach a texture to the FBO on the specified attachment
	/// This doesn't call glDrawElements
	/// </summary>
	/// <param name="attachment">The attachment to attach the texture to</param>
	/// <param name="texture">The texture to add</param>
	void buffer(GLuint attachment, GLTexture& texture);

	/// <summary>
	/// Get the ID assigned by OpenGL for the FBO
	/// </summary>
	/// <returns>The ID of the FBO</returns>
	GLuint getID();
	
	void blit(FrameBufferObject& source, int srcWidth, int srcHeight, GLuint type = GL_COLOR_BUFFER_BIT);
	int getWidth();
	int getHeight();
private:
	/// <summary>
	/// Attaches buffers to the FBO
	/// This sets the attachments in the order they are received in the vector.
	/// And calls glDrawBuffers accordingly with the attachments
	/// </summary>
	/// <param name="buffers"></param>
	void attachBuffers(std::vector<GLTexture*>& buffers);

	GLuint _id;
	GLuint _rbo;
	int _width;
	int _height;
};