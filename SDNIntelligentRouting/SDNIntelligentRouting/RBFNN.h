double  W_R[OUTPUT_R][HIDDEN_R];
double  B_R[OUTPUT_R];
double  Err_R=1e-6;
double  WRate_R=0.1;
double  BRate_R=0.1;

//高斯函数
double GetGaussian(Tuple R[],Tuple X,int K)
{
	double sum=0;
	sum+=(X.src-R[K].src)*(X.src-R[K].src)+(X.dest-R[K].dest)*(X.dest-R[K].dest)+(X.type-R[K].type)*(X.type-R[K].type);
	sum=(sum*(-1))/(Width[K]);
	sum=exp(sum);

	return sum;
}


void CopyWeightThreshold(double W[OUTPUT_R][HIDDEN_R],double Wcopy[OUTPUT_R][HIDDEN_R],double B[],double Bcopy[],int h)
{
	int i,j;
	for(i=0;i<OUTPUT_R;i++)
		for(j=0;j<h;j++)
			Wcopy[i][j]=W[i][j];
	for(i=0;i<OUTPUT_R;i++)
		Bcopy[i]=B[i];
}

//计算误差
double GetErrorSum(double T[],double Y[])
{
	int i;
	double error=0;
	for(i=0;i<OUTPUT_R;i++)
		error+=(T[i]-Y[i])*(T[i]-Y[i]);

	return error;
}

void RBFInitialization(int h)
{
	int i,j;
	for(i=0;i<OUTPUT_R;i++)
	{
		for(j=0;j<h;j++)
			W_R[i][j]=rand()*1.0/(RAND_MAX+1.0);
		B_R[i]=rand()*1.0/(RAND_MAX+1.0);
	}
}


void RBFTrain(char * TrainData,int h,double e,int TrainSize)
{
	FILE * fp;
	Tuple  X;
	int    i,j,count;
	int   size=0;
	double Error,err[OUTPUT_R],werror[OUTPUT_R], H[HIDDEN_R];
	double T[OUTPUT_R],Y[OUTPUT_R],W_Rcopy[OUTPUT_R][HIDDEN_R],B_Rcopy[OUTPUT_R];
	
	count=0;
	do
	{
		Error=0;
		size=0;
		if((fp=fopen(TrainData,"r"))==NULL)
		{
			printf("%-60s can not be opened!\n",TrainData);
			exit(0);
		}
		while(!feof(fp))
		{
			size++;
			fscanf(fp,"%lf%lf%lf",&X.src,&X.dest,&X.type);
			for(i=0;i<OUTPUT_R;i++)
			{
				fscanf(fp,"%lf",&T[i]);
				Y[i]=0;
			}
			for(i=0;i<h;i++)
				H[i]=GetGaussian(Center,X,i);
			for(i=0;i<OUTPUT_R;i++)
			{
				for(j=0;j<h;j++)
					Y[i]+=H[j]*W_R[i][j];
				Y[i]+=B_R[i];
			}
			for(i=0;i<OUTPUT_R;i++)
				err[i]=Y[i]-T[i];
			CopyWeightThreshold(W_R,W_Rcopy,B_R,B_Rcopy,h);
			for(i=0;i<OUTPUT_R;i++)
				for(j=0;j<h;j++)
					W_R[i][j]=W_Rcopy[i][j]+WRate_R*H[i]*err[i];
			for(i=0;i<OUTPUT_R;i++)
				for(j=0;j<h;j++)
					werror[i]+=(BRate_R*H[i]*err[i])*(BRate_R*H[i]*err[i]);
			 Error+=GetErrorSum(T,Y);
		}
		fclose(fp);
		count++;
		Error/=size;
	}while(Error<e&&count<TrainSize);

}


int IsAcceptableError(double T[OUTPUT_R],double Y[OUTPUT_R])
{
	int i,flag,count;

	flag=0;
	count=0;
	for(i=0;i<OUTPUT_R;i++)
	{
		if(abs( (T[i]-0.1)/0.8*20-(T[i]-0.1)/0.8*20 )<0.0005)
			count++;
	}
	if(count==OUTPUT_R)
		flag=1;

	return flag;
}


double RBFTest(char *TestData,int h,int TestSize)
{
	FILE *fp;
	Tuple X;
	int i,j,count,correctCount;
	double ErrSum,H[HIDDEN_R],T[OUTPUT_R],Y[OUTPUT_R];

	if((fp=fopen(TestData,"r"))==NULL)
	{
		printf("%-60s can not be opened!\n",TestData);
		exit(0);
	}
	count=0;
	correctCount=0;
	ErrSum=0;
	while(!feof(fp)&&count<TestSize)
	{
		fscanf(fp,"%lf%lf%lf",&X.src,&X.dest,&X.type);
		for(i=0;i<OUTPUT_R;i++)
			fscanf(fp,"%lf",&T[i]);
		for(i=0;i<h;i++)
			H[i]=GetGaussian(Center,X,i);
        for(i=0;i<OUTPUT_R;i++)
		{
			Y[i]=0;
			for(j=0;j<h;j++)
				Y[i]+=W_R[i][j]*H[j];
			Y[i]+=B_R[i];
		}
		if(IsAcceptableError(T,Y)==1)
			correctCount++;
	    ErrSum+=GetErrorSum(T,Y);
		count++;
	}
	fclose(fp);

	return (correctCount*1.0)/(count*1.0);
}


void RBFPathPredict(Tuple X,double Y[],int Node[],int h)
{
	int i,j;
	double H[HIDDEN_R];

	for(i=0;i<h;i++)
		H[i]=GetGaussian(Center,X,i);
    for(i=0;i<OUTPUT_R;i++)
	{
		Y[i]=0;
		for(j=0;j<h;j++)
			Y[i]+=W_R[i][j]*H[j];
		Y[i]+=B_R[i];
		Node[i]=(int)((Y[i]-0.1)/(0.9-0.1)*(19+1)-1.0+0.5);
	}
}


void GetSDTPath(Tuple X,double T[],double TPath[])
{
	int i;
	TPath[0]=X.src;
	TPath[1]=X.dest;
	TPath[2]=X.type;
	for(i=INPUT_R;i<INPUT_R+OUTPUT_R;i++)
		TPath[i]=T[i];
}



void RBFPredict(char *PredictData,char *ResultData,int h,int TestSize)
{
	FILE *fp,*fp2;;
	Tuple X;
	int i,j,count,TData[OUTPUT_R+INPUT_R],YData[OUTPUT_R+INPUT_R];
	double H[HIDDEN_R],T[OUTPUT_R],Y[OUTPUT_R],TPath[OUTPUT_R+INPUT_R],YPath[OUTPUT_R+INPUT_R];
	
	if((fp=fopen(PredictData,"r"))==NULL)
	{
		printf("%-60s can not be opened!\n",PredictData);
		exit(0);
	}
	fp2=fopen(ResultData,"w");
	count=0;
	while(!feof(fp)&&count<TestSize)
	{
		fscanf(fp,"%lf%lf%lf",&X.src,&X.dest,&X.type);
		for(i=0;i<OUTPUT_R;i++)
			fscanf(fp,"%lf",&T[i]);
		for(i=0;i<h;i++)
			H[i]=GetGaussian(Center,X,i);
        for(i=0;i<OUTPUT_R;i++)
		{
			Y[i]=0;
			for(j=0;j<h;j++)
				Y[i]+=W_R[i][j]*H[j];
			Y[i]+=B_R[i];
		}
		count++;
		GetSDTPath(X,T,TPath);
	    GetSDTPath(X,Y,YPath);
		SampleDataReverseNormalization(TPath,TData);
		SampleDataReverseNormalization(YPath,YData);
		for(i=0;i<OUTPUT_R+INPUT_R;i++)
			fprintf(fp2,"%-6d",TData[i]);
		fprintf(fp2,"\n");
		for(i=0;i<OUTPUT_R+INPUT_R;i++)
			fprintf(fp2,"%d",YData[i]);
		fprintf(fp2,"\n");
	}
	fclose(fp);
	fclose(fp2);

}



//////////////////////////////////////////////////////////////////////////
/*test 5*/
void TestRBFNN()
{
	char trainset[]="E:\\Data\\1900trainset1.txt";
	char testset[]="E:\\Data\\1900testset1.txt";
	char testsetresult[]="E:\\Data\\1900testresultRBF1.txt";
	int h=HNum;
	double e=1e-3;
	int TrainSize=100000,TestSize=50000;
	
	RBFInitialization(h);
	printf("RBFInitialization\n");
	RBFTrain(trainset,h,e,TrainSize);
	printf("RBFNetworkTrain\n");
    RBFTest(testset, h, TestSize);
	printf("RBFPredict\n");
	RBFPredict(testset,testsetresult,h, TestSize);
	
}  