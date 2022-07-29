#pragma once

#include "../ModelObject.h"

class EnvironmentObject :public ModelObject
{
public:
	EnvironmentObject() = default;
	virtual ~EnvironmentObject() = default;

	virtual void SetStageData(const StageData& stagedata) { m_stageData = stagedata; }

protected:
	StageData m_stageData;
};