#define MAX         20 
#define INPUT_R     3
#define HIDDEN_R	6
#define OUTPUT_R    20
#define MAX_TRAIN_R	10000

typedef struct 
{
	double src;
	double dest;
	double type;
}Tuple; 
int     HNum=HIDDEN_R;
Tuple   Center[HIDDEN_R];;
double  Width[HIDDEN_R];

char sampleNormX[62]="E:\\Data\\1900samplenormX1.txt";
char clusterFile[HIDDEN_R][30]={"E:\\Data\\cluster1.txt","E:\\Data\\cluster2.txt","E:\\Data\\cluster3.txt","E:\\Data\\cluster4.txt",
	                            "E:\\Data\\cluster5.txt","E:\\Data\\cluster6.txt"};
char clusterWidth[]="E:\\Data\\width.txt";
char clusterCenter[]="E:\\Data\\center.txt";


void InitCulster(Tuple cluster[],int HNum)
{
	FILE *fp;
	int rand_n[HIDDEN_R],i,j,count;
	double src,dest,Apptype,path[OUTPUT_R];

	for(i=0;i<HNum;i++)
	{
		cluster[i].src=rand()%100/(double)101*0.5;
		cluster[i].dest=rand()%100/(double)101*0.5;
		cluster[i].type=rand()%100/(double)101*0.5;
	}

}

double GetDistance(Tuple X,Tuple Y)
{
	double sum;
	sum=(X.src-Y.src)*(X.src-Y.src)+(X.dest-Y.dest)*(X.dest-Y.dest)+(X.type-Y.type)*(X.type-Y.type);	

	return (sum);
}


int ClusterOfTuple(Tuple Center[],Tuple X,int HNum)
{
	int i,label=0;
	double temp,dist;
	
	dist=GetDistance(Center[0],X);
	for(i=1;i<HNum;i++)
	{
		temp=GetDistance(Center[i],X);
		if(temp<dist)
		{
			dist=temp;
			label=i;
		}
	}

	return label;
}


void UpdataCluster(char *srcFileX,char clusterFile[][30],Tuple Center[],int HNum)
{
//	printf("void UpdataCluster( )\n");
	Tuple X;
	int i,label,count=0;;
	double path[NODENUM];
	FILE *fp, *clusterfp[HIDDEN_R];

	if((fp=fopen(srcFileX,"r"))==NULL)
	{
		printf("%-60s can not be opened\n",srcFileX);
		exit(0);
	}
	else
	{		
		for(i=0;i<HNum;i++)
			clusterfp[i]=fopen(clusterFile[i],"w");	
		while(!feof(fp)&&count<100000)
		{
			fscanf(fp,"%lf%lf%lf",&X.src,&X.dest,&X.type);
			//for(i=0;i<OUTPUT_R;i++)
			//	fscanf(fp,"%lf",&path[i]);
		    label=ClusterOfTuple(Center,X,HNum);
			fprintf(clusterfp[label],"%-14.8lf%-14.8lf%-14.8lf%\n",X.src,X.dest,X.type);
			count++;
		}
		for(i=0;i<HNum;i++)
			fclose(clusterfp[i]);
	}
	fclose(fp);

}


void CopyCenter(Tuple R[],Tuple Rtemp[],int HNum)
{
	int i;
	for(i=0;i<HNum;i++)
	{
		Rtemp[i].src=R[i].src;
		Rtemp[i].dest=R[i].dest;
		Rtemp[i].type=R[i].type;
	}
}


double GetError(Tuple R[],Tuple Rtemp[],int HNum)
{
	int i;
	double error=0;

	for(i=0;i<HNum;i++)
		error+=GetDistance(R[i],Rtemp[i]);

	return error;
}


double GetVar(char clusterFile[][30],Tuple Center[],int HNum)
{
//	printf("double GetVar( )\n");
	FILE *fp ;
	Tuple X;
	int i;
	double var=0;

	for(i=0;i<HNum;i++)
	  	if((fp=fopen(clusterFile[i],"r"))==NULL)
		{
			printf("%-60s can not be opened\n",clusterFile[i]);
			exit(0);
		}
		else
		{
			while(!feof(fp))
			{
				fscanf(fp,"%lf%lf%lf",&X.src,&X.dest,&X.type);
				var+=GetDistance(Center[i],X);
			}

		    fclose(fp);
		}

	return var;
}


Tuple GetMeans(char *filenameX,Tuple Center[],int HNum)
{
    //printf("Tuple GetMeans( )\n");
	FILE *fp;
	Tuple X,Y;
	int i,size,count=0;

	if((fp=fopen(filenameX,"r"))==NULL)
	{
		printf("%-60s can not be opened\n",filenameX);
		exit(0);
	}
	else
	{
		fseek(fp,0L,SEEK_END);
		size=ftell(fp);
		if(size==0)
		{
			for(i=0;i<HNum;i++)
				if(strcmp(clusterFile[i],filenameX)==0)
					break;
			Y.src= Center[i].src;
			Y.dest= Center[i].dest;
			Y.type= Center[i].type;
		}
		else
		{
			rewind(fp);
			Y.src=0;
			Y.dest=0;
			Y.type=0;
			while(!feof(fp))
			{
				fscanf(fp,"%lf%lf%lf",&X.src,&X.dest,&X.type);
				Y.src+=X.src;
				Y.dest+=X.dest;
				Y.type+=X.type;
				count++;
			}
			Y.src=Y.src/(count*1.0);
			Y.dest=Y.dest/(count*1.0);
			Y.type=Y.type/(count*1.0);	
		 }
	 }
	fclose(fp);	

	return Y;
}


void K_MeansClustering(char *sampleNormX,Tuple Center[],int HNum)
{
//	printf("void K_MeansClustering()\n");
	int i,count=0;
	double error=0;
	Tuple Rtemp[HIDDEN_R];

	InitCulster(Center,HNum);
	do{
		UpdataCluster(sampleNormX,clusterFile,Center,HNum);
		CopyCenter(Center,Rtemp,HNum);

		for(i=0;i<HNum;i++)
			Center[i]=GetMeans(clusterFile[i],Center,HNum);
		error=sqrt(GetError(Center,Rtemp,HNum));	
		count++;
	}while(error>1e-10&&count<50000);
	
}


double GetCenterMaxDistance(int K,Tuple Center[],int HNum)
{
//	printf("double GetCenterMaxDistance( )\n");
	int i;
    double max=0,distance;

	for(i=0;i<HNum;i++)
	{
		distance=GetDistance(Center[i],Center[K]);
		if(distance>max)
			max=distance;
	}

	return max;
}


//根据中心计算宽度
void GetCenterWidth(Tuple Center[],double Width[],int HNum)
{
	int i;
	for(i=0;i<HNum;i++)
		Width[i]=GetCenterMaxDistance(i,Center,HNum)/(2*HNum);
}


//运行K-means聚类算法并将中心和宽度保存在文件中
void  SaveExpParameters(char *sampleNormX,Tuple Center[],char*clusterCenter,char *clusterWidth,int HNum) 
{
//	printf("void SaveCenterandWidth( )\n");
	int i;
	FILE *fpc,*fpw;

	K_MeansClustering(sampleNormX,Center,HNum);
	for(i=0;i<HNum;i++)
		Width[i]=GetCenterMaxDistance(i,Center,HNum)/(2*HNum);
	
	fpc=fopen(clusterCenter,"w");
	fpw=fopen(clusterWidth,"w");
	for(i=0;i<HNum;i++)
		{
			fprintf(fpc,"%-12.6lf%-12.6lf%-12.6lf\n", Center[i].src, Center[i].dest, Center[i].type);
			fprintf(fpw,"%-18.12lf\n",Width[i]);
		}

	fclose(fpc);
}


//从文件中读出中心和宽度到数据中
void GetExpParameters(char *clusterCenter,char *clusterWidth,Tuple Center[],double Width[],int HNum )
{
	int i;
	FILE *fpc,*fpw;

	if((fpc=fopen(clusterCenter,"r"))==NULL)
	{
		printf("%-60s can not be opened\n",clusterCenter);
		exit(0);
	}
	if((fpw=fopen(clusterWidth,"r"))==NULL)
	{
		printf("%-60s can not be opened\n",clusterWidth);
		exit(0);
	}
	for(i=0;i<HNum;i++)
		{
			fscanf(fpc,"%lf%lf%lf\n",& Center[i].src,& Center[i].dest,& Center[i].type);
			fscanf(fpw,"%lf\n",&Width[i]);
		}

	fclose(fpc);
	fclose(fpw);	
}


//////////////////

void Getcenter()
{
	SaveExpParameters(sampleNormX,Center,clusterCenter,clusterWidth,HNum);

}