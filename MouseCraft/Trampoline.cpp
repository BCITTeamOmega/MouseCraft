#include "Trampoline.h"

Trampoline::Trampoline() : HandleOnCollision(this, &Trampoline::OnCollision)
{
	Contraption::type = CONTRAPTIONS::TRAMPOLINE;
}


Trampoline::~Trampoline()
{
}

bool Trampoline::use(Mouse* m) {
	std::cout << "Trampoline is being used" << std::endl;

	auto pos = GetEntity()->transform.getWorldPosition();
	auto up = GetEntity()->GetParent()->GetComponent<PhysicsComponent>()->isUp;

	//physics
	auto ptype = up ? PhysObjectType::CONTRAPTION_UP : PhysObjectType::CONTRAPTION_DOWN;
	auto c_physics = PhysicsManager::instance()->createGridObject(pos.x, pos.z, 5, 5, ptype);

	GetEntity()->transform.setLocalPosition(pos);
	GetEntity()->SetParent(OmegaEngine::Instance().GetRoot());
	fieldEntity->SetEnabled(true);

	_isPlaced = true;

	auto ctype = up ? PhysObjectType::MOUSE_UP : PhysObjectType::MOUSE_DOWN;
	checkFor.insert(ctype);

	GetEntity()->AddComponent(c_physics);

	return true;
	
}

void Trampoline::show() {
	Contraption::show();
}

void Trampoline::OnCollision(PhysicsComponent * other)
{

}

void Trampoline::OnInitialized() {
	Contraption::OnInitialized();
}

void Trampoline::Update(float dt) {
	if (!_isPlaced) return;

	auto pos = GetEntity()->transform.getWorldPosition();
	auto bl = glm::vec2(pos.x, pos.z) + glm::vec2(-1, -1);
	auto tr = glm::vec2(pos.x, pos.z) + glm::vec2(1, 1);

	auto hits = PhysicsManager::instance()->areaCheck(nullptr, checkFor, new Vector2D(bl), new Vector2D(tr));
	bool hitMice = hits.size() > 0;

	if (hits.size() > 0)
	{
		//for now i'm just going to destroy the trampoline after 1 jump
		std::cout << "Mouse touched trampoline" << std::endl;
		
		for (int i = 0; i < hits.size(); i++)
		{
			hits[i]->GetEntity()->GetComponent<PhysicsComponent>()->onBounce.Notify(GetEntity()->GetComponent<PhysicsComponent>());
		}

		//destroy trampoline
	}
}

Component* Trampoline::Create(json json) 
{
	auto c = ComponentManager<Trampoline>::Instance().Create<Trampoline>();
	c->_isPlaced = json["_isPlaced"].get<bool>();
	return c;
}

PrefabRegistrar Trampoline::reg("Trampoline", Trampoline::Create);