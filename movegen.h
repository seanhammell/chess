#ifndef MOVEGEN_H_
#define MOVEGEN_H_

#include "./types.h"

struct MovementArrays {
  U8 edge_dist[8][64];
  U64 white_pawns[64];
  U64 black_pawns[64];
  U64 knights[64];
};

void InitDistances(struct MovementArrays *lookup);
void InitWhitePawnMovement(struct MovementArrays *lookup);
void InitBlackPawnMovement(struct MovementArrays *lookup);

void InitMovementArrays(struct MovementArrays *lookup);

#endif  // MOVEGEN_H_
