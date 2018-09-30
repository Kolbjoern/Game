#pragma once

enum class NetHeader : unsigned short int
{
	Register = 0,
	Action = 1,
	Draw = 2,
	Death = 3,
	Assign = 4
};
