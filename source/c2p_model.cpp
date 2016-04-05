#include "../include/c2p_model.h"

void C2P_Model::runover_init(void* rundata) {
	C2P::RunData* runData = reinterpret_cast<C2P::RunData*>(rundata);
	C2P_DataManipulator::C2P_FileData fileData;
	fileData.dataType = DataType::IMAGE;
	fileData.fileName = runData->inFileName;
	eg::DataReference dataReference;
	dataReference.dataPointer = static_cast<void*>(&fileData);
	dataReference.dataType = DataType::IMAGE;
	if (loadData(dataReference) == eg::GlbRtrn::RTRN_FAILURE) {
		std::cout << "C2P_Model: Notice: Image probably wrong size, max size: " << C2P::IMAGE_WIDTH << "x" << C2P::IMAGE_HEIGHT << " - Convertng anyway..." << std::endl;
	}
	
	if (!fileData.imageData->rawDataSize) {
		std::cout << "C2P_Model: Error: No image data detected, abort" << std::endl;
		abort();
	}

	//Compress Data to zlib format
	eg::util::edianSwap(static_cast<void*>(fileData.imageData->rawData), fileData.imageData->rawDataSize, sizeof(uint16_t));
	int result;
	uLongf outbuf_len = compressBound(fileData.imageData->rawDataSize);
	uint8_t* outbuf = new uint8_t[outbuf_len];
	result = compress2(outbuf, &outbuf_len, fileData.imageData->rawData, fileData.imageData->rawDataSize, Z_DEFAULT_COMPRESSION);
	if (result != Z_OK) {
		abort();
	}

	//Add into header and whatever

	//Copy into full size header
	size_t outfilebuf_len = C2P::OUTPUT_HEADER_SIZE + C2P::OUTPUT_FOOTER_SIZE + outbuf_len;

	uint8_t* outfilebuf = new uint8_t[outfilebuf_len];
	memcpy(outfilebuf, C2P::OUTPUT_HEADER, C2P::OUTPUT_HEADER_SIZE);
	memcpy(outfilebuf + C2P::OUTPUT_HEADER_SIZE, outbuf, outbuf_len);
	memcpy(outfilebuf + C2P::OUTPUT_HEADER_SIZE + outbuf_len, C2P::OUTPUT_FOOTER, C2P::OUTPUT_FOOTER_SIZE);
	
	uint8_t* offset;
	
	//Part A
	uint32_t data = ~(outfilebuf_len & 0xFFFFFF);
	offset = outfilebuf + C2P::OUTPUT_HEADER_OFFSET_A;
	*reinterpret_cast<uint32_t*>(offset) = data;

	//Part C
	data = outfilebuf_len - 0x20;
	offset = outfilebuf + C2P::OUTPUT_HEADER_OFFSET_C;
	*reinterpret_cast<uint32_t*>(offset) = data;

	//Part D
	data = outfilebuf_len - 0x234;
	offset = outfilebuf + C2P::OUTPUT_HEADER_OFFSET_D;
	*reinterpret_cast<uint32_t*>(offset) = data;

	//Part E
	data = outfilebuf_len - 0x254;
	offset = outfilebuf + C2P::OUTPUT_HEADER_OFFSET_E;
	*reinterpret_cast<uint32_t*>(offset) = data;

	//Part F
	data = outfilebuf_len - 0x258;
	offset = outfilebuf + C2P::OUTPUT_HEADER_OFFSET_F;
	*reinterpret_cast<uint32_t*>(offset) = data;

	//Now that all the uint32_t have been done
	eg::util::edianSwap(outfilebuf, C2P::OUTPUT_HEADER_SIZE, sizeof(uint32_t));
	eg::util::edianSwap(outfilebuf + C2P::OUTPUT_HEADER_SIZE + outbuf_len, C2P::OUTPUT_FOOTER_SIZE, sizeof(uint32_t));

	//Part W
	uint16_t data16 = (fileData.imageData->image_width & 0xFFFF);
	offset = outfilebuf + C2P::OUTPUT_HEADER_OFFSET_W;
	*reinterpret_cast<uint16_t*>(offset) = data16;
	eg::util::edianSwap(offset, sizeof(uint16_t), sizeof(uint16_t));

	//Part H
	data16 = (fileData.imageData->image_height & 0xFFFF);
	offset = outfilebuf + C2P::OUTPUT_HEADER_OFFSET_H;
	*reinterpret_cast<uint16_t*>(offset) = data16;
	eg::util::edianSwap(offset, sizeof(uint16_t), sizeof(uint16_t));

	//Part B
	uint8_t data8 = (0x1d1 - (outfilebuf_len & 0xff)) & 0xff;
	offset = outfilebuf + C2P::OUTPUT_HEADER_OFFSET_B;
	*offset = data8;

	C2P_DataManipulator::C2P_FileData write_fileData;
	write_fileData.dataType = DataType::IMAGE_C2P;
	write_fileData.dataSize = outfilebuf_len;
	write_fileData.dataPointer = static_cast<void*>(outfilebuf);
	write_fileData.fileName = runData->outFileName;
	writeData(static_cast<eg::DataReference const&>(write_fileData));

	//Cleanup
	freeData(dataReference);
	delete[] outbuf;
	delete[] outfilebuf;
	
	//Quit
	runData->quit = true;
}