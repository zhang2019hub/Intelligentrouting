#define INPUT_B  3
#define HIDDEN_B 20
#define OUTPUT_B 20
double B_Er=1e+1;
double  H[HIDDEN_B],H2[HIDDEN_B],H3[HIDDEN_B];
double  EH[HIDDEN_B],EH2[HIDDEN_B],EH3[HIDDEN_B];
double  WIH_B[HIDDEN_B][INPUT_B],WH2_B[HIDDEN_B][HIDDEN_B],WH3_B[HIDDEN_B][HIDDEN_B];
double  WHO_B[OUTPUT_B][HIDDEN_B];
double  BH_B[HIDDEN_B],BH2_B[HIDDEN_B],BH3_B[HIDDEN_B];
double  BO_B[OUTPUT_B],BO2_B[OUTPUT_B],BO3_B[OUTPUT_B];
double  WIHRate_B=0.1,WH2Rate_B=0.1,WH3Rate_B=0.1;
double  WHORate_B=0.1;
double  BHRate_B=0.5,BH2Rate_B=0.5,BH3Rate_B=0.5;
double  BORate_B=0.5;

//Sigmoid函数（）
double GetSigmoid(double x)
{
	double y;

	y=1/(1+exp(-x));

	return y;
}
//线性函数
double GetLinearFunction(double  X[],double W[],double b,int n)
{
	int i;
	double y=0;

	for(i=0;i<n;i++)
		y+=X[i]*W[i];
	y+=b;

	return y;
}
//计算实际输出与期望输出之间的误差
double GetYError(double Y[],double T[],int k)
{
	int i;
	double error=0;
	for(i=0;i<k;i++)
		error+=(T[i]-Y[i])*(T[i]-Y[i]);

	return error;
}
//计算BP神经网络输出层的误差
void GetOutputError(double Y[],double T[],double EO[] ,int k)
{
	int i;
	for(i=0;i<k;i++)
		EO[i]=Y[i]*(1-Y[i])*(T[i]-Y[i]);
	
}

//计算BP神经网络隐含层的误差
void GetHiddenError(double EO[],int h1,int h2,int h3,int k)
{
    int i,j;
	if(h2==0&&h3==0)
	{
		for(i=0;i<h1;i++)
		{
			EH[i]=0;
			for(j=0;j<k;j++)
				EH[i]+=WHO_B[j][i]*EO[j];
			EH[i]*=H[i]*(1-H[i]);
		}
	}
	else if(h3==0)
	{
		for(i=0;i<h2;i++)
		{
			EH2[i]=0;
			for(j=0;j<k;j++)
				EH2[i]+=WHO_B[j][i]*EO[j];
			EH2[i]*=H2[i]*(1-H2[i]);
		}
		for(i=0;i<h1;i++)
		{
			EH[i]=0;
			for(j=0;j<h2;j++)
				EH[i]+=WH2_B[j][i]*EH2[j];
			EH[i]*=H[i]*(1-H[i]);
		}
	}
	else
	{
		for(i=0;i<h3;i++)
		{
			EH3[i]=0;
			for(j=0;j<k;j++)
				EH3[i]+=WHO_B[j][i]*EO[j];
			EH3[i]*=H3[i]*(1-H3[i]);
		}
		for(i=0;i<h2;i++)
		{
			EH2[i]=0;
			for(j=0;j<h3;j++)
				EH2[i]+=WH3_B[j][i]*EH3[j];
			EH2[i]*=H2[i]*(1-H2[i]);
		}
		for(i=0;i<h1;i++)
		{
			EH[i]=0;
			for(j=0;j<h2;j++)
				EH[i]+=WH2_B[j][i]*EH2[j];
			EH[i]*=H[i]*(1-H[i]);
		}
	}
}


int IsAcceptableYError(double Y[],double T[],int k)
{
	int i,flag=0,count=0;
	for(i=0;i<k;i++)
		if(abs(Y[i]-T[i])<B_Er)
			count++;
	if(count==k)
		flag=1;

	return flag;
}


void BPForward(double X[],double Y[],int m,int h1,int h2,int h3,int k)
{
	int i,j;
	for(i=0;i<h1;i++)
	{
		H[i]=0;
		for(j=0;j<m;j++)
			H[i]+=X[j]*WIH_B[i][j];
		H[i]+=BH_B[i];
		H[i]=GetSigmoid(H[i]);
	}
	
	for(i=0;i<h2;i++)
	{
		H2[i]=0;
		for(j=0;j<h1;j++)
			H2[i]+=H[j]*WH2_B[i][j];
		H2[i]+=BH2_B[i];
		H2[i]=GetSigmoid(H2[i]);
	}
	for(i=0;i<h3;i++)
	{
		H3[i]=0;
		for(j=0;j<h2;j++)
			H3[i]+=H2[j]*WH3_B[i][j];
		H3[i]+=BH3_B[i];
		H3[i]=GetSigmoid(H3[i]);
	}

	for(i=0;i<k;i++)
	{
		Y[i]=0;
		if(h2==0&&h3==0)
			Y[i]=GetLinearFunction(H,WHO_B[i],BO_B[i],h1);
		else if(h3==0)
			Y[i]=GetLinearFunction(H2,WH2_B[i],BO_B[i],h2);
		else
			Y[i]=GetLinearFunction(H3,WH3_B[i],BO_B[i],h3);
	}


	//printf("BPForward\n");
}



void BPReverse(double X[],double H[],double EO[],int m,int h1,int h2,int h3,int k)
{
  	int i,j;
	if(h2==0&&h3==0)
	{
		for(i=0;i<k;i++)
		{
			for(j=0;j<h1;j++)
				WHO_B[i][j]+=WHORate_B*H[j]*EO[i];
			BO_B[i]+=BORate_B*EO[i]; 
		}
		for(i=0;i<h1;i++)
		{
			for(j=0;j<m;j++)
				WIH_B[i][j]+=WIHRate_B*X[j]*EH[i];
			BH_B[i]+=BHRate_B*EH[i];  
		}
	}
	else if(h3==0)
	{
		for(i=0;i<k;i++)
		{
			for(j=0;j<h2;j++)
				WHO_B[i][j]+=WHORate_B*H2[j]*EO[i];
			BO_B[i]+=BORate_B*EO[i]; 
		}
		for(i=0;i<h2;i++)
		{
			for(j=0;j<h1;j++)
				WH2_B[i][j]+=WH2Rate_B*H[j]*EH2[i];
			BH2_B[i]+=BH2Rate_B*EH2[i];  
		}
		for(i=0;i<h1;i++)
		{
			for(j=0;j<m;j++)
				WIH_B[i][j]+=WIHRate_B*X[j]*EH[i];
			BH_B[i]+=BHRate_B*EH[i];  
		}
	}
	else
	{
		for(i=0;i<k;i++)
		{
			for(j=0;j<h3;j++)
				WHO_B[i][j]+=WHORate_B*H3[j]*EO[i];
			BO_B[i]+=BORate_B*EO[i]; 
		}

		for(i=0;i<h3;i++)
		{
			for(j=0;j<h3;j++)
				WH3_B[i][j]+=WH3Rate_B*H2[j]*EH3[i];
			BH3_B[i]+=BH3Rate_B*EH3[i];  
		}
		for(i=0;i<h2;i++)
		{
			for(j=0;j<h1;j++)
				WH2_B[i][j]+=WH2Rate_B*H[j]*EH2[i];
			BH2_B[i]+=BH2Rate_B*EH2[i];  
		}
		for(i=0;i<h1;i++)
		{
			for(j=0;j<m;j++)
				WIH_B[i][j]+=WIHRate_B*X[j]*EH[i];
			BH_B[i]+=BHRate_B*EH[i];  
		}
	}
	
}


void BPupdate(double X[],double H[],double T[],double Y[],int m,int h1,int h2,int h3,int k)
{
	//printf("BPudpate\n");
	double EO[OUTPUT_B];

	BPForward(X,Y,m,h1,h2,h3,k);
	GetOutputError(Y,T,EO,k);
	GetHiddenError(EO,h1,h2,h3,k);
	BPReverse(X,H,EO,m,h1,h2,h3,k);

}


void BPInitialization(int m,int h1,int h2,int h3,int k)
{
	printf("Begin Intialize BPNN\n");
	int i,j;
	if(h2==0&&h3==0)
	{
		for(i=0;i<h1;i++)
		{
			for(j=0;j<m;j++)
				WIH_B[i][j]=rand()*1.0/(RAND_MAX+1.0);
			BH_B[i]=rand()*1.0/(RAND_MAX+1.0);
		}
		for(i=0;i<k;i++)
		{
			for(j=0;j<h1;j++)
				WHO_B[i][j]=rand()*1.0/(RAND_MAX+1.0);
			BO_B[i]=rand()*1.0/(RAND_MAX+1.0);
		}
	}
	else if(h3==0)
	{
		for(i=0;i<h1;i++)
		{
			for(j=0;j<m;j++)
				WIH_B[i][j]=rand()*1.0/(RAND_MAX+1.0);
			BH_B[i]=rand()*1.0/(RAND_MAX+1.0);
		}
		for(i=0;i<h2;i++)
		{
			for(j=0;j<h1;j++)
				WH2_B[i][j]=rand()*1.0/(RAND_MAX+1.0);
			BO2_B[i]=rand()*1.0/(RAND_MAX+1.0);
		}
		for(i=0;i<k;i++)
		{
			for(j=0;j<h2;j++)
				WHO_B[i][j]=rand()*1.0/(RAND_MAX+1.0);			
			BO_B[i]=rand()*1.0/(RAND_MAX+1.0);

		}
	}
	else
	{
		for(i=0;i<h1;i++)
		{
			for(j=0;j<m;j++)
				WIH_B[i][j]=rand()*1.0/(RAND_MAX+1.0);
			BH_B[i]=rand()*1.0/(RAND_MAX+1.0);
		}
		for(i=0;i<h2;i++)
		{
			for(j=0;j<h1;j++)
				WH2_B[i][j]=rand()*1.0/(RAND_MAX+1.0);
			BO2_B[i]=rand()*1.0/(RAND_MAX+1.0);
		}
		for(i=0;i<h3;i++)
		{
			for(j=0;j<h2;j++)
				WH3_B[i][j]=rand()*1.0/(RAND_MAX+1.0);
			BO3_B[i]=rand()*1.0/(RAND_MAX+1.0);
		}
		for(i=0;i<k;i++)
		{
			for(j=0;j<h3;j++)
				WHO_B[i][j]=rand()*1.0/(RAND_MAX+1.0);			    
			BO_B[i]=rand()*1.0/(RAND_MAX+1.0);
			//printf("%lf\n",BO_B[i]);
		}
	}

}


void BPNetworkTrain(char *trainData,int h1,int h2,int h3,int k, double error,int Epochs)
{
	printf("Begin train BPNN\n");

	FILE * fp;
	int    i,TrainCount;
	double ErrSum,H[HIDDEN_B];
	double X[INPUT_B],T[OUTPUT_B],Y[OUTPUT_B];
	int number;
	TrainCount=0;
	do
	{
		if((fp=fopen(trainData,"r"))==NULL)
		{
			printf("%-60s can not be opened!\n",trainData);
			exit(0);
		}

		ErrSum=0;
		number=0;
		while(!feof(fp)&&number<100000)
		{
			number++;
			for(i=0;i<INPUT_B;i++)
				fscanf(fp,"%lf",&X[i]);
			for(i=0;i<OUTPUT_B;i++)
				fscanf(fp,"%lf",&T[i]);

			BPupdate(X,H,T,Y,INPUT_B,h1,h2,h3,OUTPUT_B);
			ErrSum+=GetYError(Y,T,OUTPUT_B);	
		}
		fclose(fp);
		TrainCount++;
		ErrSum/=number;
	}while(ErrSum<error||TrainCount<Epochs);
		printf("Finsih train BPNN\n");

}


void MergeData(double X[],double Y[],double YPath[],int m,int k)
{
	int i=0;
	for(i=0;i<m;i++)
		YPath[i]=X[i];
	for( ;i<m+k;i++)
		YPath[i]=Y[i];
}


double BPTest(char *testData,char *resultData,int h1,int h2, int h3,int k, int TestSize)
{
	FILE *fp,*fp2;
	int i,j,count=0,success=0;
	int TData[OUTPUT_B+INPUT_B],YData[OUTPUT_B+INPUT_B];
	double ErrSum=0,X[INPUT_B],H[HIDDEN_B],T[OUTPUT_B],Y[OUTPUT_B];
	double TPath[OUTPUT_B+INPUT_B],YPath[OUTPUT_B+INPUT_B];
	
	if((fp=fopen(testData,"r"))==NULL)
	{
		printf("%-60s can not be opened!\n",testData);
		exit(0);
	}
	fp2=fopen(resultData,"w");

	while(!feof(fp)&&count<TestSize)
	{
		count++;
		for(i=0;i<INPUT_B;i++)
			fscanf(fp,"%lf",&X[i]);
		for(i=0;i<OUTPUT_B;i++)
			fscanf(fp,"%lf",&T[i]);

		BPForward(X,Y,INPUT_B,h1,h2,h3,k)	;	
		if(IsAcceptableYError(Y,T,OUTPUT_B)==1)
			success++;

	    ErrSum+=GetYError(Y,T,OUTPUT_B);
		MergeData(X,T,TPath,INPUT_B,OUTPUT_B);
		MergeData(X,Y,YPath,INPUT_B,OUTPUT_B);
		SampleDataReverseNormalization(TPath,TData);
		SampleDataReverseNormalization(YPath,YData);
		
		for(i=0;i<OUTPUT_B+INPUT_B;i++)
			fprintf(fp2,"%-6d",TData[i]);
		fprintf(fp2,"\n");
		for(i=0;i<OUTPUT_B+INPUT_B;i++)
			fprintf(fp2,"%d",YData[i]);
		fprintf(fp2,"\n");

	}
	fprintf(fp2,"%-12.7lf\n",success*1.0/count);
	printf("%-12.7lf\n",success*1.0/count);

	fclose(fp);
	fclose(fp2);
	
	return (success*1.0/count);
}


int BPPathPredict(double X[],double T[],double Y[],int m,int h1,int h2,int h3,int k)
{
	int i,j,flag=0;
	int TData[OUTPUT_B+INPUT_B],YData[OUTPUT_B+INPUT_B];
	double H[HIDDEN_B],TPath[OUTPUT_B+INPUT_B],YPath[OUTPUT_B+INPUT_B];

	BPForward(X,Y,INPUT_B,h1,h2,h3,k)	;
	flag=IsAcceptableYError(Y,T,k);	
	MergeData(X,T,TPath,INPUT_B,OUTPUT_B);
	MergeData(X,Y,YPath,INPUT_B,OUTPUT_B);
	SampleDataReverseNormalization(TPath,TData);
	SampleDataReverseNormalization(YPath,YData);	

	for(i=0;i<m+k;i++)
		printf("%-6d",TData[i]);
	printf("\n");
	for(i=0;i<m+k;i++)
		printf("%-6d",YData[i]);
	printf("\n");

	return flag;
}


double BPPredict(char *predictData,char *resultData,int h1,int h2,int h3,int k, int TestSize)
{
	FILE *fp,*fp2;
	int i,j,count=0,success=1;
	int TData[OUTPUT_B+INPUT_B],YData[OUTPUT_B+INPUT_B];
	double ErrSum=0,X[INPUT_B],H[HIDDEN_B],T[OUTPUT_B],Y[OUTPUT_B];
	double TPath[OUTPUT_B+INPUT_B],YPath[OUTPUT_B+INPUT_B];
	
	if((fp=fopen(predictData,"r"))==NULL)
	{
		printf("%-60s can not be opened!\n",predictData);
		exit(0);
	}
	fp2=fopen(resultData,"w");

	while(!feof(fp)&&count<TestSize)
	{
		count++;
		for(i=0;i<INPUT_B;i++)
			fscanf(fp,"%lf",&X[i]);
		for(i=0;i<OUTPUT_B;i++)
			fscanf(fp,"%lf",&T[i]);

		BPForward(X,Y,INPUT_B,h1,h2,h3,k)	;
		if(IsAcceptableYError(Y,T,OUTPUT_B)==1)
			success++;

	    ErrSum+=GetYError(Y,T,OUTPUT_B);
		MergeData(X,T,TPath,INPUT_B,OUTPUT_B);
		MergeData(X,Y,YPath,INPUT_B,OUTPUT_B);
	  	SampleDataReverseNormalization(TPath,TData);
		SampleDataReverseNormalization(YPath,YData);
		
		for(i=0;i<OUTPUT_B+INPUT_B;i++)
			fprintf(fp2,"%-6d",TData[i]);
		fprintf(fp2,"\n");
		for(i=0;i<OUTPUT_B+INPUT_B;i++)
			fprintf(fp2,"%-6d",YData[i]);
		fprintf(fp2,"\n");

	}
	fprintf(fp2,"%-12.7lf\n",success*1.0/count);

	fclose(fp);
	fclose(fp2);

	return  ErrSum;
}


///////////////////////////////////////////////////////////
/* Test 4 */
void TestBPNN()
{
	char trainset[]="E:\\Data\\1900trainset1.txt";
	char testset[]="E:\\Data\\1900testset1.txt";
	char testsetresult[]="E:\\Data\\1900testresultBP1.txt";
	int m=3;
	int h1=10, h2=12, h3=12;
	int k=20;
	double error=1e-6;
	int Epochs=2500;  
	int TestSize=100000;

	//BPNN train
	BPInitialization(m,h1,h2, h3, k);
	printf("BPInitialization\n");
	BPNetworkTrain(trainset, h1, h2,h3,k, error,Epochs);
	printf("BPNetworkTrain\n");

	//BPNN predict
	BPPredict(testset,testsetresult, h1, h2, h3, k,TestSize);
	printf("BPPredict\n");
	
}  

