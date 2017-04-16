//
//	ANN.cpp
//	Nomoku
//
//	输入层(width*height)个神经元
//	中间层1/2*(width*height)个神经元
//	输出层1个神经元
//一共3/2(width*height)+1个神经元
//	一共(1 / 2)*width*height*width*height+(1 / 2)*width*height个权值
//	itoa并不是一个标准的C函数，它是Windows特有的，如果要写跨平台的程序，请用sprintf。是Windows平台下扩展的，标准库中有sprintf，功能比这个更强，用法跟printf类似：
//	
//	char str[255];
//	sprintf(str, "%x", 100); //将100转为16进制表示的字符串
//
//

#include "ANN.h"
#include "pisqpipe.h"
#include "AI_Wrapper.h"
#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>

int temp_board[MAX_BOARD][MAX_BOARD];						//全局变量棋盘状态

int mutex_ans = 0;											//0:未使用，1：正在被占用
int semaphore_weight = 0;									//表示是否有可用神经网络0：表示没有，1表示有
int ans = 0;												//0：没有结果，1：胜，-1输。

char file_path[1000] = "E:/";							//文件路径，默认E盘
int input_number = width*height;						//输入层神经元个数
int mid_number = (1.0 / 2)*width*height;				//中间层神经元个数
int output_number = 1;								//输出层神经元个数
int weight_number = input_number*mid_number + mid_number*output_number;//所有权值个数 输入*中间+中间*输出
int neure_number = input_number + mid_number + output_number;//所有神经元个数 输入+中间+输出
int winp_denominator = 10;							//胜率的分母

int *neure = new int[neure_number];							//神经元序列
short *weight = new short[weight_number];					//权值序列

int get_result(){
	int temp = 0;
	for (;;){
		if (mutex_ans == 0){
			mutex_ans = 1;
			if (ans == 0);
			else if (ans == 1)temp = ans;
			else if (ans == -1) temp = ans;
			mutex_ans = 0;
		}
		if (temp != 0) break;
	}
	return temp;
};

void init_ANN_N(int(*board)[MAX_BOARD], int *neure){//初始化神经元
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			neure[i*height + j] = board[i][j];
		}
	}

	memset(&(neure[width*height]), 0, sizeof(int)*(mid_number + output_number));
}

void init_ANN_W(short *weight){//初始化神经网权值
	srand((unsigned)time(NULL));
	for (int i = 0; i < weight_number; i++){
		weight[i] = rand() % 0xffff;
	}
}

//神将网络分配内存交给GA搜素算法部分
void init_ANN(int *neure, short *weight){//初始化ANN网络获得随机权值，输入神经元赋值，中间及输出神经元置0

	//初始化前input_number个神经元值为棋盘状态
	init_ANN_N(temp_board, neure);
	init_ANN_W(weight);
}

int Update_ANN(int *neure, short *weight){//根据权值更新中间，输出神经元值
	//整合函数=加权求和g(x)=sum(wi*ai)
	//激活函数=线性函数f(x)=x
	for (int i = 0; i < mid_number; i++){//计算中间层神经元值
		//neure[input_number+i]=sum(wi*ai)
		for (int j = 0; j < input_number; j++){
			//每一个输入神经元J*乘上到中间神经元I的权重   
			neure[input_number + i] += neure[j] * weight[j*mid_number + i];
		}
	}
	for (int i = 0; i <= mid_number; i++){//计算输出层神经元值
		//nneure[input_number+mid_number] =sum(wi*ai)
		neure[input_number + mid_number] += neure[input_number + i] * weight[input_number*mid_number + i];
	}
	return neure[input_number + mid_number];
}
//
double winrateCalc(std::vector<short> *nowWeight){//用来返回胜率
	semaphore_weight = 1;
	for (int i = 0; i < weight_number; i++){
		weight[i] = (*nowWeight)[i];
	}
	//计算一局的函数
	int counts;
	int win_times = 0;
	for (counts = 0; counts < winp_denominator; counts++){
		if (get_result() == 1){
			win_times++;
		}
	}
	semaphore_weight = 0;
	return (win_times*1.0) / counts;
}

int outputevaluation(){//给AB剪枝用来返回评价值
	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){
			temp_board[i][j] = bc->board[i][j];
		}
	}
	init_ANN_N(temp_board, neure);
	return Update_ANN(neure, weight);
}




void FOutANN(std::vector<short> *nowWeight){//神将网络权值输出到width_height.txt中
	for (int i = 0; i < weight_number; i++){
		weight[i] = (*nowWeight)[i];
	}
	//计算一局的函数
	
	char swidth[10], sheight[10];
	char underline[2] = "_";
	char filename[20];
	sprintf(swidth, "%d", width);
	sprintf(sheight, "%d", height);
	strcpy(filename, file_path);
	strcat(filename, swidth);
	strcat(filename, "_");
	strcat(filename, sheight);
	strcat(filename, ".txt");
	std::ofstream file;

	file.open(filename);
	for (int i = 0; i < weight_number; i++){
		file << weight[i] << '\n';
	}
	file.close();
}
void FINANN(std::vector<short> *nowWeight){//神将网络权值从width_height.txt输入
	char swidth[10], sheight[10];
	char underline[2] = "_";
	char filename[20];
	sprintf(swidth, "%d", width);
	sprintf(sheight, "%d", height);
	strcpy(filename, file_path);
	strcat(filename, swidth);
	strcat(filename, "_");
	strcat(filename, sheight);
	strcat(filename, ".txt");
	std::ifstream file;

	file.open(filename);
	if (file.is_open() == 0){
		init_ANN_W(weight);
		for (int i = 0; i < weight_number; i++){
			(*nowWeight)[i] = weight[i];
		}
	}
	else {
		for (int i = 0; i < weight_number; i++){
			file >> weight[i];
			(*nowWeight)[i] = weight[i];
		}
	}

	file.close();
	//计算一局的函数
}
