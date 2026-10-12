#pragma once

#include <stdio.h>
#include <string.h>

#include "raylib.h"
#include "rlgl.h"
#include "level.h"

#define PPS 5 //Pixels Per Square
#define MAX_MOVE_COUNT BUFFER_SIZE

const float BRIGHTNESS_COLOR_VALUE = -0.2f;

const Color COLOR_U = {0, 0, 0, 255};
const Color COLOR_W = {80, 80, 80, 255};
const Color COLOR_O = {140, 140, 140, 255};
const Color COLOR_H = {40, 40, 40, 255};
const Color COLOR_T = {150, 255, 220, 255};
const Color COLOR_R = {255, 0, 0, 255};
const Color COLOR_G = {0, 255, 0, 255};
const Color COLOR_B = {0, 0, 255, 255};
const Color COLOR_C = { 0, 255, 255, 255 };
const Color COLOR_M = { 255, 0, 255, 255 };
const Color COLOR_Y = { 255, 255, 0, 255 };
const Color COLOR_player = { 255, 255, 255, 255 };

void DrawBoard(Board* board, Level* level, RenderTexture2D* small_texture, char pointer_x, char pointer_y)
{
	Color color_gem; //wall complementary color, changes with byte 0
	switch (level->cosmetics[0])
	{
	case 1:
	{
		color_gem = (Color){ 80, 20, 100, 255 }; //crystal
		break;
	}
	case 2:
	{
		color_gem = (Color){ 50, 50, 50, 255 }; //coal
		break;
	}
	case 3:
	{
		color_gem = (Color){ 190, 160, 0, 255 }; //gold
		break;
	}
	case 4:
	{
		color_gem = (Color){ 50, 150, 150, 255 }; //diamond
		break;
	}
	default:
	{
		color_gem = (Color){150, 0, 150, 255};
		break;
	}
	}

	BeginTextureMode(*small_texture);
	ClearBackground(BLACK);
	rlColorMask(true, true, true, false);
	
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			switch (*GetSquare(board, j, i))
			{
			case 'W':
			case 'S':
			{
				DrawRectangle(j * PPS, i * PPS, PPS, PPS, COLOR_W);
				DrawPixel(j * PPS + 0, i * PPS + 1, color_gem);
				DrawPixel(j * PPS + 1, i * PPS + 4, color_gem);
				DrawPixel(j * PPS + 3, i * PPS + 2, color_gem);
				break;
			}
			case 'O':
			{
				DrawRectangle(j * PPS, i * PPS, PPS, PPS, COLOR_O);
				break;
			}
			case 'H':
			{
				DrawRectangle(j * PPS, i * PPS, PPS, PPS, BLACK);
				for (int n = 0; n < PPS; n++)
				{
					for (int m = 0; m < PPS; m++)
					{
						if ((n + m) % 2 == (i + j) % 2)
						{
							DrawPixel(j * PPS + m, i * PPS + n, COLOR_H);
						}
					}
				}
				break;
			}
			case 'T':
			{
				DrawRectangle(j * PPS, i * PPS, PPS, PPS, WHITE);
				DrawRectangle(j * PPS + 1, i * PPS + 1, 3, 1, COLOR_T);
				DrawRectangle(j * PPS + 1, i * PPS + 3, 3, 1, COLOR_T);
				DrawPixel(j * PPS + 1, i * PPS + 2, COLOR_T);
				DrawPixel(j * PPS + 3, i * PPS + 2, COLOR_T);
				break;
			}
			case 'R':
			case 'G':
			case 'B':
			case 'C':
			case 'M':
			case 'Y':
			{
				Color color;
				switch (*GetSquare(board, j, i))
				{
				case 'R':
					color = COLOR_R;
					break;
				case 'G':
					color = COLOR_G;
					break;
				case 'B':
					color = COLOR_B;
					break;
				case 'C':
					color = COLOR_C;
					break;
				case 'M':
					color = COLOR_M;
					break;
				case 'Y':
					color = COLOR_Y;
					break;
				}
				
				DrawRectangle(j * PPS, i * PPS, PPS, PPS, color);

				//outlines
				Color color_outline = ColorBrightness(color, BRIGHTNESS_COLOR_VALUE);
				char same = SafeGetSquare(board, j, i);
				if (SafeGetSquare(board, j, i - 1) != same)
				{
					DrawRectangle(j * PPS, i * PPS, PPS, 1, color_outline); //top
				}
				if (SafeGetSquare(board, j, i + 1) != same)
				{
					DrawRectangle(j * PPS, i * PPS + PPS - 1, PPS, 1, color_outline); //bottom
				}
				if (SafeGetSquare(board, j - 1, i) != same)
				{
					DrawRectangle(j * PPS, i * PPS, 1, PPS, color_outline); //left
				}
				if (SafeGetSquare(board, j + 1, i) != same)
				{
					DrawRectangle(j * PPS + PPS - 1, i * PPS, 1, PPS, color_outline); //right
				}
				if (SafeGetSquare(board, j - 1, i) == same && SafeGetSquare(board, j, i - 1) == same && SafeGetSquare(board, j - 1, i - 1) != same)
				{
					DrawPixel(j * PPS, i * PPS, color_outline); //top left
				}
				if (SafeGetSquare(board, j + 1, i) == same && SafeGetSquare(board, j, i - 1) == same && SafeGetSquare(board, j + 1, i - 1) != same)
				{
					DrawPixel(j * PPS + PPS - 1, i * PPS, color_outline); //top right
				}
				if (SafeGetSquare(board, j - 1, i) == same && SafeGetSquare(board, j, i + 1) == same && SafeGetSquare(board, j - 1, i + 1) != same)
				{
					DrawPixel(j * PPS, i * PPS + PPS - 1, color_outline); //bottom left
				}
				if (SafeGetSquare(board, j + 1, i) == same && SafeGetSquare(board, j, i + 1) == same && SafeGetSquare(board, j + 1, i + 1) != same)
				{
					DrawPixel(j * PPS + PPS - 1, i * PPS + PPS - 1, color_outline); //bottom right
				}
				break;
			}
			default:
			{
				break;
			}
			}
		}
	}

	if (level->cosmetics[1] & 0b00000001) //dark mode
	{
		for (int i = 0; i < BOARD_HEIGHT; i++)
		{
			for (int j = 0; j < BOARD_WIDTH; j++)
			{
				if (abs(board->player_x - j) > 1 || abs(board->player_y - i) > 1) //outside view
				{
					DrawRectangle(j * PPS, i * PPS, PPS, PPS, BLACK);
				}
				else if (abs(board->player_x - j) == 1 && abs(board->player_y - i) == 1) //diagonal, slightly darker
				{
					DrawRectangle(j * PPS, i * PPS, PPS, PPS, (Color) {0, 0, 0, 30});
				}
			}
		}
	}

	//player
	Color color_player_outline = ColorBrightness(COLOR_player, BRIGHTNESS_COLOR_VALUE);
	DrawRectangle(board->player_x * PPS, board->player_y * PPS, PPS, PPS, COLOR_player);
	DrawRectangle(board->player_x* PPS, board->player_y* PPS, PPS, 1, color_player_outline);
	DrawRectangle(board->player_x * PPS, board->player_y * PPS, 1, PPS, color_player_outline);
	DrawRectangle(board->player_x * PPS + PPS - 1, board->player_y * PPS, 1, PPS, color_player_outline);
	DrawRectangle(board->player_x * PPS, board->player_y * PPS + PPS - 1, PPS, 1, color_player_outline);
	DrawRectangle(board->player_x * PPS  + 1, board->player_y * PPS + 3, 3, 1, BLACK);
	DrawPixel(board->player_x * PPS + 1, board->player_y * PPS + 1, BLACK);
	DrawPixel(board->player_x * PPS + 3, board->player_y * PPS + 1, BLACK);

	//goal
	Color color_goal_yellow = YELLOW;
	Color color_goal_orange = ORANGE;
	if (*GetSquare(board, board->goal_x, board->goal_y) == 'H')
	{
		color_goal_yellow.a = 170;
		color_goal_orange.a = 170;
	}
	DrawRectangle(board->goal_x * PPS + 1, board->goal_y * PPS + 1, 3, 3, color_goal_yellow);
	DrawPixel(board->goal_x* PPS + 2, board->goal_y* PPS + 2, color_goal_orange);

	//selection
	if (pointer_x >= 0 && pointer_y >= 0)
	{
		Color color_selection = (Color){ 255, 255, 200, 200 };
		DrawRectangle(pointer_x * PPS - 1, pointer_y * PPS - 1, PPS + 2, 1, color_selection);
		DrawRectangle(pointer_x* PPS - 1, pointer_y* PPS, 1, PPS, color_selection);
		DrawRectangle(pointer_x* PPS + PPS, pointer_y* PPS, 1, PPS, color_selection);
		DrawRectangle(pointer_x* PPS - 1, pointer_y* PPS + PPS, PPS + 2, 1, color_selection);
	}

	EndTextureMode();
	rlColorMask(true, true, true, true);
}

char GetInput()
{
	if (WindowShouldClose())
	{
		exit(-1);
	}
	if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_UP))
	{
		return 'U'; //up
	}
	if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))
	{
		return 'D'; //down
	}
	if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))
	{
		return 'L'; //left
	}
	if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
	{
		return 'R'; //right
	}

	if (IsKeyPressed(KEY_R) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP))
	{
		return 'r'; //restart
	}
	if (IsKeyPressed(KEY_Z) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
	{
		return 'u'; //undo
	}

	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
	{
		return 's'; //spacebar, select
	}
	if (IsKeyPressed(KEY_BACKSPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
	{
		return 'q'; //quit, back, escape
	}

	return 0;
}

typedef struct BoardStack
{
	Board stack[MAX_MOVE_COUNT];
	char moves[MAX_MOVE_COUNT];
	int top;

} BoardStack;

void InitBoardStack(BoardStack* board_stack, Board start_state)
{
	memset(board_stack, 0, sizeof(BoardStack));
	board_stack->stack[board_stack->top] = start_state;
}

Board* GetCurrentBoard(BoardStack* board_stack)
{
	return &(board_stack->stack[board_stack->top]);
}

char* GetCurrentMoves(BoardStack* board_stack)
{
	return board_stack->moves;
}

/*
	-1: no input
	0: illegal move, full stack
	1: moved succesfully
	2: uncovered secret wall
	3: won
	4: undo
	5: restart
*/
char MakeMove(BoardStack* board_stack, char input)
{
	switch (input)
	{
	case 'u': //undo
	{
		if (board_stack->top > 0)
		{
			board_stack->moves[--board_stack->top] = '\0';
		}
		return 4;
	}
	case 'r': //restart
	{
		InitBoardStack(board_stack, board_stack->stack[0]);
		return 5;
	}
	case 'U':
	case 'D':
	case 'L':
	case 'R':
	{
		if (board_stack->top >= MAX_MOVE_COUNT - 1) //stack full
		{
			return 0;
		}

		Board copy = *GetCurrentBoard(board_stack);
		char result = SimulateMove(&copy, input);
		
		if (result == 0)
		{
			return 0; //illegal move
		}

		board_stack->moves[board_stack->top] = input;
		board_stack->stack[++board_stack->top] = copy;

		if (IsWon(&copy))
		{
			return 3; //won
		}

		return result;
	}
	}

	return -1;
}

typedef struct GameSounds
{
	Sound sfx_moved;
	Sound sfx_illegal;
	Sound sfx_restart;
	Sound sfx_won;

}GameSounds;

void LoadGameSounds(GameSounds* game_sounds)
{
	game_sounds->sfx_moved = LoadSound("sounds/move.wav");
	game_sounds->sfx_illegal = LoadSound("sounds/blocked.wav");
	game_sounds->sfx_restart = LoadSound("sounds/restart.wav");
	game_sounds->sfx_won = LoadSound("sounds/won.wav");
}

void UnloadGameSounds(GameSounds* game_sounds)
{
	UnloadSound(game_sounds->sfx_moved);
	UnloadSound(game_sounds->sfx_illegal);
	UnloadSound(game_sounds->sfx_restart);
	UnloadSound(game_sounds->sfx_won);

	SetSoundVolume(game_sounds->sfx_moved, 0.6f);
}

void EmptyFrame()
{
	BeginDrawing();
	ClearBackground(BLACK);
	EndDrawing();
}

char PlayLevel(Level* level, Rectangle render_rectangle, char* out_moves, GameSounds* game_sounds)
{
	EmptyFrame();

	RenderTexture2D small_texture = LoadRenderTexture(BOARD_WIDTH * PPS, BOARD_HEIGHT * PPS);
	SetTextureFilter(small_texture.texture, TEXTURE_FILTER_POINT);
	Rectangle source_rect = (Rectangle){0.0f, 0.0f, (float)small_texture.texture.width, -(float)small_texture.texture.height};

	BoardStack board_stack;
	InitBoardStack(&board_stack, level->board);

	char outcome;
	char in_level = 1;
	while (in_level)
	{
		char input = GetInput();

		if (input == 'q')
		{
			outcome = 0; //quit
			in_level = 0;
		}

		if (input != 0)
		{
			char result = MakeMove(&board_stack, input);

			switch (result) //sound and other stuff
			{
			case 0:
			{
				PlaySound(game_sounds->sfx_illegal);
				break;
			}
			case 1:
			{
				PlaySound(game_sounds->sfx_moved);
				break;
			}
			case 2:
			{
				//play dinstinct sound
				break;
			}
			case 3: //won
			{
				PlaySound(game_sounds->sfx_won);
				outcome = 1;
				in_level = 0;
				break;
			}
			case 4:
			{
				PlaySound(game_sounds->sfx_restart);
				break;
			}
			case 5:
			{
				PlaySound(game_sounds->sfx_restart);
				break;
			}
			}
		}

		DrawBoard(GetCurrentBoard(&board_stack), level, &small_texture, -1, -1);

		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(level->name, 10, 10, 30, WHITE);
		//DrawText(board_stack.moves, 10, 50, 12, WHITE);
		char move_counter[10];
		sprintf(move_counter, "moves: %d", strlen(board_stack.moves));
		DrawText(move_counter, 10, 100, 30, WHITE);
		if (board_stack.top >= MAX_MOVE_COUNT - 1)
		{
			DrawText("BOARD STACK FULL", 10, 200, 30, RED);
			DrawText("UNDO OR RESTART", 10, 240, 30, RED);
		}
		DrawTexturePro(small_texture.texture, source_rect, render_rectangle, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);

		EndDrawing();
	}
	
	UnloadRenderTexture(small_texture);
	strcpy(out_moves, board_stack.moves);

	return outcome;
}
