#include "calculate_intra_inter.h"
#include "calculate_intra_inter_packed.h"
#include <stdio.h>

void justify(const double* featureData, const int* labelArray) 
{
	int pcaFeatureNum = 2;
	mxDouble* mxfeatureData = new mxDouble[400 * 2];
	mxDouble* mxlabelArray = new mxDouble[400];
	for (int i = 0; i < 400; i++) {//
		for (int k = 0; k < pcaFeatureNum; k++) {
			mxfeatureData[i * pcaFeatureNum + k] = featureData[i * pcaFeatureNum + k];
		}
		mxlabelArray[i] = labelArray[i] + 1;
	}
	if (!calculate_intra_interInitialize())
		return;
	mwArray featureDataIn(1, 400 * 2, mxDOUBLE_CLASS), labelArrayIn(1, 400, mxDOUBLE_CLASS);
	featureDataIn.SetData(mxfeatureData, 400 * 2);
	labelArrayIn.SetData(mxlabelArray, 400);

	// 结果
	mxDouble intra, inter;
	mwArray outIntra, outInter;

	calculate_intra_inter(2, outIntra, outInter, featureDataIn, labelArrayIn);
	outIntra.GetData(&intra, 1);
	outInter.GetData(&inter, 1);
	std::cout << "类内离散度:" << intra << std::endl << "类间离散度:" << inter << std::endl;

	calculate_intra_interTerminate();

	delete[]mxfeatureData;
	delete[]mxlabelArray;
}