#include"float.h"
#define NODENUM 20//NODENUM为网络节点数
#define I (1<<15)//设I为无限大的数 
#define U 1.0e-10//出错率单位

typedef struct
{
	int vexnum;
	double BandWidth[NODENUM][NODENUM];
	double Delay[NODENUM][NODENUM];
	double ErrorRate[NODENUM][NODENUM];
}Graph;

Graph G;

double LinkBandWidth[NODENUM][NODENUM]=
{
	{0,2.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{2.5,0,2.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,2.5,0,2.5,2.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,2.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,2.5,0,0,2.5,0,2.5,0,0,0,0,10,0,0,0,0,0,0,0},
	{0,0,0,0,2.5,0,2.5,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,2.5,0,0,0,0,0,0,0,2.5,0,0,0,0,0,0},
	{0,0,0,0,2.5,0,0,0,2.5,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,2.5,0,2.5,2.5,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2.5,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2.5,0,0,2.5,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,2.5,0,2.5,0,0,0,0,0,0,0},
	{0,0,0,0,10,0,0,0,0,0,0,2.5,0,0,10,0,2.5,0,0,10},
	{0,0,0,0,0,0,2.5,0,0,0,0,0,0,0,2.5,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,10,2.5,0,10,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,2.5,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,2.5,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2.5,0,0,2.5,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2.5,0,2.5},
	{0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,2.5,0}
};

double LinkDelay[NODENUM][NODENUM]=
{
	{I,5.29,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I},
	{5.29,I,16,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I},
	{I,16,I,38,20,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I},
	{I,I,38,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I},
	{I,I,20,I,I,59,I,74,I,I,I,I,29,I,I,I,I,I,I,I},
	{I,I,I,I,59,I,6.5,I,I,I,I,I,I,I,I,I,I,I,I,I},
	{I,I,I,I,I,6.5,I,I,I,I,I,I,I,26,I,I,I,I,I,I},
	{I,I,I,I,74,I,I,I,51,I,I,I,I,I,I,I,I,I,I,I},
	{I,I,I,I,I,I,I,51,I,16,40,I,I,I,I,I,I,I,I,I},
	{I,I,I,I,I,I,I,I,16,I,I,I,I,I,I,I,I,I,I,I},
	{I,I,I,I,I,I,I,I,40,I,I,8.6,I,I,I,I,I,I,I,I},
	{I,I,I,I,I,I,I,I,I,I,8.6,I,77,I,I,I,I,I,I,I},
	{I,I,I,I,29,I,I,I,I,I,I,77,I,I,29,I,31,I,I,38},
	{I,I,I,I,I,I,26,I,I,I,I,I,I,I,110,I,I,I,I,I},
	{I,I,I,I,I,I,I,I,I,I,I,I,29,110,I,34,I,I,I,I},
	{I,I,I,I,I,I,I,I,I,I,I,I,I,I,34,I,I,5,I,I},
	{I,I,I,I,I,I,I,I,I,I,I,I,31,I,I,I,I,I,I,I},
	{I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,5,I,I,37,I},
	{I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,37,I,26},
	{I,I,I,I,I,I,I,I,I,I,I,I,38,I,I,I,I,I,26,I}
};

double LinkErrorRate[NODENUM][NODENUM]=
{
	{I,2.5*U,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I},
	{2.5*U,I,3*U,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I},
	{I,3*U,I,4*U,7*U,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I},
	{I,I,4*U,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I},
	{I,I,7*U,I,I,1.2*U,I,7*U,I,I,I,I,12*U,I,I,I,I,I,I,I},
	{I,I,I,I,1.2*U,I,3.5*U,I,I,I,I,I,I,I,I,I,I,I,I,I},
	{I,I,I,I,I,3.5*U,I,I,I,I,I,I,I,2.5*U,I,I,I,I,I,I},
	{I,I,I,I,7*U,I,I,I,5*U,I,I,I,I,I,I,I,I,I,I,I},
	{I,I,I,I,I,I,I,5*U,I,6.7*U,8.5*U,I,I,I,I,I,I,I,I,I},
	{I,I,I,I,I,I,I,I,6.7*U,I,I,I,I,I,I,I,I,I,I,I},
	{I,I,I,I,I,I,I,I,8.5*U,I,I,3*U,I,I,I,I,I,I,I,I},
	{I,I,I,I,I,I,I,I,I,I,3*U,I,12*U,I,I,I,I,I,I,I},
	{I,I,I,I,12*U,I,I,I,I,I,I,12*U,I,I,5.5*U,I,3.6*U,I,I,10.3*U},
	{I,I,I,I,I,I,2.5*U,I,I,I,I,I,I,I,1.5*U,I,I,I,I,I},
	{I,I,I,I,I,I,I,I,I,I,I,I,5.5*U,1.5*U,I,10*U,I,I,I,I},
	{I,I,I,I,I,I,I,I,I,I,I,I,I,I,10*U,I,I,1.8*U,I,I},
	{I,I,I,I,I,I,I,I,I,I,I,I,3.6*U,I,I,I,I,I,I,I},
	{I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,1.8*U,I,I,10*U,I},
	{I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,10*U,I,8.5*U},
	{I,I,I,I,I,I,I,I,I,I,I,I,10.3*U,I,I,I,I,I,8.5*U,I}
};

//初始化
void GraphInitialization(Graph &G)
{
	int i,j;

	G.vexnum =NODENUM;
	for(i=0;i<G.vexnum;i++)
		for(j=0;j<G.vexnum;j++)
			G.BandWidth[i][j]=LinkBandWidth[i][j];
	for(i=0;i<G.vexnum;i++)
		for(j=0;j<G.vexnum;j++)
			G.Delay[i][j]=LinkDelay[i][j];
	for(i=0;i<G.vexnum;i++)
		for(j=0;j<G.vexnum;j++)
			G.ErrorRate[i][j]=LinkErrorRate[i][j];
}

//路径延迟为组成该路径所有链路延迟之和
double GetPathDelay(Graph &G,int Path[],int Length)
{
	int i;
	double Delay=0;

	for(i=0;i<Length-1;i++)
		Delay+=G.Delay[Path[i]][Path[i+1]];

	return Delay;
}

//路径出错率为1-(1-链路[1]出错率)*(1-链路[2]出错率)*……*(1-链路[n]出错率)
double GetPathErrorRate(Graph &G,int Path[],int Length)
{
	int i;
	double ErrorRate=1.0;

	for(i=0;i<Length-1;i++)
		ErrorRate*=(1-G.ErrorRate[Path[i]][Path[i+1]]);
	ErrorRate=1-ErrorRate;

	return ErrorRate;
}