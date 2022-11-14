#include "bitboard.h"

/* 
 * Pawn Single Push
 * 
 * DESCRIPTION:
 *      Calculate pawn single push targets.
 */
U64 push(U64 pawns, U64 empty, enum color side) {
	U64 shift = 0;

	if (side == WHITE) {
		shift |= north(pawns) & empty;
		assert(shift == ((pawns << 8) & empty));
		assert((shift & RANK_2) == 0);
	} else {
		shift |= south(pawns) & empty;
		assert(shift == ((pawns >> 8) & empty));
		assert((shift & RANK_7) == 0);
	}

	return shift;
}

/* 
 * Pawn Double Push
 *
 * DESCRIPTION:
 * 	Calculate pawn double push targets.
 */
U64 dblpush(U64 pawns, U64 empty, enum color side) {
	U64 dblshift = push(pawns, empty, side);
	dblshift = push(dblshift, empty, side);

	if (side == WHITE) {
		dblshift &= RANK_4;
		assert((dblshift & (~RANK_4)) == 0);
	} else {
		dblshift &= RANK_5;
		assert((dblshift & (~RANK_5)) == 0);
	}

	return dblshift;
}

/* 
 * Pawn Attack
 *
 * DESCRIPTION:
 * 	Calculate pawn attack targets.
 */
U64 pattack(U64 pawns, U64 enemies, enum color side) {
	U64 targets = 0;

	if (side == WHITE) {
		targets |= northeast(pawns) & enemies;
		targets |= northwest(pawns) & enemies;
		assert((targets & (~enemies)) == 0);
	} else {
		targets |= southeast(pawns) & enemies;
		targets |= southwest(pawns) & enemies;
		assert((targets & (~enemies)) == 0);
	}

	return targets;
}

/*
 * Pawn Moves
 *
 * DESCRIPTION:
 * 	Calculate all pawn targets in a given position.
 */
U64 ptargets(enum square sq, struct position *state) {
	U64 targets = 0;
	U64 pawn = 1ull << sq;
	U64 empty = state->boards[EMPTY];
	U64 enemies = state->boards[flip(state->side)];

	targets |= push(pawn, empty, state->side);
	targets |= dblpush(pawn, empty, state->side);
	targets |= pattack(pawn, enemies, state->side);

	return targets;
}

/*
 * Knight Moves
 *
 * DESCRIPTION:
 * 	Calculate all knight targets in a given position. 
 */
U64 ntargets(enum square sq, struct position *state) {
	U64 knight = 1ull << sq;
	enum color side = state->side == WHITE ? WHITE : BLACK;
	U64 allies = state->boards[side];

	U64 targets = 0;
	
	U64 eshift = east(knight);
	U64 wshift = west(knight);
	targets |= ((eshift | wshift) << 16) & (~allies);
	targets |= ((eshift | wshift) >> 16) & (~allies);

	eshift = east(eshift);
	wshift = west(wshift);
	targets |= ((eshift | wshift) << 8) & (~allies);
	targets |= ((eshift | wshift) >> 8) & (~allies);
	assert((targets & allies) == 0);

	return targets;
}

/*
 * Bishop Moves
 *
 * DESCRIPtargets
 * 	Calculate all bishop targets in a given position.
 */
U64 btargets(enum square sq, struct position *state) {
	U64 occupied = state->boards[OCCUPIED];
	enum color side = state->side == WHITE ? WHITE : BLACK;
	U64 enemies = state->boards[flip(side)];

	U64 attack;
	U64 blockers;
	U64 targets = 0;

	attack = state->rays->northeast[sq];
	blockers = attack & occupied;
	if(blockers) {
		int bsq = __builtin_ctzll(blockers);
		blockers = 1ull << bsq;
		assert((blockers & occupied) != 0);
		if (blockers & (occupied ^ enemies)) { bsq -= 9; }
		attack ^= state->rays->northeast[bsq];
		assert((attack & (occupied ^ enemies)) == 0);
	}
	targets |= attack;

	attack = state->rays->southeast[sq];
	blockers = attack & occupied;
	if(blockers) {
		int bsq = 63 - __builtin_clzll(blockers);
		blockers = 1ull << bsq;
		assert((blockers & occupied) != 0);
		if (blockers & (occupied ^ enemies)) { bsq += 7; }
		attack ^= state->rays->southeast[bsq];
		assert((attack & (occupied ^ enemies)) == 0);
	}
	targets |= attack;

	attack = state->rays->southwest[sq];
	blockers = attack & occupied;
	if(blockers) {
		int bsq = 63 - __builtin_clzll(blockers);
		blockers = 1ull << bsq;
		assert((blockers & occupied) != 0);
		if (blockers & (occupied ^ enemies)) { bsq += 9; }
		attack ^= state->rays->southwest[bsq];
		assert((attack & (occupied ^ enemies)) == 0);
	}
	targets |= attack;

	attack = state->rays->northwest[sq];
	blockers = attack & occupied;
	if(blockers) {
		int bsq = __builtin_ctzll(blockers);
		blockers = 1ull << bsq;
		assert((blockers & occupied) != 0);
		if (blockers & (occupied ^ enemies)) { bsq -= 7; }
		attack ^= state->rays->northwest[bsq];
		assert((attack & (occupied ^ enemies)) == 0);
	}
	targets |= attack;

	return targets;
}

/*
 * Rook Moves
 *
 * DESCRIPTION:
 * 	Calculate all rook targets in a given position.
 */
U64 rtargets(enum square sq, struct position *state) {
	U64 occupied = state->boards[OCCUPIED];
	enum color side = state->side == WHITE ? WHITE : BLACK;
	U64 enemies = state->boards[flip(side)];

	U64 attack;
	U64 blockers;
	U64 targets = 0;

	attack = state->rays->north[sq];
	blockers = attack & occupied;
	if(blockers) {
		int bsq = __builtin_ctzll(blockers);
		blockers = 1ull << bsq;
		assert((blockers & occupied) != 0);
		if (blockers & (occupied ^ enemies)) { bsq -= 8; }
		attack ^= state->rays->north[bsq];
		assert((attack & (occupied ^ enemies)) == 0);
	}
	targets |= attack;

	attack = state->rays->east[sq];
	blockers = attack & occupied;
	if(blockers) {
		int bsq = __builtin_ctzll(blockers);
		blockers = 1ull << bsq;
		assert((blockers & occupied) != 0);
		if (blockers & (occupied ^ enemies)) { bsq -= 1; }
		attack ^= state->rays->east[bsq];
		assert((attack & (occupied ^ enemies)) == 0);
	}
	targets |= attack;

	attack = state->rays->south[sq];
	blockers = attack & occupied;
	if(blockers) {
		int bsq = 63 - __builtin_clzll(blockers);
		blockers = 1ull << bsq;
		assert((blockers & occupied) != 0);
		if (blockers & (occupied ^ enemies)) { bsq += 8; }
		attack ^= state->rays->south[bsq];
		assert((attack & (occupied ^ enemies)) == 0);
	}
	targets |= attack;

	attack = state->rays->west[sq];
	blockers = attack & occupied;
	if(blockers) {
		int bsq = 63 - __builtin_clzll(blockers);
		blockers = 1ull << bsq;
		assert((blockers & occupied) != 0);
		if (blockers & (occupied ^ enemies)) { bsq += 1; }
		attack ^= state->rays->west[bsq];
		assert((attack & (occupied ^ enemies)) == 0);
	}
	targets |= attack;

	return targets;
}

/*
 * Queen Moves
 *
 * DESCRIPTION:
 * 	Calculate all queen moves in a given position.
 */
U64 qtargets(enum square sq, struct position *state) {
	U64 targets = 0;

	targets |= btargets(sq, state);
	targets |= rtargets(sq, state);

	return targets;
}

/*
 * King Moves
 *
 * DESCRIPTION:
 * 	Calculate all king targets in a given position.
 */
U64 ktargets(enum square sq, struct position *state) {
	U64 king = 1ull << sq;
	enum color side = state->side == WHITE ? WHITE : BLACK;
	U64 allies = state->boards[side];

	U64 targets = 0;

	targets |= west(king) | king | east(king);
	assert((targets & (~((king >> 1) | king | (king << 1)))) == 0);
	targets |= north(targets) | south(targets);
	targets &= (~allies);

	return targets;
}

/*
 * In Check
 *
 * DESCRIPTION:
 * 	Test if the current side's king is in check.
 */
int incheck(enum square kingsq, struct position *state) {
	U64 king;
	if (kingsq == NULL_SQ) {
		king = state->boards[KING] & state->boards[state->side];
		kingsq = __builtin_ctzll(king);
	} else {
		king = 1ull << kingsq;
	}
	assert(king != 0);
        U64 enemies = state->boards[flip(state->side)];
	assert((king & enemies) == 0);

        U64 brays = btargets(kingsq, state);
        U64 rrays = rtargets(kingsq, state);

        if (brays & (state->boards[BISHOP] & enemies) ||
            rrays & (state->boards[ROOK] & enemies) ||
            (brays | rrays) & (state->boards[QUEEN] & enemies) ||
            ntargets(kingsq, state) & (state->boards[KNIGHT] & enemies)) {
                return 1;
        }


	U64 enemypawns = enemies & state->boards[PAWN];
	switch (state->side) {
	case WHITE:
		if ((northeast(king) | northwest(king)) & enemypawns) {
			return 1;
		}
		break;
	case BLACK:
		if ((southeast(king) | southwest(king)) & enemypawns) {
			return 1;
		}
		break;
	}
        return 0;
}
