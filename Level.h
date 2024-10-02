#pragma once

class Level
{
public:
	Level(string name);
private:
	string name;

	vector<shared_ptr<class Mesh>> meshes;
};

