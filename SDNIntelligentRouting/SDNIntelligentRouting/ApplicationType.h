#define QoSTypeNum 6
typedef struct
{
	double BandWidth;
	double Delay;
	double ErrorRate;
}LinkQoS;

double ClassofQoS[QoSTypeNum][3]={ 1.5, 100,  0.01,
                                   1.0, 400,  0.01,
								   0.5, 300,  0.01,
								   0.3, 400,  0.01,
								   0.2, 1000, 0.01,
								   0.1, 1000, 0.01};


//随机生成应用
//0.55 0.2 0.1 0.05  0.075 0.025
int GetAppType()
{
	int type;
	double r;
	r=rand()/(RAND_MAX+1.0);
	if(r>=0&&r<0.55)
		type=0;
	else if (r<0.75)
		type=1;
	else if (r<0.85)
		type=2;
	else if (r<0.9)
		type=3;
	else if (r<0.975)
		type=4;
	else 
		type=5;

	return type;
}

//应用类型对应的QoS参数
void AppTypeMapQoS(int AppType,LinkQoS &QoSParameter)
{
	QoSParameter.BandWidth=ClassofQoS[AppType][0];
	QoSParameter.Delay=ClassofQoS[AppType][1];
	QoSParameter.ErrorRate=ClassofQoS[AppType][2];
}
