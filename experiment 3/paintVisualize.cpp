#include "paintVsualize.h"
#include "paint_silhouette.h"
#include "paint_point.h"

void paint_2D_points(const double* featureData, const int* labelArray, const char* title_name) {
	int pcaFeatureNum = 2;
	mxDouble mxfeatureData[400 * 2];
	mxDouble mxlabelArray[400];
	for (int i = 0; i < 400; i++) {//
		for (int k = 0; k < pcaFeatureNum; k++) {
			mxfeatureData[i * pcaFeatureNum + k] = featureData[i * pcaFeatureNum + k];
		}
		mxlabelArray[i] = labelArray[i] + 1;
	}
	if (!paint_pointInitialize())
		return ;
	mwArray featureDataIn(1, 400 * 2, mxDOUBLE_CLASS), labelArrayIn(1, 400, mxDOUBLE_CLASS), title_nameIn(title_name);
	featureDataIn.SetData(mxfeatureData, 400 * 2);
	labelArrayIn.SetData(mxlabelArray, 400);

	paint_point(featureDataIn, labelArrayIn, title_nameIn);
	mclWaitForFiguresToDie(NULL);

	paint_pointTerminate();
	delete[]mxfeatureData;
	delete[]mxlabelArray;

	return;
}

// featureData, clusters, centroids, title_name
// featureData
void paint_2D_silhouette(const double* featureData, const int* labelArray, const double* class_M, const char* title_name) {
	int pcaFeatureNum = 2;
	mxDouble* mxfeatureData = new mxDouble[400 * 2];
	mxDouble* mxlabelArray = new mxDouble[400];
	mxDouble* mxclassM = new mxDouble[20 * 2];
	for (int i = 0; i < 400; i++) {//
		for (int k = 0; k < pcaFeatureNum; k++) {
			mxfeatureData[i * pcaFeatureNum + k] = featureData[i * pcaFeatureNum + k];
		}
		mxlabelArray[i] = labelArray[i] + 1;
	}
	for (int i = 0; i < 20; i++) {
		mxclassM[i * 2] = class_M[i * 2];
		mxclassM[i * 2 + 1] = class_M[i * 2 + 1];
	}

	if (!paint_silhouetteInitialize())
		return;
	mwArray featureDataIn(1, 400 * 2, mxDOUBLE_CLASS), labelArrayIn(1, 400, mxDOUBLE_CLASS), centroidsIn(1, 20 * 2, mxDOUBLE_CLASS), title_nameIn(title_name);
	featureDataIn.SetData(mxfeatureData, 400 * 2);
	labelArrayIn.SetData(mxlabelArray, 400);
	centroidsIn.SetData(mxclassM, 20 * 2);

	// 结果
	mxDouble avg_silhouette_score, calinski_harabasz_score, dunnIndex;
	mwArray out_avg_silhouette_score, out_calinski_harabasz_score, outDunnIndex;

	paint_silhouette(3, out_avg_silhouette_score, out_calinski_harabasz_score, outDunnIndex, featureDataIn, labelArrayIn, centroidsIn, title_nameIn);

	out_avg_silhouette_score.GetData(&avg_silhouette_score, 1);
	out_calinski_harabasz_score.GetData(&calinski_harabasz_score, 1);
	outDunnIndex.GetData(&dunnIndex, 1);

	std::cout << "轮廓系数:" << avg_silhouette_score << std::endl << "calinski_harabasz_score:" << calinski_harabasz_score << std::endl << "Dunn 指数:" << dunnIndex << std::endl;
	mclWaitForFiguresToDie(NULL);

	paint_silhouetteTerminate();

	delete[]mxfeatureData;
	delete[]mxlabelArray;
	delete[]mxclassM;
}