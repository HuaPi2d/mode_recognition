#include "hierarchicalClustering_packed.h"
#include "hierarchicalClustering.h"

void mean_distance(double* dataArray, int* labelArray, int C) {
	mxDouble mxfeatureData[400 * 2];
	mxDouble mxlabelArray[400];

	for (int i = 0; i < 400; i++) {
		for (int k = 0; k < 2; k++) {
			mxfeatureData[i * 2 + k] = dataArray[i * 2 + k];
		}
	}

	if (!hierarchicalClusteringInitialize())
		return;
	mwArray featureDataIn(1, 400 * 2, mxDOUBLE_CLASS), clusterNum(C);
	featureDataIn.SetData(mxfeatureData, 400 * 2);
	
	mwArray labelArrayOut;

	hierarchicalClustering(1, labelArrayOut, featureDataIn, clusterNum);

	mclWaitForFiguresToDie(NULL);

	labelArrayOut.GetData(mxlabelArray, 400);

	for (int i = 0; i < 400; i++) {
		labelArray[i] = mxlabelArray[i];
	}

	hierarchicalClusteringTerminate();
}