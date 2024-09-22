#include "stdafx.h"
#include "Path.h"
#include <algorithm>
wstring Path::GetExtension(wstring filepath)
{
	size_t start = filepath.rfind(L'.');

	wstring extension = filepath.substr(start + 1);
	std::transform(extension.begin(), extension.end(), extension.begin(), towlower);

	return extension;
}
