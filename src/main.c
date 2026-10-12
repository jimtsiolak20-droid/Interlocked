#include "raylib.h"
#include "resource_dir.h"
#include "level.h"
#include "play_level.h"
#include "level_select.h"
#include "level_editor.h"
#include "menu.h"

#define WIDTH 1920
#define HEIGHT 1080
#define FPS 60

#define WORLD_COUNT 4

int main()
{
	SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_VSYNC_HINT);

	SetTargetFPS(FPS);
	InitWindow(WIDTH, HEIGHT, "Interlocked");

	InitAudioDevice();
	GameSounds game_sounds;
	LoadGameSounds(&game_sounds);

	Rectangle render_rectangle = (Rectangle){ (WIDTH - HEIGHT) / 2.0f, 0.0f, HEIGHT, HEIGHT };
	char outmoves[BUFFER_SIZE];

	char* level_paths[WORLD_COUNT] = { "levels/world1.txt", "levels/world2.txt", "levels/world3.txt", "levels/world_extra.txt" };
	LevelPack level_packs[WORLD_COUNT];
	for (char i = 0; i < WORLD_COUNT; i++)
	{
		LoadLevelPack(level_paths[i], &level_packs[i]);
	}
	
	char main_menu_result = MainMenu(&game_sounds);
	while (main_menu_result != 0)
	{
		if (main_menu_result == 1)
		{
			char world_menu_result = WorldMenu(&game_sounds);
			while (world_menu_result != -1)
			{
			    if (world_menu_result < WORLD_COUNT)
				{
					LevelPack* level_pack = &(level_packs[world_menu_result]);
					char level_selection = SelectLevel(level_pack, 0, &game_sounds);
					while (level_selection != -1)
					{
						//player
						EmptyFrame();
						Level* current_level = &level_pack->levels[level_selection];
						if (PlayLevel(current_level, render_rectangle, outmoves, &game_sounds)) //if level won
						{
							strcpy(current_level->certificate, outmoves);
							SaveLevelPack(level_paths[world_menu_result], level_pack);
							if (level_selection < LEVELS_PER_WORLD - 1)
							{
								level_selection++;
								continue;
							}
						}

						level_selection = SelectLevel(level_pack, level_selection, &game_sounds);
					}
				}
				else //world_menu_result == 5
				{
					Editor(&game_sounds);
				}

				world_menu_result = WorldMenu(&game_sounds);
			}
		}
		else //menu_result == 2
		{

		}

		main_menu_result = MainMenu(&game_sounds);
	}

	UnloadGameSounds(&game_sounds);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
