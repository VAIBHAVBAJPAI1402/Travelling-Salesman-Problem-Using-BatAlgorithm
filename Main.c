#include<stdio.h>
#include<stdlib.h>
#include<time.h>		//for using time(0) in srand function

#define VAL 30000
//check whether the randomly selected city has already been included in the path or not
int city_seen(int city_num, int size, int pattern[size+1])
{
	int i;
	for(i = 0; i < size ; i++)
	{
		if(pattern[i] == city_num)
			return 1;
	}
	return 0;
}

//generate a random path
void random_path_generator(int size, int random_path[size+1])
{

	int city_num;
	int x,i;
	//initialise each element as 0
	for(x = 0; x < size+1; x++)
		random_path[x] = 0;	
	
	for(x = 0; x < size; x++) 
	{
		while(1)
		{
			city_num = rand() % size + 1; 

			if(!city_seen(city_num, size, random_path)) 
			{	
				random_path[x] = city_num;	
				break;            
			}
		}                       
	}
	random_path[size] = random_path[0]; 
	printf("\nRandom Generated path : ");
	for(i=0;i<size+1;i++)
	{
		printf("%d - ",random_path[i]);
	}
}

//Calculate cost of the path
int cost_of_path(int size,int path[size+1], int path_matrix[size][size])
{
	int cost=0;
	
	for(int i=1;i<size+1;i++)
	{
		int city1_name = path[i-1];
		int city2_name = path[i];
		int row_idx = city1_name-1;
		int col_idx = city2_name-1;
		cost += path_matrix[row_idx][col_idx];
	}
	
	return cost;
}

//input of path matrix
void enter_path_matrix(int size, int (*path_matrix)[size])
{
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			scanf("%d",&path_matrix[i][j]);
		}
	}
	
}

//Display path matrix
void print_path_matrix(int size, int (*path_matrix)[size])
{
	printf("The path matrix is-\n");
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			printf("%d\t",path_matrix[i][j]);
		}
		printf("\n");
	}
}

void sort(int solutions, int cost[solutions],int idx[solutions])
{
	int temp;
	int temp_idx;
	
	for(int i=0;i<solutions-1;i++)
	{
		for(int j=0;j<solutions-i-1;j++)
		{
			if(cost[j]>cost[j+1])
			{
				temp=cost[j];
				cost[j]=cost[j+1];
				cost[j+1]=temp;
				
				temp_idx=idx[j];
				idx[j]=idx[j+1];
				idx[j+1]=temp_idx;
			}
		}
	}
}

void BatAlgorithm(int size, int solutions, double freq[solutions],int random_paths[solutions][size+1], int cost[solutions], int path_matrix[size][size],int idx[solutions])
{
	printf("\nStarting Bats-\n");
	int d=3;
	int n_bats=50;
	double fitness;
	int best[3][size+1];
	int k=0,flag1=0,flag2=0,cont=0;
	int temp;
	int min_path_idx;
//	int distance[];
	int min_freq_idx=0;
	fitness=freq[0];
	for(int i=1;i<solutions;++i)
	{
		if(freq[i]<fitness)
		{
			fitness=freq[i];
			min_freq_idx=i;
		}	
	}
	sort(solutions,cost,idx);
	for(int i=0;i<solutions;i++)
	{
		printf("\n%d = Freq : %d\tindex : %d",i,cost[i],idx[i]);
	}
	printf("\nBest solution : ");
	for(int i=0;i<size+1;i++)
	{
		best[k][i] = random_paths[idx[0]][i];
		best[k+1][i] = random_paths[idx[1]][i];
		best[k+2][i] = random_paths[idx[2]][i];
	}
	
	int min_cost = cost[0];
	printf("\nCost = %d",min_cost);
	
	//Initialising paramters for Bat
	double A,r;	//Loudness and pulse rate
	A=0.25;
	r=0.3;
	//counting number of iterations
	int n_iter=0;
	
	int sample_1[size+1],sample_2[size+1];
	int best_sol[size+1];
	for(int i=0;i<size+1;i++)
	{
		best_sol[i] = best[0][i];
	}
	k=0;
	int ins=0;
	while(n_bats>0)
	{
	
		for(int i=0;i<solutions;i++)
		{
			int num = rand()%size;
			float tot = (float)num/size; 
			if(tot>r)
			{
				k=0;
				while(k<3)
				{
					printf("\nFor Solutions %d",k+1);
					int temp_best[size+1];
					temp_best[0]=best[k][0];
					temp_best[size]=best[k][size];
					printf("\n\nNew path generated\t");
					printf("%d - ",temp_best[0]);
					for(int j=1;j<size;++j)
					{
						ins=0;
						int node=(rand()%size)+1;
						for(int d=0;d<j;d++)
						{
							if(node==temp_best[d])
							{
								j--;
								ins=1;
							}
						}
						if(ins==0)
						{
							temp_best[j]=node;
							printf("%d - ",node);
						}	
					}
					printf("%d",temp_best[size]);
					int new_cost=cost_of_path(size,temp_best,path_matrix);
					printf("\tCost : %d",new_cost);
					
					if(new_cost<min_cost)
					{
						min_cost=new_cost;
						for(int j=0;j<size+1;j++)
						{
							best[k][j]=temp_best[j];
						}
					}
					k++;
				}
			}
		}
		n_bats--;
	}
	int best_costs[3];
	for(int i=0;i<3;++i)
	{
		printf("\nBest %d = \t-",i+1);
		for(int j=0;j<size+1;j++)
		{
			printf("%d - ",best[i][j]);
		}
		best_costs[i]=cost_of_path(size,best[i],path_matrix);
		printf("\t Cost of this path : %d",best_costs[i]);
	}
	
	//Overall best
	int min1 = best_costs[0];
	int min2 = best_costs[1];
	int min3 = best_costs[2];
	
	if(min1<min2 && min1<min3)
	{
		min_cost=min1;
		min_path_idx=0;
	}
	else if(min2<min1 && min2<min3)
	{
		min_cost=min2;
		min_path_idx=1;
	}
	else if(min3<min1 && min3<min2)
	{
		min_cost=min3;
		min_path_idx=2;
	}
	printf("\nMin Cost path - %d\tPath : ",min_cost);
	for(int i=0;i<size+1;i++)
	{
		printf("%d - ",best[min_path_idx][i]);
	}
			/*
			while(k<1)
			{
				for(int p=0;p<(size+1)/2;p++)
				{
					sample_1[p]=best[k][p];
					sample_2[p]=best[k+1][p];
				}
				//Inheritance operator; keeping half of the path same and swapping rest cities to find a new child path; then comparing the results with parent
				for(int p=(size+1)/2;p<size;p++)
				{
					for(int c=0;c<p;c++)
					{
						if(best[k+1][p]==sample_1[c])
							flag1=1;
					}
					if(flag1==0)
					{
						sample_1[p] = best[k+1][p];
					}
					else if(flag1==1)
					{
						temp = best[k][p];
						for(int c=0;c<p;c++)
						{
							if(temp==sample_1[c])
							{
								printf("\nNode already exists\n");
								cont = 1;
								break;
							}
						}
					}
					
					
					for(int c=0;c<p;c++)
					{
						if(best[k][p]==sample_2[c])
							flag2=1;
					}
					if(flag2==0)
					{
						sample_2[p] = best[k][p];
					}
					else if(flag2==1)
					{
						temp = best[k+1][p];
						for(int c=0;c<p;c++)
						{
							if(temp==sample_2[c])
							{
								printf("\nNode already exists\n");
								cont = 1;
								break;
							}
						}
					}
					
					
				}
				if(cont==1)
					continue;
				sample_1[size] = best[k][size];
				sample_2[size] = best[k+1][size];
				int cost1 = cost_of_path(size,sample_1,path_matrix);
				int cost2 = cost_of_path(size,sample_2,path_matrix);
				if(cost1<cost2)
				{
					int min_of_two = cost1;
					if(min_of_two<min_cost)
					{
						printf("\n\nSample1 is now best - ");
						min_cost=min_of_two;
						for(int z=0;z<size+1;z++)
						{
							best_sol[z] = sample_1[z];
							printf("%d - ",best_sol[z]);
						}
					}
				}
				else if(cost1>cost2)
				{
					int min_of_two = cost2;
					if(min_of_two<min_cost)
					{
						printf("\n\nSample2 is now best - ");
						min_cost=min_of_two;
						for(int z=0;z<size+1;z++)
						{
							best_sol[z] = sample_2[z];
							printf("%d - ",best_sol[z]);
						}
					}
				}
				else
				{
					min_cost = min_cost;
					fitness = (float)min_cost/VAL;
					for(int z=0;z<size+1;z++)
					{	
						best_sol[z] = best[0][z];
					}
				}
				k++;
			}
			//if condition
		}
	}
	printf("\nBest SOl : ");
	for(int i=0;i<size+1;i++)
	{
		printf("%d - ",best_sol[i]);
	}
	printf("\nCost : %d",min_cost);*/
	
/*	for(int j=0;j<n_bats;++j)
	{	
		int new_path[size+1];
		random_path_generator(size, new_path);
		int new_cost = cost_of_path(size, new_path, path_matrix);
		if(new_cost<min_cost)
		{
			min_cost = new_cost;
			for(int k=0;k<size+1;k++)
			{
				best[k] = new_path[k];
				printf("%d - ",best[k]);
			}
			n_iter++;
		}
		//else
		
	}
	printf("Total iterations done : %d\n\n",n_iter);
	if(n_iter==0)
	{
		printf("------------------------------------------\nNo new path found...\n");
		for(int k=0;k<size+1;k++)
		{
			printf("%d - ",best[k]);
		}
		printf("\n Minimum Cost - %d",min_cost);
		
	}
	else
	{
		printf("-------------------------------------------\nFound new Route: \n");
		for(int k=0;k<size+1;k++)
		{
			printf("%d - ",best[k]);
		}
		printf("\n Minimum Cost - %d",min_cost);
	}*/
}

//Objective function
void objective()
{
	int size;
	
	
	printf("Enter number of cities : ");
	scanf("%d",&size);
	
	/*Enter the cost of travelling between cities
		1	2	3	4	5	 6
	1	0   11  7   6   8   14 
	2	11  0   7   9   12  13 
	3	7   7   0   3   7    8 
	4	6   9   3   0   4    8
	5	8   12  7   4   0   10 
	6	14  13  8   8  10    0 
	*/
	//cant declare with size as size was not given at that time
	int path_matrix[size][size];	//storing path matrix
	int pattern[size+1];		//storing a randomly created walk
	int random_paths[10][size+1];
	
	enter_path_matrix(size,path_matrix);	//funtion to take input of path
	
	print_path_matrix(size,path_matrix);	//function to print the path matrix
	
	int cost[10];
	double freq[10];
	int idx[10];
	for(int i=0;i<10;i++)
	{
		cost[i]=0;
		random_path_generator(size,pattern);	//generate a random path
		for(int j=0;j<size+1;++j)
			random_paths[i][j]=pattern[j];
		cost[i] += cost_of_path(size,pattern,path_matrix);
		printf("\t\tCost = %d",cost[i]);	
	}
	
	for(int i=0;i<10;i++)
	{
		freq[i] = (float)cost[i]/VAL;
		idx[i]=i;
		printf("\n%lf",freq[i]);
	}
	
	BatAlgorithm(size,10,freq,random_paths,cost,path_matrix,idx);
	//0 10 20 15 30 10 0 5 20 40 20 5 0 15 5 15 20 15 0 10 30 40 5 10 0
	//0 11 7 6 8 14 11 0 7 9 12 13 7 7 0 3 7 8 6 9 3 0 4 8 8 12 7 4 0 10 14 13 8 8 10 0
	//0 10 6 2 15 17 8 10 0 5 12 3 9 6 6 5 0 1 5 3 5 2 12 1 0 4 2 4 15 3 5 4 0 1 3 17 9 3 2 1 0 2 8 6 5 4 3 2 0
	
}

int main()
{
	srand(time(0));
	objective();
}
