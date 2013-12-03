#include<iostream>
#include<map>
#include<string>
#include<vector>

using namespace std;

#define SINTINEL 10

enum WhosePiece {NOPIECE, FRIEND, FOE};

enum Direction  {FORWARDS, BACKWARDS};
enum Length     {SHORT, LONG};


//DOES THE MOVE PUT THE KING IN CHECK???


class Piece;

typedef map<string, Piece*>::iterator MapIt;
typedef vector<string>::iterator VecIt;

class ChessBoard {
 private:
  map<string, Piece*> boardMap;
  string whoseTurn;

 public:
  ChessBoard();
  void       initiate     ();
  void       submitMove   (const string sourceSquare, const string destSquare);
  bool       isValidSquare(const string square) const;
  WhosePiece pieceOnSquare(const string square);
  string     notPlayer    () const;
  void       nextPlayer   ();
  bool       putsOwnKingInCheck(const string square, const string destSquare);
  void       resetBoard   ();
  ~ChessBoard();
};


class Piece {  //no Position field, only chessboard need keep track of that
protected:
  string owner;
  string position;
  ChessBoard *chboard;
  char file;
  char rank;
  vector<string> validMoves; //SENTINEL IS "'\0'"

public:
  Piece                     ();
  Piece                     (string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves();
  void   classifyMoves      (Length length, Direction dir, int* inc, string& move);
  void   increment          (Direction dir, char &coordinate1, char &coordinate2, int *inc);
  void   classifyLastMove   (const string move);
  bool   isValidMove        (string square);
  void   setPosition        (string square);
  void   printValidMoves    ();
  string getOwner           () const;
  virtual string getType() const = 0;
};

class King : public Piece {
public:
  King();
  King(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

class Queen : public Piece {
public:
  Queen();
  Queen(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

class Bishop : public Piece {
public:
  Bishop();
  Bishop(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

class Knight : public Piece {
public:
  Knight();
  Knight(string _owner, string _square, ChessBoard *_chboard);
  void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

class Rook : public Piece {
public:
  Rook();
  Rook(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

class Pawn : public Piece {
public:
  Pawn();
  Pawn(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

string concat(char ch1, char ch2);
