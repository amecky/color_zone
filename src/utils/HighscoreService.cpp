#include "HighscoreService.h"
#include <core\log\Log.h>

HighscoreService::HighscoreService() {
	//_maxEntries = MAX_LEVELS * MAX_SCORE_ENTRIES;
}


HighscoreService::~HighscoreService() {
}

// --------------------------------------------
// add score if it fits
// --------------------------------------------
int HighscoreService::add(const Highscore& score) {
	int idx = -1;
	MyScores& myscores = _scores[score.level];
	return myscores.add(score);
	/*
	if (score.score > 0) {
		int index = MAX_SCORE_ENTRIES * (score.level - 1);
		for (int i = 0; i < MAX_SCORE_ENTRIES; ++i) {
			if (idx == -1 && score.score >= _scores[i + index].score) {
				idx = i;
			}
		}
		if (idx != -1) {
			// FIXME: move blocks
			_scores[index + idx] = score;
		}
	}
	return idx;
	*/
}

// --------------------------------------------
// get entries by level and mode
// --------------------------------------------
void HighscoreService::get(int level, Highscore* scores) {
	MyScores& myscores = _scores[level];
	myscores.get(scores, 5);
}

// --------------------------------------------
// save
// --------------------------------------------
void HighscoreService::save() {
	FILE* f = fopen("scores", "rb");
	if (f) {
		Highscore sc[5];
		for (int i = 0; i < MAX_LEVELS; ++i) {
			_scores[i].get(sc, 5);
			fwrite(&sc, sizeof(Highscore) * 5, 1, f);
		}
		fclose(f);
	}	
}

// --------------------------------------------
// load
// --------------------------------------------
void HighscoreService::load() {	
	FILE* f = fopen("scores", "rb");
	if (f) {
		Highscore sc;
		for (int i = 0; i < MAX_LEVELS; ++i) {
			for (int j = 0; j < 5; ++j) {				
				fread(&sc, sizeof(Highscore), 1, f);
				LOG << "reading " << j << " score: " << sc.score << " name: " << sc.name;
				_scores[i].set(j, sc);
			}
		}
		fclose(f);
	}
	
}