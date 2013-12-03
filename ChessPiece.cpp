#include<iostream>
#include<map>
#include<string>

using namespace std;

#include "ChessBoard.hpp"


/*Piece definitions*/
Piece::Piece() {}
Piece::Piece(string _owner, string _square, ChessBoard *_chboard) : owner(_owner), square(_square), chboard(_chboard), file(_square[0]), rank(_square[1]) {
  validMoves.insert(validMoves.begin(),  "'\0'");
}

void Piece::genValidMoves() { //even if empty, need to keep it, because need it to be virtual, because dunno which subclass until runtime
  cout << "piece at " << square << " not initiated to a proper piece!" << endl;
}

/*helper function for genValidMoves*/
void Piece::classifyMoves(Length length, Direction dir, int *inc, string& move) {
  char r=rank, f=file;
  do {
    increment(dir, r, f, inc);
    move = concat(r, f);
    if (chboard->isValid(move)==0 && chboard->pieceOnSquare(move) == NOPIECE) { //last condition to make sure !pawn_attack
      cerr << move << " is a valid move for " << getType() << " from " << square << endl;
      cout << "assigning " << move << " to validMoves[" << count << "]" << endl; 
      validMoves.insert(validMoves.end(), move);
      count++;
    }
    else {
      cerr << "for " << getType() << " at " << file << rank << ", no valid position from " << move << " in " << dir << " direction because isValid(" << move << ") = " << chboard->isValid(move) << " and pieceOnSquare(" << move << ") = " << chboard->pieceOnSquare(move) << endl;
      return;
    }
  } while (length == LONG);
}

/*helper function for classifyMoves*/
void Piece::increment(Direction dir, char &coordinate1, char &coordinate2, int *inc) {
  if (dir == FORWARDS) {
    coordinate1 += inc[0];
    coordinate2 += inc[1];
  }
  else {
    coordinate1 -= inc[0];
    coordinate2 -= inc[1];
  }
}

/*helper function for classifyMoves*/
void Piece::classifyLastMove(const string move, int &count) {
  if (chboard->isValid(move)==0 && chboard->pieceOnSquare(move) == FOE)
    validMoves.insert(validMoves.begin(), move);
}

void Piece::printValidMoves() const {
  for(VecIt it=validMoves.begin(); it!=validMoves.end(); it++)
    cout << *it << ", ";
  cout << endl;
}

bool Piece::isValidMove(string square) const {
  for (VecIt it = validMoves.begin(); it != validMoves.end(); it++) {
    if(*it == square)
      return true;
  }
  return false;
}

string Piece::getOwner() const {
  return owner;
}

bool Piece::cpyPossibleMove(int i, string &move) const {
  if (validMoves[i] == "'\0'")
    return false;
  move = validMoves[i];
  return true;
}

Piece::~Piece() {
  if (validMoves)
    delete [] validMoves;
  validMoves = NULL;
}
/*end of Piece definitions*/


/*King definitions*/
King::King() {}
King::King(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void King::genValidMoves() {
  cerr << "genValidMoves called" << endl;
  string move;
  int count=0, incr[5][2] = {{1, 0},      //vertical
			     {0, 1},      //horizontal
			     {1, 1},      //diagonal1
			     {1,-1},      //diagonal2
			     {SINTINEL}};

  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(SHORT, FORWARDS, incr[i], move);
    classifyLastMove(move, count);
    classifyMoves(SHORT, BACKWARDS, incr[i], move);
    classifyLastMove(move, count);
  }
  validMoves = new string;
  validMoves[count] = "'\0'";
}

string King::getType() const {
  return "King";
}
/*end of King definitions*/

/*Queen definitions*/
Queen::Queen() {}
Queen::Queen(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Queen::genValidMoves() {
  cerr << "genValidMoves called" << endl;
  string move;
  int count=0, incr[5][2] = {{1, 0},      //vertical
			     {0, 1},      //horizontal
			     {1, 1},      //diagonal1
			     {1,-1},      //diagonal2
			     {SINTINEL}};

  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(LONG, FORWARDS, incr[i], move, count);
    classifyLastMove(move, count);
    classifyMoves(LONG, BACKWARDS, incr[i], move, count);
    classifyLastMove(move, count);
  }
  validMoves = new string;
  validMoves[count] = "'\0'";
}

string Queen::getType() const {
  return "Queen";
}
/*end of Queen definitions*/

/* Bishop definitions*/
Bishop::Bishop() {}
Bishop::Bishop(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Bishop::genValidMoves() {
  cerr << "genValidMoves called" << endl;
  string move;
  int count=0, incr[3][2] = {{1, 1},      //diagonal1
			     {1,-1},      //diagonal2
			     {SINTINEL}};

  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(LONG, FORWARDS, incr[i], move, count);
    classifyLastMove(move, count);
    classifyMoves(LONG, BACKWARDS, incr[i], move, count);
    classifyLastMove(move, count);
  }
  validMoves = new string;
  validMoves[count] = "'\0'";
}

string Bishop::getType() const {
  return "Bishop";
}
/*end of Bishop definitions*/

/* Knight definitions*/
Knight::Knight() {}
Knight::Knight(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Knight::genValidMoves() {
  cerr << "genValidMoves called" << endl;
  int i, r[8], f[8];
  string move;

  for(int j=-3; j<4; j+=2) {
    if (j<0)
      r[j+3] = f[j+4] = -2;
    else r[j+3] = f[j+4] = 2;

    f[j+3] = r[j+4] = j - r[j+3];

    for(int k=3; k<5; k++) {
      move = concat(r[j+k], f[j+k]);
      if (chboard->pieceOnSquare(move) != FRIEND && chboard->isValid(move)==0) {
	validMoves = new string;
	validMoves[i] = move;
	i++;
      }
    } 
  }
}

string Knight::getType() const {
  return "Knight";
}
/*end of Knight definitions*/

/*Rook definitions*/
Rook::Rook() {}
Rook::Rook(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Rook::genValidMoves() {
  cerr << "genValidMoves called" << endl;
  string move;
  int count=0, incr[3][2] = {{1, 0},      //vertical
			     {0, 1},      //horizontal
			     {SINTINEL}};

  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(LONG, FORWARDS, incr[i], move, count);
    classifyLastMove(move, count);
    classifyMoves(LONG, BACKWARDS, incr[i], move, count);
    classifyLastMove(move, count);
  }
  validMoves = new string;
  validMoves[count] = "'\0'";
}

string Rook::getType() const {
  return "Rook";
}
/*end of Rook definitions*/

/*Pawn definitions*/
Pawn::Pawn() {}

Pawn::Pawn(string _owner, string _square, ChessBoard *_chboard) : Piece(_owner, _square, _chboard) {
}

void Pawn::genValidMoves() {
  cerr << "genValidMoves called" << endl;
  string move;
  char r, f;
  int count=0, incr[4][2] = {{1, 0},      //vertical
			     {1, 1},      //diagonal1
			     {1,-1},      //diagonal2
			     {SINTINEL}};

  classifyMoves(SHORT, FORWARDS, incr[0], move, count);  //vertical move

  for(int i=1; incr[i][0] != SINTINEL; i++) {            //diagonal attacks
    r=rank, f=file;
    increment(FORWARDS, r, f, incr[i]);
    move = concat(r, f);
    classifyLastMove(move, count);
  }
  validMoves = new string;
  validMoves[count] = "'\0'";
}

string Pawn::getType() const {
  return "Pawn";
}
/*end of Pawn definitions*/
