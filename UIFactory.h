#pragma once

class UIFactory
{
public:
	virtual unique_ptr<class UI> CreateUI(const std::string type);

};

