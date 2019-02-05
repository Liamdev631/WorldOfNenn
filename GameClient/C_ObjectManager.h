#pragma once
#include <vector>
#include "Object.h"
#include <SFML/Graphics.hpp>

using namespace std;

class C_ObjectManager
{
private:
	class DrawableObject : public sf::Drawable
	{
	private:
		Object m_obj;
		sf::RectangleShape m_shape;

	public:
		DrawableObject(Object o)
			: m_obj(o)
		{
			m_shape.setSize(sf::Vector2f(16, 16));
			m_shape.setPosition(sf::Vector2f((float)m_obj.position.x, (float)m_obj.position.y) * 16.f);
		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
	};

private:
	vector<Object> m_objects;

private:
	C_ObjectManager();

	void loadObjects();

public:
	~C_ObjectManager();
	static C_ObjectManager& get()
	{
		static C_ObjectManager o;
		return o;
	}

	vector<Object>& getObjects();
	void addObject(const Object& object);
	void removeObject(const Object& object);
};
