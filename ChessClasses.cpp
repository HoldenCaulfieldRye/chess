#include<iostream>
#include<map>
#include<string>

using namespace std;

#include "ChessBoard.hpp"



/*Piece definitions*/

/*end of Piece definitions*/

/*ChessBoard definitions*/
ChessBoard::ChessBoard() {  //should I make constructor more elaborate?
  initiate();
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

  cerr << "A new chess game is started!" << endl;
}


/*errors can be discovered here, but return type must be void, because this constraint is implicit in main*/
void ChessBoard::submitMove(const string sourceSquare, const string destSquare) {

  /*check that squares given exist*/
  if(!isValid(sourceSquare)) {
    cerr << "invalid source square (rank or file not in range) !" << endl;
    return;
  }
  if(!isValid(destSquare)) {
    cerr << "invalid destination square (rank or file not in range) !" << endl;
    return;
  }

  /*check that there is a piece on source square, that it belongs to player whost turn it is*/
  switch(pieceOnSquare()) {
  case 0:
    return;
  case 1:
    cerr << "There is no piece at position" << sourceSquare << "!" << endl;
    return;
  case 2:
    cerr << "It is not" << notPlayer() << "'s turn to move!" << endl;
    return;
  default:
    cerr << "Unknown error!" << endl;
    return;
  }

  /*check that piece is allowed to move to destination square*/
  if (!canMoveTo(Piece piece, string destSquare)) {
    cerr << whoseTurn << "" << << "" << << endl;
  }

  /*reach here iif move is valid*/
  nextPlayer();
}

bool ChessBoard::isValid(string square) {
  if ((int) square[0] < 65 || (int) square[0] > 72 || (int) square[1] < 49 || (int) square[1] > 56 || (int) square[2] != '\0') {
    return false;
  }
  return true;
}

int ChessBoard::pieceOnSquare(string square) {

  /*check whether there exists a piece on square*/
  MapIt it = boardMap.find(square);
  if (it == boardMap.end())
    return 1;

  /*check that piece on square belongs to player whose turn it is*/
  if (boardMap[square]->whosePiece != whoseTurn)
    return 2;

  return 0;
}

void ChessBoard::Player() {
  if (whoseTurn == white)
    return "White";
  return "Black";

void ChessBoard::notPlayer() {
  if (whoseTurn == white)
    return "Black";
  return "White";
}

void ChessBoard::nextPlayer() {
  if (whoseTurn == white)
    whoseTurn = black;
  whoseTurn = white;
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

