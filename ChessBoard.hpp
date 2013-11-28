#include<iostream>
#include<map>
#include<string>

using namespace std;

#define topRow 8
#define bottomRow 1

//DYNAMIC MEMORY! DESTRUCTORS!

typedef MapIt = map<string, Piece*>::iterator;

class Piece {  //no Position field, only chessboard need keep track of that
private:
  string owner;
  string *possibleMoves; //SENTINEL IS "'\0'"

public:
  Piece();
  Piece(_owner);
  void genPossibleMoves() = 0;
  virtual string getType() = 0;
};


class ChessBoard {
 private:
  map<string, Piece*> boardMap;
  string whoseTurn;

 public:
  ChessBoard();
  void   initiate();
  void   submitMove(const string sourceSquare, const string destSquare);
  bool   isValid();
  int    pieceOnSquare(string square);
  bool   putsOwnKingInCheck(string square);
  string Player();
  string notPlayer();
  void   nextPlayer();
  bool   canMoveTo(Piece piece, string destSquare);
  void   resetBoard();
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


