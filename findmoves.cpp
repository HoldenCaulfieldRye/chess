#include<iostream>
#include<map>
#include<string>


using namespace std;

#include "ChessBoard.hpp"


enum Axis           {STRAIGHT, DIAGONAL};
enum Direction      {FRONT_OR_BACK, FRONT};
enum Length         {SHORT, LONG};


//NEW STRATEGY: USE INT[2][2] CONTAINING INCREMENTS


void Piece::findMoves(int[2][2] increment, Direction dir, Length length) {
  int r, f, count;
  string move;
  for(count=0; possibleMoves[count] != "'\0'"; count++);

  for (int i=0; i<2 && increment[i][0] != SINTINEL; i++) {
    r=rank; f=file;
    classifyMoves(r, f, increment[i][0], increment[i][1], move, count);
    classifyLastMove();
  }
  validMoves[count] = "'\0'";
}

/*YOU KNOW WHAT, USE THE "START AS FAR BACK AS POSSIBLE AND GO AS FAR FORWARD AS POSSIBLE" TECHNIQUE.
you can easily customise it with the initialisation and condition components of a for loop; it will be very clean.*/

void Piece::classifyMoves(int& r, int& f, string& move, int incr_r, int incr_f, int& count) {
  do {
  r += incr_r;
  f += incr_f;
  move = concat(r, f);
    if (chboard->isValid(move) && chboard->pieceOnSquare(move) == NOPIECE && (dir == FRONT_OR_BACK || axis == STRAIGHT)) { //last condition to make sure !pawn_attack
      validMoves[count] = move;
      count++;
    }
    else break;
  } while (length == LONG);
}

void Piece::classifyLastMove() {
  if (chboard->isValid(move) && chboard->pieceOnSquare(move) == FOE && (dir == FRONT_OR_BACK || axis == DIAGONAL)) {   //last position to make sure !pawn_move
    validMoves[count] = move;
    count++;
  }
}
