#include "Platform.h"

Platform::Platform()
{
}


Platform::~Platform()
{
}

void Platform::use() {
	Contraption::use();

	fieldEntity->SetParent(OmegaEngine::Instance().GetRoot());
	fieldEntity->transform.setLocalPosition(GetEntity()->transform.getWorldPosition());
	fieldEntity->SetEnabled(true);

}

void Platform::show() {
	Contraption::show();
}