#include "display.h"

void display_piece(struct CBoard *position, U64 bit)
{      
        if (position->bitboards[N_PAWN] & bit) {
                printf("p");
        } else if (position->bitboards[N_KNIGHT] & bit) {
                printf("N");
        } else if (position->bitboards[N_BISHOP] & bit) {
                printf("B");
        } else if (position->bitboards[N_ROOK] & bit) {
                printf("R");
        } else if (position->bitboards[N_QUEEN] & bit) {
                printf("Q");
        } else if (position->bitboards[N_KING] & bit) {
                printf("K");
        }
}

void display_square(struct CBoard *position, U64 bit)
{
        if (position->bitboards[N_WHITE] & bit) {
                printf("%s", BLUE);
                display_piece(position, bit);
        }
        else if (position->bitboards[N_BLACK] & bit) {
                printf("%s", RED);
                display_piece(position, bit);
        }
        else {
                printf("x");
        }

        printf("%s ", DEFAULT);   
}

void black_display(struct CBoard *position)
{
        printf("\n      h g f e d c b a\n\n");
        for (int i = 63; i >= 0; --i) {
                if ((i + 1) % 8 == 0) {
                        printf("   %d  ", 8 - (i / 8));
                }

                display_square(position, 1ull << i);

                if (i % 8 == 0) {
                        printf(" %d\n", 8 - (i / 8));
                }
        }
        printf("\n      h g f e d c b a\n\f");   
}

void white_display(struct CBoard *position)
{
        printf("\n      a b c d e f g h\n\n");
        for (int i = 0; i < 64; ++i) {
                if (i % 8 == 0) {
                        printf("   %d  ", 8 - (i / 8));
                }

                display_square(position, 1ull << i);

                if ((i + 1) % 8 == 0) {
                        printf(" %d\n", 8 - (i / 8));
                }
        }
        printf("\n      a b c d e f g h\n\n");
}

void display(struct CBoard *position, enum color side)
{
        printf("\033[4;0H");
        side == WHITE ? white_display(position) : black_display(position);
}