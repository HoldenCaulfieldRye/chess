#include<iostream>
#include<map>
#include<string>

using namespace std;

#define INVALID_POSITION 1

#define topRow 8
#define bottomRow 1

enum Player {white, black};

class Piece {  //no Position field, only chessboard need keep track of that
private:
  Player whosePiece;
  Position *possibleMoves;
public:
  Piece();
  virtual void genPossibleMoves() = 0;
};

class ChessBoard {
 private:
  map<string, Piece*> boardMap;
  Player whoseTurn;
 public:
  ChessBoard();
  void initiate();
  void submitMove(const string sourceSquare, const string destSquare);
  bool isValid();
  int pieceOnSquare();
  string notPlayer();
  void resetBoard();
};

class King : public Piece {
private:
public:
  void genPossibleMoves();
};

class Queen : public Piece {
private:
public:
  void genPossibleMoves();
};

class Bishop : public Piece {
private:
public:
  void genPossibleMoves();
};

class Knight : public Piece {
private:
public:
  void genPossibleMoves();
};

class Rook : public Piece {
private:
public:
  void genPossibleMoves();
};

class Pawn : public Piece {
private:
public:
  void genPossibleMoves();
};


