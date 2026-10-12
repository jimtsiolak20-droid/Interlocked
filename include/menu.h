#pragma once

#include "raylib.h"
#include "resource_dir.h"
#include "level.h"
#include "play_level.h"
#include "level_select.h"
#include "level_editor.h"

#define MENU_X_POS 500

/*
	returns 1 if selected play
	returns 2 if selected controls
	returns 0 if selected quit
*/
char MainMenu(GameSounds* game_sounds)
{
	EmptyFrame();

	/*
		selections:
		0: play
		1: controls
		2: credits
		3: quit
	*/

	char selection = 0;

	while (1)
	{
		char input = GetInput();

		switch (input)
		{
		case 'q':
		{
			return 0;
			break;
		}
		case 'U':
		{
			if (selection > 0)
			{
				selection--;
				PlaySound(game_sounds->sfx_moved);
			}
			break;
		}
		case 'D':
		{
			if (selection < 3)
			{
				selection++;
				PlaySound(game_sounds->sfx_moved);
			}
			break;
		}
		case 's':
		{
			switch (selection)
			{
			case 0:
			{
				PlaySound(game_sounds->sfx_won);
				return 1;
				break;
			}
			case 1:
			{
				PlaySound(game_sounds->sfx_won);
				return 2;
				break;
			}
			case 2:
			{
				PlaySound(game_sounds->sfx_won);
				MinimizeWindow();
				system("start https://astronut-entertainment.itch.io/");
				break;
			}
			case 3:
			{
				PlaySound(game_sounds->sfx_won);
				return 0;
				break;
			}
			}
			break;
		}
		}

		BeginDrawing();
		ClearBackground(BLACK);

		DrawText("INTERLOCKED", MENU_X_POS, 150, 100, WHITE);
		DrawText("by Dimitris Tsiolakis", MENU_X_POS, 250, 50, WHITE);

		//idle, selected
		int text_sizes[2] = { 70, 90 };
		Color text_colors[2] = { WHITE, YELLOW };

		DrawText("PLAY", MENU_X_POS, 500, text_sizes[selection == 0], text_colors[selection == 0]);
		DrawText("CONTROLS", MENU_X_POS, 600, text_sizes[selection == 1], text_colors[selection == 1]);
		DrawText("MORE GAMES", MENU_X_POS, 700, text_sizes[selection == 2], text_colors[selection == 2]);
		DrawText("QUIT", MENU_X_POS, 800, text_sizes[selection == 3], text_colors[selection == 3]);

		EndDrawing();
	}
}

/*
	-1: BACK
	1: world 1
	2: world 2
	3: world 3
	4: world extra
	5: editor
*/
char WorldMenu(GameSounds* game_sounds)
{
	EmptyFrame();

	char selection = 0;

	while (1)
	{
		char input = GetInput();

		switch (input)
		{
		case 'q':
		{
			return -1;
			break;
		}
		case 'U':
		{
			if (selection > 0)
			{
				selection--;
				PlaySound(game_sounds->sfx_moved);
			}
			break;
		}
		case 'D':
		{
			if (selection < 4)
			{
				selection++;
				PlaySound(game_sounds->sfx_moved);
			}
			break;
		}
		case 's':
		{
			PlaySound(game_sounds->sfx_won);
			return selection;
			break;
		}
		}

		BeginDrawing();
		ClearBackground(BLACK);

		//DrawText("", MENU_X_POS, 150, 100, WHITE);
		
		//idle, selected
		int text_sizes[2] = { 70, 90 };
		Color text_colors[2] = { WHITE, YELLOW };

		DrawText("WORLD 1", MENU_X_POS, 200, text_sizes[selection == 0], text_colors[selection == 0]);
		DrawText("WORLD 2", MENU_X_POS, 300, text_sizes[selection == 1], text_colors[selection == 1]);
		DrawText("WORLD 3", MENU_X_POS, 400, text_sizes[selection == 2], text_colors[selection == 2]);
		DrawText("WORLD EXTRA", MENU_X_POS, 500, text_sizes[selection == 3], text_colors[selection == 3]);
		DrawText("EDITOR", MENU_X_POS, 800, text_sizes[selection == 4], text_colors[selection == 4]);
	
		EndDrawing();
	}
}

void ControlsScreen()
{
	EmptyFrame();

	while (1)
	{
		if (GetInput() == 'q')
		{
			return;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		EndDrawing();
	}
}