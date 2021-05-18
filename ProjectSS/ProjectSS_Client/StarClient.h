#pragma once
#include "Star.h"

class StarClient : public Star
{
public:
	CLASS_IDENTIFICATION('STAR', StarClient);

	StarClient(StarStatus InStarStatus = StarStatus())
		:Star(InStarStatus) {}

	~StarClient() {}

	virtual void Render() override;
};