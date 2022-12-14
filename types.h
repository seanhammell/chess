/* 
 * DESCRIPTION:
 *      Useful types and definitions for manipulating the state of a
 *      board and gathering information on a position.
 */
#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include <sys/time.h>

typedef uint16_t U16;
typedef uint64_t U64;

#define RANK_1 0xFFull
#define RANK_2 (RANK_1 << 8)
#define RANK_3 (RANK_1 << 16)
#define RANK_4 (RANK_1 << 24)
#define RANK_5 (RANK_1 << 32)
#define RANK_6 (RANK_1 << 40)
#define RANK_7 (RANK_1 << 48)
#define RANK_8 (RANK_1 << 56)

#define FILE_A 0x0101010101010101ull
#define FILE_B (FILE_A << 1)
#define FILE_C (FILE_A << 2)
#define FILE_D (FILE_A << 3)
#define FILE_E (FILE_A << 4)
#define FILE_F (FILE_A << 5)
#define FILE_G (FILE_A << 6)
#define FILE_H (FILE_A << 7)

#define WHITE_OO_GAP 0x60ull
#define WHITE_OOO_GAP 0xeull
#define BLACK_OO_GAP WHITE_OO_GAP << 56
#define BLACK_OOO_GAP WHITE_OOO_GAP << 56

#define MOVETYPE_MASK (3ull << 14)

#define min(A, B) ((A) < (B) ? (A) : (B))
#define flip(S) ((S) == WHITE ? BLACK : WHITE)

enum movetype {
	NORMAL,
	PROMOTION = 1 << 14,
	EN_PASSANT = 2 << 14,
	CASTLING = 3 << 14
};

enum color {
	WHITE,
	BLACK
};

enum castling {
	NO_CASTLING,
	WHITE_OO,
	WHITE_OOO = WHITE_OO << 1,
	BLACK_OO = WHITE_OO << 2,
	BLACK_OOO = WHITE_OO << 3,

	WHITE_CASTLE = WHITE_OO | WHITE_OOO,
	BLACK_CASTLE = BLACK_OO | BLACK_OOO
};

enum piece {
	PAWN = 2,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	OCCUPIED,
	EMPTY,

	NO_PIECE
};

enum square {
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8,

	NULL_SQ
};

struct raylookup {
	U64 north[64];
	U64 east[64];
	U64 south[64];
	U64 west[64];

	U64 northeast[64];
	U64 southeast[64];
	U64 southwest[64];
	U64 northwest[64];
};

struct position {
	struct raylookup *rays;
	U64 history[1024][3];
	enum piece piecelist[64];
	U64 boards[10];
	enum color side;
	enum castling rights;
	enum square eptarget;
	int rule50;
	int plynb;
	int hisply;
};

struct sinfo {
        int depth;
	U64 nodes;

	int tset;
	U64 tstart;
	U64 tstop;

	int stop;
	int quit;
};

#endif /* TYPES_H_ */
