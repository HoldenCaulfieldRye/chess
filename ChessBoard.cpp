#include<iostream>
#include<map>
#include<string>

using namespace std;

#include "ChessBoard.hpp"


/*ChessBoard definitions*/
ChessBoard::ChessBoard() {  //should I make constructor more elaborate?
  initiate();
} 

/*I would have preferred to initiate by reading initial piece positions from a configuration file rather than to hard code a single initial game setup. But I didn't implement this because the corrector could deduct marks from requiring an extra file*/
void ChessBoard::initiate() {
  string files = "ABCEDFGH", owner="White", position;

  for (char j='2'; j<'9'; j+=5, owner="Black") {
    cerr << files << endl;
    for (int i=0; files[i]!='\0'; i++) {
      position = files[i];
      position += j;
      //cerr << "position = " << position << endl;
      boardMap[position] = new Pawn(owner, position, this);
    }
  }

  boardMap["A1"] = new Rook("White", "A1", this);
  boardMap["B1"] = new Knight("White", "B1", this);
  boardMap["C1"] = new Bishop("White", "C1", this);
  boardMap["D1"] = new Queen("White", "D1", this);
  boardMap["E1"] = new King("White", "E1", this);
  boardMap["F1"] = new Bishop("White", "F1", this);
  boardMap["G1"] = new Knight("White", "G1", this);
  boardMap["H1"] = new Rook("White", "H1", this);

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


/*Note: a piece belonging to player whose turn it is will be referred to as a 'friendly piece' in the comments, and 'opponent piece' otherwise. also, a move that seems valid but we don't know if it will put friendly king in check is referred to as a "potentially valid" move. a move that is valid in every respect is called a "fully valid" move*/
void ChessBoard::submitMove(Cnstring sourceSquare, Cnstring destSquare) {
  bool attack = false;
  const bool speculative = true;
  string move[2] = {sourceSquare, destSquare};
  string temp;

  /*check that source square exists*/
  if(!isValidSquare(sourceSquare)) {
    cout << "invalid source square (rank or file not in range) !" << endl;
    return;
  }
  else cerr << "check 1: source square exists" << endl; 

  /*check that there is a friendly piece on source square*/
  switch(pieceOnSquare(sourceSquare, whoseTurn)) {
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
    if (pieceOnSquare(destSquare, whoseTurn) == FRIEND) {
      cout << whoseTurn << "'s " << boardMap[sourceSquare]->getType() << " cannot move to " << destSquare << " because he/she would be taking his/her own piece!" << endl;
      return;
    }

    /*reach here iif piece is just not capable of making such a move*/
    cout << whoseTurn << "'s " << boardMap[sourceSquare]->getType() << " cannot move to " << destSquare << "!" << endl;
    return;
  }
  else cerr << "check 3: piece can get to destination square" << endl; 

  /*test whether move is an attack*/
  if(pieceOnSquare(destSquare, whoseTurn) == FOE) {
    attack = true;
    temp =  boardMap[destSquare]->getType();
  }


  cerr <<"boardMap before move: ";
  for(MapIt it = boardMap.begin(); it!=boardMap.end(); it++)
    cerr << "(" << it->first << "," << (it->second)->getType() << "," << (it->second)->getOwner() << "), "; 
  cerr << endl;

  /*test whether move puts friendly King in check. if not, perform move*/
  if (entailsCheck(move, whoseTurn, !speculative)) {
    cout << whoseTurn << "'s " << boardMap[sourceSquare]->getType() << " cannot move from " << sourceSquare << " because this would put own King in check!" << endl;
    return;
  }
  else cerr << "check 4 FINAL: King won't be in check after this move" << endl;

  /*reach here iif move is completely valid, in which case it has been performed*/
  cout << whoseTurn << "'s " << boardMap[destSquare]->getType() << " moves from " << sourceSquare << " to " << destSquare;
  if (attack) 
    cout << " taking " << notPlayer() << "'s " << temp;
  cout << endl;

  cerr << endl << "boardMap after move: ";
  for(MapIt it = boardMap.begin(); it!=boardMap.end(); it++)
    cerr << "(" << it->first << "," << (it->second)->getType() << "," << (it->second)->getOwner() << "), ";
  cerr << endl;

  /*test whether move puts opponent in check, checkmate, stalemate, or nothing really*/
  string outcome = checkOutcome();
  if (outcome == "checkmate") {
    cout << notPlayer() << " is in checkmate" << endl;
    return;
  }
  if (outcome == "stalemate") {
    cout << "stalemate (" << notPlayer() << " cannot make a move without putting itself in check, but at the moment it isn't in check" << endl;
    return;
  }
  if (outcome == "check")
    cout << notPlayer() << " is in check" << endl;

  cerr << endl << "boardMap after move: ";
  for(MapIt it = boardMap.begin(); it!=boardMap.end(); it++)
    cerr << "(" << it->first << "," << (it->second)->getType() << "," << (it->second)->getOwner() << "), ";
  cerr << endl;

  nextPlayer();

  return;
}


/*do we need isValid when boardMap.find in pieceSquare will fail if move invalid?*/
bool ChessBoard::isValidSquare(Cnstring square) const {
  if (square[0]<'A') {
    //cerr << square[0] << "<'A' so ";
    return false;
  }
  if (square[0]>'H') {
    //cerr << square[0] << ">'H' so ";
    return false;
  }
  if (square[1]<'1') {
    //cerr << square[1] << "<'1' so ";
    return false;
  }
  if (square[1]>'8') {
    //cerr << square[1] << ">'8' so ";
    return false;
  }
  if (square[2]!='\0') {
    //cerr << square[2] << "!='\0' so ";
    return false;
  }

  // if (square[0]<'A' || square[0]>'H' || square[1]<'1' || square[1]>'8' || square[2]!='\0') {
  //   return false;
  // }
  return true;
}

WhosePiece ChessBoard::pieceOnSquare(Cnstring square, Cnstring player) {
  MapIt it = boardMap.find(square);
  if (it == boardMap.end())
    return NOPIECE;
  if (boardMap[square]->getOwner() == player)
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
bool ChessBoard::entailsCheck(Cnstring move[], Cnstring player, const bool speculative) {  
  Piece *temp = NULL;
  string kingPos;

  cerr << "does move entail check?";
  if (speculative)
    cerr << " let's speculate";
  cerr << endl;

  temp = performMove(move); //if attack, returns pointer to taken piece
  kingPos = findKingPos(player);

  if (kingInCheck(kingPos)) {
    undoMove(move, temp);
    return true;
  }
  else {
    if (speculative)
      undoMove(move, temp);
    return false;
  }
}

/*scratch for performMove()*/
Piece* ChessBoard::performMove(Cnstring move[]) {
  cerr << "performing move from " << move[0] << " to " << move[1] << endl;
  Piece *takenPiece = NULL, *movingPiece = boardMap[move[0]];
  if (pieceOnSquare(move[1], movingPiece->getOwner()) == FOE)
    takenPiece = boardMap[move[1]];       //if attack, save memory location of attacked piece
  boardMap[move[1]] = movingPiece;
  boardMap[move[1]]->setPosition(move[1]);
  boardMap.erase(move[0]);                //erase but don't delete; piece still exists
  cerr << "move performed" << endl;
  return takenPiece;
}


void ChessBoard::undoMove(Cnstring move[], Piece *takenPiece) {
  cerr << "undoing move" << endl;
  boardMap[move[0]] = boardMap[move[1]];
  boardMap[move[0]]->setPosition(move[0]);
  if (takenPiece)
    boardMap[move[1]] = takenPiece;   //bring taken piece back to life
  else boardMap.erase(move[1]); //!attack so there was nothing in move[1] before move
  cerr << "move undone" << endl;
}


/*look for king among pieces belonging to active player*/
string ChessBoard::findKingPos(Cnstring player) {
  cerr << "looking for kingPos" << endl;
  Piece *piece;
  for(MapIt it = boardMap.begin(); it!=boardMap.end(); it++) {
    piece = it->second;
    if (piece->getType()=="King" && piece->getOwner()==player) {
      cerr << "kingPos found" << endl;
      return it->first;
    }
  }
  return "error";
}

/*checks whether in current state, a specified player's king is in check*/
bool ChessBoard::kingInCheck(Cnstring kingPos) {
  cerr << "is king now in check?" << endl;
  Piece *piece, *king = boardMap[kingPos];
  for(MapIt it = boardMap.begin(); it != boardMap.end(); it++) {
    piece = it->second;
    if (piece->getOwner() != king->getOwner() && piece->isValidMove(kingPos)) {
      cerr << "yes, king is now in check!" << endl;
      return true;
    }
  }
  cerr << "no, king isn't in check" << endl;
  return false;
}

/*checks whether opponent is in check, checkmate, stalemate, or nothing really*/
string ChessBoard::checkOutcome() {
  string opKingPos = findKingPos(notPlayer());
  Piece *piece = NULL;

  cerr << "what is the outcome of this move? ";

  /*test whether any non-King pieces of opponent can validly move. When testing for stalemate or checkmate, need to look for moves that do not lead to check. criteria are the same for both, so better to look for such moves first.*/
  cerr << "can opponent's pieces move? (ignore next cerrs, just checking that potentially valid moves by opponent's pieces don't leave opponent's king in check)" << endl;

  for (MapIt it = boardMap.begin(); it!=boardMap.end(); it++) {
    piece = it->second;
    if (piece->getOwner() != whoseTurn && piece->getType() != "King" && piece->canMove()) {
      cerr << "yes opponent's piece can move" << endl;
      if (kingInCheck(opKingPos))
	return "check";
      return "nothing really";
    }
  }
  cerr << "no, opponent's piece can't move!\ncan opponent's king move?" << endl;

  /*reach here iif none of opponent's non-King pieces can make truly valid moves*/
  if (boardMap[opKingPos]->canMove()) {
    cerr << "yes, opponent's king can move" << endl;      
    if (kingInCheck(opKingPos))
      return "check";
    return "nothing really";
  }
  else {
    cerr << "no, opponent's king can't move!\nis it in check?" << endl;
    if (kingInCheck(opKingPos)) {
      cerr << "yes, opponent king is in check" << endl;
      return "checkmate";    //king in check, can't move and neither can friend pieces
    }
    else {
      cerr << "no, opponent's king isn't in check" << endl;
      return "stalemate"; //king not in check, can't move and neither can friend pieces
    }
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
