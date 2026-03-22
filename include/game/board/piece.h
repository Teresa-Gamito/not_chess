#include <stdlib.h> 

typedef struct Piece Piece;

typedef enum PieceType {
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
}PieceType;

typedef enum PieceColour {
    BLACK,
    WHITE
}PieceColour;

Piece *piece_create (PieceType type, PieceColour colour);

void piece_destroy (Piece *piece);

void piece_set_type (Piece *piece, PieceType type);

void piece_set_colour (Piece *piece, PieceColour colour);

void 