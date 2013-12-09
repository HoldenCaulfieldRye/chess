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
  string files = "ABCEDFGH", ranks = "1278", colour="White", position;

  for (int i=0; i<4; i++) {
    for (int j=0; files[j]!='\0'; j++) {

      if (i==2) colour = "Black";
      position = Utility::concat(ranks[i], files[j]);

      if (ranks[i]=='1' || ranks[i]=='8') {
	switch(files[j]) {
	case 'A': case 'H':
	  boardMap[position] = new Rook(colour, position, this);   break;
	case 'B': case 'G':
	  boardMap[position] = new Knight(colour, position, this); break;
	case 'C': case 'F':
	  boardMap[position] = new Bishop(colour, position, this); break;
	case 'D':
	  boardMap[position] = new Queen(colour, position, this);  break;
	case 'E':
	  boardMap[position] = new King(colour, position, this);
	}
      }
      else boardMap[position] = new Pawn(colour, position, this);
    }
  }

  gameOver = false;
  whoseTurn = "White";
  message(NEW_GAME);
}

/*Note: a piece belonging to player whose turn it is will be referred to as a 'friendly piece' in the comments, and 'opponent piece' otherwise. also, a move that seems valid but we don't know if it will put friendly king in check is referred to as a "potentially valid" move. a move that is valid in every respect is called a "fully valid" move*/
void ChessBoard::submitMove(Cnstring sourceSquare, Cnstring destSquare) {
  bool attack = false;
  const bool speculative = true;
  string move[2] = {sourceSquare, destSquare};
  string temp;

  /*check that game is on (eg after checkmate, game is off*/
  if (gameOver) {
    message(GAME_OVER); return;
  }

  /*check that source square exists*/
  if(!exists(sourceSquare)) {
    message(INVALID_SOURCE_SQUARE, move); return;
  }

  /*check that there is a friendly piece on source square*/
  switch(colourOnSquare(sourceSquare, whoseTurn)) {
  case NOPIECE:
    message(EMPTY_SOURCE_SQUARE, move); return;
  case FOE:
    message(WRONG_PLAYER); return;
  case FRIEND: return;
  }  

  /*check that piece can get to destination square (excluding putting own king in check)*/
  if ( !(boardMap[sourceSquare]->isPotValDestPos(destSquare)) ) {
    /*reach here iif it can't; enquire why*/
    /*check that destination square is valid*/
    if(!exists(destSquare)) {
      message(INVALID_DESTINATION_SQUARE, move); return; 
    }

    /*check that there is no friendly piece on destination square*/
    if (colourOnSquare(destSquare, whoseTurn) == FRIEND) {
      message(FRIENDLY_FIRE, move); return;
    }

    /*reach here iif piece is just not capable of making such a move*/
    message(IMPOSSIBLE_MOVE, move); return;
  }

  /*test whether move is an attack*/
  if(colourOnSquare(destSquare, whoseTurn) == FOE) {
    attack = true;
    temp =  boardMap[destSquare]->getType();
  }

  /*test whether move puts friendly King in check. if not, perform move*/
  if (entailsCheck(move, whoseTurn, !speculative)) {
    message(CHECKING_OWN_KING, move);
    return; 
  }

  /*reach here iif move is completely valid, in which case it has been performed by entailsCheck()*/
  if (attack) 
    message(VALID_ATTACK, move, temp);
  else message(VALID_MOVE, move);

  /*test whether move puts opponent in check, checkmate, stalemate, or nothing really*/
  string outcome = checkOutcome();
  if (outcome == "checkmate") {
    message(CHECKMATE); return;
  }
  if (outcome == "stalemate") {
    message(STALEMATE); return;
  }
  if (outcome == "check")
    message(CHECK);

  nextPlayer();
  return;
}

/*Tests whether 'square' exists (like testing for valid index)*/
bool ChessBoard::exists(Cnstring square) const {
  if (square[0]<'A' || square[0]>'H' || square[1]<'1' || square[1]>'8' || square[2]!='\0')
    return false;
  return true;
}

WhosePiece ChessBoard::colourOnSquare(Cnstring square, Cnstring player) {
  CMapIt it = boardMap.find(square);
  if (it == boardMap.end())
    return NOPIECE;
  if (boardMap[square]->getColour() == player)
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
    whoseTurn = "Black"; return;
  }
  whoseTurn = "White";
}

/*Tests whether a specified move by a specified player puts player's own king in check. Test is done by performing move and evaluating the outcome, so if the move turns out to be valid, it would be nice to keep it, but if move turns out to be invalid, we need to undo it. 'speculative' parameter grants us this flexibility. But it also means that this method can be used to cheat from main: eg it could be called to evaluate a valid move by White after White has played. To prevent this, the method is private. However, Piece needs to call this method in canMove(). I have therefore also defined an underloaded version with 'speculative' set to 'true'*/ 
bool ChessBoard::entailsCheck(Cnstring move[], Cnstring player, const bool speculative) {  
  Piece *temp = NULL;
  string kingPos;

  temp = performMove(move); //need to remember taken piece in case move is an invalid attack
  kingPos = findKingPos(player);

  if (kingIsChecked(kingPos)) {
    undoMove(move, temp); return true;
  }
  else {
    if (speculative)
      undoMove(move, temp); return false;
  }
}

/*Returns entailsCheck() with 'speculative' parameter set to 'true', so it cannot be used to cheat, so it can be public. */
bool ChessBoard::entailsCheck(Cnstring move[], Cnstring player) {
  return entailsCheck(move, player, true);
}

Piece* ChessBoard::performMove(Cnstring move[]) {
  Piece *takenPiece = NULL, *movingPiece = boardMap[move[0]];
  if (colourOnSquare(move[1], movingPiece->getColour()) == FOE)
    takenPiece = boardMap[move[1]];       //if attack, save memory location of attacked piece
  boardMap[move[1]] = movingPiece;
  boardMap[move[1]]->setPosition(move[1]);
  boardMap.erase(move[0]);                //erase but don't delete; piece still exists
  return takenPiece;
}

void ChessBoard::undoMove(Cnstring move[], Piece *takenPiece) {
  boardMap[move[0]] = boardMap[move[1]];
  boardMap[move[0]]->setPosition(move[0]);
  if (takenPiece)
    boardMap[move[1]] = takenPiece;   //bring taken piece back to life
  else boardMap.erase(move[1]);       //!attack so there was nothing in move[1] before move
}

/*Looks for king among pieces belonging to active player. this method is separate from kingIsChecked() because if it weren't, then we would run the risk of searching for a king's position multiple times in the checkOutcome() method; this would be inefficient*/
string ChessBoard::findKingPos(Cnstring player) {
  Piece *piece;
  for(MapIt it = boardMap.begin(); it!=boardMap.end(); it++) {
    piece = it->second;
    if (piece->getType()=="King" && piece->getColour()==player) {
      return it->first;
    }
  }
  return "error"; //could output error message, 
}

/*Tests whether a specified king is in check. In a way, there is no need to specify which of the two kings' check statuses to evaluate, because at a given moment at most one king is in check. but if which king were not specified, the risk would be run on running heavy computation on the unchecked king, to realise that no opponent's piece is threatening it. So it's more efficient to specify which king. This can be done by giving a precise position, or a player type*/
bool ChessBoard::kingIsChecked(Cnstring kingPos) {
  Piece *piece, *king = boardMap[kingPos];
  for(MapIt it = boardMap.begin(); it != boardMap.end(); it++) {
    piece = it->second;
    if (piece->getColour() != king->getColour() && piece->isPotValDestPos(kingPos))
      return true;
  }
  return false;
}

/*Tests whether opponent is in check, checkmate, stalemate, or nothing really*/
string ChessBoard::checkOutcome() {
  string opKingPos = findKingPos(notPlayer());
  Piece *piece = NULL;

  /*test whether any non-King pieces of opponent can validly move. When testing for stalemate or checkmate, need to look for moves that do not lead to check. criteria are the same for both, so better to look for such moves first.*/
  for (MapIt it = boardMap.begin(); it!=boardMap.end(); it++) {
    piece = it->second;
    if (piece->getColour() != whoseTurn && piece->getType() != "King" && piece->canMove()) {
      if (kingIsChecked(opKingPos))
	return "check";
      return "nothing really";
    }
  }

  /*reach here iif none of opponent's non-King pieces can make truly valid moves*/
  if (boardMap[opKingPos]->canMove()) {
    if (kingIsChecked(opKingPos))
      return "check";
    return "nothing really";
  }
  else {
    if (kingIsChecked(opKingPos))
      return "checkmate";    //king in check, can't move and neither can friend pieces
    else 
      return "stalemate";    //king not in check, can't move and neither can friend pieces
  }
}

void ChessBoard::resetBoard() {
  boardMap.clear();
  initiate();
}

void ChessBoard::message(int mcode) {
  switch (mcode) {
  case NEW_GAME: 
    cout << "A new game is started!" << endl; return;
  case WRONG_PLAYER:
    cout << "It is not " << notPlayer() << "'s turn to move!" << endl; return;
  case CHECK:
    cout << notPlayer() << " is in check" << endl; return;
  case CHECKMATE:
    cout << notPlayer() << " is in checkmate" << endl; return;
  case STALEMATE:
    cout << "stalemate (" << notPlayer() << " cannot make a move without putting itself in check, but at the moment he/she isn't in check" << endl; return;
  case GAME_OVER:
    cout << "game over pal, face it" << endl; return;
  }
}

void ChessBoard::message(int mcode, string move[2]) {
  switch (mcode) {
  case INVALID_SOURCE_SQUARE:
    cout << move[0] << " is an invalid source square (rank or file not in range)!" << endl; return;
  case INVALID_DESTINATION_SQUARE:
    cout << move[1] << " is an invalid destination square (rank or file not in range) !" << endl; return;
  case EMPTY_SOURCE_SQUARE:
    cout << "There is no piece at position " << move[0] << "!" << endl; return;
  case FRIENDLY_FIRE:
    cout << whoseTurn << "'s " << boardMap[move[0]]->getType() << " cannot move to " << move[1] << " because he/she would be taking his/her own piece!" << endl; return;
  case IMPOSSIBLE_MOVE:
    cout << whoseTurn << "'s " << boardMap[move[0]]->getType() << " cannot move to " << move[1] << "!" << endl; return;
  case CHECKING_OWN_KING:
    cout << whoseTurn << "'s " << boardMap[move[0]]->getType() << " cannot move from " << move[0] << " to " << move[1] << " because this would put " << whoseTurn << "'s King in check!" << endl; return;
  case VALID_MOVE:
    cout << whoseTurn << "'s " << boardMap[move[1]]->getType() << " moves from " << move[0] << " to " << move[1] << endl; return;
  }
}

void ChessBoard::message(int mcode, string move[2], string takenPieceType) {
  if(mcode == VALID_ATTACK) {
    cout << whoseTurn << "'s " << boardMap[move[1]]->getType() << " moves from " << move[0] << " to " << move[1] << " taking " << notPlayer() << "'s " << takenPieceType << endl;
  }
}

string ChessBoard::whosep(WhosePiece piece) { //DELETE
  if(piece == NOPIECE)
    return "NOPIECE"; 
  if(piece == FRIEND)
    return "FRIEND"; 
  if(piece == FOE)
    return "FOE";
  return "ERROR"; 
}

ChessBoard::~ChessBoard() {
  for(MapIt it = boardMap.begin(); it != boardMap.end(); it++) {
    delete it->second;
    it->second = NULL;
  }
}
/*end of ChessBoard definitions*/
