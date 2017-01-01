#pragma once
#include "..\Constants.h"
#include <core\data\HighscoreService.h>

struct Highscore {

	int score;
	int fillrate;
	int minutes;
	int seconds;
	int level;
	char name[12];

	Highscore() : score(0), fillrate(0), minutes(0), seconds(0), level(0) {
		name[0] = '\0';
	}

	int compare(const Highscore& other) const {
		if (score < other.score) {
			return -1;
		}
		if (score > other.score) {
			return 1;
		}
		return 0;
	}
};


typedef ds::Highscores<Highscore, 5> MyScores;

class HighscoreService {

public:
	HighscoreService();
	~HighscoreService();
	int add(const Highscore& score);
	void get(int level, Highscore* scores);
	void save();
	void load();
private:	
	MyScores _scores[MAX_LEVELS];
};

