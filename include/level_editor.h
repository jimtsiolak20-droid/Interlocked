#pragma once

#include "raylib.h"
#include "level_select.h"

#define EDITOR_PATH "levels/editor.txt"

#define WIDTH 1920
#define HEIGHT 1080

#define SELECTION_WIDTH 6
#define SELECTION_HEIGHT 2

void InitSelectionLevel(Level* level)
{
	memset(level, 0, sizeof(Level));
	
	level->board.player_x = 0;
	level->board.player_y = 0;

	level->board.goal_x = 1;
	level->board.goal_y = 0;

	level->board.mboard[2] = 'O';

	level->board.mboard[3] = 'W';

	level->board.mboard[4] = 'H';

	level->board.mboard[5] = 'T';

	level->board.mboard[BOARD_WIDTH] = 'R';

	level->board.mboard[BOARD_WIDTH + 1] = 'G';

	level->board.mboard[BOARD_WIDTH + 2] = 'B';

	level->board.mboard[BOARD_WIDTH + 3] = 'C';

	level->board.mboard[BOARD_WIDTH + 4] = 'M';
}

void EditLevel(LevelPack* pack, char index, GameSounds* game_sounds)
{
	EmptyFrame();
	
	Rectangle main_rect = (Rectangle){ WIDTH - HEIGHT, 0, HEIGHT, HEIGHT };
	Rectangle selection_rect = (Rectangle){ 0, HEIGHT * 0.3f, HEIGHT * 0.7f, HEIGHT * 0.7f};
	Rectangle render_rectangle = (Rectangle){ (WIDTH - HEIGHT) / 2.0f, 0.0f, HEIGHT, HEIGHT };

	RenderTexture2D small_texture = LoadRenderTexture(BOARD_WIDTH * PPS, BOARD_HEIGHT * PPS);
	SetTextureFilter(small_texture.texture, TEXTURE_FILTER_POINT);
	Rectangle source_rect = (Rectangle){ 0.0f, 0.0f, (float)small_texture.texture.width, -(float)small_texture.texture.height };

	Level selection_level;
	InitSelectionLevel(&selection_level);

	Level* level = &pack->levels[index];

	int location_x = 0;
	int location_y = 0;
	int item = 0;
	char selection_on = 0;
	int selection_x = 0;
	int selection_y = 0;

	char solved = CertificateValidity(level);
	/*
		0: player
		1: goal
		2: ground
		3: wall
		4: hole
		5: teleport
		6: red
		7: green
		8: blue
		9: cyan
		10: magenta
		11: reset
	*/
	while (1)
	{
		//editor
		while (1)
		{
			char input = GetInput();

			if (input == 'q')
			{
				if (!solved)
				{
					strcpy(level->certificate, "unsolved");
				}
				SaveLevelPack(EDITOR_PATH, pack); //exit editor
				UnloadRenderTexture(small_texture);
				return;
			}

			else if (input == 'r')
			{
				break;
			}

			else if (input == 'u')
			{
				selection_on = !selection_on;
			}

			//grid selection
			else if (input == 'U' && !selection_on && location_y > 0)
			{
				location_y--;
			}
			else if (input == 'D' && !selection_on && location_y < BOARD_HEIGHT - 1)
			{
				location_y++;
			}
			else if (input == 'L' && !selection_on && location_x > 0)
			{
				location_x--;
			}
			else if (input == 'R' && !selection_on && location_x < BOARD_WIDTH - 1)
			{
				location_x++;
			}

			//item selection
			else if (input == 'U' && selection_on && selection_y > 0)
			{
				selection_y--;
				item = selection_y * SELECTION_WIDTH + selection_x;
			}
			else if (input == 'D' && selection_on && selection_y < SELECTION_HEIGHT - 1)
			{
				selection_y++;
				item = selection_y * SELECTION_WIDTH + selection_x;
			}
			else if (input == 'L' && selection_on && selection_x > 0)
			{
				selection_x--;
				item = selection_y * SELECTION_WIDTH + selection_x;
			}
			else if (input == 'R' && selection_on && selection_x < SELECTION_WIDTH - 1)
			{
				selection_x++;
				item = selection_y * SELECTION_WIDTH + selection_x;
			}

			else if (input == 's' && !selection_on)
			{
				solved = 0;

				int absolute_location = location_y * BOARD_WIDTH + location_x;

				switch (item)
				{
				case 0:
				{
					level->board.player_x = location_x;
					level->board.player_y = location_y;
					break;
				}
				case 1:
				{
					level->board.goal_x = location_x;
					level->board.goal_y = location_y;
					break;
				}
				case 2:
				{
					level->board.mboard[absolute_location] = 'O';
					break;
				}
				case 3:
				{
					level->board.mboard[absolute_location] = 'W';
					break;
				}
				case 4:
				{
					level->board.mboard[absolute_location] = 'H';
					break;
				}
				case 5:
				{
					level->board.mboard[absolute_location] = 'T';
					break;
				}
				case 6:
				{
					level->board.mboard[absolute_location] = 'R';
					break;
				}
				case 7:
				{
					level->board.mboard[absolute_location] = 'G';
					break;
				}
				case 8:
				{
					level->board.mboard[absolute_location] = 'B';
					break;
				}
				case 9:
				{
					level->board.mboard[absolute_location] = 'C';
					break;
				}
				case 10:
				{
					level->board.mboard[absolute_location] = 'M';
					break;
				}
				case 11:
				{
					for (int i = 0; i < BOARD_HEIGHT * BOARD_HEIGHT; i++)
					{
						level->board.mboard[i] = 'W';
					}
					level->board.player_x = 0;
					level->board.player_y = 0;
					level->board.goal_x = 1;
					level->board.goal_y = 0;
					break;
				}
				}
			}

			BeginDrawing();
			ClearBackground(BLACK);

			if (selection_on)
			{
				DrawText("ITEM SELECTION ON", 30, HEIGHT * 0.3f - 50, 40, YELLOW);
			}

			DrawBoard(&(level->board), level, &small_texture, location_x, location_y);
			DrawTexturePro(small_texture.texture, source_rect, main_rect, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);

			DrawBoard(&(selection_level.board), &selection_level, &small_texture, selection_x, selection_y);
			DrawTexturePro(small_texture.texture, source_rect, selection_rect, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
			DrawText("Clear", 480, 450, 30, WHITE);

			DrawText(level->name, 10, 10, 30, WHITE);

			EndDrawing();
		}

		//player
		EmptyFrame();
		char outmoves[BUFFER_SIZE];
		if (PlayLevel(level, render_rectangle, outmoves, game_sounds))
		{
			strcpy(level->certificate, outmoves);
			solved = 1;
		}
	}

	UnloadRenderTexture(small_texture);
}

void Editor(GameSounds* game_sounds)
{
	EmptyFrame();
	
	char selection = 0;
	while (1)
	{
		LevelPack* editor_levels = malloc(sizeof(LevelPack));
		LoadLevelPack(EDITOR_PATH, editor_levels);

		selection = SelectLevel(editor_levels, selection, game_sounds);
		if (selection == -1)
		{
			break;
		}

		EditLevel(editor_levels, selection, game_sounds);
		free(editor_levels);
	}
}