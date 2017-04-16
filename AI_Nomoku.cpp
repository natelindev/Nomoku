//
//  AI_Nomoku.cpp
//  Nomoku
//
//  Main AI structure
//  Where alpha-beta & minmax search was implemented
//

#include "AI_Nomoku.h"
#include "AI_Wrapper.h"
#include "Logger.h"
#include "Board_controller.h"
#include "pisqpipe.h"
#include "ANN.h"
#include <vector>
#include <thread>

AI_Nomoku::AI_Nomoku(){}

AI_Nomoku::~AI_Nomoku(){}

void AI_Nomoku::alpha_beta_search()
{
	if (bc->is_empty())
	{
		//1.whole empty board -> place on center
		if (bc->getWidth() >= 5 && bc->getHeight() >= 5)
		{
			do_mymove(bc->getHeight() / 2, bc->getWidth() / 2);
		}
		else{
			logEvent(LOG_ERROR, "The board size is too small for Gomoku!\n");
		}
	}
	else
	{
		//2.has some dots -> expand from current dots
		int min = INT_MAX, max = INT_MIN;
		int max_x = INT_MAX, max_y = INT_MAX;
		int min_x = INT_MIN, min_y = INT_MIN;
		int current_value;
		std::vector<std::pair<int, int>> v_expand_a;

		//alpha search
		//find alpha expandable blocks (my possible moves)
		bc->mark_edge_blocks();
		//collect marked blocks
		for (int i = 0; i < bc->getWidth(); ++i)
		{
			for (int j = 0; j < bc->getHeight(); ++j)
			{
				//in order to prevent redundant,we use B_TO_EXPAND mark
				if (bc->board[i][j] == B_TO_EXPAND) {
					std::pair<int, int> temp;
					temp.first = i;
					temp.second = j;
					v_expand_a.push_back(temp);
				}
			}
		}

		for (std::pair<int, int> current_expand_a : v_expand_a)
		{
			bc->mark_edge_blocks();
			//expand each one of them
			std::vector<std::pair<int, int>> v_expand_b;
			int x = current_expand_a.first, y = current_expand_a.second;
			bc->expand(x, y);

			//temporary change, will roll back
			bc->board[x][y] = B_MY_MOVE;

			//beta search
			//get beta expandable blocks(oppo possible moves)
			for (int i = 0; i < bc->getWidth(); ++i)
			{
				for (int j = 0; j < bc->getHeight(); ++j)
				{
					if (bc->board[i][j] == B_TO_EXPAND) {
						std::pair<int, int> temp;
						temp.first = i;
						temp.second = j;
						v_expand_b.push_back(temp);
					}
				}
			}

			bc->clear_expand_marks();

			//evaluate part
			min = INT_MAX;
			for (std::pair<int, int> current_expand_b : v_expand_b)
			{
				int x = current_expand_b.first, y = current_expand_b.second;
				//temporary change, will roll back
				bc->board[x][y] = B_OP_MOVE;
				//get the current board state

				//evaluate
				current_value = outputevaluation();
				if (current_value < min) {
					min = current_value;
					min_x = x, min_y = y;
				}

				//roll back
				bc->board[x][y] = B_FREE;

				//minmax pruning
				if (min < max)
					break;
			}
			if (min > max) {
				max = min;
				max_x = min_x, max_y = min_y;
			}

			//roll back
			bc->board[x][y] = B_FREE;
		}
		do_mymove(max_x, max_y);
	}
	bc->clear_expand_marks();
}