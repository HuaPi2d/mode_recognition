
#define _NORMAL_BAYES_CLASSIFIER_H
#include<iostream>
#include <cassert>
#include <vector>
#include <iterator>
#include <cmath>
#include <functional>
#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <iterator>
#include <cmath>
#include <functional>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace std;
//Bayes∏≈¬ ‘§≤‚
void bayesClassify(double traindata[], double testdata[], int classNum, int trainNum, int testNum, int featureNum, double* P);