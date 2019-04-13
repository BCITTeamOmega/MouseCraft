#pragma once
#include "../GL/glad.h"
#include <stdlib.h>
/// <summary>
/// Bitmap data with additional image information like width and height
/// </summary>
class Image {
public:
	/// <summary>
	/// Initializes the image with the image data, width, and height
	/// </summary>
	/// <param name="data">The bitmap data of the image</param>
	/// <param name="width">The width of the image</param>
	/// <param name="height">The height of the image</param>
	Image(unsigned char* data, int width, int height, int channels = 4)
		: _data(data),
		_width(width),
		_height(height),
		_channels(channels) {}

	/// <summary>
	/// </summary>
	~Image() {
		free(_data);
	};

	/// <summary>
	/// Get the bitmap data of the image
	/// </summary>
	/// <returns>The bitmap data of the image as an unsigned char array</returns>
	unsigned char* getData() { return _data; }

	/// <summary>
	/// Get the width of the image
	/// </summary>
	/// <returns>The width of the image</returns>
	int getWidth() { return _width; }

	/// <summary>
	/// Get the height of the image
	/// </summary>
	/// <returns>The height of the image</returns>
	int getHeight() { return _height; }

	int getChannels() { return _channels; }
private:
	/// <summary>
	/// The bitmap data of the image
	/// </summary>
	unsigned char* _data;

	/// <summary>
	/// The width of the image
	/// </summary>
	int _width;

	/// <summary>
	/// The height of the image
	/// </summary>
	int _height;

	int _channels;
};