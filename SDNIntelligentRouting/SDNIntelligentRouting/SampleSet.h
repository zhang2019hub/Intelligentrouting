#define MaxSize 20
int PathCount;
int visited[NODENUM];
int PathSet[MaxSize][NODENUM];
int ERPathSet[MaxSize][NODENUM];

//将一个IP地址与前缀进行匹配
int PrefixMatch(char *ipAddress,char *prefix)
{
	int flag,length;
	char prefixstr[50]="\0",ipaddr[50]="\0";

	flag=0;
	length=strlen(prefix);
	strncpy(prefixstr,prefix,length-4);
	strncpy(ipaddr,ipAddress,length-4);

	if(strcmp(ipaddr,prefixstr)==0)
		flag=1;
	else
		flag=0;

	return flag;
}

//根据IP地址前缀将一个IP地址映射到一个节点
int PrefixNodeMapping(char *ipAddress,char prefixFilename[25][13])
{
	char ipPrefix[40]="\0";
	FILE *fp;

	for(int i=0;i<25;i++)
	{
		fp=fopen(prefixFilename[i],"r");
		if(fp==NULL)
		{
			printf("\nThe File :  %-30s  can not be opened!\n");
			exit(-1);
		}
		while(!feof(fp))
		{
			fscanf(fp,"%s",ipPrefix);
			if(PrefixMatch(ipAddress,ipPrefix)==1)
			{
				fclose(fp);
				return i;
			}
		}
		fclose(fp);

	}
	return -1;
}


//映射到某个节点, 
void LocateNode(char *SrcFilename,char *DestFilename)
{
	FILE *fp1,*fp2;
	int  u=0,v=0,type=0,count=1;
	char ip_src[40]="\0",ip_dst[40]="\0";

	fp1=fopen(SrcFilename,"r");
	if(fp1==NULL)
	{
		printf("The File of %s  can not be opened!\n",SrcFilename);
		exit(-1);
	}
	fp2=fopen(DestFilename,"w");

	while(!feof(fp1)&&count<1000000)//count<1000000
	{
		count++;
		fscanf(fp1,"%s%s%",ip_src,ip_dst);
		u=PrefixNodeMapping(ip_src,prefixFilename);
		v=PrefixNodeMapping(ip_dst,prefixFilename);
		if(u>-1&&v>-1)
		{
			fprintf(fp2,"%-8d%-8d\n",u,v);
		}

	}
	fclose(fp1);
	fclose(fp2);
}


//节点编号重新排列
int NodeArrange(int oldNum)
{
	int newNum;
	
	switch(oldNum)
	{
		case 0:  newNum=4;   break;
		case 1:  newNum=12;  break;
		case 2:  newNum=19;	 break;
		case 3:  newNum=14;	 break;
		case 4:  newNum=15;	 break;
		case 5:  newNum=7;   break;
		case 6:  newNum=5;   break;
		case 7:  newNum=6;   break;
		case 8:  newNum=2;   break;
		case 9:  newNum=3;   break;
		case 10: newNum=1;   break;
		case 11: newNum=0;   break;
		case 12: 
		case 13: 
		case 14: newNum=4;   break;
		case 15: newNum=8;   break;
		case 16: newNum=10;  break;
		case 17: newNum=11;  break;
		case 18: newNum=9;   break;
		case 19: newNum=13;  break;
		case 20: newNum=17;  break;
		case 21: newNum=18;  break;
		case 22: newNum=16;  break;
		case 23: 
		case 24: newNum=15;  break;
	}
	
	return newNum;
}


//生成三元组
void GetSrcDestType(char *SrcFile,char *DestFile)
{
	FILE *fp1,*fp2;
	int  src,dest,type,count=0;

	if((fp1=fopen(SrcFile,"r"))==NULL)
	{
		printf("The File of %s  can not be opened!\n",SrcFile);
		exit(-1);
	}
	fp2=fopen(DestFile,"w");

	while(!feof(fp1)&&count<1000000)//count<1000
	{
		count++;
		fscanf(fp1,"%d%d",&src,&dest);
		type=GetAppType();
		fprintf(fp2,"%-8d%-8d%-8d\n", NodeArrange(src),NodeArrange(dest),type);
	}
	fclose(fp1);
	fclose(fp2);
}

//求节点u的第一个邻居节点
int first(Graph &G,int u)
{
	int i;
	for(i=0;i<G.vexnum;i++)
		if(G.Delay[u][i]!=I)
			break;
		
	return i;
}


//求节点u的邻居节点w之后的邻居节点
int next(Graph &G,int u,int w)
{
	int i;
	for(i=w+1;i<G.vexnum;i++)
		if(G.Delay[u][i]!=I)
			break;
		
	return i;
}


//深度遍历求源点到目的节点之间所有路径集合
int DFS(Graph &G,int u,int v,int path[],int &d,int &flag,double Delay)
{
	int w;
	visited[u]=1;
	d++;
	path[d]=u;
	if(u==v&&d>=0)
	{
		double DelaySum;

		DelaySum=GetPathDelay(G,path,d+1);
		if(DelaySum<=Delay)
		{
			int i;
			
			flag=1;
			for(i=0;i<=d;i++)
				PathSet[PathCount][i]=path[i];
			PathCount++;
		}
		else
			flag=0;

	}
	for(w=first(G,u);w<G.vexnum;w=next(G,u,w))
	{
		if(visited[w]==0)
			DFS(G,w,v,path,d,flag,Delay);
	}
	visited[u]=0;
	d--;

	return flag;
}


//计算QoS路由（最佳QoS路径）
int SearchBestPath(Graph &G,int u,int v,double Bandwidth,double Delay,double ErrorRate,int BestPath[])
{
	int i,j,k,index,minLength,d=-1,flag=0,count=0;   	//深度优先遍历临时保存路径的数据变量
	int path[MaxSize],PathLength[MaxSize];	
	double ErrorRateSum;	

	PathCount=0;	   
	if(u==v)
	{
		flag=-1;
		BestPath[0]=u;
		for(i=1;i<NODENUM;i++)
			BestPath[i]=-1;
		//printf("\n该节点为目的节点\n");
		return flag;
	}
	
	for(i=0;i<G.vexnum;i++)		//对visited数组进行初始化
	{
		visited[i]=0;
		path[i]=-1;
	}
	
	for(i=0;i<MaxSize;i++)		//对PathSet数组进行初始化
		for(j=0;j<MaxSize;j++)
		{
			PathSet[i][j]=-1;
			ERPathSet[i][j]=-1;
		}
		
	//过滤带宽	
	for(i=0;i<G.vexnum;i++)
		for(j=0;j<G.vexnum;j++)
		{
			if(G.BandWidth[i][j]<Bandwidth)
				G.Delay[i][j]=I;
		}

	//过滤延迟
	if(DFS(G,u,v,path,d,flag,Delay)==0)
	{
		flag=0;
		//printf("\nNO Exist BestPath\n");
	}
	else
	{
		for(i=0;i<MaxSize;i++)
			PathLength[i]=0;	
		for(i=0;i<MaxSize;i++)
		{
			for(j=0;j<G.vexnum;j++)
				if(PathSet[i][j]==-1)
				{
					k=j;
					break;
				}
			if(k==0)
				break;
			ErrorRateSum=GetPathErrorRate(G,PathSet[i],k);
			if(ErrorRateSum>ErrorRate)	
			{
				flag=0;
			}
			else
			{
				for(j=0;j<k;j++)
					ERPathSet[count][j]=PathSet[i][j];
				PathLength[count]=k;
				count++;

				k=0;
			}
	    }

	   	minLength=PathLength[0];
		index=0;
		for(i=1;i<count;i++)
			if(minLength>PathLength[i])
			{
				minLength=PathLength[i];
				index=i;
			}
	}
	if(flag==0||minLength==0)
	{
		flag=0;
		for(i=0;i<NODENUM;i++)
			BestPath[i]=-1;
	}
	else
	{
		flag=1;
		for(i=0;i<NODENUM;i++)
			BestPath[i]=-1;		
		for(i=0;i<minLength;i++)
			BestPath[i]=ERPathSet[index][i];
	}

	return flag;
}


//根据应用类型求满足条件的最佳路径
void GetBestPath(Graph &G,int src,int dest,int AppType, int BestPath[])
{
	int i,j,Length,Path[NODENUM];
	double Bandwidth,Delay,ErrorRate;
	LinkQoS QoSParameter;

	AppTypeMapQoS(AppType,QoSParameter);
	Bandwidth=QoSParameter.BandWidth;
	Delay=QoSParameter.Delay;
	ErrorRate=QoSParameter.ErrorRate;
	GraphInitialization(G);
	SearchBestPath(G,src,dest,Bandwidth,Delay,ErrorRate,Path);

	for(i=0;i<NODENUM;i++)
		BestPath[i]=-1;
	for(i=0;i<NODENUM;i++)
		if(Path[i]==-1)
		{
			Length=i;
			break;
		}
	if(Length>0)
		for(i=0;i<Length;i++)
		{
			j=Path[i];
			BestPath[j]=i;
		}
		
}

//计算路由，生成样本数据集
void GetSampleSet(char *SDTFile,char *SampleFile)
{
	FILE *fp1,*fp2;
	int  i,src,dest,appType,count=0,BestPath[NODENUM];

	if((fp1=fopen(SDTFile,"r"))==NULL)
	{
		printf("The File of %-30s  can not be opened!\n",SDTFile);
		exit(-1);
	}
	fp2=fopen(SampleFile,"w");
	
	while(!feof(fp1)&&count<1000000)//count<1000
	{
		count++;
		fscanf(fp1,"%d%d%d",&src,&dest,&appType);
		//printf("%-6d%-6d%-6d\n",src,dest,appType);
		GetBestPath(G,src,dest,appType,BestPath);

		fprintf(fp2,"%-6d%-6d%-6d",src,dest,appType);
		for(i=0;i<NODENUM;i++)
			fprintf(fp2,"%-6d",BestPath[i]);
		fprintf(fp2,"\n");
	}

	fclose(fp1);
	fclose(fp2);
}


//单个样本数据的归一化
void SampleDataNormalization(int Data[],double NormalData[])
{
	int i;
	
	for(i=0;i<2;i++)
		NormalData[i]=0.1+(Data[i]-0)/19*(0.9-0.1);
	NormalData[2]=0.1+(Data[2]-0)/5*(0.9-0.1);
	for(i=3;i<3+NODENUM;i++)
			NormalData[i]=0.1+(Data[i]+1.0)/(19+1)*(0.9-0.1);
}


//样本数据集的归一化
void SampleSetNormalization(char *srcFile,char *destFile)
{
	FILE *fp1,*fp2; 
	int i,count,src,dest,appType,Path[NODENUM];
	double s,d,t,P[NODENUM];

	if((fp1=fopen(srcFile,"r"))==NULL)
	{
		printf("%s can not be opened!\n",srcFile);
		exit(-1);
	}
	fp2=fopen(destFile,"w");

	count=0;
	while(!feof(fp1)&&count<10000)
	{
		count++;
		fscanf(fp1,"%d%d%d",&src,&dest,&appType);
		for(i=0;i<NODENUM;i++)
			fscanf(fp1,"%d",&Path[i]);

		s=0.1+(src-0)/19*(0.9-0.1);
		d=0.1+(dest-0)/19*(0.9-0.1);
		t=0.1+(appType-0)/5*(0.9-0.1);	
		for(i=0;i<NODENUM;i++)
			P[i]=0.1+(Path[i]+1.0)/(19+1)*(0.9-0.1);
		fprintf(fp2,"%-14.8lf%-14.8lf%-14.8lf",s,d,t);
		for(i=0;i<NODENUM;i++)
			fprintf(fp2,"%-15.10lf",P[i]);
		fprintf(fp2,"\n");
	}

	fclose(fp1);
	fclose(fp2);
}

//单个样本数据的反归一化
void SampleDataReverseNormalization(double NormalData[],int Data[])
{
	int i;
	
	for(i=0;i<2;i++)
		Data[i]=(int)((NormalData[i]-0.1)/(0.9-0.1)*(19-0)+0+0.5);
	Data[2]=(int)((NormalData[2]-0.1)/(0.9-0.1)*(5-0)+0+0.5);	
	for(i=3;i<3+NODENUM;i++)
		Data[i]=(int)((NormalData[i]-0.1)/(0.9-0.1)*(19+1)-1.0+0.5);
}

//样本数据集的反归一化
void SampleSetReverseNormalization(char *srcFile,char *destFile)
{
	FILE *fp1,*fp2;
	int i,count, src,dest,appType,Path[NODENUM];
	double s,d,t,P[NODENUM];

	if((fp1=fopen(srcFile,"r"))==NULL)
	{
		printf("%s can not be opened!\n",srcFile);
		exit(-1);
	}
	fp2=fopen(destFile,"w");

	count=0;
	while(!feof(fp1)&&count<10000)
	{
		count++;
		fscanf(fp1,"%lf%lf%lf",&s,&d,&t);
		for(i=0;i<NODENUM;i++)
			fscanf(fp1,"%lf",&P[i]);	
		src=(int)((s-0.1)/(0.9-0.1)*(19-0)+0+0.5);
	    dest=(int)((d-0.1)/(0.9-0.1)*(19-0)+0+0.5);
	    appType=(int)((t-0.1)/(0.9-0.1)*(5-0)+0+0.5);
	    for(i=0;i<NODENUM;i++)
			Path[i]=(int)((P[i]-0.1)/(0.9-0.1)*(19+1)-1.0+0.5);
		fprintf(fp2,"%-6d%-6d%-6d",src,dest,appType);
		for(i=0;i<NODENUM;i++)
			fprintf(fp2,"%-6d",Path[i]);
		fprintf(fp2,"\n");
	}

	fclose(fp1);
	fclose(fp2);
}

void GetTrainTestSampleSet(char *SampleSet,char *TrainSet,char *TestSet,double Ratio, int Count)
{
	FILE *fpin,*fpout1,*fpout2;
	int i,count1=0,count2=0,count=0;
	double r,src,dest,type,P[NODENUM];
	
	if((fpin=fopen(SampleSet,"r"))==NULL)
	{
		printf("Can't open the file : %-16s\n",SampleSet);
		exit(-1);
	}
	fpout1=fopen(TrainSet,"w");
	fpout2=fopen(TestSet,"w");

	while(!feof(fpin)&&count<Count)
	{
		count++;
		fscanf(fpin,"%lf%lf%lf",&src,&dest,&type);
		for(i=0;i<NODENUM;i++)
			fscanf(fpin,"%lf",&P[i]);

	    r=rand()/(RAND_MAX*1.0);
		if(r<Ratio)
		{
			fprintf(fpout1,"%-14.8lf%-14.8lf%-14.8lf",src,dest,type);
			for(i=0;i<NODENUM;i++)
				fprintf(fpout1,"%-15.10lf",P[i]);
			fprintf(fpout1,"\n");
			count1++;
		}
		else
		{
			fprintf(fpout2,"%-14.8lf%-14.8lf%-14.8lf",src,dest,type);
			for(i=0;i<NODENUM;i++)
				fprintf(fpout2,"%-15.10lf",P[i]);
			fprintf(fpout2,"\n");			
			count2++;
		}

	 }

	fclose(fpin);
	fclose(fpout1);
	fclose(fpout2);

}


void GetSampleSetX(char *SampleSet,char*SampleSetX)
{
	FILE *fp,*fp2;
	int j;
	double src,dest,Apptype,path[NODENUM];

	if((fp=fopen(SampleSet,"r"))==NULL)
	{
		printf("%-30s can not be opened\n",SampleSet);
		exit(0);
	}
	fp2=fopen(SampleSetX,"w");

	while(!feof(fp))
	{
		fscanf(fp,"%lf%lf%lf",&src,&dest,&Apptype);
		for(j=0;j<NODENUM;j++)
			fscanf(fp,"%lf",&path[j]);		
		fprintf(fp2,"%-14.8lf%-14.8lf%-14.8lf%\n",src,dest,Apptype);
	}
	fclose(fp);
	fclose(fp2);
}


int RecordExist(char *Filename,int Count,int src,int dest,int type)
{
	FILE *fp1;
	int srcn,destn,typen,flag=1,count=0;

	if((fp1=fopen(Filename,"r"))==NULL)
	{
		printf("The File of %s  can not be opened!\n",Filename);
		exit(-1);
	}
	while(!feof(fp1)&&count<Count)
	{
		fscanf(fp1,"%d%d%d",&srcn,&destn,&typen);
		count++;
		if(srcn==src&&destn==dest&&typen==type)
			break;
	}
	fclose(fp1);
	
	if(count==Count)
		flag=0;

	return flag;
}

void DeleteRedunData(char *srcFile,char *destFile,char *Filename)
{
	FILE *fp1,*fp2;
	int src,dest,type,count=1;
	
	if((fp1=fopen(srcFile,"r"))==NULL)
	{
		printf("The File of %s  can not be opened!\n",srcFile);
		exit(-1);
	}
	fp2=fopen(destFile,"w");
	
	fscanf(fp1,"%d%d%d",&src,&dest,&type);
	fprintf(fp2,"%-8d%-8d%-8d\n",src,dest,type);

	while(!feof(fp1)&&count<10000)
	{
		fscanf(fp1,"%d%d%d",&src,&dest,&type);
		count++;
		if(RecordExist(Filename,count,src,dest,type)==0)
		{
			fprintf(fp2,"%-8d%-8d%-8d\n",src,dest,type);
		}
	}

	fclose(fp1);
	fclose(fp2);
}




///////////////////////////////////////
/* Test 1 */
void TestGetThreeTuple()
{
	printf("TestGetThreeTuple\n");
	char srcFileip[]="E:\\Data\\1900aip.txt";
	char destFilenode[]="E:\\Data\\1900Node1.txt";
	char destFiletuple[]="E:\\Data\\1900tuple1.txt";

	LocateNode(srcFileip,destFilenode);  //ipaddress  mapp to integer no.  each file contains two node no.
		printf("LocateNode over\n");
    GetSrcDestType(destFilenode,destFiletuple);
		printf("Getthreetuple over\n");
}


/* Test 2  */
void TestGetSampleSet()
{
	char destFiletuple[]="E:\\Data\\1900tuple1.txt";
	char sampleSet[]="E:\\Data\\1900sample1.txt";
	
	GetSampleSet(destFiletuple, sampleSet);
	printf("GetSampleSet over\n");

}

/* Test 3 */
//void TestGetTrainSet()
void TestDataProcess()
{
	char sampleSet[]="E:\\Data\\1900sample1.txt";
    char sampleSetnorm[]="E:\\Data\\1900samplenorm1.txt";
    char sampleSetnormX[]="E:\\Data\\1900samplenormX1.txt";
	char trainset[]="E:\\Data\\1900trainset1.txt";
	char testset[]="E:\\Data\\1900testset1.txt";
	double Ratio=0.7;
	int Count=1000000;

    SampleSetNormalization(sampleSet,sampleSetnorm);
	printf("SampleSetNormalization\n");
	GetTrainTestSampleSet(sampleSetnorm,trainset,testset,Ratio,Count);
	printf("GetTrainTestSampleSet\n");
	GetSampleSetX(sampleSetnorm,sampleSetnormX); 
	printf("GetSampleSetX\n");

}