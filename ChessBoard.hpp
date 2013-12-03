#include<iostream>
#include<map>
#include<string>

using namespace std;

#define SINTINEL 10

enum WhosePiece {NOPIECE, FRIEND, FOE};

enum Direction  {FORWARDS, BACKWARDS};
enum Length     {SHORT, LONG};


//DYNAMIC MEMORY! DESTRUCTORS!

class Piece;

typedef map<string, Piece*>::iterator It;


class ChessBoard {
 private:
  map<string, Piece*> boardMap;
  string whoseTurn;

 public:
  ChessBoard();
  void initiate();
  void submitMove(const string sourceSquare, const string destSquare);
  bool isValid(const string square) const;
  WhosePiece pieceOnSquare(const string square);
  string notPlayer() const;
  void nextPlayer();
  bool canMoveTo(const string sourceSquare, const string destSquare);  
  bool putsOwnKingInCheck(const string square, const string destSquare);
  void resetBoard();
};


class Piece {  //no Position field, only chessboard need keep track of that
private:
  string owner;
  string square;
  ChessBoard *chboard;
  char file;
  char rank;
  string *validMoves; //SENTINEL IS "'\0'"

public:
  Piece();
  Piece(string _owner, string _square, ChessBoard *_chboard);

  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  void findMoves();
  void findMoves(Axis, Distance);
  void findMoves(Axis, Direction, Aggressiveness, Distance);

  string getOwner() const;
  bool cpyPossibleMove(int i, string &move) const; // --> why not letting me add this?
  virtual string getType() = 0;
};

class King : public Piece {
private:
public:
  King();
  King(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType();
};

class Queen : public Piece {
private:
public:
  Queen();
  Queen(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType();
};

class Bishop : public Piece {
private:
public:
  Bishop();
  Bishop(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType();
};

class Knight : public Piece {
private:
public:
  Knight();
  Knight(string _owner, string _square, ChessBoard *_chboard);
  void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType();
};

class Rook : public Piece {
private:
public:
  Rook();
  Rook(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType();
};

class Pawn : public Piece {
private:
public:
  Pawn();
  Pawn(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType();
};

string concat(char ch1, char ch2);
