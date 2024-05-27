#include <iostream>
#include <stdio.h>
#include "featureFiles.h"

using namespace std;

FeatureFiles* FeatureFiles::readFeatureFiles(std::vector<std::string> fileNames, int* featureNums, int picNum, int classNum)
{
	FeatureFiles *Ff = new FeatureFiles;

	Ff->fileNum = fileNames.size();
	Ff->fileNames = new char* [Ff->fileNum];
	Ff->featureNums = new int [Ff->fileNum];
	Ff->featureNum = 0;

	for (int fileIndex = 0 ; fileIndex < Ff->fileNum; fileIndex++) {
		Ff->fileNames[fileIndex] = new char [strlen(fileNames.at(fileIndex).c_str()) + 1];
		strcpy_s(Ff->fileNames[fileIndex], strlen(fileNames.at(fileIndex).c_str()) + 1, fileNames.at(fileIndex).c_str());
		Ff->featureNums[fileIndex] = featureNums[fileIndex];
		Ff->featureNum += featureNums[fileIndex];
	}

	Ff->classNum = classNum;
	Ff->picNum = picNum;
	Ff->sampleNum = classNum * picNum;

	Ff->readFiles();

	return Ff;
}

void FeatureFiles::readFiles()
{
	FILE* fp;
	this->featureData = new double [featureNum * sampleNum];
	int currentFeatureNum = 0;
	Label = new int[picNum * classNum];
	errno_t err;
	for (int fileIndex = 0; fileIndex < fileNum; fileIndex++) {
		err = fopen_s(&fp, fileNames[fileIndex], "r");
		if (err != 0) {
			cout << fileNames[fileIndex] << "打开失败";
		}
		else {
			for (int sampleIndex = 0; sampleIndex < sampleNum; sampleIndex++) {
				for (int featureIndex = 0; featureIndex < featureNums[fileIndex]; featureIndex++) {
					fscanf_s(fp, "%lf", &featureData[sampleIndex * featureNum + currentFeatureNum + featureIndex]);
				}
				fscanf_s(fp, "%d", &Label[sampleIndex]);
			}
			currentFeatureNum += featureNums[fileIndex];
			fclose(fp);
		}
	}
}

void FeatureFiles::outPutFile(char* outPutFileName)
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, outPutFileName, "w");
	if (err != 0) {
		cout << outPutFileName << "打开失败";
	}
	else {
		for (int classIndex = 0; classIndex < classNum; classIndex++) {
			for (int picIndex = 0; picIndex < picNum; picIndex++) {
				for (int featureIndex = 0; featureIndex < featureNum; featureIndex++) {
					fprintf(fp, "%lf ", featureData[((classIndex * picNum) + picIndex) * featureNum + featureIndex]);
				}
				fprintf(fp, "%d\n", classIndex + 1);
			}
		}
		fclose(fp);
	}
}

