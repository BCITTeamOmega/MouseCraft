#pragma once
class Color {
public:
	Color() : _red(0.0f), _green(0.0f), _blue(0.0f) {}
	Color(float r, float g, float b) : _red(r), _green(g), _blue(b) { }
	float getRed() { return _red; }
	void setRed(float r) { _red = r; }
	float getGreen() { return _green; }
	void setGreen(float g) { _green = g; }
	float getBlue() { return _blue; }
	void setBlue(float b) { _blue = b; }
private:
	float _red;
	float _green;
	float _blue;
};