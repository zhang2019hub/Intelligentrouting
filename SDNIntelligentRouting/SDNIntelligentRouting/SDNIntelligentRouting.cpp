// SDNIntelligentRouting.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include"stdio.h"
#include"stdlib.h"
#include"time.h"
#include"math.h"
#include"string.h"
#include"Topology.h"
#include"DataFile.h"
#include"ApplicationType.h"
#include"SampleSet.h"
#include"BPNN.h"
#include"K_means.h"
#include"APC_III.h"
#include"APC_K_means.h"
#include"RBFNN.h"


int _tmain(int argc, _TCHAR* argv[])
{
	TestGetThreeTuple();
    TestGetSampleSet();
    TestDataProcess();
    TestBPNN();
	Getcenter();
	TestRBFNN();

	return 0;
}

    