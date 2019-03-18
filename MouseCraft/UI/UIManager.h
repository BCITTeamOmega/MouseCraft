#pragma once

#include <map>
#include <string>
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#include <windows.h>
#include <tinyxml2/tinyxml2.h>
#include "UIComponent.h"

/**
Static structure used to store the UI data, and methods to interact with UI
*/
class UIManager {
public:
	static void Initialize(float width, float height, std::string uiLayout);

	// Resize the root element (and consequently every element)
    static void Resize();

	// Process a click and execute any relevant click functions
	static void ClickUI(float x, float y);

	// Add a new UIComponent to the root component
    static void AddToRoot(UIComponent *component);

	// Search for a component by it's ID string, and return it
	static UIComponent* GetComponentById(std::string id);

	// Define an on-click function passing in its label and the function to execute
    static void DefineClickFunction(const std::string name, void(*f)());

	static void DrawUI();
private:
	// Initial definition of all on-click functions
	static void defineClicks();

	// Find the top element under your cursor that has an on-click event and store it in the top pointer location
	static void findTopClick(UIComponent** top, UIComponent* comp, const float x, const float y);

	// Is the given point in the defined rectangle
    static bool pointInRect(float px, float py, float rTop, float rRight, float rLeft, float rBottom);

	// Recursive function used to traverse UI tree and draw elements
	static void traverseChildren(UIComponent *component);

	// Temp draw function until renderer implemented
	static void drawUIComponent(const UIComponent *component);

	// Reads an XML file and populates the UI system with elements generated from it
	static void loadFromXML(const char *filePath);

	// Recursive call for reading an XML element into a UIComponent
	static UIComponent* readChild(const tinyxml2::XMLElement* element);

	UIManager() {}

    static UIComponent *_root;
    static std::map<const std::string, void(*)()> _clickFunctions;
	static std::vector<const UIComponent*> _transparentList;
};