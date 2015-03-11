/*
 * minimax.c
 *
 *  Created on: Oct 17, 2014
 *      Author: tchambs
 */

#include "minimax.h"

#define TOTALSQUARES 9
#define HIGHSCORE 100
#define LOWSCORE -100
void printBoard(minimax_board_t* board); //prints out the current board
int16_t movesAvailable(minimax_board_t* board); //checks if moves are available
int minimax(minimax_board_t* board, bool player); //minimax algorithm
minimax_score_t score = 0; //initialize score to 0
int depth = 0; //initialize depth
minimax_move_t choice;

void minimax_computeNextMove(minimax_board_t* board, bool player, uint8_t* row, uint8_t* column)
{
	depth = -1; //start the depth at -1, so when we increment in minimax it begins at 0
	printBoard(board); //print current board to the console
	minimax(board, player); //recursive minimax call
	*row = choice.row; //assign the row to the final choice
	*column = choice.column; //assign the column to the final choice
}

int minimax(minimax_board_t* board, bool player)
{
	depth++; //increment depth at the beginning of minimax
	minimax_move_t moves[TOTALSQUARES]; //moves array uses same index as scores
	minimax_score_t scores[TOTALSQUARES]; //scores array uses same index as moves

	if (minimax_isGameOver(minimax_computeBoardScore(board, player))) //recursion base case: game is ended
	{
		depth--; //we are returning, so decrement our depth
		return minimax_computeBoardScore(board, !player); //evaluate board based upon PREVIOUS player/opponent.
	}
		int index = 0; //keep track of number of scores/moves in our arrays
		for (int i = 0; i < MINIMAX_BOARD_ROWS; i++) //for each row
		{
			for (int j = 0; j < MINIMAX_BOARD_COLUMNS; j++) //for each column
			{
				if (board->squares[i][j] == MINIMAX_EMPTY_SQUARE) //find empty square
				{
					if (player) //Assign the square to the player
					{
						board->squares[i][j] = MINIMAX_PLAYER_SQUARE;
					}

					else //Assign the square to the opponent
					{
						board->squares[i][j] = MINIMAX_OPPONENT_SQUARE;
					}

					score = minimax(board, !player);
					scores[index] = score; 	//add score to move-score table
					moves[index].row = i;	//add move row to move-score table
					moves[index].column = j; //add move column to move-score table

					index++;

					// Undo the change to the board (return the square to empty).
					board->squares[i][j] = MINIMAX_EMPTY_SQUARE;
				}
			}
		}
		// Once you get here, the loop has completed and so you have all of the scores computed
		// in the move-score table for boards at this level.
		// Now you need to return the score depending upon whether you are computing min or max.

		int loopIndex = index; //use loopindex for checking my moves/scores table
		int16_t tempIndex = 0; //temp index is the index of high or low
		if (player)
		{
			int16_t highScore = LOWSCORE; //initialize it to something LOW
			for (int i = 0; i < loopIndex; i++)
			{
				if (scores[i] > highScore)
				{
					highScore = scores[i];
					tempIndex = i;
				}
			}
			choice = moves[tempIndex]; 	//get the move with the highest score in the move-score table.
			score = scores[tempIndex]; 	//highest score in the move-score table.
		}
		else
		{
			int16_t lowScore = HIGHSCORE; //initialize it to something HIGH
			for (int i = 0; i < loopIndex; i++)
			{
				if (scores[i] < lowScore)
				{
					lowScore = scores[i];
					tempIndex = i;
				}
			}
			choice = moves[tempIndex]; 	//get the move with the lowest score in the move-score table.
			score = scores[tempIndex]; 	//lowest score in the move-score table.
		}
		depth--; //decrement depth because we are returning
		return score;
	}

// Determine that the game is over by looking at the score.
bool minimax_isGameOver(minimax_score_t score)
{
	if (score == MINIMAX_NOT_ENDGAME)
		return false;
	else
		return true;
}

// Returns the score of the board, based upon the player.
int16_t minimax_computeBoardScore(minimax_board_t* board, bool player)
{
	//player
//	if (player)
//	{
		//row based wins
		for (int i = 0; i < MINIMAX_BOARD_ROWS; i++)
		{
			if ((board->squares[i][0] == MINIMAX_PLAYER_SQUARE) &&
				(board->squares[i][1] == MINIMAX_PLAYER_SQUARE) &&
				(board->squares[i][2] == MINIMAX_PLAYER_SQUARE))
			{
				return MINIMAX_PLAYER_WINNING_SCORE;
			}
			else if ((board->squares[i][0] == MINIMAX_OPPONENT_SQUARE) &&
					 (board->squares[i][1] == MINIMAX_OPPONENT_SQUARE) &&
					 (board->squares[i][2] == MINIMAX_OPPONENT_SQUARE))
			{
				return MINIMAX_OPPONENT_WINNING_SCORE;
			}
		}

		//column based wins
		for (int j = 0; j < MINIMAX_BOARD_COLUMNS; j++)
		{
			if ((board->squares[0][j] == MINIMAX_PLAYER_SQUARE) &&
				(board->squares[1][j] == MINIMAX_PLAYER_SQUARE) &&
				(board->squares[2][j] == MINIMAX_PLAYER_SQUARE))
			{
				return MINIMAX_PLAYER_WINNING_SCORE;
			}
			else if ((board->squares[0][j] == MINIMAX_OPPONENT_SQUARE) &&
					 (board->squares[1][j] == MINIMAX_OPPONENT_SQUARE) &&
					 (board->squares[2][j] == MINIMAX_OPPONENT_SQUARE))
			{
				return MINIMAX_OPPONENT_WINNING_SCORE;
			}
		}

		//diagonal based wins
		if ((board->squares[0][0] == MINIMAX_PLAYER_SQUARE) &&
			(board->squares[1][1] == MINIMAX_PLAYER_SQUARE) &&
			(board->squares[2][2] == MINIMAX_PLAYER_SQUARE))
		{
			return MINIMAX_PLAYER_WINNING_SCORE;
		}
		else if((board->squares[0][0] == MINIMAX_OPPONENT_SQUARE) &&
				(board->squares[1][1] == MINIMAX_OPPONENT_SQUARE) &&
				(board->squares[2][2] == MINIMAX_OPPONENT_SQUARE))
		{
			return MINIMAX_OPPONENT_WINNING_SCORE;
		}
		else if((board->squares[0][2] == MINIMAX_PLAYER_SQUARE) &&
				(board->squares[1][1] == MINIMAX_PLAYER_SQUARE) &&
				(board->squares[2][0] == MINIMAX_PLAYER_SQUARE))
		{
			return MINIMAX_PLAYER_WINNING_SCORE;
		}
		else if((board->squares[0][2] == MINIMAX_OPPONENT_SQUARE) &&
				(board->squares[1][1] == MINIMAX_OPPONENT_SQUARE) &&
				(board->squares[2][0] == MINIMAX_OPPONENT_SQUARE))
		{
			return MINIMAX_OPPONENT_WINNING_SCORE;
		}

	//find out if we at least have a draw
	int16_t filledSquares = 0;
	for (int i = 0; i < MINIMAX_BOARD_ROWS; i++) //for every row
		{
			for (int j = 0; j < MINIMAX_BOARD_COLUMNS; j++) //for every column
			{
				if (board->squares[i][j] != MINIMAX_EMPTY_SQUARE) //check if square is not empty
				{
					filledSquares++;
				}
			}
		}
	if (filledSquares == TOTALSQUARES) //self explanatory
	{
		return MINIMAX_DRAW_SCORE;
	}

	//if we made it here, there are no victories AND its not a draw
	return MINIMAX_NOT_ENDGAME;
}

// Init the board to all empty squares.
void minimax_initBoard(minimax_board_t* board)
{
	for (int i = 0; i < MINIMAX_BOARD_ROWS; i++)
	{
		for (int j = 0; j < MINIMAX_BOARD_COLUMNS; j++)
		{
			board->squares[i][j] = MINIMAX_EMPTY_SQUARE;
		}
	}
}

void printBoard(minimax_board_t* board)
{
	printf("\r\n");
	for (int i = 0; i < MINIMAX_BOARD_ROWS; i++) //every row
	{
		for (int j = 0; j < MINIMAX_BOARD_COLUMNS; j++) //every column
		{
			if (board->squares[i][j] == MINIMAX_PLAYER_SQUARE)
				printf("X ");
			else if (board->squares[i][j] == MINIMAX_OPPONENT_SQUARE)
				printf("O ");
			else if (board->squares[i][j] == MINIMAX_EMPTY_SQUARE)
				printf("- ");
			else //error case, should never be hit
				printf("%d ", board->squares[i][j]);
		}
		printf("\r\n");
	}
}

//helper function I didn't end up using. This function
//checks to see if there are empty spaces on the board
//it could be used for optimization
int16_t movesAvailable(minimax_board_t* board)
{
	int16_t emptySquares = 0;
	for (int i = 0; i < MINIMAX_BOARD_ROWS; i++)
	{
		for (int j = 0; j < MINIMAX_BOARD_COLUMNS; j++)
		{
			if (board->squares[i][j] == MINIMAX_EMPTY_SQUARE)
				emptySquares++;
		}
	}
	return emptySquares;
}
