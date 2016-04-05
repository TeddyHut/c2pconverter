#pragma once

#include <zlib.h>
#include <cstring>

#include "../engine/include/base/eg_engine.h"
#include "c2p_dataManipulator.h"
#include "dataType.h"

class C2P_Model : public eg::Model {
public:
	void runover_init(void* rundata = nullptr);
protected:
};