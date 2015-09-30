#pragma once
#include "Common.h"



class Highscores {

public:
	Highscores();
	~Highscores();
	int add(const Highscore& score);
	void get(int level, GameMode mode, Highscore* scores);
	void save();
	void load();
private:	
	Highscore _scores[MAX_LEVELS * MAX_SCORE_ENTRIES * 2];
	int _maxEntries;
};

