#include<iostream>
#include<map>
#include<string>
using namespace std;

#include "chess.h"

#define INVALID_POSITION 1

#define topRow 8
#define bottomRow 1
enum Letter {A,B,C,D,E,F,G,H};
enum Player {white, black};

class Position { //could have called this Square
private:
  string label;
  int rank;
  Letter file;
 public:
  Position(string _label);
  int isValid();
};

    class Piece {  //no Position field, only chessboard need keep track of that
 private:
  Player whosePiece;
  Square *possibleMoves;
 public:
  virtual Piece() = 0;
  genPossibleMoves();
};

class ChessBoard {
 private:
  map<string, Piece*> boardMap;
  Player whoseTurn;
 public:
  ChessBoard();
  void initiate();
  static int errorDescription(int errorCode);
  void submitMove(const Square sorSquare, const Square destSquare);
  void resetBoard();
};

class King : public Piece {
};
class Queen : public Piece {
};
class Bishop : public Piece {
};
class Knight : public Piece {
};
class Rook : public Piece {
};
class Pawn : public Piece {
};


