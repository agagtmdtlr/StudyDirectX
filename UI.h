#pragma once
#include <functional>
class UI
{
public:
	UI() {}

	virtual ~UI() {}

	virtual void Render() = 0;
};

class UIButton
{
public:
	string label;
	//std::function<void> func;
};
