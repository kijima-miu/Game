#pragma once
class IGameObject
{
public:
	virtual ~IGameObject()
	{
	}
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Start() = 0;

};

