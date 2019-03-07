#include "Renderable.h"
#include "../Core/Entity.h"
#include "../Loading/ModelLoader.h"
#include "../Loading/ImageLoader.h"
#include <string>

Renderable::Renderable()
{
}

Model* Renderable::getModel() {
	return _model;
}

void Renderable::setModel(Model& model) {
	_model = &model;
}

Transform Renderable::getTransform() {
	Entity* e = GetEntity();
	return e->transform;
}

Color Renderable::getColor() {
	return _color;
}

void Renderable::setColor(Color color) {
	_color = color;
}

#pragma region prefab support 

PrefabRegistrar<Renderable> Renderable::reg("Renderable");
Renderable::Renderable(json json)
{
	// parse color 
	if (json.find("color") != json.end())
	{
		_color = Color(
			json["color"]["r"].get<double>(),
			json["color"]["g"].get<double>(),
			json["color"]["b"].get<double>());
	}

	// parse geometry and texture
	_model = ModelLoader::loadModel(json["model_path"].get<std::string>());
	Image* i = ImageLoader::loadImage(json["texture_path"].get<std::string>());
	_model->setTexture(i);
}

#pragma endregion