#include<iostream>
#include<map>
#include<string>
#include<vector>

using namespace std;

#define SINTINEL 10

enum WhosePiece {NOPIECE, FRIEND, FOE};

enum Direction  {FORWARDS, BACKWARDS};
enum Length     {SHORT, LONG};


//DYNAMIC MEMORY! DESTRUCTORS!

class Piece;

typedef map<string, Piece*>::iterator MapIt;
typedef vector<string>::iterator VecIt;

class ChessBoard {
 private:
  map<string, Piece*> boardMap;
  string whoseTurn;

 public:
  ChessBoard              ();
  void       initiate     ();
  void       submitMove   (const string sourceSquare, const string destSquare);
  bool       isValidSquare(const string square) const;
  WhosePiece pieceOnSquare(const string square);
  string     notPlayer    () const;
  void       nextPlayer   ();
  bool       putsOwnKingInCheck(const string square, const string destSquare);
  void       resetBoard   ();
};


class Piece {  //no Position field, only chessboard need keep track of that
protected:
  string owner;
  string square;
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
  void   printValidMoves    ();
  string getOwner           () const;
  bool   cpyPossibleMove    (int i, string &move) const; // --> why not letting me add this?
  virtual string getType() const = 0;
};

class King : public Piece {
private:
public:
  King();
  King(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

class Queen : public Piece {
private:
public:
  Queen();
  Queen(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

class Bishop : public Piece {
private:
public:
  Bishop();
  Bishop(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

class Knight : public Piece {
private:
public:
  Knight();
  Knight(string _owner, string _square, ChessBoard *_chboard);
  void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

class Rook : public Piece {
private:
public:
  Rook();
  Rook(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

class Pawn : public Piece {
private:
  bool firstMoveMade;

public:
  Pawn();
  Pawn(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
};

string concat(char ch1, char ch2);
