#pragma once

#include <iostream>
#include <string>
#include "../engine/include/base/eg_engine.h"
#include "../include/dataType.h"


class C2P_Input : public eg::Input {
public:
	struct RunData : public C2P::RunData {
		int argc;
		char** argv;
	};
	void runover_init(void* rundata);
};