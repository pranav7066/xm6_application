/* Copyright 2021 OpenFive */

/*
 *  XM6 based CDNN application
 */

#ifndef __APP_H
#define __APP_H

#include "AppUtility.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
* \brief network parameters
*/
typedef struct CDNNNetworkParameters {
	/*! \brief  An opaque reference to a network object */
	cdnnNetwork network;
	/*! \brief  The network's input buffers */
	cdnnDataBuffersArray *pInBuffers;
	/*! \brief  The network's output buffers */
	cdnnDataBuffersArray *pOutBuffers;
	/*! \brief  The number of network outputs */
	int tailCnt;
}cdnnNetworkParameters_st;

/*! \brief The application process, initializes hardware and runs all stages for executing inference.
* \param [in] None
*/
int AppProcess(void);

#ifdef __cplusplus
}
#endif

#endif
