#pragma once

class Resource
{
public:
	Resource();
	~Resource();

	char Type{};
	int Amount{};
	int MaxAmount{};
	int CitizenCount{};

};

