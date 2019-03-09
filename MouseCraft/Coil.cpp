#include "Coil.h"

Coil::Coil()
{
}


Coil::~Coil()
{
}

void Coil::use() {
	Contraption::use();
	
	// drop coil field
	fieldEntity->SetParent(OmegaEngine::Instance().GetRoot());
	fieldEntity->transform.setLocalPosition(GetEntity()->transform.getWorldPosition());
	fieldEntity->SetEnabled(true);
}

void Coil::show() {
	Contraption::show();
}