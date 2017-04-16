//
//  Board_controller.cpp
//  Nomoku
//
//  This class stores board infomation
//  and apply operations to the board
//

#include <iostream>
#include "pisqpipe.h"
#include "Logger.h"
#include "Board_controller.h"

Board_controller::Board_controller(int w, int h) :width(w), height(h), count(0), i_w(false), h_r(false)
{
	if (width<5 || height < 5){
		pipeOut("ERROR Invalid size of the board");
		return;
	}
	if (width > MAX_BOARD_WIDTH){
		pipeOut("ERROR Maximal board width is %d", MAX_BOARD_WIDTH);
		return;
	}
	if (height > MAX_BOARD_HEIGHT){
		pipeOut("ERROR Maximal board height is %d", MAX_BOARD_HEIGHT);
		return;
	}

	//allocate memory
	board = new int*[width];
	for (int j = 0; j < width; ++j) {
		board[j] = new int[height];
	}
}

Board_controller::~Board_controller()
{
	//free memory
	for (int j = 0; j < width; ++j) {
		delete[] board[j];
	}
	delete[] board;
}

void Board_controller::visualize()
{
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			std::cout << board[i][j];
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

bool Board_controller::is_empty()
{
	return (count == 0);
}

bool Board_controller::is_free(int x, int y)
{
	return (board[x][y] == B_FREE);
}

bool Board_controller::is_in_range(int x,int y)
{
	return (x >= 0 && y >= 0 && x < width && y < height);
}

bool Board_controller::have_result()
{
	return h_r;
}

bool Board_controller::is_winner()
{
	return i_w;
}

void Board_controller::move_self(int x, int y)
{
	if (is_in_range(x,y) && is_free(x, y))
	{
		++count;
		board[x][y] = B_MY_MOVE;
	}
	else{
		pipeOut("ERROR my move [%d,%d]", x, y);
		logEvent(LOG_ERROR, "Invalid move at %d,%d, not empty or out of range\n", x, y);
	}
}

void Board_controller::move_oppo(int x, int y)
{
	if (is_in_range(x, y) && is_free(x, y))
	{
		++count;
		board[x][y] = B_OP_MOVE;
	}
	else{
		pipeOut("ERROR opponents's move [%d,%d]", x, y);
		logEvent(LOG_ERROR, "Invalid move at %d,%d, not empty or out of range\n", x, y);
	}
}

void Board_controller::move_win(int x, int y)
{
	h_r = true;
	if (is_in_range(x, y) && is_free(x, y))
	{
		board[x][y] = B_WIN_MOVE;
	}
	else{
		pipeOut("ERROR winning move [%d,%d]", x, y);
		logEvent(LOG_ERROR, "Invalid winning move at %d,%d, already empty or out of range\n", x, y);
	}
}

int Board_controller::take_back(int x, int y)
{
	if (is_in_range(x, y) && !is_free(x, y))
	{
		--count;
		board[x][y] = B_FREE;
		return 0;
	}
	else
	{
		logEvent(LOG_ERROR, "Invalid takeback at %d,%d, already empty or out of range\n", x, y);
		return 2;
	}
}

int Board_controller::getWidth()
{
	return width;
}

int Board_controller::getHeight()
{
	return height;
}

void Board_controller::setWidth(int w)
{
	logEvent(LOG_WARNING, "At current version,change the board size on the fly is not supported\n");
	width = w;
}

void Board_controller::setHeight(int h)
{
	logEvent(LOG_WARNING, "At current version,change the board size on the fly is not supported\n");
	height = h;
}

void Board_controller::mark_edge_blocks()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			if (board[i][j] == B_MY_MOVE || board[i][j] == B_OP_MOVE)
			{
				expand(i, j);
			}
		}
	}
}

void Board_controller::expand(int x, int y)
{
	int up_tag, down_tag, left_tag, right_tag;
	//if tag is ture then it's a valid move, otherwise it's invalid
	up_tag = (x - 1 >= 0);
	left_tag = (y - 1 >= 0);
	down_tag = (x + 1 < width);
	right_tag = (y + 1 < height);

	//expand the blocks at the edge of existing moves
	if (up_tag && is_free(x - 1, y))
		board[x - 1][y] = B_TO_EXPAND;
	if (down_tag && is_free(x + 1, y))
		board[x + 1][y] = B_TO_EXPAND;
	if (left_tag && is_free(x, y - 1))
		board[x][y - 1] = B_TO_EXPAND;
	if (right_tag && is_free(x, y + 1))
		board[x][y + 1] = B_TO_EXPAND;
	if (up_tag && left_tag && is_free(x - 1, y - 1))
		board[x - 1][y - 1] = B_TO_EXPAND;
	if (up_tag && right_tag && is_free(x - 1, y + 1))
		board[x - 1][y + 1] = B_TO_EXPAND;
	if (down_tag && left_tag && is_free(x + 1, y - 1))
		board[x + 1][y - 1] = B_TO_EXPAND;
	if (down_tag && right_tag && is_free(x + 1, y + 1))
		board[x + 1][y + 1] = B_TO_EXPAND;
}

void Board_controller::clear_expand_marks()
{
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			if (board[i][j] == B_TO_EXPAND) {
				board[i][j] = B_FREE;
			}
		}
	}
}

void Board_controller::clear_board()
{
	count = 0;
	i_w = false;
	h_r = false;

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			board[i][j] = B_FREE;
		}
	}
}
