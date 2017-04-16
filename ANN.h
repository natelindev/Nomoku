#ifndef _ANN_H
#define _ANN_H
#include <vector>

#ifndef MAX_BOARD
#define MAX_BOARD 100
#endif // !MAX_BOARD

extern int mutex_ans;											//0:未使用，1：正在被占用
extern int semaphore_weight;									//表示是否有可用神经网络0：表示没有，1表示有
extern int ans;

//特殊情况下可使用MAX_BOARD代替width和height神经网络
void init_ANN(int *neure, short *weight);					//初始化ANN网络获得随机权值，输入神经元赋值，中间及输出神经元置0
int Update_ANN(int *neure, short *weight);					//根据权值更新中间，输出神经元值
void init_ANN_N(int(*board)[MAX_BOARD], int *neure);		//初始化神经元
void init_ANN_W(short *weight);								//初始化神经网权值
int get_result();											//获得一个次棋局结果 0：输，1：赢
double winrateCalc(std::vector<short> *nowWeight);			//用来返回胜率
int outputevaluation();										//给AB剪枝用来返回评价值
void FINANN(std::vector<short> *nowWeight);					//神将网络权值从width_height.txt输入
void FOutANN(std::vector<short> *nowWeight);				//神将网络权值输出到width_height.txt中

#endif