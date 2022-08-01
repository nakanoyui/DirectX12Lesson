#pragma once

class ScriptComponent:public Component
{
public:
	ScriptComponent();
	~ScriptComponent() {}

	virtual void Init() override{}
	virtual void Start() override{}
	virtual void Update() override{}
	void Draw()final {}

protected:
};