#include<iostream>
#include<map>
#include<string>

using namespace std;

#include "ChessBoard.hpp"


/*ChessBoard definitions*/
ChessBoard::ChessBoard() {  //should I make constructor more elaborate?
  initiate();
} 

void ChessBoard::initiate() {  //should I embed this in constructor?
  boardMap["A1"] = new Rook("White", "A1", this);
  boardMap["B1"] = new Knight("White", "B1", this);
  boardMap["C1"] = new Bishop("White", "C1", this);
  boardMap["D1"] = new Queen("White", "D1", this);
  boardMap["E1"] = new King("White", "E1", this);
  boardMap["F1"] = new Bishop("White", "F1", this);
  boardMap["G1"] = new Knight("White", "G1", this);
  boardMap["H1"] = new Rook("White", "H1", this);

  boardMap["A2"] = new Pawn("White", "A2", this);
  boardMap["B2"] = new Pawn("White", "B2", this);
  boardMap["C2"] = new Pawn("White", "C2", this);
  boardMap["D2"] = new Pawn("White", "D2", this);
  boardMap["E2"] = new Pawn("White", "E2", this);
  boardMap["F2"] = new Pawn("White", "F2", this);
  boardMap["G2"] = new Pawn("White", "G2", this);
  boardMap["H2"] = new Pawn("White", "H2", this);

  boardMap["A7"] = new Pawn("Black", "A7", this);
  boardMap["B7"] = new Pawn("Black", "B7", this);
  boardMap["C7"] = new Pawn("Black", "C7", this);
  boardMap["D7"] = new Pawn("Black", "D7", this);
  boardMap["E7"] = new Pawn("Black", "E7", this);
  boardMap["F7"] = new Pawn("Black", "F7", this);
  boardMap["G7"] = new Pawn("Black", "G7", this);
  boardMap["H7"] = new Pawn("Black", "H7", this);

  boardMap["A8"] = new Rook("Black", "A8", this);
  boardMap["B8"] = new Knight("Black", "B8", this);
  boardMap["C8"] = new Bishop("Black", "C8", this);
  boardMap["D8"] = new Queen("Black", "D8", this);
  boardMap["E8"] = new King("Black", "E8", this);
  boardMap["F8"] = new Bishop("Black", "F8", this);
  boardMap["G8"] = new Knight("Black", "G8", this);
  boardMap["H8"] = new Rook("Black", "H8", this);

  whoseTurn = "White";

  cerr << "A new chess game is started!" << endl;
}


/*errors can be discovered here, but return type must be void, because this constraint is implicit in main*/
void ChessBoard::submitMove(const string sourceSquare, const string destSquare) {
  bool attack = false;

  /*check that squares given exist*/
  if(!isValidSquare(sourceSquare)) {
    cerr << "invalid source square (rank or file not in range) !" << endl;
    return;
  }
  else cerr << "check 1: source square exists" << endl; 

  if(!isValidSquare(destSquare)) {
    cerr << "invalid destination square (rank or file not in range) !" << endl;
    return;
  }
  else cerr << "check 2: destination square valid" << endl; 

  /*check that there is a piece on source square, that it belongs to player whose turn it is*/
  switch(pieceOnSquare(sourceSquare)) {
  case NOPIECE:
    cerr << "There is no piece at position " << sourceSquare << "!" << endl;
    return;
  case FOE:
    cerr << "It is not " << notPlayer() << "'s turn to move!" << endl;
    return;
  case FRIEND: 
    cerr << "check 3: there is one of " << whoseTurn << "'s pieces on " << sourceSquare << endl; 
  }

  /*check that there is no piece belonging to player whose turn it is on destination square*/
  WhosePiece wpiece = pieceOnSquare(destSquare);
  if (wpiece == FOE)
    attack = true;
  if (wpiece == FRIEND) {
    cerr << whoseTurn << "'s " << boardMap[sourceSquare]->getType() << " cannot move to" << destSquare << " because he/she would be taking his/her own piece!" << endl;
    return;
  }
  else cerr << "check 4: no friendly piece on destination square" << endl;

  /*check that piece can theoretically get to destination square*/
  if (boardMap[sourceSquare]->isValidMove(destSquare) == false) {
    cerr << whoseTurn << "'s " << boardMap[sourceSquare]->getType() << " cannot move to" << destSquare << "!" << endl;
    return;
  }
  else cerr << "check 5: piece can theoretically get to destination square" << endl;

  /*check that King won't be in check after this move*/
  if (putsOwnKingInCheck(sourceSquare, destSquare)) {
    cerr << whoseTurn << "'s " << boardMap[sourceSquare]->getType() << " cannot move from " << sourceSquare << " because this would put own King in check!" << endl;
    return;
  }
  else cerr << "check 6 FINAL: King won't be in check after this move" << endl;

  /*reach here iif move is valid*/
  cout << whoseTurn << "'s " << boardMap[sourceSquare]->getType() << " moves from " << sourceSquare << " to " << destSquare << endl;
  cout <<"boardMap before move: ";
  for(MapIt it = boardMap.begin(); it!=boardMap.end(); it++)
    cout << "(" << it->first << "," << (it->second)->getType() << "), ";
  cout << endl << "boardMap after move: ";

  boardMap[destSquare] = boardMap[sourceSquare];
  boardMap.erase(sourceSquare);

  for(MapIt it = boardMap.begin(); it!=boardMap.end(); it++)
    cout << "(" << it->first << "," << (it->second)->getType() << "), ";
  cout << endl;

  if (attack)
    boardMap.erase(sourceSquare);

  nextPlayer();
  return;
}


/*do we need isValid when boardMap.find in pieceSquare will fail if move invalid?*/
bool ChessBoard::isValidSquare(const string square) const {
  if (square[0]<'A' || square[0]>'H' || square[1]<'1' || square[1]>'8' || square[2]!='\0') {
    return false;
  }
  return true;
}

WhosePiece ChessBoard::pieceOnSquare(const string square) {
  MapIt it = boardMap.find(square);
  if (it == boardMap.end())
    return NOPIECE;
  if (boardMap[square]->getOwner() == whoseTurn)
    return FRIEND;
  return FOE;
}

string ChessBoard::notPlayer() const {
  if (whoseTurn == "White")
    return "Black";
  return "White";
}

void ChessBoard::nextPlayer() {
  if (whoseTurn == "White") {
    cerr << endl << "White has played, now it's Black's turn" << endl << endl;
    whoseTurn = "Black";
    return;
  }
  whoseTurn = "White";
  cout << endl << "Black has played, now it's White's turn" << endl << endl;
}

bool ChessBoard::putsOwnKingInCheck(const string square, const string destSquare) {
  //save piece on square's type in temp
  //literally move piece on square from sourceSquare to destSquare
  //iterate through mapBoard to see where King is
  //save king's position
  //loop: compute possible moves of all opponent pieces until king's position met or all moves have been computes
  //move piece back to where it was
  //based on how loop stops (value of i), return true or false
  return false;
}

void ChessBoard::resetBoard() {
  boardMap.clear();
  initiate();
}

// ChessBoard::~ChessBoard() {
//   for(MapIt it = boardMap.begin(); it != boardMap.end(); it++) {
//     delete [] it->second;
//     it-> second = NULL;
//   }
// }
/*end of ChessBoard definitions*/
