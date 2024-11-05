/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "common/type/date_type.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg, bool flag = false)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  error_sql_node->error.flag = flag;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

FunctionExpr *create_aggregate_expression(const char *aggregate_name,
                                           std::vector<std::unique_ptr<Expression>> * expression_list,
                                           const char *sql_string,
                                           YYLTYPE *llocp)
{
  // UnboundAggregateExpr *expr = new UnboundAggregateExpr(aggregate_name, child);
  FunctionExpr *expr = new FunctionExpr(aggregate_name, expression_list);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}


#line 128 "yacc_sql.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_BY = 4,                         /* BY  */
  YYSYMBOL_CREATE = 5,                     /* CREATE  */
  YYSYMBOL_JOIN = 6,                       /* JOIN  */
  YYSYMBOL_LEFT = 7,                       /* LEFT  */
  YYSYMBOL_RIGHT = 8,                      /* RIGHT  */
  YYSYMBOL_INNER = 9,                      /* INNER  */
  YYSYMBOL_OUTER = 10,                     /* OUTER  */
  YYSYMBOL_DROP = 11,                      /* DROP  */
  YYSYMBOL_GROUP = 12,                     /* GROUP  */
  YYSYMBOL_ORDER = 13,                     /* ORDER  */
  YYSYMBOL_TABLE = 14,                     /* TABLE  */
  YYSYMBOL_TABLES = 15,                    /* TABLES  */
  YYSYMBOL_INDEX = 16,                     /* INDEX  */
  YYSYMBOL_CALC = 17,                      /* CALC  */
  YYSYMBOL_SELECT = 18,                    /* SELECT  */
  YYSYMBOL_DESC = 19,                      /* DESC  */
  YYSYMBOL_SHOW = 20,                      /* SHOW  */
  YYSYMBOL_SYNC = 21,                      /* SYNC  */
  YYSYMBOL_INSERT = 22,                    /* INSERT  */
  YYSYMBOL_DELETE = 23,                    /* DELETE  */
  YYSYMBOL_UPDATE = 24,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 25,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 26,                    /* RBRACE  */
  YYSYMBOL_COMMA = 27,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 28,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 29,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 30,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 31,                     /* INT_T  */
  YYSYMBOL_STRING_T = 32,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 33,                   /* FLOAT_T  */
  YYSYMBOL_TEXT_T = 34,                    /* TEXT_T  */
  YYSYMBOL_VECTOR_T = 35,                  /* VECTOR_T  */
  YYSYMBOL_DATE_T = 36,                    /* DATE_T  */
  YYSYMBOL_HELP = 37,                      /* HELP  */
  YYSYMBOL_EXIT = 38,                      /* EXIT  */
  YYSYMBOL_DOT = 39,                       /* DOT  */
  YYSYMBOL_INTO = 40,                      /* INTO  */
  YYSYMBOL_VALUES = 41,                    /* VALUES  */
  YYSYMBOL_FROM = 42,                      /* FROM  */
  YYSYMBOL_WHERE = 43,                     /* WHERE  */
  YYSYMBOL_AND = 44,                       /* AND  */
  YYSYMBOL_ASC = 45,                       /* ASC  */
  YYSYMBOL_SET = 46,                       /* SET  */
  YYSYMBOL_ON = 47,                        /* ON  */
  YYSYMBOL_LOAD = 48,                      /* LOAD  */
  YYSYMBOL_INFILE = 49,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 50,                   /* EXPLAIN  */
  YYSYMBOL_STORAGE = 51,                   /* STORAGE  */
  YYSYMBOL_FORMAT = 52,                    /* FORMAT  */
  YYSYMBOL_EQ = 53,                        /* EQ  */
  YYSYMBOL_LT = 54,                        /* LT  */
  YYSYMBOL_GT = 55,                        /* GT  */
  YYSYMBOL_LE = 56,                        /* LE  */
  YYSYMBOL_GE = 57,                        /* GE  */
  YYSYMBOL_NE = 58,                        /* NE  */
  YYSYMBOL_IS = 59,                        /* IS  */
  YYSYMBOL_NOT = 60,                       /* NOT  */
  YYSYMBOL_LIKE = 61,                      /* LIKE  */
  YYSYMBOL_NULL_ = 62,                     /* NULL_  */
  YYSYMBOL_NULLABLE = 63,                  /* NULLABLE  */
  YYSYMBOL_HAVING = 64,                    /* HAVING  */
  YYSYMBOL_IN = 65,                        /* IN  */
  YYSYMBOL_UNIQUE = 66,                    /* UNIQUE  */
  YYSYMBOL_OR = 67,                        /* OR  */
  YYSYMBOL_AS = 68,                        /* AS  */
  YYSYMBOL_VIEW = 69,                      /* VIEW  */
  YYSYMBOL_LIMIT = 70,                     /* LIMIT  */
  YYSYMBOL_WITH = 71,                      /* WITH  */
  YYSYMBOL_NUMBER = 72,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 73,                     /* FLOAT  */
  YYSYMBOL_ID = 74,                        /* ID  */
  YYSYMBOL_SSS = 75,                       /* SSS  */
  YYSYMBOL_DATE_STR = 76,                  /* DATE_STR  */
  YYSYMBOL_77_ = 77,                       /* '+'  */
  YYSYMBOL_78_ = 78,                       /* '-'  */
  YYSYMBOL_79_ = 79,                       /* '*'  */
  YYSYMBOL_80_ = 80,                       /* '/'  */
  YYSYMBOL_UMINUS = 81,                    /* UMINUS  */
  YYSYMBOL_82_ = 82,                       /* '['  */
  YYSYMBOL_83_ = 83,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 84,                  /* $accept  */
  YYSYMBOL_commands = 85,                  /* commands  */
  YYSYMBOL_command_wrapper = 86,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 87,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 88,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 89,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 90,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 91,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 92,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 93,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 94,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 95,           /* desc_table_stmt  */
  YYSYMBOL_create_view_stmt = 96,          /* create_view_stmt  */
  YYSYMBOL_create_index_stmt = 97,         /* create_index_stmt  */
  YYSYMBOL_with_block = 98,                /* with_block  */
  YYSYMBOL_index_type = 99,                /* index_type  */
  YYSYMBOL_drop_index_stmt = 100,          /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 101,        /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 102,            /* attr_def_list  */
  YYSYMBOL_attr_def = 103,                 /* attr_def  */
  YYSYMBOL_as_option = 104,                /* as_option  */
  YYSYMBOL_number = 105,                   /* number  */
  YYSYMBOL_type = 106,                     /* type  */
  YYSYMBOL_insert_stmt = 107,              /* insert_stmt  */
  YYSYMBOL_field_columns = 108,            /* field_columns  */
  YYSYMBOL_value = 109,                    /* value  */
  YYSYMBOL_value_list = 110,               /* value_list  */
  YYSYMBOL_storage_format = 111,           /* storage_format  */
  YYSYMBOL_delete_stmt = 112,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 113,              /* update_stmt  */
  YYSYMBOL_select_stmt = 114,              /* select_stmt  */
  YYSYMBOL_limit_block = 115,              /* limit_block  */
  YYSYMBOL_join_list = 116,                /* join_list  */
  YYSYMBOL_join = 117,                     /* join  */
  YYSYMBOL_calc_stmt = 118,                /* calc_stmt  */
  YYSYMBOL_expression_list = 119,          /* expression_list  */
  YYSYMBOL_aggre_type = 120,               /* aggre_type  */
  YYSYMBOL_aggre_list = 121,               /* aggre_list  */
  YYSYMBOL_expression = 122,               /* expression  */
  YYSYMBOL_rel_attr = 123,                 /* rel_attr  */
  YYSYMBOL_relation = 124,                 /* relation  */
  YYSYMBOL_rel_list = 125,                 /* rel_list  */
  YYSYMBOL_where = 126,                    /* where  */
  YYSYMBOL_condition_list = 127,           /* condition_list  */
  YYSYMBOL_condition = 128,                /* condition  */
  YYSYMBOL_comp_op = 129,                  /* comp_op  */
  YYSYMBOL_group_by = 130,                 /* group_by  */
  YYSYMBOL_having_condition = 131,         /* having_condition  */
  YYSYMBOL_order_unit = 132,               /* order_unit  */
  YYSYMBOL_order_unit_list = 133,          /* order_unit_list  */
  YYSYMBOL_opt_order_by = 134,             /* opt_order_by  */
  YYSYMBOL_alias = 135,                    /* alias  */
  YYSYMBOL_explain_stmt = 136,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 137,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 138             /* opt_semicolon  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  72
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   251

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  84
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  149
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  260

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   332


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    79,    77,     2,    78,     2,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    82,     2,    83,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    81
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   236,   236,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   261,
     262,   263,   264,   268,   274,   279,   285,   291,   297,   303,
     310,   316,   324,   332,   344,   364,   386,   389,   395,   398,
     401,   410,   420,   441,   457,   468,   471,   484,   493,   502,
     511,   520,   529,   538,   547,   559,   562,   568,   571,   572,
     573,   574,   575,   576,   579,   596,   599,   605,   609,   613,
     619,   636,   640,   656,   660,   672,   675,   682,   694,   707,
     749,   752,   758,   761,   769,   779,   787,   796,   805,   820,
     824,   827,   831,   839,   842,   845,   848,   851,   855,   858,
     863,   869,   872,   875,   881,   886,   893,   898,   907,   916,
     921,   935,   938,   944,   947,   952,   958,   964,   971,   981,
     982,   983,   984,   985,   986,   987,   988,   989,   990,   991,
     992,   998,  1001,  1007,  1010,  1015,  1022,  1029,  1037,  1044,
    1052,  1055,  1062,  1065,  1068,  1086,  1094,  1102,  1112,  1113
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "BY",
  "CREATE", "JOIN", "LEFT", "RIGHT", "INNER", "OUTER", "DROP", "GROUP",
  "ORDER", "TABLE", "TABLES", "INDEX", "CALC", "SELECT", "DESC", "SHOW",
  "SYNC", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA",
  "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T",
  "FLOAT_T", "TEXT_T", "VECTOR_T", "DATE_T", "HELP", "EXIT", "DOT", "INTO",
  "VALUES", "FROM", "WHERE", "AND", "ASC", "SET", "ON", "LOAD", "INFILE",
  "EXPLAIN", "STORAGE", "FORMAT", "EQ", "LT", "GT", "LE", "GE", "NE", "IS",
  "NOT", "LIKE", "NULL_", "NULLABLE", "HAVING", "IN", "UNIQUE", "OR", "AS",
  "VIEW", "LIMIT", "WITH", "NUMBER", "FLOAT", "ID", "SSS", "DATE_STR",
  "'+'", "'-'", "'*'", "'/'", "UMINUS", "'['", "']'", "$accept",
  "commands", "command_wrapper", "exit_stmt", "help_stmt", "sync_stmt",
  "begin_stmt", "commit_stmt", "rollback_stmt", "drop_table_stmt",
  "show_tables_stmt", "desc_table_stmt", "create_view_stmt",
  "create_index_stmt", "with_block", "index_type", "drop_index_stmt",
  "create_table_stmt", "attr_def_list", "attr_def", "as_option", "number",
  "type", "insert_stmt", "field_columns", "value", "value_list",
  "storage_format", "delete_stmt", "update_stmt", "select_stmt",
  "limit_block", "join_list", "join", "calc_stmt", "expression_list",
  "aggre_type", "aggre_list", "expression", "rel_attr", "relation",
  "rel_list", "where", "condition_list", "condition", "comp_op",
  "group_by", "having_condition", "order_unit", "order_unit_list",
  "opt_order_by", "alias", "explain_stmt", "set_variable_stmt",
  "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-175)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-90)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     111,     0,    40,   -14,   -14,   -57,    10,  -175,     3,    34,
      13,  -175,  -175,  -175,  -175,  -175,    18,   111,    81,    91,
    -175,  -175,  -175,  -175,  -175,  -175,  -175,  -175,  -175,  -175,
    -175,  -175,  -175,  -175,  -175,  -175,  -175,  -175,  -175,  -175,
      41,  -175,   -28,    43,  -175,    98,    44,    45,   -14,  -175,
    -175,  -175,    -9,  -175,  -175,   -14,    58,    31,  -175,  -175,
    -175,   113,    76,  -175,    94,  -175,  -175,    63,    68,    97,
     -45,  -175,  -175,  -175,  -175,    -5,   129,     7,    72,  -175,
     100,   125,    21,   -48,  -175,    73,   131,    77,   -14,    85,
    -175,   -14,   -14,   -14,   -14,   135,    89,   149,   132,   -14,
      31,  -175,   102,  -175,   160,   105,   -14,   160,   133,   107,
    -175,  -175,  -175,  -175,  -175,    31,  -175,  -175,   157,  -175,
      11,    11,  -175,  -175,   -14,   -53,   158,    28,   -14,   145,
     -14,  -175,   112,   132,   -22,  -175,   162,   161,  -175,   140,
     173,  -175,   128,  -175,  -175,  -175,  -175,  -175,    89,    89,
     197,   132,   179,   181,  -175,  -175,  -175,  -175,  -175,  -175,
    -175,   148,   -38,  -175,  -175,   -14,  -175,   -14,   -14,   -14,
    -175,  -175,  -175,  -175,  -175,  -175,    17,   102,   183,   136,
     143,   187,  -175,    28,   166,    89,   204,  -175,   -14,  -175,
    -175,  -175,    32,  -175,  -175,  -175,   146,   159,  -175,  -175,
     161,   -15,   195,   160,   -14,  -175,   -14,    28,   218,   163,
     198,  -175,   199,  -175,  -175,   171,   160,  -175,   -14,  -175,
     200,  -175,  -175,   -14,   -14,   215,  -175,    23,   176,  -175,
     205,   164,  -175,  -175,   226,   167,   170,  -175,  -175,   165,
     164,   208,  -175,   -14,   168,  -175,  -175,  -175,  -175,   -14,
      -7,   207,  -175,  -175,   210,  -175,  -175,   -14,  -175,  -175
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    38,     0,     0,     0,     0,     0,    25,     0,     0,
       0,    26,    27,    28,    24,    23,     0,     0,     0,   148,
      22,    21,    14,    15,    17,    18,     9,    10,    11,    16,
      12,    13,     8,     5,     7,     6,     4,     3,    19,    20,
       0,    40,    38,     0,    39,     0,     0,     0,     0,    71,
      67,    68,   104,    69,    70,     0,   101,     0,    99,   102,
      86,     0,   142,   100,     0,    31,    30,     0,     0,     0,
       0,   145,     1,   149,     2,    55,     0,     0,     0,    29,
       0,     0,   142,     0,    98,     0,    73,     0,    90,     0,
     143,     0,     0,     0,     0,    87,     0,    65,   111,   113,
       0,   147,     0,    56,     0,     0,     0,     0,     0,     0,
     103,    97,   105,   107,   106,     0,    72,    91,     0,   144,
      93,    94,    95,    96,     0,   142,   109,    82,     0,     0,
     113,    77,     0,   111,   114,   146,     0,    45,    44,     0,
       0,    32,     0,    41,    74,    92,    88,   108,     0,     0,
       0,   111,     0,     0,   112,   119,   120,   121,   122,   123,
     124,   127,     0,   125,   129,     0,    78,   113,   113,   113,
      58,    59,    60,    63,    61,    62,    48,     0,     0,     0,
       0,     0,   110,    82,     0,     0,   131,    66,     0,   128,
     126,   130,   118,   117,   115,   116,     0,     0,    54,    53,
      45,    75,     0,     0,     0,    83,   113,    82,     0,   133,
       0,    57,     0,    52,    46,     0,     0,    42,     0,    33,
       0,    85,    84,     0,   113,   140,    64,    47,     0,    43,
       0,    36,   132,   134,     0,    80,     0,    50,    51,     0,
      36,     0,    34,     0,     0,    79,    49,    76,    35,   113,
     135,   138,   141,    81,     0,   136,   137,     0,    37,   139
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -175,  -175,   221,  -175,  -175,  -175,  -175,  -175,  -175,  -175,
    -175,  -175,  -175,  -175,     2,   201,  -175,  -175,    46,    64,
      47,  -175,  -175,  -175,  -175,   -33,   130,  -175,  -175,  -175,
       1,  -175,  -174,    59,  -175,    -4,  -175,  -175,   -42,  -175,
    -130,    99,  -123,  -128,  -175,  -175,  -175,  -175,  -175,    -8,
    -175,   126,  -175,  -175,  -175
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,   242,    45,    31,    32,   178,   137,
     104,   212,   176,    33,   129,    58,    87,   217,    34,    35,
      59,   245,   151,   183,    37,    60,    61,   118,    62,    63,
     126,   127,   131,   133,   134,   165,   209,   225,   251,   252,
     235,    95,    38,    39,    74
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      64,    36,   154,   -55,     4,   167,    82,    41,   100,   205,
     166,    48,   255,    84,    40,    89,   -89,    65,    36,   184,
     102,    90,   168,   190,    86,    66,   112,   191,   186,   101,
      83,   113,   106,   222,   149,    41,   215,   150,   256,   193,
     194,   195,   196,    67,    81,   169,    44,   111,    49,   120,
     121,   122,   123,   103,    46,   184,    47,   132,    50,    51,
      52,    53,    54,   103,    55,    56,    42,   135,    57,    43,
      91,    92,    93,    94,    44,   107,    68,   197,   221,   198,
     199,    72,    86,   236,   117,   237,   238,    69,   132,    89,
      93,    94,    70,    49,    73,    90,   233,    85,    91,    92,
      93,    94,   140,    50,    51,   138,    53,    54,   141,    91,
      92,    93,    94,    57,    78,    75,     1,    77,    79,    80,
     146,   254,     2,   192,   152,   132,   132,   132,     3,     4,
       5,     6,     7,     8,     9,    10,    96,    97,    88,    11,
      12,    13,    98,    99,    89,   105,   108,   109,    14,    15,
      90,   110,   114,    91,    92,    93,    94,    16,   115,   119,
     116,    17,   124,   125,   132,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   128,   130,   136,   164,     4,   139,
     142,   143,   132,   145,   210,   148,   153,   179,   177,    91,
      92,    93,    94,   170,   171,   172,   173,   174,   175,   180,
     220,   250,   181,   185,   219,   187,   188,   132,   189,   201,
     202,   203,   204,   206,   230,   250,   208,   229,   211,   232,
     218,   213,   223,   228,   226,   227,   231,   224,   234,   239,
     243,   240,   246,   249,   257,   241,   258,   244,    71,   247,
     253,   200,   248,    76,   207,   144,   214,   182,   216,   259,
       0,   147
};

static const yytype_int16 yycheck[] =
{
       4,     0,   130,    18,    18,    27,    48,    35,    53,   183,
     133,    25,    19,    55,    14,    68,    25,    74,    17,   149,
      25,    74,    44,    61,    57,    15,    74,    65,   151,    74,
      39,    79,    25,   207,     6,    35,    51,     9,    45,   167,
     168,   169,    25,    40,    48,    67,    74,    26,    62,    91,
      92,    93,    94,    68,    14,   185,    16,    99,    72,    73,
      74,    75,    76,    68,    78,    79,    66,   100,    82,    69,
      77,    78,    79,    80,    74,    68,    42,    60,   206,    62,
      63,     0,   115,    60,    88,    62,    63,    74,   130,    68,
      79,    80,    74,    62,     3,    74,   224,    39,    77,    78,
      79,    80,   106,    72,    73,   104,    75,    76,   107,    77,
      78,    79,    80,    82,    16,    74,     5,    74,    74,    74,
     124,   249,    11,   165,   128,   167,   168,   169,    17,    18,
      19,    20,    21,    22,    23,    24,    42,    74,    25,    28,
      29,    30,    74,    46,    68,    16,    74,    47,    37,    38,
      74,    26,    79,    77,    78,    79,    80,    46,    27,    74,
      83,    50,    27,    74,   206,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    25,    43,    74,    65,    18,    74,
      47,    74,   224,    26,   188,    27,    41,    47,    27,    77,
      78,    79,    80,    31,    32,    33,    34,    35,    36,    26,
     204,   243,    74,     6,   203,    26,    25,   249,    60,    26,
      74,    68,    25,    47,   218,   257,    12,   216,    72,   223,
      25,    62,     4,    52,    26,    26,    26,    64,    13,    53,
       4,    26,    62,    25,    27,    71,    26,    70,    17,    74,
      72,   177,   240,    42,   185,   115,   200,   148,   201,   257,
      -1,   125
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    11,    17,    18,    19,    20,    21,    22,    23,
      24,    28,    29,    30,    37,    38,    46,    50,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,   100,   101,   107,   112,   113,   114,   118,   136,   137,
      14,    35,    66,    69,    74,    99,    14,    16,    25,    62,
      72,    73,    74,    75,    76,    78,    79,    82,   109,   114,
     119,   120,   122,   123,   119,    74,    15,    40,    42,    74,
      74,    86,     0,     3,   138,    74,    99,    74,    16,    74,
      74,   119,   122,    39,   122,    39,   109,   110,    25,    68,
      74,    77,    78,    79,    80,   135,    42,    74,    74,    46,
      53,    74,    25,    68,   104,    16,    25,    68,    74,    47,
      26,    26,    74,    79,    79,    27,    83,   119,   121,    74,
     122,   122,   122,   122,    27,    74,   124,   125,    25,   108,
      43,   126,   122,   127,   128,   109,    74,   103,   114,    74,
     119,   114,    47,    74,   110,    26,   119,   135,    27,     6,
       9,   116,   119,    41,   127,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    65,   129,   126,    27,    44,    67,
      31,    32,    33,    34,    35,    36,   106,    27,   102,    47,
      26,    74,   125,   117,   124,     6,   126,    26,    25,    60,
      61,    65,   122,   127,   127,   127,    25,    60,    62,    63,
     103,    26,    74,    68,    25,   116,    47,   117,    12,   130,
     119,    72,   105,    62,   102,    51,   104,   111,    25,   114,
     119,   127,   116,     4,    64,   131,    26,    26,    52,   114,
     119,    26,   119,   127,    13,   134,    60,    62,    63,    53,
      26,    71,    98,     4,    70,   115,    62,    74,    98,    25,
     122,   132,   133,    72,   127,    19,    45,    27,    26,   133
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    84,    85,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    96,    97,    97,    98,    98,    99,    99,
      99,   100,   101,   101,   101,   102,   102,   103,   103,   103,
     103,   103,   103,   103,   103,   104,   104,   105,   106,   106,
     106,   106,   106,   106,   107,   108,   108,   109,   109,   109,
     109,   109,   109,   110,   110,   111,   111,   112,   113,   114,
     115,   115,   116,   116,   116,   117,   118,   119,   119,   120,
     121,   121,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   123,   123,   123,   123,   124,   125,
     125,   126,   126,   127,   127,   127,   127,   127,   128,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   130,   130,   131,   131,   132,   132,   132,   133,   133,
     134,   134,   135,   135,   135,   136,   137,   137,   138,   138
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     5,     8,    10,    11,     0,     4,     0,     1,
       1,     5,     8,     9,     5,     0,     3,     5,     2,     7,
       6,     6,     4,     3,     3,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     8,     0,     3,     1,     1,     1,
       1,     1,     3,     1,     3,     0,     4,     4,     5,    10,
       0,     2,     0,     3,     4,     3,     2,     2,     4,     1,
       0,     1,     4,     3,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     3,     1,     3,     3,     3,     2,     1,
       3,     0,     2,     0,     1,     3,     3,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     2,     1,
       2,     0,     3,     0,     2,     1,     2,     2,     1,     3,
       0,     3,     0,     1,     2,     2,     4,     3,     0,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* commands: command_wrapper opt_semicolon  */
#line 237 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1850 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 268 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1859 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 274 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1867 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 279 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1875 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 285 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1883 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 291 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1891 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 297 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1899 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 303 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1909 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 310 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1917 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC ID  */
#line 316 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1927 "yacc_sql.cpp"
    break;

  case 32: /* create_view_stmt: CREATE VIEW ID AS select_stmt  */
#line 325 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_VIEW);;
      (yyval.sql_node)->flag = SCF_CREATE_VIEW;
      (yyval.sql_node)->create_view.view_name = (yyvsp[-2].string);
      (yyval.sql_node)->create_view.select_sql_node = &(yyvsp[0].sql_node)->selection;
      free((yyvsp[-2].string));
    }
#line 1939 "yacc_sql.cpp"
    break;

  case 33: /* create_view_stmt: CREATE VIEW ID LBRACE expression_list RBRACE AS select_stmt  */
#line 333 "yacc_sql.y"
     {
       (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_VIEW);;
       (yyval.sql_node)->flag = SCF_CREATE_VIEW;
       (yyval.sql_node)->create_view.expressions.swap(*(yyvsp[-3].expression_list));
       (yyval.sql_node)->create_view.view_name = (yyvsp[-5].string);
       (yyval.sql_node)->create_view.select_sql_node = &(yyvsp[0].sql_node)->selection;
       free((yyvsp[-5].string));
     }
#line 1952 "yacc_sql.cpp"
    break;

  case 34: /* create_index_stmt: CREATE index_type INDEX ID ON ID LBRACE expression_list RBRACE with_block  */
#line 345 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;

      if((yyvsp[-8].string) != nullptr){
        create_index.index_type = (yyvsp[-8].string);
      }else{
        create_index.index_type = "normal";
      }
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);
      create_index.unique = false;
      create_index.columns.swap(*(yyvsp[-2].expression_list));
      if((yyvsp[0].condition_list) != nullptr){
        create_index.equal_expression.swap(*(yyvsp[0].condition_list));
      }
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
    }
#line 1976 "yacc_sql.cpp"
    break;

  case 35: /* create_index_stmt: CREATE UNIQUE index_type INDEX ID ON ID LBRACE expression_list RBRACE with_block  */
#line 365 "yacc_sql.y"
     {
       (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
       CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
       if((yyvsp[-8].string) != nullptr){
         create_index.index_type = (yyvsp[-8].string);
       }else{
         create_index.index_type = "normal";
       }
       create_index.index_name = (yyvsp[-6].string);
       create_index.relation_name = (yyvsp[-4].string);
       create_index.unique = true;
       create_index.columns.swap(*(yyvsp[-2].expression_list));
       if((yyvsp[0].condition_list) != nullptr){
         create_index.equal_expression.swap(*(yyvsp[0].condition_list));
       }
       free((yyvsp[-6].string));
       free((yyvsp[-4].string));
     }
#line 1999 "yacc_sql.cpp"
    break;

  case 36: /* with_block: %empty  */
#line 386 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2007 "yacc_sql.cpp"
    break;

  case 37: /* with_block: WITH LBRACE condition_list RBRACE  */
#line 389 "yacc_sql.y"
                                        {
      (yyval.condition_list) = (yyvsp[-1].condition_list);
    }
#line 2015 "yacc_sql.cpp"
    break;

  case 38: /* index_type: %empty  */
#line 395 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2023 "yacc_sql.cpp"
    break;

  case 39: /* index_type: ID  */
#line 398 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2031 "yacc_sql.cpp"
    break;

  case 40: /* index_type: VECTOR_T  */
#line 401 "yacc_sql.y"
               {
      std::string str = "vector";
      // 获取可修改的 char*
      char* cstr = new char[str.size() + 1]; // +1 for the null terminator
      strcpy(cstr, str.data()); // 复制内容
      (yyval.string) = cstr;
    }
#line 2043 "yacc_sql.cpp"
    break;

  case 41: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 411 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2055 "yacc_sql.cpp"
    break;

  case 42: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format  */
#line 421 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-5].string);
      free((yyvsp[-5].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-2].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-3].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-3].attr_info);
      if ((yyvsp[0].string) != nullptr) {
        create_table.storage_format = (yyvsp[0].string);
        free((yyvsp[0].string));
      }
    }
#line 2080 "yacc_sql.cpp"
    break;

  case 43: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE as_option select_stmt  */
#line 442 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-6].string);
      free((yyvsp[-6].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-3].attr_infos);
      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-4].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-4].attr_info);
    }
#line 2100 "yacc_sql.cpp"
    break;

  case 44: /* create_table_stmt: CREATE TABLE ID as_option select_stmt  */
#line 458 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
    }
#line 2112 "yacc_sql.cpp"
    break;

  case 45: /* attr_def_list: %empty  */
#line 468 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2120 "yacc_sql.cpp"
    break;

  case 46: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 472 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2134 "yacc_sql.cpp"
    break;

  case 47: /* attr_def: ID type LBRACE number RBRACE  */
#line 485 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-3].number);
      (yyval.attr_info)->name = (yyvsp[-4].string);
      (yyval.attr_info)->length = (yyvsp[-1].number);
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-4].string));
    }
#line 2147 "yacc_sql.cpp"
    break;

  case 48: /* attr_def: ID type  */
#line 494 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[0].number);
      (yyval.attr_info)->name = (yyvsp[-1].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-1].string));
    }
#line 2160 "yacc_sql.cpp"
    break;

  case 49: /* attr_def: ID type LBRACE number RBRACE NOT NULL_  */
#line 503 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-5].number);
      (yyval.attr_info)->name = (yyvsp[-6].string);
      (yyval.attr_info)->length = (yyvsp[-3].number);
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-6].string));
    }
#line 2173 "yacc_sql.cpp"
    break;

  case 50: /* attr_def: ID type LBRACE number RBRACE NULL_  */
#line 512 "yacc_sql.y"
        {
          (yyval.attr_info) = new AttrInfoSqlNode;
          (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
          (yyval.attr_info)->name = (yyvsp[-5].string);
          (yyval.attr_info)->length = (yyvsp[-2].number);
          (yyval.attr_info)->nullable = true;
          free((yyvsp[-5].string));
        }
#line 2186 "yacc_sql.cpp"
    break;

  case 51: /* attr_def: ID type LBRACE number RBRACE NULLABLE  */
#line 521 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-5].string));
    }
#line 2199 "yacc_sql.cpp"
    break;

  case 52: /* attr_def: ID type NOT NULL_  */
#line 530 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-2].number);
      (yyval.attr_info)->name = (yyvsp[-3].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-3].string));
    }
#line 2212 "yacc_sql.cpp"
    break;

  case 53: /* attr_def: ID type NULLABLE  */
#line 539 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-2].string));
    }
#line 2225 "yacc_sql.cpp"
    break;

  case 54: /* attr_def: ID type NULL_  */
#line 548 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-2].string));
    }
#line 2238 "yacc_sql.cpp"
    break;

  case 55: /* as_option: %empty  */
#line 559 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2246 "yacc_sql.cpp"
    break;

  case 56: /* as_option: AS  */
#line 563 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2254 "yacc_sql.cpp"
    break;

  case 57: /* number: NUMBER  */
#line 568 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2260 "yacc_sql.cpp"
    break;

  case 58: /* type: INT_T  */
#line 571 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2266 "yacc_sql.cpp"
    break;

  case 59: /* type: STRING_T  */
#line 572 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2272 "yacc_sql.cpp"
    break;

  case 60: /* type: FLOAT_T  */
#line 573 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2278 "yacc_sql.cpp"
    break;

  case 61: /* type: VECTOR_T  */
#line 574 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2284 "yacc_sql.cpp"
    break;

  case 62: /* type: DATE_T  */
#line 575 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::DATES); }
#line 2290 "yacc_sql.cpp"
    break;

  case 63: /* type: TEXT_T  */
#line 576 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::TEXTS); }
#line 2296 "yacc_sql.cpp"
    break;

  case 64: /* insert_stmt: INSERT INTO ID field_columns VALUES LBRACE expression_list RBRACE  */
#line 580 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-5].string);
      if ((yyvsp[-4].expression_list) != nullptr) {
        (yyval.sql_node)->insertion.columns.swap(*(yyvsp[-4].expression_list));
        delete (yyvsp[-4].expression_list);
      }
      if ((yyvsp[-1].expression_list) != nullptr) {
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[-1].expression_list));
        delete (yyvsp[-1].expression_list);
      }
      free((yyvsp[-5].string));
    }
#line 2314 "yacc_sql.cpp"
    break;

  case 65: /* field_columns: %empty  */
#line 596 "yacc_sql.y"
    {
        (yyval.expression_list) = nullptr;
    }
#line 2322 "yacc_sql.cpp"
    break;

  case 66: /* field_columns: LBRACE expression_list RBRACE  */
#line 599 "yacc_sql.y"
                                   {
        (yyval.expression_list) = (yyvsp[-1].expression_list);
    }
#line 2330 "yacc_sql.cpp"
    break;

  case 67: /* value: NUMBER  */
#line 605 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2339 "yacc_sql.cpp"
    break;

  case 68: /* value: FLOAT  */
#line 609 "yacc_sql.y"
            {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2348 "yacc_sql.cpp"
    break;

  case 69: /* value: SSS  */
#line 613 "yacc_sql.y"
          {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2359 "yacc_sql.cpp"
    break;

  case 70: /* value: DATE_STR  */
#line 619 "yacc_sql.y"
               {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      std::string str(tmp);
      Value * value = new Value();
      int date;
      if(DateType::string_to_date(str,date) < 0)
      {
        yyerror(&(yyloc),sql_string,sql_result,scanner,"date invaid",true);
        YYERROR;
      }
      else
      {
        value->set_date(date);
      }
      (yyval.value) = value;
      free(tmp);
    }
#line 2381 "yacc_sql.cpp"
    break;

  case 71: /* value: NULL_  */
#line 636 "yacc_sql.y"
            {
      (yyval.value) = new Value();
      (yyloc) = (yylsp[0]);
    }
#line 2390 "yacc_sql.cpp"
    break;

  case 72: /* value: '[' value_list ']'  */
#line 640 "yacc_sql.y"
                         {
        (yyval.value) = new Value();
        (yyval.value)->set_type(AttrType::VECTORS);
        vector<float> float_data;
        for (auto &v : *(yyvsp[-1].value_list)) {
            // 获取信息，封装为vector
          if (v->attr_type() == AttrType::INTS) {
            float_data.insert(float_data.begin(), static_cast<float>(v->get_int()));
          }else {
            float_data.insert(float_data.begin(), v->get_float());
          }
        }
        (yyval.value)->set_vector(float_data);
    }
#line 2409 "yacc_sql.cpp"
    break;

  case 73: /* value_list: value  */
#line 656 "yacc_sql.y"
          {
        (yyval.value_list) = new std::vector<Value*>;
        (yyval.value_list)->push_back((yyvsp[0].value));
    }
#line 2418 "yacc_sql.cpp"
    break;

  case 74: /* value_list: value COMMA value_list  */
#line 660 "yacc_sql.y"
                            {
        if((yyvsp[0].value_list) != nullptr){
           (yyvsp[0].value_list)->push_back((yyvsp[-2].value));
           (yyval.value_list) = (yyvsp[0].value_list);
        }else{
           (yyval.value_list) = new std::vector<Value*>;
           (yyval.value_list)->push_back((yyvsp[-2].value));
        }
    }
#line 2432 "yacc_sql.cpp"
    break;

  case 75: /* storage_format: %empty  */
#line 672 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2440 "yacc_sql.cpp"
    break;

  case 76: /* storage_format: STORAGE FORMAT EQ ID  */
#line 676 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2448 "yacc_sql.cpp"
    break;

  case 77: /* delete_stmt: DELETE FROM ID where  */
#line 683 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2462 "yacc_sql.cpp"
    break;

  case 78: /* update_stmt: UPDATE ID SET condition_list where  */
#line 695 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      (yyval.sql_node)->update.relation_name = (yyvsp[-3].string);
      (yyval.sql_node)->update.set_expression.swap(*(yyvsp[-1].condition_list));
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->update.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-3].string));
    }
#line 2477 "yacc_sql.cpp"
    break;

  case 79: /* select_stmt: SELECT expression_list FROM rel_list join_list where group_by having_condition opt_order_by limit_block  */
#line 708 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-8].expression_list) != nullptr) {
        (yyval.sql_node)->selection.expressions.swap(*(yyvsp[-8].expression_list));
        delete (yyvsp[-8].expression_list);
      }

      if ((yyvsp[-6].relation_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap(*(yyvsp[-6].relation_list));
        delete (yyvsp[-6].relation_list);
      }
      if ((yyvsp[-5].join_list) != nullptr) {
        (yyval.sql_node)->selection.join_list.swap(*(yyvsp[-5].join_list));
        delete (yyvsp[-5].join_list);
      }

      if ((yyvsp[-4].condition_list) != nullptr) {
        (yyval.sql_node)->selection.conditions.swap(*(yyvsp[-4].condition_list));
        delete (yyvsp[-4].condition_list);
      }

      if ((yyvsp[-3].expression_list) != nullptr) {
        (yyval.sql_node)->selection.group_by.swap(*(yyvsp[-3].expression_list));
        delete (yyvsp[-3].expression_list);
      }

      if ((yyvsp[-2].condition_list) != nullptr) {
        (yyval.sql_node)->selection.group_by_having.swap(*(yyvsp[-2].condition_list));
        delete (yyvsp[-2].condition_list);
      }

      if ((yyvsp[-1].order_unit_list) != nullptr) {
        (yyval.sql_node)->selection.order_unit_list.swap(*(yyvsp[-1].order_unit_list));
        delete (yyvsp[-1].order_unit_list);
      }

      (yyval.sql_node)->selection.limit = (yyvsp[0].number);
    }
#line 2520 "yacc_sql.cpp"
    break;

  case 80: /* limit_block: %empty  */
#line 749 "yacc_sql.y"
    {
      (yyval.number) = -1;
    }
#line 2528 "yacc_sql.cpp"
    break;

  case 81: /* limit_block: LIMIT NUMBER  */
#line 752 "yacc_sql.y"
                   {
      (yyval.number) = (yyvsp[0].number);
    }
#line 2536 "yacc_sql.cpp"
    break;

  case 82: /* join_list: %empty  */
#line 758 "yacc_sql.y"
    {
      (yyval.join_list) = nullptr;
    }
#line 2544 "yacc_sql.cpp"
    break;

  case 83: /* join_list: JOIN join join_list  */
#line 761 "yacc_sql.y"
                          {
          if ((yyvsp[0].join_list) != nullptr) {
            (yyval.join_list) = (yyvsp[0].join_list);
          } else {
            (yyval.join_list) = new std::vector<JoinSqlNode>;
          }
          (yyval.join_list)->emplace((yyval.join_list)->begin(), *(yyvsp[-1].join));
        }
#line 2557 "yacc_sql.cpp"
    break;

  case 84: /* join_list: INNER JOIN join join_list  */
#line 769 "yacc_sql.y"
                                {
      if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new std::vector<JoinSqlNode>;
      }
      (yyval.join_list)->emplace((yyval.join_list)->begin(), *(yyvsp[-1].join));
    }
#line 2570 "yacc_sql.cpp"
    break;

  case 85: /* join: relation ON condition_list  */
#line 779 "yacc_sql.y"
                               {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation = *(yyvsp[-2].relation_type);
      (yyval.join)->conditions.swap(*(yyvsp[0].condition_list));
    }
#line 2580 "yacc_sql.cpp"
    break;

  case 86: /* calc_stmt: CALC expression_list  */
#line 788 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2590 "yacc_sql.cpp"
    break;

  case 87: /* expression_list: expression alias  */
#line 797 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      if (nullptr != (yyvsp[0].string)) {
        (yyvsp[-1].expression)->set_alias((yyvsp[0].string));
      }
      (yyval.expression_list)->emplace_back((yyvsp[-1].expression));
      free((yyvsp[0].string));
    }
#line 2603 "yacc_sql.cpp"
    break;

  case 88: /* expression_list: expression alias COMMA expression_list  */
#line 806 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      }
      if (nullptr != (yyvsp[-2].string)) {
        (yyvsp[-3].expression)->set_alias((yyvsp[-2].string));
      }
      (yyval.expression_list)->emplace((yyval.expression_list)->begin(), (yyvsp[-3].expression));
      free((yyvsp[-2].string));
    }
#line 2620 "yacc_sql.cpp"
    break;

  case 89: /* aggre_type: ID  */
#line 820 "yacc_sql.y"
       { (yyval.string) = (yyvsp[0].string); }
#line 2626 "yacc_sql.cpp"
    break;

  case 90: /* aggre_list: %empty  */
#line 824 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 2634 "yacc_sql.cpp"
    break;

  case 91: /* aggre_list: expression_list  */
#line 827 "yacc_sql.y"
                      {
        (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 2642 "yacc_sql.cpp"
    break;

  case 92: /* expression: aggre_type LBRACE aggre_list RBRACE  */
#line 831 "yacc_sql.y"
                                        {
      (yyval.expression) = nullptr;
      if ((yyvsp[-1].expression_list) != nullptr) {
        (yyval.expression) = create_aggregate_expression((yyvsp[-3].string), (yyvsp[-1].expression_list), sql_string, &(yyloc));
      }else{
        (yyval.expression) = create_aggregate_expression("unsupport", nullptr, sql_string, &(yyloc));
      }
    }
#line 2655 "yacc_sql.cpp"
    break;

  case 93: /* expression: expression '+' expression  */
#line 839 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2663 "yacc_sql.cpp"
    break;

  case 94: /* expression: expression '-' expression  */
#line 842 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2671 "yacc_sql.cpp"
    break;

  case 95: /* expression: expression '*' expression  */
#line 845 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2679 "yacc_sql.cpp"
    break;

  case 96: /* expression: expression '/' expression  */
#line 848 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2687 "yacc_sql.cpp"
    break;

  case 97: /* expression: LBRACE expression RBRACE  */
#line 851 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2696 "yacc_sql.cpp"
    break;

  case 98: /* expression: '-' expression  */
#line 855 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2704 "yacc_sql.cpp"
    break;

  case 99: /* expression: value  */
#line 858 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));

    }
#line 2714 "yacc_sql.cpp"
    break;

  case 100: /* expression: rel_attr  */
#line 863 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 2725 "yacc_sql.cpp"
    break;

  case 101: /* expression: '*'  */
#line 869 "yacc_sql.y"
          {
      (yyval.expression) = new StarExpr();
    }
#line 2733 "yacc_sql.cpp"
    break;

  case 102: /* expression: select_stmt  */
#line 872 "yacc_sql.y"
                  {
      (yyval.expression) = new SubQueryExpr(&((yyvsp[0].sql_node)->selection));
    }
#line 2741 "yacc_sql.cpp"
    break;

  case 103: /* expression: LBRACE expression_list RBRACE  */
#line 875 "yacc_sql.y"
                                    {
      (yyval.expression) = new SubQueryExpr((yyvsp[-1].expression_list));
    }
#line 2749 "yacc_sql.cpp"
    break;

  case 104: /* rel_attr: ID  */
#line 881 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2759 "yacc_sql.cpp"
    break;

  case 105: /* rel_attr: ID DOT ID  */
#line 886 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2771 "yacc_sql.cpp"
    break;

  case 106: /* rel_attr: '*' DOT '*'  */
#line 893 "yacc_sql.y"
                  {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = "*";
      (yyval.rel_attr)->attribute_name = "*";
    }
#line 2781 "yacc_sql.cpp"
    break;

  case 107: /* rel_attr: ID DOT '*'  */
#line 898 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = "*";
      free((yyvsp[-2].string));
    }
#line 2792 "yacc_sql.cpp"
    break;

  case 108: /* relation: ID alias  */
#line 907 "yacc_sql.y"
             {
      if((yyvsp[0].string) != nullptr){
        (yyval.relation_type) = new std::pair<std::string, std::string>((yyvsp[-1].string), (yyvsp[0].string));
      } else {
        (yyval.relation_type) = new std::pair<std::string, std::string>((yyvsp[-1].string), "");
      }
    }
#line 2804 "yacc_sql.cpp"
    break;

  case 109: /* rel_list: relation  */
#line 916 "yacc_sql.y"
             {
      (yyval.relation_list) = new std::vector<std::pair<std::string, std::string>>;
      (yyval.relation_list)->push_back(*(yyvsp[0].relation_type));
      delete (yyvsp[0].relation_type);
    }
#line 2814 "yacc_sql.cpp"
    break;

  case 110: /* rel_list: relation COMMA rel_list  */
#line 921 "yacc_sql.y"
                              {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::pair<std::string, std::string>>;
      }

      (yyval.relation_list)->insert((yyval.relation_list)->begin(), *(yyvsp[-2].relation_type));
      delete (yyvsp[-2].relation_type);
    }
#line 2829 "yacc_sql.cpp"
    break;

  case 111: /* where: %empty  */
#line 935 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2837 "yacc_sql.cpp"
    break;

  case 112: /* where: WHERE condition_list  */
#line 938 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 2845 "yacc_sql.cpp"
    break;

  case 113: /* condition_list: %empty  */
#line 944 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2853 "yacc_sql.cpp"
    break;

  case 114: /* condition_list: condition  */
#line 947 "yacc_sql.y"
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 2863 "yacc_sql.cpp"
    break;

  case 115: /* condition_list: condition AND condition_list  */
#line 952 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyvsp[-2].condition)->is_or = false;
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 2874 "yacc_sql.cpp"
    break;

  case 116: /* condition_list: condition OR condition_list  */
#line 958 "yacc_sql.y"
                                  {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyvsp[-2].condition)->is_or = true;
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 2885 "yacc_sql.cpp"
    break;

  case 117: /* condition_list: condition COMMA condition_list  */
#line 964 "yacc_sql.y"
                                    {
        (yyval.condition_list) = (yyvsp[0].condition_list);
        (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
        delete (yyvsp[-2].condition);
    }
#line 2895 "yacc_sql.cpp"
    break;

  case 118: /* condition: expression comp_op expression  */
#line 972 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-2].expression);
      (yyval.condition)->right_expr = (yyvsp[0].expression);
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 2906 "yacc_sql.cpp"
    break;

  case 119: /* comp_op: EQ  */
#line 981 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 2912 "yacc_sql.cpp"
    break;

  case 120: /* comp_op: LT  */
#line 982 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 2918 "yacc_sql.cpp"
    break;

  case 121: /* comp_op: GT  */
#line 983 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 2924 "yacc_sql.cpp"
    break;

  case 122: /* comp_op: LE  */
#line 984 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 2930 "yacc_sql.cpp"
    break;

  case 123: /* comp_op: GE  */
#line 985 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 2936 "yacc_sql.cpp"
    break;

  case 124: /* comp_op: NE  */
#line 986 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 2942 "yacc_sql.cpp"
    break;

  case 125: /* comp_op: LIKE  */
#line 987 "yacc_sql.y"
           {(yyval.comp) = LIKE_OP; }
#line 2948 "yacc_sql.cpp"
    break;

  case 126: /* comp_op: NOT LIKE  */
#line 988 "yacc_sql.y"
               {(yyval.comp) = NOT_LIKE_OP; }
#line 2954 "yacc_sql.cpp"
    break;

  case 127: /* comp_op: IS  */
#line 989 "yacc_sql.y"
         {  (yyval.comp) = IS_NULL;}
#line 2960 "yacc_sql.cpp"
    break;

  case 128: /* comp_op: IS NOT  */
#line 990 "yacc_sql.y"
             {  (yyval.comp) = IS_NOT_NULL;}
#line 2966 "yacc_sql.cpp"
    break;

  case 129: /* comp_op: IN  */
#line 991 "yacc_sql.y"
         {  (yyval.comp) = IN_;}
#line 2972 "yacc_sql.cpp"
    break;

  case 130: /* comp_op: NOT IN  */
#line 992 "yacc_sql.y"
             {  (yyval.comp) = NOT_IN;}
#line 2978 "yacc_sql.cpp"
    break;

  case 131: /* group_by: %empty  */
#line 998 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 2986 "yacc_sql.cpp"
    break;

  case 132: /* group_by: GROUP BY expression_list  */
#line 1001 "yacc_sql.y"
                               {
      (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 2994 "yacc_sql.cpp"
    break;

  case 133: /* having_condition: %empty  */
#line 1007 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3002 "yacc_sql.cpp"
    break;

  case 134: /* having_condition: HAVING condition_list  */
#line 1010 "yacc_sql.y"
                            {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3010 "yacc_sql.cpp"
    break;

  case 135: /* order_unit: expression  */
#line 1016 "yacc_sql.y"
    {
        (yyval.order_unit) = new OrderBySqlNode(); // 默认是升序
        (yyval.order_unit)->expr = (yyvsp[0].expression);              // 提取表达式
        (yyval.order_unit)->is_asc = true;          // 设置为升序
    }
#line 3020 "yacc_sql.cpp"
    break;

  case 136: /* order_unit: expression DESC  */
#line 1023 "yacc_sql.y"
    {
        (yyval.order_unit) = new OrderBySqlNode(); // 解析带有 DESC 的排序单元
        (yyval.order_unit)->expr = (yyvsp[-1].expression);
        (yyval.order_unit)->is_asc = false;         // 设置为降序
    }
#line 3030 "yacc_sql.cpp"
    break;

  case 137: /* order_unit: expression ASC  */
#line 1030 "yacc_sql.y"
    {
        (yyval.order_unit) = new OrderBySqlNode(); // 解析带有 ASC 的排序单元
        (yyval.order_unit)->expr = (yyvsp[-1].expression);
        (yyval.order_unit)->is_asc = true;          // 设置为升序
    }
#line 3040 "yacc_sql.cpp"
    break;

  case 138: /* order_unit_list: order_unit  */
#line 1038 "yacc_sql.y"
        {
    (yyval.order_unit_list) = new std::vector<OrderBySqlNode>;
    (yyval.order_unit_list)->emplace_back(*(yyvsp[0].order_unit));
    delete (yyvsp[0].order_unit);
	}
#line 3050 "yacc_sql.cpp"
    break;

  case 139: /* order_unit_list: order_unit COMMA order_unit_list  */
#line 1045 "yacc_sql.y"
        {
    (yyvsp[0].order_unit_list)->emplace_back(*(yyvsp[-2].order_unit));
    (yyval.order_unit_list) = (yyvsp[0].order_unit_list);
    delete (yyvsp[-2].order_unit);
	}
#line 3060 "yacc_sql.cpp"
    break;

  case 140: /* opt_order_by: %empty  */
#line 1052 "yacc_sql.y"
                    {
   (yyval.order_unit_list) = nullptr;
  }
#line 3068 "yacc_sql.cpp"
    break;

  case 141: /* opt_order_by: ORDER BY order_unit_list  */
#line 1056 "yacc_sql.y"
        {
      (yyval.order_unit_list) = (yyvsp[0].order_unit_list);
      std::reverse((yyval.order_unit_list)->begin(),(yyval.order_unit_list)->end());
	}
#line 3077 "yacc_sql.cpp"
    break;

  case 142: /* alias: %empty  */
#line 1062 "yacc_sql.y"
                {
      (yyval.string) = nullptr;
    }
#line 3085 "yacc_sql.cpp"
    break;

  case 143: /* alias: ID  */
#line 1065 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3093 "yacc_sql.cpp"
    break;

  case 144: /* alias: AS ID  */
#line 1068 "yacc_sql.y"
            {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3101 "yacc_sql.cpp"
    break;

  case 145: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1087 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3110 "yacc_sql.cpp"
    break;

  case 146: /* set_variable_stmt: SET ID EQ value  */
#line 1095 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3122 "yacc_sql.cpp"
    break;

  case 147: /* set_variable_stmt: SET ID ID  */
#line 1103 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-1].string);
      (yyval.sql_node)->set_variable.value_ = (yyvsp[0].string);
      free((yyvsp[-1].string));
      free((yyvsp[0].string));
    }
#line 3134 "yacc_sql.cpp"
    break;


#line 3138 "yacc_sql.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1115 "yacc_sql.y"

//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
