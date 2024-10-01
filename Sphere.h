#pragma once

class Sphere
{

public:
	Sphere();
	

	struct PrimitiveBuffer* GetBuffer();
	UINT GetIndexCount();
private:

	static std::unique_ptr<struct PrimitiveBuffer> buffer;
};

