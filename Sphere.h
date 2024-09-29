#pragma once

class Sphere
{

public:
	Sphere();


	struct PrimitiveBuffer* GetBuffer();
private:

	static std::unique_ptr<struct PrimitiveBuffer> buffer;
};

