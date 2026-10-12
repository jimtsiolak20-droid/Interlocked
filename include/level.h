#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256
#define MAX_LEVELS 32

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

const char* AVAILABLE_STRUCTURES = "UWSOHTRGBCMY";

typedef struct Board
{
	int player_x, player_y;
	int goal_x, goal_y;
	/*
		0 through LEVEL_WIDTH (or LEVEL_HEIGHT) - 1
		-1 for unspecified (should not be used)
	*/

	char mboard[BOARD_WIDTH * BOARD_HEIGHT];
	/*
		U: Unspecified (should not be used)
		W: Wall
		S: wall that is Secretly ground (should not be used by main levels)
		O: grOund
		H: Hole
		T: Teleport orb that bridges opposite sides
		R: Red block
		G: Green block
		B: Blue block
		C: Cyan block
		M: Mangenta block
		Y: Yellow block (should not be used, confused with the color of the goal)
	*/

} Board;

typedef struct Level
{
	char name[BUFFER_SIZE];
	char certificate[BUFFER_SIZE]; //proof that the level is possible to beat (solution)
	unsigned char cosmetics[4];
	/*
		Cosmetic choices when rendering:
		Byte 0 corresponds to wall colors
		The others I don't know yet, reserved for future purposes
	*/
	Board board;

} Level;

typedef struct LevelPack
{
	char name[BUFFER_SIZE];
	int level_count;
	Level levels[MAX_LEVELS];

} LevelPack;

char* GetSquare(Board* board, int x, int y)
{
	if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
	{
		return NULL;
	}

	return &(board->mboard[x + BOARD_WIDTH * y]);
}

char SafeGetSquare(Board* board, int x, int y)
{
	if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
	{
		return 'U';
	}

	return board->mboard[x + BOARD_WIDTH * y];
}

void InitBoard(Board* board)
{
	board->player_x = -1;
	board->player_y = -1;

	board->goal_x = -1;
	board->goal_y = -1;

	memset(board->mboard, 'U', BOARD_WIDTH * BOARD_HEIGHT);
}

char IsBoardValid(Board* board)
{
	if (!(board->player_x >= 0 && board->player_x < BOARD_WIDTH && board->player_y >= 0 && board->player_y < BOARD_HEIGHT && board->goal_x >= 0 && board->goal_x < BOARD_WIDTH && board->goal_y >= 0 && board->goal_y < BOARD_HEIGHT))
	{
		return 0;
	}

	for (int i = 0; i < sizeof(board->mboard); i++)
	{
		if (board->mboard[i] == 'U' || strchr(AVAILABLE_STRUCTURES, board->mboard[i]) == NULL)
		{
			return 0;
		}
	}

	return 1;
}

char IsWon(Board* board)
{
	if (board->player_x == board->goal_x && board->player_y == board->goal_y)
	{
		return 1;
	}

	return 0;
}

char WhatIsAheadWithTeleport(Board* board, int x, int y, int dir_x, int dir_y, int* out_x, int* out_y)
{
	x += dir_x;
	y += dir_y;
	while (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT)
	{
		char ahead = *GetSquare(board, x, y);
		if (ahead != 'T')
		{
			*out_x = x;
			*out_y = y;
			return ahead;
		}

		x += dir_x;
		y += dir_y;
	}

	*out_x = -1;
	*out_y = -1;
	return 'U'; //out of bounds
}

char SimulateMove(Board* board, char move) //changes board
{
	if (!IsBoardValid(board))
	{
		return 0;
	}

	/*
		U: Up
		D: Down
		L: Left
		R: Right
	*/

	int x = board->player_x;
	int y = board->player_y;
	
	int out_x;
	int out_y;

	int dir_x = 0;
	int dir_y = 0;
	switch (move)
	{
	case 'U':
	{
		dir_y = -1;
		break;
	}
	case 'D':
	{
		dir_y = 1;
		break;
	}
	case 'L':
	{
		dir_x = -1;
		break;
	}
	case 'R':
	{
		dir_x = 1;
		break;
	}
	default:
	{
		return 0;
	}
	}

	char ahead_player = WhatIsAheadWithTeleport(board, x, y, dir_x, dir_y, &out_x, &out_y);

	if (ahead_player == 'U' || ahead_player == 'W' || ahead_player == 'H') //out of bounds or wall or hole ahead
	{
		return 0;
	}
	else if (ahead_player == 'O' || ahead_player == 'S') //ground ahead
	{
		//the code that moves the player happens bellow
	}
	else //color ahead
	{
		//check if every block of the same color can move in that direction (pass 1)
		for (int i = 0; i < BOARD_HEIGHT; i++)
		{
			for (int j = 0; j < BOARD_WIDTH; j++)
			{
				if (*GetSquare(board, j, i) == ahead_player) //same color
				{
					int unused;
					char ahead_block = WhatIsAheadWithTeleport(board, j, i, dir_x, dir_y, &unused, &unused);
					if (ahead_block != ahead_player && ahead_block != 'O' && ahead_block != 'S' && ahead_block != 'H') //cannot move in that direction (if it is a hole then the block fill the hole)
					{
						return 0;
					}
				}
			}
		}

		//apply to the board (pass 2)
		int start_i = 0;
		int start_j = 0;
		int step_i = 1;
		int step_j = 1;

		//flip order to not get in the way
		if (dir_y == 1)
		{
			start_i = BOARD_HEIGHT - 1;
			step_i = -1;
		}
		if (dir_x == 1)
		{
			start_j = BOARD_WIDTH - 1;
			step_j = -1;
		}

		for (int i = start_i; i >= 0 && i < BOARD_HEIGHT; i += step_i)
		{
			for (int j = start_j; j >= 0 && j < BOARD_WIDTH; j += step_j)
			{
				if (*GetSquare(board, j, i) == ahead_player) //same color
				{
					int block_out_x;
					int block_out_y;
					char ahead_block = WhatIsAheadWithTeleport(board, j, i, dir_x, dir_y, &block_out_x, &block_out_y);

					*GetSquare(board, j, i) = 'O';

					if (ahead_block == 'H') //if it is hole annihilate it with the block
					{
						*GetSquare(board, block_out_x, block_out_y) = 'O';
					}
					else
					{
						*GetSquare(board, block_out_x, block_out_y) = ahead_player;
					}
				}
			}
		}
	}
	
	//move player
	board->player_x = out_x;
	board->player_y = out_y;

	//destroy secret wall if player stands on it
	if (*GetSquare(board, out_x, out_y) == 'S')
	{
		*GetSquare(board, out_x, out_y) = 'O';
		return 2; //the user has discovered a secret
	}
	
	return 1;
}

char CertificateValidity(Level* level)
{
	Board copy = level->board;

	if (IsWon(&copy))
	{
		return 1;
	}

	for (int i = 0; i < strlen(level->certificate); i++)
	{
		SimulateMove(&copy, level->certificate[i]); //illegal moves are ignored
		if (IsWon(&copy))
		{
			return 1;
		}
	}

	return 0; //did not reach a won state
}

char LoadLevel(char* filepath, Level* out_level)
{
	memset(out_level, 0, sizeof(Level));
	InitBoard(&out_level->board);

	FILE* file = fopen(filepath, "r");
	if (!file)
	{
		return 0;
	}

	char line[2 * BUFFER_SIZE];

	//line 1: name
	if (!fgets(out_level->name, sizeof(out_level->name), file)) 
	{ 
		fclose(file); 
		return 0; 
	}
	out_level->name[strcspn(out_level->name, "\r\n")] = 0;

	//line 2: certificate
	if (!fgets(out_level->certificate, sizeof(out_level->certificate), file)) 
	{ 
		fclose(file); 
		return 0; 
	}
	out_level->certificate[strcspn(out_level->certificate, "\r\n")] = 0;

	//line 3: cosmetics
	if (!fgets(line, sizeof(line), file)) 
	{ 
		fclose(file); 
		return 0; 
	}
	int c1, c2, c3, c4;
	if (sscanf(line, "%d, %d, %d, %d", &c1, &c2, &c3, &c4) == 4)
	{
		out_level->cosmetics[0] = (unsigned char)c1;
		out_level->cosmetics[1] = (unsigned char)c2;
		out_level->cosmetics[2] = (unsigned char)c3;
		out_level->cosmetics[3] = (unsigned char)c4;
	}

	//line 4: player position
	if (!fgets(line, sizeof(line), file)) 
	{ 
		fclose(file); 
		return 0; 
	}
	sscanf(line, "%d, %d", &out_level->board.player_x, &out_level->board.player_y);

	//line 5: goal position
	if (!fgets(line, sizeof(line), file)) 
	{
		fclose(file); 
		return 0; 
	}
	sscanf(line, "%d, %d", &out_level->board.goal_x, &out_level->board.goal_y);

	//lines 6-13: 8x8 board
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		if (!fgets(line, sizeof(line), file)) //unexpected EOF
		{
			return 0;
		}

		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			char c = line[j];

			if (c != '\n' && c != '\r' && c != '\0')
			{
				*GetSquare(&out_level->board, j, i) = c;
			}
		}
	}

	fclose(file);
	return 1;
}

char LoadLevelPack(char* filepath, LevelPack* out_pack)
{
	memset(out_pack, 0, sizeof(LevelPack));

	FILE* file = fopen(filepath, "r");
	if (!file)
	{
		return 0;
	}

	char line[2 * BUFFER_SIZE];

	//line 1: pack name
	if (!fgets(out_pack->name, sizeof(out_pack->name), file))
	{
		fclose(file);
		return 0;
	}
	out_pack->name[strcspn(out_pack->name, "\r\n")] = 0;

	//line 2: level count
	if (!fgets(line, sizeof(line), file))
	{
		fclose(file);
		return 0;
	}
	sscanf(line, "%d", &out_pack->level_count);

	if (out_pack->level_count > MAX_LEVELS)
	{
		out_pack->level_count = MAX_LEVELS;
	}

	for (int i = 0; i < out_pack->level_count; i++) //for every level
	{
		Level* current_level = &out_pack->levels[i];
		InitBoard(&current_level->board);

		while (fgets(line, sizeof(line), file)) //remove empty lines
		{
			if (line[0] != '\n' && line[0] != '\r')
			{
				break;
			}
		}

		if (feof(file))
		{
			out_pack->level_count = i;
			break;
		}

		//level line 1: name (already read by the while loop above)
		strcpy(current_level->name, line);
		current_level->name[strcspn(current_level->name, "\r\n")] = 0;

		//level line 2: certificate
		if (!fgets(current_level->certificate, sizeof(current_level->certificate), file))
		{
			out_pack->level_count = i;
			break;
		}
		current_level->certificate[strcspn(current_level->certificate, "\r\n")] = 0;

		//level line 3: cosmetics
		if (!fgets(line, sizeof(line), file))
		{
			out_pack->level_count = i;
			break;
		}
		int c1, c2, c3, c4;
		if (sscanf(line, "%d, %d, %d, %d", &c1, &c2, &c3, &c4) == 4)
		{
			current_level->cosmetics[0] = (unsigned char)c1;
			current_level->cosmetics[1] = (unsigned char)c2;
			current_level->cosmetics[2] = (unsigned char)c3;
			current_level->cosmetics[3] = (unsigned char)c4;
		}

		//level line 4: player position
		if (!fgets(line, sizeof(line), file))
		{
			out_pack->level_count = i;
			break;
		}
		sscanf(line, "%d, %d", &current_level->board.player_x, &current_level->board.player_y);

		//level line 5: goal position
		if (!fgets(line, sizeof(line), file))
		{
			out_pack->level_count = i;
			break;
		}
		sscanf(line, "%d, %d", &current_level->board.goal_x, &current_level->board.goal_y);

		//level lines 6-13: 8x8 board
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			if (!fgets(line, sizeof(line), file))
			{
				out_pack->level_count = i;
				break;
			}

			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				char c = line[x];

				if (c != '\n' && c != '\r' && c != '\0')
				{
					*GetSquare(&current_level->board, x, y) = c;
				}
			}
		}
	}

	fclose(file);
	return 1;
}

char SaveLevelPack(const char* filepath, LevelPack* pack)
{
	FILE* file = fopen(filepath, "w");
	if (file == NULL)
	{	
		return 0;
	}

	// Line 1: Pack Name
	fprintf(file, "%s\n", pack->name);

	// Line 2: Level Count
	fprintf(file, "%d\n", pack->level_count);

	// Loop through and write each level
	for (int i = 0; i < pack->level_count; i++)
	{
		Level* current_level = &pack->levels[i];

		// Put a blank line before every level to keep the text file readable
		fprintf(file, "\n");

		// Level Line 1: Name
		fprintf(file, "%s\n", current_level->name);

		// Level Line 2: Certificate
		// (If the certificate is empty, print a placeholder or just a newline)
		if (strlen(current_level->certificate) > 0)
		{
			fprintf(file, "%s\n", current_level->certificate);
		}
		else
		{
			fprintf(file, "unsolved\n");
		}

		// Level Line 3: Cosmetics
		fprintf(file, "%d, %d, %d, %d\n",
			current_level->cosmetics[0], current_level->cosmetics[1],
			current_level->cosmetics[2], current_level->cosmetics[3]);

		// Level Line 4: Player Position
		fprintf(file, "%d, %d\n", current_level->board.player_x, current_level->board.player_y);

		// Level Line 5: Goal Position
		fprintf(file, "%d, %d\n", current_level->board.goal_x, current_level->board.goal_y);

		// Level Lines 6-13: The 8x8 Board
		for (int y = 0; y < BOARD_HEIGHT; y++)
		{
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				// We can safely dereference GetSquare here because we know our loop bounds are exactly the board dimensions
				char c = *GetSquare(&current_level->board, x, y);
				fputc(c, file);
			}
			fputc('\n', file); // Newline at the end of each row
		}
	}

	fclose(file);
	return 1;
}