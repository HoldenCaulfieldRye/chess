#include<iostream>
#include<map>
#include<string>
#include<vector> 

using namespace std;

#define SINTINEL                     -10

#define INVALID_SOURCE_SQUARE          1
#define INVALID_DESTINATION_SQUARE     2
#define EMPTY_SOURCE_SQUARE            3
#define WRONG_PLAYER                   5
#define FRIENDLY_FIRE                  6
#define IMPOSSIBLE_MOVE                7
#define CHECKING_OWN_KING              8
#define VALID_MOVE                     9
#define VALID_ATTACK                  10
#define CHECK                         11
#define CHECKMATE                     12
#define STALEMATE                     13

enum WhosePiece {NOPIECE, FRIEND, FOE};

enum Direction  {FORWARDS, BACKWARDS};
enum Range      {SHORT, LONG};


//DYNAMIC MEMORY! DESTRUCTORS!

class Piece;

typedef const string Cnstring;
typedef vector<string> Vecstr;
typedef vector<string>::iterator VecIt;
typedef map<string, Piece*>::iterator MapIt;

class ChessBoard {
 private:
  string whoseTurn;
  map<string, Piece*> boardMap;

 public:
  ChessBoard();
  void       initiate     ();
  void       submitMove   (Cnstring sourceSquare, Cnstring destSquare);
  bool       isValidSquare(Cnstring square) const;
  WhosePiece colourOnSquare(Cnstring square, Cnstring player);
  Piece*     performMove  (Cnstring move[]);
  void       undoMove     (Cnstring move[], Piece *takenPiece);
  bool       entailsCheck (Cnstring move[], Cnstring checkedPlayer, const bool speculative);
  bool       kingInCheck  (Cnstring kingPos);
  string     checkOutcome ();
  string     findKingPos  (Cnstring player);
  string     notPlayer    () const;
  void       nextPlayer   ();
  void       message      (int mcode, string move[2]);
  void       message      (int mcode, string move[2], string takenPieceType);
  void       resetBoard   ();
  //~ChessBoard();
};


class Piece {
protected:
  string colour;
  string position;
  ChessBoard *chboard;
  char file;
  char rank;
  Vecstr potValDestPos; //'potentially valid destination positions' ie moving to such a position is valid if it doesn't put friendly king in check

public:
  Piece                        ();
  Piece                        (string _colour, string _position, ChessBoard *_chboard);
  virtual void genPotValDestPos() = 0; //generate potentially valid destination positions
  void    classifyDestPos      (Range range, Direction dir, int* inc, string& move);
  void    classifyLastDestPos  (string move);
  void    increment            (Direction dir, char &coordinate1, char &coordinate2, int *inc);
  bool    isPotValDestPos      (string square);
  bool    canMove              ();
  void    setPosition          (Cnstring);
  string  getColour            () const;
  void    printPotValDestPos   (); //delete
  virtual string getType       () const = 0;
  virtual ~Piece               ();
};

class King : public Piece {
private:
public:
  King();
  King(string _colour, string _square, ChessBoard *_chboard);
  virtual void genPotValDestPos(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~King();
};

class Queen : public Piece {
private:
public:
  Queen();
  Queen(string _colour, string _square, ChessBoard *_chboard);
  virtual void genPotValDestPos(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~Queen();
};

class Bishop : public Piece {
private:
public:
  Bishop();
  Bishop(string _colour, string _square, ChessBoard *_chboard);
  virtual void genPotValDestPos(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~Bishop();
};

class Knight : public Piece {
private:
public:
  Knight();
  Knight(string _colour, string _square, ChessBoard *_chboard);
  void genPotValDestPos(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~Knight();
};

class Rook : public Piece {
private:
public:
  Rook();
  Rook(string _colour, string _square, ChessBoard *_chboard);
  virtual void genPotValDestPos(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~Rook();
};

class Pawn : public Piece {
public:
  Pawn();
  Pawn(string _colour, string _square, ChessBoard *_chboard);
  virtual void genPotValDestPos(); //EMBED A VIRTUAL FUNCTION
  string getType() const;
  ~Pawn();
};

string concat(char ch1, char ch2);

string whosep(WhosePiece);
