#include<iostream>
#include<map>
#include<string>


using namespace std;

#include "ChessBoard.hpp"

enum Direction  {FORWARDS, BACKWARDS};
enum Length     {SHORT, LONG};


//NEW STRATEGY: USE INT[2][2] CONTAINING INCREMENTS

// void Piece::setCoordinates(Length length, int& r, int& f, int incr[2], string& move, ) {
//   do {
//     r -= inc[0];
//     f -= inc[1];
//     move = concat(r, f);
//   } while (length == LONG)



//     for(move=concat(r, f);
// 	chboard->isValid(move) && chboard->pieceOnSquare(move) == NOPIECE;
// 	r-=incr[i][0], f-=[i][1], move=concat(r,f));
// }


void Piece::classifyMoves(Length length, Direction dir, int& r, int& f, int[2] inc, string& move, int& count) {
  int r=rank, f=file;
  do {
    increment(dir, r, inc[0]);
    increment(dir, f, inc[1]);
    move = concat(r, f);
    if (chboard->isValid(move) && chboard->pieceOnSquare(move) == NOPIECE) { //last condition to make sure !pawn_attack
      validMoves[count] = move;
      count++;
    }
    else {
      //cout << "for " << getType() << " at (" << r << ", " << f << "), no valid position past " << move << " in " << dir << " direction" << endl;
      return;
    }
  } while (length == LONG);
}

void Piece::increment(Direction dir, int &coordinate, int inc) {
  if (dir = FORWARDS)
    coordinate += inc;
  else coordinate -= inc;
}

void Piece::classifyLastMove(string move, int &count) {
  if (chboard->isValid(move) && chboard->pieceOnSquare(move) == FOE {   //last position to make sure !pawn_move
    validMoves[count] = move;
    count++;
  }
}

// void Piece::findMoves(int[2][2] increment, Direction dir, Length length) {
//   int r, f, count;
//   string move;
//   for(count=0; possibleMoves[count] != "'\0'"; count++);

//   for (int i=0; i<2 && increment[i][0] != SINTINEL; i++) {
//     r=rank; f=file;
//     classifyMoves(r, f, increment[i][0], increment[i][1], move, count);
//     classifyLastMove();
//   }
//   va
//     lidMoves[count] = "'\0'";
// }
