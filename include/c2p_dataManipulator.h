#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <FreeImage.h>
#include "../engine/include/base/eg_engine.h"
#include "dataType.h"

class C2P_DataManipulator : public eg::DataManipulator {
public:
	struct ImageData {
		uint8_t* rawData;
		size_t rawDataSize;
		size_t image_width;
		size_t image_height;
		size_t image_pitch;
	};
	struct C2P_FileData : public eg::DataReference {
		std::string fileName;
		ImageData* imageData;
	};
	C2P_DataManipulator();
protected:
	static eg::GlbRtrn dataManipulator_loadData_impl(eg::DataReference& dataReference);
	static eg::GlbRtrn dataManipulator_freeData_impl(eg::DataReference& dataReference);
	static eg::GlbRtrn dataManipulator_writeData_impl(eg::DataReference const& dataReference);
};