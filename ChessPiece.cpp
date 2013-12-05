#include<iostream>
#include<map>
#include<string>

using namespace std;

#include "ChessBoard.hpp"


/*Piece definitions*/
Piece::Piece() {}

Piece::Piece(const Piece& other) : owner(other.owner), position(other.position), chboard(other.chboard), file(other.file), rank(other.rank), validMoves(other.validMoves) {}

Piece::Piece(string _owner, string _position, ChessBoard *_chboard) : owner(_owner), position(_position), chboard(_chboard), file(_position[0]), rank(_position[1]) {
  validMoves.insert(validMoves.begin(),  "'\0'");
}

void Piece::genValidMoves() { //even if empty, need to keep it, because need it to be virtual, because dunno which subclass until runtime
  cerr << "piece at " << position << " not initiated to a proper piece!" << endl;
}

/*helper function for genValidMoves*/
void Piece::classifyMoves(Length length, Direction dir, int *inc, string& move) {
  char r=rank, f=file; 
  //int count=0;
  do {
    increment(dir, r, f, inc);
    move = concat(r, f);
    //count++;
    if (chboard->isValidSquare(move) && chboard->pieceOnSquare(move) == NOPIECE) {
      cerr << move << " is a valid move for " << getType() << " from " << position << endl;
      validMoves.insert(validMoves.end(), move);
    }
    else {
      cerr << "for " << getType() << " at " << file << rank << ", no valid position from " << move << " onwards, in direction " << dir << endl;
      return;
    }
  } while (length == LONG /*&& count<20*/);
}

/*helper function for classifyMoves*/
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
  if (chboard->isValidSquare(move) && chboard->pieceOnSquare(move) == FOE) {
    cerr << move << " is a valid attack move!" << endl;
    validMoves.insert(validMoves.begin(), move);
  }
  else cerr << move << " is invalid because pieceOnSquare(" << move << ") = " << whosep(chboard->pieceOnSquare(move)) << " or because isValidSquare(" << move << ") = " << chboard->isValidSquare(move) << endl;
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
      cerr << "move is valid" << endl;
      return true;
    }
  }
  return false;
}

string Piece::getOwner() const {
  return owner;
}

void Piece::setPosition(const string newPos) {
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
  cerr << "genValidMoves called" << endl;
  string move;
  int incr[5][2] = {{1, 0},      //vertical
		    {0, 1},      //horizontal
		    {1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {SINTINEL}};

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
  cerr << "genValidMoves called" << endl;
  string move;
  int incr[5][2] = {{1, 0},      //vertical
		    {0, 1},      //horizontal
		    {1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {SINTINEL}};

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
  cerr << "genValidMoves called" << endl;
  string move;
  int incr[3][2] = {{1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {SINTINEL}};

  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(LONG, FORWARDS, incr[i], move);
    cerr << "about to classify last move: " << move << endl;
    classifyLastMove(move);
    classifyMoves(LONG, BACKWARDS, incr[i], move);
    cerr << "about to classify last move: " << move << endl;
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
  cerr << "genValidMoves called" << endl;
  int incr[8][2] = {{2,1}, {1,2}, {-2,1}, {1,-2}, {2,-1}, {-1,2}, {-2,-1}, {-1,-2}};
  char r, f;
  string move;

  for(int i=0; i<8; i++) {
    r = rank + incr[i][0]; 
    f = file + incr[i][1];
    move = concat(r, f);
    cerr << "checking if Knight can reach " << move << " from " << file << rank << endl;
    if (chboard->pieceOnSquare(move) != FRIEND && chboard->isValidSquare(move)) {
        cerr << move << " is a valid move for " << getType() << " from " << position << endl;
        validMoves.insert(validMoves.begin(), move);
      }
      else {
        cerr << "for " << getType() << ", "  << move << " is an invalid move from " << file << rank << " because pieceOnSquare(move) = " << chboard->pieceOnSquare(move) << " or because chboard->isValidSquare(move) = " << chboard->isValidSquare(move) << endl;
      }
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
  cerr << "genValidMoves called" << endl;
  string move;
  int incr[3][2] = {{1, 0},      //vertical
		    {0, 1},      //horizontal
		    {SINTINEL}};

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
  cerr << "genValidMoves called" << endl;
  string move;
  char r=rank, f=file;
  int count=0, incr[4][2] = {{1, 0},      //vertical
			     {1, 1},      //diagonal1
			     {1,-1},      //diagonal2
			     {SINTINEL}};
  do {
    increment(FORWARDS, r, f, incr[0]);
    move = concat(r, f);
    count++;
    if (chboard->isValidSquare(move) && chboard->pieceOnSquare(move) == NOPIECE) {
      cerr << move << " is a valid move for " << getType() << " from " << position << endl;
      validMoves.insert(validMoves.end(), move);
    }
    else {
      cerr << "for " << getType() << " at " << file << rank << ", no valid position from " << move << " in 0 direction" << endl;
      return;
    }
  } while (count<2 && ( (rank=='2' && owner=="White") || (rank=='7' && owner=="Black") ));
  //Pawn can move two squares forward only on first move
  //since it cannot move back in rank, it is only at starting rank on its first move 

  for(int i=1; incr[i][0] != SINTINEL; i++) {       //diagonal attacks
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
