/*
 * DESCRIPTION:
 *      Construction and traversal of search trees in order to find possible
 *      game states that may be evaluated to select a move from the current
 *      position.
 */
#ifndef SEARCH_H_
#define SEARCH_H_

#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "types.h"
#include "position.h"
#include "movegen.h"

U64 gettimems();

void clrinfo(struct sinfo *info);

int inputwaiting();
void readinput(struct sinfo *info);

void checkstop(struct sinfo *info);

U64 perft(struct position *state, struct sinfo *info, int depth);

#endif /* SEARCH_H_ */