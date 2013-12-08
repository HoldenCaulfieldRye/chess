#include<iostream>
#include<map>
#include<string>

using namespace std;

#include "ChessBoard.hpp"


/*Piece definitions*/
Piece::Piece() {}

Piece::Piece(string _colour, string _position, ChessBoard *_chboard) : colour(_colour), position(_position), chboard(_chboard), file(_position[0]), rank(_position[1]) {
  potValDestPos.insert(potValDestPos.begin(),  "'\0'");
}

/*Given HOW to search, adds ALL potentially valid destination positions which do not currently host an opponent's piece. 'inc' gives values with which to increment rank and fileThis method is used by all sub-Pieces apart from Knight*/
void Piece::classifyDestPos(Range range, Direction dir, int *inc, string &move) {
  char r=rank, f=file; 
  do {
    increment(dir, r, f, inc);
    move = concat(r, f);
    /*if destination position is potentially valid an not an attack, add it*/
    if (chboard->isValidSquare(move) && chboard->pieceOnSquare(move, colour) == NOPIECE) {
      //cerr << move << " is a potentially valid move" << endl;
      if (potValDestPos.empty()) {
	//cerr << "potValDestPos is empty" << endl;
	potValDestPos.insert(potValDestPos.begin(), move);
      }
      else potValDestPos.insert(potValDestPos.end(), move);
    }
    else {
      //cerr << "no valid position from " << move << " onwards, in direction " << dir << endl;
      return;
    }
  } while (range == LONG);
}

/*helper function for classifyDestPos(). 'inc' gives values with which to increment rank and file*/
void Piece::increment(Direction dir, char &coordinate1, char &coordinate2, int *inc) {
  if ( (dir == FORWARDS && colour == "White") || (dir == BACKWARDS && colour == "Black") ) {
    coordinate1 += inc[0];
    coordinate2 += inc[1];
  }
  else {
    coordinate1 -= inc[0];
    coordinate2 -= inc[1];
  }
}

/*given WHICH square to evaluate, adds AT MOST ONE potentially valid destination position which currently hosts an opponent's piece. This method is used by all sub-Pieces apart from Knight*/
void Piece::classifyLastDestPos(string move) {
  if (chboard->isValidSquare(move) && chboard->pieceOnSquare(move, colour) == FOE) {
    cerr << move << " is a valid attack move for " << getType() << " at " << position << "!" << endl;
    potValDestPos.insert(potValDestPos.begin(), move);
  }
  // else 
    // cerr << move << " is invalid because pieceOnSquare(" << move << ") = " << whosep(chboard->pieceOnSquare(move, colour)) << " or because isValidSquare(" << move << ") = " << chboard->isValidSquare(move) << endl;
}

void Piece::printPotValDestPos() {
  for(VecIt it=potValDestPos.begin(); it!=potValDestPos.end(); it++)
cerr << *it << ", ";
cerr << endl;
}

bool Piece::isPotValDestPos(string square) {
  genPotValDestPos();
  for (VecIt it = potValDestPos.begin(); it != potValDestPos.end(); it++) {
    if(*it == square) {
      //cerr << "move is valid" << endl;
      return true;
    }
  }
  return false;
}

/*tests whether piece can move without putting 'his/her' King in check. computationally heavy.*/
bool Piece::canMove() {
  string move[2] = {position};
  const bool speculative = true;
  genPotValDestPos();

  cerr << "canMove() called in (" << position << ", " << getType() << ", " << colour << ")" << endl << "potValidMoves: "; printPotValDestPos();

  for (VecIt it=potValDestPos.begin(); it!=potValDestPos.end(); it++) {
    move[1] = *it;
    cerr << "canMove: does move from " << move[0] << " to " << move[1] << " entail check?" << endl;
    if ( !(chboard->entailsCheck(move, colour, speculative)) )
      return true;
  }
  return false;
}

string Piece::getColour() const {
  return colour;
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
King::King(string _colour, string _position, ChessBoard *_chboard) : Piece::Piece(_colour, _position, _chboard) {}

void King::genPotValDestPos() {
  //cerr << "genPotValDestPos called" << endl;
  string move;
  int incr[5][2] = {{1, 0},      //vertical
		    {0, 1},      //horizontal
		    {1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {SINTINEL}};

  potValDestPos.clear();
  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyDestPos(SHORT, FORWARDS, incr[i], move);
    classifyLastDestPos(move);
    classifyDestPos(SHORT, BACKWARDS, incr[i], move);
    classifyLastDestPos(move);
  }
}

string King::getType() const {
  return "King";
}

King::~King() {}
/*end of King definitions*/

/*Queen definitions*/
Queen::Queen() {}
Queen::Queen(string _colour, string _position, ChessBoard *_chboard) : Piece::Piece(_colour, _position, _chboard) {}

void Queen::genPotValDestPos() {
  //cerr << "genPotValDestPos called" << endl;
  string move;
  int incr[5][2] = {{1, 0},      //vertical
		    {0, 1},      //horizontal
		    {1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {SINTINEL}};

  potValDestPos.clear();
  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyDestPos(LONG, FORWARDS, incr[i], move);
    classifyLastDestPos(move);
    classifyDestPos(LONG, BACKWARDS, incr[i], move);
    classifyLastDestPos(move);
  }
}

string Queen::getType() const {
  return "Queen";
}

Queen::~Queen() {}
/*end of Queen definitions*/

/* Bishop definitions*/
Bishop::Bishop() {}
Bishop::Bishop(string _colour, string _position, ChessBoard *_chboard) : Piece::Piece(_colour, _position, _chboard) {}

void Bishop::genPotValDestPos() {
  //cerr << "genPotValDestPos called" << endl;
  string move;
  int incr[3][2] = {{1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {SINTINEL}};

  potValDestPos.clear();
  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyDestPos(LONG, FORWARDS, incr[i], move);
    //cerr << "about to classify last move: " << move << endl;
    classifyLastDestPos(move);
    classifyDestPos(LONG, BACKWARDS, incr[i], move);
    //cerr << "about to classify last move: " << move << endl;
    classifyLastDestPos(move);
  }
}

string Bishop::getType() const {
  return "Bishop";
}

Bishop::~Bishop() {}
/*end of Bishop definitions*/

/* Knight definitions*/
Knight::Knight() {}
Knight::Knight(string _colour, string _position, ChessBoard *_chboard) : Piece::Piece(_colour, _position, _chboard) {}

void Knight::genPotValDestPos() {
  //cerr << "genPotValDestPos called" << endl;
  int incr[8][2] = {{2,1}, {1,2}, {-2,1}, {1,-2}, {2,-1}, {-1,2}, {-2,-1}, {-1,-2}};
  char r, f;
  string move;

  potValDestPos.clear();
  for(int i=0; i<8; i++) {
    r = rank + incr[i][0]; 
    f = file + incr[i][1];
    move = concat(r, f);
    //cerr << "checking if Knight can reach " << move << " from " << file << rank << endl;
    if (chboard->pieceOnSquare(move, colour) != FRIEND && chboard->isValidSquare(move)) {
      //cerr << move << " is a valid move for " << getType() << " from " << position << endl;
        potValDestPos.insert(potValDestPos.begin(), move);
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
Rook::Rook(string _colour, string _position, ChessBoard *_chboard) : Piece::Piece(_colour, _position, _chboard) {}

void Rook::genPotValDestPos() {
  //cerr << "genPotValDestPos called" << endl;
  string move;
  int incr[3][2] = {{1, 0},      //vertical
		    {0, 1},      //horizontal
		    {SINTINEL}};

  potValDestPos.clear();
  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyDestPos(LONG, FORWARDS, incr[i], move);
    classifyLastDestPos(move);
    classifyDestPos(LONG, BACKWARDS, incr[i], move);
    classifyLastDestPos(move);
  }
}

string Rook::getType() const {
  return "Rook";
}

Rook::~Rook() {}
/*end of Rook definitions*/

/*Pawn definitions*/
Pawn::Pawn() {}

Pawn::Pawn(string _colour, string _position, ChessBoard *_chboard) : Piece(_colour, _position, _chboard) {}

void Pawn::genPotValDestPos() {
  //cerr << "genPotValDestPos called" << endl;
  /*because a pawn can only go forward in rank, it is at starting rank iif it hasn't made a first move. I use this property to distinguish the case where it can move forward by 2 squares. Also need to make sure 'move forward by 1 square' is valid*/
  bool firstMove = ((rank=='2' && colour=="White") || (rank=='7' && colour=="Black"));
  string move;
  char r, f;  
  int incr[4][2] = {{1, 0},      //vertical
		    {1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {2, 0}};     //vertical, first move

  potValDestPos.clear();
  classifyDestPos(SHORT, FORWARDS, incr[0], move); //vertical

  /*if access to '1 square in front' is blocked, potValDestPos is empty*/
  if(firstMove && !potValDestPos.empty())
    classifyDestPos(SHORT, FORWARDS, incr[3], move);

  for (int i=1; i<3; i++) {      //diagonals
    r=rank, f=file;
    increment(FORWARDS, r, f, incr[i]);
    move = concat(r, f);
    classifyLastDestPos(move);
  }
}

string Pawn::getType() const {
  return "Pawn";
}

Pawn::~Pawn() {} 
/*end of Pawn definitions*/
