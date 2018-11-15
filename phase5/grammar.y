%{
#define YYDEBUG 1
#include <algorithm>
#include <functional>
#include <iostream>
#include <iomanip>
#include <stack>
#include "ast.h"
#include "tc.h"
#include "SymTab.h"
using namespace std;

extern void print_idents();
extern unsigned int lexerrs;
extern bool dump_classes;

namespace{
  void prologue(){
    cout <<
      " .text\n"
      " .globl  main\n"
      "main:\n"
      " pushl %ebp\n"
      " movl  %esp, %ebp\n"
      ;
  }

  void epilogue(){
    cout <<
      "# end main\n"
      " movl  $0, %eax\n"
      ".main_exit:\n"
      " popl  %ebp\n"
      " ret\n"
      ;
  }

  void analyze_decl(CDecl *d){
    try{
      d->analyze();
    }catch(SyntaxError& e){
      //cerr << "bloop\n";
      // do nothing
    }
  }

  void compile(Node *n, list<CDecl*> *a=0, list<CDecl*> *b=0){
    if(dump_before){
      cerr << '\n';
      if(a) for_each(a->begin(),a->end(), mem_fun(&CDecl::dump));
      if(b) for_each(b->begin(),b->end(), mem_fun(&CDecl::dump));
      n->dump();
    }
    if(a) for_each(a->begin(),a->end(), analyze_decl);
    if(b) for_each(b->begin(),b->end(), analyze_decl);
    n->analyze();
    if(dump_classes)
      print_classes();
    if(dump_after){
      cerr << '\n';
      if(a) for_each(a->begin(),a->end(), mem_fun(&CDecl::dump));
      if(b) for_each(b->begin(),b->end(), mem_fun(&CDecl::dump));
      n->dump();
    }
//    print_idents();
    if(Node::errors() == 0 && lexerrs == 0){
      prologue();
      n->encode();
      epilogue();
      Type::object()->encode();
      Type::encode_array();
      if(a) for_each(a->begin(), a->end(), mem_fun(&CDecl::encode));
      if(b) for_each(b->begin(), b->end(), mem_fun(&CDecl::encode));
    }else{
      int errs = Node::errors() + lexerrs;
      cerr << errs << " Error" << (errs == 1? "" : "s")
        << " found - code will not be generated\n";
      exit(1);
    }
  }
}

extern int yylex(void), srcline;

void nonfatal(const char *s){
  ++lexerrs;
  cout.flush();
  cerr << "Error on line " << srcline << ":\n " << s << "\n";
}

void yyerror(const char *s){
  nonfatal(s);
  exit(1);
}

std::stack<Ident> idents;
inline Ident next_ident(){
  Ident i = idents.top();
  idents.pop();
  return i;
}

%}

%union{
  unsigned int n;
  char *cstr;
  Node *node;
  Stat *stat;
  Expr *expr;
  Type *type;
  VDecl *decl;
  list<VDecl*> *decls;
  list<Stat*> *stats;
  CDecl *cdecl;
  list<CDecl*> *cdecls;
  CBDecl *cbdecl;
  list<CBDecl*> *cbdecls;
  list<Expr*> *args;
  Block *body;
  list<Param*> *params;
  Param *param;
}

%token <cstr> IDENTIFIER
%token <n> INTEGER_LITERAL
%token NULL_LITERAL
%token EQ_OP
%token INT
%token MAIN CLASS EXTENDS
%token THIS SUPER
%token IF ELSE WHILE NEW DELETE RETURN BREAK CONTINUE
%token OUT
%token BAD

/* Phase 1 */
%type <node> CompilationUnit MainFunctionDeclaration MainFunctionBody MainBlock
%type <decls> VariableDeclarators
%type <decl> VariableDeclarator
%type <cstr> Identifier
%type <stat> BlockStatement Statement MainBlockStatement Block
%type <stats> MainBlockStatements BlockStatements
%type <stat> MainVariableDeclarationStatement MainVariableDeclaration EmptyStatement
%type <type> Type PrimitiveType NumericType IntegralType
%type <stat> ExpressionStatement OutputStatement
%type <expr> ParenExpression Expression StatementExpression AssignmentExpression Assignment
%type <expr> EqualityExpression LeftHandSide RelationalExpression AdditiveExpression
%type <expr> MultiplicativeExpression UnaryExpression CastExpression Primary PrimaryNoNewArray
%type <expr> Literal
/* Phase 2 */
%type <stat> IfThenElseStatement WhileStatement BreakStatement ContinueStatement ReturnStatement
/* Phase 3 */
%type <cdecl> ClassDeclaration
%type <cdecls> ClassDeclarations
%type <cbdecls> ClassBody ClassBodyDeclarations
%type <cbdecl> ClassBodyDeclaration ClassMemberDeclaration FieldDeclaration
%type <expr> FieldAccess ClassInstanceCreationExpression
%type <stat> DeleteStatement
%type <type> ClassType ReferenceType
%type <args> Arguments ArgumentList
/* Phase 4 */
%type <cbdecl> DestructorDeclaration ConstructorDeclaration MethodDeclaration
%type <cstr> DestructorDeclarator
%type <body> DestructorBody ConstructorBody MethodBody
%type <params> FormalParameters FormalParameterList
%type <param> FormalParameter
/*%type <cstr> VariableDeclaratorID*/
%type <expr> MethodInvocation
%type <stats> ConstructorInvoked
%type <stat> ConstructorInvocation
/* Phase 5 */
%type <n> Dimension Dimensions
%type <expr> DimensionExpression ArrayAccess ArrayCreationExpression
%type <args> DimensionExpressions
%type <type> ArrayType

%start CompilationUnit

%%

CompilationUnit
  : /* empty */                               { yyerror("All T programs require a main() function"); }
  | MainFunctionDeclaration                   { compile($1); }
  | MainFunctionDeclaration ClassDeclarations { compile($1,$2); }
  | ClassDeclarations MainFunctionDeclaration { compile($2,$1); }
  | ClassDeclarations MainFunctionDeclaration ClassDeclarations { compile($2,$1,$3); }
  ;

MainFunctionDeclaration
  : INT MAIN '(' ')' MainFunctionBody { $$ = $5; }
  ;

MainFunctionBody
  : MainBlock { $$ = $1; }
  ;

ClassDeclarations
  : ClassDeclarations ClassDeclaration  { ($$ = $1)->push_back($2); }
  | ClassDeclaration                    { ($$ = new list<CDecl*>())->push_back($1); }
  ;

ClassDeclaration
  : CLASS Identifier ClassBody                    { $$ = new CDecl(next_ident(),Type::object(),$3); }
  | CLASS Identifier EXTENDS ClassType ClassBody  { $$ = new CDecl(next_ident(),$4,$5); }
  | CLASS Identifier EXTENDS INT ClassBody        { yyerror("Cannot inherit from int"); }
  | CLASS Identifier EXTENDS Literal ClassBody    { yyerror("Literal found where class type expected"); }
  ;

ClassBody
  : '{' ClassBodyDeclarations '}' { $$ = $2; }
  | '{' '}'                       { $$ = new list<CBDecl*>(); }
  ;

ClassBodyDeclarations
  : ClassBodyDeclarations ClassBodyDeclaration  { ($$ = $1)->push_back($2); }
  | ClassBodyDeclaration                        { ($$ = new list<CBDecl*>())->push_back($1); }
  ;

ClassBodyDeclaration
  : ClassMemberDeclaration  { $$ = $1; }
  | ConstructorDeclaration  { $$ = $1; }
  | DestructorDeclaration   { $$ = $1; }
  | ';'                     { $$ = empty_decl(); }
  ;

ClassMemberDeclaration
  : FieldDeclaration  { $$ = $1; }
  | MethodDeclaration { $$ = $1; }
  ;

FieldDeclaration
  : Type VariableDeclarators ';'  { $$ = new FDecl($1,$2); }
  ;

VariableDeclarators
  : VariableDeclarators ',' VariableDeclarator  { ($$ = $1)->push_back($3); }
  | VariableDeclarators ','                     { yyerror("Missing declarator"); }
  | VariableDeclarator                          { ($$ = new list<VDecl*>())->push_back($1); }
  ;

VariableDeclarator
  : Identifier Dimensions  { $$ = new VDecl(next_ident(), $2); }
  | Identifier             { $$ = new VDecl(next_ident(), 0); }
  | Literal                { yyerror("Literal found where an identifier is expected"); }
  ;

MethodDeclaration
  : Type Identifier FormalParameters MethodBody { $$ = new Meth($1,next_ident(),$3,$4); }
  | Type Identifier FormalParameters Dimensions MethodBody
      { $$ = new Meth(Type::array($1,$4),next_ident(),$3,$5); }
  ;

MethodBody
  : Block { $$ = dynamic_cast<Block*>($1); }
  ;

ConstructorDeclaration
  : Type FormalParameters ConstructorBody { $$ = new Ctor($1,$2,$3); }
  ;

ConstructorBody
  : ConstructorInvoked  { $$ = new Block($1); }
  | Block               { $$ = dynamic_cast<Block*>($1); }
  ;

ConstructorInvoked
  : '{' ConstructorInvocation BlockStatements '}' { ($$ = $3)->push_front($2); }
  | '{' ConstructorInvocation '}'                 { ($$ = new list<Stat*>())->push_front($2); }
  ;

ConstructorInvocation
  : THIS Arguments ';'  { $$ = new CInvoke(CInvoke::This,$2); }
  | SUPER Arguments ';' { $$ = new CInvoke(CInvoke::Super,$2); }
  ;

DestructorDeclaration
  : DestructorDeclarator DestructorBody { $$ = new Dest(next_ident(),$2); }
  ;

DestructorDeclarator
  : '~' Identifier '(' ')'  { $$ = $2; }
  ;

DestructorBody
  : Block { $$ = dynamic_cast<Block*>($1); }
  ;

FormalParameters
  : '(' FormalParameterList ')' { $$ = $2; }
  | '(' ')'                     { $$ = new list<Param*>(); }
  ;

FormalParameterList
  : FormalParameterList ',' FormalParameter { ($$ = $1)->push_back($3); }
  | FormalParameter                         { ($$ = new list<Param*>())->push_back($1); }
  ;

FormalParameter
  : Type Identifier Dimensions  { $$ = new Param($1,next_ident(),$3); }
  | Type Identifier             { $$ = new Param($1,next_ident(), 0); }
  ;

Block
  : '{' BlockStatements '}' { $$ = new Block($2); }
  | '{' '}'                 { $$ = new Block(new list<Stat*>()); }
  ;

BlockStatements
  : BlockStatements BlockStatement  { ($$ = $1)->push_back($2); }
  | BlockStatement                  { ($$ = new list<Stat*>())->push_back($1); }
  ;

BlockStatement
  : Statement { $$ = $1; }
  ;

MainBlock
  : '{' MainBlockStatements '}' { $$ = new Block($2); }
  | '{' '}'                     { $$ = new Block(new list<Stat*>()); }
  ;

MainBlockStatements
  : MainBlockStatements MainBlockStatement  { ($$ = $1)->push_back($2); }
  | MainBlockStatement                      { ($$ = new list<Stat*>())->push_back($1); }
  ;

MainBlockStatement
  : MainVariableDeclarationStatement  { $$ = $1; }
  | BlockStatement                    { $$ = $1; }
  ;

MainVariableDeclarationStatement
  : MainVariableDeclaration ';' { $$ = $1; }
  ;

MainVariableDeclaration
  : Type VariableDeclarators  { $$ = new VDeclStat($1,$2); }
  ;

Statement
  : Block               { $$ = $1; }
  | EmptyStatement      { $$ = $1; }
  | ExpressionStatement { $$ = $1; }
  | IfThenElseStatement { $$ = $1; }
  | WhileStatement      { $$ = $1; }
  | ReturnStatement     { $$ = $1; }
  | DeleteStatement     { $$ = $1; }
  | OutputStatement     { $$ = $1; }
  | BreakStatement      { $$ = $1; }
  | ContinueStatement   { $$ = $1; }
  ;

IfThenElseStatement
  : IF ParenExpression Statement ELSE Statement { $$ = new IfElse($2,$3,$5); }
  ;

WhileStatement
  : WHILE ParenExpression Statement { $$ = new While($2,$3); }
  ;

ReturnStatement
  : RETURN Expression ';' { $$ = new Return($2); }
  | RETURN ';'            { $$ = new Return(0); }
  ;

DeleteStatement
  : DELETE Expression ';' { $$ = new Del($2); }
  ;

OutputStatement
  : OUT Expression ';'  { $$ = new Out($2); }
  ;

BreakStatement
  : BREAK ';' { $$ = new Break(); }
  ;

ContinueStatement
  : CONTINUE ';'   { $$ = new Continue(); }
  ;

EmptyStatement
  : ';' { $$ = empty_stat(); }
  ;

ParenExpression
  : '(' Expression ')'  { $$ = $2; }
  ;

ExpressionStatement
  : StatementExpression ';' { $$ = new ExprStat($1); }
  | MethodInvocation ';'    { $$ = new ExprStat($1); }
  ;

StatementExpression
  : Assignment  { $$ = $1; }
  ;

Expression
  : AssignmentExpression  { $$ = $1; }
  ;

AssignmentExpression
  : Assignment          { $$ = $1; }
  | EqualityExpression  { $$ = $1; }
  ;

Assignment
  : LeftHandSide '=' AssignmentExpression { $$ = new Ass($1,$3); }
  ;

LeftHandSide
  : Identifier        { $$ = new Var(next_ident()); }
  | FieldAccess       { $$ = $1; }
  | ArrayAccess       { $$ = $1; }
  | Literal           { yyerror("Left-hand side is not assignable"); }
  | MethodInvocation  { yyerror("Left-hand side is not assignable"); }
  ;

EqualityExpression
  : EqualityExpression EQ_OP RelationalExpression { $$ = new Equals($1,$3); }
  | RelationalExpression                          { $$ = $1; }
  ;

RelationalExpression
  : RelationalExpression '<' AdditiveExpression { $$ = new Less($1,$3); }
  | RelationalExpression '>' AdditiveExpression { $$ = new Greater($1,$3); }
  | AdditiveExpression                          { $$ = $1; }
  ;

AdditiveExpression
  : AdditiveExpression '+' MultiplicativeExpression { $$ = new Add($1,$3); }
  | AdditiveExpression '-' MultiplicativeExpression { $$ = new Sub($1,$3); }
  | MultiplicativeExpression                        { $$ = $1; }
  ;

MultiplicativeExpression
  : MultiplicativeExpression '*' UnaryExpression    { $$ = new Mul($1,$3); }
  | MultiplicativeExpression '/' UnaryExpression    { $$ = new Div($1,$3); }
  | UnaryExpression                                 { $$ = $1; }
  ;

UnaryExpression
  : '-' UnaryExpression { $$ = new Neg($2); }
  | '!' UnaryExpression { $$ = new Not($2); }
  | CastExpression      { $$ = $1; }
  ;

CastExpression
  : ParenExpression CastExpression { $$ = new Cast($1,$2); }
  | '(' ArrayType ')' CastExpression { $$ = new CastA($2,$4); }
  | Primary { $$ = $1; }
  ;

Primary
  : ArrayCreationExpression { $$ = $1; }
  | Identifier              { $$ = new Var(next_ident()); }
  | PrimaryNoNewArray       { $$ = $1; }
  ;

PrimaryNoNewArray
  : ParenExpression                 { $$ = $1; }
  | THIS                            { $$ = this_expr(); }
  | FieldAccess                     { $$ = $1; }
  | MethodInvocation                { $$ = $1; }
  | ArrayAccess                     { $$ = $1; }
  | ClassInstanceCreationExpression { $$ = $1; }
  | Literal                         { $$ = $1; }
  ;

ClassInstanceCreationExpression
  : NEW ClassType Arguments { $$ = new New($2,$3); }
  ;

ArrayCreationExpression
  : NEW ClassType DimensionExpressions Dimensions { $$ = new NewArray($2,$3,$4); }
  | NEW ClassType DimensionExpressions            { $$ = new NewArray($2,$3,0); }
  | NEW PrimitiveType DimensionExpressions Dimensions { $$ = new NewArray($2,$3,$4); }
  | NEW PrimitiveType DimensionExpressions        { $$ = new NewArray($2,$3,0); }
  ;

DimensionExpressions
  : DimensionExpressions DimensionExpression  { ($$ = $1)->push_back($2); }
  | DimensionExpression                       { ($$ = new list<Expr*>())->push_back($1); }
  ;

DimensionExpression
  : '[' Expression ']'  { $$ = $2; }
  ;

Dimensions
  : Dimensions Dimension  { $$ = $1 + 1; }
  | Dimension             { $$ = $1; }
  ;

Dimension
  : '[' ']' { $$ = 1; }
  ;

FieldAccess
  : Primary '.' Identifier  { $$ = new FAccess($1,next_ident()); }
  | SUPER '.' Identifier    { $$ = new FAccess(super_expr(), next_ident()); }
  ;

MethodInvocation
  : Identifier Arguments              { $$ = new MInvoke(this_expr(), next_ident(), $2); }
  | Primary '.' Identifier Arguments  { $$ = new MInvoke($1,next_ident(),$4); }
  | SUPER '.' Identifier Arguments    { $$ = new MInvoke(super_expr(),next_ident(),$4); }
  ;

ArrayAccess
  : Identifier DimensionExpression        { $$ = new AAccess(new Var(next_ident()),$2);  }
  | PrimaryNoNewArray DimensionExpression { $$ = new AAccess($1,$2); }
  ;

Arguments
  : '(' ')'               { $$ = new list<Expr*>(); }
  | '(' ArgumentList ')'  { $$ = $2; }
  ;

ArgumentList
  : ArgumentList ',' Expression { ($$ = $1)->push_back($3); }
  | Expression                  { ($$ = new list<Expr*>())->push_back($1); }
  ;

Type
  : PrimitiveType { $$ = $1; }
  | ReferenceType { $$ = $1; }
  ;

PrimitiveType
  : NumericType { $$ = $1; }
  ;

NumericType
  : IntegralType  { $$ = $1; }
  ;

IntegralType
  : INT { $$ = Type::int_type(); }
  ;

ReferenceType
  : ClassType { $$ = $1; }
  | ArrayType { $$ = $1; }
  ;

ClassType
  : Identifier  { $$ = Type::user(next_ident()); }
  ;

ArrayType
  : PrimitiveType Dimensions { $$ = Type::array($1,$2); }
  | Identifier Dimensions    { $$ = Type::array(Type::user(next_ident()),$2); }
  ;

Identifier
  : IDENTIFIER  { idents.push(string($1)); $$ = $1; }
  ;

Literal
  : INTEGER_LITERAL { $$ = new IntLit($1); }
  | NULL_LITERAL    { $$ = null_lit(); }
  ;

%%
