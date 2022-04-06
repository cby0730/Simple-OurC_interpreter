# include <stdio.h>
# include <iostream>
# include <vector>
# include <string.h>
# include <string>
# include <sstream>
# include <cctype>
# include <iomanip>
# include <cmath>

using namespace std ;

int gline = 1 ;

struct ID
{
  string tokenName ;
  string tokenValue ;
} ;

class Lexical_error : public exception{
    
public:
    
  Lexical_error( string error_str )
  {
    cout << "> Unrecognized token with first char : '" << error_str << "'" << endl;
    while ( cin.peek() != '\n' ) cin.get() ;
    gline ++ ;
    
  } // Lexical_error()
    
} ; // Lexical_error

class Syntax_error : public exception{
    
public:
    
  Syntax_error( string error_str, bool need_getline )
  {
        
    cout << "> Unexpected token : '" << error_str << "'" << endl;
    while ( cin.peek() != '\n' ) cin.get() ;
    gline ++ ;
        
  } // Syntax_error()
    
} ; // Syntax_error

class Defined_error : public exception{
    
public:
    
  Defined_error( string error_str )
  {
        
    cout << "> Undefined identifier : '" << error_str << "'" << endl;
    while ( cin.peek() != '\n' ) cin.get() ;
    gline ++ ;
        
  } // Defined_error()
    
} ; // Defined_error

class Error : public exception {
  
public:
  
  Error( ) {
    
    cout << "> Error" << endl;
    while ( cin.peek() != '\n' ) cin.get() ;
    gline ++ ;
    
  } // Error()
  
} ;

class Input{

private:
    
  void Skip_white_space()
  {
        
    while ( cin.peek() == '\n' || cin.peek() == '\t' || cin.peek() == ' ' )
      if ( cin.get() == '\n' ) gline ++ ;
        
  } // Skip_white_space()
    
  bool Is_number_correct( string& str )
  {
    
    int dot_num = 0 ;
    string error_str ;
        
    for ( int i = 0; i < str.length() ; i ++ )
    { // check wether number has more than one dot
            
      if ( str[i] == '.' ) dot_num ++ ;
            
      if ( dot_num == 2 && str[i] == '.'  )
      {
        do {
          error_str += str[i] ;
          i ++ ;
        } while ( isdigit( str[i] ) ) ;
                
      } // if
            
    } // for
        
    if ( dot_num > 1 )
    {
      str = error_str ;
      return false ;
            
    } // if
    else return true ;
        
  } // Is_number_correct()
  
public:
  
  char GetChar( ) {
    
    char ch, trash[200] ;
    
    Skip_white_space() ;
    ch = cin.get() ;
    
    if ( ch == '/' && cin.peek() == '/' ) {
      
      cin.getline( trash, 200 ) ;
      gline ++ ;
      return GetChar() ;
      
    } //
    else return ch ;
    
  } // GetChar()
  
  char PeekChar() {
    
    char ch ;
    
    ch = GetChar() ;
    cin.putback( ch ) ;
    return ch ;
    
  } // PeekChar()
    
  void GetToken( string& tokenType, string& tokenValue ) {  // only get one token
                        // peek first, actually get the char if it is ok
        
    string input_token = "" ;
    bool is_error = false ;
    Skip_white_space() ; // skip white space at first
    char trash[200] ; // put the comment line in this (e.g //this is a comment)
        
    if ( cin.peek() == EOF )
    {
            
      tokenType = "EOF" ;
      tokenValue = EOF ;
      return;
            
    } // if
    else if ( isalpha( cin.peek() ) ) { // this token is id-type
            
      input_token += cin.get() ;
            
      while ( isdigit( cin.peek() ) || isalpha( cin.peek() ) || cin.peek() == '_' )
        input_token += cin.get() ; // only number, alpha and _ can be behind the id-type
            
      tokenType = "id" ;
            
    } // else if
    else if ( isdigit( cin.peek() ) ) { // this token is number-type
            
      input_token += cin.get() ;
            
      while ( isdigit( cin.peek() ) || cin.peek() == '.' )
        input_token += cin.get() ; // only number and . can be behind the number-type
            
      if ( Is_number_correct( input_token ) ) tokenType = "number" ; // the number is correct
      else throw Syntax_error( input_token, true ) ; // if happen 3.4.5
            
    } // else if
    else if ( ispunct( cin.peek() ) ) {
            // this token is special-type, someone need to check next char, if don't just return
            
      tokenType = "special" ;
      input_token += cin.get() ;
            
      if ( input_token == ":" ) { // :
                
        if ( cin.peek() == '=' ) input_token += cin.get() ; // :=
        else throw Lexical_error( input_token ) ;
                
      } // if
      else if ( input_token == "<" ) { // <
                
        if ( cin.peek() == '=' ) input_token += cin.get() ; // <=
        else if ( cin.peek() == '>' ) input_token += cin.get() ; // <>
                
      } // else if
      else if ( input_token == ">" ) { // >
        
        if ( cin.peek() == '=' ) input_token += cin.get() ; // >=
                
      } // else if
      else if ( input_token == "/" ) { // /
                
        if ( cin.peek() == '/' ) { // //
                    
          cin.getline( trash, 200 ) ;
          gline ++ ;
          return GetToken( tokenType, tokenValue ) ;
                    
        } // if
                
      } // else if
      else if ( input_token == "." ) { // .
                
        if ( isdigit( cin.peek() ) ) { // peek is digit behind .
                    
          while ( isdigit( cin.peek() ) || cin.peek() == '.' ) // get every number behind the .
            input_token += cin.get() ;
                    
          if ( Is_number_correct( input_token ) ) tokenType = "number" ; // the number is correct
          else throw Syntax_error( input_token, true ) ; // if happened .3.4.5
          
        } // if
        else is_error = true ; // . can't be alone
        if ( is_error ) throw Lexical_error( input_token ) ;
                
      } // else if
      else if ( input_token == ";" || input_token == "(" || input_token == ")" || input_token == "="
                || input_token == "+" || input_token == "-" || input_token == "*" )
        tokenType = "special" ; // this kinds of special char don't need to check next char
      
      else throw Lexical_error( input_token ) ;
      
    } // else if
    else is_error = true ; // if this char don't belong to above, then this is a wrong char
    if ( is_error ) { // 夏氏 Style
      
      input_token += cin.get() ;
      throw Lexical_error( input_token ) ;
      
    } // if
    
    tokenValue = input_token ;
        
  } // GetToken()
    
  void PeekToken( string& tokenType, string& tokenValue )
  { // peek the next token
        
    GetToken( tokenType, tokenValue ) ;
    for ( int i = tokenValue.length() - 1 ; i >= 0 ; i -- ) // put every char back
      cin.putback( tokenValue[i] ) ;
        
  } // PeekToken()

} ; // end class Input

class Parser {
  
# define add_sub( operator, op1, op2 ) \
( operator == '+' ) ? ( op1 + Stof( op2 ) ) : ( op1 - Stof( op2 ) )
  
# define mul_div( operator, op1, op2 ) \
( operator == '*' ) ? \
  ( op1 * Stof( op2 ) ) : ( Stof( op2 ) == 0 ) ? throw Error() : op1 / Stof( op2 )
  
# define Ftos_Itos( is_float, value ) is_float ? Ftos( value, 9 ) : Itos( value )
  
private:
  Input mdata ;
  vector<ID> mid_list ;
  
public:
  string Ftos( float value, int position )
  {
    stringstream temp_str ;
    temp_str << fixed << setprecision( position ) << value ;
    return temp_str.str() ;
  } // Ftos()
  
  string Itos( int value )
  {
    stringstream temp_str ;
    temp_str << value ;
    return temp_str.str() ;
  } // Itos()
  
  float Stof( string value )
  {
    float result ;
    stringstream ss( value ) ;
    ss >> result ;
    return result ;
  } // Stof()
    
  bool Command( string& value )
  {     // <Command> ::= IDENT ( ':=' <ArithExp> | <IDlessArithExpOrBexp> ) ';'
        //               | <NOT_IDStartArithExpOrBexp> ';' | QUIT
        
    string tokenType, tokenValue, equal_tokenType ;
    string equal_tokenValue ;
    string last_tokenType, last_tokenValue, cal_or_bool ;
    string arithexp_tokenValue, idless_tokenValue, not_idstart_tokenValue ;
    bool arithexp_correct, idless_correct, not_idstart_correct ;
    bool is_error = false ;
    stringstream temp_str ;
      
    mdata.PeekToken( tokenType, tokenValue ) ;
    if ( tokenValue == "quit" ) { // if first token is quit
          
      mdata.GetToken( tokenType, tokenValue ) ;
      cout << "> Program exits..." << endl ;
      value = "quit" ;
      return true;
          
    } // if
    else if ( tokenType == "id" ) { // ID
      
      mdata.GetToken( tokenType, tokenValue ) ;
          
      mdata.PeekToken( equal_tokenType, equal_tokenValue ) ;
      if ( equal_tokenValue == ":=" ) { // :=
            
        mdata.GetToken( equal_tokenType, equal_tokenValue ) ;
        ArithExp( arithexp_correct, arithexp_tokenValue ) ;
        if ( arithexp_correct ) { // ArithExp
                
          mdata.PeekToken( last_tokenType, last_tokenValue ) ;
          if ( last_tokenValue == ";" ) { // ID := ArithExp ;
            
            value = arithexp_tokenValue ;
            Define_ID( tokenValue, arithexp_tokenValue ) ;
            mdata.GetToken( last_tokenType, last_tokenValue ) ; // get ";"
            return true;
                  
          } // if
          else throw Syntax_error( last_tokenValue, true ) ;
                
        } // if
        else is_error = true ;
        if ( is_error ) throw Syntax_error( arithexp_tokenValue, true ) ;
          // if there have a := but don't have a match ArithExp
            
      } // if
      else { // IDENT <IDlessArithExpOrBexp> ';'
        
        IDlessArithExpOrBexp( idless_correct, idless_tokenValue, tokenValue ) ;
        
        mdata.PeekToken( tokenType, tokenValue ) ;
        if ( tokenValue == ";" ) { // check the last token to see wether it is a ";"
                    
          mdata.GetToken( tokenType, tokenValue ) ;
          value = idless_tokenValue ;
          return true;
                    
        } // if
        else throw Syntax_error( tokenValue, true ) ;
        
      } // else
          
    } // if
    else { // <NOT_IDStartArithExpOrBexp> ';'
          
      NOT_ID_StartArithExpOrBexp( not_idstart_correct, not_idstart_tokenValue ) ;
      
      mdata.PeekToken( tokenType, tokenValue ) ;
      if ( tokenValue == ";" ) { // check the last token to see wether it is a ";"
              
        mdata.GetToken( tokenType, tokenValue ) ;
        value = not_idstart_tokenValue ;
        return true;
              
      } // if
      else throw Syntax_error( tokenValue, true ) ;
          
    } // else
    
    return false ;
      
  } // Command()
    
  void IDlessArithExpOrBexp( bool& correct, string& value, string ID_token ) {
    // <IDlessArithExpOrBexp> ::= { '+' <Term> | '-' <Term> | '*' <Factor> | '/' <Factor> }
    //                             [ <BooleanOperator> <ArithExp> ]

    string tokenType, tokenValue, booloperater_tokenValue ;
    string term_tokenValue, factor_tokenValue, arithexp_tokenValue ;
    string id_tokenValue ;
    char op ;
    float tolerance = 0.0001, ans = 0.0 ;
    bool booloperator_correct, term_correct ;
    bool factor_correct, arithexp_correct ;
    bool is_idinlist, is_float ;
        
    mdata.PeekToken( tokenType, tokenValue ) ; // need a delimiter behind ID
    if ( tokenType != "special" ) throw Syntax_error( tokenValue, true ) ;
    
    Is_IDinList( is_idinlist, id_tokenValue, ID_token ) ; // check is ID in the List
    if ( !is_idinlist ) throw Defined_error( ID_token ) ;
    
    is_float = id_tokenValue.find( '.' ) < id_tokenValue.size() ; // check is tokenValue float
    ans = Stof( id_tokenValue ) ; // let ans be first operand
    
    op = mdata.PeekChar() ;
    while ( op == '+' || op == '-' || op == '*' || op == '/' ) {
      // if there has a operator
      op = mdata.GetChar() ;
            
      if ( op == '+' || op == '-' ) { // if the operator is + or -
        
        Term( term_correct, term_tokenValue ) ;
        
        if ( term_tokenValue.find( '.' ) < term_tokenValue.size() ) is_float = true ;
        
        if ( term_correct ) ans = add_sub( op, ans, term_tokenValue ) ; // add or sub
        else throw Syntax_error( term_tokenValue, true ) ;
                
      } // if
      else if ( op == '*' || op == '/' ) { // if the operator is * or /
        
        Factor( factor_correct, factor_tokenValue ) ;
        
        if ( factor_tokenValue.find( '.' ) < factor_tokenValue.size() )
          is_float = true ;
        
        if ( factor_correct ) ans = mul_div( op, ans, factor_tokenValue ) ;
        else throw Syntax_error( factor_tokenValue, true ) ;
                
      } // else if
            
      op = mdata.PeekChar() ; // peek next token
            
    } // while
    
    id_tokenValue = Ftos_Itos( is_float, ans ) ;
        
    BooleanOprator( booloperator_correct, booloperater_tokenValue ) ;
    if ( !booloperator_correct ) { // if there is no boolean, only calculate
            
      correct = true ;
      value = id_tokenValue ;
      return ;
            
    } // if
    else { // if this is a boolean command
            
      ArithExp( arithexp_correct, arithexp_tokenValue ) ;
      if ( !arithexp_correct ) throw Syntax_error( arithexp_tokenValue, true ) ;
      // if there have a BoolOperater but don't have a match ArithExp
            
      correct = true ;
      
      float diff = abs( Stof( id_tokenValue ) - Stof( arithexp_tokenValue ) ) ;
            
      if ( booloperater_tokenValue == "<" ) {
                
        if ( diff < tolerance && diff != 0 ) value = "false" ;
        else if ( Stof( id_tokenValue ) < Stof( arithexp_tokenValue ) ) value = "true" ;
        else value = "false" ;
                
      } // if
      else if ( booloperater_tokenValue == ">" ) {
                
        if ( diff < tolerance && diff != 0 ) value = "false" ;
        else if ( Stof( id_tokenValue ) > Stof( arithexp_tokenValue ) ) value = "true" ;
        else value = "false" ;
                
      } // else if
      else if ( booloperater_tokenValue == "=" ) {
                
        if ( diff < tolerance && diff != 0 ) value = "true" ;
        else if ( Stof( id_tokenValue ) == Stof( arithexp_tokenValue ) ) value = "true" ;
        else value = "false" ;
                
      } // else if
      else if ( booloperater_tokenValue == ">=" ) {
                
        if ( diff < tolerance && diff != 0 ) value = "true" ;
        else if ( Stof( id_tokenValue ) >= Stof( arithexp_tokenValue ) ) value = "true" ;
        else value = "false" ;
                
      } // else if
      else if ( booloperater_tokenValue == "<=" ) {
              
        if ( diff < tolerance && diff != 0 ) value = "true" ;
        else if ( Stof( id_tokenValue ) <= Stof( arithexp_tokenValue ) ) value = "true" ;
        else value = "false" ;
                
      } // else if
      else if ( booloperater_tokenValue == "<>" ) {
              
        if ( diff < tolerance && diff != 0 ) value = "false" ;
        else if ( Stof( id_tokenValue ) != Stof( arithexp_tokenValue ) ) value = "true" ;
        else value = "false" ;
                
      } // else if
      else correct = false ;
            
    } // else
        
  } // IDlessArithExpOrBexp()
    
  void BooleanOprator( bool& correct, string& value ) {
      
    string tokenType, tokenValue ;
    mdata.PeekToken( tokenType, tokenValue ) ;
    
    if ( tokenValue == "=" || tokenValue == "<>" || tokenValue == ">" ||
         tokenValue == "<" || tokenValue == ">=" || tokenValue == "<=" ) {
      
      mdata.GetToken( tokenType, tokenValue ) ;
      
      correct = true ;
      value = tokenValue ;
      
    } // if
    else correct = false ;
    
  } // BooleanOprator()
  
  void NOT_ID_StartArithExpOrBexp( bool& correct, string& value ) {
    // <NOT_ID_StartArithExpOrBexp> ::= <NOT_ID_StartArithExp> [ <BooleanOperator> <ArithExp> ]

    string boolean_operator_tokenValue ;
    string not_id_startArithexp_tokenValue, arithexp_tokenValue ;
    float tolerance = 0.0001 ;
    bool not_id_startArithexp_correct, boolean_operator_correct, arithexp_correct ;
    
    Not_ID_startArithExp( not_id_startArithexp_correct, not_id_startArithexp_tokenValue ) ;
    if ( !not_id_startArithexp_correct ) throw Syntax_error( not_id_startArithexp_tokenValue, true ) ;
    
    BooleanOprator( boolean_operator_correct, boolean_operator_tokenValue ) ;
    if ( !boolean_operator_correct ) { // only has a number
      
      value = not_id_startArithexp_tokenValue ;
      correct = true ; // if only calculate
      return ;
      
    } // if
    
    ArithExp( arithexp_correct, arithexp_tokenValue ) ;
    if ( !arithexp_correct ) throw Syntax_error( arithexp_tokenValue, true ) ;
      // if there have a BoolOperater but don't have a match ArithExp
    
    correct = true ;
    
    float diff = abs( Stof( not_id_startArithexp_tokenValue ) - Stof( arithexp_tokenValue ) ) ;
    
    if ( boolean_operator_tokenValue == "<" ) {
      
      if ( diff < tolerance && diff != 0 ) value = "false" ;
      else if ( Stof( not_id_startArithexp_tokenValue ) < Stof( arithexp_tokenValue ) ) value = "true" ;
      else value = "false" ;
      
    } // if
    else if ( boolean_operator_tokenValue == ">" ) {
      
      if ( diff < tolerance && diff != 0 ) value = "false" ;
      else if ( Stof( not_id_startArithexp_tokenValue ) > Stof( arithexp_tokenValue ) ) value = "true" ;
      else value = "false" ;
      
    } // else if
    else if ( boolean_operator_tokenValue == "=" ) {
      
      if ( diff < tolerance && diff != 0 ) value = "true" ;
      else if ( Stof( not_id_startArithexp_tokenValue ) == Stof( arithexp_tokenValue ) ) value = "true" ;
      else value = "false" ;
      
    } // else if
    else if ( boolean_operator_tokenValue == ">=" ) {
      
      if ( diff < tolerance && diff != 0 ) value = "true" ;
      else if ( Stof( not_id_startArithexp_tokenValue ) >= Stof( arithexp_tokenValue ) ) value = "true" ;
      else value = "false" ;
      
    } // else if
    else if ( boolean_operator_tokenValue == "<=" ) {
      
      if ( diff < tolerance && diff != 0 ) value = "true" ;
      else if ( Stof( not_id_startArithexp_tokenValue ) <= Stof( arithexp_tokenValue ) ) value = "true" ;
      else value = "false" ;
      
    } // else if
    else if ( boolean_operator_tokenValue == "<>" ) {
      
      if ( diff < tolerance && diff != 0 ) value = "false" ;
      else if ( Stof( not_id_startArithexp_tokenValue ) != Stof( arithexp_tokenValue ) ) value = "true" ;
      else value = "false" ;
      
    } // else if
    else correct = false ;
    
  } // NOT_ID_StartArithExpOrBexp()
  
  void Not_ID_startArithExp( bool& correct, string& value ) {
    // <NOT_ID_StartArithExp> ::= <NOT_ID_StartTerm> { '+' <Term> | '-' <Term> }
    
    string tokenType, tokenValue ;
    string not_id_start_term_tokenValue, term_tokenValue ;
    char op ;
    float ans = 0.0 ;
    bool not_id_start_term_correct, term_correct ;
    bool is_float ;
    Not_ID_startTerm( not_id_start_term_correct, not_id_start_term_tokenValue ) ;
    
    is_float = not_id_start_term_tokenValue.find( '.' ) < not_id_start_term_tokenValue.size() ;
    ans = Stof( not_id_start_term_tokenValue ) ; // let ans be the first operand
    
    if ( not_id_start_term_correct ) {
      
      do {
        
        op = mdata.PeekChar() ;
        if ( ( op == '+' || op == '-' ) && op != EOF ) {
          
          op = mdata.GetChar() ;
          Term( term_correct, term_tokenValue ) ;
          
          if ( term_tokenValue.find( '.' ) < term_tokenValue.size() ) is_float = true ;
          
          if ( term_correct ) ans = add_sub( op, ans, term_tokenValue ) ; // add or sub
          else throw Syntax_error( term_tokenValue, true ) ;
          // if there have a + or - but don't have a match Term
          
        } // if
        else {
          
          correct = true ;
          value = Ftos_Itos( is_float, ans ) ;
          return ;
          
        } // else
        
      } while ( true ) ;
      
    } // if
    else throw Syntax_error( not_id_start_term_tokenValue, true ) ;
    
  } // Not_ID_startArithExp()
  
  void Not_ID_startTerm( bool& correct, string& value ) {
    // <NOT_ID_StartTerm> ::= <NOT_ID_StartFactor> { '*' <Factor> | '/' <Factor> }
    
    string tokenType, tokenValue ;
    string not_id_start_factor_tokenValue, factor_tokenValue ;
    char op ;
    float ans = 0.0 ;
    bool not_id_start_factor_correct, factor_correct ;
    bool is_float ;
    Not_ID_startFactor( not_id_start_factor_correct, not_id_start_factor_tokenValue ) ;
    
    is_float = not_id_start_factor_tokenValue.find( '.' ) < not_id_start_factor_tokenValue.size() ;
    ans = Stof( not_id_start_factor_tokenValue ) ;
    
    if ( not_id_start_factor_correct ) {
      
      do {
        
        op = mdata.PeekChar() ;
        if ( ( op == '*' || op == '/' ) && op != EOF ) {
          
          op = mdata.GetChar() ;
          Factor( factor_correct, factor_tokenValue ) ;
          
          if ( factor_tokenValue.find( '.' ) < factor_tokenValue.size() )
            is_float = true ;
          
          if ( factor_correct ) ans = mul_div( op, ans, factor_tokenValue ) ;
          else throw Syntax_error( factor_tokenValue, true ) ;
          
        } // if
        else {
          
          correct = true ;
          value = Ftos_Itos( is_float, ans ) ;
          return ;
          
        } // else
        
      } while ( true ) ;
      
    } // if
    else throw Syntax_error( not_id_start_factor_tokenValue, true ) ;
    
  } // Not_ID_startTerm()
  
  void Not_ID_startFactor( bool& correct, string& value ) {
    // <NOT_ID_StartFactor> ::= [ SIGN ] NUM | '(' <ArithExp> ')'
    
    string tokenType, tokenValue ;
    string signed_tokenType, signed_tokenValue ;
    string arithexp_tokenValue ;
    bool is_correct = false ;
    
    mdata.GetToken( tokenType, tokenValue ) ;
    
    if ( tokenType != "number" && tokenType != "special" )
      throw Syntax_error( tokenValue, true ) ;
    
    if ( tokenType == "number" ) {
      
      correct = true ;
      value = tokenValue ;
      return ;
      
    } // if
    else if ( tokenValue == "+" || tokenValue == "-" ) {
      
      mdata.GetToken( signed_tokenType, signed_tokenValue ) ;
      if ( signed_tokenType == "number" ) {
        
        correct = true ;
        value = tokenValue + signed_tokenValue ;
        return ;
        
      } // if
      else throw Syntax_error( signed_tokenValue, true ) ;
        // if there have a sign but don't have a match number
      
    } // else if
    else if ( tokenValue == "(" ) {
      
      ArithExp( correct, arithexp_tokenValue ) ;
      if ( correct ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenValue == ")" ) {
          
          correct = true ;
          value = arithexp_tokenValue ;
          return ;
          
        } // if
        else throw Syntax_error( tokenValue, true ) ;
          // if there have a ArithExp but don't have a match ")"
         
      } // if
      
      else is_correct = true ;
      if ( is_correct )  throw Syntax_error( arithexp_tokenValue, true ) ;
        // if there have a "(" but don't have a match ArithExp
      
    } // else if
    
    else is_correct = true ;
    if ( is_correct ) throw Syntax_error( tokenValue, true ) ;
    
  } // Not_ID_startFactor()
  
  void ArithExp( bool& correct, string& value ) { // <ArithExp> ::= <Term> { '+' <Term> | '-' <Term> }
    
    string tokenType, tokenValue ;
    string term1_tokenValue, term2_tokenValue ;
    char op ;
    float ans = 0.0 ;
    bool term1_correct, term2_correct ;
    bool is_correct = false, is_float ;
    Term( term1_correct, term1_tokenValue ) ;
    
    is_float = term1_tokenValue.find( '.' ) < term1_tokenValue.size() ;
    ans = Stof( term1_tokenValue ) ;
    
    if ( term1_correct ) {
      
      do {
        
        op = mdata.PeekChar() ;
        if ( ( op == '+' || op == '-' ) && op != EOF ) {
          
          op = mdata.GetChar() ;
          Term( term2_correct, term2_tokenValue ) ;
          
          if ( term2_tokenValue.find( '.' ) < term2_tokenValue.size() )
            is_float = true ;
          
          if ( term2_correct ) ans = add_sub( op, ans, term2_tokenValue ) ; // add or sub
          else throw Syntax_error( term2_tokenValue, true ) ; // have a + or - but don't have Term
          
        } // if
        else {
          
          correct = true ;
          value = Ftos_Itos( is_float, ans ) ;
          return ;
          
        } // else
        
      } while ( true ) ;
      
    } // if
    else is_correct = true ;
    if ( is_correct ) throw Syntax_error( term1_tokenValue, true ) ;
    
  } // ArithExp()
  
  void Term( bool& correct, string& value ) { // <Term> ::= <Factor> { '*' <Factor> | '/' <Factor> }

    
    string tokenType, tokenValue ;
    string factor1_tokenValue, factor2_tokenValue ;
    char op ;
    float ans = 0.0 ;
    bool factor1_bool, factor2_bool ;
    bool is_correct = false, is_float ;
    Factor( factor1_bool, factor1_tokenValue ) ;
    
    is_float = factor1_tokenValue.find( '.' ) < factor1_tokenValue.size() ;
    ans = Stof( factor1_tokenValue ) ;
    
    if ( factor1_bool ) {
      
      do {
        
        op = mdata.PeekChar() ;
        
        if ( ( op == '*' || op == '/' ) && op != EOF ) {
          // next token is * or / meanwhile, next token can't be eof
          op = mdata.GetChar() ;
          Factor( factor2_bool, factor2_tokenValue ) ;
          
          if ( factor2_tokenValue.find( '.' ) < factor2_tokenValue.size() )
            is_float = true ;
          
          if ( factor2_bool ) ans = mul_div( op, ans, factor2_tokenValue ) ; // TODO
          else is_correct = true ;
          if ( is_correct )throw Syntax_error( factor2_tokenValue, true ) ;
          
        } // if
        else {
          
          correct = true ;
          value = Ftos_Itos( is_float, ans ) ;
          return ;
          
        } // else
        
      } while ( true ) ;
      
    } // if
    else is_correct = true ;
    if ( is_correct ) throw Syntax_error( factor1_tokenValue, true ) ;
    
  } // Term()
  
  void Factor( bool& correct, string& value ) { // <Factor> ::= [ SIGN ] NUM | IDENT | '(' <ArithExp> ')'
    
    string tokenType, tokenValue ;
    string signed_tokenType, signed_tokenValue ;
    string arithexp_tokenValue, idinlist_tokenValue ;
    bool idinlist_correct ;
    bool is_correct = false ;
    
    mdata.GetToken( tokenType, tokenValue ) ;
    if ( tokenType != "number" && tokenType != "id" && tokenType != "special" )
      throw Syntax_error( tokenValue, true ) ;
    
    if ( tokenType == "number" ) {
      
      correct = true ;
      value = tokenValue ;
      return ;
      
    } // if
    else if ( tokenType == "id" ) {
      
      Is_IDinList( idinlist_correct, idinlist_tokenValue, tokenValue ) ;
      if ( idinlist_correct ) { // check if the ID has been declared
        
        correct = true ;
        value = idinlist_tokenValue ;
        return ;
        
      } // else if
      else throw Defined_error( tokenValue ) ;
      
    } // else if
    else if ( tokenValue == "+" || tokenValue == "-" ) {
      
      mdata.GetToken( signed_tokenType, signed_tokenValue ) ;
      if ( signed_tokenType == "number" ) {
        
        correct = true ;
        value = tokenValue + signed_tokenValue ;
        return ;
        
      } // if
      else throw Syntax_error( signed_tokenValue, true ) ;
        // if there have a sign but don't have a match number
      
    } // else if
    else if ( tokenValue == "(" ) {
      
      ArithExp( correct, arithexp_tokenValue ) ;
      if ( correct ) {
        
        mdata.GetToken( tokenType, tokenValue ) ;
        if ( tokenValue == ")" ) {
          
          correct = true ;
          value = arithexp_tokenValue ;
          return ;
          
        } // if
        
        else is_correct = true ;
        if ( is_correct ) throw Syntax_error( tokenValue, true ) ;
          // if there have a ArithExp but don't have a match ")"
        
      } // if
      
      else is_correct = true ;
      if ( is_correct ) throw Syntax_error( arithexp_tokenValue, true ) ;
        // if there have a "(" but don't have a match ArithExp
      
    } // else if
    
    else is_correct = true ;
    if ( is_correct ) throw Syntax_error( tokenValue, true ) ;
      // if this token don't belong to above, then it is Unexpected token
    
  } // Factor()
  
  void Is_IDinList( bool& correct, string& value, string idName ) {
    
    for ( int i = 0, len = mid_list.size() ; i < len ; i ++ ) {
      
      if ( mid_list.at( i ).tokenName == idName )
      {
        
        correct = true ;
        value = mid_list.at( i ).tokenValue ;
        return ;
        
      } // if
      
    } // for
    
    correct = false ;
    
  } // Is_IDinList()
  
  void Define_ID( string idName, string idValue ) {
    
    ID temp ;
    bool is_inList = false ;
    string trash ;
    
    Is_IDinList( is_inList, trash, idName ) ;
    if ( !is_inList ) { // define a new ID
      
      temp.tokenName = idName ;
      temp.tokenValue = idValue ;
      mid_list.push_back( temp ) ; // put the ID into vector
      
    } // if
    else { // change an old ID's value
      
      for ( int i = 0, len = mid_list.size() ; i < len ; i ++ ) {
        
        if ( mid_list.at( i ).tokenName == idName ) {
          
          mid_list.at( i ).tokenValue = idValue ;
          return ;
          
        } // if
        
      } // for
      
    } // else
    
  } // Define_ID()
  
} ;

int main() {
  
  Parser t ;
  
  char trash[200] ;
  string result ;
  cin.getline( trash, 200 ) ;
  
  cout << "Program starts..." << endl ;
  
  do {
      
    try {
        
      if ( t.Command( result ) && result != "quit" ) {
        
        if ( result == "true" || result == "false" ) cout <<  "> " << result << endl ;
        else {
          
          if ( result.find( '.' ) < result.size() ) printf( "> %1.3f\n", t.Stof( result ) ) ;
          else cout <<  "> " << result << endl ;
          
        } // else
        
      } // if
        
    }  // try
    catch ( Lexical_error& e ) {
    } // catch
    catch ( Syntax_error& e ) {
    }  // catch
    catch( Defined_error& e ) {
    } // catch
    catch( Error& e ) {
    } // catch
      
  } while ( result != "quit" ) ;
  
} // main()
