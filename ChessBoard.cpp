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

  cout << "A new chess game is started!" << endl;
}


/*Note: a piece belonging to player whose turn it is will be referred to as a 'friendly piece' in the comments, and 'opponent piece' otherwise*/
void ChessBoard::submitMove(const string sourceSquare, const string destSquare) {
  bool attack = false;
  string move[2] = {sourceSquare, destSquare};

  /*check that source square exists*/
  if(!isValidSquare(sourceSquare)) {
    cout << "invalid source square (rank or file not in range) !" << endl;
    return;
  }
  else cerr << "check 1: source square exists" << endl; 


  /*check that there is a friendly piece on source square*/
  switch(pieceOnSquare(sourceSquare)) {
  case NOPIECE:
    cout << "There is no piece at position " << sourceSquare << "!" << endl;
    return;
  case FOE:
    cout << "It is not " << notPlayer() << "'s turn to move!" << endl;
    return;
  case FRIEND: 
    cerr << "check 2: there is one of " << whoseTurn << "'s pieces on " << sourceSquare << endl;
    break;
  }  

  /*check that piece can get to destination square (excluding putting own king in check)*/
  if ( !(boardMap[sourceSquare]->isValidMove(destSquare)) ) {
    /*reach here iif it can't; enquire why*/
    /*check that destination square is valid*/
    if(!isValidSquare(destSquare)) {
      cout << "invalid destination square (rank or file not in range) !" << endl;
      return;
    }

    /*check that there is no friendly piece on destination square*/
    if (pieceOnSquare(destSquare) == FRIEND) {
      cout << whoseTurn << "'s " << boardMap[sourceSquare]->getType() << " cannot move to" << destSquare << " because he/she would be taking his/her own piece!" << endl;
      return;
    }

    /*reach here iif piece is just not capable of making such a move*/
    cout << whoseTurn << "'s " << boardMap[sourceSquare]->getType() << " cannot move to" << destSquare << "!" << endl;
    return;
  }
  else {
    /*test whether move is an attack*/
    if(pieceOnSquare(destSquare) == FOE)
      attack = true;
    cerr << "check 3: piece can get to destination square" << endl; 
  }


  cerr <<"boardMap before move: ";
  for(MapIt it = boardMap.begin(); it!=boardMap.end(); it++)
    cerr << "(" << it->first << "," << (it->second)->getType() << "), "; 
  cerr << endl;

  
  /*test whether move puts friendly King in check. if not, perform move*/
  if (moveEntailsCheck(move, whoseTurn, attack)) {
    cout << whoseTurn << "'s " << boardMap[sourceSquare]->getType() << " cannot move from " << sourceSquare << " because this would put own King in check!" << endl;
    return;
  }
  else cerr << "check 4 FINAL: King won't be in check after this move" << endl;


  /*reach here iif move is completely valid, in which case it has been performed*/
  cout << whoseTurn << "'s " << boardMap[destSquare]->getType() << " moves from " << sourceSquare << " to " << destSquare << " ";


  cerr << endl << "boardMap after move: ";
  for(MapIt it = boardMap.begin(); it!=boardMap.end(); it++)
    cerr << "(" << it->first << "," << (it->second)->getType() << "), ";
  cerr << endl;


  /*test whether move puts opponent in check, checkmate, stalemate, or nothing really*/
  switch (checkOutcome()) {
  case "checkmate":
    cout << notPlayer() << " is in checkmate" << endl;
    return;
  case "stalemate":
    cout << "stalemate" << endl;
    return;
  case "check":
    cout << notPlayer() << " is in check" << endl;
    break;
  case "nothing really":
    break; //do nothing
  }


  cout << endl;
  nextPlayer();

  return;
}


/*do we need isValid when boardMap.find in pieceSquare will fail if move invalid?*/
bool ChessBoard::isValidSquare(const string square) const {
  if (square[0]<'A') {
    cerr << square[0] << "<'A' so ";
    return false;
  }
  if (square[0]>'H') {
    cerr << square[0] << ">'H' so ";
    return false;
  }
  if (square[1]<'1') {
    cerr << square[1] << "<'1' so ";
    return false;
  }
  if (square[1]>'8') {
    cerr << square[1] << ">'8' so ";
    return false;
  }
  if (square[2]!='\0') {
    cerr << square[2] << "!='\0' so ";
    return false;
  }

  // if (square[0]<'A' || square[0]>'H' || square[1]<'1' || square[1]>'8' || square[2]!='\0') {
  //   return false;
  // }
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
    cerr << endl << "White has played, now it's Black's turn" <<  endl;
    whoseTurn = "Black";
    return;
  }
  whoseTurn = "White";
  cerr << endl << "Black has played, now it's White's turn" << endl;
}


/*checks whether a specified move by a specified player puts player's own king in check*/
bool ChessBoard::moveEntailsCheck(const string move[], const string checkedPlayer, bool attack) {
  Piece *temp = NULL;

  /*test whether move is an attack*/
  if (attack)
    temp = boardMap[move[1]];     //make copy of attacked piece

  /*perform move*/
  boardMap[move[1]] = boardMap[move[0]];
  boardMap[move[1]]->setPosition(move[1]);
  boardMap.erase(move[0]);        //erase but don't delete; piece still exists

  /*if friendly King is now in check, output error and undo the move*/
  string frKingPos = findKingPos(whoseTurn);
  if (kingInCheck(frKingPos)) {
    boardMap[move[0]] = boardMap[move[1]];
    boardMap[move[0]]->setPosition(move[0]);

    if (attack) {
      boardMap[move[1]] = temp;   //bring taken piece back to life
      temp = NULL;                //no longer need temp (but don't delete what's on heap)
    }
    else boardMap.erase(move[1]); //!attack so there was nothing in move[1] before move

    return false;
  }
  else {                          //reach here iif move doesn't put friendly king in check
    if (attack) {
      cout << " taking " << notPlayer() << "'s " << boardMap[move[1]]->getType() << endl;
      delete temp;                //permanently remove taken piece from heap
      temp = NULL; 
    }
    return true;
  }
}


/*look for king among pieces belonging to active player*/
string ChessBoard::findKingPos(const string player) {
  for(MapIt it = boardMap.begin(); it!=boardMap.end(); it++) {
    if ((it->second)->getType()=="King" && (it->second)->getOwner()==player) {
      return kingPos;
    }
  }
}


/*checks whether in current state, a specified player's king is in check*/
bool ChessBoard::kingInCheck(const string kingPos) {
  for(MapIt it = boardMap.begin(); it != boardMap.end(); it++) {
    if ((it->second)->getOwner() != player && (it->second)->isValidMove(kingPos))
      return true;
  }
  return false;
}


/*checks whether opponent is in check, checkmate, stalemate, or nothing really*/
string ChessBoard::checkOutcome() {
  bool anAttack = false;
  string opKingPos;                          //position of King of notPlayer()
  Piece *piece = NULL, *king = NULL;

  /*test whether any non-King pieces of opponent can validly move. When testing for stalemate or checkmate, need to look for moves that do not lead to check. criteria are the same for both, so better to look for such moves first.*/
  for (MapIt it = boardMap.begin(); it!=boardMap.end(); it++) {
    &piece = it->second;
    if (piece->getOwner() != whoseTurn) {
      if (piece->getType() != "King") { 
	if (piece->canMove())
	return "nothing really";
      }
      else opKingPos = it->first; //take the opportunity to save King's position
    }
  }
  cerr << "opponent will be in check for any move of its non-King pieces. but is opponent's King even in check?" << endl;

  /*reach here iif opponent will be in check for any move of its non-King pieces*/
  &king = boardMap[opKingPos];
  if (king->canMove())
    return "nothing really";
  else {
    if (kingInCheck())
      return "checkmate";    //king in check, can't move and neither can friend pieces
    else return "stalemate"; //king not in check, can't move and neither can friend pieces
  }
}


void ChessBoard::resetBoard() {
  boardMap.clear();
  initiate();
}

// ChessBoard::~ChessBoard() {
//   for(MapIt it = boardMap.begin(); it != boardMap.end(); it++) {
//     delete it->second;
//     it-> second = NULL;
//   }
// }
/*end of ChessBoard definitions*/
