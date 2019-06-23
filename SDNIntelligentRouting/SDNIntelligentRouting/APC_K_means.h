#define SampleMinNum  10

void APC_K_means(char *SampleSetX,double d,int MinSampleNum)
{
	FILE *fp;
	int i;
	double R,X[INPUT_R];
	double distance[MaxNum];

	for(i=0;i<MaxNum;i++)
	{
		sampleNum[i]=0;
		distance[i]=0;
	}
	delta=d;
	m=0;
	if((fp=fopen(SampleSetX,"r"))==NULL)
	{
		printf("%s can not open!\n",SampleSetX);
		exit(-1);
	}
	for(i=0;i<INPUT_R;i++)
		fscanf(fp,"%lf",&X[i]);	
	GetCenter(center,X,m);
	sampleNum[m]=1;

	R=GetDistanceRadius(SampleSetX,delta);
	while(!feof(fp))
	{
		for(i=0;i<INPUT_R;i++)
			fscanf(fp,"%lf",&X[i]);		
		CalculateDistance(X,center,distance,m);
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

	int num=m;
	for(i=0;i<=num;i++)
		if(sampleNum[i]<MinSampleNum)
			m--;
}


void Getcenter1()
{
	APC_K_means(SampleSetX,0.5,SampleMinNum);
	SaveExpParameters(sampleNormX,Center,clusterCenter,clusterWidth,HNum);
}