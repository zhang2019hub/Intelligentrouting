#define MAXNUM  20
char prefix[]="E:\\Data\\";
char suffix[]=".txt";
char subFileName[MAXNUM][MAXNUM];
int Epochs=300;
double error=0.0000001;

void GetSubFileName(char FileName[MAXNUM][],char *prefix,char *suffix,int k)
{
	int i;
	char numstr[4]="\0";

	for(i=0;i<k;i++)
	{
		strcpy(FileName[i],"\0");
		itoa(i,numstr,10);
		strcat(FileName[i],prefix);
		strcat(FileName[i],numstr);
		strcat(FileName[i],suffix);
	}

}


void DataPartition(char *DataSet,char FileName[MAXNUM][MAXNUM],int k)
{
	FILE *fpin,*fpout[MAXNUM];
	int i,index;
	double r,data[INPUT_B+OUTPUT_B];

	if((fpin=fopen(DataSet,"r"))==NULL)
	{
		printf("%s can not be opened!\n",DataSet);
		exit(0);
	}
	for(i=0;i<k;i++)
		fpout[i]=fopen(FileName[i],"w");

	while(!feof(fpin))
	{
		for(i=0;i<INPUT_B+OUTPUT_B;i++)
				fscanf(fpin,"%lf",&data[i]);

        r=rand()*10.0/(RAND_MAX+1.0);
		index=floor(r);

		for(i=0;i<INPUT_B+OUTPUT_B;i++)
				fprintf(fpout[index],"%-14.8lf",data[i]);
		fprintf(fpout[index],"\n");
	}

	fclose(fpin);
	for(i=0;i<k;i++)
		fclose(fpout[i]);
}


void GetTrainTestSet(char FileName[MAXNUM][MAXNUM],char *TrainSet,char *TestSet,int k,int index)
{
	FILE *fp1,*fp2;
	int i,j;
	double data[INPUT_B+OUTPUT_B];

	fp1=fopen(TrainSet,"w");
	for(i=0;i<k-1;i++)
	{
		j=(index+i)%k;
		if((fp2=fopen(FileName[j],"r"))==NULL)
		{
			printf("%s can not be opened!\n",FileName[j]);
			exit(0);
		}
		while(!feof(fp2))
		{
			for(j=0;j<INPUT_B+OUTPUT_B;j++)
				fscanf(fp1,"%lf",&data[j]);
       		for(j=0;j<INPUT_B+OUTPUT_B;j++)
				fprintf(fp2,"%-14.8lf",data[j]);
			fprintf(fp2,"\n");
		}
		fclose(fp2);
	}
	fclose(fp1);

	fp1=fopen(TestSet,"w");
	if((fp2=fopen(FileName[(index+i)%k],"r"))==NULL)
	{
		printf("%s can not be opened!\n",FileName[(index+i)%k]);
		exit(0);
	}
	while(!feof(fp2))
	{
		for(j=0;j<INPUT_B+OUTPUT_B;j++)
			fscanf(fp1,"%lf",&data[j]);
     	for(j=0;j<INPUT_B+OUTPUT_B;j++)
			fprintf(fp2,"%-14.8lf",data[j]);
		fprintf(fp2,"\n");
	}
	fclose(fp1);
	fclose(fp2);
}


void NeuralNetworkTrain(char *TrainSet,int type,int h1,int h2,int h3,double error,int Epochs)
{ 
	if(type==0)
	{
		BPInitialization(INPUT_B,h1,h2,h3,OUTPUT_B);
		BPNetworkTrain(TrainSet,h1,h2,h3,error,Epochs);
	}
	else if(type==1)
	{
		RBFInitialization(h1);
		RBFTrain(TrainSet,h1,error,Epochs);
	}
}


void NeuralNetworkTest(char *TestSet,char *resultData,int type,int h1,int h2,int h3,int TestSize)
{
	if(type==0)
	{
		 BPTest(TestSet,resultData,h1,h2,h3,TestSize);
	}
	else if(type==1)
	{
		RBFTest(TestSet,h1,TestSize);
	}
}


void NeuralNetworkPredict(char *predictData,char *resultData,int type,int h1,int h2,int h3,int TestSize)
{
	if(type==0)
	{
		 BPPredict(predictData,resultData,h1,h2,h3,TestSize);
	}
	else if(type==1)
	{
		RBFPredict(predictData,resultData,h1,TestSize);
	}
}


void K_foldCrossValidation(char *DataSet,char *TrainSet,char *TestSet,int k,int type)
{
	int i;
	char predictData[]="";
	char resultData[]="E:\\data.txt";

	GetSubFileName(subFileName,prefix,suffix,k);
	DataPartition(DataSet,subFileName,k);
	for(i=0;i<k;i++)
	{
		GetTrainTestSet(subFileName,TrainSet,TestSet,k,i);
		NeuralNetworkTrain(TrainSet,type,h1,h2,h3,error,Epochs);
		NeuralNetworkTest(TestSet,resultData,type,h1,h2,h3,TestSize);
	}
}
