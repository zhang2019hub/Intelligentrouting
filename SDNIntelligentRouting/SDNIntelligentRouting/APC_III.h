#define MaxNum 20
int m=0;
double delta=0.5;
Tuple center[MaxNum];
double sampleNum[MaxNum]; 
char SampleSetX[]="E:\\Data\\1900samplenormX1.txt";
char centerapc[]="E:\\Data\\centerAPC.txt";

double GetDistance(double X[INPUT_R],double Y[INPUT_R])
{
	int i;
	double sum=0;

	for(i=0;i<INPUT_R;i++)
		sum+=(X[i]-Y[i])*(X[i]-Y[i]);

	return sum;
}


double GetDistanceRadius(char *SampleSetX,double delat)
{
	FILE *fp;
	int i,j,count=0;
	double Data[INPUT_R],R,sum=0;
	double X[INPUT_R];


	if((fp=fopen(SampleSetX,"r"))==NULL)
	{
		printf("%s can not open file\n",SampleSetX);
		exit(-1);
	}
	while(!feof(fp)&&count<1)
	{
		for(i=0;i<INPUT_R;i++)
			fscanf(fp,"%lf",&X[i]);
	}

	while(!feof(fp))
	{
		for(i=0;i<INPUT_R;i++)
			fscanf(fp,"%lf",&Data[i]);
		sum+=sqrt(GetDistance(X,Data));
		count++;
	 }	
	R=delta*sum/count;
	fclose(fp);

	return R;
}

void GetCenter(Tuple center[MaxNum],double X[INPUT_R],int m)
{
	center[m].src=X[0];
	center[m].dest=X[1];
	center[m].type=X[2];
}


void CalculateDistance(double X[INPUT_R],Tuple center[MaxNum],double distance[MaxNum],int m)
{
	int i;
	for(i=0;i<m;i++)
		distance[i]=(X[0]-center[i].src)*(X[0]-center[i].src)+(X[1]-center[i].dest)*(X[1]-center[i].dest)
		             +(X[2]-center[i].type)*(X[2]-center[i].type);

}

void APC_IIIAlogorithm(char *SampleSetX,double d)
{
	FILE *fp;
	int i;
	int size=0;
	double R,X[INPUT_R];
	double distance[MaxNum];

	for(i=0;i<MaxNum;i++)
	{
		sampleNum[i]=0;
		distance[i]=0.5;
	}
	center[0].src=rand()%100/(double)101*0.5;
	center[0].dest=rand()%100/(double)101*0.5;
	center[0].type=rand()%100/(double)101*0.5;

	delta=d;
	if((fp=fopen(SampleSetX,"r"))==NULL)
	{
		printf("%s can not open!\n",SampleSetX);
		exit(-1);
	}
	sampleNum[m]=1;
	R=GetDistanceRadius(SampleSetX,delta);

	while(!feof(fp)&&size<100000)
	{
		
		size++;
		for(i=0;i<INPUT_R;i++)
			fscanf(fp,"%lf",&X[i]);	
		CalculateDistance(X,center,distance,m);
		distance[i]=rand()%100/(double)101;
		for(i=0;i<m;i++)
		{
			if(distance[i]<=R)
			{
				center[i].src=(center[i].src*sampleNum[i]+X[0])/(sampleNum[i]+1);
				center[i].dest=(center[i].dest*sampleNum[i]+X[1])/(sampleNum[i]+1);
				center[i].type=(center[i].type*sampleNum[i]+X[2])/(sampleNum[i]+1);
				sampleNum[i]=sampleNum[i]+1;
			}
			else
			{
				m++;
				center[m].src=X[0];
				center[m].dest=X[1];
				center[m].type=X[2];
				sampleNum[m]=1;
			}
		}
	}
	fclose(fp);

}


void  SaveAPCCenters(Tuple center[],char*clusterCenter,int HNum) 
{
	int i;
	FILE *fpc;

	fpc=fopen(clusterCenter,"w");
	for(i=0;i<HNum;i++)
		{
			fprintf(fpc,"%-12.6lf%-12.6lf%-12.6lf\n", center[i].src, center[i].dest, center[i].type);
			printf("%-4d:   %-12.6lf%-12.6lf%-12.6lf\n",i+1, center[i].src, center[i].dest, center[i].type);
		}

	fclose(fpc);
}



void Getcenter2()
{
	APC_IIIAlogorithm(SampleSetX,0.5);
	SaveAPCCenters(center,centerapc,m);

}