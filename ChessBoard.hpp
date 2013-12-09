#include<iostream>
#include<map>
#include<string>
#include<vector> 

using namespace std;

#define SINTINEL                     -10

#define NEW_GAME                       0
#define INVALID_SOURCE_SQUARE          1
#define INVALID_DESTINATION_SQUARE     2
#define EMPTY_SOURCE_SQUARE            3
#define WRONG_PLAYER                   4
#define FRIENDLY_FIRE                  5
#define IMPOSSIBLE_MOVE                6
#define CHECKING_OWN_KING              7
#define VALID_MOVE                     8
#define VALID_ATTACK                   9
#define CHECK                         10
#define CHECKMATE                     11
#define STALEMATE                     12
#define GAME_OVER                     13

enum WhosePiece {NOPIECE, FRIEND, FOE};

enum Direction  {FORWARDS, BACKWARDS};
enum Range      {SHORT, LONG};

class Piece;

typedef const string Cnstring;
typedef vector<string> Vecstr;
typedef vector<string>::iterator VecIt;
typedef map<string, Piece*>::iterator MapIt;

class ChessBoard {
 private:
  /*private fields*/
  bool gameOver;
  string whoseTurn;
  map<string, Piece*> boardMap;

  /*private methods*/
  void       initiate      ();
  bool       exists        (Cnstring square) const;
  WhosePiece colourOnSquare(Cnstring square, Cnstring player);
  Piece*     performMove   (Cnstring move[]);
  void       undoMove      (Cnstring move[], Piece *takenPiece);
  string     findKingPos   (Cnstring player);
  bool       entailsCheck  (Cnstring move[], Cnstring checkedPlayer, const bool speculative);
  bool       kingIsChecked (Cnstring kingPos);
  string     checkOutcome  ();
  string     notPlayer     () const;
  void       nextPlayer    ();
  void       message       (int mcode);
  void       message       (int mcode, string move[2]);
  void       message       (int mcode, string move[2], string takenPieceType);


 public:  //very few methods are public to ensure no cheating
  ChessBoard              ();
  void       submitMove   (Cnstring sourceSquare, Cnstring destSquare);
  void       resetBoard   ();
  static string whosep    (WhosePiece piece); //DELETE
  ~ChessBoard();
};


class Piece {
  friend class ChessBoard;

protected:
  /*protected fields*/
  string colour;
  string position;
  ChessBoard *chboard;
  char file;
  char rank;
  int howMove[8][2];
  Vecstr potValDestPos;
  /*'potentially valid destination positions', ie moving to such a position is valid if it doesn't put friendly king in check. sorry it's an awkward term, but qualifying it as 'valid' would be incorrect; qualifying it as 'moves' would also be incorrect because a move is a 2-uple of strings representing board squares, and this is not a vector of 2-uple strings*/

  /*protected methods - not public to ensure no 'cheating from main'*/
  Piece() {}
  Piece                        (string _colour, string _position, ChessBoard *_chboard);
  virtual void genPotValDestPos() = 0; //generate potentially valid destination positions
  void    classifyDestPos      (Range range, Direction dir, int* inc, string& move);
  void    classifyLastDestPos  (string move);
  void    setPosition          (Cnstring);
  void    printPotValDestPos   (); //DELETE!

  //public:
  bool    isPotValDestPos      (string square);
  bool    canMove              ();
  virtual string getType       () const = 0;
  string  getColour            () const;
  void    increment            (Direction dir, char &coordinate1, char &coordinate2, int *inc);
  static  string concat        (char ch1, char ch2);
  virtual ~Piece() {}
};

class King : public Piece {
private:
public:
  King() {}
  King(string _colour, string _square, ChessBoard *_chboard);
  virtual void genPotValDestPos();
  string getType() const;
  ~King() {} //empty destructor; no class fields stored on the heap
};

class Queen : public Piece {
private:
public:
  Queen() {}
  Queen(string _colour, string _square, ChessBoard *_chboard);
  virtual void genPotValDestPos();
  string getType() const;
  ~Queen() {} //empty destructor; no class fields stored on the heap
};

class Bishop : public Piece {
private:
public:
  Bishop() {}
  Bishop(string _colour, string _square, ChessBoard *_chboard);
  virtual void genPotValDestPos();
  string getType() const;
  ~Bishop() {} //empty destructor; no class fields stored on the heap
};

class Knight : public Piece {
private:
public:
  Knight() {}
  Knight(string _colour, string _square, ChessBoard *_chboard);
  void genPotValDestPos();
  string getType() const;
  ~Knight() {} //empty destructor; no class fields stored on the heap
};

class Rook : public Piece {
private:
public:
  Rook() {}
  Rook(string _colour, string _square, ChessBoard *_chboard);
  virtual void genPotValDestPos();
  string getType() const;
  ~Rook() {} //empty destructor; no class fields stored on the heap
};

class Pawn : public Piece {
public:
  Pawn() {}
  Pawn(string _colour, string _square, ChessBoard *_chboard);
  virtual void genPotValDestPos();
  string getType() const;
  ~Pawn() {} //empty destructor; no class fields stored on the heap
};
