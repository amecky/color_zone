#include "Highscores.h"


Highscores::Highscores() {
	_maxEntries = MAX_LEVELS * MAX_SCORE_ENTRIES * 2;
}


Highscores::~Highscores() {
}

// --------------------------------------------
// add score if it fits
// --------------------------------------------
int Highscores::add(const Highscore& score) {
	int idx = -1;
	if (score.score > 0) {
		int index = 2 * MAX_SCORE_ENTRIES * (score.level - 1);
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
}

// --------------------------------------------
// get entries by level and mode
// --------------------------------------------
void Highscores::get(int level, Highscore* scores) {
	int index = 2 * MAX_SCORE_ENTRIES * (level - 1);
	for (int i = 0; i < MAX_SCORE_ENTRIES; ++i) {
		scores[i] = _scores[i + index];
	}
}

// --------------------------------------------
// save
// --------------------------------------------
void Highscores::save() {
	FILE* f = fopen("scores", "wb");
	for (int i = 0; i < _maxEntries; ++i ) {
		fwrite(&_scores[i], sizeof(Highscore), 1, f);
	}
	fclose(f);
}

// --------------------------------------------
// load
// --------------------------------------------
void Highscores::load() {
	FILE* f = fopen("scores", "rb");
	if (f) {
		for (int i = 0; i < _maxEntries; ++i) {
			fread(&_scores[i], sizeof(Highscore), 1, f);
		}
		fclose(f);
	}
}