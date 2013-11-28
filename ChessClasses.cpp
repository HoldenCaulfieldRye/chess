#include<iostream>
#include<map>
#include<string>

using namespace std;

#include "ChessBoard.hpp"


/*Position definitions*/
Position::Position(string _label) : label(_label) {}

/*separate validity check from constructor because it will be used when evaluating moves*/
int Position::isValid() {
  if ((int) label[0] < 65 || (int) label[0] > 72 || (int) label[1] < 49 || (int) label[1] > 56 || (int) label[2] != '\0')
    return 1;
  rank = label[0];
  file = label[1];
  return 0;
}
/*end of Position definitions*/


/*Piece definitions*/

/*end of Piece definitions*/

/*ChessBoard definitions*/
ChessBoard::ChessBoard() {  //should I make constructor more elaborate?
}

void ChessBoard::initiate() {  //should I embed this in constructor?
  boardMap["A1"] = new Rook;
  boardMap["B1"] = new Knight;
  boardMap["C1"] = new Bishop;
  boardMap["D1"] = new Queen;
  boardMap["E1"] = new King;
  boardMap["F1"] = new Bishop;
  boardMap["G1"] = new Knight;
  boardMap["H1"] = new Rook;

  boardMap["A2"] = new Pawn;
  boardMap["B2"] = new Pawn;
  boardMap["C2"] = new Pawn;
  boardMap["D2"] = new Pawn;
  boardMap["E2"] = new Pawn;
  boardMap["F2"] = new Pawn;
  boardMap["G2"] = new Pawn;
  boardMap["H2"] = new Pawn;

  boardMap["A7"] = new Pawn;
  boardMap["B7"] = new Pawn;
  boardMap["C7"] = new Pawn;
  boardMap["D7"] = new Pawn;
  boardMap["E7"] = new Pawn;
  boardMap["F7"] = new Pawn;
  boardMap["G7"] = new Pawn;
  boardMap["H7"] = new Pawn;

  boardMap["A8"] = new Rook;
  boardMap["B8"] = new Knight;
  boardMap["C8"] = new Bishop;
  boardMap["D8"] = new Queen;
  boardMap["E8"] = new King;
  boardMap["F8"] = new Bishop;
  boardMap["G8"] = new Knight;
  boardMap["H8"] = new Rook;
}

int ChessBoard::errorDescription(int errorCode) {
  switch (errorCode) {
  case 1:
    cerr << "invalid position given (rank or file not in range)" << endl;
    return 1;
  default:
    cerr << "unknown error" << endl;
    return -1;
  }
}

/*errors can be discovered here, but return type must be void, because this constraint is implicit in main*/
void ChessBoard::submitMove(const string sourceSquare, const string destSquare) {
}

void ChessBoard::resetBoard() {
}
/*end of ChessBoard definitions*/


/*Piece definitions*/
Piece::Piece() {
}
/*end of Piece definitions*/


/*King definitions*/
void King::genPossibleMoves() {
}

/*end of King definitions*/

/*Queen definitions*/
void  Queen::genPossibleMoves() {
}

/*end of Queen definitions*/

/* Bishop definitions*/
void Bishop::genPossibleMoves() {
}

/*end of Bishop definitions*/

/* Knight definitions*/
void Knight::genPossibleMoves() {
}

/*end of Knight definitions*/

/*Rook definitions*/
void Rook::genPossibleMoves() {
}

/*end of Rook definitions*/

/*Pawn definitions*/
void Pawn::genPossibleMoves() {
}

/*end of Pawn definitions*/

