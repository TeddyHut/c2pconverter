#pragma once

#include <stdint.h>
#include <string>

extern bool TRACKING;

enum DataType {
	IMAGE,
	IMAGE_C2P
};

enum GlbRtrn{
	IMAGE_WRONGSIZE = 2
};

namespace C2P {
	struct RunData {
		bool quit;
		std::string inFileName;
		std::string outFileName;
	};
	extern size_t const IMAGE_WIDTH;
	extern size_t const IMAGE_HEIGHT;
	constexpr size_t OUTPUT_HEADER_SIZE = 0xdc;
	constexpr size_t OUTPUT_FOOTER_SIZE = 0x17c;
	extern uint32_t OUTPUT_HEADER[OUTPUT_HEADER_SIZE];
	extern uint32_t OUTPUT_FOOTER[OUTPUT_FOOTER_SIZE];
	extern size_t const OUTPUT_HEADER_OFFSET_A;
	extern size_t const OUTPUT_HEADER_OFFSET_B;
	extern size_t const OUTPUT_HEADER_OFFSET_C;
	extern size_t const OUTPUT_HEADER_OFFSET_D;
	extern size_t const OUTPUT_HEADER_OFFSET_E;
	extern size_t const OUTPUT_HEADER_OFFSET_F;
	extern size_t const OUTPUT_HEADER_OFFSET_W;
	extern size_t const OUTPUT_HEADER_OFFSET_H;
};
