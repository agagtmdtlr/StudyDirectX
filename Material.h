#pragma once


class Material
{
public:
	struct Description
	{
		float amb = 0;
		float diff= 0;
		float spec = 0;
		float alpha = 1;
	};

	Description desc;
};


