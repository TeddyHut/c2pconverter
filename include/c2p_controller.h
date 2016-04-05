#pragma once

#include <iostream>
#include "../engine/include/base/eg_engine.h"
#include "c2p_dataManipulator.h"
#include "c2p_model.h"
#include "c2p_input.h"

class C2P_Controller : public eg::Controller {
public:
	C2P_Controller();
protected:
	C2P_DataManipulator c2p_dataManipulator;
	C2P_Model c2p_model;
	C2P_Input c2p_input;
};