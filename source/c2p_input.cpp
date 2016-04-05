#include "../include/c2p_input.h"

void C2P_Input::runover_init(void* rundata) {
	RunData* runData = static_cast<RunData*>(rundata);
	if (runData->argc < 2) {
		std::cout << "C2P_input: runover_init: Param error, abort" << std::endl;
		abort();
	}
	std::string* argv_s = new std::string[runData->argc - 1];
	for (size_t i = 0; i < static_cast<size_t>(runData->argc) - 1; i++) {
		argv_s[i] = runData->argv[i+1];
	}
	for (size_t i = 0; i < static_cast<size_t>(runData->argc) - 1; i++) {
		if (argv_s[i].find("-i") != std::string::npos) {
			i++;
			runData->inFileName = argv_s[i];
			continue;
		}
		if (argv_s[i].find("-o") != std::string::npos) {
			i++;
			runData->outFileName = argv_s[i];
			continue;
		}
		if (argv_s[i].find("-t") != std::string::npos) {
			TRACKING = true;
			continue;
		}
	}
	if ((!runData->inFileName.size()) || (!runData->outFileName.size())) {
		std::cout << "C2P_input: runover_init: Param error, abort" << std::endl;
		abort();
	}
}