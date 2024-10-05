#pragma once
#include "Callback.h"
class UI
{
public:
	UI() {}

	virtual ~UI() {}

	virtual void Render() = 0;

	std::string label;
	unique_ptr<Callback> callback;
};

