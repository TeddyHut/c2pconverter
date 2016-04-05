#include "../include/c2p_dataManipulator.h"

C2P_DataManipulator::C2P_DataManipulator() {
	dataManipulator_loadData_exe = [](eg::DataReference& dataReference)->eg::GlbRtrn { return(dataManipulator_loadData_impl(dataReference)); };
	dataManipulator_freeData_exe = [](eg::DataReference& dataReference)->eg::GlbRtrn { return(dataManipulator_freeData_impl(dataReference)); };
	dataManipulator_writeData_exe = [](eg::DataReference const& dataReference)->eg::GlbRtrn { return(dataManipulator_writeData_impl(dataReference)); };
	functionType.functionType.push_back(DataType::IMAGE);
	functionType.functionType.push_back(DataType::IMAGE_C2P);
}

eg::GlbRtrn C2P_DataManipulator::dataManipulator_loadData_impl(eg::DataReference& dataReference) {
	eg::GlbRtrn rtrn = eg::GlbRtrn::RTRN_SUCCESS;
	switch (dataReference.dataType) {
	case(DataType::IMAGE) : {
		//Load Image
		C2P_FileData* fileData = static_cast<C2P_FileData*>(dataReference.dataPointer);
		if (TRACKING) std::cout << "dataManipulator_loadData_impl: Loading file: " << fileData->fileName << std::endl;
		FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(fileData->fileName.c_str()), fileData->fileName.c_str());
		if ((FreeImage_GetWidth(bitmap) > C2P::IMAGE_WIDTH)||(FreeImage_GetHeight(bitmap) > C2P::IMAGE_HEIGHT)) {
			rtrn = GlbRtrn::IMAGE_WRONGSIZE;
		}

		//Create raw data
		FIBITMAP* rgb565 = FreeImage_ConvertTo16Bits565(bitmap);
		FreeImage_Unload(bitmap);

		ImageData* imageData = new ImageData;
		imageData->image_width = FreeImage_GetWidth(rgb565); if (TRACKING) std::cout << "dataManipulator_loadData_impl: image_width: " << imageData->image_width << std::endl;
		imageData->image_height = FreeImage_GetHeight(rgb565); if (TRACKING) std::cout << "dataManipulator_loadData_impl: image_height: " << imageData->image_height << std::endl;
		imageData->image_pitch = FreeImage_GetPitch(rgb565); if (TRACKING) std::cout << "dataManipulator_loadData_impl: image_pitch: " << imageData->image_pitch << std::endl;
		imageData->rawDataSize = imageData->image_pitch*imageData->image_height; if (TRACKING) std::cout << "dataManipulator_loadData_impl: rawDataSize: " << std::hex << imageData->rawDataSize << std::endl;
		imageData->rawData = new uint8_t[imageData->rawDataSize];
		FreeImage_ConvertToRawBits(imageData->rawData, rgb565, imageData->image_pitch, 16, FI16_565_RED_MASK, FI16_565_GREEN_MASK, FI16_565_BLUE_MASK, true);

		fileData->imageData = imageData;
		FreeImage_Unload(rgb565);
		break;
	}
	default: {
		break;
	}
	}
	return(rtrn);
}

eg::GlbRtrn C2P_DataManipulator::dataManipulator_freeData_impl(eg::DataReference& dataReference) {
	eg::GlbRtrn rtrn = eg::GlbRtrn::RTRN_SUCCESS;
	switch (dataReference.dataType) {
	case(DataType::IMAGE) : {
		if(TRACKING) std::cout << "dataManipulator_freeData_impl: Unloading Image" << std::endl;
		C2P_FileData* fileData = static_cast<C2P_FileData*>(dataReference.dataPointer);
		delete[] fileData->imageData->rawData;
		delete fileData->imageData;
		break;
	}
	default: {
		break;
	}
	}
	return(rtrn);
}

eg::GlbRtrn C2P_DataManipulator::dataManipulator_writeData_impl(eg::DataReference const& dataReference) {
	eg::GlbRtrn rtrn = eg::GlbRtrn::RTRN_SUCCESS;
	switch (dataReference.dataType) {
	case(DataType::IMAGE_C2P) : {
		C2P_FileData const* fileData = static_cast<C2P_FileData const*>(&dataReference);
		if (TRACKING) std::cout << "dataManipulator_writeData_impl: Writing File: " << fileData->fileName << std::endl;
		std::ofstream outfile;
		outfile.open(fileData->fileName.c_str(), std::ios::binary);
		outfile.write(static_cast<char const*const>(fileData->dataPointer), fileData->dataSize);
		outfile.close();
		break;
	}
	default: {
		break;
	}
	}
	return(rtrn);
}