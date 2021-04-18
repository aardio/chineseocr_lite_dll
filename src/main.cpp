 
#include <omp.h>
#include <cstdio>
#include "main.h"
#include "version.h"
#include "OcrLite.h"
#include "OcrUtils.h"


int __stdcall DllMain(void * hinstDLL, unsigned long fdwReason, void * lpvReserved) {

	if (fdwReason == 1/*DLL_PROCESS_ATTACH*/) {
 
	}
	return 1;
}

extern "C" _declspec(dllexport) OcrLite* OcrCreateW(int numThread, const wchar_t * detModelPath, const wchar_t * clsModelPath,
	const wchar_t * recModelPath, const wchar_t * keysPath, int isOutputConsole,int isOutputResultImg) {
	omp_set_num_threads(numThread);

	OcrLite * ocrLite = new OcrLite();
	ocrLite->setNumThread(numThread);

	if(isOutputConsole){
		freopen("CONIN$", "r+t", stdin);
		freopen("CONOUT$", "w+t", stdout);
		freopen("CONOUT$", "w+t", stderr);
	}

	ocrLite->initLogger(
		isOutputConsole, //true,//isOutputConsole
		false, //false,//isOutputPartImg
		isOutputResultImg //true);//isOutputResultImg
	);

	ocrLite->initModels(detModelPath, clsModelPath, recModelPath, keysPath);
	return ocrLite;

}

extern "C" _declspec(dllexport)  void OcrDelete(OcrLite *ocrLite) {

	if (ocrLite) delete ocrLite;
}

struct OCR_DETECT_CONFIG {
	int padding; 
	int maxSideLen; 
	float boxScoreThresh; 
	float boxThresh; 
	float unClipRatio; 
	int doAngle;
	int mostAngle;
};

struct OCR_DETECT_RESULT_POINT {
	int x;
	int y;
};
 
struct OCR_DETECT_RESULT_TEXTBLOCK { 
	char * text;
	float boxScore;
	unsigned int charScoreSize;
	float *charScores;
	OCR_DETECT_RESULT_POINT boxPoint[4];
};

struct OCR_DETECT_RESULT {
	unsigned int blockSize;
	char * text;
	char * resultImageBuffer;
	unsigned int resultImageBufferSize;
	OCR_DETECT_RESULT_TEXTBLOCK  blocks[0];
};

extern "C" _declspec(dllexport) OCR_DETECT_RESULT * OcrDetect(OcrLite *ocrLite, char * buffer, size_t bufferSize, OCR_DETECT_CONFIG * config) {

	OcrResult ocrData = ocrLite->detect(buffer, bufferSize,config->padding, config->maxSideLen,
		config->boxScoreThresh, config->boxThresh, config->unClipRatio, config->doAngle, config->mostAngle);
	
	size_t textblockSize = ocrData.textBlocks.size(); 
	if (!textblockSize) return nullptr;

	OCR_DETECT_RESULT * result = (OCR_DETECT_RESULT *)malloc(sizeof(OCR_DETECT_RESULT) + sizeof(OCR_DETECT_RESULT_TEXTBLOCK) * textblockSize);
	result->text = strdup(ocrData.strRes.c_str());
	result->resultImageBufferSize = ocrData.resultImageBuffer.size();
	if (result->resultImageBufferSize) { 
		result->resultImageBuffer = (char *)malloc(result->resultImageBufferSize);
		memcpy((void *)result->resultImageBuffer, (void *)&ocrData.resultImageBuffer[0], result->resultImageBufferSize);
	}
	else {
		result->resultImageBuffer = nullptr;
	}

	result->blockSize = textblockSize; 

	for (int i = 0; i < textblockSize; i++) {   
		auto tb = &ocrData.textBlocks[i];
		auto tbOut = &result->blocks[i];

		for (int j = 0; j < 4; j++) {
			tbOut->boxPoint[j].x = tb->boxPoint[j].x;
			tbOut->boxPoint[j].y = tb->boxPoint[j].y; 
		}; 
		 
		if(tb->text.size())tbOut->text = strdup(tb->text.c_str());
		else tbOut->text = nullptr;

		tbOut->boxScore = tb->boxScore;

		if (tb->charScores.size()) {
			tbOut->charScoreSize = tb->charScores.size();
			tbOut->charScores = (float *)malloc(sizeof(float)*tb->charScores.size());
			memcpy((void *)tbOut->charScores, (void *)&tb->charScores[0], sizeof(float)*tb->charScores.size());
		}
		else {
			tbOut->charScores = nullptr;
			tbOut->charScoreSize = 0;
		}

	}

	return result; 
}


extern "C" _declspec(dllexport)  void OcrFreeDetectResult(OCR_DETECT_RESULT * result) {  
	for (int i = 0; i < result->blockSize; i++) {  
		if(result->blocks[i].text) free( (void *)result->blocks[i].text); 
		if(result->blocks[i].charScores) free((void *)result->blocks[i].charScores);
	} 

	if(result->text)free((void *)result->text);
	if (result->resultImageBuffer) free(result->resultImageBuffer);
	free(result);
} 