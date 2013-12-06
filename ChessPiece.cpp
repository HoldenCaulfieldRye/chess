#include<iostream>
#include<map>
#include<string>

using namespace std;

#include "ChessBoard.hpp"


/*Piece definitions*/
Piece::Piece() {}

Piece::Piece(string _owner, string _position, ChessBoard *_chboard) : owner(_owner), position(_position), chboard(_chboard), file(_position[0]), rank(_position[1]) {
  validMoves.insert(validMoves.begin(),  "'\0'");
}

/*given where to look, adds potentially valid moves that are not an attack (used by all subPieces apart from Knight)*/
void Piece::classifyMoves(Length length, Direction dir, int *inc, string& move) {
  char r=rank, f=file; 
  do {
    increment(dir, r, f, inc);
    move = concat(r, f);
    /*if move is potentially valid an not an attack, add it*/
    if (chboard->isValidSquare(move) && chboard->pieceOnSquare(move, owner) == NOPIECE) {
      //cerr << move << " is a potentially valid move" << endl;
      if (validMoves.empty()) {
	//cerr << "validMoves is empty" << endl;
	validMoves.insert(validMoves.begin(), move);
      }
      else validMoves.insert(validMoves.end(), move);
    }
    else {
      //cerr << "no valid position from " << move << " onwards, in direction " << dir << endl;
      return;
    }
  } while (length == LONG /*&& count<20*/);
}

/*given where to look, adds potentially valid move that is an attack (used by all subPieces apart from Knight)*/
void Piece::increment(Direction dir, char &coordinate1, char &coordinate2, int *inc) {
  if ( (dir == FORWARDS && owner == "White") || (dir == BACKWARDS && owner == "Black") ) {
    coordinate1 += inc[0];
    coordinate2 += inc[1];
  }
  else {
    coordinate1 -= inc[0];
    coordinate2 -= inc[1];
  }
}

/*helper function for classifyMoves*/
void Piece::classifyLastMove(string move) {
  if (chboard->isValidSquare(move) && chboard->pieceOnSquare(move, owner) == FOE) {
    cerr << move << " is a valid attack move for " << getType() << " at " << position << "!" << endl;
    validMoves.insert(validMoves.begin(), move);
  }
  // else 
    // cerr << move << " is invalid because pieceOnSquare(" << move << ") = " << whosep(chboard->pieceOnSquare(move, owner)) << " or because isValidSquare(" << move << ") = " << chboard->isValidSquare(move) << endl;
}

void Piece::printValidMoves() {
  for(VecIt it=validMoves.begin(); it!=validMoves.end(); it++)
cerr << *it << ", ";
cerr << endl;
}

bool Piece::isValidMove(string square) {
  genValidMoves();
  for (VecIt it = validMoves.begin(); it != validMoves.end(); it++) {
    if(*it == square) {
      //cerr << "move is valid" << endl;
      return true;
    }
  }
  return false;
}

/*tests whether piece can move without putting 'his/her' King in check*/
bool Piece::canMove() {
  string move[2] = {position};
  const bool speculative = true;
  genValidMoves();

  cerr << "canMove() called in (" << position << ", " << getType() << ", " << owner << ")" << endl << "potValidMoves: "; printValidMoves();

  for (VecIt it=validMoves.begin(); it!=validMoves.end(); it++) {
    move[1] = *it;
    cerr << "canMove: does move from " << move[0] << " to " << move[1] << " entail check?" << endl;
    if ( !(chboard->entailsCheck(move, owner, speculative)) )
      return true;
  }
  return false;
}

// Vecs Piece::getValidMoves() {
//   Vecs moves;
//   for(VecIt it=validMoves.begin(); it!=validMoves.end(); it++) {
//     moves.insert(moves.end(), {position, *it});
//   }
//   return moves;
// }

string Piece::getOwner() const {
  return owner;
}

void Piece::setPosition(Cnstring newPos) {
  position = newPos;
  file = newPos[0];
  rank = newPos[1];
}
Piece::~Piece() {}
/*end of Piece definitions*/


/*King definitions*/
King::King() {}
King::King(string _owner, string _position, ChessBoard *_chboard) : Piece::Piece(_owner, _position, _chboard) {}

void King::genValidMoves() {
  //cerr << "genValidMoves called" << endl;
  string move;
  int incr[5][2] = {{1, 0},      //vertical
		    {0, 1},      //horizontal
		    {1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {SINTINEL}};

  validMoves.clear();
  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(SHORT, FORWARDS, incr[i], move);
    classifyLastMove(move);
    classifyMoves(SHORT, BACKWARDS, incr[i], move);
    classifyLastMove(move);
  }
}

string King::getType() const {
  return "King";
}

King::~King() {}
/*end of King definitions*/

/*Queen definitions*/
Queen::Queen() {}
Queen::Queen(string _owner, string _position, ChessBoard *_chboard) : Piece::Piece(_owner, _position, _chboard) {}

void Queen::genValidMoves() {
  //cerr << "genValidMoves called" << endl;
  string move;
  int incr[5][2] = {{1, 0},      //vertical
		    {0, 1},      //horizontal
		    {1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {SINTINEL}};

  validMoves.clear();
  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(LONG, FORWARDS, incr[i], move);
    classifyLastMove(move);
    classifyMoves(LONG, BACKWARDS, incr[i], move);
    classifyLastMove(move);
  }
}

string Queen::getType() const {
  return "Queen";
}

Queen::~Queen() {}
/*end of Queen definitions*/

/* Bishop definitions*/
Bishop::Bishop() {}
Bishop::Bishop(string _owner, string _position, ChessBoard *_chboard) : Piece::Piece(_owner, _position, _chboard) {}

void Bishop::genValidMoves() {
  //cerr << "genValidMoves called" << endl;
  string move;
  int incr[3][2] = {{1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {SINTINEL}};

  validMoves.clear();
  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(LONG, FORWARDS, incr[i], move);
    //cerr << "about to classify last move: " << move << endl;
    classifyLastMove(move);
    classifyMoves(LONG, BACKWARDS, incr[i], move);
    //cerr << "about to classify last move: " << move << endl;
    classifyLastMove(move);
  }
}

string Bishop::getType() const {
  return "Bishop";
}

Bishop::~Bishop() {}
/*end of Bishop definitions*/

/* Knight definitions*/
Knight::Knight() {}
Knight::Knight(string _owner, string _position, ChessBoard *_chboard) : Piece::Piece(_owner, _position, _chboard) {}

void Knight::genValidMoves() {
  //cerr << "genValidMoves called" << endl;
  int incr[8][2] = {{2,1}, {1,2}, {-2,1}, {1,-2}, {2,-1}, {-1,2}, {-2,-1}, {-1,-2}};
  char r, f;
  string move;

  validMoves.clear();
  for(int i=0; i<8; i++) {
    r = rank + incr[i][0]; 
    f = file + incr[i][1];
    move = concat(r, f);
    //cerr << "checking if Knight can reach " << move << " from " << file << rank << endl;
    if (chboard->pieceOnSquare(move, owner) != FRIEND && chboard->isValidSquare(move)) {
      //cerr << move << " is a valid move for " << getType() << " from " << position << endl;
        validMoves.insert(validMoves.begin(), move);
      }
    //else {
        //cerr << "for " << getType() << ", "  << move << " is an invalid move from " << file << rank << " because pieceOnSquare(move) = " << chboard->pieceOnSquare(move) << " or because chboard->isValidSquare(move) = " << chboard->isValidSquare(move) << endl; uncomment around also
    //}
  }
}

string Knight::getType() const {
  return "Knight";
}

Knight::~Knight() {}
/*end of Knight definitions*/

/*Rook definitions*/
Rook::Rook() {}
Rook::Rook(string _owner, string _position, ChessBoard *_chboard) : Piece::Piece(_owner, _position, _chboard) {}

void Rook::genValidMoves() {
  //cerr << "genValidMoves called" << endl;
  string move;
  int incr[3][2] = {{1, 0},      //vertical
		    {0, 1},      //horizontal
		    {SINTINEL}};

  validMoves.clear();
  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(LONG, FORWARDS, incr[i], move);
    classifyLastMove(move);
    classifyMoves(LONG, BACKWARDS, incr[i], move);
    classifyLastMove(move);
  }
}

string Rook::getType() const {
  return "Rook";
}

Rook::~Rook() {}
/*end of Rook definitions*/

/*Pawn definitions*/
Pawn::Pawn() {}

Pawn::Pawn(string _owner, string _position, ChessBoard *_chboard) : Piece(_owner, _position, _chboard) {}

void Pawn::genValidMoves() {
  //cerr << "genValidMoves called" << endl;
  /*because a pawn can only go forward in rank, it is at starting rank iif it hasn't made a first move. I use this property to distinguish the case where it can move forward by 2 squares. Also need to make sure 'move forward by 1 square' is valid*/
  bool firstMove = ((rank=='2' && owner=="White") || (rank=='7' && owner=="Black"));
  string move;
  char r, f;  
  int incr[4][2] = {{1, 0},      //vertical
		    {1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {2, 0}};     //vertical, first move

  validMoves.clear();
  classifyMoves(SHORT, FORWARDS, incr[0], move); //vertical

  /*if access to '1 square in front' is blocked, validMoves is empty*/
  if(firstMove && !validMoves.empty())
    classifyMoves(SHORT, FORWARDS, incr[3], move);

  for (int i=1; i<3; i++) {      //diagonals
    r=rank, f=file;
    increment(FORWARDS, r, f, incr[i]);
    move = concat(r, f);
    classifyLastMove(move);
  }
}

string Pawn::getType() const {
  return "Pawn";
}

Pawn::~Pawn() {} 
/*end of Pawn definitions*/
