#include <iostream>
#include <assert.h>
#include <zlib.h>
#include <FreeImage.h>
#include "../include/base/eg_engine.h"
#include "../include/dataType.h"
#include "../include/c2p_dataManipulator.h"
#include "../include/c2p_input.h"
#include "../include/c2p_controller.h"

int main(int argc, char** argv) {
	C2P_Controller c2p_controller;
	C2P_Input::RunData rundata;
	rundata.argc = argc;
	rundata.argv = argv;
	rundata.quit = false;
	while (!rundata.quit) {
		c2p_controller.runover_controller(static_cast<void*>(&rundata));
		c2p_controller.runover_dataManipulator(static_cast<void*>(&rundata));
		c2p_controller.runover_input(static_cast<void*>(&rundata));
		c2p_controller.runover_model(static_cast<void*>(&rundata));
		c2p_controller.runover_view(static_cast<void*>(&rundata));
	}
	//system("pause");
}