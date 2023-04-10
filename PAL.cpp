# include <iostream>
# include <stdio.h>
# include <cstring>
# include <string>
# include <vector>
using namespace std;


enum TokenType { OPE_ADD, OPE_SUB, OPE_MUL, OPE_DIV, OPE_COMPARE, OPE_STATEMENT, SEMICOLON,     //  0 ~  6
                 INT, FLOAT, L_PAREN, R_PAREN, L_BIG_PAREN, R_BIG_PAREN, SYMBOL_UNKNOW,         //  7 ~ 13
                 SYMBOL_INT, SYMBOL_FLOAT, SYMBOL_CHAR, SYMBOL_STRING, SYMBOL_BOOL,             // 14 ~ 18
                 QUIT, ARITHEXP, BOOLEANEXP, STAMENT, DONE, ERROR, NULLL, BOOLEXP_ANS,          // 19 ~ 27
                 UNEXPECTED_TOKEN, UNRECOGNIZED_TOKEN, UNDEFINED_SYMBOL,                        // 28 ~ 30
                 TYPE_INT, TYPE_CHAR, TYPE_STRING, TYPE_FLOAT, TYPE_BOOL, L_MID_PAREN, R_MID_PAREN, 
                 TRUE, FALSE, COUT, CIN, L_ARROW, R_ARROW, DOUBLE_COMMA, COMMA, DEFINED, REDEFINED
};
typedef int Type;


class Unit { // Αい token
  public : string mUnit;  // mUnit 琌 (1) 计 (2) 珹腹 (3) 笲衡才 (4) 跑计
  public : Type mType;
  public : int mIntValue;
  public : float mFloatValue;

  public : Unit() {
    mUnit  = "", mIntValue = 0, mFloatValue = 0, mType = NULLL;
  } // Unit() -> 篶 1

  public : Unit( string s, Type type ) {
    mUnit = s, mIntValue = 0, mFloatValue = 0, mType = type;
  } // Unit() -> 篶 2

  public : Unit( int i, Type type ) {
    mUnit  = "", mIntValue = i, mFloatValue = i, mType = type;
  } // Unit() -> 篶 2

  public : Unit( float f, Type type ) {
    mUnit  = "", mIntValue = 0, mFloatValue = f, mType = type;
  } // Unit() -> 篶 3

  public : void StringToInt() {
    for ( int a = 0 ; a < mUnit.length() ; a++ ) {
      if ( IsDigit( mUnit[a] ) ) {
        mIntValue = mIntValue * 10 + ( mUnit[a] - '0' );
      } // if()
    } // for(a)

    mFloatValue = mIntValue;
  } // StringToInt()

  public : void StringToFloat() {
    int division = 1; // 矪瞶计
    bool findDot = false;

    for ( int a = 0 ; a < mUnit.length() ; a++ ) {
      if ( IsDigit( mUnit[a] ) ) {
        mFloatValue = mFloatValue * 10 + ( mUnit[a] - '0' );
        if ( findDot ) division *= 10;
      } // if()
      else if ( mUnit[a] == '.' ) findDot = true;
    } // for(a)

    mFloatValue /= division;
  } // StringToFloat()

  private : bool IsDigit( char ch ) {
    if ( '0' <= ch && ch <= '9' ) return true;
    else return false;
  } // IsDigit()

}; // class Unit


class Formula {
  private : vector<Unit*> mSymbolTable;
  public  : int mErrorLine; 
  // private : Unit *token1, *token2;


  public : Formula() {
    mSymbolTable.clear();
    // token1->mType = token2->mType = NULLL;
  } // Formula() -> constructor


  private : Unit* IsDefinedSymbol( Unit* u ) {
    // cout << "To find " << u->mUnit << "\n";

    for ( int a = 0 ; a < mSymbolTable.size() ; a++ ) 
      if ( mSymbolTable[a]->mUnit == u->mUnit ) return mSymbolTable[a];

    // cout << "Not find this symbol\n";
    return NULL;
  } // IsDefinedSymbol()


  private : void PrintSymbolTable() {
    for ( int a = 0 ; a < mSymbolTable.size() ; a++ ) {
      cout << "In PrintSymbolTable, mSymbolTable[" << a << "]->mUnit = " << mSymbolTable[a]->mUnit << "\n";
      cout << "In PrintSymbolTable, mSymbolTable[" << a << "]->mType = " << mSymbolTable[a]->mType << "\n";
      cout << "In PrintSymbolTable, mSymbolTable[" << a << "]->mIntValue = " 
           << mSymbolTable[a]->mIntValue << "\n";
      cout << "In PrintSymbolTable, mSymbolTable[" << a << "]->mFloatValue = " 
           << mSymbolTable[a]->mFloatValue << "\n\n";    
    } // for(a)
  } // PrintSymbolTable()


  private : Unit* GetToken( bool IsFactorOrNot ) {
    // num, symbol, +, -, *, / , (, ), <, =, >, :=, ;
    SkipWhiteSpace();
    char ch = cin.peek();
    Unit* re; // use to return

    if ( IsAlpha( ch ) ) { // this token is a symbol
      string symbol = ""; // ﹃钡ノ

      while ( IsAlpha( ch ) || IsDigit( ch ) ) {
        cin.get();
        symbol += ch;
        ch = cin.peek();
      } // while()

      if ( symbol == "quit" ) re = new Unit( "quit", QUIT );
      else if ( symbol == "int" )    re = new Unit( "int",    TYPE_INT );
      else if ( symbol == "string" ) re = new Unit( "string", TYPE_STRING );
      else if ( symbol == "char"   ) re = new Unit( "char",   TYPE_CHAR );
      else if ( symbol == "float" )  re = new Unit( "float",  TYPE_FLOAT );
      else if ( symbol == "bool" )   re = new Unit( "bool",   TYPE_BOOL );
      else if ( symbol == "true" )   re = new Unit( "true",   TRUE );
      else if ( symbol == "false" )  re = new Unit( "false",  FALSE );
      else if ( symbol == "cout" ) re = new Unit( "cout", COUT );
      else if ( symbol == "cin"  ) re = new Unit( "cin", CIN );
      else if ( symbol == "Done" ) re = new Unit( "Done", DONE );
      else {
        re = new Unit( symbol, SYMBOL_UNKNOW );

        // PrintSymbolTable();
        if ( IsDefinedSymbol( re ) ) { // find in table
          re->mType = IsDefinedSymbol( re )->mType;
          re->mIntValue = IsDefinedSymbol( re )->mIntValue;
          re->mFloatValue = IsDefinedSymbol( re )->mFloatValue;
          // cout << "find it\n";
        } // if()
        // else cout << "not find ~\n";
      } // else 
    } // if()

    else if ( IsDigit( ch ) || ch == '.' ) { // this token is a number
      if ( ch == '.' ) {
        cin.get(); // get '.'
        ch = cin.peek();
        cin.putback( '.' );

        if ( !IsDigit( ch ) ) { // input is only a dot 
          re = new Unit( ".", UNEXPECTED_TOKEN );
          return re;
        } // if()
        else ch = cin.peek();
      } // if()

      bool isFloat = false;
      string number = ""; // ﹃钡ノ

      while ( IsDigit( ch ) ) { // 讽硂 while,俱计场だЧΘ
        cin.get();
        number += ch;
        ch = cin.peek();
      } // while()

      if ( ch == '.' ) { // 矪瞶计翴㎝计场だ
        isFloat = true;
        cin.get();
        number += ch; // 计翴
        ch = cin.peek();

        while ( IsDigit( ch ) ) { // 讽 while,计场だЧΘ
          cin.get();
          number += ch;
          ch = cin.peek();        
        } // while()
      } // if()

      if ( isFloat ) {
        re = new Unit( number, FLOAT );
        re->StringToFloat();
      } // if()
      else {
        re = new Unit( number, INT );
        re->StringToInt();
      } // else        
    } // else if()

    else if ( ch == '"' ) { // 34 = "
      cin.get();
      re = new Unit( "\"", DOUBLE_COMMA );
    } // else if()

    else if ( ch == 39 ) { // 39 = '
      cin.get();
      re = new Unit( "'", COMMA );
    } // else if()

    else if ( ch == '(' ) { // ( = left parenthesis    
      cin.get();
      re = new Unit( "(", L_PAREN );
    } // else if()

    else if ( ch == ')' ) { // ( = right parenthesis    
      cin.get();
      re = new Unit( ")", R_PAREN );
    } // else if()

    else if ( ch == '{' ) {   
      cin.get();
      re = new Unit( "{", L_BIG_PAREN );
    } // else if()

    else if ( ch == '}' ) {    
      cin.get();
      re = new Unit( "}", R_BIG_PAREN );
    } // else if()

    else if ( ch == '[' ) {    
      cin.get();
      re = new Unit( "[", L_MID_PAREN );
    } // else if()

    else if ( ch == ']' ) {    
      cin.get();
      re = new Unit( "]", R_MID_PAREN );
    } // else if()

    else if ( ch == ';' ) { // ; = semicolon   
      cin.get();
      re = new Unit( ";", SEMICOLON );
    } // else if()

    else if ( ch == ':' ) { // := = statement operator
      cin.get();
      ch = cin.peek();

      if ( ch == '=' ) {
        cin.get();
        re = new Unit( ":=", OPE_STATEMENT );
      } // if()
      else re = new Unit( ":", UNRECOGNIZED_TOKEN );
    } // else if()

    else if ( ch == '=' ) { // = statement operator
      cin.get();
      re = new Unit( "=", OPE_STATEMENT );
    } // else if()

    else if ( ch == '<' ) { // <, <=, <> = compare operator
      cin.get();
      ch = cin.peek();

      if ( ch == '=' ) {
        cin.get();
        re = new Unit( "<=", OPE_COMPARE );
      } // if()
      else if ( ch == '>' ) {
        cin.get();
        re = new Unit( "<>", OPE_COMPARE );
      } // else if()
      else if ( ch == '<' ) {
        cin.get();
        re = new Unit( "<<", L_ARROW );
      } // else if()
      else re = new Unit( "<", OPE_COMPARE );
    } // else if()

    else if ( ch == '>' ) { // >, >= = compare operator
      cin.get();
      ch = cin.peek();

      if ( ch == '=' ) {
        cin.get();
        re = new Unit( ">=", OPE_COMPARE );
      } // if()
      else if ( ch == '>' ) {
        cin.get();
        re = new Unit( ">>", R_ARROW );
      } // else if()
      else re = new Unit( ">", OPE_COMPARE );
    } // else if()

    else if ( ch == '=' ) { // '=' = compare operator
      cin.get();
      re = new Unit( "=", OPE_COMPARE );
    } // else if()

    else if ( ch == '*' ) { // * = multiply operator
      cin.get();
      re = new Unit( "*", OPE_MUL );
    } // else if()

    else if ( ch == '/' ) { // '/' = divide operator
      cin.get();
      ch = cin.peek();
    
      if ( ch == '/' ) { // is comment
        IgnoreThisLine();
        mErrorLine++;
        re = GetToken( IsFactorOrNot ); // to get next token
      } // if()
      else re = new Unit( "/", OPE_DIV );
    } // else if()

    else if ( ch == '+' ) { // '+' = add operator
      cin.get();
    
      if ( IsFactorOrNot ) { // ex : +9.9, +6
        IsFactorOrNot = false;
        re = GetToken( IsFactorOrNot ); // to get next token ( Num )

        if ( re->mType == INT || re->mType == FLOAT ) ; // +9
        else if ( re->mType == UNRECOGNIZED_TOKEN ) ;   // +@
        else re->mType = UNEXPECTED_TOKEN;              // ++
      } // if
      else re = new Unit( "+", OPE_ADD );
    } // else if()

    else if ( ch == '-' ) { // '-' = sub operator
      cin.get();
    
      if ( IsFactorOrNot ) { // ex : -3, -9.9
        IsFactorOrNot = false;
        re = GetToken( IsFactorOrNot ); // to get next token ( Num )

        if ( re->mType == INT || re->mType == FLOAT ) { // -9
          re->mIntValue   = -re->mIntValue; 
          re->mFloatValue = -re->mFloatValue; 
        } // if ()
        else if ( re->mType == UNRECOGNIZED_TOKEN ) ;   // -@
        else re->mType = UNEXPECTED_TOKEN;              // -+
      } // if()
      else re = new Unit( "-", OPE_SUB );
    } // else if()

    else { // error, ex : @ ! ? [ ]
      cin.get();
      string s = "";
      s += ch;

      re = new Unit( s, UNRECOGNIZED_TOKEN );
    } // else

    return re;
  } // GetToken()


  private : Unit* PeekToken( bool IsFactorOrNot ) {
    // num, symbol, +, -, *, / , (, ), <, =, >, :=, ;
    SkipWhiteSpace();
    char ch = cin.peek();
    Unit* re; // use to return

    if ( ch == ';' ) re = new Unit( ";", SEMICOLON );
    else if ( ch == '=' ) re = new Unit( "=", OPE_STATEMENT );
    else if ( ch == 39 )  re = new Unit( "'", COMMA ); // 39 = '
    else if ( ch == ':' ) { // := = statement operator
      cin.get();
      ch = cin.peek();

      if ( ch == '=' ) {
        cin.putback( ':' );
        re = new Unit( ":=", OPE_STATEMENT );
      } // if()
      else re = new Unit( ":", UNRECOGNIZED_TOKEN );
    } // else if()
    else if ( ch == '<' ) {
      cin.get();
      ch = cin.peek();
 
      if ( ch == '<' ) re = new Unit( "<<", L_ARROW );
      else re = new Unit( "<=>", OPE_COMPARE );

      cin.putback( '<' );
    } // else if()
    else if ( ch == '>' ) {
      cin.get();
      ch = cin.peek();
 
      if ( ch == '>' ) re = new Unit( ">>", R_ARROW );
      else re = new Unit( "<=>", OPE_COMPARE );

      cin.putback( '>' );
    } // else if()
    else if ( ch == '+' ) {

      if ( IsFactorOrNot ) { // ex : +9.9, +6
        cin.get();
        IsFactorOrNot = false;
        re = PeekToken( IsFactorOrNot ); // to get next token ( Num )

        if ( re->mType == INT ) cin.putback( '+' );   // +9
        else if ( re->mType == UNRECOGNIZED_TOKEN ) ; // +@
        else re->mType = UNEXPECTED_TOKEN;            // ++
      } // if
      else re = new Unit( "+", OPE_ADD );
    } // else if()
    else if ( ch == '-' ) {

      if ( IsFactorOrNot ) { // ex : +9.9, +6
        cin.get();
        IsFactorOrNot = false;
        re = PeekToken( IsFactorOrNot ); // to get next token ( Num )

        if ( re->mType == INT ) cin.putback( '-' );   // +9
        else if ( re->mType == UNRECOGNIZED_TOKEN ) ; // +@
        else re->mType = UNEXPECTED_TOKEN;            // ++
      } // if
      else re = new Unit( "", OPE_SUB );
    } // else if()
    else if ( ch == '*' ) re = new Unit( "", OPE_MUL );
    else if ( ch == '/' ) {
      cin.get();
      ch = cin.peek();

      if ( ch == '/' ) {
        IgnoreThisLine();
        mErrorLine++;
        re = PeekToken( IsFactorOrNot );
      } // if()
      else {
        cin.putback( '/' );
        re = new Unit( "", OPE_DIV );
      } // else
    } // else if()
    else if ( ch == '(' ) re = new Unit( "(", L_PAREN );
    else if ( ch == ')' ) re = new Unit( ")", R_PAREN );
    else if ( ch == '{' ) re = new Unit( "{", L_BIG_PAREN );
    else if ( ch == '}' ) re = new Unit( "}", R_BIG_PAREN );
    else if ( ch == '[' ) re = new Unit( "[", L_MID_PAREN );
    else if ( ch == ']' ) re = new Unit( "]", R_MID_PAREN );
    else if ( IsDigit( ch ) || ch == '.' ) {

      if ( ch == '.' ) {
        cin.get(); // get '.'
        ch = cin.peek();
        cin.putback( '.' );

        if ( !IsDigit( ch ) ) { // input is only a dot 
          re = new Unit( ".", UNEXPECTED_TOKEN );
          return re;
        } // if()
        else ch = cin.peek();
      } // if()

      string number = ""; // ﹃钡ノ

      while ( IsDigit( ch ) ) { // 讽硂 while,俱计场だЧΘ
        cin.get();
        number += ch;
        ch = cin.peek();
      } // while()

      if ( ch == '.' ) { // 矪瞶计翴㎝计场だ
        cin.get();
        number += ch; // 计翴
        ch = cin.peek();

        while ( IsDigit( ch ) ) { // 讽 while,计场だЧΘ
          cin.get();
          number += ch;
          ch = cin.peek();        
        } // while()
      } // if()

      re = new Unit( number, INT ); // or float

      while ( number.length() > 0 ) { // to put this token back
        cin.putback( number[number.length() - 1] );
        number = number.substr( 0, number.length() - 1 );
      } // while()

    } // else if()
    else if ( IsAlpha( ch ) ) {
      string symbol = ""; // ﹃钡ノ

      while ( IsAlpha( ch ) || IsDigit( ch ) ) {
        cin.get();
        symbol += ch;
        ch = cin.peek();
      } // while()

      if ( symbol == "quit" ) re = new Unit( "quit", QUIT );
      else if ( symbol == "int" )    re = new Unit( "int",    TYPE_INT );
      else if ( symbol == "string" ) re = new Unit( "string", TYPE_STRING );
      else if ( symbol == "char"   ) re = new Unit( "char",   TYPE_CHAR );
      else if ( symbol == "bool" )   re = new Unit( "bool",   TYPE_BOOL );
      else if ( symbol == "true" )   re = new Unit( "true",   TRUE );
      else if ( symbol == "false" )  re = new Unit( "false",  FALSE );
      else if ( symbol == "cout" ) re = new Unit( "cout", COUT );
      else if ( symbol == "Done" ) re = new Unit( "Done", DONE );
      else {
        re = new Unit( symbol, SYMBOL_UNKNOW );

        if ( IsDefinedSymbol( re ) ) { // find it in table
          re->mType = IsDefinedSymbol( re )->mType;
          re->mIntValue = IsDefinedSymbol( re )->mIntValue;
          re->mFloatValue = IsDefinedSymbol( re )->mFloatValue;
        } // if()
      } // else 

      while ( symbol.length() > 0 ) { // to put this token back
        cin.putback( symbol[symbol.length() - 1] );
        symbol = symbol.substr( 0, symbol.length() - 1 );
      } // while()

    } // else if()
    else { // illegal char, ex : [, ], #, @, &
      string s = "";
      s += ch;
      re = new Unit( s, UNRECOGNIZED_TOKEN );
    } // else()

    return re;
  } // PeekToken()


  private : Unit* Factor( Unit* gettedToken ) {
    // <factor> ::= NUM | SYMBOL | (<Exp>)
    Unit* u;
    bool thisTokenIsFactor = true, thisTokenIsNotFactor = false;

    if ( gettedToken == NULL ) u = GetToken( thisTokenIsFactor );
    else u = gettedToken;

    // cout << "In Factor(), u = " << u->mUnit << "\n";
    // cout << "In Factor(), u->mType = " << u->mType << "\n";

    if ( u->mType == QUIT || u->mType == SYMBOL_UNKNOW ) { // factor error
      u->mType = UNDEFINED_SYMBOL;
      return u;
    } // if()
    else if ( u->mType != INT && u->mType != FLOAT && // factor error
              u->mType != SYMBOL_INT && u->mType != SYMBOL_FLOAT &&
              u->mType != L_PAREN ) {
      if ( u->mType == UNRECOGNIZED_TOKEN ) ; // @
      else u->mType = UNEXPECTED_TOKEN;       // - '+'

      return u;
    } // else if()
  
    else if ( u->mType == INT || u->mType == FLOAT || u->mType == SYMBOL_FLOAT ) { // factor correct 1
      return u;
    } // else if()
  
    else if ( u->mType == SYMBOL_INT ) {
      Unit* ope1 = PeekToken( thisTokenIsNotFactor );

      if ( ope1->mType == OPE_ADD || ope1->mType == OPE_SUB ) {
        ope1 = GetToken( thisTokenIsNotFactor );
        Unit* ope2 = PeekToken( thisTokenIsNotFactor );

        if ( ope1->mType == ope2->mType ) { // maybe is ++ or --
          ope2 = GetToken( thisTokenIsNotFactor );
          Unit* ope3 = PeekToken( thisTokenIsNotFactor );

          if ( ope3->mType == OPE_ADD || ope3->mType == OPE_SUB || 
               ope3->mType == OPE_MUL || ope3->mType == OPE_DIV || 
               ope3->mType == SEMICOLON ) { // is ++ or --
            u->mIntValue++;
            u->mFloatValue = u->mIntValue;
            return u;
          } // if()
          else { // not ++ or --
            cin.putback( ope2->mUnit[0] );
            cin.putback( ope1->mUnit[0] );
            return u;
          } // else
        } // if()
        else {
          cin.putback( ope1->mUnit[0] );
          return u;
        } // else
      } // if()
      else return u;
    } // else if()

    else { // factor correct 2, tokenType = L_PAREN = '('
      u = Exp( NULL );
      // cout << "In Factor(), u->mUnit = " << u->mUnit << "\n";
      // cout << "In Factor(), u->mType = " << u->mType << "\n";

      if ( u->mType == UNDEFINED_SYMBOL ) { // exp error
        return u;
      } // if()
      else if ( u->mType != INT        && u->mType != FLOAT && 
                u->mType != SYMBOL_INT && u->mType != SYMBOL_FLOAT ) {
        if ( u->mType == UNRECOGNIZED_TOKEN ) ; // @
        else u->mType = UNEXPECTED_TOKEN;       // *

        return u;
      } // if()
      else { // exp correct
        Unit* r_paren = GetToken( thisTokenIsNotFactor );
      
        if ( r_paren->mType != R_PAREN ) { // r_paren error
          if ( r_paren->mType == UNRECOGNIZED_TOKEN ) ; // illegal char
          else r_paren->mType = UNEXPECTED_TOKEN;       // syntax error

          return r_paren;
        } // if()
        else { // r_paren correct
          return u;
        } // else 
      } // else
    } // else

  } // Factor()


  private : Unit* Term( Unit* gettedToken ) {
    // <term> ::= <factor> {* <factor> | / <factor>}
    bool thisTokenIsNotFactor = false;
    Unit* factor1 = Factor( gettedToken );
    // cout << "In Term(), factor1->mUnit = " << factor1->mUnit << "\n";
    // cout << "In Term(), factor1->mType = " << factor1->mType << "\n";
    // cout << "In Term(), factor1->mIntValue = " << factor1->mIntValue << "\n";
    // cout << "In Term(), factor1->mFloatValue = " << factor1->mFloatValue << "\n";

    // factor1 error
    if ( factor1->mType == UNDEFINED_SYMBOL ) {
      return factor1;
    } // if()
    else if ( factor1->mType != INT && factor1->mType != FLOAT &&
              factor1->mType != SYMBOL_INT  && factor1->mType != SYMBOL_FLOAT ) {
      if ( factor1->mType == UNRECOGNIZED_TOKEN ) ; // @
      else factor1->mType = UNEXPECTED_TOKEN;       // *

      return factor1;
    } // else if()
  
    // factor1 correct
    do {
      Unit* ope = PeekToken( thisTokenIsNotFactor );

      if ( ope->mType != OPE_MUL && ope->mType != OPE_DIV ) { 
        return factor1;
      } // if()

      // there is '*' or '/' behind the factor1
      ope = GetToken( thisTokenIsNotFactor );
      Unit* factor2 = Factor( NULL );
      // cout << "In Term(), factor2->mUnit = " << factor2->mUnit << "\n";
      // cout << "In Term(), factor2->mType = " << factor2->mType << "\n";

      // factor2 error
      if ( factor2->mType == UNDEFINED_SYMBOL ) {
        return factor2;
      } // if()
      else if ( factor2->mType != INT && factor2->mType != FLOAT && 
                factor2->mType != SYMBOL_INT && factor2->mType != SYMBOL_FLOAT ) {
        if ( factor2->mType == UNRECOGNIZED_TOKEN ) ; // @
        else factor2->mType = UNEXPECTED_TOKEN;       // *

        return factor2;
      } // if()  
    
      // factor2 correct
      if ( ope->mType == OPE_MUL ) { // ope is '*'
        if ( ( factor1->mType == INT || factor1->mType == SYMBOL_INT ) &&
             ( factor2->mType == INT || factor2->mType == SYMBOL_INT ) ) { // int = int * int
          factor1->mIntValue *= factor2->mIntValue;
          factor1->mFloatValue = factor1->mIntValue;
          // cout << "value = " << factor1->mFloatValue << "\n";
        } // if()
        else { // float = int * float = float * int = float * float
          if ( factor1->mType == SYMBOL_INT ) factor1->mType = SYMBOL_FLOAT;
          else factor1->mType = FLOAT;

          factor1->mIntValue = 0;
          factor1->mFloatValue *= factor2->mFloatValue;
        } // else
      } // if()

      else { // ope is '/'
        if ( ( factor1->mType == INT || factor1->mType == SYMBOL_INT ) &&
             ( factor2->mType == INT || factor2->mType == SYMBOL_INT ) &&
             factor1->mIntValue % factor2->mIntValue == 0 ) {
          factor1->mIntValue /= factor2->mIntValue;
          factor1->mFloatValue = factor1->mIntValue; 
        } // if()
        else {
          if ( factor1->mType == SYMBOL_INT ) factor1->mType = SYMBOL_FLOAT;
          else factor1->mType = FLOAT;

          factor1->mIntValue = 0;
          factor1->mFloatValue /= factor2->mFloatValue;
        } // else 
      } // else 

    } while ( 1 );

  } // Term()


  private : Unit* Exp( Unit* gettedToken ) {
    // <Exp> ::= <term> {+ <term> | - <term>}
    bool thisTokenIsNotFactor = false;
    Unit* term1 = Term( gettedToken );
    // cout << "In Exp(), term1->mUnit = " << term1->mUnit << "\n";
    // cout << "In Exp(), term1->mType = " << term1->mType << "\n";
    // cout << "In Exp(), term1->mIntValue = " << term1->mIntValue << "\n";

    // Term1 error
    if ( term1->mType == UNDEFINED_SYMBOL ) {
      return term1;
    } // if()
    else if ( term1->mType != INT && term1->mType != FLOAT && 
              term1->mType != SYMBOL_INT && term1->mType != SYMBOL_FLOAT ) {
      if ( term1->mType == UNRECOGNIZED_TOKEN ) ; // @
      else term1->mType = UNEXPECTED_TOKEN;       // *

      return term1;
    } // if()

    do { // Term1 correct
      Unit* ope = PeekToken( thisTokenIsNotFactor );
      // cout << "PeekToken ope->mUnit = " << ope->mUnit << "\n";
      // cout << "PeekToken ope->mType = " << ope->mType << "\n";

      if ( ope->mType == UNEXPECTED_TOKEN || ope->mType == UNRECOGNIZED_TOKEN )  // ope error
        return ope;
      else if ( ope->mType == INT || ope->mType == FLOAT || ope->mType == L_PAREN || // ope error
                ope->mType == SYMBOL_INT || ope->mType == SYMBOL_FLOAT || ope->mType == SYMBOL_UNKNOW ||
                ope->mType == UNDEFINED_SYMBOL ) {
        ope->mType = UNEXPECTED_TOKEN;
        return ope;
      } // else if()
      else if ( ope->mType != OPE_ADD && ope->mType != OPE_SUB ) { // correct, term is end
        return term1;
      } // if()

      // there is '+' or '-' behind the term1
      ope = GetToken( thisTokenIsNotFactor );
      // Unit* ope2 = PeekToken( thisTokenIsNotFactor );
      Unit* term2 = Term( NULL );
      // cout << "In Exp(), term2->mUnit = " << term2->mUnit << "\n";
      // cout << "In Exp(), term2->mType = " << term2->mType << "\n";

      // Term2 error
      if ( term2->mType == UNDEFINED_SYMBOL ) {
        term2->mType = UNDEFINED_SYMBOL;
        return term2;
      } // if()
      else if ( term2->mType != INT && term2->mType != FLOAT && 
                term2->mType != SYMBOL_UNKNOW && 
                term2->mType != SYMBOL_INT && term2->mType != SYMBOL_FLOAT ) {
        if ( term2->mType == UNRECOGNIZED_TOKEN ) ; // @
        else term2->mType = UNEXPECTED_TOKEN;       // *

        return term2;
      } // if()

      // Term2 correct
      if ( ope->mType == OPE_ADD ) { // ope is '+'
        if ( term1->mType == INT && term2->mType == INT ) { // int = int + int  
          term1->mIntValue += term2->mIntValue;
          term1->mFloatValue = term1->mIntValue;
        } // if()
        else { // float = int + float = float + int = float + float
          term1->mType = FLOAT;
          term1->mIntValue = 0;
          term1->mFloatValue += term2->mFloatValue;
        } // else
      } // if()

      else { // ope is '-'
        if ( term1->mType == INT && term2->mType == INT ) {
          term1->mIntValue -= term2->mIntValue;
          term1->mFloatValue = term1->mIntValue; 
        } // if()
        else {
          term1->mType = FLOAT;
          term1->mIntValue = 0;
          term1->mFloatValue -= term2->mFloatValue;
        } // else 
      } // else 
  
    } while ( 1 );

  } // Exp()


  private : Unit* BoolExp( Unit* gettedToken ) {
    // <BoolExp> ::= <Exp> = <Exp>
    bool thisTokenIsFactor = true, thisTokenIsNotFactor = false;
    Unit* u1 = gettedToken;
    Unit* u2 = GetToken( thisTokenIsNotFactor );
    // cout << "In BoolExp, u1->mIntValue = " << u1->mIntValue << "\n";
    // cout << "In BoolExp, u2->mUnit = " << u2->mUnit << "\n";

    if ( u2->mType == OPE_COMPARE ) { // u2 correct
      Unit* u3 = Exp( NULL );
      // cout << "In BoolExp, u3->mUnit = " << u3->mUnit << "\n";
      // cout << "In BoolExp, u3->mIntValue = " << u3->mIntValue << "\n";
      // cout << "In BoolExp, u3->mFloatValue = " << u3->mFloatValue << "\n";

      if ( u3->mType == INT || u3->mType == FLOAT ||
           u3->mType == SYMBOL_INT || u3->mType == SYMBOL_FLOAT ) { // u3 correct
        float num1 = u1->mFloatValue * 10000;
        float num2 = u3->mFloatValue * 10000;
        float difference = num1 - num2;
        Unit* re; // use to return result

        if ( num1 < num2 ) difference = num2 - num1;

        if ( u2->mUnit == "<" && num1 < num2 && difference > 1 ) // <
          re = new Unit( "true", BOOLEXP_ANS );
        else if ( u2->mUnit == "<=" && ( ( num1 < num2 && difference > 1 ) || difference <= 1 ) ) // < or =
          re = new Unit( "true", BOOLEXP_ANS );
        else if ( u2->mUnit == "<>" && difference > 1 ) // num1 != num2
          re = new Unit( "true", BOOLEXP_ANS );
        else if ( u2->mUnit == "=" && difference <= 1 ) // =
          re = new Unit( "true", BOOLEXP_ANS );
        else if ( u2->mUnit == ">" && num1 > num2 && difference > 1 ) // >
          re = new Unit( "true", BOOLEXP_ANS );
        else if ( u2->mUnit == ">=" && ( ( num1 > num2 && difference > 1 ) || difference <= 1 ) ) // > or =
          re = new Unit( "true", BOOLEXP_ANS );
        else
          re = new Unit( "false", BOOLEXP_ANS );

        return re;
      } // if()   
      else if ( u3->mType == UNDEFINED_SYMBOL || u3->mType == UNRECOGNIZED_TOKEN ) // u3 error
        return u3;
      else { // u3 error -> syntax error
        u3->mType = UNEXPECTED_TOKEN;
        return u3;
      } // else
    } // if()

    else if ( u2->mType == UNRECOGNIZED_TOKEN ) // u2 error -> illegal char
      return u2;

    else { // u2 error -> syntax error
      u2->mType = UNEXPECTED_TOKEN;
      return u2;
    } // else

    return NULL; 

  } // BoolExp()


  private : Unit* Cout() {
    Unit *u1, *u2;
    bool thisTokenIsNotFactor = false, break_ = false;
 
    do { // cout << x << "123123" << z ;
      u1 = GetToken( thisTokenIsNotFactor ); // <<

      if ( u1->mType != L_ARROW ) {
        if ( u1->mType == UNEXPECTED_TOKEN ) return u1;
        else {
          u1->mType = UNEXPECTED_TOKEN;
          return u1;
        } // else
      } // if()

      u2 = GetToken( thisTokenIsNotFactor ); // x

      if ( u2->mType == DOUBLE_COMMA ) {
        char ch = cin.peek();

        while ( ch != '"' ) {
          cin.get();
          ch = cin.peek();
        } // while()

        if ( ch == '"' ) cin.get();
      } // if()
      else if ( u2->mType == UNEXPECTED_TOKEN ) return u2;
      else if ( ! IsDefinedSymbol( u2 ) ) {
        u2->mType = UNDEFINED_SYMBOL;
        return u2;
      } // if()

      u1 = PeekToken( thisTokenIsNotFactor ); // <<
      if ( u1->mType != L_ARROW ) break_ = true;
    } while ( ! break_ ) ;

    return NULL;
  } // Cout()


  private : Unit* Cin() {
    Unit *u1, *u2;
    bool thisTokenIsFactor = true, thisTokenIsNotFactor = false, break_ = false;
 
    do { // cin >> x >> z ;
      u1 = GetToken( thisTokenIsNotFactor ); // >> 

      if ( u1->mType != R_ARROW ) {
        if ( u1->mType == UNEXPECTED_TOKEN ) return u1;
        else {
          u1->mType = UNEXPECTED_TOKEN;
          return u1;
        } // else
      } // if()

      u2 = GetToken( thisTokenIsFactor ); // x

      if ( u2->mType == UNEXPECTED_TOKEN ) return u2;
      else if ( ! IsDefinedSymbol( u2 ) ) {
        u2->mType = UNDEFINED_SYMBOL;
        return u2;
      } // if()

      u1 = PeekToken( thisTokenIsNotFactor ); // >> 
      if ( u1->mType != R_ARROW ) break_ = true;
    } while ( ! break_ ) ;

    return NULL;
  } // Cin()


  private : Unit* AssignString() {
    Unit* u1;
    bool thisTokenIsNotFactor = false, break_ = false;
    // string s;
    // s = "a" + s + "c" 
    u1 = GetToken( thisTokenIsNotFactor ); // to get " or SYMBOL_STRING
 
    if ( u1->mType == SYMBOL_STRING || u1->mType == SYMBOL_CHAR ) ;
    else if ( u1->mType == DOUBLE_COMMA ) {
      char ch = cin.peek();

      while ( ch != '"' ) { // 34 = "
        cin.get();
        ch = cin.peek();
      } // while()

      if ( ch == '"' ) cin.get();
    } // else if()
    else { // u1 error
      return u1;
    } // else


    do { // string s = "a" + "b" + "c" ;
         // s = "a";
      u1 = PeekToken( thisTokenIsNotFactor ); // + or ;

      if ( u1->mType == SEMICOLON ) break_ = true;
      else if ( u1->mType == OPE_ADD ) {
        u1 = GetToken( thisTokenIsNotFactor ); // to get +
        u1 = GetToken( thisTokenIsNotFactor ); // to get " or SYMBOL_STRING

        if ( u1->mType == SYMBOL_STRING || u1->mType == SYMBOL_CHAR ) ;
        else if ( u1->mType == DOUBLE_COMMA ) {
          char ch = cin.peek();

          while ( ch != '"' ) { // 34 = "
            cin.get();
            ch = cin.peek();
          } // while()

          if ( ch == '"' ) cin.get();
        } // else if()
        else {
          return u1;
        } // else
      } // else if
      else {
        return u1;
      } // else 

    } while ( ! break_ ) ;

    return NULL;
  } // AssignString()


  public : Unit* Command() {
    // 1. quit | 2. symbol := <exp> ; | 3. <exp> >= <exp> ; | 4. <exp> ;
    // 5. defined | 6. cin cout | 7. Done(); | 8. {  } | 9. ReDefined
    SkipWhiteSpace();
    mErrorLine = 1;
    bool thisTokenIsFactor = true, thisTokenIsNotFactor = false;
    Unit* u1 = GetToken( thisTokenIsFactor );
    // cout << "In command, u1->mUnit = " << u1->mUnit << "\n";
    // cout << "In command, u1->mType = " << u1->mType << "\n";

    if ( u1->mType == QUIT ) return u1; // case 1
    else if ( u1->mType == DONE ) return u1; // case 7
    else if ( u1->mType == SEMICOLON ) { // input is only ';'
      return u1;
    } // else if()
    else if ( u1->mType == UNRECOGNIZED_TOKEN ) { // u1 error -> illegal char
      IgnoreThisLine();
      return u1;
    } // else if()
    else if ( u1->mType == SYMBOL_UNKNOW ) { // u1 error -> undefined symbol
      IgnoreThisLine();
      u1->mType = UNDEFINED_SYMBOL;
      return u1;
    } // else if()
    else if ( u1->mType != INT && u1->mType != FLOAT && u1->mType != L_PAREN && // u1 error -> syntax error
              u1->mType != SYMBOL_UNKNOW && u1->mType != SYMBOL_INT && u1->mType != SYMBOL_FLOAT && 
              u1->mType != SYMBOL_STRING && u1->mType != SYMBOL_CHAR && u1->mType != SYMBOL_BOOL && 
              u1->mType != TYPE_INT  && u1->mType != TYPE_STRING && u1->mType != TYPE_FLOAT && 
              u1->mType != TYPE_CHAR && u1->mType != TYPE_BOOL   && u1->mType != L_BIG_PAREN &&
              u1->mType != CIN && u1->mType != COUT ) {
      u1->mType = UNEXPECTED_TOKEN;
      IgnoreThisLine();
      return u1;
    } // else if ()

    Unit* u2 = PeekToken( thisTokenIsNotFactor );
    // cout << "In command, u2->mUnit = " << u2->mUnit << "\n";
    // cout << "In command, u2->mType = " << u2->mType << "\n";

    if ( u2->mType == UNEXPECTED_TOKEN || u2->mType == UNRECOGNIZED_TOKEN || // u2 error
         u2->mType == UNDEFINED_SYMBOL ) {
      IgnoreThisLine();
      return u2; 
    } // if
    else if ( u1->mType == CIN ) { // case 6
      u2 = Cin();

      if ( u2 != NULL ) { // error
        IgnoreThisLine();
        return u2;
      } // if()
      else {
        u2 = GetToken( thisTokenIsNotFactor ); // to get ';'
        return u1;
      } // else
    } // else if
    else if ( u1->mType == COUT ) { // case 6
      u2 = Cout();

      if ( u2 != NULL ) { // error
        IgnoreThisLine();
        return u2;
      } // if()
      else {
        u2 = GetToken( thisTokenIsNotFactor ); // to get ';'
        return u1;
      } // else
    } // else if
    else if ( u1->mType == L_BIG_PAREN ) { // case 8
      u1 = GetToken( thisTokenIsNotFactor );

      while ( u1->mType != R_BIG_PAREN ) u1 = GetToken( thisTokenIsNotFactor );

      return u1; 
    } // if
    else if ( u1->mType == TYPE_INT && u2->mType == SYMBOL_UNKNOW ) { // case 5
      // int x ;
      // int x = 5;
      u1 = GetToken( thisTokenIsFactor );    // to get symbol
      u2 = GetToken( thisTokenIsNotFactor ); // to get ';' or '=' or '['

      if ( u2->mType == SEMICOLON ) {
        u1->mType = SYMBOL_INT, u1->mIntValue = u1->mFloatValue = 0;
        mSymbolTable.push_back( u1 );

        u2->mType = DEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // if ()
      else if ( u2->mType == OPE_STATEMENT ) {
        u2 = Exp( NULL );

        if ( u2->mType == INT || u2->mType == SYMBOL_INT ) {
          u1->mType = SYMBOL_INT, u1->mIntValue = u1->mFloatValue = u2->mIntValue;
          mSymbolTable.push_back( u1 );

          u2 = GetToken( thisTokenIsNotFactor ); // to get ';'

          if ( u2->mType == SEMICOLON ) {
            u2->mType = DEFINED, u2->mUnit = u1->mUnit;
            return u2;
          } // if()
          else if ( u2->mType != SEMICOLON ) { // error
            IgnoreThisLine();
            mSymbolTable.pop_back();

            if ( u2->mType == UNRECOGNIZED_TOKEN ) ;
            else u2->mType = UNEXPECTED_TOKEN;              
            return u2;
          } // else if()
        } // if()
        else { // u2 error
          IgnoreThisLine();
          return u2;
        } // else
      } // else if()
      else if ( u2->mType == L_MID_PAREN ) {
        u2 = GetToken( thisTokenIsNotFactor ); // to get number
        u2 = GetToken( thisTokenIsNotFactor ); // to get ]
        u2 = GetToken( thisTokenIsNotFactor ); // to get ;
        u2->mType = DEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // else if()
      else {
        ; // error
      } // else
    } // else if ()
    else if ( u1->mType == TYPE_STRING && u2->mType == SYMBOL_UNKNOW ) { // case 5
      u1 = GetToken( thisTokenIsFactor );    // to get symbol
      u2 = GetToken( thisTokenIsNotFactor ); // to get ';' or '='

      if ( u2->mType == SEMICOLON ) {
        u1->mType = SYMBOL_STRING, u1->mIntValue = u1->mFloatValue = 0;
        mSymbolTable.push_back( u1 );

        u2->mType = DEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // if ()
      else if ( u2->mType == OPE_STATEMENT ) {
        AssignString(); // string s = "a" + "b" + "c" ; 
        GetToken( thisTokenIsNotFactor ); // to get ';'
        u2->mType = DEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // else if()
      else if ( u2->mType == L_MID_PAREN ) {
        u2 = GetToken( thisTokenIsNotFactor ); // to get number
        u2 = GetToken( thisTokenIsNotFactor ); // to get ]
        u2 = GetToken( thisTokenIsNotFactor ); // to get ;
        u2->mType = DEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // else if()
      else { // u2 error
        IgnoreThisLine();
        return u2;
      } // else
    } // else if()
    else if ( u1->mType == TYPE_CHAR && u2->mType == SYMBOL_UNKNOW ) { // case 5
      u1 = GetToken( thisTokenIsFactor );    // to get symbol
      u2 = GetToken( thisTokenIsNotFactor ); // to get ';' or '='

      if ( u2->mType == SEMICOLON ) {
        u1->mType = SYMBOL_CHAR, u1->mIntValue = u1->mFloatValue = 0;
        mSymbolTable.push_back( u1 );

        u2->mType = DEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // if ()
      else if ( u2->mType == L_MID_PAREN ) {
        u2 = GetToken( thisTokenIsNotFactor ); // to get number
        u2 = GetToken( thisTokenIsNotFactor ); // to get ]

        u1->mType = SYMBOL_CHAR, u1->mIntValue = u1->mFloatValue = 0;
        mSymbolTable.push_back( u1 );        
 
        u2 = GetToken( thisTokenIsNotFactor ); // to get ;
        u2->mType = DEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // else if()
    } // else if()
    else if ( u1->mType == TYPE_FLOAT && u2->mType == SYMBOL_UNKNOW ) { // case 5
      u1 = GetToken( thisTokenIsFactor );    // to get symbol
      u2 = GetToken( thisTokenIsNotFactor ); // to get ';' or '='

      if ( u2->mType == SEMICOLON ) {
        u1->mType = SYMBOL_FLOAT, u1->mIntValue = u1->mFloatValue = 0;
        mSymbolTable.push_back( u1 );

        u2->mType = DEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // if ()
      else if ( u2->mType == OPE_STATEMENT ) {
        u2 = Exp( NULL );

        if ( u2->mType == FLOAT || u2->mType == SYMBOL_FLOAT ) {
          u1->mType = SYMBOL_FLOAT, u1->mIntValue =  0, u1->mFloatValue = u2->mFloatValue;
          mSymbolTable.push_back( u1 );

          u2 = GetToken( thisTokenIsNotFactor ); // to get ';'

          if ( u2->mType == SEMICOLON ) {
            u2->mType = DEFINED, u2->mUnit = u1->mUnit;
            return u2;
          } // if()
          else if ( u2->mType != SEMICOLON ) { // error
            IgnoreThisLine();
            mSymbolTable.pop_back();

            if ( u2->mType == UNRECOGNIZED_TOKEN ) ;
            else u2->mType = UNEXPECTED_TOKEN;              
            return u2;
          } // if()
        } // if()
        else { // u2 error
          IgnoreThisLine();
          return u2;
        } // else
      } // else if()
    } // else if()
    else if ( u1->mType == TYPE_INT && // case 9, ReDefined to int
              ( u2->mType == SYMBOL_INT || u2->mType == SYMBOL_FLOAT || u2->mType == SYMBOL_CHAR ||
                u2->mType == SYMBOL_STRING || u2->mType == SYMBOL_BOOL ) ) {
      // int x ;
      // int x = 5;
      u1 = GetToken( thisTokenIsFactor );    // to get symbol
      u2 = GetToken( thisTokenIsNotFactor ); // to get ';' or '='

      if ( u2->mType == SEMICOLON ) {
        u1->mType = SYMBOL_INT, u1->mIntValue = u1->mFloatValue = 0;
        mSymbolTable.push_back( u1 );

        u2->mType = REDEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // if ()
      else if ( u2->mType == OPE_STATEMENT ) {
        u2 = Exp( NULL );

        if ( u2->mType == INT || u2->mType == SYMBOL_INT ) {
          u1->mType = SYMBOL_INT, u1->mIntValue = u1->mFloatValue = u2->mIntValue;
          mSymbolTable.push_back( u1 );

          u2 = GetToken( thisTokenIsNotFactor ); // to get ';'

          if ( u2->mType == SEMICOLON ) {
            u2->mType = REDEFINED, u2->mUnit = u1->mUnit;
            return u2;
          } // if()
          else if ( u2->mType != SEMICOLON ) { // error
            IgnoreThisLine();
            mSymbolTable.pop_back();

            if ( u2->mType == UNRECOGNIZED_TOKEN ) ;
            else u2->mType = UNEXPECTED_TOKEN;              
            return u2;
          } // if()
        } // if()
        else { // u2 error
          IgnoreThisLine();
          return u2;
        } // else
      } // else if()
    } // else if ()
    
    else if ( u1->mType == TYPE_FLOAT && // case 9, ReDefined to float
              ( u2->mType == SYMBOL_INT || u2->mType == SYMBOL_FLOAT || u2->mType == SYMBOL_CHAR ||
                u2->mType == SYMBOL_STRING || u2->mType == SYMBOL_BOOL ) ) {
      // int x ;
      // int x = 5;
      u1 = GetToken( thisTokenIsFactor );    // to get symbol
      u2 = GetToken( thisTokenIsNotFactor ); // to get ';' or '='

      if ( u2->mType == SEMICOLON ) {
        u1->mType = SYMBOL_INT, u1->mIntValue = u1->mFloatValue = 0;
        mSymbolTable.push_back( u1 );

        u2->mType = REDEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // if ()
      else if ( u2->mType == OPE_STATEMENT ) {
        u2 = Exp( NULL );

        if ( u2->mType == FLOAT || u2->mType == SYMBOL_FLOAT ) {
          u1->mType = SYMBOL_FLOAT, u1->mIntValue = 0, u1->mFloatValue = u2->mFloatValue;
          mSymbolTable.push_back( u1 );

          u2 = GetToken( thisTokenIsNotFactor ); // to get ';'

          if ( u2->mType == SEMICOLON ) {
            u2->mType = REDEFINED, u2->mUnit = u1->mUnit;
            return u2;
          } // if()
          else if ( u2->mType != SEMICOLON ) { // error
            IgnoreThisLine();
            mSymbolTable.pop_back();

            if ( u2->mType == UNRECOGNIZED_TOKEN ) ;
            else u2->mType = UNEXPECTED_TOKEN;              
            return u2;
          } // if()
        } // if()
        else { // u2 error
          IgnoreThisLine();
          return u2;
        } // else
      } // else if()
    } // else if ()
    
    else if ( u1->mType == TYPE_STRING && // case 9, ReDefined to string
              ( u2->mType == SYMBOL_INT || u2->mType == SYMBOL_FLOAT || u2->mType == SYMBOL_CHAR ||
                u2->mType == SYMBOL_STRING || u2->mType == SYMBOL_BOOL ) ) {
      // int x ;
      // int x = 5;
      u1 = GetToken( thisTokenIsFactor );    // to get symbol
      u2 = GetToken( thisTokenIsNotFactor ); // to get ';' or '='

      if ( u2->mType == SEMICOLON ) {
        u1->mType = SYMBOL_STRING, u1->mIntValue = u1->mFloatValue = 0;
        mSymbolTable.push_back( u1 );

        u2->mType = REDEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // if ()
      else if ( u2->mType == OPE_STATEMENT ) {
        AssignString(); // string s = "a" + "b" + "c" ; 
        GetToken( thisTokenIsNotFactor ); // to get ';'
        u2->mType = REDEFINED, u2->mUnit = u1->mUnit;
        return u2;
      } // else if()
    } // else if ()
    
    else if ( u1->mType == SYMBOL_STRING && u2->mType == OPE_STATEMENT ) { // case 2 : assign string
      u2 = GetToken( thisTokenIsNotFactor ); // to get '='
      u2 = AssignString(); // string s = "a" + "b" + "c" ;

      if ( u2 ) {
        IgnoreThisLine();
        return u2;
      } // if()
      else {
        GetToken( thisTokenIsNotFactor ); // to get ';' 
        return u1;
      } // else
    } // else if()
    
    else if ( u1->mType == SYMBOL_CHAR && u2->mType == OPE_STATEMENT ) { // case 2 : assign char
      u2 = GetToken( thisTokenIsNotFactor ); // to get =
      u2 = GetToken( thisTokenIsNotFactor ); // to get '
 
      char ch = cin.peek();

      while ( ch != 39 ) { // 39 = '
        cin.get();
        ch = cin.peek();
      } // while()

      if ( ch == 39 ) cin.get();

      GetToken( thisTokenIsNotFactor ); // to get ';' 
      return u1;
    } // else if()
    
    else if ( ( u1->mType == SYMBOL_UNKNOW || u1->mType == SYMBOL_INT || u1->mType == SYMBOL_FLOAT ) &&
              u2->mType == OPE_STATEMENT ) {  // case 2
      GetToken( thisTokenIsNotFactor ); // to get ':='

      if ( u1->mType == SYMBOL_UNKNOW ) { // case 2-1
        u2 = Exp( NULL );
        // cout << "In command, u2->mUnit = " << u2->mUnit << "\n";
        // cout << "In command, u2->mType = " << u2->mType << "\n";
        // cout << "In command, u2->mIntValue= " << u2->mIntValue << "\n";

        if ( u2->mType == INT || u2->mType == SYMBOL_INT ) {
          u1->mType = SYMBOL_INT, u1->mIntValue = u1->mFloatValue = u2->mIntValue;
          mSymbolTable.push_back( u1 );

          u2 = GetToken( thisTokenIsNotFactor ); // to get ';'
          if ( u2->mType != SEMICOLON ) { // error
            IgnoreThisLine();
            mSymbolTable.pop_back();
            if ( u2->mType == UNRECOGNIZED_TOKEN ) ;
            else u2->mType = UNEXPECTED_TOKEN;
              
            return u2;
          } // if()
        } // if()
        else if ( u2->mType == FLOAT || u2->mType == SYMBOL_FLOAT ) {
          u1->mType = SYMBOL_FLOAT, u1->mIntValue = 0, u1->mFloatValue = u2->mFloatValue;
          mSymbolTable.push_back( u1 );

          u2 = GetToken( thisTokenIsNotFactor ); // to get ';'
          if ( u2->mType != SEMICOLON ) { // error
            IgnoreThisLine();
            if ( u2->mType == UNRECOGNIZED_TOKEN ) ;
            else u2->mType = UNEXPECTED_TOKEN;
              
            return u2;
          } // if()
        } // else if()
        else { // u2 error
          IgnoreThisLine();
          return u2;
        } // else

        return u1;
      } // if()
      else if ( u1->mType == SYMBOL_INT || u1->mType == SYMBOL_FLOAT ) { // case 2-2
        // cout << "In command, u1->mUnit = " << u1->mUnit << "\n";
        // cout << "In command, u1->mType = " << u1->mType << "\n";
        // cout << "In command, u1->mIntValue= " << u1->mIntValue << "\n";
        u2 = Exp( NULL );
        // cout << "In command, u2->mUnit = " << u2->mUnit << "\n";
        // cout << "In command, u2->mType = " << u2->mType << "\n";
        // cout << "In command, u2->mIntValue= " << u2->mIntValue << "\n";
        if ( u2->mType == INT || u2->mType == SYMBOL_INT ) {
          u1->mType = SYMBOL_INT, u1->mIntValue = u1->mFloatValue = u2->mIntValue;
          // cout << "In command, u1->mUnit = " << u1->mUnit << "\n";
          // cout << "In command, u1->mType = " << u1->mType << "\n";
          // cout << "In command, u1->mIntValue= " << u1->mIntValue << "\n";
          u2 = GetToken( thisTokenIsNotFactor ); // to get ';'
          if ( u2->mType != SEMICOLON ) { // error
            IgnoreThisLine();

            if ( u2->mType == UNRECOGNIZED_TOKEN ) ;
            else u2->mType = UNEXPECTED_TOKEN;
              
            return u2;
          } // if()

          for ( int a = 0 ; a < mSymbolTable.size() ; a++ ) // can go there, means u2 is correct
            if ( mSymbolTable[a]->mUnit == u1->mUnit ) mSymbolTable[a] = u1, a = mSymbolTable.size();
        } // if()
        else if ( u2->mType == FLOAT || u2->mType == SYMBOL_FLOAT ) {
          u1->mType = SYMBOL_FLOAT, u1->mIntValue = 0, u1->mFloatValue = u2->mFloatValue;

          u2 = GetToken( thisTokenIsNotFactor ); // to get ';'
          if ( u2->mType != SEMICOLON ) { // error
            IgnoreThisLine();
            if ( u2->mType == UNRECOGNIZED_TOKEN ) ;
            else u2->mType = UNEXPECTED_TOKEN;
              
            return u2;
          } // if()

          for ( int a = 0 ; a < mSymbolTable.size() ; a++ ) // can go there, means u2 is correct
            if ( mSymbolTable[a]->mUnit == u1->mUnit ) mSymbolTable[a] = u1, a = mSymbolTable.size();
        } // else if()
        else { // u2 error
          IgnoreThisLine();
          return u2;
        } // else

        return u1;
      } // else if () 

    } // else if()

    else if ( u1->mType == SYMBOL_UNKNOW && // a + 3; a is an Undefined Symbol
              ( u2->mType == OPE_ADD || u2->mType == OPE_SUB || u2->mType == OPE_MUL || 
                u2->mType == OPE_DIV || u2->mType == OPE_COMPARE || u2->mType == SEMICOLON ) ) {
      u1->mType = UNDEFINED_SYMBOL;
      IgnoreThisLine();
      return u1;
    } // else if

    else if ( u1->mType == INT || u1->mType == FLOAT || u1->mType == L_PAREN || // case 3, case 4
              u1->mType == SYMBOL_INT || u1->mType == SYMBOL_FLOAT ) {

      u1 = Exp( u1 );
      // cout << "case 3, In command, u1->mUnit = " << u1->mUnit << "\n";
      // cout << "case 3, In command, u1->mIntValue = " << u1->mIntValue << "\n";
      // cout << "case 3, In command, u1->mType = " << u1->mType << "\n";

      if ( u1->mType == UNDEFINED_SYMBOL || u1->mType == UNRECOGNIZED_TOKEN || // u1 error
           u1->mType == UNEXPECTED_TOKEN ) {
        IgnoreThisLine();
        return u1;
      } // if()

      // u1 correct, go to peek u2
      u2 = PeekToken( thisTokenIsNotFactor );
      // cout << "In command, u2->mUnit = " << u2->mUnit << "\n";
      // cout << "In command, u2->mType = " << u2->mType << "\n";

      if ( u2->mType == SEMICOLON ) { // u2 is ';' -> case 4
        GetToken( thisTokenIsNotFactor ); // to get ;
        return u1; 
      } // if ()
      else if ( u2->mType == OPE_COMPARE ) { // u2 is '>', '>='....  -> case 3
        u1 = BoolExp( u1 );

        if ( u1->mType == BOOLEXP_ANS ) GetToken( thisTokenIsNotFactor ); // u1 correct, to get ;
        else IgnoreThisLine(); // u1 error

        return u1;
      } // else if ()
      else { // u2 error
        u2->mType = UNEXPECTED_TOKEN;
        IgnoreThisLine();
        return u2;
      } // else
    } // else if ()

    else { // u2 error, ex : a 3
      u2->mType = UNEXPECTED_TOKEN;
      IgnoreThisLine();
      return u2; 
    } // else 

    return NULL;
  } // Command()


  private : bool IsAlpha( char ch ) {
    if ( ( 'a' <= ch && ch <= 'z' ) || ( 'A' <= ch && ch <= 'Z' ) || ch == '_' ) return true;
    else return false;
  } // IsAlpha()


  private : bool IsDigit( char ch ) {
    if ( '0' <= ch && ch <= '9' ) return true;
    else return false;
  } // IsDigit()


  private : void SkipWhiteSpace() {
    char ch = cin.peek();

    while ( ch == ' ' || ch == '\t' || ch == '\n' ) { // white space
      if ( ch == '\n' ) {
        mErrorLine++;
      } // if()

      cin.get();
      ch = cin.peek();
    } // while()
  } // SkipWhiteSpace()


  private : void IgnoreThisLine() {
    char ch = cin.peek();

    while ( ch != '\n' ) { // white space
      cin.get();
      ch = cin.peek();
    } // while()

    cin.get();
  } // IgnoreThisLine()

}; // class Formula


int main() {
  //cin.ignore(); // 弄肈腹
  //cin.ignore();
  cout << "Our-C running ...\n";

  Formula* f = new Formula();
  Unit* u;
 
  do {
    u = f->Command();

    if ( u->mType == INT || u->mType == SYMBOL_INT ) 
      cout << "> Statement executed ...\n";
    else if ( u->mType == FLOAT || u->mType == SYMBOL_FLOAT ) 
      cout << "> Statement executed ...\n";
    else if ( u->mType == SYMBOL_STRING ) 
      cout << "> Statement executed ...\n";
    else if ( u->mType == SYMBOL_CHAR ) 
      cout << "> Statement executed ...\n";
    else if ( u->mType == R_BIG_PAREN ) 
      cout << "> Statement executed ...\n";
    else if ( u->mType == BOOLEXP_ANS ) // true or false
      cout << "> " << u->mUnit << "\n";
    else if ( u->mType == UNEXPECTED_TOKEN ) 
      cout << "> Line " << f->mErrorLine << " : unexpected token : '" << u->mUnit << "'\n";
    else if ( u->mType == UNRECOGNIZED_TOKEN ) 
      cout << "> Unrecognized token with first char : '" << u->mUnit << "'\n";
    else if ( u->mType == UNDEFINED_SYMBOL || u->mType == SYMBOL_UNKNOW )
      cout << "> Line " << f->mErrorLine << " : undefined identifier : '" << u->mUnit << "'\n";
    else if ( u->mType == DEFINED )
      cout << "> Definition of " << u->mUnit << " entered ...\n";
    else if ( u->mType == REDEFINED )
      cout << "> New definition of " << u->mUnit << " entered ...\n";
    else if ( u->mType == CIN || u->mType == COUT )
      cout << "> Statement executed ...\n";
    else if ( u->mType == SEMICOLON )
      cout << "> Statement executed ...\n";
    else if ( u->mType == DONE ) 
      ;
    else
      cout << "Error in main()\n";

  } while ( u->mType != DONE );

  cout << "> Our-C exited ...";
} // main()
