#pragma once
#include "Star.h"

class StarServer : public Star
{
public:
	CLASS_IDENTIFICATION('STAR', StarServer);
	
	StarServer(StarStatus InStarStatus = StarStatus())
	:Star(InStarStatus){}

	~StarServer(){}

private:
};