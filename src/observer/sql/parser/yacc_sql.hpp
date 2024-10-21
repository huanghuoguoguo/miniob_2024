/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_YACC_SQL_HPP_INCLUDED
# define YY_YY_YACC_SQL_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SEMICOLON = 258,               /* SEMICOLON  */
    BY = 259,                      /* BY  */
    CREATE = 260,                  /* CREATE  */
    JOIN = 261,                    /* JOIN  */
    LEFT = 262,                    /* LEFT  */
    RIGHT = 263,                   /* RIGHT  */
    INNER = 264,                   /* INNER  */
    OUTER = 265,                   /* OUTER  */
    DROP = 266,                    /* DROP  */
    GROUP = 267,                   /* GROUP  */
    TABLE = 268,                   /* TABLE  */
    TABLES = 269,                  /* TABLES  */
    INDEX = 270,                   /* INDEX  */
    CALC = 271,                    /* CALC  */
    SELECT = 272,                  /* SELECT  */
    DESC = 273,                    /* DESC  */
    SHOW = 274,                    /* SHOW  */
    SYNC = 275,                    /* SYNC  */
    INSERT = 276,                  /* INSERT  */
    DELETE = 277,                  /* DELETE  */
    UPDATE = 278,                  /* UPDATE  */
    LBRACE = 279,                  /* LBRACE  */
    RBRACE = 280,                  /* RBRACE  */
    COMMA = 281,                   /* COMMA  */
    TRX_BEGIN = 282,               /* TRX_BEGIN  */
    TRX_COMMIT = 283,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 284,            /* TRX_ROLLBACK  */
    INT_T = 285,                   /* INT_T  */
    STRING_T = 286,                /* STRING_T  */
    FLOAT_T = 287,                 /* FLOAT_T  */
    VECTOR_T = 288,                /* VECTOR_T  */
    DATE_T = 289,                  /* DATE_T  */
    HELP = 290,                    /* HELP  */
    EXIT = 291,                    /* EXIT  */
    DOT = 292,                     /* DOT  */
    INTO = 293,                    /* INTO  */
    VALUES = 294,                  /* VALUES  */
    FROM = 295,                    /* FROM  */
    WHERE = 296,                   /* WHERE  */
    AND = 297,                     /* AND  */
    SET = 298,                     /* SET  */
    ON = 299,                      /* ON  */
    LOAD = 300,                    /* LOAD  */
    DATA = 301,                    /* DATA  */
    INFILE = 302,                  /* INFILE  */
    EXPLAIN = 303,                 /* EXPLAIN  */
    STORAGE = 304,                 /* STORAGE  */
    FORMAT = 305,                  /* FORMAT  */
    EQ = 306,                      /* EQ  */
    LT = 307,                      /* LT  */
    GT = 308,                      /* GT  */
    LE = 309,                      /* LE  */
    GE = 310,                      /* GE  */
    NE = 311,                      /* NE  */
    IS = 312,                      /* IS  */
    NOT = 313,                     /* NOT  */
    LIKE = 314,                    /* LIKE  */
    NULL_ = 315,                   /* NULL_  */
    NULLABLE = 316,                /* NULLABLE  */
    HAVING = 317,                  /* HAVING  */
    NUMBER = 318,                  /* NUMBER  */
    FLOAT = 319,                   /* FLOAT  */
    ID = 320,                      /* ID  */
    SSS = 321,                     /* SSS  */
    DATE_STR = 322,                /* DATE_STR  */
    UMINUS = 323                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 131 "yacc_sql.y"

  ParsedSqlNode *                            sql_node;
  ConditionSqlNode *                         condition;
  Value *                                    value;
  enum CompOp                                comp;
  RelAttrSqlNode *                           rel_attr;
  std::vector<AttrInfoSqlNode> *             attr_infos;
  AttrInfoSqlNode *                          attr_info;
  Expression *                               expression;
  std::vector<std::unique_ptr<Expression>> * expression_list;
  std::vector<Value> *                       value_list;
  std::vector<ConditionSqlNode> *            condition_list;
  std::vector<JoinSqlNode> *                 join_list;
  JoinSqlNode *                              join;
  std::vector<RelAttrSqlNode> *              rel_attr_list;
  std::vector<std::string> *                 relation_list;
  char *                                     string;
  int                                        number;
  float                                      floats;

#line 153 "yacc_sql.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner);


#endif /* !YY_YY_YACC_SQL_HPP_INCLUDED  */
