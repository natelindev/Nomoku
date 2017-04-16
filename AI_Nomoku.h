//
//  AI_Nomoku.h
//  Nomoku
//
//

#ifndef AI_Nomoku_h
#define AI_Nomoku_h

class AI_Nomoku
{
public:
	//每次搜索 对应 下一步棋
	void alpha_beta_search();

	//用于在控制台输出 棋盘的状态
	void visualize();

	//构造函数
	AI_Nomoku();
	//析构函数
	~AI_Nomoku();
};

#endif /* AI_Nomoku_h */
