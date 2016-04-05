#include "../include/c2p_controller.h"

C2P_Controller::C2P_Controller() {
	c2p_dataManipulator.set_boundController(this);
	c2p_model.set_boundController(this);
	c2p_input.set_boundController(this);
}