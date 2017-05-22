# include <cstdlib>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <iostream>
# include <vector>
using namespace std ;

enum TokenType { IDENTIFIER = 34512, CONSTANT = 87232, SPECIAL = 29742, FLOAT = 48763,
                 NONIDENTIFIER = 29859, ERROR = 18612
} ;

typedef char * CharPtr ;
/*
struct Column {
  int column ;                  // 此token(的字首)在本line有出現於此column
  Column * next;                // 下一個此token(在本line)有出現的column
} ; // Column

typedef Column * ColumnPtr ;

struct Line {                   // struct Line 記錄了4種資訊
  int line ;                    // 此token放在哪一 line
  ColumnPtr first ;   // 指到此token第一次出現在此line的哪一column
  ColumnPtr last ;     // 指到此token最後出現在此line的哪一column
  Line * next;                 // 同一 token 下一次出現在哪一 line
} ;

typedef Line * LinePtr ;
*/
struct Token {                  // Struct token 紀錄了4種資訊
  CharPtr tokenStr ;            // tokenStr 放你切下來的token
  TokenType type ;               // type 紀錄此token是屬於哪一個case
  // LinePtr first ;       // 此 token 第一次出現在哪一 line
  // LinePtr last ;        // 此 token 最後出現在哪一 line
  Token * next ;                // 下一個token
} ; // struct Token

typedef Token * TokenPtr ;

typedef char Str100[ 100 ] ;
// TokenPtr gFront = NULL, gRear = NULL ;
int gLine = 0;              // 「下一個要讀進來的字元」所在的line number
int gColumn = 0 ;            // 「下一個要讀進來的字元」所在的column number
TokenPtr gCurrentToken = NULL ;

struct ID {
  Str100 name ;
  Str100 type ;
  Str100 length ;
  float value ;
  bool isfloat ;
} ; // struct ID

vector<ID> gIDtable ;
vector<ID> gDefiningIDtable ;
vector<TokenPtr> gFunctiontable ;
TokenPtr gDefiningFunction = NULL ;
TokenPtr gDefiningFunctionwalk = NULL ;
Str100 gFunctionname = "" ;

void AddID( ID definingID ) ;
void AddIDToDefining( ID definingID ) ;
void AddDefiningToIDtable() ;
void EraseDefiningID() ;
bool SearchID( Str100 tokenStr, float& value, bool& isfloat ) ;
bool SearchFunctionname( Str100 tokenStr ) ;
void RunFunction() ;
void ListAllVariables() ;
void ListVariable() ;
void ListAllFunctions() ;
void ListFunction() ;

// ---------------------------------input--------------------------------------

bool IsWhiteSpace( char test ) ;
bool IsDigit( char test ) ;
bool IsLetter( char test ) ;
bool IsSpecial( char test ) ;
bool GetChar( char& test ) ;
TokenPtr GetToken( char& test ) ;
TokenPtr PeekFirstToken( char& test ) ;
TokenPtr PeekCurrentToken( char& test ) ;
TokenPtr GetCurrentToken( char& test ) ;
/*
bool Command( char& test ) ;
bool IDlessArithExpOrBexp( char& test, float& value, bool& isfloat, bool& ifbexp, bool& bexp ) ;
bool BooleanOprator( char& test, float& value, bool& ifbexp, char bop[10] ) ;
bool NOT_ID_StartArithExpOrBexp( char& test, float& value, bool& isfloat, bool& ifbexp, bool& bexp ) ;
bool NOT_ID_StartArithExp( char& test, float& value, bool& isfloat ) ;
bool NOT_ID_StartTerm( char& test, float& value, bool& isfloat ) ;
bool NOT_ID_StartFactor( char& test, float& value, bool& isfloat ) ;
bool ArithExp( char& test, float& value, bool& isfloat ) ;
bool Term( char& test, float& value, bool& isfloat ) ;
bool Factor( char& test, float& value, bool& isfloat ) ;
*/

bool User_input( char& test ) ;
bool Definition( char& test ) ;
bool Type_specifier( char& test ) ;
bool Function_definition_or_declarators( char& test, ID definingID ) ;
bool Rest_of_declarators( char& test, ID definingID ) ;
bool Function_definition_without_ID( char& test ) ;
bool Formal_parameter_list( char& test ) ;
bool Compound_statement( char& test ) ;
bool Declaration( char& test ) ;
bool Statement( char& test ) ;
bool Expression( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                 bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Basic_expression( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                       bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_Identifier_started_basic_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                           bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_PPMM_Identifier_started_basic_exp( char& test, bool& isfloat, float& floatvalue,
                                                Str100& stringvalue, bool& boolvalue, bool& iscout,
                                                bool& isbexp, bool& isexp ) ;
bool Sign( char& test ) ;
bool Actual_parameter_list( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                            bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Assignment_operator( char& test ) ;
bool Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( char& test, bool& isfloat,
                                                                     float& floatvalue, Str100& stringvalue,
                                                                     bool& boolvalue, bool& iscout,
                                                                     bool& isbexp, bool& isexp ) ;
bool Rest_of_maybe_logical_OR_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                   bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Maybe_logical_AND_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                            bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_maybe_logical_AND_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                    bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Maybe_bit_OR_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                       bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_maybe_bit_OR_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                               bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Maybe_bit_ex_OR_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                          bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_maybe_bit_ex_OR_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                  bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Maybe_bit_AND_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                        bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_maybe_bit_AND_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Maybe_equality_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                         bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_maybe_equality_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                 bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Maybe_relational_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                           bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_maybe_relational_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                   bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Maybe_shift_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                      bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_maybe_shift_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                              bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Maybe_additive_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                         bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_maybe_additive_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                 bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Maybe_mult_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                     bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Rest_of_maybe_mult_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                             bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Unary_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Signed_unary_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                       bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;
bool Unsigned_unary_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                         bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) ;

int main() {
  int uTestNum = 0 ;
  scanf( "%d", &uTestNum ) ;
  if ( uTestNum == 3 )
    printf( "3\n" ) ;
  char test = '\0' ;
  GetChar( test ) ;
  printf( "Our-C running ...\n" ) ;

  while ( 1 ) {
    User_input( test ) ;

  } // while

  // system("pause") ;

} // main()

// ---------------------------------input--------------------------------------

bool IsWhiteSpace( char test ) {
  if ( test == '\t' || test == '\n' || test == ' ' || test == '\0' ) {
    return true;
  } // if
  else
    return false ;
} // IsWhiteSpace()

bool IsDigit( char test ) {
  if ( test == '0' || test == '1' || test == '2' || test == '3' || test == '4' ||
       test == '5' || test == '6' || test == '7' || test == '8' || test == '9' )
    return true ;
  else
    return false ;
} // IsDigit()

bool IsLetter( char test ) {
  if ( ( test >= 'a' && test <= 'z' ) || ( test >= 'A' && test <= 'Z' ) )
    return true ;
  else
    return false ;
} // IsLetter()

bool IsSpecial( char test ) {
  if ( test == '+' || test == '-' || test == '*' || test == '/' || test == '>' ||
       test == '<' || test == '=' || test == '?' || test == '%' || test == '&' ||
       test == '|' || test == '^' || test == '\"' || test == '\'' || test == '.' ||
       test == ',' || test == '(' || test == ')' || test == '[' || test == ']' ||
       test == '{' || test == '}' || test == '!' || test == ':' || test == ';' ||
       test == '#' || test == '_' )
    return true ;
  else
    return false ;
} // IsSpecial()

bool GetChar( char& test ) {
  scanf( "%c", &test ) ;
  if ( test == '\n' ) {
    gLine++ ;
    gColumn = 0 ;
    return true ;
  } // if
  else {
    gColumn++ ;
    return true ;
  } // else

  return false ;
} // GetChar()
/*
TokenPtr GetToken( char& test ) {
  TokenPtr token1 = new Token ;
  token1->tokenStr = new Str100 ;
  strcpy( token1->tokenStr, "" ) ;
  int i = 0 ;
  char testnext = '\0' ;
  while ( IsWhiteSpace( test ) ) {
    GetChar( test ) ;
  } // while

  if ( IsDigit( test ) ) { // -------------------------------------------
    token1->type = CONSTANT ;
    token1->tokenStr[i] = test ;
    i++ ;
    GetChar( testnext ) ;
    while ( IsDigit( testnext ) ) {
      token1->tokenStr[i] = testnext ;
      i++ ;
      GetChar( testnext ) ;
    } // while

    if ( testnext == '.' ) {
      token1->type = FLOAT ;
      token1->tokenStr[i] = testnext ;
      i++ ;
      GetChar( testnext ) ;
      while ( IsDigit( testnext ) ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        GetChar( testnext ) ;
      } // while
    } // if

    token1->tokenStr[i] = '\0' ;
    test = testnext ;
  } // if
  else if ( IsLetter( test ) ) { // -------------------------------------------
    token1->type = IDENTIFIER ;
    token1->tokenStr[i] = test ;
    i++ ;
    GetChar( testnext ) ;
    while ( IsLetter( testnext ) || IsDigit( testnext ) || testnext == '_' ) {
      token1->tokenStr[i] = testnext ;
      i++ ;
      GetChar( testnext ) ;
    } // while

    token1->tokenStr[i] = '\0' ;
    test = testnext ;
  } // else if
  else if ( IsSpecial( test ) ) { // -------------------------------------------
    if ( test == '_' ) {
      token1->type = IDENTIFIER ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( !IsLetter( testnext ) && !IsDigit( testnext ) ) {
        printf( "> Unrecognized token with first char : '%c'\n", test ) ;
        test = testnext ;
        strcpy( token1->tokenStr, "" ) ;
      } // if

      while ( IsLetter( testnext ) || IsDigit( testnext ) ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        GetChar( testnext ) ;
      } // while

      token1->tokenStr[i] = '\0' ;

      test = testnext ;
    } // if
    else if ( test == ':' ) {
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '=' ) {
        token1->type = SPECIAL ;
        token1->tokenStr[i] = testnext  ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        printf( "> Unrecognized token with first char : '%c'\n", test ) ;
        test = testnext ;
        strcpy( token1->tokenStr, "" ) ;
      } // else
    } // else if
    else if ( test == '/' ) {
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '/' ) {
        while ( testnext != '\n' ) GetChar( testnext ) ;
        test = '\0' ;
        return NULL ;
      } // if
      else if ( testnext == '*' ) {
        for ( int j = 0 ; j != -1 ; ) {
          GetChar( testnext ) ;
          if ( testnext == '*' ) {
            GetChar( testnext ) ;
            if ( testnext == '/' )
              j = -1 ;
          } // if
        } // for

        test = '\0' ;
        return NULL ;
      } // else if
      else {
        token1->type = SPECIAL ;
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '.' ) {
      token1->type = FLOAT ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( !IsDigit( testnext ) ) {
        printf( "> Unrecognized token with first char : '%c'\n", test ) ;
        test = testnext ;
        strcpy( token1->tokenStr, "" ) ;
      } // if

      while ( IsDigit( testnext ) ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        GetChar( testnext ) ;
      } // while

      token1->tokenStr[i] = '\0' ;
      test = testnext ;
    } // else if
    else if ( test == '>' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '=' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '<' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '>' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else if ( testnext == '=' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // else if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '+' || test == '-' || test == '*' || test == '=' ||
              test == '(' || test == ')' || test == ';' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      token1->tokenStr[i] = '\0' ;
      test = '\0' ;
    } // else if
    else {
      printf( "> Unrecognized token with first char : '%c'\n", test ) ;
      test = '\0' ;
      strcpy( token1->tokenStr, "" ) ;
    } // else
  } // else if
  else {
    printf( "> Unrecognized token with first char : '%c'\n", test ) ;
    test = '\0' ;
    strcpy( token1->tokenStr, "" ) ;
  } // else

  return token1 ;
} // GetToken()
*/
TokenPtr GetToken( char& test ) {
  TokenPtr token1 = new Token ;
  token1->tokenStr = new Str100 ;
  strcpy( token1->tokenStr, "" ) ;
  int i = 0 ;
  char testnext = '\0' ;
  while ( IsWhiteSpace( test ) ) {
    GetChar( test ) ;
  } // while

  if ( IsDigit( test ) ) { // -------------------------------------------
    token1->type = CONSTANT ;
    token1->tokenStr[i] = test ;
    i++ ;
    GetChar( testnext ) ;
    while ( IsDigit( testnext ) ) {
      token1->tokenStr[i] = testnext ;
      i++ ;
      GetChar( testnext ) ;
    } // while

    if ( testnext == '.' ) {
      token1->type = CONSTANT ;
      token1->tokenStr[i] = testnext ;
      i++ ;
      GetChar( testnext ) ;
      while ( IsDigit( testnext ) ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        GetChar( testnext ) ;
      } // while
    } // if

    token1->tokenStr[i] = '\0' ;
    test = testnext ;
  } // if
  else if ( IsLetter( test ) ) { // -------------------------------------------
    token1->type = IDENTIFIER ;

    token1->tokenStr[i] = test ;
    i++ ;
    GetChar( testnext ) ;
    while ( IsLetter( testnext ) || IsDigit( testnext ) || testnext == '_' ) {
      token1->tokenStr[i] = testnext ;
      i++ ;
      GetChar( testnext ) ;
    } // while

    token1->tokenStr[i] = '\0' ;

    if ( strcmp( token1->tokenStr, "int" ) == 0 ||
         strcmp( token1->tokenStr, "float" ) == 0 ||
         strcmp( token1->tokenStr, "char" ) == 0 ||
         strcmp( token1->tokenStr, "bool" ) == 0 ||
         strcmp( token1->tokenStr, "string" ) == 0 ||
         strcmp( token1->tokenStr, "void" ) == 0 ||
         strcmp( token1->tokenStr, "if" ) == 0 ||
         strcmp( token1->tokenStr, "else" ) == 0 ||
         strcmp( token1->tokenStr, "while" ) == 0 ||
         strcmp( token1->tokenStr, "do" ) == 0 ||
         strcmp( token1->tokenStr, "return" ) == 0 ) {
      token1->type = NONIDENTIFIER ;
    } // if
    else if ( strcmp( token1->tokenStr, "true" ) == 0 ||
              strcmp( token1->tokenStr, "false" ) == 0 ) {
      token1->type = CONSTANT ;
    } // else if
    /*
    else if ( strcmp( token1->tokenStr, "Done" ) == 0 ) {
      while ( IsWhiteSpace( testnext ) ) {
        GetChar( testnext ) ;
      } // while

      if ( testnext == '(' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        GetChar( testnext ) ;
        while ( IsWhiteSpace( testnext ) ) {
          GetChar( testnext ) ;
        } // while

        if ( testnext == ')' ) {
          token1->tokenStr[i] = testnext ;
          i++ ;
          GetChar( testnext ) ;
          while ( IsWhiteSpace( testnext ) ) {
            GetChar( testnext ) ;
          } // while

          if ( testnext == ';' ) {
            token1->tokenStr[i] = testnext ;
            i++ ;
            token1->tokenStr[i] = '\0' ;
            printf( "Our-C exited ...\n" ) ;
            exit( 0 ) ;
          } // if
          else {
            printf( "Unrecognized token with first char : '%c'\n", test ) ;
            while ( testnext != '\n' )
              GetChar( testnext ) ;
            test = testnext ;
            strcpy( token1->tokenStr, "" ) ;
          } // else
        } // if
        else {
          token1->type = IDENTIFIER ;
          test = testnext ;
        } // else
      } // if
      else
        token1->type = IDENTIFIER ;
    } // else if
    */
    else
      token1->type = IDENTIFIER ;

    test = testnext ;
  } // else if
  else if ( IsSpecial( test ) ) { // -------------------------------------------
    if ( test == '/' ) {
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '/' ) {
        while ( testnext != '\n' ) GetChar( testnext ) ;
        test = testnext ;
        return NULL ;
      } // if
      else if ( testnext == '*' ) {
        for ( int j = 0 ; j != -1 ; ) {
          GetChar( testnext ) ;
          if ( testnext == '*' ) {
            GetChar( testnext ) ;
            if ( testnext == '/' )
              j = -1 ;
          } // if
        } // for

        GetChar( testnext ) ;
        test = testnext ;
        return NULL ;
      } // else if
      else if ( testnext == '=' ) {
        token1->type = SPECIAL ;
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // else if
      else {
        token1->type = SPECIAL ;
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // if
    else if ( test == '_' ) {
      token1->type = IDENTIFIER ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( !IsLetter( testnext ) && !IsDigit( testnext ) ) {
        printf( "Unrecognized token with first char : '%c'\n", test ) ;
        while ( testnext != '\n' )
          GetChar( testnext ) ;
        test = testnext ;
        token1->type = ERROR ;
      } // if

      while ( IsLetter( testnext ) || IsDigit( testnext ) ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        GetChar( testnext ) ;
      } // while

      token1->tokenStr[i] = '\0' ;

      test = testnext ;
    } // else if
    else if ( test == '\'' ) {
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '\'' || testnext == '\"' || testnext == '\n' ) {
        printf( "Unrecognized token with first char : '%c'\n", test ) ;
        while ( testnext != '\n' )
          GetChar( testnext ) ;
        test = testnext ;
        token1->type = ERROR ;
      } // if
      else {
        token1->tokenStr[i] = testnext ;
        i++ ;
        GetChar( testnext ) ;
        if ( testnext != '\'' ) {
          printf( "Unrecognized token with first char : '%c'\n", test ) ;
          while ( testnext != '\n' )
            GetChar( testnext ) ;
          test = testnext ;
          token1->type = ERROR ;
        } // if
        else {
          token1->type = CONSTANT ;
          token1->tokenStr[i] = testnext ;
          i++ ;
          token1->tokenStr[i] = '\0' ;
          test = '\0' ;
        } // else
      } // else
    } // else if
    else if ( test == '\"' ) {
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      while ( testnext != '\"' && testnext != '\n' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        GetChar( testnext ) ;
      } // while

      if ( testnext == '\n' ) {
        printf( "Unrecognized token with first char : '%c'\n", test ) ;
        while ( testnext != '\n' )
          GetChar( testnext ) ;
        test = testnext ;
        token1->type = ERROR ;
      } // if
      else {
        token1->type = CONSTANT ;
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // else
    } // else if
    else if ( test == '.' ) {
      token1->type = CONSTANT ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( !IsDigit( testnext ) ) {
        printf( "Unrecognized token with first char : '%c'\n", test ) ;
        while ( testnext != '\n' )
          GetChar( testnext ) ;
        test = testnext ;
        token1->type = ERROR ;
      } // if

      while ( IsDigit( testnext ) ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        GetChar( testnext ) ;
      } // while

      token1->tokenStr[i] = '\0' ;
      test = testnext ;
    } // else if
    else if ( test == '+' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '+' || testnext == '=' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '-' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '-' || testnext == '=' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '*' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '=' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '%' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '=' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '^' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '=' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '>' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '=' || testnext == '>' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '<' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '=' || testnext == '<' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '=' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '=' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '!' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '=' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '&' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '&' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '|' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      GetChar( testnext ) ;
      if ( testnext == '|' ) {
        token1->tokenStr[i] = testnext ;
        i++ ;
        token1->tokenStr[i] = '\0' ;
        test = '\0' ;
      } // if
      else {
        token1->tokenStr[i] = '\0' ;
        test = testnext ;
      } // else
    } // else if
    else if ( test == '(' || test == ')' || test == '[' || test == ']' ||
              test == '{' || test == '}' || test == ';' || test == ',' ||
              test == '?' || test == ':' ) {
      token1->type = SPECIAL ;
      token1->tokenStr[i] = test ;
      i++ ;
      token1->tokenStr[i] = '\0' ;
      test = '\0' ;
    } // else if
    else {
      printf( "Unrecognized token with first char : '%c'\n", test ) ;
      while ( testnext != '\n' )
        GetChar( testnext ) ;
      test = testnext ;
      token1->type = ERROR ;
    } // else
  } // else if
  else {
    printf( "Unrecognized token with first char : '%c'\n", test ) ;
    while ( testnext != '\n' )
      GetChar( testnext ) ;
    test = testnext ;
    token1->type = ERROR ;
  } // else

  // printf( "%s\n", token1->tokenStr ) ;
  return token1 ;
} // GetToken()

TokenPtr PeekFirstToken( char& test ) {
  if ( gCurrentToken == NULL ) {
    gCurrentToken = GetToken( test ) ;
    return gCurrentToken ;
  } // if
  else
    return gCurrentToken ;
} // PeekFirstToken()

TokenPtr PeekCurrentToken( char& test ) {
  if ( gCurrentToken == NULL ) {
    gCurrentToken = GetToken( test ) ;
    while ( gCurrentToken == NULL )
      gCurrentToken = GetToken( test ) ;
    return gCurrentToken ;
  } // if
  else
    return gCurrentToken ;
} // PeekCurrentToken()

TokenPtr GetCurrentToken( char& test ) {
  TokenPtr getCurrentToken = new Token ;
  getCurrentToken->tokenStr = new Str100 ;
  if ( gCurrentToken == NULL ) {
    gCurrentToken = GetToken( test ) ;
    while ( gCurrentToken == NULL )
      gCurrentToken = GetToken( test ) ;
  } // if

  strcpy( getCurrentToken->tokenStr, gCurrentToken->tokenStr ) ;
  getCurrentToken->type = gCurrentToken->type ;
  delete gCurrentToken ;
  gCurrentToken = NULL ;
  if ( gDefiningFunctionwalk == NULL ) {
    gDefiningFunction = getCurrentToken ;
    gDefiningFunctionwalk = gDefiningFunction ;
  } // if
  else {
    gDefiningFunctionwalk->next = getCurrentToken ;
    gDefiningFunctionwalk = gDefiningFunctionwalk->next ;
  } // else

  return getCurrentToken ;
} // GetCurrentToken()

void AddID( ID definingID ) {
  for ( vector<ID>::iterator it = gIDtable.begin() ; it != gIDtable.end() ; it++ ) {
    if ( strcmp( it->name, definingID.name ) == 0 ) {
      strcpy( it->type, definingID.type ) ;
      strcpy( it->length, definingID.length ) ;
      it->value = definingID.value ;
      it->isfloat = definingID.isfloat ;
      printf( "New definition of %s entered ...\n", definingID.name ) ;
      return ;
    } // if
  } // for

  if ( gIDtable.empty() ) {
    gIDtable.push_back( definingID ) ;
    printf( "Definition of %s entered ...\n", definingID.name ) ;
    return ;
  } // if
  else {
    for ( vector<ID>::iterator it = gIDtable.begin() ; it != gIDtable.end() ; it++ ) {
      if ( strcmp( it->name, definingID.name ) > 0 ) {
        gIDtable.insert( it, definingID ) ;
        printf( "Definition of %s entered ...\n", definingID.name ) ;
        return ;
      } // if

    } // for
  } // else

  gIDtable.push_back( definingID ) ;
  printf( "Definition of %s entered ...\n", definingID.name ) ;
} // AddID()

void AddIDToDefining( ID definingID ) {
  for ( vector<ID>::iterator it = gDefiningIDtable.begin() ; it != gDefiningIDtable.end() ; it++ ) {
    if ( strcmp( it->name, definingID.name ) == 0 && strcmp( it->type, definingID.type ) == 0 ) {
      it->value = definingID.value ;
      it->isfloat = definingID.isfloat ;
      return ;
    } // if
  } // for

  gDefiningIDtable.push_back( definingID ) ;
} // AddIDToDefining()

void AddDefiningToIDtable() {
  for ( int i = 0 ; i < ( int ) gDefiningIDtable.size() ; i++ ) {
    AddID( gDefiningIDtable[i] ) ;
  } // for

  gDefiningIDtable.erase( gDefiningIDtable.begin(), gDefiningIDtable.end() ) ;
} // AddDefiningToIDtable()

void EraseDefiningID() {
  gDefiningIDtable.erase( gDefiningIDtable.begin(), gDefiningIDtable.end() ) ;
} // EraseDefiningID()

bool SearchID( Str100 tokenStr, float& value, bool& isfloat ) {
  for ( vector<ID>::iterator it = gIDtable.begin() ; it != gIDtable.end() ; it++ ) {
    if ( strcmp( it->name, tokenStr ) == 0 ) {
      value = it->value ;
      if ( it->isfloat )
        isfloat = true ;
      return true ;
    } // if
  } // for

  for ( vector<ID>::iterator it = gDefiningIDtable.begin() ; it != gDefiningIDtable.end() ; it++ ) {
    if ( strcmp( it->name, tokenStr ) == 0 ) {
      value = it->value ;
      if ( it->isfloat )
        isfloat = true ;
      return true ;
    } // if
  } // for

  return false ;
} // SearchID()

bool SearchFunctionname( Str100 tokenStr ) {
  if ( strcmp( "ListAllVariables", tokenStr ) == 0 ||
       strcmp( "ListVariable", tokenStr ) == 0 ||
       strcmp( "ListAllFunctions", tokenStr ) == 0 ||
       strcmp( "ListFunction", tokenStr ) == 0 ||
       strcmp( "Done", tokenStr ) == 0 )
    return true ;

  for ( int i = 0 ; i < ( int ) gFunctiontable.size() ; i++ ) {
    if ( strcmp( gFunctiontable[i]->next->tokenStr, tokenStr ) == 0 ) {
      return true ;
    } // if
  } // for

  return false ;
} // SearchFunctionname()

void AddDefiningToFunctiontable() {
  for ( int i = 0 ; i < ( int ) gFunctiontable.size() ; i++ ) {
    if ( strcmp( gFunctiontable[i]->next->tokenStr, gDefiningFunction->next->tokenStr ) == 0 &&
         strcmp( gFunctiontable[i]->tokenStr, gDefiningFunction->tokenStr ) == 0 ) {

      delete [] gFunctiontable[i] ;
      gFunctiontable[i] = gDefiningFunction ;
      printf( "New definition of %s() entered ...\n", gDefiningFunction->next->tokenStr ) ;
      gDefiningFunction = NULL ;
      return ;
    } // if
  } // for

  if ( gFunctiontable.empty() ) {
    gFunctiontable.push_back( gDefiningFunction ) ;
    printf( "Definition of %s() entered ...\n", gDefiningFunction->next->tokenStr ) ;
    gDefiningFunction = NULL ;
    return ;
  } // if
  else {
    for ( int i = 0 ; i < ( int ) gFunctiontable.size() ; i++ ) {
      if ( strcmp( gFunctiontable[i]->next->tokenStr, gDefiningFunction->next->tokenStr ) > 0 ) {
        gFunctiontable.insert( gFunctiontable.begin() + i, gDefiningFunction ) ;
        printf( "Definition of %s() entered ...\n", gDefiningFunction->next->tokenStr ) ;
        gDefiningFunction = NULL ;
        return ;
      } // if

    } // for
  } // else

  gFunctiontable.push_back( gDefiningFunction ) ;
  printf( "Definition of %s() entered ...\n", gDefiningFunction->next->tokenStr ) ;
  gDefiningFunction = NULL ;
} // AddDefiningToFunctiontable()

void RunFunction() {
  if ( strcmp( "ListAllVariables", gFunctionname ) == 0 ) {
    ListAllVariables() ;
  } // if
  else if ( strcmp( "ListVariable", gFunctionname ) == 0 ) {
    ListVariable() ;
  } // else if
  else if ( strcmp( "ListAllFunctions", gFunctionname ) == 0 ) {
    ListAllFunctions() ;
  } // else if
  else if ( strcmp( "ListFunction", gFunctionname ) == 0 ) {
    ListFunction() ;
  } // else if
  else if ( strcmp( "Done", gFunctionname ) == 0 ) {
    printf( "Our-C exited ...\n" ) ;
    exit( 0 ) ;
  } // else if
  else {

  } // else
} // RunFunction()


void ListAllVariables() {
  for ( vector<ID>::iterator it = gIDtable.begin() ; it != gIDtable.end() ; it++ ) {
    printf( "%s\n", it->name ) ;
  } // for
} // ListAllVariables()

void ListVariable() {
  Str100 nametoString = "" ;
  for ( vector<ID>::iterator it = gIDtable.begin() ; it != gIDtable.end() ; it++ ) {
    strcat( nametoString, "\"" ) ;
    strcat( nametoString, it->name ) ;
    strcat( nametoString, "\"" ) ;
    if ( strcmp( gDefiningFunction->next->next->tokenStr, nametoString ) == 0 ) {
      if ( strcmp( it->length, "" ) != 0 )
        printf( "%s %s[ %s ] ;\n", it->type, it->name, it->length ) ;
      else
        printf( "%s %s ;\n", it->type, it->name ) ;
    } // if

    strcpy( nametoString, "" ) ;
  } // for
} // ListVariable()

void ListAllFunctions() {
  for ( int i = 0 ; i < ( int ) gFunctiontable.size() ; i++ ) {
    printf( "%s()\n", gFunctiontable[i]->next->tokenStr ) ;
  } // for
} // ListAllFunctions()

void ListFunction() {
  Str100 nametoString = "" ;
  for ( int i = 0 ; i < ( int ) gFunctiontable.size() ; i++ ) {
    strcat( nametoString, "\"" ) ;
    strcat( nametoString, gFunctiontable[i]->next->tokenStr ) ;
    strcat( nametoString, "\"" ) ;
    if ( strcmp( gDefiningFunction->next->next->tokenStr, nametoString ) == 0 ) {
      int numofwhitespace = 0 ;
      TokenPtr walk = gFunctiontable[i] ;
      while ( walk != NULL ) {
        if ( strcmp( walk->tokenStr, ";" ) == 0 ) {
          printf( "%s\n", walk->tokenStr ) ;
          for ( int i = 0 ; i < numofwhitespace ; i++ )
            printf( " " ) ;
        } // if
        else if ( strcmp( walk->tokenStr, "{" ) == 0 ) {
          printf( "%s\n", walk->tokenStr ) ;
          numofwhitespace += 2 ;
          for ( int i = 0 ; i < numofwhitespace ; i++ )
            printf( " " ) ;
        } // else if
        else if ( strcmp( walk->tokenStr, "}" ) == 0 ) {
          numofwhitespace -= 2 ;
          for ( int i = 0 ; i < numofwhitespace ; i++ )
            printf( " " ) ;
          printf( "%s\n", walk->tokenStr ) ;
        } // else if
        else
          printf( "%s ", walk->tokenStr ) ;

        walk = walk->next ;
      } // while

      return ;
    } // if

    strcpy( nametoString, "" ) ;
  } // for
} // ListFunction()

/*
bool Command( char& test ) {
  float value = 0 ;
  bool ifbexp = false ;
  bool bexp = false ;
  bool isfloat = false ;
  int checkvalue = 0 ;
  if ( PeekCurrentToken( test )->type == IDENTIFIER &&
       strcmp( PeekCurrentToken( test )->tokenStr, "quit" ) == 0 ) {
    printf( "> Program exits..." ) ;
    exit( 0 ) ;
  } // if
  else if ( PeekCurrentToken( test )->type == IDENTIFIER ) {
    ID definingID ;
    strcpy( definingID.name, GetCurrentToken( test )->tokenStr ) ;
    if ( PeekCurrentToken( test )->type != SPECIAL ) {
      printf( "> Unexpected token : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
      return false ;
    } // if
    else if ( !SearchID( definingID.name, value, isfloat ) &&
              strcmp( PeekCurrentToken( test )->tokenStr, ":=" ) != 0 ) {
      GetCurrentToken( test ) ;
      printf( "> Undefined identifier : '%s'\n", definingID.name ) ;
      return false ;
    } // else if
    else {
      checkvalue = value ;
      if ( checkvalue == value ) {
      } // if
      else
        isfloat = true ;
      if ( strcmp( PeekCurrentToken( test )->tokenStr, ":=" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( ArithExp( test, value, isfloat ) ) {
          if ( strcmp( PeekCurrentToken( test )->tokenStr, ";" ) == 0 ) {
            GetCurrentToken( test ) ;
            definingID.value = value ;
            definingID.isfloat = isfloat ;
            AddID( definingID ) ;
            if ( !isfloat ) {
              checkvalue = value ;
              if ( checkvalue == value )
                printf( "> %d\n", checkvalue ) ;
              else
                printf( "> %.3f\n", value ) ;
            } // if
            else
              printf( "> %.3f\n", value ) ;
            return true ;
          } // if
          else {
            if ( strcmp( PeekCurrentToken( test )->tokenStr, "" ) == 0 )
              GetCurrentToken( test ) ;
            else if ( PeekCurrentToken( test )->type == IDENTIFIER )
              printf( "> Undefined identifier : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
            else
              printf( "> Unexpected token : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
            return false ;
          } // else
        } // if
        else {
          if ( strcmp( PeekCurrentToken( test )->tokenStr, "" ) == 0 )
            GetCurrentToken( test ) ;
          else if ( PeekCurrentToken( test )->type == IDENTIFIER )
            printf( "> Undefined identifier : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
          else
            printf( "> Unexpected token : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
          return false ;
        } // else
      } // if
      else if ( IDlessArithExpOrBexp( test, value, isfloat, ifbexp, bexp ) ) {
        if ( strcmp( PeekCurrentToken( test )->tokenStr, ";" ) == 0 ) {
          GetCurrentToken( test ) ;
          if ( !ifbexp ) {
            if ( !isfloat ) {
              checkvalue = value ;
              if ( checkvalue == value )
                printf( "> %d\n", checkvalue ) ;
              else
                printf( "> %.3f\n", value ) ;
            } // if
            else
              printf( "> %.3f\n", value ) ;
          } // if
          else {
            if ( bexp )
              printf( "> true\n" ) ;
            else
              printf( "> false\n" ) ;
          } // else

          return true ;
        } // else if
        else {
          if ( strcmp( PeekCurrentToken( test )->tokenStr, "" ) == 0 )
            GetCurrentToken( test ) ;
          else if ( PeekCurrentToken( test )->type == IDENTIFIER )
            printf( "> Undefined identifier : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
          else
            printf( "> Unexpected token : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
          return false ;
        } // else
      } // else if
      else {
        if ( strcmp( PeekCurrentToken( test )->tokenStr, "" ) == 0 )
          GetCurrentToken( test ) ;
        else if ( PeekCurrentToken( test )->type == IDENTIFIER )
          printf( "> Undefined identifier : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
        else
          printf( "> Unexpected token : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
        return false ;
      } // else
    } // else
  } // if
  else if ( NOT_ID_StartArithExpOrBexp( test, value, isfloat, ifbexp, bexp ) ) {
    if ( strcmp( PeekCurrentToken( test )->tokenStr, ";" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( !ifbexp ) {
        if ( !isfloat ) {
          checkvalue = value ;
          if ( checkvalue == value )
            printf( "> %d\n", checkvalue ) ;
          else
            printf( "> %.3f\n", value ) ;
        } // if
        else
          printf( "> %.3f\n", value ) ;
      } // if
      else {
        if ( bexp )
          printf( "> true\n" ) ;
        else
          printf( "> false\n" ) ;
      } // else

      return true ;
    } // if
    else {
      if ( strcmp( PeekCurrentToken( test )->tokenStr, "" ) == 0 )
        GetCurrentToken( test ) ;
      else
        printf( "> Unexpected token : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
      return false ;
    } // else
  } // else if
  else {
    if ( strcmp( PeekCurrentToken( test )->tokenStr, "" ) == 0 )
      GetCurrentToken( test ) ;
    else
      printf( "> Unexpected token : '%s'\n", GetCurrentToken( test )->tokenStr ) ;
    return false ;
  } // else

} // Command()

bool IDlessArithExpOrBexp( char& test, float& value, bool& isfloat, bool& ifbexp, bool& bexp ) {
  float value1 = 0 ;
  while ( PeekCurrentToken( test )->type == SPECIAL &&
          ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ||
            strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ||
            strcmp( PeekCurrentToken( test )->tokenStr, "*" ) == 0 ||
            strcmp( PeekCurrentToken( test )->tokenStr, "/" ) == 0 ) ) {
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ||
           strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) ) {
      if ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Term( test, value1, isfloat ) ) {
          value = value + value1 ;
        } // if
        else
          return false ;
      } // if
      else if ( strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Term( test, value1, isfloat ) ) {
          value = value - value1 ;
        } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // if
    else if ( PeekCurrentToken( test )->type == SPECIAL &&
              ( strcmp( PeekCurrentToken( test )->tokenStr, "*" ) == 0 ||
                strcmp( PeekCurrentToken( test )->tokenStr, "/" ) == 0 ) ) {
      if ( strcmp( PeekCurrentToken( test )->tokenStr, "*" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Factor( test, value1, isfloat ) ) {
          value = value * value1 ;
        } // if
        else
          return false ;
      } // if
      else if ( strcmp( PeekCurrentToken( test )->tokenStr, "/" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Factor( test, value1, isfloat ) ) {
          value = value / value1 ;
        } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // else if
    else
      return false ;
    value1 = 0 ;
  } // while

  char bop[10] = "" ;
  if ( BooleanOprator( test, value, ifbexp, bop ) ) {
    float value2 = 0 ;
    if ( ArithExp( test, value2, isfloat ) ) {
      if ( strcmp( bop, "=" ) == 0 ) {
        if ( value == value2 )
          bexp = true ;
        else
          bexp = false ;
      } // if
      else if ( strcmp( bop, "<>" ) == 0 ) {
        if ( value != value2 )
          bexp = true ;
        else
          bexp = false ;
      } // else if
      else if ( strcmp( bop, ">" ) == 0 ) {
        if ( value > value2 )
          bexp = true ;
        else
          bexp = false ;
      } // else if
      else if ( strcmp( bop, "<" ) == 0 ) {
        if ( value < value2 )
          bexp = true ;
        else
          bexp = false ;
      } // else if
      else if ( strcmp( bop, ">=" ) == 0 ) {
        if ( value >= value2 )
          bexp = true ;
        else
          bexp = false ;
      } // else if
      else if ( strcmp( bop, "<=" ) == 0 ) {
        if ( value <= value2 )
          bexp = true ;
        else
          bexp = false ;
      } // else if
      else
        return false ;
    } // if
    else
      return false ;
  } // if

  return true ;
} // IDlessArithExpOrBexp()

bool BooleanOprator( char& test, float& value, bool& ifbexp, char bop[10] ) {
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       ( strcmp( PeekCurrentToken( test )->tokenStr, "=" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, "<>" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, ">" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, "<" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, ">=" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, "<=" ) == 0 ) ) {
    ifbexp = true ;
    strcpy( bop, GetCurrentToken( test )->tokenStr ) ;
    return true ;
  } // if
  else
    return false ;
} // BooleanOprator()

bool NOT_ID_StartArithExpOrBexp( char& test, float& value, bool& isfloat, bool& ifbexp, bool& bexp ) {
  if ( NOT_ID_StartArithExp( test, value, isfloat ) ) {
    char bop[10] = "" ;
    if ( BooleanOprator( test, value, ifbexp, bop ) ) {
      float value1 = 0 ;
      if ( ArithExp( test, value1, isfloat ) ) {
        if ( strcmp( bop, "=" ) == 0 ) {
          if ( value == value1 )
            bexp = true ;
          else
            bexp = false ;
        } // if
        else if ( strcmp( bop, "<>" ) == 0 ) {
          if ( value != value1 )
            bexp = true ;
          else
            bexp = false ;
        } // else if
        else if ( strcmp( bop, ">" ) == 0 ) {
          if ( value > value1 )
            bexp = true ;
          else
            bexp = false ;
        } // else if
        else if ( strcmp( bop, "<" ) == 0 ) {
          if ( value < value1 )
            bexp = true ;
          else
            bexp = false ;
        } // else if
        else if ( strcmp( bop, ">=" ) == 0 ) {
          if ( value >= value1 )
            bexp = true ;
          else
            bexp = false ;
        } // else if
        else if ( strcmp( bop, "<=" ) == 0 ) {
          if ( value <= value1 )
            bexp = true ;
          else
            bexp = false ;
        } // else if
        else
          return false ;
      } // if
      else
        return false ;
    } // if

    return true ;
  } // if
  else
    return false ;
} // NOT_ID_StartArithExpOrBexp()

bool NOT_ID_StartArithExp( char& test, float& value, bool& isfloat ) {
  if ( NOT_ID_StartTerm( test, value, isfloat ) ) {
    float value1 = 0 ;
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ||
              strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) ) {
      if ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Term( test, value1, isfloat ) ) {
          value = value + value1 ;
        } // if
        else
          return false ;
      } // if
      else if ( strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Term( test, value1, isfloat ) ) {
          value = value - value1 ;
        } // if
        else
          return false ;
      } // else if
      else
        return false ;
      value1 = 0 ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // NOT_ID_StartArithExp()

bool NOT_ID_StartTerm( char& test, float& value, bool& isfloat ) {
  if ( NOT_ID_StartFactor( test, value, isfloat ) ) {
    float value1 = 0 ;
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            ( strcmp( PeekCurrentToken( test )->tokenStr, "*" ) == 0 ||
              strcmp( PeekCurrentToken( test )->tokenStr, "/" ) == 0 ) ) {
      if ( strcmp( PeekCurrentToken( test )->tokenStr, "*" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Factor( test, value1, isfloat ) ) {
          value = value * value1 ;
        } // if
        else
          return false ;
      } // if
      else if ( strcmp( PeekCurrentToken( test )->tokenStr, "/" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Factor( test, value1, isfloat ) ) {
          value = value / value1 ;
        } // if
        else
          return false ;
      } // else if
      else
        return false ;
      value1 = 0 ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // NOT_ID_StartTerm()

bool NOT_ID_StartFactor( char& test, float& value, bool& isfloat ) {
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) ) {
    if ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( PeekCurrentToken( test )->type == CONSTANT ) {
        value = atof( GetCurrentToken( test )->tokenStr ) ;
        return true ;
      } // if
      else if ( PeekCurrentToken( test )->type == FLOAT ) {
        value = atof( GetCurrentToken( test )->tokenStr ) ;
        isfloat = true ;
        return true ;
      } // else if
      else
        return false ;
    } // if
    else if ( strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( PeekCurrentToken( test )->type == CONSTANT ) {
        value = -atof( GetCurrentToken( test )->tokenStr ) ;
        return true ;
      } // if
      else if ( PeekCurrentToken( test )->type == FLOAT ) {
        value = -atof( GetCurrentToken( test )->tokenStr ) ;
        isfloat = true ;
        return true ;
      } // else if
      else
        return false ;
    } // else if
    else
      return false ;
  } // if

  if ( PeekCurrentToken( test )->type == CONSTANT ) {
    value = atof( GetCurrentToken( test )->tokenStr ) ;
    return true ;
  } // if
  else if ( PeekCurrentToken( test )->type == FLOAT ) {
    value = atof( GetCurrentToken( test )->tokenStr ) ;
    isfloat = true ;
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( ArithExp( test, value, isfloat ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
        GetCurrentToken( test ) ;
        return true ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // else if
  else
    return false ;
} // NOT_ID_StartFactor()

bool ArithExp( char& test, float& value, bool& isfloat ) {
  if ( Term( test, value, isfloat ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ||
              strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) ) {
      float value1 = 0 ;
      if ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Term( test, value1, isfloat ) ) {
          value = value + value1 ;
        } // if
        else
          return false ;
      } // if
      else if ( strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Term( test, value1, isfloat ) ) {
          value = value - value1 ;
        } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // ArithExp()

bool Term( char& test, float& value, bool& isfloat ) {
  if ( Factor( test, value, isfloat ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            ( strcmp( PeekCurrentToken( test )->tokenStr, "*" ) == 0 ||
              strcmp( PeekCurrentToken( test )->tokenStr, "/" ) == 0 ) ) {
      float value1 = 0 ;
      if ( strcmp( PeekCurrentToken( test )->tokenStr, "*" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Factor( test, value1, isfloat ) ) {
          value = value * value1 ;
        } // if
        else
          return false ;
      } // if
      else if ( strcmp( PeekCurrentToken( test )->tokenStr, "/" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Factor( test, value1, isfloat ) ) {
          value = value / value1 ;
        } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Term()

bool Factor( char& test, float& value, bool& isfloat ) {
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) ) {
    if ( strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( PeekCurrentToken( test )->type == CONSTANT ) {
        value = atof( GetCurrentToken( test )->tokenStr ) ;
        return true ;
      } // if
      else if ( PeekCurrentToken( test )->type == FLOAT ) {
        value = atof( GetCurrentToken( test )->tokenStr ) ;
        isfloat = true ;
        return true ;
      } // else if
      else
        return false ;
    } // if
    else if ( strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( PeekCurrentToken( test )->type == CONSTANT ) {
        value = -atof( GetCurrentToken( test )->tokenStr ) ;
        return true ;
      } // if
      else if ( PeekCurrentToken( test )->type == FLOAT ) {
        value = -atof( GetCurrentToken( test )->tokenStr ) ;
        isfloat = true ;
        return true ;
      } // else if
      else
        return false ;
    } // else if
    else
      return false ;
  } // if

  if ( PeekCurrentToken( test )->type == IDENTIFIER ) {
    if ( SearchID( PeekCurrentToken( test )->tokenStr, value, isfloat ) ) {
      GetCurrentToken( test ) ;
      int checkvalue = 0 ;
      checkvalue = value ;
      if ( checkvalue == value ) {
      } // if
      else
        isfloat = true ;
      return true ;
    } // if
    else {
      printf( "> Undefined identifier : '%s'\n", PeekCurrentToken( test )->tokenStr ) ;
      strcpy( PeekCurrentToken( test )->tokenStr, "" ) ;
      return false ;
    } // else
  } // if
  else if ( PeekCurrentToken( test )->type == CONSTANT ) {
    value = atof( GetCurrentToken( test )->tokenStr ) ;
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == FLOAT ) {
    value = atof( GetCurrentToken( test )->tokenStr ) ;
    isfloat = true ;
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( ArithExp( test, value, isfloat ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
        GetCurrentToken( test ) ;
        return true ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // else if
  else
    return false ;
} // Factor()
*/

bool User_input( char& test ) {
  printf( "> " ) ;
  gLine = 1 ;
  gColumn = 0 ;
  if ( PeekFirstToken( test ) == NULL && test == '\n' ) {
    gLine-- ;
  } // if

  if ( Definition( test ) ) {
    EraseDefiningID() ;
    GetChar( test ) ;
    strcpy( gFunctionname, "" ) ;
    if ( gDefiningFunction != NULL ) {
      delete [] gDefiningFunction ;
      gDefiningFunction = NULL ;
    } // if

    gDefiningFunctionwalk = NULL ;
    return true ;
  } // if
  else if ( Statement( test ) ) {
    EraseDefiningID() ;
    printf( "Statement executed ...\n" ) ;
    GetChar( test ) ;
    strcpy( gFunctionname, "" ) ;
    delete [] gDefiningFunction ;
    gDefiningFunction = NULL ;
    gDefiningFunctionwalk = NULL ;
    return true ;
  } // else if
  else {
    if ( PeekCurrentToken( test )->type != ERROR ) {
      if ( test == '\n' ) {
        printf( "line %d : syntax error when token is '%s'\n", gLine-1,
                 PeekCurrentToken( test )->tokenStr ) ;
      } // if
      else {
        printf( "line %d : syntax error when token is '%s'\n", gLine,
                 PeekCurrentToken( test )->tokenStr ) ;
      } // else
    } // if

    GetCurrentToken( test ) ;
    while ( test != '\n' ) GetChar( test ) ;
    strcpy( gFunctionname, "" ) ;
    delete [] gDefiningFunction ;
    gDefiningFunction = NULL ;
    gDefiningFunctionwalk = NULL ;
    EraseDefiningID() ;
    return false ;
  } // else
} // User_input()

bool Definition( char& test ) {
  ID definingID ;
  if ( PeekCurrentToken( test )->type == NONIDENTIFIER &&
       strcmp( PeekCurrentToken( test )->tokenStr, "void" ) == 0 ) {
    strcpy( definingID.type, GetCurrentToken( test )->tokenStr ) ;
    if ( PeekCurrentToken( test )->type == IDENTIFIER ) {
      strcpy( definingID.name, GetCurrentToken( test )->tokenStr ) ;
      AddIDToDefining( definingID ) ;
      if ( Function_definition_without_ID( test ) ) {
        AddDefiningToFunctiontable() ;
        return true ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // if
  else if ( Type_specifier( test ) ) {
    strcpy( definingID.type, GetCurrentToken( test )->tokenStr ) ;
    if ( PeekCurrentToken( test )->type == IDENTIFIER ) {
      strcpy( definingID.name, GetCurrentToken( test )->tokenStr ) ;
      if ( Function_definition_or_declarators( test, definingID ) ) {
        return true ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // else if
  else
    return false ;
} // Definition()

bool Type_specifier( char& test ) {
  if ( PeekCurrentToken( test )->type == NONIDENTIFIER &&
       ( strcmp( PeekCurrentToken( test )->tokenStr, "int" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, "char" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, "float" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, "string" ) == 0 ||
         strcmp( PeekCurrentToken( test )->tokenStr, "bool" ) == 0 ) ) {
    return true ;
  } // if
  else
    return false ;
} // Type_specifier()

bool Function_definition_or_declarators( char& test, ID definingID ) {
  if ( Function_definition_without_ID( test ) ) {
    AddDefiningToFunctiontable() ;
    return true ;
  } // if
  else if ( Rest_of_declarators( test, definingID ) ) {
    AddDefiningToIDtable() ;
    return true ;
  } // else if
  else
    return false ;
} // Function_definition_or_declarators()

bool Rest_of_declarators( char& test, ID definingID ) {
  ID restofdefiningID ;
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( PeekCurrentToken( test )->type == CONSTANT ) {
      strcpy( definingID.length, GetCurrentToken( test )->tokenStr ) ;
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "]" ) == 0 ) {
        GetCurrentToken( test ) ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // if
  else
    strcpy( definingID.length, "" ) ;

  AddIDToDefining( definingID ) ;

  while ( PeekCurrentToken( test )->type == SPECIAL &&
          strcmp( PeekCurrentToken( test )->tokenStr, "," ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( PeekCurrentToken( test )->type == IDENTIFIER ) {
      strcpy( restofdefiningID.name, GetCurrentToken( test )->tokenStr ) ;
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( PeekCurrentToken( test )->type == CONSTANT ) {
          strcpy( restofdefiningID.length, GetCurrentToken( test )->tokenStr ) ;
          if ( PeekCurrentToken( test )->type == SPECIAL &&
               strcmp( PeekCurrentToken( test )->tokenStr, "]" ) == 0 ) {
            GetCurrentToken( test ) ;
          } // if
          else
            return false ;
        } // if
        else
          return false ;
      } // if
      else
        strcpy( restofdefiningID.length, "" ) ;

      AddIDToDefining( restofdefiningID ) ;
    } // if
    else
      return false ;
  } // while

  if ( PeekCurrentToken( test )->type == SPECIAL &&
       strcmp( PeekCurrentToken( test )->tokenStr, ";" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // if
  else
    return false ;
} // Rest_of_declarators()

bool Function_definition_without_ID( char& test ) {
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( PeekCurrentToken( test )->type == NONIDENTIFIER &&
         strcmp( PeekCurrentToken( test )->tokenStr, "void" ) == 0 ) {
      GetCurrentToken( test ) ;
    } // if
    else if ( Formal_parameter_list( test ) ) {
    } // else if

    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Compound_statement( test ) )
        return true ;
      else
        return false ;
    } // if
    else
      return false ;
  } // if
  else
    return false ;
} // Function_definition_without_ID()

bool Formal_parameter_list( char& test ) {
  ID definingID ;
  if ( Type_specifier( test ) ) {
    strcpy( definingID.type, GetCurrentToken( test )->tokenStr ) ;
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "&" ) == 0 ) {
      GetCurrentToken( test ) ;
    } // if

    if ( PeekCurrentToken( test )->type == IDENTIFIER ) {
      strcpy( definingID.name, GetCurrentToken( test )->tokenStr ) ;
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( PeekCurrentToken( test )->type == CONSTANT ) {
          strcpy( definingID.length, GetCurrentToken( test )->tokenStr ) ;
          if ( PeekCurrentToken( test )->type == SPECIAL &&
               strcmp( PeekCurrentToken( test )->tokenStr, "]" ) == 0 ) {
            GetCurrentToken( test ) ;
          } // if
          else
            return false ;
        } // if
        else
          return false ;
      } // if
      else
        strcpy( definingID.length, "" ) ;

      AddIDToDefining( definingID ) ;

      while ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "," ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Type_specifier( test ) ) {
          strcpy( definingID.type, GetCurrentToken( test )->tokenStr ) ;
          if ( PeekCurrentToken( test )->type == SPECIAL &&
               strcmp( PeekCurrentToken( test )->tokenStr, "&" ) == 0 ) {
            GetCurrentToken( test ) ;
          } // if

          if ( PeekCurrentToken( test )->type == IDENTIFIER ) {
            strcpy( definingID.name, GetCurrentToken( test )->tokenStr ) ;
            if ( PeekCurrentToken( test )->type == SPECIAL &&
                 strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) {
              strcpy( definingID.length, GetCurrentToken( test )->tokenStr ) ;
              if ( PeekCurrentToken( test )->type == CONSTANT ) {
                GetCurrentToken( test ) ;
                if ( PeekCurrentToken( test )->type == SPECIAL &&
                     strcmp( PeekCurrentToken( test )->tokenStr, "]" ) == 0 ) {
                  GetCurrentToken( test ) ;
                } // if
                else
                  return false ;
              } // if
              else
                return false ;
            } // if
            else
              strcpy( definingID.length, "" ) ;

            AddIDToDefining( definingID ) ;

            return true ;
          } // if
          else
            return false ;
        } // if
        else
          return false ;
      } // while

      return true ;
    } // if
    else
      return false ;
  } // if
  else
    return false ;
} // Formal_parameter_list()

bool Compound_statement( char& test ) {
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       strcmp( PeekCurrentToken( test )->tokenStr, "{" ) == 0 ) {
    GetCurrentToken( test ) ;
    while ( Declaration( test ) || Statement( test ) ) { } // while

    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "}" ) == 0 ) {
      GetCurrentToken( test ) ;
      return true ;
    } // if
    else
      return false ;
  } // if
  else
    return false ;
} // Compound_statement()

bool Declaration( char& test ) {
  ID definingID ;
  if ( Type_specifier( test ) ) {
    strcpy( definingID.type, GetCurrentToken( test )->tokenStr ) ;
    if ( PeekCurrentToken( test )->type == IDENTIFIER ) {
      strcpy( definingID.name, GetCurrentToken( test )->tokenStr ) ;
      AddIDToDefining( definingID ) ;
      if ( Rest_of_declarators( test, definingID ) ) {
        return true ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // if
  else
    return false ;
} // Declaration()

bool Statement( char& test ) {
  bool isfloat = false ;
  float floatvalue = 0 ;
  Str100 stringvalue = "" ;
  bool boolvalue = false ;
  bool iscout = false ;
  bool isbexp = false ;
  bool isexp = false ;
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       strcmp( PeekCurrentToken( test )->tokenStr, ";" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // if
  else if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                        isbexp, isexp ) ) {
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, ";" ) == 0 ) {
      GetCurrentToken( test ) ;
      RunFunction() ;
      return true ;
    } // if
    else
      return false ;
  } // else if
  else if ( PeekCurrentToken( test )->type == NONIDENTIFIER &&
            strcmp( PeekCurrentToken( test )->tokenStr, "return" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                     isbexp, isexp ) ) { } // if

    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, ";" ) == 0 ) {
      GetCurrentToken( test ) ;
      return true ;
    } // if
    else
      return false ;
  } // else if
  else if ( Compound_statement( test ) ) {
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == NONIDENTIFIER &&
            strcmp( PeekCurrentToken( test )->tokenStr, "if" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                       isbexp, isexp ) ) {
        if ( PeekCurrentToken( test )->type == SPECIAL &&
             strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
          GetCurrentToken( test ) ;
          if ( Statement( test ) ) {
            if ( PeekCurrentToken( test )->type == NONIDENTIFIER &&
                 strcmp( PeekCurrentToken( test )->tokenStr, "else" ) == 0 ) {
              GetCurrentToken( test ) ;
              if ( Statement( test ) ) {
                return true ;
              } // if
              else
                return false ;
            } // if

            return true ;
          } // if
          else
            return false ;
        } // if
        else
          return false ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // else if
  else if ( PeekCurrentToken( test )->type == NONIDENTIFIER &&
            strcmp( PeekCurrentToken( test )->tokenStr, "while" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                       isbexp, isexp ) ) {
        if ( PeekCurrentToken( test )->type == SPECIAL &&
             strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
          GetCurrentToken( test ) ;
          if ( Statement( test ) ) {
            return true ;
          } // if
          else
            return false ;
        } // if
        else
          return false ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // else if
  else if ( PeekCurrentToken( test )->type == NONIDENTIFIER &&
            strcmp( PeekCurrentToken( test )->tokenStr, "do" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( Statement( test ) ) {
      if ( PeekCurrentToken( test )->type == NONIDENTIFIER &&
           strcmp( PeekCurrentToken( test )->tokenStr, "while" ) == 0 ) {
        if ( PeekCurrentToken( test )->type == SPECIAL &&
             strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
          GetCurrentToken( test ) ;
          if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                           isbexp, isexp ) ) {
            if ( PeekCurrentToken( test )->type == SPECIAL &&
                 strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
              GetCurrentToken( test ) ;
              if ( PeekCurrentToken( test )->type == SPECIAL &&
                   strcmp( PeekCurrentToken( test )->tokenStr, ";" ) == 0 ) {
                GetCurrentToken( test ) ;
                return true ;
              } // if
              else
                return false ;
            } // if
            else
              return false ;
          } // if
          else
            return false ;
        } // if
        else
          return false ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // else if
  else
    return false ;
} // Statement()

bool Expression( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                 bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Basic_expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout, isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "," ) == 0 ) {
      if ( !Basic_expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout, isbexp, isexp ) )
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Expression()

bool Basic_expression( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                       bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  ID definingID ;
  if ( PeekCurrentToken( test )->type == IDENTIFIER &&
       ( ( SearchID( PeekCurrentToken( test )->tokenStr, definingID.value, definingID.isfloat ) ||
           strcmp( PeekCurrentToken( test )->tokenStr, "cin" ) == 0 ||
           strcmp( PeekCurrentToken( test )->tokenStr, "cout" ) == 0 ) ||
         SearchFunctionname( PeekCurrentToken( test )->tokenStr ) ) ) {
    if ( SearchID( PeekCurrentToken( test )->tokenStr, definingID.value, definingID.isfloat ) ) {
      strcpy( definingID.name, GetCurrentToken( test )->tokenStr ) ;
      if ( Rest_of_Identifier_started_basic_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                                 isbexp, isexp ) ) {
        return true ;
      } // if
      else
        return false ;
    } // if
    else if ( strcmp( PeekCurrentToken( test )->tokenStr, "cin" ) == 0 ||
              strcmp( PeekCurrentToken( test )->tokenStr, "cout" ) == 0 ) {
      GetCurrentToken( test ) ;
      iscout = true ;
      if ( Rest_of_Identifier_started_basic_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                                 isbexp, isexp ) ) {

        return true ;
      } // if
      else
        return false ;
    } // else if
    else if ( SearchFunctionname( PeekCurrentToken( test )->tokenStr ) ) {
      strcpy( gFunctionname, GetCurrentToken( test )->tokenStr ) ;
      if ( Rest_of_Identifier_started_basic_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                                 isbexp, isexp ) ) {

        return true ;
      } // if
      else
        return false ;
    } // else if
    else {
      GetCurrentToken( test ) ;
      if ( Rest_of_Identifier_started_basic_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                                 isbexp, isexp ) ) {
        return true ;
      } // if
      else
        return false ;
    } // else
  } // if
  else if ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) ) {
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( PeekCurrentToken( test )->type == IDENTIFIER &&
           ( SearchID( PeekCurrentToken( test )->tokenStr, definingID.value, definingID.isfloat ) ||
             SearchFunctionname( PeekCurrentToken( test )->tokenStr ) ) ) {
        GetCurrentToken( test ) ;
        if ( Rest_of_PPMM_Identifier_started_basic_exp( test, isfloat, floatvalue, stringvalue, boolvalue,
                                                        iscout, isbexp, isexp ) ) {
          return true ;
        } // if
        else
          return false ;
      } // if
      else
        return false ;
    } // if
    else if ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( PeekCurrentToken( test )->type == IDENTIFIER &&
           ( SearchID( PeekCurrentToken( test )->tokenStr, definingID.value, definingID.isfloat ) ||
             SearchFunctionname( PeekCurrentToken( test )->tokenStr ) ) ) {
        GetCurrentToken( test ) ;
        if ( Rest_of_PPMM_Identifier_started_basic_exp( test, isfloat, floatvalue, stringvalue, boolvalue,
                                                        iscout, isbexp, isexp ) ) {
          return true ;
        } // if
        else
          return false ;
      } // if
      else
        return false ;
    } // else if
    else
      return false ;
  } // else if
  else if ( Sign( test ) ) {
    while ( Sign( test ) ) { } // while

    if ( Signed_unary_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout, isbexp, isexp ) ) {
      if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue,
                                                                           stringvalue, boolvalue, iscout,
                                                                           isbexp, isexp ) )
        return true ;
      else
        return false ;
    } // if
    else
      return false ;
  } // else if
  else if ( PeekCurrentToken( test )->type == CONSTANT ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) ) {
    if ( PeekCurrentToken( test )->type == CONSTANT ) {
      GetCurrentToken( test ) ;
      if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue,
                                                                           stringvalue, boolvalue,
                                                                           iscout, isbexp, isexp ) )
        return true ;
      else
        return false ;
    } // if
    else if ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                       isbexp, isexp ) ) {
        if ( PeekCurrentToken( test )->type == SPECIAL &&
             strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
          GetCurrentToken( test ) ;
          if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue,
                                                                               stringvalue, boolvalue,
                                                                               iscout, isbexp, isexp ) )
            return true ;
          else
            return false ;
        } // if
        else
          return false ;
      } // if
      else
        return false ;
    } // else if
    else
      return false ;
  } // else if
  else
    return false ;
} // Basic_expression()

bool Rest_of_Identifier_started_basic_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                           bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( Actual_parameter_list( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                isbexp, isexp ) ) { } // if

    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue,
                                                                           stringvalue, boolvalue, iscout,
                                                                           isbexp, isexp ) )
        return true ;
      else
        return false ;
    } // if
    else
      return false ;
  } // if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                     isbexp, isexp ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "]" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Assignment_operator( test ) ) {
          if ( Basic_expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) {
            return true ;
          } // if
          else
            return false ;
        } // if
        else if ( ( PeekCurrentToken( test )->type == SPECIAL &&
                    strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) ||
                  ( PeekCurrentToken( test )->type == SPECIAL &&
                    strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) ) {
          if ( PeekCurrentToken( test )->type == SPECIAL &&
               strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) {
            GetCurrentToken( test ) ;
            if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue,
                                                                                 stringvalue, boolvalue,
                                                                                 iscout, isbexp, isexp ) )
              return true ;
            else
              return false ;
          } // if
          else if ( PeekCurrentToken( test )->type == SPECIAL &&
                    strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) {
            GetCurrentToken( test ) ;
            if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue,
                                                                                 stringvalue, boolvalue,
                                                                                 iscout, isbexp, isexp ) )
              return true ;
            else
              return false ;
          } // else if
          else
            return false ;
        } // else if
        else
          return true ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // else if
  else if ( Assignment_operator( test ) ) {
    if ( Basic_expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout, isbexp, isexp ) ) {
      return true ;
    } // if
    else
      return false ;
  } // else if
  else if ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) ) {
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue,
                                                                           stringvalue, boolvalue,
                                                                           iscout, isbexp, isexp ) )
        return true ;
      else
        return false ;
    } // if
    else if ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue,
                                                                           stringvalue, boolvalue,
                                                                           iscout, isbexp, isexp ) )
        return true ;
      else
        return false ;
    } // else if
    else
      return false ;
  } // else if
  else if ( Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue,
                                                                            stringvalue, boolvalue,
                                                                            iscout, isbexp, isexp ) ) {
    return true ;
  } // else if
  else
    return false ;
} // Rest_of_Identifier_started_basic_exp()

bool Rest_of_PPMM_Identifier_started_basic_exp( char& test, bool& isfloat, float& floatvalue,
                                                Str100& stringvalue, bool& boolvalue,
                                                bool& iscout, bool& isbexp, bool& isexp ) {
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                     isbexp, isexp ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "]" ) == 0 ) {
        GetCurrentToken( test ) ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // if

  if ( Rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                     isbexp, isexp ) ) {
    return true ;
  } // if
  else
    return false ;
} // Rest_of_PPMM_Identifier_started_basic_exp()

bool Sign( char& test ) {
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "!" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // else if
  else
    return false ;
} // Sign()

bool Actual_parameter_list( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                            bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Basic_expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout, isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "," ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( !Basic_expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout, isbexp, isexp ) )
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Actual_parameter_list()

bool Assignment_operator( char& test ) {
  if ( PeekCurrentToken( test )->type == SPECIAL &&
       strcmp( PeekCurrentToken( test )->tokenStr, "=" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "*=" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "/=" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "%=" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "+=" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "-=" ) == 0 ) {
    GetCurrentToken( test ) ;
    return true ;
  } // else if
  else
    return false ;
} // Assignment_operator()

bool Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp( char& test, bool& isfloat,
                                                                     float& floatvalue, Str100& stringvalue,
                                                                     bool& boolvalue, bool& iscout,
                                                                     bool& isbexp, bool& isexp ) {
  if ( Rest_of_maybe_logical_OR_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                     isbexp, isexp ) ) {
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "?" ) == 0 ) {
      if ( Basic_expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                             isbexp, isexp ) ) {
        if ( PeekCurrentToken( test )->type == SPECIAL &&
             strcmp( PeekCurrentToken( test )->tokenStr, ":" ) == 0 ) {
          if ( Basic_expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) {
            return true ;
          } // if
          else
            return false ;
        } // if
        else
          return false ;
      } // if
      else
        return false ;
    } // if

    return true ;
  } // if
  else
    return false ;
} // Rest_of_maybe_conditional_exp_and_rest_of_maybe_logical_OR_exp()

bool Rest_of_maybe_logical_OR_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                   bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Rest_of_maybe_logical_AND_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                      isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "||" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Maybe_logical_AND_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                  isbexp, isexp ) ) { } // if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Rest_of_maybe_logical_OR_exp()

bool Maybe_logical_AND_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                            bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Maybe_bit_OR_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                         isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "&&" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Maybe_bit_OR_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                             isbexp, isexp ) ) { } // if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Maybe_logical_AND_exp()

bool Rest_of_maybe_logical_AND_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                    bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Rest_of_maybe_bit_OR_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "&&" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Maybe_bit_OR_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                             isbexp, isexp ) ) { } // if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Rest_of_maybe_logical_AND_exp()

bool Maybe_bit_OR_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                       bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Maybe_bit_ex_OR_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                            isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "|" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Maybe_bit_ex_OR_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                isbexp, isexp ) ) { } // if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Maybe_bit_OR_exp()

bool Rest_of_maybe_bit_OR_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                               bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Rest_of_maybe_bit_ex_OR_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                    isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "|" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Maybe_bit_ex_OR_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                isbexp, isexp ) ) { } // if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Rest_of_maybe_bit_OR_exp()

bool Maybe_bit_ex_OR_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                          bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Maybe_bit_AND_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                          isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "^" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Maybe_bit_AND_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                              isbexp, isexp ) ) { } // if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Maybe_bit_ex_OR_exp()

bool Rest_of_maybe_bit_ex_OR_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                  bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Rest_of_maybe_bit_AND_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                  isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "^" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Maybe_bit_AND_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                              isbexp, isexp ) ) { } // if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Rest_of_maybe_bit_ex_OR_exp()

bool Maybe_bit_AND_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                        bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Maybe_equality_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                           isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "&" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Maybe_equality_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                               isbexp, isexp ) ) { } // if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Maybe_bit_AND_exp()

bool Rest_of_maybe_bit_AND_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Rest_of_maybe_equality_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                   isbexp, isexp ) ) {
    while ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "&" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Maybe_equality_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                               isbexp, isexp ) ) { } // if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Rest_of_maybe_bit_AND_exp()

bool Maybe_equality_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                         bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Maybe_relational_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                             isbexp, isexp ) ) {
    while ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "==" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "!=" ) == 0 ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "==" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_equality_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) { } // if
        else
          return false ;
      } // if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, "!=" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_equality_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) { } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Maybe_equality_exp()

bool Rest_of_maybe_equality_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                 bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Rest_of_maybe_relational_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                     isbexp, isexp ) ) {
    while ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "==" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "!=" ) == 0 ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "==" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_equality_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) { } // if
        else
          return false ;
      } // if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, "!=" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_equality_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) { } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Rest_of_maybe_equality_exp()

bool Maybe_relational_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                           bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Maybe_shift_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                        isbexp, isexp ) ) {
    while ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "<" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, ">" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "<=" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, ">=" ) == 0 ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "<" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_shift_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                              isbexp, isexp ) ) { } // if
        else
          return false ;
      } // if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, ">" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_shift_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                              isbexp, isexp ) ) { } // if
        else
          return false ;
      } // else if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, "<=" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_shift_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                              isbexp, isexp ) ) { } // if
        else
          return false ;
      } // else if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, ">=" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_shift_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                              isbexp, isexp ) ) { } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Maybe_relational_exp()

bool Rest_of_maybe_relational_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                   bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Rest_of_maybe_shift_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                isbexp, isexp ) ) {
    while ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "<" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, ">" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "<=" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, ">=" ) == 0 ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "<" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_shift_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                              isbexp, isexp ) ) { } // if
        else
          return false ;
      } // if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, ">" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_shift_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                              isbexp, isexp ) ) { } // if
        else
          return false ;
      } // else if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, "<=" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_shift_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                              isbexp, isexp ) ) { } // if
        else
          return false ;
      } // else if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, ">=" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_shift_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                              isbexp, isexp ) ) { } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Rest_of_maybe_relational_exp()

bool Maybe_shift_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                      bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Maybe_additive_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                           isbexp, isexp ) ) {
    while ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "<<" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, ">>" ) == 0 ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "<<" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_additive_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) { } // if
        else
          return false ;
      } // if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, ">>" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_additive_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) { } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Maybe_shift_exp()

bool Rest_of_maybe_shift_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                              bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Rest_of_maybe_additive_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                   isbexp, isexp ) ) {
    while ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "<<" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, ">>" ) == 0 ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "<<" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( iscout ) {
          if ( isexp )
            printf( "%f", floatvalue ) ;
          else if ( isbexp )
            printf( "%d", boolvalue ) ;
        } // if

        if ( Maybe_additive_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) { } // if
        else
          return false ;
      } // if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, ">>" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_additive_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) { } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Rest_of_maybe_shift_exp()

bool Maybe_additive_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                         bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  bool isfloat1 = false ;
  float floatvalue1 = 0 ;
  Str100 stringvalue1 = "" ;
  bool boolvalue1 = false ;
  if ( Maybe_mult_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                       isbexp, isexp ) ) {
    while ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_mult_exp( test, isfloat1, floatvalue1, stringvalue1, boolvalue1, iscout,
                             isbexp, isexp ) ) {
          floatvalue = floatvalue + floatvalue1 ;
        } // if
        else
          return false ;
      } // if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_mult_exp( test, isfloat1, floatvalue1, stringvalue1, boolvalue1, iscout,
                             isbexp, isexp ) ) {
          floatvalue = floatvalue - floatvalue1 ;
        } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Maybe_additive_exp()

bool Rest_of_maybe_additive_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                                 bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  bool isfloat1 = false ;
  float floatvalue1 = 0 ;
  Str100 stringvalue1 = "" ;
  bool boolvalue1 = false ;
  if ( Rest_of_maybe_mult_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                               isbexp, isexp ) ) {
    while ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "+" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_mult_exp( test, isfloat1, floatvalue1, stringvalue1, boolvalue1, iscout,
                             isbexp, isexp ) ) {
          floatvalue = floatvalue + floatvalue1 ;
        } // if
        else
          return false ;
      } // if
      else if ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, "-" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Maybe_mult_exp( test, isfloat1, floatvalue1, stringvalue1, boolvalue1, iscout,
                             isbexp, isexp ) ) {
          floatvalue = floatvalue - floatvalue1 ;
        } // if
        else
          return false ;
      } // else if
      else
        return false ;
    } // while

    return true ;
  } // if
  else
    return false ;
} // Rest_of_maybe_additive_exp()

bool Maybe_mult_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                     bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  if ( Unary_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                  isbexp, isexp ) ) {
    if ( Rest_of_maybe_mult_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                 isbexp, isexp ) ) {
      return true ;
    } // if
    else
      return false ;
  } // if
  else
    return false ;
} // Maybe_mult_exp()

bool Rest_of_maybe_mult_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                             bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  bool isfloat1 = false ;
  float floatvalue1 = 0 ;
  Str100 stringvalue1 = "" ;
  bool boolvalue1 = false ;
  int intvalue = 0, intvalue1 = 0 ;
  while ( ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "*" ) == 0 ) ||
          ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "/" ) == 0 ) ||
          ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "%" ) == 0 ) ) {
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "*" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Unary_exp( test, isfloat1, floatvalue1, stringvalue1, boolvalue1, iscout,
                      isbexp, isexp ) ) {
        floatvalue = floatvalue * floatvalue1 ;
      } // if
      else
        return false ;
    } // if
    else if ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "/" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Unary_exp( test, isfloat1, floatvalue1, stringvalue1, boolvalue1, iscout,
                      isbexp, isexp ) ) {
        floatvalue = floatvalue / floatvalue1 ;
      } // if
      else
        return false ;
    } // else if
    else if ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "%" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Unary_exp( test, isfloat1, floatvalue1, stringvalue1, boolvalue1, iscout,
                      isbexp, isexp ) ) {
        intvalue = floatvalue ;
        intvalue1 = floatvalue1 ;
        intvalue = intvalue % intvalue1 ;

      } // if
      else
        return false ;
    } // else if
    else
      return false ;
  } // while

  return true ;
} // Rest_of_maybe_mult_exp()

bool Unary_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  ID definingID ;
  if ( Sign( test ) ) {
    while ( Sign( test ) ) { } // while

    if ( Signed_unary_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                           isbexp, isexp ) ) {
      return true ;
    } // if
    else
      return false ;
  } // if
  else if ( Unsigned_unary_exp( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                isbexp, isexp ) ) {
    return true ;
  } // else if
  else if ( ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) ||
            ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) ) {
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( PeekCurrentToken( test )->type == IDENTIFIER &&
           ( SearchID( PeekCurrentToken( test )->tokenStr, definingID.value, definingID.isfloat ) ||
             SearchFunctionname( PeekCurrentToken( test )->tokenStr ) ) ) {
        GetCurrentToken( test ) ;
        if ( PeekCurrentToken( test )->type == SPECIAL &&
             strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) {
          GetCurrentToken( test ) ;
          if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                           isbexp, isexp ) ) {
            if ( PeekCurrentToken( test )->type == SPECIAL &&
                 strcmp( PeekCurrentToken( test )->tokenStr, "]" ) == 0 ) {
              GetCurrentToken( test ) ;
              return true ;
            } // if
            else
              return false ;
          } // if
          else
            return false ;
        } // if

        return true ;
      } // if
      else
        return false ;
    } // if
    else if ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( PeekCurrentToken( test )->type == IDENTIFIER &&
           ( SearchID( PeekCurrentToken( test )->tokenStr, definingID.value, definingID.isfloat ) ||
             SearchFunctionname( PeekCurrentToken( test )->tokenStr ) ) ) {
        strcpy( definingID.name, GetCurrentToken( test )->tokenStr ) ;
        if ( PeekCurrentToken( test )->type == SPECIAL &&
             strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) {
          GetCurrentToken( test ) ;
          if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                           isbexp, isexp ) ) {
            if ( PeekCurrentToken( test )->type == SPECIAL &&
                 strcmp( PeekCurrentToken( test )->tokenStr, "]" ) == 0 ) {
              GetCurrentToken( test ) ;
              return true ;
            } // if
            else
              return false ;
          } // if
          else
            return false ;
        } // if

        return true ;
      } // if
      else
        return false ;
    } // else if
    else
      return false ;
  } // else if
  else
    return false ;
} // Unary_exp()

bool Signed_unary_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                       bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  ID definingID ;
  if ( PeekCurrentToken( test )->type == IDENTIFIER &&
       ( SearchID( PeekCurrentToken( test )->tokenStr, definingID.value, definingID.isfloat ) ||
         SearchFunctionname( PeekCurrentToken( test )->tokenStr ) ) ) {
    strcpy( definingID.name, GetCurrentToken( test )->tokenStr ) ;
    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Actual_parameter_list( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                  isbexp, isexp ) ) { } // if

      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
        GetCurrentToken( test ) ;
        return true ;
      } // if
      else
        return false ;
    } // if
    else if ( PeekCurrentToken( test )->type == SPECIAL &&
              strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                       isbexp, isexp ) ) {
        if ( PeekCurrentToken( test )->type == SPECIAL &&
             strcmp( PeekCurrentToken( test )->tokenStr, "]" ) == 0 ) {
          GetCurrentToken( test ) ;
          return true ;
        } // if
        else
          return false ;
      } // if
      else
        return false ;
    } // else if

    return true ;
  } // if
  else if ( PeekCurrentToken( test )->type == CONSTANT ) {
    floatvalue = atof( GetCurrentToken( test )->tokenStr ) ;
    isexp = true ;
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                     isbexp, isexp ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
        GetCurrentToken( test ) ;
        return true ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // else if
  else
    return false ;
} // Signed_unary_exp()

bool Unsigned_unary_exp( char& test, bool& isfloat, float& floatvalue, Str100& stringvalue,
                         bool& boolvalue, bool& iscout, bool& isbexp, bool& isexp ) {
  ID definingID ;
  if ( PeekCurrentToken( test )->type == IDENTIFIER &&
       ( SearchID( PeekCurrentToken( test )->tokenStr, definingID.value, definingID.isfloat ) ||
         SearchFunctionname( PeekCurrentToken( test )->tokenStr ) ) ) {
    strcpy( definingID.name, GetCurrentToken( test )->tokenStr ) ;

    if ( PeekCurrentToken( test )->type == SPECIAL &&
         strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
      GetCurrentToken( test ) ;
      if ( Actual_parameter_list( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                                  isbexp, isexp ) ) { } // if

      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
        GetCurrentToken( test ) ;
        return true ;
      } // if
      else
        return false ;
    } // if
    else if ( ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) ||
              ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) ||
              ( PeekCurrentToken( test )->type == SPECIAL &&
                strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, "[" ) == 0 ) {
        GetCurrentToken( test ) ;
        if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                         isbexp, isexp ) ) {
          if ( PeekCurrentToken( test )->type == SPECIAL &&
               strcmp( PeekCurrentToken( test )->tokenStr, "]" ) == 0 ) {
            GetCurrentToken( test ) ;
            if ( ( PeekCurrentToken( test )->type == SPECIAL &&
                   strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) ||
                 ( PeekCurrentToken( test )->type == SPECIAL &&
                   strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) ) {
              GetCurrentToken( test ) ;
            } // if

            return true ;
          } // if
          else
            return false ;
        } // if
      } // if
      else if ( ( PeekCurrentToken( test )->type == SPECIAL &&
                  strcmp( PeekCurrentToken( test )->tokenStr, "++" ) == 0 ) ||
                ( PeekCurrentToken( test )->type == SPECIAL &&
                  strcmp( PeekCurrentToken( test )->tokenStr, "--" ) == 0 ) ) {
        GetCurrentToken( test ) ;
        return true ;
      } // else if
      else
        return false ;
    } // else if

    return true ;
  } // if
  else if ( PeekCurrentToken( test )->type == CONSTANT ) {
    floatvalue = atof( GetCurrentToken( test )->tokenStr ) ;
    isexp = true ;
    return true ;
  } // else if
  else if ( PeekCurrentToken( test )->type == SPECIAL &&
            strcmp( PeekCurrentToken( test )->tokenStr, "(" ) == 0 ) {
    GetCurrentToken( test ) ;
    if ( Expression( test, isfloat, floatvalue, stringvalue, boolvalue, iscout,
                     isbexp, isexp ) ) {
      if ( PeekCurrentToken( test )->type == SPECIAL &&
           strcmp( PeekCurrentToken( test )->tokenStr, ")" ) == 0 ) {
        GetCurrentToken( test ) ;
        return true ;
      } // if
      else
        return false ;
    } // if
    else
      return false ;
  } // else if
  else
    return false ;
} // Unsigned_unary_exp()
