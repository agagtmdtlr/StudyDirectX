#pragma once


class Material
{
public:
	struct Description
	{
		float amb;
		float diff;
		float spec;
		float alpha;
	};

	Description desc;
};


