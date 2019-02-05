#pragma once
#include <vector>
#include "Object.h"

using namespace std;

class S_ObjectManager
{
private:
	vector<Object> m_objects;

private:
	S_ObjectManager();

	void loadObjects();

public:
	~S_ObjectManager();
	static S_ObjectManager& get()
	{
		static S_ObjectManager o;
		return o;
	}

	vector<Object>& getObjects();
};
