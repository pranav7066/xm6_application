/* Copyright 2021 OpenFive */
/*
 *  XM6 based CDNN application
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <CDNNBringupInfo.h>
#include "CDNNUserInterface.h"
#include "platform_manager.h"
#include <cnn_runtime_if.h>
#include <neupro_runtime.h>
#include "app.h"

#define CevaCnnAccT  CevaCnnAcc::ICnnRuntime
//#define NEUPRO_APB_CNN_HWA  (0x60600000) // RTL/FPGA0p5 address
//#define NEUPRO_APB_CNN_HWA  (0x60C04000) // RTL/FPGA0p8 address
#define NEUPRO_APB_CNN_HWA  		(0x18000000) //Toolbox Simulation
#define NEUPRO_AXI_SLAVE_BASE_ADDR 	(0x10000000)

#define CDNN_DEBUG_MAX_OUTPUT_PREDICTIONS 5
#define NUM_OF_INFERENCES 500
#define ACCURACY_THRESH 50.

/* Network Parameters */
cdnnDataBuffersArray inBuffers;
cdnnDataBuffersArray outBuffers;
cdnnNetworkParameters_st cdnnNetworkParameters, *pCDNNNetworkParameters = &cdnnNetworkParameters;

/* Target Parameters */
cdnnBringUpInfo_st cdnnBringUpInfo = { 0 };

cdnnDatabufferParameters_t databufparams0, databufparams1;
float f32MeanAndStdValuesArr[8] = { 127, 127, 127, -1, 127, 127, 127, -1 };

unsigned int u32NetSize = 0;

extern unsigned char shmem;
/* Print memory buffer */
//unsigned char* PrintBuffer = &shmem;
//
//unsigned char PrintEnable = 0;

/* CDNN Qdata binary */
extern unsigned char deploy002060_cdnnQdata[];
/* Array of images */
extern unsigned char* img[];

static int Finish(void *pCDNNHandle, cdnnBringUpInfo_st *pCdnnBringUpInfo, cdnnNetworkParameters_st *pCDNNNetworkParameters);
static int CreateInputBuffers(void *pCDNNHandle, cdnnDataBuffersArray *pInBuffers);
static int CreateOutBuffers(void *pCDNNHandle, cdnnNetwork network, int *tailCnt, cdnnDataBuffersArray *pOutBuffers);
static int PrintResults(char* psPredLabels, unsigned int u32LabelsLen, float *pf32Pred, unsigned int u32PredLen);
static int HwInit(cdnnBringUpInfo_st *pCdnnBringUpInfo);

static int CreateInputBuffers(void *pCDNNHandle, cdnnDataBuffersArray *pInBuffers)
{
	static cdnnDatab Datab1 = NULL;
	static int count = 0;

	databufparams0.eTranspose = E_CDNN_BUFFER_IMAGE_TRANSPOSE_FALSE;
	databufparams0.f32ZeroPoint = 0;
	databufparams0.resizeHeight = 0;
	databufparams0.resizeWidth = 0;
	databufparams0.nInputs = 1;
	databufparams0.nChannels = 3;
	databufparams0.height = 224;
	databufparams0.width = 224;
	databufparams0.dataOrder = E_CDNN_MEMORY_DATAORDER_NHWC;
	databufparams0.scaleFactor = 1;//(256 / 256)
	databufparams0.dataType = E_CDNN_DATATYPE_U8;
	databufparams0.depth = E_CDNN_PRECISION_8BIT;
	databufparams0.padding[0] = databufparams0.padding[1] = databufparams0.padding[2] = databufparams0.padding[3] = 0;
	databufparams0.bufferId = 0;
	databufparams0.bufferType = E_CDNN_BUFFER_TYPE_IMAGE;
	databufparams0.pBufferName = NULL;
	databufparams0.eChannelSwap = E_CDNN_BUFFER_IMAGE_CHANNEL_SWAP_FALSE;
	databufparams0.eResize = E_CDNN_BUFFER_IMAGE_RESIZE_FALSE;

	if(count == 0)
	{
		pInBuffers->pArray[0] = CDNNCreateDataBufferFromHandle(pCDNNHandle, &databufparams0, (void*)img[count]);
	}
	else
	{
		CDNNReleaseDataBuffer(pCDNNHandle, &(pInBuffers->pArray[0]));
		pInBuffers->pArray[0] = CDNNCreateDataBufferFromHandle(pCDNNHandle, &databufparams0, (void*)img[count]);
	}

	/* max image array index */
	if(count < 7)
	{
		count++;
	}
	else
	{
		count = 1;
	}

	if(Datab1 == 0)
	{
	memset(&databufparams1, 0, sizeof(cdnnDatabufferParameters_t));
	databufparams1.width = 1;
	databufparams1.height = 1;
	databufparams1.dataType = E_CDNN_DATATYPE_F32;
	databufparams1.depth = E_CDNN_PRECISION_32BIT;
	databufparams1.nChannels = E_CDNN_CHANNELS_PER_INPUT;
	databufparams1.dataOrder = E_CDNN_MEMORY_DATAORDER_NCHW;
	databufparams1.nInputs = 2; // 1 is for mean values and another is for std values
	databufparams1.pBufferName = NULL;
	databufparams1.bufferId = 1;
	databufparams1.bufferType = E_CDNN_BUFFER_TYPE_DATA;
	databufparams1.f32ZeroPoint = 0;
	Datab1 = CDNNCreateDataBufferFromHandle(pCDNNHandle, &databufparams1, (void*)f32MeanAndStdValuesArr);
	}

	pInBuffers->pArray[1] = Datab1;

	return 0;
}

static int CreateOutBuffers(void *pCDNNHandle, cdnnNetwork network, int *tailCnt, cdnnDataBuffersArray *pOutBuffers)
{
	int s32status = 0;
	int tailIdx;
	uchar* pRawData = NULL;

	/* query for the network mode and change the output data order accordingly */
	int s32NetworkMode = 0;
	s32status |= CDNNNetworkGetAttribute(network, E_CDNN_ATTRIBUTE_NETWORK_NETWORK_MODE, &s32NetworkMode, sizeof(s32NetworkMode));

	/* retrieve number of network tails (network outputs) */
	s32status |= CDNNNetworkGetAttribute(network, E_CDNN_ATTRIBUTE_NETWORK_TAILS_COUNT, tailCnt, sizeof(tailCnt));

	/* create a buffer-array for all tails */
	pOutBuffers->arrSize = *tailCnt;
	pOutBuffers->pArray = (cdnnDatab*)malloc(sizeof(cdnnDatab)*pOutBuffers->arrSize);
	assert(pOutBuffers->pArray != NULL);

	if (s32NetworkMode == E_CDNN_NETWORK_MODE_FULLYCONVOLUTIONAL)
		assert(*tailCnt == 1);

	/* retrieve network tails */
	for (tailIdx = 0; tailIdx < *tailCnt; tailIdx++)
	{
		char psLayerName[E_CDNN_NETWORK_MAX_LAYER_NAME_SIZE];
		cdnnDatabufferParameters_t newParams, *pNewParams = &newParams;
		pNewParams->pBufferName = psLayerName;

		s32status |= CDNNNetworkGetTailBuffParamsByIndex(network, tailIdx, pNewParams);
		if (s32status != 0)
			return s32status;

		/* make all outputs float(even in fixed-point run) */
		pNewParams->dataOrder = (s32NetworkMode == E_CDNN_NETWORK_MODE_FULLYCONVOLUTIONAL) ? E_CDNN_MEMORY_DATAORDER_CNHW : E_CDNN_MEMORY_DATAORDER_NCHW;
		pNewParams->dataType = E_CDNN_DATATYPE_F32;
		/* for float output scale factor must be 0 */
		pNewParams->scaleFactor = 0;
		pNewParams->eChannelSwap = (E_CDNN_BUFFER_IMAGE_CHANNEL_SWAP)-1;
		/* for float depth must be set to 4 bytes (32bits) */
		pNewParams->depth = E_CDNN_PRECISION_32BIT;
		pNewParams->f32ZeroPoint = 0;

		pRawData = (uchar*)malloc(pNewParams->nInputs * pNewParams->nChannels * pNewParams->height * pNewParams->width * (pNewParams->depth >> 3));

		if (!pRawData)
			return -1;
		pOutBuffers->pArray[tailIdx] = CDNNCreateDataBufferFromHandle(pCDNNHandle, pNewParams, pRawData);
	}

	s32status |= CDNNNetworkSetParameter(network, (cdnnReference)pOutBuffers, E_CDNN_NETWORK_STRUCT_OUTPUT_BUFFERS);

	return s32status;
}

static int PrintResults(char* psPredLabels, unsigned int u32LabelsLen, float *pf32Pred, unsigned int u32PredLen)
{
	unsigned int index, u32minIdx;
	float f32minVal = (float)+3.4E+38, f32MaxPred = (float)-3.4E+38;
	unsigned int u32predNumber = MIN(u32PredLen, CDNN_DEBUG_MAX_OUTPUT_PREDICTIONS);

	int label[CDNN_DEBUG_MAX_OUTPUT_PREDICTIONS];
	float prob[CDNN_DEBUG_MAX_OUTPUT_PREDICTIONS];

	for (index = 0; index < CDNN_DEBUG_MAX_OUTPUT_PREDICTIONS; index++)
		prob[index] = (float)-3.4E+38;

	for (index = 0; index < u32PredLen; index++)
	{
		const float predictionValue = pf32Pred[index];
		if (predictionValue > f32MaxPred)
		{
			unsigned int j;
			f32minVal = (float)+3.4E+38;
			u32minIdx = 0;
			for (j = 0; j < u32predNumber; j++)
			{
				if (f32minVal > prob[j])
				{
					f32minVal = prob[j];
					u32minIdx = j;
				}
			}
			prob[u32minIdx] = predictionValue;
			label[u32minIdx] = index;
			f32MaxPred = f32minVal;
		}
	}

	for (index = 0; index < u32predNumber; index += 1)
	{
		if (prob[index] * 100 > ACCURACY_THRESH){
			CEVAPRINTF("%s", &psPredLabels[label[index] * u32LabelsLen]);
			CEVAPRINTF(" : %f ", prob[index] * 100);
		}
	}
	CEVAPRINTF("\n");

	return 0;
}

int GetResults(cdnnDataBuffersArray *pOutBuffers, cdnnNetwork network, int tailCnt)
{
	int s32status = 0;
	unsigned int u32LabelsNamelength = 0, channels = 0, width = 0, height = 0, inputs = 0;
	int elementSize = 0, elementCount = 0;
	float* pf32Data = NULL;
	char* psPredictionLabels = NULL;
	int tailIdx;

	for (tailIdx = 0; tailIdx < tailCnt; ++tailIdx)
	{
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_ELEMENT_SIZE_IN_BYTES, &elementSize, sizeof(elementSize));
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_ELEMENT_COUNT, &elementCount, sizeof(elementCount));
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_CHANNELS, &channels, sizeof(channels));
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_WIDTH, &width, sizeof(width));
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_HEIGHT, &height, sizeof(height));
		s32status |= CDNNQueryDataBuffer(pOutBuffers->pArray[tailIdx], E_CDNN_BUFFER_ATTRIBUTE_INPUTS, &inputs, sizeof(inputs));
		s32status |= CDNNAccessDataBuffer(pOutBuffers->pArray[tailIdx], (void**)&pf32Data);
		s32status |= CDNNNetworkGetAttribute(network, E_CDNN_ATTRIBUTE_NETWORK_CLASSES_NAME_SIZE, &u32LabelsNamelength, sizeof(u32LabelsNamelength));

		/* print results */
		if (!s32status)
		{
			if (u32LabelsNamelength)
			{
				if (width == 1 && height == 1 && inputs == 1)
				{
					s32status |= CDNNAccessNetworkClassesLabels(network, &psPredictionLabels);
					s32status |= PrintResults(psPredictionLabels, u32LabelsNamelength, pf32Data, elementCount);
				}
				else if (tailCnt == 1)
				{
					CEVAPRINTF("Error: Prediction labels are not displayed.\nThe network output buffer dimensions are expected to be [1,label_count,1,1] but its: [%d,%d,%d,%d]\n", inputs, channels, height, width);
				}
			}
		}
	}

	return s32status;
}

int FreeInputBuffers(void *pCDNNHandle, cdnnDataBuffersArray *pInBuffers, int numberOfBuffers)
{
	int s32status = 0, s32BufferIdx;

	for (s32BufferIdx = 0; s32BufferIdx < numberOfBuffers; s32BufferIdx++)
	{
		s32status |= CDNNReleaseDataBuffer(pCDNNHandle, &(pInBuffers->pArray[s32BufferIdx]));
	}

	return s32status;
}

int SetupCDNN(void **pCDNNHandle, cdnnBringUpInfo_st *pCdnnBringUpInfo, cdnnNetworkParameters_st *pCDNNNetworkParameters, void **pNetworkRawData)
{
	int s32verId;
	int s32Status = 0;
	int bInputDummies = TRUE;
	cdnnNetworkParams_st networkParam = { 0 };

	s32Status |= CDNNCreate(pCDNNHandle, pCdnnBringUpInfo);
	if (s32Status != 0)
	{
		return s32Status;
	}

	/* Printing CDNN version*/
	s32Status |= CDNNGetAttribute(*pCDNNHandle, E_CDNN_ATTRIBUTE_VERSION_ID, &s32verId, sizeof(int));
	if (s32Status != 0)
	{
		CEVAPRINTF("\nNo CDNN Version number \t\n");
		return s32Status;
	}
	else
	{
		int s32Year, s32Major, s32Minor;

		s32Minor = s32verId % 10;
		s32verId /= 10;
		s32Major = s32verId % 10;
		s32verId /= 10;
		s32Year = s32verId;
		CEVAPRINTF("\nCDNN Version %d.%d.%d\nCopyright (c) 2020 CEVA DSP Inc \n", s32Year, s32Major, s32Minor);
	}

//	E_CDNN_PROFILER_LEVEL u32Level = E_CDNN_PROFILER_LEVEL_SHORT;
//	s32Status |= CDNNSetAttribute(*pCDNNHandle, E_CDNN_ATTRIBUTE_PROFILER_ENABLE, &u32Level, sizeof(E_CDNN_PROFILER_LEVEL));

	//1 network with 1 input, but also this input needs to do mean subtraction, then the number of input will be 1, number of input buffer will be 2
	//1 network with 2 input, but also only 1 input needs to do mean subtraction, then the number of input will be 2, number of input buffer will be 3 (2 input buffers contain image values and 1 input contain mean&std values.)
	int s32NumInputBuffers = 2;

	/* allocate memory for the input buffers array according to the inputs number of the network */
	pCDNNNetworkParameters->pInBuffers->arrSize = s32NumInputBuffers;
	pCDNNNetworkParameters->pInBuffers->pArray = (cdnnDatab*)malloc(s32NumInputBuffers * sizeof(cdnnDatab));
	if (!pCDNNNetworkParameters->pInBuffers->arrSize)
	{
		CEVAPRINTF("Memory allocation failure\n");
		return s32Status;
	}

	CreateInputBuffers(*pCDNNHandle, pCDNNNetworkParameters->pInBuffers);

	/* initialize network and set network attributes */
	networkParam.pInDataBuffersArray = pCDNNNetworkParameters->pInBuffers;
	networkParam.pNetworkFilePointer = (char*)*pNetworkRawData;
	pCDNNNetworkParameters->network = CDNNCreateNetwork(*pCDNNHandle, &networkParam);

	if (NULL == pCDNNNetworkParameters->network)
	{
		CEVAPRINTF("Error in network creation\n");
		return -1;
	}

	/* Initialize  CDNN */
	s32Status |= CDNNInitialize(*pCDNNHandle);
	if (s32Status != 0)
	{
		CEVAPRINTF("Failed to Initialize CDNN \n");
		s32Status |= FreeInputBuffers(pCDNNHandle, pCDNNNetworkParameters->pInBuffers, s32NumInputBuffers);
		return s32Status;
	}

	/* Set-up output buffers */
	s32Status |= CreateOutBuffers(*pCDNNHandle, pCDNNNetworkParameters->network, &(pCDNNNetworkParameters->tailCnt), pCDNNNetworkParameters->pOutBuffers);
	if (s32Status != 0)
	{
		CEVAPRINTF("Failed to create output buffers \n");
		return s32Status;
	}

	return s32Status;
}

int RunInference(void *pCDNNHandle, cdnnNetworkParameters_st *pCDNNNetworkParameters)
{
	int	s32Status = 0;

	for(int i = 0; i < NUM_OF_INFERENCES; i++)
	{
		CEVAPRINTF("\nInference Run %d \n", i);

		if(i != 0)
		{
			s32Status |= CreateInputBuffers(pCDNNHandle, pCDNNNetworkParameters->pInBuffers);
			s32Status |= CDNNNetworkUpdateParameter(pCDNNNetworkParameters->network, (cdnnReference)pCDNNNetworkParameters->pInBuffers, E_CDNN_NETWORK_STRUCT_INPUT_BUFFERS);
		}

		/* Execute inference */
		s32Status |= CDNNNetworkInference(pCDNNHandle, pCDNNNetworkParameters->network);
		if (s32Status != 0)
		{
			return s32Status;
		}

		CEVAPRINTF("\nResult %d \n", i);

		/* Output the inference results */
		s32Status |= GetResults(pCDNNNetworkParameters->pOutBuffers, pCDNNNetworkParameters->network, pCDNNNetworkParameters->tailCnt);
		if (s32Status != 0)
		{
			CEVAPRINTF("Failed to analyze output : %d\n", s32Status);
			return s32Status;
		}

//		char *pString = NULL, *cycles;
//		s32Status |= CDNNAccessNetworkProfilerResults(pCDNNNetworkParameters->network, (char**)&pString);
//
//		cycles = strstr(pString, "Total optimized cycles");
//		cycles = strstr(cycles, "\t");
//		CEVAPRINTF("\nProf cycles: %s\n", cycles);
	}

	return s32Status;
}

static int HwInit(cdnnBringUpInfo_st *pCdnnBringUpInfo)
{
	int s32Status = 0;

	/************************ Fill CDNN bringupinfo *********************************/
	// Allocate external memory
	pCdnnBringUpInfo->eDDRAlignVal = E_CDNN_MEMORY_ALIGNMENT_NONE;
	pCdnnBringUpInfo->eL2MemAlignVal = E_CDNN_MEMORY_ALIGNMENT_NONE;

	// Platform parameters
	pCdnnBringUpInfo->u32AXISizeInBits = DEFAULT_AXI_SIZE_IN_BITS;
	pCdnnBringUpInfo->u32FrequencyInMHz = DEFAULT_FREQUENCY_IN_MHZ;

	// Memory parameters
	pCdnnBringUpInfo->u16DDRFirstAccessLatency = DEFAULT_DDR_FIRST_ACCESS_LATNECY_IN_CYCLES;
	pCdnnBringUpInfo->u16DDRMiddleAccessLatency = DEFAULT_DDR_MIDDLE_ACCESS_LATNECY_IN_CYCLES;
	pCdnnBringUpInfo->u16L2FirstAccessLatency = DEFAULT_SRAM_FIRST_ACCESS_LATNECY_IN_CYCLES;
	pCdnnBringUpInfo->u16L2MiddleAccessLatency = DEFAULT_SRAM_MIDDLE_ACCESS_LATNECY_IN_CYCLES;

	// CDNN memory follows network memory in external memory
	pCdnnBringUpInfo->u32DDRSize = ((unsigned int)&gu8MemoryEndExternalAllocationBuffer) - ((unsigned int)&gu8MemoryStartExternalAllocationBuffer) - u32NetSize;
	pCdnnBringUpInfo->pu8DDRAddress = &gu8MemoryStartExternalAllocationBuffer + u32NetSize;
	pCdnnBringUpInfo->u32L2MemorySize = ((unsigned int)&gu8MemoryEndL2AllocationBuffer) - ((unsigned int)&gu8MemoryStartL2AllocationBuffer);
	pCdnnBringUpInfo->pu8L2MemoryAddress = pCdnnBringUpInfo->u32L2MemorySize ? &gu8MemoryStartL2AllocationBuffer : NULL;

	uint u32BaseDeviceId = 0;
	cdnnTargetInfo_st *pCdnnTargetInfo = NULL;

	/************************ Create The Target *********************************/
	pCdnnBringUpInfo->u32NumberOfTargets = 1;
	pCdnnBringUpInfo->u32MasterTargetId = 0;

	pCdnnTargetInfo = (cdnnTargetInfo_st *)malloc(sizeof(cdnnTargetInfo_st)*pCdnnBringUpInfo->u32NumberOfTargets);
	if (NULL == pCdnnTargetInfo)
	{
		CEVAPRINTF(" Failed to allocate memory for the CDNN target \n");
		return -1;
	}
	pCdnnBringUpInfo->pTargetsInformation = pCdnnTargetInfo;

	/************************ Create The Devices *********************************/
	// Set some  system configurations
	u32BaseDeviceId = 0; // CDNN expects u32BaseDeviceId to be 0
	pCdnnTargetInfo->u32BaseDeviceId = u32BaseDeviceId;

	// Right now the max number of u32NumOfDevices is 2 (DSP device & optional HWA)
    pCdnnTargetInfo->u32NumOfDevices = 2;

	// Set devices configuration
	unsigned int u32AllocSize = sizeof(cdnnDevicesInfo_u) * pCdnnTargetInfo->u32NumOfDevices;
	pCdnnTargetInfo->pDevicesInformation = (void*)malloc(u32AllocSize);

	if (NULL == pCdnnTargetInfo->pDevicesInformation)
	{
		free(pCdnnBringUpInfo->pTargetsInformation);
		CEVAPRINTF("Failed to create the device information structure \n");
		return -1;
	}
	memset(pCdnnTargetInfo->pDevicesInformation, 0, u32AllocSize);

	uchar *pcInternalMemAdd = NULL;
	cdnnDeviceInfo_st *pDevicesInfo = (cdnnDeviceInfo_st *)pCdnnTargetInfo->pDevicesInformation;

	cdnnDeviceInfo_st *pDeviceInfo = NULL;
	unsigned int u32InternalMemorySize = 0;
	unsigned char *pu8InternalDTCMAddress = NULL;

	/*This part is to initialize the cache of the DSP */
	mss_enable_global_program_cache();

	/* Init the DMA manager. It is the user responsibility to init the DMA manager of the DSP */
	s32Status = dma_init_manager();

	u32InternalMemorySize = ((unsigned int)&gu8MemoryEndInternalAllocationBuffer) - ((unsigned int)&gu8MemoryStartInternalAllocationBuffer);
	pu8InternalDTCMAddress = &gu8MemoryStartInternalAllocationBuffer;

	pDeviceInfo = &pDevicesInfo[u32BaseDeviceId];
	pDeviceInfo->eDeviceType = E_CDNN_DEVICE_XM6;
	pDeviceInfo->pu8InternalDTCMAddress = pu8InternalDTCMAddress;
	pDeviceInfo->u32InternalMemorySize = u32InternalMemorySize;
	pDeviceInfo->pDeviceHandle = NULL;
	pDeviceInfo->eRunningMode = E_CDNN_RUNNING_MODE_ALL;
	pDeviceInfo->u8NumberOfInternalMemoryBlocks = E_XM_NUMBER_OF_INTERNAL_MEMORY_BLOCKS;

	 // Code below assumes DSP device information comes before HWA device information
	int s32HWADeviceIdx = 1;
	pDeviceInfo = &pDevicesInfo[s32HWADeviceIdx];
	pDeviceInfo->eDeviceType = E_CDNN_DEVICE_NEUPROS;
	pDeviceInfo->eRunningMode = E_CDNN_RUNNING_MODE_ALL;
	CevaCnnAccT *pCnnDevice;
	pDeviceInfo->u8NumberOfInternalMemoryBlocks = E_CANT_USE_INTERNAL_MEMORY;

	std::map<std::string, std::string> config;
	E_CDNN_DEVICE_WEIGHTS_COMPRESSION_MODE eWeightsCompressionMode = E_CDNN_WEIGHTS_COMPRESSION_MODE_NONE;

	// DACU configuration for accessing NeuPro
	mss_data_memory_region_t mem_region;
	memset(&mem_region, 0, sizeof(mss_data_memory_region_t));
	mem_region.memory_region = MSS_MEMORY_REGION_1;
	mem_region.start_address = NEUPRO_AXI_SLAVE_BASE_ADDR;
	mem_region.active = MSS_MEMORY_REGION_ACTIVE;
	mem_region.master_id = MSS_DATA_MEM_REGION_MASTER_AXIM0;
	mem_region.ddma_max_burst_size = MSS_DDMA_MAX_BURST_SIZE_8_INCR;
	mss_set_data_memory_region(&mem_region);

	// DACU configuration for accessing the rest of the external memory
	mem_region.start_address = NEUPRO_AXI_SLAVE_BASE_ADDR + 0x800000;
	mem_region.memory_region = MSS_MEMORY_REGION_2;
	mem_region.master_id = MSS_DATA_MEM_REGION_MASTER_EDP;
	mem_region.ddma_max_burst_size = MSS_DDMA_MAX_BURST_SIZE_16_INCR;
	mem_region.ddma_max_outstanding_read = MSS_DDMA_REGION_MAX_OUTSTANDING_READ_16;
	mem_region.ddma_max_outstanding_write = MSS_DDMA_REGION_MAX_OUTSTANDING_WRITE_32;
	mss_set_data_memory_region(&mem_region);

	pCnnDevice = CevaPlatform::DspPlatformManager::instance().addDevice<NeuPro::CNNHARuntime>(std::string("Runtime"), NEUPRO_APB_CNN_HWA, NEUPRO_AXI_SLAVE_BASE_ADDR);
	CevaCnnAccT::DeviceStatus eDeviceStatus;

	eDeviceStatus = pCnnDevice->initDevice();

	if (CevaCnnAccT::OK != eDeviceStatus)
	{
		CevaPlatform::DspPlatformManager::instance().deleteDevice("Runtime");
		CEVAPRINTF("NeuPro driver failed to initialize !\n");
		return -1;
	}
	((cdnnNeuProSInfo_st *)pDeviceInfo)->eWeightsCompressionMode = eWeightsCompressionMode;
	pDeviceInfo->pDeviceHandle = *(cdnnDeviceHandle*)&pCnnDevice;

	return s32Status;
}

int AppProcess(void)
{
	int s32Status = 0;
	void *pCDNNHandle = NULL;
	void *pNetworkRawData = NULL;

	inBuffers.pArray = NULL;
	inBuffers.arrSize = 0;
	outBuffers.pArray = NULL;
	outBuffers.arrSize = 0;

	memset(pCDNNNetworkParameters, 0, sizeof(cdnnNetworkParameters_st));
	pCDNNNetworkParameters->pInBuffers = &inBuffers;
	pCDNNNetworkParameters->pOutBuffers = &outBuffers;

	/* Initialize hardware, returns cdnnBringUpInfo which is used to create the CDNN */
	s32Status |= HwInit(&cdnnBringUpInfo);
	if (s32Status != 0)
	{
		CEVAPRINTF("\n Failed to initialize hardware (status: %d)\n", s32Status);
		return s32Status;
	}

	/* Load cdnnQdata file into memory */
//	s32Status = LoadNetwork(&pNetworkRawData);
    pNetworkRawData = &deploy002060_cdnnQdata[0];

//	if (s32Status != 0)
//	{
//		CEVAPRINTF("\n Load Network failed (status: %d)\n", s32Status);
//		return s32Status;
//	}

	/*Setup CDNN for inference runs */
	s32Status |= SetupCDNN(&pCDNNHandle, &cdnnBringUpInfo, pCDNNNetworkParameters, &pNetworkRawData);

	if (s32Status != 0)
	{
		CEVAPRINTF("\n Failed to setup CDNN (status: %d)\n", s32Status);
		return s32Status;
	}

	//PrintEnable = 1;

	/* Execute inference over inputs */
	s32Status |= RunInference(pCDNNHandle, pCDNNNetworkParameters);

	if (s32Status != 0)
	{
		CEVAPRINTF("\n Inference run failed (status: %d)\n", s32Status);
		return s32Status;
	}

	s32Status |= Finish(pCDNNHandle, &cdnnBringUpInfo, pCDNNNetworkParameters);

	if (s32Status != 0)
	{
		CEVAPRINTF("\n De-init failed (status: %d)\n", s32Status);
		return s32Status;
	}

	return s32Status;
}

int Finish(void *pCDNNHandle, cdnnBringUpInfo_st *pCdnnBringUpInfo, cdnnNetworkParameters_st *pCDNNNetworkParameters)
{
	int s32Status = 0;
	int tailIdx;

	/* Free allocated buffers */
	s32Status |= FreeInputBuffers(pCDNNHandle, pCDNNNetworkParameters->pInBuffers, pCDNNNetworkParameters->pInBuffers->arrSize);

	/*free all tail data buffers and malloc-ed memory */
	for (tailIdx = 0; tailIdx < pCDNNNetworkParameters->tailCnt; tailIdx++)
	{
		void* pData = NULL;
		s32Status |= CDNNAccessDataBuffer(pCDNNNetworkParameters->pOutBuffers->pArray[tailIdx], &pData);
		if (NULL != pData)
			free(pData);
		s32Status |= CDNNReleaseDataBuffer(pCDNNHandle, &(pCDNNNetworkParameters->pOutBuffers->pArray[tailIdx]));
	}

	/* release in/out arrays */
	if (NULL != pCDNNNetworkParameters->pInBuffers->pArray)
		free(pCDNNNetworkParameters->pInBuffers->pArray);
	if (NULL != pCDNNNetworkParameters->pOutBuffers->pArray)
		free(pCDNNNetworkParameters->pOutBuffers->pArray);

	/* release all objects */
	s32Status |= CDNNReleaseNetwork(pCDNNHandle, &(pCDNNNetworkParameters->network));
	s32Status |= CDNNRelease(pCDNNHandle);
	s32Status |= CevaHardwareDeinit(pCdnnBringUpInfo);

	return s32Status;
}
