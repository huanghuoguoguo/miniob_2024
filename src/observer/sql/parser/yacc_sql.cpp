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
  YYSYMBOL_DATA = 49,                      /* DATA  */
  YYSYMBOL_INFILE = 50,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 51,                   /* EXPLAIN  */
  YYSYMBOL_STORAGE = 52,                   /* STORAGE  */
  YYSYMBOL_FORMAT = 53,                    /* FORMAT  */
  YYSYMBOL_EQ = 54,                        /* EQ  */
  YYSYMBOL_LT = 55,                        /* LT  */
  YYSYMBOL_GT = 56,                        /* GT  */
  YYSYMBOL_LE = 57,                        /* LE  */
  YYSYMBOL_GE = 58,                        /* GE  */
  YYSYMBOL_NE = 59,                        /* NE  */
  YYSYMBOL_IS = 60,                        /* IS  */
  YYSYMBOL_NOT = 61,                       /* NOT  */
  YYSYMBOL_LIKE = 62,                      /* LIKE  */
  YYSYMBOL_NULL_ = 63,                     /* NULL_  */
  YYSYMBOL_NULLABLE = 64,                  /* NULLABLE  */
  YYSYMBOL_HAVING = 65,                    /* HAVING  */
  YYSYMBOL_IN = 66,                        /* IN  */
  YYSYMBOL_UNIQUE = 67,                    /* UNIQUE  */
  YYSYMBOL_OR = 68,                        /* OR  */
  YYSYMBOL_LIMIT = 69,                     /* LIMIT  */
  YYSYMBOL_NUMBER = 70,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 71,                     /* FLOAT  */
  YYSYMBOL_ID = 72,                        /* ID  */
  YYSYMBOL_SSS = 73,                       /* SSS  */
  YYSYMBOL_DATE_STR = 74,                  /* DATE_STR  */
  YYSYMBOL_75_ = 75,                       /* '+'  */
  YYSYMBOL_76_ = 76,                       /* '-'  */
  YYSYMBOL_77_ = 77,                       /* '*'  */
  YYSYMBOL_78_ = 78,                       /* '/'  */
  YYSYMBOL_UMINUS = 79,                    /* UMINUS  */
  YYSYMBOL_80_ = 80,                       /* '['  */
  YYSYMBOL_81_ = 81,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 82,                  /* $accept  */
  YYSYMBOL_commands = 83,                  /* commands  */
  YYSYMBOL_command_wrapper = 84,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 85,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 86,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 87,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 88,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 89,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 90,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 91,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 92,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 93,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 94,         /* create_index_stmt  */
  YYSYMBOL_drop_index_stmt = 95,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 96,         /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 97,             /* attr_def_list  */
  YYSYMBOL_attr_def = 98,                  /* attr_def  */
  YYSYMBOL_number = 99,                    /* number  */
  YYSYMBOL_type = 100,                     /* type  */
  YYSYMBOL_insert_stmt = 101,              /* insert_stmt  */
  YYSYMBOL_value = 102,                    /* value  */
  YYSYMBOL_value_list = 103,               /* value_list  */
  YYSYMBOL_storage_format = 104,           /* storage_format  */
  YYSYMBOL_delete_stmt = 105,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 106,              /* update_stmt  */
  YYSYMBOL_select_stmt = 107,              /* select_stmt  */
  YYSYMBOL_limit_block = 108,              /* limit_block  */
  YYSYMBOL_join_list = 109,                /* join_list  */
  YYSYMBOL_join = 110,                     /* join  */
  YYSYMBOL_calc_stmt = 111,                /* calc_stmt  */
  YYSYMBOL_expression_list = 112,          /* expression_list  */
  YYSYMBOL_aggre_type = 113,               /* aggre_type  */
  YYSYMBOL_aggre_list = 114,               /* aggre_list  */
  YYSYMBOL_expression = 115,               /* expression  */
  YYSYMBOL_rel_attr = 116,                 /* rel_attr  */
  YYSYMBOL_relation = 117,                 /* relation  */
  YYSYMBOL_rel_list = 118,                 /* rel_list  */
  YYSYMBOL_where = 119,                    /* where  */
  YYSYMBOL_condition_list = 120,           /* condition_list  */
  YYSYMBOL_condition = 121,                /* condition  */
  YYSYMBOL_comp_op = 122,                  /* comp_op  */
  YYSYMBOL_group_by = 123,                 /* group_by  */
  YYSYMBOL_having_condition = 124,         /* having_condition  */
  YYSYMBOL_order_unit = 125,               /* order_unit  */
  YYSYMBOL_order_unit_list = 126,          /* order_unit_list  */
  YYSYMBOL_opt_order_by = 127,             /* opt_order_by  */
  YYSYMBOL_load_data_stmt = 128,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 129,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 130,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 131             /* opt_semicolon  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  71
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   218

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  82
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  131
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  230

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   330


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
       2,     2,    77,    75,     2,    76,     2,    78,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    80,     2,    81,     2,     2,     2,     2,     2,     2,
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
      79
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   225,   225,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   256,   262,   267,   273,   279,   285,   291,
     298,   304,   313,   324,   338,   348,   372,   375,   388,   397,
     406,   415,   424,   433,   442,   451,   462,   465,   466,   467,
     468,   469,   470,   473,   487,   491,   495,   501,   518,   522,
     538,   542,   554,   557,   564,   576,   589,   631,   634,   640,
     643,   651,   661,   670,   679,   684,   695,   699,   702,   706,
     714,   717,   720,   723,   726,   730,   733,   738,   744,   747,
     750,   756,   761,   771,   776,   781,   795,   798,   804,   807,
     812,   818,   824,   831,   841,   842,   843,   844,   845,   846,
     847,   848,   849,   850,   851,   852,   858,   861,   867,   870,
     875,   882,   889,   897,   904,   912,   915,   922,   935,   943,
     953,   954
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
  "VALUES", "FROM", "WHERE", "AND", "ASC", "SET", "ON", "LOAD", "DATA",
  "INFILE", "EXPLAIN", "STORAGE", "FORMAT", "EQ", "LT", "GT", "LE", "GE",
  "NE", "IS", "NOT", "LIKE", "NULL_", "NULLABLE", "HAVING", "IN", "UNIQUE",
  "OR", "LIMIT", "NUMBER", "FLOAT", "ID", "SSS", "DATE_STR", "'+'", "'-'",
  "'*'", "'/'", "UMINUS", "'['", "']'", "$accept", "commands",
  "command_wrapper", "exit_stmt", "help_stmt", "sync_stmt", "begin_stmt",
  "commit_stmt", "rollback_stmt", "drop_table_stmt", "show_tables_stmt",
  "desc_table_stmt", "create_index_stmt", "drop_index_stmt",
  "create_table_stmt", "attr_def_list", "attr_def", "number", "type",
  "insert_stmt", "value", "value_list", "storage_format", "delete_stmt",
  "update_stmt", "select_stmt", "limit_block", "join_list", "join",
  "calc_stmt", "expression_list", "aggre_type", "aggre_list", "expression",
  "rel_attr", "relation", "rel_list", "where", "condition_list",
  "condition", "comp_op", "group_by", "having_condition", "order_unit",
  "order_unit_list", "opt_order_by", "load_data_stmt", "explain_stmt",
  "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-138)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-77)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       1,    -1,    94,   -15,   -15,   -65,    17,  -138,    13,    22,
     -31,  -138,  -138,  -138,  -138,  -138,     7,    31,     1,   102,
     109,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,    51,    53,   108,    61,    62,   -15,  -138,  -138,  -138,
      70,  -138,  -138,   -15,  -138,    20,  -138,  -138,  -138,   107,
       9,  -138,    98,  -138,  -138,    79,    80,   104,    99,   105,
    -138,  -138,  -138,  -138,   129,   110,    84,  -138,   112,   135,
       0,    90,  -138,   136,    83,   -15,   -15,   -15,   -15,   -15,
     -15,    93,   125,   124,   -15,    20,    96,   100,   103,   121,
     106,  -138,  -138,  -138,    20,  -138,  -138,   144,  -138,   -61,
     -61,  -138,  -138,  -138,   147,    95,   146,   -15,  -138,    60,
     124,    45,  -138,   137,   113,   149,   154,   111,  -138,  -138,
    -138,    93,    93,   174,   124,   -15,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,   126,   -58,  -138,  -138,   -15,  -138,   -15,
     -15,   -15,   168,  -138,  -138,  -138,  -138,  -138,  -138,    35,
     100,   159,   -15,   161,  -138,    95,   141,    93,   177,   164,
    -138,  -138,  -138,    52,  -138,  -138,  -138,   119,   122,   130,
    -138,  -138,   149,   142,   169,   -15,  -138,   -15,    95,   192,
     132,  -138,  -138,  -138,   172,  -138,  -138,   148,  -138,  -138,
     176,  -138,  -138,   -15,   -15,   187,    78,   150,  -138,  -138,
    -138,   199,   138,   143,  -138,  -138,   133,   -15,   139,  -138,
    -138,  -138,    -8,   181,  -138,  -138,  -138,  -138,   -15,  -138
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,    25,     0,     0,
       0,    26,    27,    28,    24,    23,     0,     0,     0,     0,
     130,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,     0,     0,     0,     0,     0,    58,    54,    55,
      91,    56,    57,     0,    88,     0,    86,    89,    73,     0,
      74,    87,     0,    31,    30,     0,     0,     0,     0,     0,
     128,     1,   131,     2,     0,     0,     0,    29,     0,     0,
       0,     0,    85,    60,     0,    77,     0,     0,     0,     0,
       0,     0,     0,    96,    98,     0,     0,     0,     0,     0,
       0,    90,    84,    92,     0,    59,    78,     0,    75,    80,
      81,    82,    83,    93,    94,    69,     0,    98,    64,     0,
      96,    99,   129,     0,     0,    36,     0,     0,    34,    61,
      79,     0,     0,     0,    96,     0,    97,   104,   105,   106,
     107,   108,   109,   112,     0,   110,   114,     0,    65,    98,
      98,    98,     0,    47,    48,    49,    52,    50,    51,    39,
       0,     0,     0,     0,    95,    69,     0,     0,   116,     0,
     113,   111,   115,   103,   102,   100,   101,     0,     0,     0,
      45,    44,    36,    62,     0,     0,    70,    98,    69,     0,
     118,    53,   127,    46,     0,    43,    37,     0,    35,    32,
       0,    72,    71,     0,    98,   125,    38,     0,    33,   117,
     119,     0,    67,     0,    41,    42,     0,     0,     0,    66,
      40,    63,   120,   123,   126,    68,   121,   122,     0,   124
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -138,  -138,   193,  -138,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,    28,    54,  -138,  -138,  -138,
     -41,   114,  -138,  -138,  -138,    74,  -138,  -137,    46,  -138,
      -4,  -138,  -138,   -44,  -138,  -127,    81,   -23,  -116,  -138,
    -138,  -138,  -138,  -138,   -13,  -138,  -138,  -138,  -138,  -138
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,   161,   125,   194,   159,    33,
      56,    84,   198,    34,    35,    57,   219,   134,   165,    37,
      58,    59,   107,    60,    61,   114,   115,   118,   120,   121,
     147,   190,   205,   223,   224,   212,    38,    39,    40,    73
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      62,   136,    80,     4,   171,   166,     1,    63,   172,    82,
      46,   226,     2,    41,    83,    42,    89,    90,     3,     4,
       5,     6,     7,     8,     9,    10,   102,    86,   186,    11,
      12,    13,    64,   174,   175,   176,    86,   227,    14,    15,
     166,    67,    79,   109,   110,   111,   112,    16,    47,    17,
     119,   202,    18,    65,   122,    48,    49,    50,    51,    52,
     178,    53,    54,    83,    66,    55,    43,    87,    88,    89,
      90,   201,   149,   119,    36,    87,    88,    89,    90,    68,
      69,   106,   108,    47,    87,    88,    89,    90,   210,   150,
      48,    49,    36,    51,    52,   -76,   179,   148,   180,   181,
      55,   132,    71,   173,   133,   119,   119,   119,    44,    81,
      45,   168,    72,   151,   137,   138,   139,   140,   141,   142,
     143,   144,   145,    74,    76,    75,   146,    87,    88,    89,
      90,   169,    85,    77,    78,    87,    88,    89,    90,   213,
      91,   214,   215,   119,   153,   154,   155,   156,   157,   158,
      94,    92,    93,    95,    97,    96,    99,    98,   184,   100,
     119,   101,   103,   104,   105,   113,   116,   117,   127,   123,
     130,   135,   124,   222,   131,   126,   160,   152,   128,   162,
     167,   200,   177,   163,   222,   183,   185,   170,   187,   189,
     191,   192,   193,   195,   197,   199,   203,   204,   206,   209,
     211,   207,   208,   217,   216,   221,   220,   218,   228,   225,
     196,    70,   164,   188,   182,   229,     0,     0,   129
};

static const yytype_int16 yycheck[] =
{
       4,   117,    46,    18,    62,   132,     5,    72,    66,    53,
      25,    19,    11,    14,    55,    16,    77,    78,    17,    18,
      19,    20,    21,    22,    23,    24,    26,    27,   165,    28,
      29,    30,    15,   149,   150,   151,    27,    45,    37,    38,
     167,    72,    46,    87,    88,    89,    90,    46,    63,    48,
      94,   188,    51,    40,    95,    70,    71,    72,    73,    74,
      25,    76,    77,   104,    42,    80,    67,    75,    76,    77,
      78,   187,    27,   117,     0,    75,    76,    77,    78,    72,
      49,    85,    86,    63,    75,    76,    77,    78,   204,    44,
      70,    71,    18,    73,    74,    25,    61,   120,    63,    64,
      80,     6,     0,   147,     9,   149,   150,   151,    14,    39,
      16,   134,     3,    68,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    72,    16,    72,    66,    75,    76,    77,
      78,   135,    25,    72,    72,    75,    76,    77,    78,    61,
      42,    63,    64,   187,    31,    32,    33,    34,    35,    36,
      46,    72,    72,    54,    25,    50,    72,    47,   162,    47,
     204,    26,    72,    27,    81,    72,    41,    43,    47,    73,
      26,    25,    72,   217,    27,    72,    27,    40,    72,    25,
       6,   185,    14,    72,   228,    26,    25,    61,    47,    12,
      26,    72,    70,    63,    52,    26,     4,    65,    26,   203,
      13,    53,    26,     4,    54,    72,    63,    69,    27,    70,
     182,    18,   131,   167,   160,   228,    -1,    -1,   104
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    11,    17,    18,    19,    20,    21,    22,    23,
      24,    28,    29,    30,    37,    38,    46,    48,    51,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,   101,   105,   106,   107,   111,   128,   129,
     130,    14,    16,    67,    14,    16,    25,    63,    70,    71,
      72,    73,    74,    76,    77,    80,   102,   107,   112,   113,
     115,   116,   112,    72,    15,    40,    42,    72,    72,    49,
      84,     0,     3,   131,    72,    72,    16,    72,    72,   112,
     115,    39,   115,   102,   103,    25,    27,    75,    76,    77,
      78,    42,    72,    72,    46,    54,    50,    25,    47,    72,
      47,    26,    26,    72,    27,    81,   112,   114,   112,   115,
     115,   115,   115,    72,   117,   118,    41,    43,   119,   115,
     120,   121,   102,    73,    72,    98,    72,    47,    72,   103,
      26,    27,     6,     9,   109,    25,   120,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    66,   122,   119,    27,
      44,    68,    40,    31,    32,    33,    34,    35,    36,   100,
      27,    97,    25,    72,   118,   110,   117,     6,   119,   112,
      61,    62,    66,   115,   120,   120,   120,    14,    25,    61,
      63,    64,    98,    26,   112,    25,   109,    47,   110,    12,
     123,    26,    72,    70,    99,    63,    97,    52,   104,    26,
     112,   120,   109,     4,    65,   124,    26,    53,    26,   112,
     120,    13,   127,    61,    63,    64,    54,     4,    69,   108,
      63,    72,   115,   125,   126,    70,    19,    45,    27,   126
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    82,    83,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    94,    95,    96,    97,    97,    98,    98,
      98,    98,    98,    98,    98,    98,    99,   100,   100,   100,
     100,   100,   100,   101,   102,   102,   102,   102,   102,   102,
     103,   103,   104,   104,   105,   106,   107,   108,   108,   109,
     109,   109,   110,   111,   112,   112,   113,   114,   114,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   116,   116,   117,   118,   118,   119,   119,   120,   120,
     120,   120,   120,   121,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   123,   123,   124,   124,
     125,   125,   125,   126,   126,   127,   127,   128,   129,   130,
     131,   131
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     8,     9,     5,     8,     0,     3,     5,     2,
       7,     6,     6,     4,     3,     3,     1,     1,     1,     1,
       1,     1,     1,     7,     1,     1,     1,     1,     1,     3,
       1,     3,     0,     4,     4,     5,    10,     0,     2,     0,
       3,     4,     3,     2,     1,     3,     1,     0,     1,     4,
       3,     3,     3,     3,     3,     2,     1,     1,     1,     1,
       3,     1,     3,     1,     1,     3,     0,     2,     0,     1,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     2,     1,     2,     0,     3,     0,     2,
       1,     2,     2,     1,     3,     0,     3,     7,     2,     4,
       0,     1
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
#line 226 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1819 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 256 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1828 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 262 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1836 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 267 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1844 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 273 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1852 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 279 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1860 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 285 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1868 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 291 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1878 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 298 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1886 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC ID  */
#line 304 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1896 "yacc_sql.cpp"
    break;

  case 32: /* create_index_stmt: CREATE INDEX ID ON ID LBRACE expression_list RBRACE  */
#line 314 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.index_name = (yyvsp[-5].string);
      create_index.relation_name = (yyvsp[-3].string);
      create_index.unique = false;
      create_index.columns.swap(*(yyvsp[-1].expression_list));
      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
    }
#line 1911 "yacc_sql.cpp"
    break;

  case 33: /* create_index_stmt: CREATE UNIQUE INDEX ID ON ID LBRACE expression_list RBRACE  */
#line 325 "yacc_sql.y"
     {
       (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
       CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
       create_index.index_name = (yyvsp[-5].string);
       create_index.relation_name = (yyvsp[-3].string);
       create_index.unique = true;
       create_index.columns.swap(*(yyvsp[-1].expression_list));
       free((yyvsp[-5].string));
       free((yyvsp[-3].string));
     }
#line 1926 "yacc_sql.cpp"
    break;

  case 34: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 339 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 1938 "yacc_sql.cpp"
    break;

  case 35: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format  */
#line 349 "yacc_sql.y"
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
#line 1963 "yacc_sql.cpp"
    break;

  case 36: /* attr_def_list: %empty  */
#line 372 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 1971 "yacc_sql.cpp"
    break;

  case 37: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 376 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 1985 "yacc_sql.cpp"
    break;

  case 38: /* attr_def: ID type LBRACE number RBRACE  */
#line 389 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-3].number);
      (yyval.attr_info)->name = (yyvsp[-4].string);
      (yyval.attr_info)->length = (yyvsp[-1].number);
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-4].string));
    }
#line 1998 "yacc_sql.cpp"
    break;

  case 39: /* attr_def: ID type  */
#line 398 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[0].number);
      (yyval.attr_info)->name = (yyvsp[-1].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-1].string));
    }
#line 2011 "yacc_sql.cpp"
    break;

  case 40: /* attr_def: ID type LBRACE number RBRACE NOT NULL_  */
#line 407 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-5].number);
      (yyval.attr_info)->name = (yyvsp[-6].string);
      (yyval.attr_info)->length = (yyvsp[-3].number);
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-6].string));
    }
#line 2024 "yacc_sql.cpp"
    break;

  case 41: /* attr_def: ID type LBRACE number RBRACE NULL_  */
#line 416 "yacc_sql.y"
        {
          (yyval.attr_info) = new AttrInfoSqlNode;
          (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
          (yyval.attr_info)->name = (yyvsp[-5].string);
          (yyval.attr_info)->length = (yyvsp[-2].number);
          (yyval.attr_info)->nullable = true;
          free((yyvsp[-5].string));
        }
#line 2037 "yacc_sql.cpp"
    break;

  case 42: /* attr_def: ID type LBRACE number RBRACE NULLABLE  */
#line 425 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-5].string));
    }
#line 2050 "yacc_sql.cpp"
    break;

  case 43: /* attr_def: ID type NOT NULL_  */
#line 434 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-2].number);
      (yyval.attr_info)->name = (yyvsp[-3].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = false;
      free((yyvsp[-3].string));
    }
#line 2063 "yacc_sql.cpp"
    break;

  case 44: /* attr_def: ID type NULLABLE  */
#line 443 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-2].string));
    }
#line 2076 "yacc_sql.cpp"
    break;

  case 45: /* attr_def: ID type NULL_  */
#line 452 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = true;
      free((yyvsp[-2].string));
    }
#line 2089 "yacc_sql.cpp"
    break;

  case 46: /* number: NUMBER  */
#line 462 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2095 "yacc_sql.cpp"
    break;

  case 47: /* type: INT_T  */
#line 465 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2101 "yacc_sql.cpp"
    break;

  case 48: /* type: STRING_T  */
#line 466 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2107 "yacc_sql.cpp"
    break;

  case 49: /* type: FLOAT_T  */
#line 467 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2113 "yacc_sql.cpp"
    break;

  case 50: /* type: VECTOR_T  */
#line 468 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2119 "yacc_sql.cpp"
    break;

  case 51: /* type: DATE_T  */
#line 469 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::DATES); }
#line 2125 "yacc_sql.cpp"
    break;

  case 52: /* type: TEXT_T  */
#line 470 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::TEXTS); }
#line 2131 "yacc_sql.cpp"
    break;

  case 53: /* insert_stmt: INSERT INTO ID VALUES LBRACE expression_list RBRACE  */
#line 474 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-4].string);
      if ((yyvsp[-1].expression_list) != nullptr) {
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[-1].expression_list));
        delete (yyvsp[-1].expression_list);
      }
      free((yyvsp[-4].string));
    }
#line 2145 "yacc_sql.cpp"
    break;

  case 54: /* value: NUMBER  */
#line 487 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2154 "yacc_sql.cpp"
    break;

  case 55: /* value: FLOAT  */
#line 491 "yacc_sql.y"
            {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2163 "yacc_sql.cpp"
    break;

  case 56: /* value: SSS  */
#line 495 "yacc_sql.y"
          {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2174 "yacc_sql.cpp"
    break;

  case 57: /* value: DATE_STR  */
#line 501 "yacc_sql.y"
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
#line 2196 "yacc_sql.cpp"
    break;

  case 58: /* value: NULL_  */
#line 518 "yacc_sql.y"
            {
      (yyval.value) = new Value();
      (yyloc) = (yylsp[0]);
    }
#line 2205 "yacc_sql.cpp"
    break;

  case 59: /* value: '[' value_list ']'  */
#line 522 "yacc_sql.y"
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
#line 2224 "yacc_sql.cpp"
    break;

  case 60: /* value_list: value  */
#line 538 "yacc_sql.y"
          {
        (yyval.value_list) = new std::vector<Value*>;
        (yyval.value_list)->push_back((yyvsp[0].value));
    }
#line 2233 "yacc_sql.cpp"
    break;

  case 61: /* value_list: value COMMA value_list  */
#line 542 "yacc_sql.y"
                            {
        if((yyvsp[0].value_list) != nullptr){
           (yyvsp[0].value_list)->push_back((yyvsp[-2].value));
           (yyval.value_list) = (yyvsp[0].value_list);
        }else{
           (yyval.value_list) = new std::vector<Value*>;
           (yyval.value_list)->push_back((yyvsp[-2].value));
        }
    }
#line 2247 "yacc_sql.cpp"
    break;

  case 62: /* storage_format: %empty  */
#line 554 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2255 "yacc_sql.cpp"
    break;

  case 63: /* storage_format: STORAGE FORMAT EQ ID  */
#line 558 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2263 "yacc_sql.cpp"
    break;

  case 64: /* delete_stmt: DELETE FROM ID where  */
#line 565 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        (yyval.sql_node)->deletion.conditions.swap(*(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2277 "yacc_sql.cpp"
    break;

  case 65: /* update_stmt: UPDATE ID SET condition_list where  */
#line 577 "yacc_sql.y"
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
#line 2292 "yacc_sql.cpp"
    break;

  case 66: /* select_stmt: SELECT expression_list FROM rel_list join_list where group_by having_condition opt_order_by limit_block  */
#line 590 "yacc_sql.y"
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
#line 2335 "yacc_sql.cpp"
    break;

  case 67: /* limit_block: %empty  */
#line 631 "yacc_sql.y"
    {
      (yyval.number) = -1;
    }
#line 2343 "yacc_sql.cpp"
    break;

  case 68: /* limit_block: LIMIT NUMBER  */
#line 634 "yacc_sql.y"
                   {
      (yyval.number) = (yyvsp[0].number);
    }
#line 2351 "yacc_sql.cpp"
    break;

  case 69: /* join_list: %empty  */
#line 640 "yacc_sql.y"
    {
      (yyval.join_list) = nullptr;
    }
#line 2359 "yacc_sql.cpp"
    break;

  case 70: /* join_list: JOIN join join_list  */
#line 643 "yacc_sql.y"
                          {
          if ((yyvsp[0].join_list) != nullptr) {
            (yyval.join_list) = (yyvsp[0].join_list);
          } else {
            (yyval.join_list) = new std::vector<JoinSqlNode>;
          }
          (yyval.join_list)->emplace((yyval.join_list)->begin(), *(yyvsp[-1].join));
        }
#line 2372 "yacc_sql.cpp"
    break;

  case 71: /* join_list: INNER JOIN join join_list  */
#line 651 "yacc_sql.y"
                                {
      if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new std::vector<JoinSqlNode>;
      }
      (yyval.join_list)->emplace((yyval.join_list)->begin(), *(yyvsp[-1].join));
    }
#line 2385 "yacc_sql.cpp"
    break;

  case 72: /* join: relation ON condition_list  */
#line 661 "yacc_sql.y"
                               {
      (yyval.join) = new JoinSqlNode;
      (yyval.join)->relation = (yyvsp[-2].string);
      free((yyvsp[-2].string));
      (yyval.join)->conditions.swap(*(yyvsp[0].condition_list));
    }
#line 2396 "yacc_sql.cpp"
    break;

  case 73: /* calc_stmt: CALC expression_list  */
#line 671 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2406 "yacc_sql.cpp"
    break;

  case 74: /* expression_list: expression  */
#line 680 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      (yyval.expression_list)->emplace_back((yyvsp[0].expression));
    }
#line 2415 "yacc_sql.cpp"
    break;

  case 75: /* expression_list: expression COMMA expression_list  */
#line 685 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      }
      (yyval.expression_list)->emplace((yyval.expression_list)->begin(), (yyvsp[-2].expression));
    }
#line 2428 "yacc_sql.cpp"
    break;

  case 76: /* aggre_type: ID  */
#line 695 "yacc_sql.y"
       { (yyval.string) = (yyvsp[0].string); }
#line 2434 "yacc_sql.cpp"
    break;

  case 77: /* aggre_list: %empty  */
#line 699 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 2442 "yacc_sql.cpp"
    break;

  case 78: /* aggre_list: expression_list  */
#line 702 "yacc_sql.y"
                      {
        (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 2450 "yacc_sql.cpp"
    break;

  case 79: /* expression: aggre_type LBRACE aggre_list RBRACE  */
#line 706 "yacc_sql.y"
                                        {
      (yyval.expression) = nullptr;
      if ((yyvsp[-1].expression_list) != nullptr) {
        (yyval.expression) = create_aggregate_expression((yyvsp[-3].string), (yyvsp[-1].expression_list), sql_string, &(yyloc));
      }else{
        (yyval.expression) = create_aggregate_expression("unsupport", nullptr, sql_string, &(yyloc));
      }
    }
#line 2463 "yacc_sql.cpp"
    break;

  case 80: /* expression: expression '+' expression  */
#line 714 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2471 "yacc_sql.cpp"
    break;

  case 81: /* expression: expression '-' expression  */
#line 717 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2479 "yacc_sql.cpp"
    break;

  case 82: /* expression: expression '*' expression  */
#line 720 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2487 "yacc_sql.cpp"
    break;

  case 83: /* expression: expression '/' expression  */
#line 723 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2495 "yacc_sql.cpp"
    break;

  case 84: /* expression: LBRACE expression RBRACE  */
#line 726 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2504 "yacc_sql.cpp"
    break;

  case 85: /* expression: '-' expression  */
#line 730 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2512 "yacc_sql.cpp"
    break;

  case 86: /* expression: value  */
#line 733 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));

    }
#line 2522 "yacc_sql.cpp"
    break;

  case 87: /* expression: rel_attr  */
#line 738 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 2533 "yacc_sql.cpp"
    break;

  case 88: /* expression: '*'  */
#line 744 "yacc_sql.y"
          {
      (yyval.expression) = new StarExpr();
    }
#line 2541 "yacc_sql.cpp"
    break;

  case 89: /* expression: select_stmt  */
#line 747 "yacc_sql.y"
                  {
      (yyval.expression) = new SubQueryExpr(&((yyvsp[0].sql_node)->selection));
    }
#line 2549 "yacc_sql.cpp"
    break;

  case 90: /* expression: LBRACE expression_list RBRACE  */
#line 750 "yacc_sql.y"
                                    {
      (yyval.expression) = new SubQueryExpr((yyvsp[-1].expression_list));
    }
#line 2557 "yacc_sql.cpp"
    break;

  case 91: /* rel_attr: ID  */
#line 756 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2567 "yacc_sql.cpp"
    break;

  case 92: /* rel_attr: ID DOT ID  */
#line 761 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2579 "yacc_sql.cpp"
    break;

  case 93: /* relation: ID  */
#line 771 "yacc_sql.y"
       {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2587 "yacc_sql.cpp"
    break;

  case 94: /* rel_list: relation  */
#line 776 "yacc_sql.y"
             {
      (yyval.relation_list) = new std::vector<std::string>();
      (yyval.relation_list)->push_back((yyvsp[0].string));
      free((yyvsp[0].string));
    }
#line 2597 "yacc_sql.cpp"
    break;

  case 95: /* rel_list: relation COMMA rel_list  */
#line 781 "yacc_sql.y"
                              {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }

      (yyval.relation_list)->insert((yyval.relation_list)->begin(), (yyvsp[-2].string));
      free((yyvsp[-2].string));
    }
#line 2612 "yacc_sql.cpp"
    break;

  case 96: /* where: %empty  */
#line 795 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2620 "yacc_sql.cpp"
    break;

  case 97: /* where: WHERE condition_list  */
#line 798 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);  
    }
#line 2628 "yacc_sql.cpp"
    break;

  case 98: /* condition_list: %empty  */
#line 804 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2636 "yacc_sql.cpp"
    break;

  case 99: /* condition_list: condition  */
#line 807 "yacc_sql.y"
                {
      (yyval.condition_list) = new std::vector<ConditionSqlNode>;
      (yyval.condition_list)->emplace_back(*(yyvsp[0].condition));
      delete (yyvsp[0].condition);
    }
#line 2646 "yacc_sql.cpp"
    break;

  case 100: /* condition_list: condition AND condition_list  */
#line 812 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyvsp[-2].condition)->is_or = false;
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 2657 "yacc_sql.cpp"
    break;

  case 101: /* condition_list: condition OR condition_list  */
#line 818 "yacc_sql.y"
                                  {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyvsp[-2].condition)->is_or = true;
      (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
      delete (yyvsp[-2].condition);
    }
#line 2668 "yacc_sql.cpp"
    break;

  case 102: /* condition_list: condition COMMA condition_list  */
#line 824 "yacc_sql.y"
                                    {
        (yyval.condition_list) = (yyvsp[0].condition_list);
        (yyval.condition_list)->emplace_back(*(yyvsp[-2].condition));
        delete (yyvsp[-2].condition);
    }
#line 2678 "yacc_sql.cpp"
    break;

  case 103: /* condition: expression comp_op expression  */
#line 832 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = (yyvsp[-2].expression);
      (yyval.condition)->right_expr = (yyvsp[0].expression);
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 2689 "yacc_sql.cpp"
    break;

  case 104: /* comp_op: EQ  */
#line 841 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 2695 "yacc_sql.cpp"
    break;

  case 105: /* comp_op: LT  */
#line 842 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 2701 "yacc_sql.cpp"
    break;

  case 106: /* comp_op: GT  */
#line 843 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 2707 "yacc_sql.cpp"
    break;

  case 107: /* comp_op: LE  */
#line 844 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 2713 "yacc_sql.cpp"
    break;

  case 108: /* comp_op: GE  */
#line 845 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 2719 "yacc_sql.cpp"
    break;

  case 109: /* comp_op: NE  */
#line 846 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 2725 "yacc_sql.cpp"
    break;

  case 110: /* comp_op: LIKE  */
#line 847 "yacc_sql.y"
           {(yyval.comp) = LIKE_OP; }
#line 2731 "yacc_sql.cpp"
    break;

  case 111: /* comp_op: NOT LIKE  */
#line 848 "yacc_sql.y"
               {(yyval.comp) = NOT_LIKE_OP; }
#line 2737 "yacc_sql.cpp"
    break;

  case 112: /* comp_op: IS  */
#line 849 "yacc_sql.y"
         {  (yyval.comp) = IS_NULL;}
#line 2743 "yacc_sql.cpp"
    break;

  case 113: /* comp_op: IS NOT  */
#line 850 "yacc_sql.y"
             {  (yyval.comp) = IS_NOT_NULL;}
#line 2749 "yacc_sql.cpp"
    break;

  case 114: /* comp_op: IN  */
#line 851 "yacc_sql.y"
         {  (yyval.comp) = IN_;}
#line 2755 "yacc_sql.cpp"
    break;

  case 115: /* comp_op: NOT IN  */
#line 852 "yacc_sql.y"
             {  (yyval.comp) = NOT_IN;}
#line 2761 "yacc_sql.cpp"
    break;

  case 116: /* group_by: %empty  */
#line 858 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 2769 "yacc_sql.cpp"
    break;

  case 117: /* group_by: GROUP BY expression_list  */
#line 861 "yacc_sql.y"
                               {
      (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 2777 "yacc_sql.cpp"
    break;

  case 118: /* having_condition: %empty  */
#line 867 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 2785 "yacc_sql.cpp"
    break;

  case 119: /* having_condition: HAVING condition_list  */
#line 870 "yacc_sql.y"
                            {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 2793 "yacc_sql.cpp"
    break;

  case 120: /* order_unit: expression  */
#line 876 "yacc_sql.y"
    {
        (yyval.order_unit) = new OrderBySqlNode(); // 默认是升序
        (yyval.order_unit)->expr = (yyvsp[0].expression);              // 提取表达式
        (yyval.order_unit)->is_asc = true;          // 设置为升序
    }
#line 2803 "yacc_sql.cpp"
    break;

  case 121: /* order_unit: expression DESC  */
#line 883 "yacc_sql.y"
    {
        (yyval.order_unit) = new OrderBySqlNode(); // 解析带有 DESC 的排序单元
        (yyval.order_unit)->expr = (yyvsp[-1].expression);
        (yyval.order_unit)->is_asc = false;         // 设置为降序
    }
#line 2813 "yacc_sql.cpp"
    break;

  case 122: /* order_unit: expression ASC  */
#line 890 "yacc_sql.y"
    {
        (yyval.order_unit) = new OrderBySqlNode(); // 解析带有 ASC 的排序单元
        (yyval.order_unit)->expr = (yyvsp[-1].expression);
        (yyval.order_unit)->is_asc = true;          // 设置为升序
    }
#line 2823 "yacc_sql.cpp"
    break;

  case 123: /* order_unit_list: order_unit  */
#line 898 "yacc_sql.y"
        {
    (yyval.order_unit_list) = new std::vector<OrderBySqlNode>;
    (yyval.order_unit_list)->emplace_back(*(yyvsp[0].order_unit));
    delete (yyvsp[0].order_unit);
	}
#line 2833 "yacc_sql.cpp"
    break;

  case 124: /* order_unit_list: order_unit COMMA order_unit_list  */
#line 905 "yacc_sql.y"
        {
    (yyvsp[0].order_unit_list)->emplace_back(*(yyvsp[-2].order_unit));
    (yyval.order_unit_list) = (yyvsp[0].order_unit_list);
    delete (yyvsp[-2].order_unit);
	}
#line 2843 "yacc_sql.cpp"
    break;

  case 125: /* opt_order_by: %empty  */
#line 912 "yacc_sql.y"
                    {
   (yyval.order_unit_list) = nullptr;
  }
#line 2851 "yacc_sql.cpp"
    break;

  case 126: /* opt_order_by: ORDER BY order_unit_list  */
#line 916 "yacc_sql.y"
        {
      (yyval.order_unit_list) = (yyvsp[0].order_unit_list);
      std::reverse((yyval.order_unit_list)->begin(),(yyval.order_unit_list)->end());
	}
#line 2860 "yacc_sql.cpp"
    break;

  case 127: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 923 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 2874 "yacc_sql.cpp"
    break;

  case 128: /* explain_stmt: EXPLAIN command_wrapper  */
#line 936 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 2883 "yacc_sql.cpp"
    break;

  case 129: /* set_variable_stmt: SET ID EQ value  */
#line 944 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 2895 "yacc_sql.cpp"
    break;


#line 2899 "yacc_sql.cpp"

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

#line 956 "yacc_sql.y"

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
