#include <stdlib.h> 

typedef struct Piece Piece;

typedef enum PieceType
{
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
}PieceType;

typedef enum PieceColour
{
    BLACK,
    WHITE
}PieceColour;

// ========== create ==========
Piece* piece_create(PieceType type, PieceColour colour);

// ========== destroy ==========
void piece_destroy(Piece *piece);

// ========== set ==========
void piece_set_type(Piece* piece, PieceType type);
void piece_set_colour(Piece* piece, PieceColour colour);

// ========== get ==========
PieceColour piece_get_colour(const Piece* piece);
PieceType piece_get_type(const Piece* piece); 

//bool piece_move_valid (Piece* piece, int x, )