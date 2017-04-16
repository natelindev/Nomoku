//
//  Board_controller.h
//  Nomoku
//
//  This is the controller of the board
//  Due to the strange piskvork developer's preference
//  We use height as x-axis
//         width  as y-axis (which is fortran style arrays)
//

#ifndef Board_controller_h
#define Board_controller_h

#ifndef MAX_BOARD_WIDTH
#define MAX_BOARD_WIDTH 100
#endif

#ifndef MAX_BOARD_HEIGHT
#define MAX_BOARD_HEIGHT 100
#endif

//棋盘标记
enum{ B_FREE, B_MY_MOVE, B_OP_MOVE, B_WIN_MOVE, B_TO_EXPAND};

class Board_controller 
{
	int width;
	int height;
	int count;
	bool i_w;
	bool h_r;

public:
	//存储棋盘
	int **board;

	//Constructor Destructor
	Board_controller(int w, int h);
	~Board_controller();

	//getter setter
	int getWidth();
	int getHeight();
	void setWidth(int w);
	void setHeight(int h);

	//判断整个棋盘是否为空
	bool is_empty();
	//判断指定的位置xy 是否为空
	bool is_free(int x, int y);
	//判断指定的位置xy 是否在棋盘范围内
	bool is_in_range(int x, int y);
	bool have_result();
	bool is_winner();

	//我方走的一步
	void move_self(int x, int y);
	//对方走的一步
	void move_oppo(int x, int y);
	//最终获胜步的设定
	void move_win(int x, int y);
	//撤销某一步用的函数
	int take_back(int x, int y);

	//棋盘状态输出到控制台
	void visualize();
	//用于alpha-beta搜索，标记当前可能要走的步
	void mark_edge_blocks();
	//用于清理多余的扩展标记
	void clear_expand_marks();
	//从x,y这一个棋盘位置进行扩展 ab搜索用
	void expand(int x, int y);
	//下完一盘棋之后，清理棋盘
	void clear_board();

};

#endif /* Board_controller_h */
