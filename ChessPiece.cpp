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
  //  cout << "starting to generate possible moves" << endl;
  validMoves = new string [5];
  validMoves[0] = "D6";
  validMoves[1] = "H6";
  validMoves[2] = "D4";
  validMoves[3] = "B4";
  validMoves[4] = "'\0'";
}

void Piece::findMoves() {
  int i, r[8], f[8];
  string move;
  for(i=0; validMoves[i] != "'\0'"; i++);

  cerr << "genValidMoves about to run; currently " << i << " possible moves known" << endl;

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
      } } }
}


void Piece::findMoves(Axis axis, Distance dist) {
  int i, r, f;
  string move;
  for(i=0; validMoves[i] != "'\0'"; i++);

  cerr << "genValidMoves about to run; currently " << i << " possible moves known" << endl;

  if (axis == HORIZONTAL && dist == LONG) {
    /*right*/
    r = rank;
    f = file + 1;
    move = concat(r, f);

    while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) {
      validMoves[i] = move;
      //r++;
      f++; 
      i++;
      move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    /*left*/
    r = rank;
    f = file - 1;
    move = concat(r, f);

    while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      //r--;
      f--; 
      i++;
      move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }
  }

  if (axis == HORIZONTAL && dist == SHORT) {
    /*right*/
    r = rank;
    f = file + 1;
    move = concat(r, f);

    if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      //r++;
      //f++; 
      i++;
      //move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    /*left*/
    r = rank;
    f = file - 1;
    move = concat(r, f);

    if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      //r--;
      //f--; 
      i++;
      //move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }
  }
}


void Piece::findMoves(Axis axis, Direction dir, Aggressiveness aggr, Distance dist) {

  int i, r, f;
  string move;
  for(i=0; validMoves[i] != "'\0'"; i++);

  cerr << "genValidMoves about to run; currently" << i << "possible moves known" << endl;

  if (axis == VERTICAL && dir == FRONT_OR_BACK && aggr == ATTACK_OR_MOVE && dist == LONG) {
    /*front*/

    //step 1
     r = rank + 1; //diff
     f = file;
    move = concat(r, f);

    //step 2
    while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      r++;
      i++;
      move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    /*back*/
    r = rank - 1; //diff
    f = file;
    move = concat(r, f);

    while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      r--; //r++;       
      i++;
      move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }
  }


  if (axis == VERTICAL && dir == FRONT_OR_BACK && aggr == ATTACK_OR_MOVE && dist == SHORT) {
    /*front*/
    r = rank + 1;
     f = file;
    move = concat(r, f);

    if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      //r++;
      i++;
      //move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    /*back*/
    r = rank - 1;
    f = file;
    move = concat(r, f);

    if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      //r--;
      i++;
      //move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }
  }


  if (axis == DIAGONAL && dir == FRONT_OR_BACK && aggr == ATTACK_OR_MOVE && dist == LONG) {
    /*front right*/
    r = rank + 1; //diff
    f = file + 1; //diff
    move = concat(r, f);

    while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      r++;
      f++; //diff
      i++;
      move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    /*back left*/
    r = rank - 1; //diff
    f = file - 1; //diff
    move = concat(r, f);

    while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      r--;      //r++;
      f--;      //diff
      i++;
      move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    /*front left*/
    r = rank + 1; //diff
    f = file - 1; //diff
    move = concat(r, f);

    while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      r++;
      f--; //diff
      i++;
      move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    /*back right*/
    r = rank - 1; //diff
    f = file + 1; //diff
    move = concat(r, f);

    while (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      r--;      //r++;
      f++;      //diff
      i++;
      move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }
  }


  if (axis == DIAGONAL && dir == FRONT_OR_BACK && aggr == ATTACK_OR_MOVE && dist == SHORT) {
    /*front right*/
    r = rank + 1;   //diff
    f = file + 1;   //diff
    move = concat(r, f);

    if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      //r++;
      //f++; 
      i++;
      //move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    /*back left*/
    r = rank - 1;   //diff
    f = file - 1;   //diff
    move = concat(r, f);

    if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      //r--;
      //f--; 
      i++;
      //move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    /*front left*/
    r = rank + 1;   //diff
    f = file - 1;   //diff
    move = concat(r, f);

    if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      //r++;
      //f++; 
      i++;
      //move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    /*back right*/
    r = rank - 1;   //diff
    f = file + 1;   //diff
    move = concat(r, f);

    if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      //r--;
      //f--; 
      i++;
      //move = concat(r, f);
    }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }
  }



  if (axis == VERTICAL && dir == FRONT && aggr == MOVE && dist == SHORT) {
    /*front*/
    int r = rank + 1;
    char f = file;
    move = concat(r, f);

    if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && chboard->isValid(move)) { 
      validMoves[i] = move;
      //r++;
      i++;
      //move = concat(r, f);
    }
    //if(chboard->pieceOnSquare(move) == FOE) {
    //  validMoves[i] = move;
    //  i++;
    //}

    /*back*/
    // r = rank - 1;
    // f = file;
    // move = concat(r, f);

    // if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && move[j][0] < 'I' && move[j][0] >= 'A' && move[j][1] < '9' && move[j][1] > '0') {
    //   validMoves[i] = move;
    //   //r--;
    //   i++;
    //   //move = concat(r, f);
    // }
    // if(chboard->pieceOnSquare(move) == FOE) {
    //   validMoves[i] = move;
    //   i++;
    // }
  }


  if (axis == DIAGONAL && dir == FRONT && aggr == ATTACK && dist == SHORT) { 
    /*front right*/
    r = rank + 1;   //diff
    f = file + 1;   //diff
    move = concat(r, f);

    // if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && move[j][0] < 'I' && move[j][0] >= 'A' && move[j][1] < '9' && move[j][1] > '0') {
    //   validMoves[i] = move;
    //   //r++;
    //   //f++; 
    //   i++;
    //   //move = concat(r, f);
    // }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    // /*back left*/
    // r = rank - 1;   //diff
    // f = file - 1;   //diff
    // move = concat(r, f);

    // if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && move[j][0] < 'I' && move[j][0] >= 'A' && move[j][1] < '9' && move[j][1] > '0') {
    //   validMoves[i] = move;
    //   //r--;
    //   //f--; 
    //   i++;
    //   //move = concat(r, f);
    // }
    // if(chboard->pieceOnSquare(move) == FOE) {
    //   validMoves[i] = move;
    //   i++;
    // }

    /*front left*/
    r = rank + 1;   //diff
    f = file - 1;   //diff
    move = concat(r, f);

    // if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && move[j][0] < 'I' && move[j][0] >= 'A' && move[j][1] < '9' && move[j][1] > '0') {
    //   validMoves[i] = move;
    //   //r++;
    //   //f--; 
    //   i++;
    //   //move = concat(r, f);
    // }
    if(chboard->pieceOnSquare(move) == FOE) {
      validMoves[i] = move;
      i++;
    }

    // /*back right*/
    // r = rank - 1;   //diff
    // f = file + 1;   //diff
    // move = concat(r, f);

    // if /*while*/ (chboard->pieceOnSquare(move) == NOPIECE && move[j][0] < 'I' && move[j][0] >= 'A' && move[j][1] < '9' && move[j][1] > '0') {
    //   validMoves[i] = move;
    //   //r--;
    //   //f++; 
    //   i++;
    //   //move = concat(r, f);
    // }
    // if(chboard->pieceOnSquare(move) == FOE) {
    //   validMoves[i] = move;
    //   i++;
    // }
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
  int vertical[2][2]   = {{1, 0}, {-1, 0}};
  int horizontal[2][2] = {{0, 1}, { 0,-1}};
  int diag1[2][2]   = {{1, 1}, {-1,-1}};
  int diag2[2][2]   = {{1,-1}, {-1,1}};

  findMoves(vertical, ATTACK_OR_MOVE, SHORT);
  findMoves(horizontal, ATTACK_OR_MOVE, SHORT);
  findMoves(diagonal, ATTACK_OR_MOVE, SHORT);
}

string King::getType() {
  return "King";
}
/*end of King definitions*/

/*Queen definitions*/
Queen::Queen() {}
Queen::Queen(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Queen::genValidMoves() {
  int vertical[2][2]   = {{1, 0}, {-1, 0}};
  int horizontal[2][2] = {{0, 1}, { 0,-1}};
  int diag1[2][2]   = {{1, 1}, {-1,-1}};
  int diag2[2][2]   = {{1,-1}, {-1,1}};

  findMoves(vertical, ATTACK_OR_MOVE, LONG);
  findMoves(horizontal, ATTACK_OR_MOVE,LONG);
  findMoves(diagonal, ATTACK_OR_MOVE,LONG);
}

string Queen::getType() {
  return "Queen";
}
/*end of Queen definitions*/

/* Bishop definitions*/
Bishop::Bishop() {}
Bishop::Bishop(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Bishop::genValidMoves() {
  int diag1[2][2]   = {{1, 1}, {-1,-1}};
  int diag2[2][2]   = {{1,-1}, {-1,1}};
  findMoves(diagonal, ATTACK_OR_MOVE,LONG);
}

string Bishop::getType() {
  return "Bishop";
}
/*end of Bishop definitions*/

/* Knight definitions*/
Knight::Knight() {}
Knight::Knight(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Knight::genValidMoves() {
  //ENTER CODE HERE, DONT USE FINDMOVES, KNIGHT HAS NOTHING TO SHARE;
  findMoves();
}

string Knight::getType() {
  return "Knight";
}
/*end of Knight definitions*/

/*Rook definitions*/
Rook::Rook() {}
Rook::Rook(string _owner, string _square, ChessBoard *_chboard) : Piece::Piece(_owner, _square, _chboard) {}

void Rook::genValidMoves() {
  int vertical[2][2]   = {{1, 0}, {-1, 0}};
  int horizontal[2][2] = {{0, 1}, { 0,-1}};

  findMoves(vertical, ATTACK_OR_MOVE, LONG);
  findMoves(horizontal, ATTACK_OR_MOVE, LONG);
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
  int vertical[2][2]= {{1, 0}, {SINTINEL}};
  int diag1[2][2]   = {{1, 1}, {SINTINEL}};
  int diag2[2][2]   = {{1,-1}, {SINTINEL}};

  findMoves(vertical, MOVE, SHORT);
  findMoves(diagonal, ATTACK, SHORT);
}

string Pawn::getType() {
  return "Pawn";
}
/*end of Pawn definitions*/

