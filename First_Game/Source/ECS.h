#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

//manager controls entitys, entity controls components

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID getComponentTypeID() {
	static ComponentID lastID = 0;
	return lastID++;
}
template <typename T> inline ComponentID getComponentTypeID() noexcept{
	static ComponentID typeID = getComponentTypeID();  //only runs once per type because of static so only new component types will get a new value for typeID
	return typeID;
}
constexpr std::size_t maxComponents = 32;    //constexpr means it can be evaluate at compile time

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component {
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw () {}

	virtual ~Component() {}
};

class Entity
{
private:
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;
	ComponentArray	componentArray;
	ComponentBitSet componentBitSet;
public:
	void update() {
		for (auto& c : components) c->update();
	}
	void draw() {
		for (auto& c : components) c->draw();
	}
	bool isActive() const { return active; }
	void destroy() { active = false; }

	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs) {                 //argument forwarding, maintains lvalue and rvalue types enabling perfect forwarding
		T* c(new T(std::forward<TArgs>(mArgs)...));     //() means direct initilization , creates a new pointer to component type T
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };		    
		components.emplace_back(std::move(uPtr));    //emplace_back faster and move required to keep pointer unique

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;
	
		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;

public:
	void update() {
		for (auto& e : entities) e->update();
	}
	void draw() {
		for (auto& e : entities) e->draw();
	}
	void refresh() {
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),     //moves entities that are active to begining of vector then erases entities that come after
			[](const std::unique_ptr<Entity> & mEntity)
		{
			return !mEntity->isActive();
		}),
			std::end(entities));
	}
	
	Entity& addEntity() {
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};