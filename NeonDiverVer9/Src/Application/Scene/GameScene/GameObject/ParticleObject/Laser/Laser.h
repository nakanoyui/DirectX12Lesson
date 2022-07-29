#pragma once

#include "../ParticleObject.h"

class Laser :public ParticleObject
{
public:
	Laser() = default;
	~Laser() = default;

	void Init()override;
	void Update()override;
	void DrawParticle()override;

	void ImGuiUpdateParticle()override;

	void ReferenceJson()override;
private:

};