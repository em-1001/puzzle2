#define _CRT_SECURE_NO_WARNINGS
#include <bangtal.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>


clock_t starttime, endtime;

SceneID scene;
ObjectID start;
ObjectID game_board[9], original_board[9];

int blank = 8;

bool game = false;

TimerID timer, timer2;
float timerValue = 0.05f;
int mixCount;


int indexTOX(int i)
{
	return 101 + 367 * (i % 3);
}
int indexTOY(int i)
{
	return 480 - 190 * (i / 3);
}

int game_index(ObjectID object)
{
	for (int i = 0; i < 9; i++) {
		if (game_board[i] == object) return i;
	}
	return -1;
}

void game_move(int i)
{
	ObjectID object = game_board[i];
	game_board[i] = game_board[blank];
	locateObject(game_board[i], scene, indexTOX(i), indexTOY(i));
	game_board[blank] = object;
	locateObject(game_board[blank], scene, indexTOX(blank), indexTOY(blank));

	blank = i;
}

bool possible_move(int i) {
	if (i % 3 > 0 && blank == i - 1) return true;
	if (i % 3 < 2 && blank == i + 1) return true;
	if (i / 3 > 0 && blank == i - 3) return true;
	if (i / 3 < 2 && blank == i + 3) return true;

	return false;
}

int random_move()
{

	
	int i = -1;
	while (i == -1)
	{
		switch (rand() % 4) {
			case 0: if (blank % 3 > 0) i = blank - 1;		
				break;
			case 1: if (blank % 3 < 2) i = blank + 1;
				break;
			case 2:	if (blank / 3 > 0) i = blank - 3;
				break;
			case 3: if (blank / 3 < 2) i = blank + 3;
				break;
		}

	}

	return i;
}

bool game_end() {
	for (int i = 0; i < 9; i++) {
		if (game_board[i] != original_board[i]) return false;
	}
	return true;
}

void game_start()
{
	blank = 8;
	hideObject(game_board[blank]);

	mixCount = 50;

	setTimer(timer, timerValue);
	startTimer(timer);

	
	
}


void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (game) {
		int i = game_index(object);
		if (possible_move(i)) {
			game_move(i);

			if (game_end())
			{
				game = false;
				showObject(start);
				showObject(game_board[blank]);
				endtime = clock();
				int time = (endtime - starttime)/CLOCKS_PER_SEC;
				char temp[50];
				sprintf(temp, "Completed!!!\n걸린시간 %d초", time);

				showMessage(temp);
				
			}

		}
	}
	else
	{
		if (object == start)
		{
			game_start();
		}
	}
	
}

void timerCallback(TimerID timer)
{
	game_move(random_move());

	mixCount--;

	if (mixCount > 0) 
	{
		setTimer(timer, timerValue);
		startTimer(timer);
	}
	else
	{
		game = true;
		hideObject(start);
		starttime = clock();
		
	}
}



ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true)
{
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) showObject(object);

	return object;
}

void game_init()
{
	scene = createScene("퍼즐 맞추기", "scene1.png");

	char path[50];
	for (int i = 0; i < 9; i++)
	{
		sprintf(path, "image%d.png", i + 1);
		game_board[i] = createObject(path, scene, indexTOX(i), indexTOY(i));
		original_board[i] = game_board[i];
	}

	start = createObject("start.png", scene, 590, 100);
	
	timer = createTimer(timerValue);
	

}


int main()
{
	srand(time(NULL));

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	

	game_init();
	startGame(scene);
}