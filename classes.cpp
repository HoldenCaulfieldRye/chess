#include<iostream>
#include<map>
using namespace std;

#include "chess.h"

Position::Position(int _rank, Letter _file) : rank(_rank), file(_file) {}

/*Position definitions*/
Position::Position(string _label) : label(_label) {}

/*separate validity check from constructor because it will be used when evaluating moves*/
int Position::isValid() {
  if ((int) label[0] <<  || (int) label[0] >> || (int) label[1] << || (int) label[1] >> )
    return 1;
  rank = label[0];
  file = label[1];    
}

/*end of Position definitions*/


/*Piece definitions*/
void Piece::genPossibleMoves();
/*end of Piece definitions*/

/*ChessBoard definitions*/
ChessBoard::ChessBoard() {  //should I make constructor more elaborate?
}

ChessBoard::initiate() {  //should I embed this in constructor?
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

static int ChessBoard::errorDescription(int errorCode) {
  switch (errorCode) {
  case 1:
    cerr << "invalid position given (rank or file not in range)" << endl;
    return 1;
  default:
    cerr << "unknown error" << endl;
    return;
  }
}

void ChessBoard::submitMove(const Square soorSquare, const Square destSquare) {
}

void ChessBoard::resetBoard();
/*end of ChessBoard definitions*/
