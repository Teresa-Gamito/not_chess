#include "../../../include/game/board/piece.h"

struct Piece {
    PieceType type;
    PieceColour colour;
};

struct PieceProperties {

};

Piece *piece_create (PieceType type, PieceColour colour) {
    Piece *piece = malloc(sizeof(Piece));
    piece->type = type;
    piece->colour = colour;
    return piece;
}

void piece_destroy (Piece *piece) {
    free(piece);
}






