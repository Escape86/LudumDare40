#pragma once

#pragma region Filepaths
#define BLUE_CIRCLE_TEXTURE_PATH	"resources/Shrine_Water.png"
#define BLUE_DOT_TEXTURE_PATH		"resources/Orb_Water.png"
#define BLACK_DOT_TEXTURE_PATH		"resources/Orb_Neutral.png"
#define GREEN_CIRCLE_TEXTURE_PATH	"resources/Shrine_Plant.png"
#define GREEN_DOT_TEXTURE_PATH		"resources/Orb_Plant.png"
#define RED_CIRCLE_TEXTURE_PATH		"resources/Shrine_Fire.png"
#define RED_DOT_TEXTURE_PATH		"resources/Orb_Fire.png"
#define PLAYER_OVERLAY_TEXTURE_PATH	"resources/PlayerOverlay.png"
#define FONT_FILEPATH				"resources/Fonts/Orbitron/Orbitron-Regular.ttf"
#define COLLECT_ORB_WAVE_FILEPATH	"resources/Audio/orb_collect.wav"
#define DESTROY_ORB_WAVE_FILEPATH	"resources/Audio/orb_destroy.wav"
#pragma endregion

#pragma region Colors
#define BLUE						{ 0, 0, 255 }
#define GREEN						{ 0, 255, 0 }
#define RED							{ 255, 0, 0 }
#define BLACK						{ 0, 0, 0 }
#define WHITE						{ 255, 255, 255}
#pragma endregion

#define SCREEN_WIDTH				800
#define SCREEN_HEIGHT				600

#define PLAYER_VELOCITY				10
#define ENEMY_VELOCITY				1

#define JOYSTICK_DEAD_ZONE			8000

#define TITLE_LEVEL_ID				0
#define STORY_LEVEL_ID				1
#define GAMEOVER_LEVEL_ID			-999

#define ENEMY_REACHES_SHRINE_HP_COST 5
#define PLAYER_TOUCHES_ENEMY_ORB_HP_COST 10

#define LEVEL_END_TRANSITION_DURATION 4000