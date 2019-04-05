#pragma once

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "../Core/Component.h"
#include "../Core/Entity.h"
#include "../Graphics/Model.h"

enum VerticalAnchor {
    ANCHOR_TOP, ANCHOR_VCENTER, ANCHOR_BOTTOM
};

enum HorizontalAnchor {
    ANCHOR_LEFT, ANCHOR_HCENTER, ANCHOR_RIGHT
};

// The unit type for the anchor
enum AnchorType {
    ANCHOR_PERCENT, ANCHOR_PIXEL
};

// The unit type for the size of the UIComponent
enum UnitType {
    UNIT_PIXEL, UNIT_PERCENT, UNIT_SCALE
};

/**
Basic UI panel that makes up all of the UI system.
UI systems are made by creating multiple of these nested within each other
*/
class UIComponent : public Component {
public:
    UIComponent(float width, float height, float x, float y);
    ~UIComponent();

	// Recalculates screen position and size
    virtual void Resize();

	// Determine whether this panel uses transparency
    virtual bool IsTransparent() const;

	// String id used for uniquely identifying this UIComponent
	std::string			id;

	// Whether or not this UIComponent and its children should be drawn
    bool                visible;

	// UIComponent should be resized if Valid is set to false
	bool				valid;

	glm::vec4			color;

    glm::vec2           size;
    glm::vec2           anchor;
	int					z;
	int					zForce;
    VerticalAnchor      vAnchor;
    HorizontalAnchor    hAnchor;
    AnchorType          anchorXType;
    AnchorType          anchorYType;
    UnitType            xType;
    UnitType            yType;

	// If using scaling type sizing on one side, defines the ratio by which to scale on
    float               aspectRatio;

	// Pointer to the parent element
    UIComponent         *parent;

	// Calculated screen coordinates, size and rotation in pixels
    glm::vec2           screenPosition;
    glm::vec2           screenSize;
    float				screenRotation;

	// Name of UIComponent's associated action to take when clicked
	// Blank string if no action
    std::string         ClickAction;

	std::vector<Model*>* models;
protected:
	void calculateScreenPosition();
	virtual void setupModels();
};