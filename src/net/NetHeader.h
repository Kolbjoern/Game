#pragma once

enum class NetHeader : unsigned short int
{
	Register = 0,
	Move = 1,
	Draw = 2,
	Death = 3
};
