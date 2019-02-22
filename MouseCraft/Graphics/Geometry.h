#pragma once
#include "../GL/glad.h"
#include <vector>

/// <summary>
/// A basic model geometry containing vertex data, normal data, texture coordinates, and face indices.
/// </summary>
class Geometry {
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Geometry() = default;

	/// <summary>
	/// Default destructor
	/// </summary>
	~Geometry() = default;

	/// <summary>
	/// Get the vertex data of the shape
	/// </summary>
	/// <returns>The vertex data as an array of GLfloats (3 per coordinate)</returns>
	std::vector<GLfloat>& getVertexData() { return _vertexData; }

	/// <summary>
	/// Set the vertex data of the shape
	/// </summary>
	/// <param name="vertexData">The vertex data as an array of GLfloats (3 per coodinate)</param>
	void setVertexData(std::vector<GLfloat>& vertexData) { _vertexData = vertexData; }

	/// <summary>
	/// Get the normal data of the shape
	/// </summary>
	/// <returns>The normal data as an array of GLfloats (3 per coodinate)</returns>
	std::vector<GLfloat>& getNormalData() { return _normalData; }

	/// <summary>
	/// Set the normal data of the shape
	/// </summary>
	/// <param name="normalData">The normal data as an array of GLfloats (3 per coordinate)</param>
	void setNormalData(std::vector<GLfloat>& normalData) { _normalData = normalData; }

	/// <summary>
	/// Get the texture coordinate data of the shape
	/// </summary>
	/// <returns>The texture coordinate data as an array of GLfloats (2 per coordinate)</returns>
	std::vector<GLfloat>& getTexCoordData() { return _texCoordData; }

	/// <summary>
	/// Set the texture coordinate data of the shape
	/// </summary>
	/// <param name="texCoordData">The texture coordinate data as an array of GLfloats (2 per coordinate)</param>
	void setTexCoordData(std::vector<GLfloat>& texCoordData) { _texCoordData = texCoordData; }

	/// <summary>
	/// Get the face indices of the shape
	/// </summary>
	/// <returns>The face indices as an array of GLuints (3 per triangle)</returns>
	std::vector<GLuint>& getIndices() { return _indices; }

	/// <summary>
	/// Set the face indices of the shape
	/// </summary>
	/// <param name="indices">The face indices as an array of GLuints (3 per triangle)</param>
	void setIndices(std::vector<GLuint>& indices) { _indices = indices; }
private:
	/// <summary>
	/// An array of vertex data. Each vertex is stored across 3 indices in the array.
	/// </summary>
	std::vector<GLfloat> _vertexData;

	/// <summary>
	/// An array of normal data. Each normal is stored across 3 indices in the array.
	/// </summary>
	std::vector<GLfloat> _normalData;

	/// <summary>
	/// An array of texture coordinate data. Each texture coordinate is stored across 2 indices in the array.
	/// </summary>
	std::vector<GLfloat> _texCoordData;

	/// <summary>
	/// The indices which define the triangles in the model. Each triangle face is 3 indices.
	/// </summary>
	std::vector<GLuint> _indices;
};

