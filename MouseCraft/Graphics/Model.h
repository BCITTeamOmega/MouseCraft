#pragma once
#include "Geometry.h"
#include "Image.h"
/// <summary>
/// A 3D model with geometry information and associated texture data.
/// If no texture is defined the renderer will simply render with a blank texture.
/// </summary>
class Model {
public:
	/// <summary>
	/// Initializes the model with just geometry information
	/// </summary>
	/// <param name="geometry">The geometry of the model</param>
	Model(Geometry* geometry) : _geometry(geometry), _texture(nullptr) {}

	/// <summary>
	/// Initializes the model with geometry and texture information
	/// </summary>
	/// <param name="geometry">The geometry of the model</param>
	/// <param name="texture">The texture of the model</param>
	Model(Geometry* geometry, Image* texture) : _geometry(geometry), _texture(texture) {}

	/// <summary>
	/// Get the geometry of the model
	/// </summary>
	/// <returns>The model geometry</returns>
	Geometry* getGeometry() { return _geometry; }

	/// <summary>
	/// Set the geometry of the model
	/// </summary>
	/// <param name="geometry">The model geometry to change to</param>
	void setGeometry(Geometry* geometry) { _geometry = geometry; }

	/// <summary>
	/// Get the texture of the model
	/// If nullptr, there is no texture for the model
	/// </summary>
	/// <returns>The image of the model texture</returns>
	Image* getTexture() { return _texture; }

	/// <summary>
	/// Set the texture of the model
	/// Can set to nullptr to set it to no texture
	/// </summary>
	/// <param name="image">An image for the model texture</param>
	void setTexture(Image* image) { _texture = image; }

	/// <summary>
	/// Returns whether the model has a texture or not
	/// </summary>
	/// <returns>True if the model has a texture, false if it doesn't</returns>
	bool hasTexture() { return _texture == nullptr; }
private:
	/// <summary>
	/// The geometry of the model
	/// </summary>
	Geometry* _geometry;

	/// <summary>
	/// The texture of the model
	/// </summary>
	Image* _texture;
};