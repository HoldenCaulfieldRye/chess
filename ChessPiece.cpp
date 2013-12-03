#include<iostream>
#include<map>
#include<string>

using namespace std;

#include "ChessBoard.hpp"


/*Piece definitions*/
Piece::Piece() {}
Piece::Piece(string _owner, string _square, ChessBoard *_chboard) : owner(_owner), square(_square), chboard(_chboard), file(_square[0]), rank(_square[1]) {
  validMoves = new string;
  validMoves[0] = "'\0'";
  //  cout << "validMoves pointer newed" << endl;
}

void Piece::genValidMoves() { //even if empty, need to keep it, because need it to be virtual, because dunno which subclass until runtime
  cout << "piece at " << square << " not initiated to a proper piece!" << endl;
}

/*helper function for genValidMoves*/
void Piece::classifyMoves(Length length, Direction dir, int& r, int& f, int[2] inc, string& move, int& count) {
  int r=rank, f=file;
  do {
    increment(dir, r, inc[0]);
    increment(dir, f, inc[1]);
    move = concat(r, f);
    if (chboard->isValid(move) && chboard->pieceOnSquare(move) == NOPIECE && (dir == FRONT_OR_BACK || axis == STRAIGHT)) { //last condition to make sure !pawn_attack
      validMoves[count] = move;
      count++;
    }
    else {
      //cout << "for " << getType() << " at (" << r << ", " << f << "), no valid position past " << move << " in " << dir << " direction" << endl;
      return;
    }
  } while (length == LONG);
}

/*helper function for classifyMoves*/
void Piece::increment(Direction dir, int &coordinate, int inc) {
  if (dir = FORWARDS)
    coordinate += inc;
  else coordinate -= inc;
}

/*helper function for classifyMoves*/
void Piece::classifyLastMove(const string move, int &count) {
  if (chboard->isValid(move) && chboard->pieceOnSquare(move) == FOE) {   //last position to make sure !pawn_move
    validMoves[count] = move;
    count++;
  }
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
/*end of Piece definitions*/


/*King definitions*/
King::King() {}
King::King(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void King::genValidMoves() {
  string move;
  int count=0, incr[5][2] = {{1, 0},      //vertical
			     {0, 1},      //horizontal
			     {1, 1},      //diagonal1
			     {1,-1},      //diagonal2
			     {SINTINEL}};

  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(SHORT, FORWARDS, r, f, incr[i], move, count);
    classifyLastMove(move, count);
    classifyMoves(SHORT, BACKWARDS, r, f, incr[i], move, count);
    classifyLastMove(move, count);
  }
  validMoves[count] = "'\0'";
}

string King::getType() {
  return "King";
}
/*end of King definitions*/

/*Queen definitions*/
Queen::Queen() {}
Queen::Queen(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Queen::genValidMoves() {
  string move;
  int count=0, incr[5][2] = {{1, 0},      //vertical
			     {0, 1},      //horizontal
			     {1, 1},      //diagonal1
			     {1,-1},      //diagonal2
			     {SINTINEL}};

  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(LONG, FORWARDS, r, f, incr[i], move, count);
    classifyLastMove(move, count);
    classifyMoves(LONG, BACKWARDS, r, f, incr[i], move, count);
    classifyLastMove(move, count);
  }
  validMoves[count] = "'\0'";
}

string Queen::getType() {
  return "Queen";
}
/*end of Queen definitions*/

/* Bishop definitions*/
Bishop::Bishop() {}
Bishop::Bishop(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Bishop::genValidMoves() {
  string move;
  int count=0, incr[3][2] = {{1, 1},      //diagonal1
		    {1,-1},      //diagonal2
		    {SINTINEL}};

  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(LONG, FORWARDS, r, f, incr[i], move, count);
    classifyLastMove(move, count);
    classifyMoves(LONG, BACKWARDS, r, f, incr[i], move, count);
    classifyLastMove(move, count);
  }
  validMoves[count] = "'\0'";
}

string Bishop::getType() {
  return "Bishop";
}
/*end of Bishop definitions*/

/* Knight definitions*/
Knight::Knight() {}
Knight::Knight(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Knight::genValidMoves() {
  int i, r[8], f[8];
  string move;

  for(int j=-3; j<4; j+=2) {
    if (j<0)
      r[j+3] = f[j+4] = -2;
    else r[j+3] = f[j+4] = 2;

    f[j+3] = r[j+4] = j - r[j+3];

    for(int k=3; k<5; k++) {
      move = concat(r[j+k], f[j+k]);
      if (chboard->pieceOnSquare(move) != FRIEND && chboard->isValid(move)) {
	validMoves[i] = move;
	i++;
      }
    } 
  }
}

string Knight::getType() {
  return "Knight";
}
/*end of Knight definitions*/

/*Rook definitions*/
Rook::Rook() {}
Rook::Rook(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Rook::genValidMoves() {
  string move;
  int count=0, incr[3][2] = {{1, 0},      //vertical
			     {0, 1},      //horizontal
			     {SINTINEL}};

  for(int i=0; incr[i][0] != SINTINEL; i++) {
    classifyMoves(LONG, FORWARDS, r, f, incr[i], move, count);
    classifyLastMove(move, count);
    classifyMoves(LONG, BACKWARDS, r, f, incr[i], move, count);
    classifyLastMove(move, count);
  }
  validMoves[count] = "'\0'";
}

string Rook::getType() {
  return "Rook";
}
/*end of Rook definitions*/

/*Pawn definitions*/
Pawn::Pawn() {}

Pawn::Pawn(string _owner, string _square, ChessBoard *_chboard) : Piece(_owner, _square, _chboard) {
}

void Pawn::genValidMoves() {
  string move;
  int count=0, incr[4][2] = {{1, 0},      //vertical
			     {1, 1},      //horizontal
			     {1,-1},
			     {SINTINEL}};

  classifyMoves(SHORT, FORWARDS, r, f, incr[0], move, count);  //vertical move

  for(int i=1; incr[i][0] != SINTINEL; i++) {                  //diagonal attacks
    increment(dir, r, inc[0]);
    increment(dir, f, inc[1]);
    move = concat(r, f);
    classifyLastMove(move, count);
  }
  validMoves[count] = "'\0'";
}

string Pawn::getType() {
  return "Pawn";
}
/*end of Pawn definitions*/



// void Piece::findMoves() {
//   int i, r[8], f[8];
//   string move;
//   for(i=0; validMoves[i] != "'\0'"; i++);

//   cerr << "genValidMoves about to run; currently " << i << " possible moves known" << endl;

//   for(int j=-3; j<4; j+=2) {

//     if (j<0)
//       r[j+3] = f[j+4] = -2;
//     else r[j+3] = f[j+4] = 2;

//     f[j+3] = r[j+4] = j - r[j+3];

//     for(int k=3; k<5; k++) {
//       move = concat(r[j+k], f[j+k]);
//       if (chboard->pieceOnSquare(move) != FRIEND && chboard->isValid(move)) {
// 	validMoves[i] = move;
// 	i++;
//       } } }
// }


// void Piece::findMoves(Axis axis, Distance dist) {
//   int i, r, f;
//   string move;
//   for(i=0; validMoves[i] != "'\0'"; i++);

//   cerr << "genValidMoves about to run; currently " << i << " possible moves known" << endl;

//   if (axis == HORIZONTAL && dist == LONG) {
//     /*right*/
//     r = rank;
//     f = file + 1;
//     move = concat(r, f);

//     while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) {
//       validMoves[i] = move;
//       //r++;
//       f++; 
//       i++;
//       move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     /*left*/
//     r = rank;
//     f = file - 1;
//     move = concat(r, f);

//     while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       //r--;
//       f--; 
//       i++;
//       move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }
//   }

//   if (axis == HORIZONTAL && dist == SHORT) {
//     /*right*/
//     r = rank;
//     f = file + 1;
//     move = concat(r, f);

//     if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       //r++;
//       //f++; 
//       i++;
//       //move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     /*left*/
//     r = rank;
//     f = file - 1;
//     move = concat(r, f);

//     if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       //r--;
//       //f--; 
//       i++;
//       //move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }
//   }
// }


// void Piece::findMoves(Axis axis, Direction dir, Aggressiveness aggr, Distance dist) {

//   int i, r, f;
//   string move;
//   for(i=0; validMoves[i] != "'\0'"; i++);

//   cerr << "genValidMoves about to run; currently" << i << "possible moves known" << endl;

//   if (axis == VERTICAL && dir == FRONT_OR_BACK && aggr == ATTACK_OR_MOVE && dist == LONG) {
//     /*front*/

//     //step 1
//      r = rank + 1; //diff
//      f = file;
//     move = concat(r, f);

//     //step 2
//     while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       r++;
//       i++;
//       move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     /*back*/
//     r = rank - 1; //diff
//     f = file;
//     move = concat(r, f);

//     while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       r--; //r++;       
//       i++;
//       move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }
//   }


//   if (axis == VERTICAL && dir == FRONT_OR_BACK && aggr == ATTACK_OR_MOVE && dist == SHORT) {
//     /*front*/
//     r = rank + 1;
//      f = file;
//     move = concat(r, f);

//     if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       //r++;
//       i++;
//       //move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     /*back*/
//     r = rank - 1;
//     f = file;
//     move = concat(r, f);

//     if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       //r--;
//       i++;
//       //move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }
//   }


//   if (axis == DIAGONAL && dir == FRONT_OR_BACK && aggr == ATTACK_OR_MOVE && dist == LONG) {
//     /*front right*/
//     r = rank + 1; //diff
//     f = file + 1; //diff
//     move = concat(r, f);

//     while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       r++;
//       f++; //diff
//       i++;
//       move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     /*back left*/
//     r = rank - 1; //diff
//     f = file - 1; //diff
//     move = concat(r, f);

//     while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       r--;      //r++;
//       f--;      //diff
//       i++;
//       move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     /*front left*/
//     r = rank + 1; //diff
//     f = file - 1; //diff
//     move = concat(r, f);

//     while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       r++;
//       f--; //diff
//       i++;
//       move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     /*back right*/
//     r = rank - 1; //diff
//     f = file + 1; //diff
//     move = concat(r, f);

//     while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       r--;      //r++;
//       f++;      //diff
//       i++;
//       move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }
//   }


//   if (axis == DIAGONAL && dir == FRONT_OR_BACK && aggr == ATTACK_OR_MOVE && dist == SHORT) {
//     /*front right*/
//     r = rank + 1;   //diff
//     f = file + 1;   //diff
//     move = concat(r, f);

//     if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       //r++;
//       //f++; 
//       i++;
//       //move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     /*back left*/
//     r = rank - 1;   //diff
//     f = file - 1;   //diff
//     move = concat(r, f);

//     if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       //r--;
//       //f--; 
//       i++;
//       //move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     /*front left*/
//     r = rank + 1;   //diff
//     f = file - 1;   //diff
//     move = concat(r, f);

//     if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       //r++;
//       //f++; 
//       i++;
//       //move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     /*back right*/
//     r = rank - 1;   //diff
//     f = file + 1;   //diff
//     move = concat(r, f);

//     if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       //r--;
//       //f--; 
//       i++;
//       //move = concat(r, f);
//     }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }
//   }



//   if (axis == VERTICAL && dir == FRONT && aggr == MOVE && dist == SHORT) {
//     /*front*/
//     int r = rank + 1;
//     char f = file;
//     move = concat(r, f);

//     if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
//       validMoves[i] = move;
//       //r++;
//       i++;
//       //move = concat(r, f);
//     }
//     //if(chboard->pieceOnSquare(move) == FOE) {
//     //  validMoves[i] = move;
//     //  i++;
//     //}

//     /*back*/
//     // r = rank - 1;
//     // f = file;
//     // move = concat(r, f);

//     // if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && move[j][0] < 'I' && move[j][0] >= 'A' && move[j][1] < '9' && move[j][1] > '0') {
//     //   validMoves[i] = move;
//     //   //r--;
//     //   i++;
//     //   //move = concat(r, f);
//     // }
//     // if(chboard->pieceOnSquare(move) == FOE) {
//     //   validMoves[i] = move;
//     //   i++;
//     // }
//   }


//   if (axis == DIAGONAL && dir == FRONT && aggr == ATTACK && dist == SHORT) { 
//     /*front right*/
//     r = rank + 1;   //diff
//     f = file + 1;   //diff
//     move = concat(r, f);

//     // if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && move[j][0] < 'I' && move[j][0] >= 'A' && move[j][1] < '9' && move[j][1] > '0') {
//     //   validMoves[i] = move;
//     //   //r++;
//     //   //f++; 
//     //   i++;
//     //   //move = concat(r, f);
//     // }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     // /*back left*/
//     // r = rank - 1;   //diff
//     // f = file - 1;   //diff
//     // move = concat(r, f);

//     // if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && move[j][0] < 'I' && move[j][0] >= 'A' && move[j][1] < '9' && move[j][1] > '0') {
//     //   validMoves[i] = move;
//     //   //r--;
//     //   //f--; 
//     //   i++;
//     //   //move = concat(r, f);
//     // }
//     // if(chboard->pieceOnSquare(move) == FOE) {
//     //   validMoves[i] = move;
//     //   i++;
//     // }

//     /*front left*/
//     r = rank + 1;   //diff
//     f = file - 1;   //diff
//     move = concat(r, f);

//     // if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && move[j][0] < 'I' && move[j][0] >= 'A' && move[j][1] < '9' && move[j][1] > '0') {
//     //   validMoves[i] = move;
//     //   //r++;
//     //   //f--; 
//     //   i++;
//     //   //move = concat(r, f);
//     // }
//     if(chboard->pieceOnSquare(move) == FOE) {
//       validMoves[i] = move;
//       i++;
//     }

//     // /*back right*/
//     // r = rank - 1;   //diff
//     // f = file + 1;   //diff
//     // move = concat(r, f);

//     // if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && move[j][0] < 'I' && move[j][0] >= 'A' && move[j][1] < '9' && move[j][1] > '0') {
//     //   validMoves[i] = move;
//     //   //r--;
//     //   //f++; 
//     //   i++;
//     //   //move = concat(r, f);
//     // }
//     // if(chboard->pieceOnSquare(move) == FOE) {
//     //   validMoves[i] = move;
//     //   i++;
//     // }
//   }
// }

