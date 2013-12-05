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
  ChessBoard();
  void       initiate     ();
  void       submitMove   (const string sourceSquare, const string destSquare);
  bool       isValidSquare(const string square) const;
  WhosePiece pieceOnSquare(const string square);
  string     notPlayer    () const;
  void       nextPlayer   ();
  bool       kingInCheck  (const string player, string& kingpos);
  void       resetBoard   ();
  //~ChessBoard();
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
  Piece();
  Piece                     (string _owner, string _position, ChessBoard *_chboard);
  virtual void genValidMoves();
  void    classifyMoves     (Length length, Direction dir, int* inc, string& move);
  void    increment         (Direction dir, char &coordinate1, char &coordinate2, int *inc);
  void    classifyLastMove  (string move);
  bool    isValidMove       (string square);
  void    printValidMoves   ();
  string  getOwner          () const;
  void    setPosition       (const string);
  virtual string getType    () const = 0;
  virtual ~Piece();
};

class King : public Piece {
private:
public:
  King();
  King(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~King();
};

class Queen : public Piece {
private:
public:
  Queen();
  Queen(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~Queen();
};

class Bishop : public Piece {
private:
public:
  Bishop();
  Bishop(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~Bishop();
};

class Knight : public Piece {
private:
public:
  Knight();
  Knight(string _owner, string _square, ChessBoard *_chboard);
  void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~Knight();
};

class Rook : public Piece {
private:
public:
  Rook();
  Rook(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~Rook();
};

class Pawn : public Piece {
public:
  Pawn();
  Pawn(string _owner, string _square, ChessBoard *_chboard);
  virtual void genValidMoves(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~Pawn();
};

string concat(char ch1, char ch2);

string whosep(WhosePiece);
