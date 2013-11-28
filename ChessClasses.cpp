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
  boardMap["A1"] = new Rook("White");
  boardMap["B1"] = new Knight("White");
  boardMap["C1"] = new Bishop("White");
  boardMap["D1"] = new Queen("White");
  boardMap["E1"] = new King("White");
  boardMap["F1"] = new Bishop("White");
  boardMap["G1"] = new Knight("White");
  boardMap["H1"] = new Rook("White");

  boardMap["A2"] = new Pawn("White");
  boardMap["B2"] = new Pawn("White");
  boardMap["C2"] = new Pawn("White");
  boardMap["D2"] = new Pawn("White");
  boardMap["E2"] = new Pawn("White");
  boardMap["F2"] = new Pawn("White");
  boardMap["G2"] = new Pawn("White");
  boardMap["H2"] = new Pawn("White");

  boardMap["A7"] = new Pawn("Black");
  boardMap["B7"] = new Pawn("Black");
  boardMap["C7"] = new Pawn("Black");
  boardMap["D7"] = new Pawn("Black");
  boardMap["E7"] = new Pawn("Black");
  boardMap["F7"] = new Pawn("Black");
  boardMap["G7"] = new Pawn("Black");
  boardMap["H7"] = new Pawn("Black");

  boardMap["A8"] = new Rook("Black");
  boardMap["B8"] = new Knight("Black");
  boardMap["C8"] = new Bishop("Black");
  boardMap["D8"] = new Queen("Black");
  boardMap["E8"] = new King("Black");
  boardMap["F8"] = new Bishop("Black");
  boardMap["G8"] = new Knight("Black");
  boardMap["H8"] = new Rook("Black");

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
  switch(pieceOnSquare(sourceSquare)) {
  case 1:
    cerr << "There is no piece at position" << sourceSquare << "!" << endl;
    return;
  case 2:
    cerr << "It is not" << notPlayer() << "'s turn to move!" << endl;
    return;
  case 3:
    return;
  default:
    cerr << "Unknown error!" << endl;
    return;
  }

  /*check that moving piece away from source square doesn't put own king in check*/
  if (putsOwnKingInCheck(sourceSquare)) {
    cerr << whoseTurn << "'s " << piece.getType() << "cannot move from " << sourceSquare << " because this would put own King in check!" << endl;
  }

  /*check that there is no piece belonging to player whose turn it is on destination square*/
  if(pieceOnSquare(destSquare) == 3) {
    cerr << whoseTurn << "'s " << piece.getType() << "cannot move to" << destSquare << " because he/she would be taking his/her own piece!" << endl;
    return;
  }

  /*check that piece can theoretically get to destination square*/
  if (!canMoveTo(piece, destSquare)) {
    cerr << whoseTurn << "'s " << piece.getType() << "cannot move to" << destSquare << "!" << endl;
    return;
  }

  /*reach here iif move is valid*/
  nextPlayer();
  return;
}

bool ChessBoard::isValid(string square) {
  if ((int) square[0] < 65 || (int) square[0] > 72 || (int) square[1] < 49 || (int) square[1] > 56 || (int) square[2] != '\0') {
    return false;
  }
  return true;
}

int ChessBoard::pieceOnSquare(string square) {

  /*does there exists a piece on square?*/
  MapIt it = boardMap.find(square);
  if (it == boardMap.end())
    return 1;

  /*which player does the piece on square belong to?*/
  if (boardMap[square]->owner != whoseTurn)
    return 2;      //belongs to player whose turn it is
  else return 3;   //belongs to player whose turn it isn't
}

bool ChessBoard::putsOwnKingInCheck(string square) {
  //iterate through mapBoard to see where King is
  //save king's position
  //save piece on square's type in temp
  //literally move piece on square from sourceSquare to destSquare
  //compute possible moves of all opponent pieces until king's position met or all moves have been computes
  //based on how loop stops, return true or false
  return false;
}

string ChessBoard::Player() {
  if (whoseTurn == "White")
    return "White";
  return "Black";
}

string ChessBoard::notPlayer() {
  if (whoseTurn == "White")
    return "Black";
  return "White";
}

void ChessBoard::nextPlayer() {
  if (whoseTurn == "White")
    whoseTurn = "Black";
  whoseTurn = "White";
}

bool ChessBoard::canMoveTo(Piece piece, string destSquare) {
  piece.genPossibleMoves();
  for (int i=0; piece.possibleMoves[i] != "'\0'"; i++) {      //should compile
    if (piece.possibleMoves[i] == destSquare)
      return true;
  }
  return false;
}

void ChessBoard::resetBoard() {
}
/*end of ChessBoard definitions*/


/*Piece definitions*/
Piece::Piece() {
  possibleMoves = NULL;
}

Piece::Piece(_owner) : owner(_owner) {}

void Piece::genPossibleMoves() { //gen squares can go to, then check no piece blocking path,
  possibleMoves[0] = "'\0'";
}
/*end of Piece definitions*/


/*King definitions*/
King::King(_owner) : Piece(_owner) {}

void King::genPossibleMoves() : Piece::genPossibleMoves() {
}

string getType() {
  return "King";
}
/*end of King definitions*/

/*Queen definitions*/
Queen::Queen(_owner) : Piece(_owner) {}

void  Queen::genPossibleMoves() : Piece::genPossibleMoves() {
}

string getType() {
  return "Queen";
}
/*end of Queen definitions*/

/* Bishop definitions*/
Bishop::Bishop(_owner) : Piece(_owner) {}

void Bishop::genPossibleMoves() : Piece::genPossibleMoves() {
}

string getType() {
  return "Bishop";
}
/*end of Bishop definitions*/

/* Knight definitions*/
Knight::Knight(_owner) : Piece(_owner) {}

void Knight::genPossibleMoves() : Piece::genPossibleMoves() {
}

string getType() {
  return "Knight";
}
/*end of Knight definitions*/

/*Rook definitions*/
Rook::Rook(_owner) : Piece(_owner) {}

void Rook::genPossibleMoves() : Piece::genPossibleMoves() {
}

string getType() {
  return "Rook";
}
/*end of Rook definitions*/

/*Pawn definitions*/
Pawn::Pawn(_owner) : Piece(_owner) {}

void Pawn::genPossibleMoves() : Piece::genPossibleMoves() {
}

string getType() {
  return "Pawn";
}
/*end of Pawn definitions*/

