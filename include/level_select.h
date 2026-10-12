#pragma once

#include "raylib.h"
#include "play_level.h"

#define SELECT_SCREEN_GRID_WIDTH 4
#define SELECT_SCREEN_GRID_HEIGHT 3
#define LEVELS_PER_WORLD SELECT_SCREEN_GRID_WIDTH * SELECT_SCREEN_GRID_HEIGHT
#define SELECT_SCREEN_X_OFFSET 350
#define SELECT_SCREEN_Y_OFFSET 150
#define SELECT_SCREEN_ELEMENT_LENGTH 250
#define SELECT_SCREEN_SPACING 50
#define SELECT_SCREEN_STEP (SELECT_SCREEN_ELEMENT_LENGTH + SELECT_SCREEN_SPACING)
#define SELECT_OUTLINE 12
#define WON_OUTLINE 5

char SelectLevel(LevelPack* level_pack, char starting_selection, GameSounds* game_sounds)
{
	EmptyFrame();

	RenderTexture2D small_texture = LoadRenderTexture(BOARD_WIDTH * PPS, BOARD_HEIGHT * PPS);
	SetTextureFilter(small_texture.texture, TEXTURE_FILTER_POINT);
	Rectangle source_rect = (Rectangle){ 0.0f, 0.0f, (float)small_texture.texture.width, -(float)small_texture.texture.height };

	char selection = starting_selection;

	while (1)
	{
		char input = GetInput();

		if (input == 'q')
		{
			selection = -1; //exit
			break;
		}
		else if (input == 's')
		{
			break;
		}
		else if (input == 'U' && selection - SELECT_SCREEN_GRID_WIDTH >= 0)
		{
			PlaySound(game_sounds->sfx_moved);
			selection -= SELECT_SCREEN_GRID_WIDTH;
		}
		else if (input == 'D' && selection + SELECT_SCREEN_GRID_WIDTH < level_pack->level_count)
		{
			PlaySound(game_sounds->sfx_moved);
			selection += SELECT_SCREEN_GRID_WIDTH;
		}
		else if (input == 'L' && selection - 1 >= 0)
		{
			PlaySound(game_sounds->sfx_moved);
			selection -= 1;
		}
		else if (input == 'R' && selection + 1 < level_pack->level_count)
		{
			PlaySound(game_sounds->sfx_moved);
			selection += 1;
		}
		
		BeginDrawing();
		ClearBackground(BLACK);

		DrawText(level_pack->name, SELECT_SCREEN_X_OFFSET, 40, 40, WHITE);

		char cord_x = selection % SELECT_SCREEN_GRID_WIDTH;
		char cord_y = selection / SELECT_SCREEN_GRID_WIDTH;

		DrawRectangle(cord_x * SELECT_SCREEN_STEP + SELECT_SCREEN_X_OFFSET - SELECT_OUTLINE, cord_y * SELECT_SCREEN_STEP + SELECT_SCREEN_Y_OFFSET - SELECT_OUTLINE, SELECT_SCREEN_ELEMENT_LENGTH + 2 * SELECT_OUTLINE, SELECT_SCREEN_ELEMENT_LENGTH + 2 * SELECT_OUTLINE, YELLOW);

		for (int i = 0; i < SELECT_SCREEN_GRID_HEIGHT; i++)
		{
			for (int j = 0; j < SELECT_SCREEN_GRID_WIDTH; j++)
			{
				char lv = i * SELECT_SCREEN_GRID_WIDTH + j;
				if (lv < level_pack->level_count)
				{
					if (CertificateValidity(&level_pack->levels[lv])) //green outline if the level is complete
					{
						DrawRectangle(j * SELECT_SCREEN_STEP + SELECT_SCREEN_X_OFFSET - WON_OUTLINE, i * SELECT_SCREEN_STEP + SELECT_SCREEN_Y_OFFSET - WON_OUTLINE, SELECT_SCREEN_ELEMENT_LENGTH + 2 * WON_OUTLINE, SELECT_SCREEN_ELEMENT_LENGTH + 2 * WON_OUTLINE, GREEN);
					}
					DrawText(level_pack->levels[lv].name, j * SELECT_SCREEN_STEP + SELECT_SCREEN_X_OFFSET, i * SELECT_SCREEN_STEP + SELECT_SCREEN_Y_OFFSET - 30, 20, WHITE);
					DrawBoard(&level_pack->levels[lv].board, &level_pack->levels[lv], &small_texture, -1, -1);
					DrawTexturePro(small_texture.texture, source_rect, (Rectangle) { j * SELECT_SCREEN_STEP + SELECT_SCREEN_X_OFFSET, i * SELECT_SCREEN_STEP + SELECT_SCREEN_Y_OFFSET, SELECT_SCREEN_ELEMENT_LENGTH, SELECT_SCREEN_ELEMENT_LENGTH}, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
				}
			}
		}

		EndDrawing();
	}

	UnloadRenderTexture(small_texture);

	return selection;
}