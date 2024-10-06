#pragma once

class ControllerFactory
{
public:
	virtual unique_ptr<class Controller> CreateUI(const std::string type);

};

