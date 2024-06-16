#include "dbscan.h"
#include "dbscan_algorithm.h"


void dbscan(int* labelArray, double* featureData, double epsilon, int minPts) 
{
	int pcaFeatureNum = 2;
	mxDouble* mxfeatureData = new mxDouble[400 * 2];
	mxDouble* mxlabelArray = new mxDouble[400];
	for (int i = 0; i < 400; i++) {//
		for (int k = 0; k < pcaFeatureNum; k++) {
			mxfeatureData[i * pcaFeatureNum + k] = featureData[i * pcaFeatureNum + k];
		}
	}
	if (!dbscan_algorithmInitialize()) {
		return;
	}
	mwArray featureDataIn(1, 400 * 2, mxDOUBLE_CLASS), labelArrayIn(1, 400, mxDOUBLE_CLASS), epsilonIn(epsilon), minPtsIn(minPts);
	featureDataIn.SetData(mxfeatureData, 400 * 2);

	dbscan_algorithm(1, labelArrayIn, featureDataIn, epsilonIn, minPtsIn);
	labelArrayIn.GetData(mxlabelArray, 400);
	for (int i = 0; i < 400; i++) {
		labelArray[i] = mxlabelArray[i] - 1;
	}
	mclWaitForFiguresToDie(NULL);

	// dbscan_algorithmTerminate();
}