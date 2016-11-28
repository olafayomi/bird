/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         cf_parse
#define yylex           cf_lex
#define yyerror         cf_error
#define yydebug         cf_debug
#define yynerrs         cf_nerrs

#define yylval          cf_lval
#define yychar          cf_char

/* Copy the first part of user declarations.  */
#line 1 "cf-parse.y" /* yacc.c:339  */

/* Headers from ../../conf/confbase.Y */

#define PARSER 1

#include "nest/bird.h"
#include "conf/conf.h"
#include "lib/resource.h"
#include "lib/socket.h"
#include "lib/zeromq.h"
#include "lib/timer.h"
#include "lib/string.h"
#include "nest/protocol.h"
#include "nest/iface.h"
#include "nest/route.h"
#include "nest/cli.h"
#include "filter/filter.h"

/* FIXME: Turn on YYERROR_VERBOSE and work around lots of bison bugs? */

/* Defines from ../../conf/confbase.Y */

static void
check_u16(unsigned val)
{
  if (val > 0xFFFF)
    cf_error("Value %d out of range (0-65535)", val);
}

/* Headers from ../../sysdep/unix/config.Y */

#include "lib/unix.h"
#include <stdio.h>

/* Headers from ../../sysdep/unix/krt.Y */

#include "lib/krt.h"

/* Defines from ../../sysdep/unix/krt.Y */

#define THIS_KRT ((struct krt_config *) this_proto)
#define THIS_KIF ((struct kif_config *) this_proto)

/* Headers from ../../sysdep/linux/netlink.Y */

/* Headers from ../../nest/config.Y */

#include "nest/rt-dev.h"
#include "nest/password.h"
#include "nest/cmds.h"
#include "lib/lists.h"

/* Defines from ../../nest/config.Y */

static struct proto_config *this_proto;
static struct iface_patt *this_ipatt;
static struct iface_patt_node *this_ipn;
static struct roa_table_config *this_roa_table;
static list *this_p_list;
static struct password_item *this_p_item;
static int password_id;

static void
iface_patt_check(void)
{
  struct iface_patt_node *pn;

  WALK_LIST(pn, this_ipatt->ipn_list)
    if (!pn->pattern || pn->pxlen)
      cf_error("Interface name/mask expected, not IP prefix");
}


static inline void
reset_passwords(void)
{
  this_p_list = NULL;
}

static inline list *
get_passwords(void)
{
  list *rv = this_p_list;
  this_p_list = NULL;
  return rv;
}

#define DIRECT_CFG ((struct rt_dev_config *) this_proto)

/* Headers from ../../filter/config.Y */

/* Defines from ../../filter/config.Y */

#define P(a,b) ((a << 8) | b)

static inline u32 pair(u32 a, u32 b) { return (a << 16) | b; }
static inline u32 pair_a(u32 p) { return p >> 16; }
static inline u32 pair_b(u32 p) { return p & 0xFFFF; }


/*
 * Sets and their items are during parsing handled as lists, linked
 * through left ptr. The first item in a list also contains a pointer
 * to the last item in a list (right ptr). For convenience, even items
 * are handled as one-item lists. Lists are merged by f_merge_items().
 */
static int
f_valid_set_type(int type)
{
  switch (type)
  {
  case T_INT:
  case T_PAIR:
  case T_QUAD:
  case T_ENUM:
  case T_IP:
  case T_EC:
    return 1;

  default:
    return 0;
  }
}

static inline struct f_tree *
f_new_item(struct f_val from, struct f_val to)
{
  struct f_tree *t = f_new_tree();
  t->right = t;
  t->from = from;
  t->to = to;
  return t;
}

static inline struct f_tree *
f_merge_items(struct f_tree *a, struct f_tree *b)
{
  if (!a) return b;
  a->right->left = b;
  a->right = b->right;
  b->right = NULL;
  return a;
}

static inline struct f_tree *
f_new_pair_item(int fa, int ta, int fb, int tb)
{
  struct f_tree *t = f_new_tree();
  t->right = t;
  t->from.type = t->to.type = T_PAIR;
  t->from.val.i = pair(fa, fb);
  t->to.val.i = pair(ta, tb);
  return t;
}

static inline struct f_tree *
f_new_pair_set(int fa, int ta, int fb, int tb)
{
  struct f_tree *lst = NULL;
  int i;

  if ((fa == ta) || ((fb == 0) && (tb == 0xFFFF)))
    return f_new_pair_item(fa, ta, fb, tb);
  
  if ((ta < fa) || (tb < fb))
    cf_error( "From value cannot be higher that To value in pair sets");

  for (i = fa; i <= ta; i++)
    lst = f_merge_items(lst, f_new_pair_item(i, i, fb, tb));

  return lst;
}

#define EC_ALL 0xFFFFFFFF

static struct f_tree *
f_new_ec_item(u32 kind, u32 ipv4_used, u32 key, u32 vf, u32 vt)
{
  u64 fm, to;

  if (ipv4_used || (key >= 0x10000)) {
    check_u16(vf);
    if (vt == EC_ALL)
      vt = 0xFFFF;
    else
      check_u16(vt);
  }

  if (kind == EC_GENERIC) {
    fm = ec_generic(key, vf);
    to = ec_generic(key, vt);
  }
  else if (ipv4_used) {
    fm = ec_ip4(kind, key, vf);
    to = ec_ip4(kind, key, vt);
  }
  else if (key < 0x10000) {
    fm = ec_as2(kind, key, vf);
    to = ec_as2(kind, key, vt);
  }
  else {
    fm = ec_as4(kind, key, vf);
    to = ec_as4(kind, key, vt);
  }

  struct f_tree *t = f_new_tree();
  t->right = t;
  t->from.type = t->to.type = T_EC;
  t->from.val.ec = fm;
  t->to.val.ec = to;
  return t;
}

static inline struct f_inst *
f_generate_empty(struct f_inst *dyn)
{ 
  struct f_inst *e = f_new_inst();
  e->code = 'E';

  switch (dyn->aux & EAF_TYPE_MASK) {
    case EAF_TYPE_AS_PATH:
      e->aux = T_PATH;
      break;
    case EAF_TYPE_INT_SET:
      e->aux = T_CLIST;
      break;
    case EAF_TYPE_EC_SET:
      e->aux = T_ECLIST;
      break;
    default:
      cf_error("Can't empty that attribute");
  }

  dyn->code = P('e','S');
  dyn->a1.p = e;
  return dyn;
}


static inline struct f_inst *
f_generate_dpair(struct f_inst *t1, struct f_inst *t2)
{
  struct f_inst *rv;

  if ((t1->code == 'c') && (t2->code == 'c')) {
    if ((t1->aux != T_INT) || (t2->aux != T_INT))
      cf_error( "Can't operate with value of non-integer type in pair constructor");

    check_u16(t1->a2.i);
    check_u16(t2->a2.i);

    rv = f_new_inst();
    rv->code = 'c';
    rv->aux = T_PAIR;
    rv->a2.i = pair(t1->a2.i, t2->a2.i);
  }
  else {
    rv = f_new_inst();
    rv->code = P('m', 'p');
    rv->a1.p = t1;
    rv->a2.p = t2;
  }

  return rv;
}

static inline struct f_inst *
f_generate_ec(u16 kind, struct f_inst *tk, struct f_inst *tv)
{
  struct f_inst *rv;
  int c1 = 0, c2 = 0, ipv4_used = 0;
  u32 key = 0, val2 = 0;

  if (tk->code == 'c') {
    c1 = 1;

    if (tk->aux == T_INT) {
      ipv4_used = 0; key = tk->a2.i;
    }
    else if (tk->aux == T_QUAD) {
      ipv4_used = 1; key = tk->a2.i;
    }
    else
      cf_error("Can't operate with key of non-integer/IPv4 type in EC constructor");
  }

#ifndef IPV6
  /* IP->Quad implicit conversion */
  else if (tk->code == 'C') {
    c1 = 1;
    struct f_val *val = tk->a1.p;

    if (val->type == T_INT) {
      ipv4_used = 0; key = val->val.i;
    }
    else if (val->type == T_QUAD) {
      ipv4_used = 1; key = val->val.i;
    }
    else if (val->type == T_IP) {
      ipv4_used = 1; key = ipa_to_u32(val->val.px.ip);
    }
    else
      cf_error("Can't operate with key of non-integer/IPv4 type in EC constructor");
  }
#endif

  if (tv->code == 'c') {
    if (tv->aux != T_INT)
      cf_error("Can't operate with value of non-integer type in EC constructor");
    c2 = 1;
    val2 = tv->a2.i;
  }

  if (c1 && c2) {
    u64 ec;
  
    if (kind == EC_GENERIC) {
      ec = ec_generic(key, val2);
    }
    else if (ipv4_used) {
      check_u16(val2);
      ec = ec_ip4(kind, key, val2);
    }
    else if (key < 0x10000) {
      ec = ec_as2(kind, key, val2);
    }
    else {
      check_u16(val2);
      ec = ec_as4(kind, key, val2);
    }

    NEW_F_VAL;
    rv = f_new_inst();
    rv->code = 'C';
    rv->a1.p = val;    
    val->type = T_EC;
    val->val.ec = ec;
  }
  else {
    rv = f_new_inst();
    rv->code = P('m','c');
    rv->aux = kind;
    rv->a1.p = tk;
    rv->a2.p = tv;
  }

  return rv;
}



/* Headers from ../../proto/bfd/config.Y */

#include "proto/bfd/bfd.h"

/* Defines from ../../proto/bfd/config.Y */

#define BFD_CFG ((struct bfd_config *) this_proto)
#define BFD_IFACE ((struct bfd_iface_config *) this_ipatt)
#define BFD_NEIGHBOR this_bfd_neighbor

static struct bfd_neighbor *this_bfd_neighbor;

extern struct bfd_config *bfd_cf;

/* Headers from ../../proto/bgp/config.Y */

#include "proto/bgp/bgp.h"

/* Defines from ../../proto/bgp/config.Y */

#define BGP_CFG ((struct bgp_config *) this_proto)

/* Headers from ../../proto/ospf/config.Y */

#include "proto/ospf/ospf.h"

/* Defines from ../../proto/ospf/config.Y */

#define OSPF_CFG ((struct ospf_config *) this_proto)
#define OSPF_PATT ((struct ospf_iface_patt *) this_ipatt)

static struct ospf_area_config *this_area;
static struct nbma_node *this_nbma;
static list *this_nets;
static struct area_net_config *this_pref;
static struct ospf_stubnet_config *this_stubnet;

static inline int ospf_cfg_is_v2(void) { return OSPF_CFG->ospf2; }
static inline int ospf_cfg_is_v3(void) { return ! OSPF_CFG->ospf2; }

static void
ospf_iface_finish(void)
{
  struct ospf_iface_patt *ip = OSPF_PATT;

  if (ip->deadint == 0)
    ip->deadint = ip->deadc * ip->helloint;

  if (ip->waitint == 0)
    ip->waitint = ip->deadc * ip->helloint;

  ip->passwords = get_passwords();

  if ((ip->autype == OSPF_AUTH_CRYPT) && (ip->helloint < 5))
    log(L_WARN "Hello or poll interval less that 5 makes cryptographic authenication prone to replay attacks");

  if ((ip->autype == OSPF_AUTH_NONE) && (ip->passwords != NULL))
    log(L_WARN "Password option without authentication option does not make sense");
}

static void
ospf_area_finish(void)
{
  if ((this_area->areaid == 0) && (this_area->type != OPT_E))
    cf_error("Backbone area cannot be stub/NSSA");

  if (this_area->summary && (this_area->type == OPT_E))
    cf_error("Only stub/NSSA areas can use summary propagation");

  if (this_area->default_nssa && ((this_area->type != OPT_N) || ! this_area->summary))
    cf_error("Only NSSA areas with summary propagation can use NSSA default route");

  if ((this_area->default_cost & LSA_EXT3_EBIT) && ! this_area->default_nssa)
    cf_error("Only NSSA default route can use type 2 metric");
}

static void
ospf_proto_finish(void)
{
  struct ospf_config *cf = OSPF_CFG;

  if (EMPTY_LIST(cf->area_list))
    cf_error( "No configured areas in OSPF");

  int areano = 0;
  int backbone = 0;
  int nssa = 0;
  struct ospf_area_config *ac;
  WALK_LIST(ac, cf->area_list)
  {
    areano++;
    if (ac->areaid == 0)
      backbone = 1;
    if (ac->type == OPT_N)
      nssa = 1;
  }

  cf->abr = areano > 1;

  /* Route export or NSSA translation (RFC 3101 3.1) */
  cf->asbr = (this_proto->out_filter != FILTER_REJECT) || (nssa && cf->abr);

  if (cf->abr && !backbone)
  {
    struct ospf_area_config *ac = cfg_allocz(sizeof(struct ospf_area_config));
    ac->type = OPT_E; /* Backbone is non-stub */
    add_head(&cf->area_list, NODE ac);
    init_list(&ac->patt_list);
    init_list(&ac->net_list);
    init_list(&ac->enet_list);
    init_list(&ac->stubnet_list);
  }

  if (!cf->abr && !EMPTY_LIST(cf->vlink_list))
    cf_error("Vlinks cannot be used on single area router");

  if (cf->asbr && (areano == 1) && (this_area->type == 0))
    cf_error("ASBR must be in non-stub area");
}

static inline void
ospf_check_defcost(int cost)
{
  if ((cost <= 0) || (cost >= LSINFINITY))
   cf_error("Default cost must be in range 1-%d", LSINFINITY-1);
}

static inline void
ospf_check_auth(void)
{
  if (ospf_cfg_is_v3())
    cf_error("Authentication not supported in OSPFv3");
}


/* Headers from ../../proto/pipe/config.Y */

#include "proto/pipe/pipe.h"

/* Defines from ../../proto/pipe/config.Y */

#define PIPE_CFG ((struct pipe_config *) this_proto)

/* Headers from ../../proto/rip/config.Y */

#include "proto/rip/rip.h"
#include "nest/iface.h"

/* Defines from ../../proto/rip/config.Y */

#define RIP_CFG ((struct rip_proto_config *) this_proto)
#define RIP_IPATT ((struct rip_patt *) this_ipatt)

#ifdef IPV6
#define RIP_DEFAULT_TTL_SECURITY 2
#else
#define RIP_DEFAULT_TTL_SECURITY 0
#endif

/* Headers from ../../proto/static/config.Y */

#include "proto/static/static.h"

/* Defines from ../../proto/static/config.Y */

#define STATIC_CFG ((struct static_config *) this_proto)
static struct static_route *this_srt, *this_srt_nh, *last_srt_nh;

/* Headers from ../../proto/sdn/config.Y */

#include "proto/sdn/sdn.h"
#include "nest/iface.h"

/* Defines from ../../proto/sdn/config.Y */

#define SDN_CFG ((struct sdn_proto_config *) this_proto)
#define SDN_IPATT ((struct sdn_patt *) this_ipatt)
#define SDN_DEFAULT_TTL_SECURITY 0


#line 606 "cf-parse.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "cf-parse.tab.h".  */
#ifndef YY_CF_CF_PARSE_TAB_H_INCLUDED
# define YY_CF_CF_PARSE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int cf_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    END = 258,
    CLI_MARKER = 259,
    INVALID_TOKEN = 260,
    ELSECOL = 261,
    DDOT = 262,
    GEQ = 263,
    LEQ = 264,
    NEQ = 265,
    AND = 266,
    OR = 267,
    PO = 268,
    PC = 269,
    NUM = 270,
    ENUM = 271,
    RTRID = 272,
    IPA = 273,
    SYM = 274,
    TEXT = 275,
    PREFIX_DUMMY = 276,
    DEFINE = 277,
    ON = 278,
    OFF = 279,
    YES = 280,
    NO = 281,
    S = 282,
    MS = 283,
    US = 284,
    PORT = 285,
    LOG = 286,
    SYSLOG = 287,
    ALL = 288,
    DEBUG = 289,
    TRACE = 290,
    INFO = 291,
    REMOTE = 292,
    WARNING = 293,
    ERROR = 294,
    AUTH = 295,
    FATAL = 296,
    BUG = 297,
    STDERR = 298,
    SOFT = 299,
    TIMEFORMAT = 300,
    ISO = 301,
    OLD = 302,
    SHORT = 303,
    LONG = 304,
    BASE = 305,
    NAME = 306,
    CONFIRM = 307,
    UNDO = 308,
    CHECK = 309,
    TIMEOUT = 310,
    LATENCY = 311,
    LIMIT = 312,
    WATCHDOG = 313,
    CONFIGURE = 314,
    DOWN = 315,
    KERNEL = 316,
    PERSIST = 317,
    SCAN = 318,
    TIME = 319,
    LEARN = 320,
    DEVICE = 321,
    ROUTES = 322,
    GRACEFUL = 323,
    RESTART = 324,
    KRT_SOURCE = 325,
    KRT_METRIC = 326,
    TABLE = 327,
    KRT_PREFSRC = 328,
    KRT_REALM = 329,
    ROUTER = 330,
    ID = 331,
    PROTOCOL = 332,
    TEMPLATE = 333,
    PREFERENCE = 334,
    DISABLED = 335,
    DIRECT = 336,
    INTERFACE = 337,
    IMPORT = 338,
    EXPORT = 339,
    FILTER = 340,
    NONE = 341,
    STATES = 342,
    FILTERS = 343,
    RECEIVE = 344,
    ACTION = 345,
    WARN = 346,
    BLOCK = 347,
    DISABLE = 348,
    KEEP = 349,
    FILTERED = 350,
    PASSWORD = 351,
    FROM = 352,
    PASSIVE = 353,
    TO = 354,
    EVENTS = 355,
    PACKETS = 356,
    PROTOCOLS = 357,
    INTERFACES = 358,
    PRIMARY = 359,
    STATS = 360,
    COUNT = 361,
    FOR = 362,
    COMMANDS = 363,
    PREEXPORT = 364,
    NOEXPORT = 365,
    GENERATE = 366,
    ROA = 367,
    LISTEN = 368,
    BGP = 369,
    V6ONLY = 370,
    DUAL = 371,
    ADDRESS = 372,
    PASSWORDS = 373,
    DESCRIPTION = 374,
    SORTED = 375,
    RELOAD = 376,
    IN = 377,
    OUT = 378,
    MRTDUMP = 379,
    MESSAGES = 380,
    RESTRICT = 381,
    MEMORY = 382,
    IGP_METRIC = 383,
    CLASS = 384,
    DSCP = 385,
    WAIT = 386,
    MAX = 387,
    FLUSH = 388,
    AS = 389,
    SHOW = 390,
    STATUS = 391,
    SUMMARY = 392,
    ROUTE = 393,
    SYMBOLS = 394,
    ADD = 395,
    DELETE = 396,
    DUMP = 397,
    RESOURCES = 398,
    SOCKETS = 399,
    NEIGHBORS = 400,
    ATTRIBUTES = 401,
    EVAL = 402,
    ECHO = 403,
    ENABLE = 404,
    FUNCTION = 405,
    PRINT = 406,
    PRINTN = 407,
    UNSET = 408,
    RETURN = 409,
    ACCEPT = 410,
    REJECT = 411,
    QUITBIRD = 412,
    INT = 413,
    BOOL = 414,
    IP = 415,
    PREFIX = 416,
    PAIR = 417,
    QUAD = 418,
    EC = 419,
    SET = 420,
    STRING = 421,
    BGPMASK = 422,
    BGPPATH = 423,
    CLIST = 424,
    ECLIST = 425,
    IF = 426,
    THEN = 427,
    ELSE = 428,
    CASE = 429,
    TRUE = 430,
    FALSE = 431,
    RT = 432,
    RO = 433,
    UNKNOWN = 434,
    GENERIC = 435,
    GW = 436,
    NET = 437,
    MASK = 438,
    PROTO = 439,
    SOURCE = 440,
    SCOPE = 441,
    CAST = 442,
    DEST = 443,
    IFNAME = 444,
    IFINDEX = 445,
    LEN = 446,
    DEFINED = 447,
    CONTAINS = 448,
    RESET = 449,
    PREPEND = 450,
    FIRST = 451,
    LAST = 452,
    MATCH = 453,
    ROA_CHECK = 454,
    EMPTY = 455,
    WHERE = 456,
    BFD = 457,
    MIN = 458,
    IDLE = 459,
    RX = 460,
    TX = 461,
    INTERVAL = 462,
    MULTIPLIER = 463,
    MULTIHOP = 464,
    NEIGHBOR = 465,
    DEV = 466,
    LOCAL = 467,
    SESSIONS = 468,
    HOLD = 469,
    CONNECT = 470,
    RETRY = 471,
    KEEPALIVE = 472,
    STARTUP = 473,
    VIA = 474,
    NEXT = 475,
    HOP = 476,
    SELF = 477,
    DEFAULT = 478,
    PATH = 479,
    METRIC = 480,
    START = 481,
    DELAY = 482,
    FORGET = 483,
    AFTER = 484,
    BGP_PATH = 485,
    BGP_LOCAL_PREF = 486,
    BGP_MED = 487,
    BGP_ORIGIN = 488,
    BGP_NEXT_HOP = 489,
    BGP_ATOMIC_AGGR = 490,
    BGP_AGGREGATOR = 491,
    BGP_COMMUNITY = 492,
    BGP_EXT_COMMUNITY = 493,
    RR = 494,
    RS = 495,
    CLIENT = 496,
    CLUSTER = 497,
    AS4 = 498,
    ADVERTISE = 499,
    IPV4 = 500,
    CAPABILITIES = 501,
    PREFER = 502,
    OLDER = 503,
    MISSING = 504,
    LLADDR = 505,
    DROP = 506,
    IGNORE = 507,
    REFRESH = 508,
    INTERPRET = 509,
    COMMUNITIES = 510,
    BGP_ORIGINATOR_ID = 511,
    BGP_CLUSTER_LIST = 512,
    IGP = 513,
    GATEWAY = 514,
    RECURSIVE = 515,
    MED = 516,
    TTL = 517,
    SECURITY = 518,
    DETERMINISTIC = 519,
    SECONDARY = 520,
    ALLOW = 521,
    PATHS = 522,
    AWARE = 523,
    LINK = 524,
    OSPF = 525,
    AREA = 526,
    OSPF_METRIC1 = 527,
    OSPF_METRIC2 = 528,
    OSPF_TAG = 529,
    OSPF_ROUTER_ID = 530,
    RFC1583COMPAT = 531,
    STUB = 532,
    TICK = 533,
    COST = 534,
    COST2 = 535,
    RETRANSMIT = 536,
    HELLO = 537,
    TRANSMIT = 538,
    PRIORITY = 539,
    DEAD = 540,
    TYPE = 541,
    BROADCAST = 542,
    BCAST = 543,
    NONBROADCAST = 544,
    NBMA = 545,
    POINTOPOINT = 546,
    PTP = 547,
    POINTOMULTIPOINT = 548,
    PTMP = 549,
    SIMPLE = 550,
    AUTHENTICATION = 551,
    STRICT = 552,
    CRYPTOGRAPHIC = 553,
    ELIGIBLE = 554,
    POLL = 555,
    NETWORKS = 556,
    HIDDEN = 557,
    VIRTUAL = 558,
    ONLY = 559,
    BUFFER = 560,
    LARGE = 561,
    NORMAL = 562,
    STUBNET = 563,
    TAG = 564,
    EXTERNAL = 565,
    LSADB = 566,
    ECMP = 567,
    WEIGHT = 568,
    NSSA = 569,
    TRANSLATOR = 570,
    STABILITY = 571,
    GLOBAL = 572,
    LSID = 573,
    INSTANCE = 574,
    REAL = 575,
    NETMASK = 576,
    LENGTH = 577,
    MERGE = 578,
    LSA = 579,
    SUPPRESSION = 580,
    TOPOLOGY = 581,
    STATE = 582,
    PIPE = 583,
    PEER = 584,
    MODE = 585,
    OPAQUE = 586,
    TRANSPARENT = 587,
    RIP = 588,
    INFINITY = 589,
    PERIOD = 590,
    GARBAGE = 591,
    MULTICAST = 592,
    QUIET = 593,
    NOLISTEN = 594,
    VERSION1 = 595,
    PLAINTEXT = 596,
    MD5 = 597,
    HONOR = 598,
    NEVER = 599,
    ALWAYS = 600,
    RIP_METRIC = 601,
    RIP_TAG = 602,
    STATIC = 603,
    PROHIBIT = 604,
    MULTIPATH = 605,
    BLACKHOLE = 606,
    UNREACHABLE = 607,
    SDN = 608,
    UNIXSOCKET = 609
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 535 "cf-parse.y" /* yacc.c:355  */

  int i;
  u32 i32;
  ip_addr a;
  struct symbol *s;
  char *t;
  struct rtable_config *r;
  struct f_inst *x;
  struct filter *f;
  struct f_tree *e;
  struct f_trie *trie;
  struct f_val v;
  struct f_path_mask *h;
  struct password_item *p;
  struct rt_show_data *ra;
  struct roa_show_data *ro;
  struct sym_show_data *sd;
  struct lsadb_show_data *ld;
  struct iface *iface;
  struct roa_table *rot;
  void *g;
  bird_clock_t time;
  struct prefix px;
  struct proto_spec ps;
  struct timeformat *tf;

#line 1028 "cf-parse.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE cf_lval;

int cf_parse (void);

#endif /* !YY_CF_CF_PARSE_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 1045 "cf-parse.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  68
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2788

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  376
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  267
/* YYNRULES -- Number of rules.  */
#define YYNRULES  877
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1657

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   609

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,     2,     2,     2,    30,     2,     2,
     367,   368,    28,    26,   372,    27,    32,    29,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   369,   366,
      23,    22,    24,   373,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   374,     2,   375,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   370,     2,   371,    25,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   257,   258,   259,   260,   261,   262,   263,   264,   265,
     266,   267,   268,   269,   270,   271,   272,   273,   274,   275,
     276,   277,   278,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   309,   310,   311,   312,   313,   314,   315,
     316,   317,   318,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   332,   333,   334,   335,
     336,   337,   338,   339,   340,   341,   342,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   731,   731,   732,   735,   737,   745,   754,   755,   756,
     763,   764,   765,   773,   774,   775,   776,   777,   778,   784,
     785,   792,   793,   797,   804,   805,   809,   813,   820,   828,
     829,   836,   837,   843,   852,   853,   857,   862,   863,   867,
     868,   872,   873,   877,   878,   879,   880,   881,   882,   883,
     884,   885,   891,   892,   902,   903,   904,   905,   908,   909,
     910,   911,   912,   913,   917,   923,   924,   925,   926,   934,
     937,   944,   947,   950,   953,   957,   958,   962,   963,   964,
     972,   977,   978,   982,   989,   990,   996,  1001,  1005,  1019,
    1033,  1034,  1038,  1039,  1040,  1051,  1053,  1055,  1059,  1060,
    1061,  1062,  1067,  1073,  1074,  1078,  1086,  1090,  1092,  1098,
    1099,  1106,  1107,  1111,  1117,  1121,  1127,  1137,  1139,  1143,
    1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,  1153,
    1154,  1158,  1159,  1160,  1161,  1165,  1166,  1167,  1168,  1169,
    1173,  1179,  1183,  1191,  1192,  1200,  1208,  1209,  1210,  1214,
    1215,  1219,  1224,  1225,  1229,  1231,  1239,  1243,  1244,  1250,
    1257,  1258,  1259,  1263,  1271,  1277,  1278,  1279,  1283,  1284,
    1288,  1289,  1290,  1291,  1292,  1293,  1299,  1300,  1301,  1305,
    1306,  1310,  1311,  1317,  1318,  1321,  1323,  1327,  1328,  1332,
    1350,  1351,  1352,  1353,  1354,  1355,  1363,  1366,  1369,  1372,
    1376,  1377,  1380,  1383,  1387,  1391,  1396,  1402,  1409,  1414,
    1419,  1424,  1428,  1432,  1436,  1445,  1453,  1457,  1464,  1465,
    1466,  1471,  1475,  1482,  1489,  1493,  1501,  1502,  1503,  1508,
    1512,  1515,  1516,  1517,  1518,  1519,  1520,  1521,  1526,  1531,
    1539,  1546,  1553,  1561,  1563,  1565,  1567,  1569,  1571,  1573,
    1575,  1578,  1582,  1588,  1589,  1590,  1594,  1595,  1601,  1603,
    1605,  1607,  1609,  1611,  1615,  1619,  1622,  1626,  1627,  1628,
    1632,  1633,  1634,  1642,  1642,  1652,  1656,  1657,  1658,  1659,
    1660,  1661,  1662,  1663,  1664,  1665,  1666,  1667,  1668,  1689,
    1700,  1701,  1708,  1709,  1716,  1725,  1729,  1733,  1757,  1758,
    1762,  1775,  1775,  1791,  1792,  1795,  1796,  1800,  1803,  1812,
    1825,  1826,  1827,  1828,  1829,  1833,  1841,  1842,  1843,  1844,
    1845,  1849,  1852,  1853,  1854,  1858,  1861,  1870,  1873,  1874,
    1875,  1876,  1880,  1881,  1882,  1886,  1887,  1888,  1889,  1893,
    1894,  1895,  1896,  1900,  1901,  1905,  1906,  1910,  1917,  1918,
    1919,  1920,  1927,  1928,  1931,  1932,  1939,  1951,  1952,  1956,
    1957,  1961,  1962,  1963,  1964,  1965,  1969,  1970,  1971,  1975,
    1976,  1977,  1978,  1979,  1980,  1981,  1982,  1983,  1984,  1985,
    1989,  1990,  2000,  2004,  2027,  2041,  2042,  2043,  2044,  2045,
    2046,  2047,  2048,  2049,  2050,  2054,  2055,  2056,  2057,  2058,
    2059,  2060,  2061,  2062,  2063,  2064,  2065,  2066,  2067,  2068,
    2069,  2071,  2072,  2073,  2075,  2077,  2079,  2081,  2082,  2083,
    2084,  2085,  2095,  2096,  2097,  2098,  2099,  2100,  2101,  2103,
    2104,  2109,  2132,  2133,  2134,  2135,  2136,  2137,  2141,  2144,
    2145,  2146,  2154,  2161,  2170,  2171,  2175,  2181,  2191,  2200,
    2206,  2211,  2218,  2223,  2229,  2230,  2231,  2239,  2240,  2241,
    2242,  2243,  2249,  2261,  2262,  2263,  2264,  2267,  2269,  2273,
    2277,  2288,  2289,  2290,  2291,  2292,  2293,  2296,  2298,  2301,
    2303,  2306,  2309,  2314,  2315,  2316,  2320,  2321,  2325,  2326,
    2329,  2348,  2354,  2378,  2380,  2381,  2385,  2386,  2387,  2388,
    2389,  2390,  2396,  2397,  2398,  2399,  2400,  2401,  2402,  2403,
    2404,  2405,  2406,  2407,  2408,  2409,  2410,  2411,  2412,  2413,
    2414,  2415,  2416,  2417,  2418,  2419,  2420,  2421,  2422,  2423,
    2424,  2425,  2426,  2427,  2428,  2429,  2430,  2431,  2432,  2438,
    2439,  2440,  2441,  2442,  2443,  2444,  2445,  2446,  2447,  2448,
    2449,  2450,  2451,  2452,  2463,  2473,  2474,  2478,  2479,  2480,
    2481,  2482,  2483,  2484,  2485,  2486,  2489,  2504,  2507,  2509,
    2513,  2514,  2515,  2516,  2517,  2518,  2519,  2520,  2521,  2522,
    2522,  2523,  2523,  2524,  2525,  2526,  2530,  2531,  2535,  2543,
    2545,  2549,  2550,  2551,  2555,  2556,  2559,  2561,  2564,  2565,
    2566,  2567,  2568,  2569,  2570,  2571,  2572,  2573,  2574,  2577,
    2597,  2598,  2599,  2600,  2601,  2602,  2603,  2604,  2605,  2606,
    2607,  2608,  2609,  2610,  2611,  2612,  2613,  2614,  2615,  2616,
    2617,  2618,  2619,  2620,  2621,  2622,  2623,  2624,  2625,  2626,
    2627,  2628,  2629,  2630,  2631,  2632,  2633,  2634,  2635,  2638,
    2640,  2643,  2645,  2654,  2656,  2657,  2660,  2662,  2666,  2667,
    2670,  2680,  2701,  2703,  2707,  2707,  2710,  2712,  2715,  2717,
    2721,  2725,  2726,  2731,  2734,  2737,  2742,  2745,  2750,  2753,
    2757,  2761,  2764,  2765,  2766,  2767,  2768,  2769,  2770,  2771,
    2777,  2784,  2785,  2786,  2791,  2792,  2798,  2805,  2806,  2807,
    2808,  2809,  2810,  2811,  2812,  2813,  2814,  2815,  2816,  2817,
    2821,  2822,  2823,  2828,  2829,  2830,  2831,  2832,  2835,  2836,
    2837,  2838,  2839,  2840,  2841,  2844,  2846,  2849,  2851,  2855,
    2867,  2874,  2881,  2882,  2883,  2884,  2885,  2888,  2897,  2905,
    2912,  2913,  2917,  2922,  2928,  2931,  2936,  2937,  2938,  2939,
    2940,  2943,  2949,  2956,  2957,  2958,  2959,  2963,  2964,  2965,
    2966,  2967,  2970,  2971,  2972,  2973,  2974,  2975,  2976,  2979,
    2981,  2984,  2986,  2990,  3002,  3009,  3009,  3009,  3009,  3009,
    3009,  3009,  3009,  3009,  3009,  3009,  3009,  3009,  3009,  3009,
    3009,  3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,
    3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,
    3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,
    3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,
    3010,  3010,  3010,  3010,  3010,  3010,  3010,  3010,  3011,  3011,
    3011,  3011,  3011,  3011,  3011,  3011,  3011,  3011,  3012,  3012,
    3012,  3012,  3013,  3013,  3013,  3014,  3014,  3014,  3014,  3014,
    3015,  3015,  3016,  3017,  3018,  3019,  3020,  3021,  3022,  3023,
    3024,  3025,  3026,  3026,  3026,  3026,  3026,  3026
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "END", "CLI_MARKER", "INVALID_TOKEN",
  "ELSECOL", "DDOT", "GEQ", "LEQ", "NEQ", "AND", "OR", "PO", "PC", "NUM",
  "ENUM", "RTRID", "IPA", "SYM", "TEXT", "PREFIX_DUMMY", "'='", "'<'",
  "'>'", "'~'", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'.'", "DEFINE",
  "ON", "OFF", "YES", "NO", "S", "MS", "US", "PORT", "LOG", "SYSLOG",
  "ALL", "DEBUG", "TRACE", "INFO", "REMOTE", "WARNING", "ERROR", "AUTH",
  "FATAL", "BUG", "STDERR", "SOFT", "TIMEFORMAT", "ISO", "OLD", "SHORT",
  "LONG", "BASE", "NAME", "CONFIRM", "UNDO", "CHECK", "TIMEOUT", "LATENCY",
  "LIMIT", "WATCHDOG", "CONFIGURE", "DOWN", "KERNEL", "PERSIST", "SCAN",
  "TIME", "LEARN", "DEVICE", "ROUTES", "GRACEFUL", "RESTART", "KRT_SOURCE",
  "KRT_METRIC", "TABLE", "KRT_PREFSRC", "KRT_REALM", "ROUTER", "ID",
  "PROTOCOL", "TEMPLATE", "PREFERENCE", "DISABLED", "DIRECT", "INTERFACE",
  "IMPORT", "EXPORT", "FILTER", "NONE", "STATES", "FILTERS", "RECEIVE",
  "ACTION", "WARN", "BLOCK", "DISABLE", "KEEP", "FILTERED", "PASSWORD",
  "FROM", "PASSIVE", "TO", "EVENTS", "PACKETS", "PROTOCOLS", "INTERFACES",
  "PRIMARY", "STATS", "COUNT", "FOR", "COMMANDS", "PREEXPORT", "NOEXPORT",
  "GENERATE", "ROA", "LISTEN", "BGP", "V6ONLY", "DUAL", "ADDRESS",
  "PASSWORDS", "DESCRIPTION", "SORTED", "RELOAD", "IN", "OUT", "MRTDUMP",
  "MESSAGES", "RESTRICT", "MEMORY", "IGP_METRIC", "CLASS", "DSCP", "WAIT",
  "MAX", "FLUSH", "AS", "SHOW", "STATUS", "SUMMARY", "ROUTE", "SYMBOLS",
  "ADD", "DELETE", "DUMP", "RESOURCES", "SOCKETS", "NEIGHBORS",
  "ATTRIBUTES", "EVAL", "ECHO", "ENABLE", "FUNCTION", "PRINT", "PRINTN",
  "UNSET", "RETURN", "ACCEPT", "REJECT", "QUITBIRD", "INT", "BOOL", "IP",
  "PREFIX", "PAIR", "QUAD", "EC", "SET", "STRING", "BGPMASK", "BGPPATH",
  "CLIST", "ECLIST", "IF", "THEN", "ELSE", "CASE", "TRUE", "FALSE", "RT",
  "RO", "UNKNOWN", "GENERIC", "GW", "NET", "MASK", "PROTO", "SOURCE",
  "SCOPE", "CAST", "DEST", "IFNAME", "IFINDEX", "LEN", "DEFINED",
  "CONTAINS", "RESET", "PREPEND", "FIRST", "LAST", "MATCH", "ROA_CHECK",
  "EMPTY", "WHERE", "BFD", "MIN", "IDLE", "RX", "TX", "INTERVAL",
  "MULTIPLIER", "MULTIHOP", "NEIGHBOR", "DEV", "LOCAL", "SESSIONS", "HOLD",
  "CONNECT", "RETRY", "KEEPALIVE", "STARTUP", "VIA", "NEXT", "HOP", "SELF",
  "DEFAULT", "PATH", "METRIC", "START", "DELAY", "FORGET", "AFTER",
  "BGP_PATH", "BGP_LOCAL_PREF", "BGP_MED", "BGP_ORIGIN", "BGP_NEXT_HOP",
  "BGP_ATOMIC_AGGR", "BGP_AGGREGATOR", "BGP_COMMUNITY",
  "BGP_EXT_COMMUNITY", "RR", "RS", "CLIENT", "CLUSTER", "AS4", "ADVERTISE",
  "IPV4", "CAPABILITIES", "PREFER", "OLDER", "MISSING", "LLADDR", "DROP",
  "IGNORE", "REFRESH", "INTERPRET", "COMMUNITIES", "BGP_ORIGINATOR_ID",
  "BGP_CLUSTER_LIST", "IGP", "GATEWAY", "RECURSIVE", "MED", "TTL",
  "SECURITY", "DETERMINISTIC", "SECONDARY", "ALLOW", "PATHS", "AWARE",
  "LINK", "OSPF", "AREA", "OSPF_METRIC1", "OSPF_METRIC2", "OSPF_TAG",
  "OSPF_ROUTER_ID", "RFC1583COMPAT", "STUB", "TICK", "COST", "COST2",
  "RETRANSMIT", "HELLO", "TRANSMIT", "PRIORITY", "DEAD", "TYPE",
  "BROADCAST", "BCAST", "NONBROADCAST", "NBMA", "POINTOPOINT", "PTP",
  "POINTOMULTIPOINT", "PTMP", "SIMPLE", "AUTHENTICATION", "STRICT",
  "CRYPTOGRAPHIC", "ELIGIBLE", "POLL", "NETWORKS", "HIDDEN", "VIRTUAL",
  "ONLY", "BUFFER", "LARGE", "NORMAL", "STUBNET", "TAG", "EXTERNAL",
  "LSADB", "ECMP", "WEIGHT", "NSSA", "TRANSLATOR", "STABILITY", "GLOBAL",
  "LSID", "INSTANCE", "REAL", "NETMASK", "LENGTH", "MERGE", "LSA",
  "SUPPRESSION", "TOPOLOGY", "STATE", "PIPE", "PEER", "MODE", "OPAQUE",
  "TRANSPARENT", "RIP", "INFINITY", "PERIOD", "GARBAGE", "MULTICAST",
  "QUIET", "NOLISTEN", "VERSION1", "PLAINTEXT", "MD5", "HONOR", "NEVER",
  "ALWAYS", "RIP_METRIC", "RIP_TAG", "STATIC", "PROHIBIT", "MULTIPATH",
  "BLACKHOLE", "UNREACHABLE", "SDN", "UNIXSOCKET", "';'", "'('", "')'",
  "':'", "'{'", "'}'", "','", "'?'", "'['", "']'", "$accept", "config",
  "conf_entries", "definition", "expr", "expr_us", "bool", "ipa",
  "ipa_scope", "prefix", "prefix_or_ipa", "pxlen", "datetime", "text",
  "text_or_none", "log_config", "syslog_name", "log_file", "log_mask",
  "log_mask_list", "log_cat", "mrtdump_base", "timeformat_which",
  "timeformat_spec", "timeformat_base", "debug_unix", "cmd_CONFIGURE",
  "cmd_CONFIGURE_SOFT", "cmd_CONFIGURE_CONFIRM", "cmd_CONFIGURE_UNDO",
  "cmd_CONFIGURE_CHECK", "cmd_DOWN", "cfg_name", "cfg_timeout",
  "kern_proto_start", "kern_item", "kif_proto_start", "kif_item",
  "kern_sys_item", "rtrid", "idval", "listen", "listen_opts", "listen_opt",
  "gr_opts", "tab_sorted", "newtab", "roa_table_start", "roa_table_opts",
  "roa_table", "proto_start", "proto_name", "proto_item", "imexport",
  "limit_action", "limit_spec", "rtable", "debug_default",
  "iface_patt_node_init", "iface_patt_node_body", "iface_negate",
  "iface_patt_node", "iface_patt_list", "iface_patt_list_nopx",
  "iface_patt_init", "iface_patt", "tos", "dev_proto_start", "dev_proto",
  "dev_iface_init", "dev_iface_patt", "debug_mask", "debug_list",
  "debug_flag", "mrtdump_mask", "mrtdump_list", "mrtdump_flag",
  "password_list", "password_items", "password_item",
  "password_item_begin", "password_item_params", "cmd_SHOW_STATUS",
  "cmd_SHOW_MEMORY", "cmd_SHOW_PROTOCOLS", "cmd_SHOW_PROTOCOLS_ALL",
  "optsym", "cmd_SHOW_INTERFACES", "cmd_SHOW_INTERFACES_SUMMARY",
  "cmd_SHOW_ROUTE", "r_args", "export_mode", "cmd_SHOW_ROA", "roa_args",
  "roa_mode", "cmd_SHOW_SYMBOLS", "sym_args", "roa_table_arg",
  "cmd_ADD_ROA", "cmd_DELETE_ROA", "cmd_FLUSH_ROA", "cmd_DUMP_RESOURCES",
  "cmd_DUMP_SOCKETS", "cmd_DUMP_EVENTS", "cmd_DUMP_INTERFACES",
  "cmd_DUMP_NEIGHBORS", "cmd_DUMP_ATTRIBUTES", "cmd_DUMP_ROUTES",
  "cmd_DUMP_PROTOCOLS", "cmd_EVAL", "cmd_ECHO", "echo_mask", "echo_size",
  "cmd_DISABLE", "cmd_ENABLE", "cmd_RESTART", "cmd_RELOAD",
  "cmd_RELOAD_IN", "cmd_RELOAD_OUT", "cmd_DEBUG", "cmd_MRTDUMP",
  "cmd_RESTRICT", "proto_patt", "proto_patt2", "filter_def", "$@1",
  "filter_eval", "type", "one_decl", "decls", "declsn", "filter_body",
  "filter", "where_filter", "function_params", "function_body",
  "function_def", "$@2", "cmds", "cmds_int", "block", "fipa", "set_atom",
  "switch_atom", "pair_expr", "pair_atom", "pair_item", "ec_expr",
  "ec_kind", "ec_item", "set_item", "switch_item", "set_items",
  "switch_items", "fprefix_s", "fprefix", "fprefix_set", "switch_body",
  "bgp_path_expr", "bgp_path", "bgp_path_tail1", "bgp_path_tail2",
  "constant", "constructor", "rtadot", "function_call", "symbol",
  "static_attr", "term", "break_command", "print_one", "print_list",
  "var_listn", "var_list", "cmd", "bfd_proto_start", "bfd_proto_item",
  "bfd_proto_opts", "bfd_proto", "bfd_iface_start", "bfd_iface_item",
  "bfd_iface_opts", "bfd_iface_opt_list", "bfd_iface", "bfd_multihop",
  "bfd_neigh_iface", "bfd_neigh_local", "bfd_neigh_multihop",
  "bfd_neighbor", "cmd_SHOW_BFD_SESSIONS", "bgp_proto_start",
  "bgp_nbr_opts", "bgp_proto", "ospf_proto_start", "ospf_proto",
  "ospf_proto_item", "ospf_area_start", "ospf_area", "ospf_area_opts",
  "ospf_area_item", "$@3", "$@4", "ospf_stubnet", "ospf_stubnet_start",
  "ospf_stubnet_opts", "ospf_stubnet_item", "ospf_vlink",
  "ospf_vlink_opts", "ospf_vlink_item", "ospf_vlink_start",
  "ospf_iface_item", "pref_list", "pref_item", "pref_base", "pref_opt",
  "nbma_list", "nbma_eligible", "nbma_item", "ospf_iface_start",
  "ospf_instance_id", "ospf_iface_patt_list", "$@5", "ospf_iface_opts",
  "ospf_iface_opt_list", "ospf_iface", "opttext", "cmd_SHOW_OSPF",
  "cmd_SHOW_OSPF_NEIGHBORS", "cmd_SHOW_OSPF_INTERFACE",
  "cmd_SHOW_OSPF_TOPOLOGY", "cmd_SHOW_OSPF_TOPOLOGY_ALL",
  "cmd_SHOW_OSPF_STATE", "cmd_SHOW_OSPF_STATE_ALL", "cmd_SHOW_OSPF_LSADB",
  "lsadb_args", "pipe_proto_start", "pipe_proto", "rip_cfg_start",
  "rip_cfg", "rip_auth", "rip_mode", "rip_iface_item", "rip_iface_opts",
  "rip_iface_opt_list", "rip_iface_init", "rip_iface",
  "static_proto_start", "static_proto", "stat_route0", "stat_multipath1",
  "stat_multipath", "stat_route", "cmd_SHOW_STATIC", "sdn_cfg_start",
  "sdn_cfg", "sdn_mode", "sdn_iface_item", "sdn_iface_opts",
  "sdn_iface_opt_list", "sdn_iface_init", "sdn_iface", "conf", "cli_cmd",
  "proto", "kern_proto", "kif_proto", "dynamic_attr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,    61,    60,    62,   126,    43,    45,    42,    47,
      37,    33,    46,   277,   278,   279,   280,   281,   282,   283,
     284,   285,   286,   287,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   315,   316,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     424,   425,   426,   427,   428,   429,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   499,   500,   501,   502,   503,
     504,   505,   506,   507,   508,   509,   510,   511,   512,   513,
     514,   515,   516,   517,   518,   519,   520,   521,   522,   523,
     524,   525,   526,   527,   528,   529,   530,   531,   532,   533,
     534,   535,   536,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   578,   579,   580,   581,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,   600,   601,   602,   603,
     604,   605,   606,   607,   608,   609,    59,    40,    41,    58,
     123,   125,    44,    63,    91,    93
};
# endif

#define YYPACT_NINF -1298

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1298)))

#define YYTABLE_NINF -445

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-445)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      69,  2566,   196,   617,   604,   725,   202,   604,   604,   533,
     604,   222,   128,  1178,   141,   157,  1482,  1502,    68,   604,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
     304,   977,   323,   638,   165,   250,   322,   275, -1298, -1298,
     377,   369,   281,   227,  1502,   404, -1298, -1298, -1298, -1298,
   -1298, -1298,   286,   286, -1298, -1298, -1298, -1298,   104, -1298,
    2081, -1298,   286,  2379, -1298, -1298, -1298,   286, -1298,   286,
    2226,   286,  2248,   286,  2263,   286,  2096,   286,  1127,   286,
    2323, -1298, -1298,  2048,  1411, -1298, -1298, -1298,   124, -1298,
     449,   512,   516,   449,   435, -1298,   552,   560,   604,   604,
     567,   134, -1298,   491,   750,   181, -1298,   576,   580, -1298,
   -1298,   371,   380,   660,   395,   395,   600,   669,   694,   698,
     701,   709,   713,   724,    36, -1298, -1298, -1298,   704,   372,
   -1298,   525,   106,     3,  1502, -1298,   405,   410,   412, -1298,
   -1298,   414,   416,   418,   857,    41, -1298, -1298, -1298, -1298,
   -1298,  1971, -1298,  2720, -1298, -1298,  1684,   778,   792,   775,
   -1298, -1298,   738, -1298, -1298,    19,   498,   124,   183, -1298,
   -1298, -1298, -1298,   528,   458,   183,   183,   685,   699,   781,
   -1298,   813, -1298,   134,   483,  2745, -1298,   751,   843,   510,
     540, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298,   542,   124,   863,   832,   183,   634, -1298,  1074,
    1026,   854,   711,   134, -1298,   562,   564,   565,   581,    65,
     675,   887,   608,   961,   744,   711,   634,   925,   722,   -33,
     870,   634,    87,   395,   457,   928,    22,   946,   800,   795,
     501,   796,   797,   569,   807,   806,   634,   812,   808,   826,
      30,    56,   862,   805,   828,   634,   894, -1298,   753,   756,
     851,   634,  1034,   183,   634,  1047,   814, -1298, -1298,   755,
     763, -1298,   766,  1061,   408, -1298,   779,   777,   183,  1079,
   -1298,   711,   785,    -4,   183,   183,  1082,    32, -1298,   799,
     802, -1298,   788,   791,   886,   395,  1086, -1298,   809,   720,
     815,   803, -1298,  1158, -1298,   818,  1097,   634,  1107,   634,
    1108,  1105, -1298,   822,   825,   830,  1120,  1180, -1298,   835,
     838, -1298, -1298,   609,  1202,   435, -1298, -1298,  1203,   183,
    1204, -1298, -1298,  1206,  1212, -1298, -1298, -1298,   314,  1213,
    1200,  1225, -1298, -1298,   816,  1229, -1298,  1230,   745, -1298,
   -1298,  2510,  1171,   660, -1298,   660,   660, -1298,   136,   271,
    1236,  1238, -1298, -1298,    -5,  1099,  1101, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298,    36, -1298,    36,  1502,    36,
      36,  1233, -1298,  1234,   898,  1222,  1039,  1228,     3,     3,
    1223,  1224,  1502,  1502,  1502,  1502,  1502,  1239, -1298, -1298,
    1246, -1298,   896,    72, -1298, -1298, -1298, -1298,   789, -1298,
    1262, -1298, -1298, -1298,    91,    52, -1298,   166, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298,  1502,  1502,  1502,  1502,  1502,  1502,
    1502,  1502,  1502,  1502,  1502,  1502,  1502,  1018, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298,   471, -1298, -1298,
    1268, -1298,  1502,   711, -1298, -1298,  1684,   907, -1298, -1298,
   -1298, -1298, -1298, -1298,   183,  1502, -1298, -1298, -1298, -1298,
     183,   833,   889, -1298,   645, -1298, -1298,   183, -1298, -1298,
   -1298, -1298, -1298, -1298,   909,  1681, -1298,   102,   912, -1298,
     916,  1260, -1298, -1298, -1298,   -95, -1298, -1298, -1298, -1298,
     851, -1298, -1298, -1298, -1298,   139,  1927, -1298,  1181,  1502,
   -1298, -1298,   139, -1298,   139, -1298, -1298, -1298, -1298, -1298,
   -1298,  1218,  1220,   634,   100, -1298,   922,  1250,   930,   938,
     183,   208,  1041,   634,   395,   940, -1298,   945,  1283,    11,
     183,  1169,   183,  1242,  1248,   183,  1249,   -52,   183,   183,
     634,  1251,   955,  1243,   963,   634,   968,   634,   365,   634,
     863,   634,   969,   975,   634,   634,   634,   976,  1205, -1298,
   -1298, -1298, -1298,   634, -1298,  1279,   183,   634, -1298, -1298,
   -1298,  1329,   983,   987, -1298, -1298,   989,   183, -1298,   990,
   -1298,  1253, -1298, -1298, -1298,   991,   992,   996,   183,   997,
     998,  1005, -1298, -1298,   260, -1298,   634, -1298,   863, -1298,
   -1298,   793, -1298,   395, -1298,  1145, -1298, -1298, -1298, -1298,
   -1298,  1010,  1011, -1298,   183, -1298,   183, -1298,   634,   634,
   -1298, -1298, -1298,   183, -1298,   395, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298,   641, -1298, -1298,  1376, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298,   658, -1298, -1298, -1298, -1298,
    1378, -1298, -1298, -1298,  1364, -1298, -1298,  1369,   395, -1298,
   -1298,  1367,  1368, -1298,  1927, -1298, -1298, -1298, -1298,   395,
   -1298, -1298, -1298,  1370, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298,  1385,  1372,  1372,  1211,   660,  1372,   660,
    1372, -1298, -1298,   183,   395, -1298,  1383,  1384, -1298, -1298,
     666, -1298, -1298, -1298, -1298,    99, -1298,  1032, -1298,  1375,
   -1298, -1298, -1298, -1298,   259,   284,   310,   934,   343,   125,
   -1298,  1502, -1298,  1502, -1298,  1396,  1033,  1035,  1029,    53,
     123, -1298, -1298, -1298,  1391,  1390, -1298,  2658,  2658,  2658,
    2756,  2756,  2658,  2658,  2658,  2658,   958,   958,  1224,  1224,
   -1298,  1043, -1298, -1298, -1298, -1298,  1684, -1298,  1931, -1298,
     706, -1298, -1298,  1438,  1392, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298,  1048, -1298,  1049, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,   113,  1050,
    1051, -1298, -1298,   183, -1298, -1298,   395, -1298, -1298, -1298,
     584,  1681, -1298,   395, -1298, -1298,  1386, -1298,  1056, -1298,
    1321, -1298, -1298, -1298, -1298,   634,  2745, -1298, -1298,  2150,
     183,   183,  1057,   183,  1070,  1072, -1298,   634, -1298, -1298,
    1073,  1085,  1088,  1089,   634,  1091,  1092, -1298, -1298,  1433,
   -1298,   183,   183, -1298,  1103,   183,  1109,   183,   183,  1110,
     183,  1111,  1114,  1121,  1122,  1123,   183, -1298,   851, -1298,
    1124, -1298,  1126,  1129,  1130,  1137,  1143,  1146,  1147, -1298,
   -1298,  1148,  1150,  1157, -1298,   178, -1298,   183, -1298, -1298,
    1019,  1159, -1298, -1298, -1298,  1164,   246, -1298,  1102,  1166,
   -1298, -1298, -1298,  1168, -1298, -1298, -1298,   183,   273,   337,
    1153,  1170,  1172, -1298,  1283, -1298,   395,  1154,  1145,   263,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298,    -5, -1298, -1298,
   -1298,   609, -1298, -1298,   314, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298,  1539,  1541, -1298, -1298,
     851, -1298, -1298,   851,  1528, -1298,   851,  1372,  1542,  1372,
    1548, -1298, -1298,  1407,  1408, -1298,  1502, -1298,  1529,  1502,
    1502,  1502, -1298,  1502, -1298,  1502,   499,  1559,  1502,  1112,
    1502, -1298, -1298,  1502, -1298, -1298,  1183,   704, -1298,  1502,
   -1298, -1298, -1298, -1298, -1298, -1298,  1056, -1298, -1298, -1298,
    1681,   479, -1298, -1298, -1298,  1191,  1194, -1298,  1420, -1298,
    1046, -1298,   993, -1298, -1298, -1298, -1298,   395, -1298, -1298,
    1198,  1231,  1240, -1298,  1244, -1298, -1298,  1252, -1298, -1298,
   -1298, -1298,  1255, -1298, -1298, -1298,    34, -1298, -1298, -1298,
    1256, -1298,  1257,  1267, -1298,  1274, -1298, -1298, -1298, -1298,
   -1298,  1280,  1281, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298,  1282, -1298, -1298,   634,   114,
     524, -1298,  1310,   395, -1298, -1298,   810, -1298,  1284, -1298,
    1272, -1298, -1298, -1298, -1298, -1298,  1253, -1298,  1285,  1577,
    1577,  1577,  1577, -1298, -1298, -1298, -1298,  1283,   183,  1154,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
    1601, -1298,  1602, -1298,  1593,  1594, -1298, -1298,  1603,  1633,
    1658,  1691,   582,  1502, -1298, -1298,  2745,  1266,  1247,  2745,
    1029,  1634,  1716, -1298,   -11, -1298,  1630, -1298, -1298,  1296,
    1502, -1298, -1298, -1298,  1502,  1502,  1245,    24,  1971,  1298,
    1133, -1298,  1681, -1298,  1656,   395, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298,  1302, -1298,    75, -1298, -1298,
     183, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298,   183,   183,   634,   183,
   -1298,  1303,   851, -1298, -1298,  1304,  1305,   183, -1298, -1298,
     183,  1306,   340, -1298,   260, -1298,  1311,  1325,  1326,  1327,
   -1298, -1298,   493, -1298, -1298,   491,   491, -1298, -1298, -1298,
   -1298,  1502,  1741,  1687,  1414,  1324, -1298,  1502,   898,  1502,
   -1298,  1956,  2533,   829, -1298, -1298,  1674,   151, -1298,  2745,
    1332,  1331, -1298,  1553, -1298,  1056,  1302, -1298, -1298,  1688,
     711,  1483,  1343,  1056,  1340, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298,   217,   183,  1437,   583,
   -1298,  1355, -1298,   260,   260,   260,   260,   270,   183,  1448,
     667, -1298,  1380,  1744,  1751,  1767, -1298,  1388,  1394,     5,
   -1298,  1981,  1403,  2006,   653, -1298,   741, -1298,  1502,  1502,
     503,  1133, -1298,  1745, -1298,   353, -1298, -1298,   395,  1536,
   -1298,  1272, -1298, -1298,   -10,    13,    14,  1465,   183,   183,
     183, -1298, -1298,   602, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298,   183, -1298, -1298,   970, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,  1502,
   -1298,  1502, -1298, -1298, -1298, -1298,  1406,   479,  1596, -1298,
      31,  2038,  2063,  1416,  1417,  1418,  1419,  1421, -1298,  1431,
     634,   890,  1581,   183,   183, -1298,  1439, -1298,   634, -1298,
   -1298,  2368, -1298, -1298, -1298,  -103,   634,   183,   634, -1298,
    1441, -1298,   183,   183,   183,  1570,   184,   -32, -1298, -1298,
    1443, -1298, -1298, -1298,  1495, -1298, -1298,  1496, -1298,  1440,
    1445,  1456,  1452,   741,    70, -1298, -1298, -1298,   789, -1298,
   -1298,  1823, -1298, -1298, -1298,   243, -1298, -1298,  1502,  1502,
    1502,  1502, -1298, -1298, -1298,  1613,  1614,  1615, -1298, -1298,
   -1298, -1298,  1554,   183,  1467,   634,  1519,   204,  1564,   634,
    1512,   634,   183,   183,   183,  1610,   183,   197,  1275,  1525,
     -31,  1568,   183,  1546,  1573, -1298, -1298,  1506, -1298,   183,
    1507, -1298, -1298, -1298, -1298, -1298, -1298, -1298,   183,   183,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298,  1502, -1298, -1298,
   -1298, -1298, -1298,  1792,   129,    70,   135,  1817,  1855,  1880,
    1906,   183,   183,   183,   634, -1298, -1298, -1298,     7,   183,
     183, -1298,  1053, -1298,  1538, -1298, -1298, -1298, -1298,   183,
   -1298,   183, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298,   634, -1298, -1298, -1298,   634, -1298,   183,   634, -1298,
   -1298, -1298, -1298, -1298,  1517, -1298,  1502, -1298, -1298, -1298,
    1509,  1520,  1527,  1530, -1298, -1298, -1298, -1298,    26, -1298,
   -1298, -1298, -1298, -1298,  1579, -1298,   634, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298,  1792, -1298, -1298, -1298, -1298, -1298,
    1585, -1298, -1298, -1298, -1298,  1531, -1298
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     0,     0,    75,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   382,     0,     0,
     791,   792,   793,   794,   795,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
     821,   822,   823,   824,   825,   826,   827,   828,   829,   830,
     831,   832,   833,   834,   835,   836,   837,     3,     1,     2,
       0,     0,     0,     0,     0,     0,     0,     0,   111,   112,
       0,     0,     0,     0,   382,     0,   775,   776,   777,   778,
     779,   780,   113,   113,   781,   782,   783,   784,   109,   785,
       0,   787,   113,   117,   788,   789,   790,   113,   841,   113,
     117,   113,   117,   113,   117,   113,   117,   113,   117,   113,
     117,     5,   786,   117,   117,   267,   269,   268,     0,    76,
      75,     0,     0,    75,    77,    74,     0,     0,     0,     0,
       0,     0,   266,   238,   271,     0,   222,     0,     0,   205,
     230,     0,   201,   201,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   365,   369,   378,   375,   309,   384,
     372,     0,     0,   368,   382,   414,     0,     0,     0,   370,
     371,     0,     0,     0,   382,     0,   373,   374,   379,   412,
     413,     0,   411,     0,   254,   253,     0,   256,     0,     0,
      30,    29,    35,    38,    36,     0,    18,     0,     0,    57,
      56,    55,    54,     0,     0,     0,     0,     0,   103,     0,
     273,     0,    96,     0,     0,   275,   301,   114,     0,     0,
       0,   107,    80,    86,   159,   492,   462,   554,   690,   696,
     731,   752,     0,     0,     0,     0,     0,    18,   163,     0,
       0,     0,     0,     0,   840,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    18,     0,     0,     0,
       0,    18,     0,   493,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    18,     0,     0,     0,
       0,     0,     0,     0,     0,    18,     0,   842,     0,     0,
       0,    18,     0,     0,    18,     0,     0,   843,   557,     0,
       0,   565,     0,     0,     0,   844,     0,     0,     0,     0,
     729,     0,     0,     0,     0,     0,     0,     0,   845,     0,
       0,   184,   188,     0,     0,     0,     0,   846,     0,     0,
       0,     0,   773,     0,   847,     0,     0,    18,     0,    18,
       0,     0,   838,     0,     0,     0,     0,    32,   839,     0,
       0,   166,   165,     0,     0,    77,    71,    72,     0,    78,
       0,   260,   258,     0,     0,   261,   177,   176,     0,     0,
       0,     0,   270,   272,   271,     0,   202,     0,   226,   197,
     196,     0,     0,   201,   200,   201,   201,   681,   201,   201,
       0,     0,    19,    20,     0,     0,     0,   249,   245,   250,
     246,   243,   244,   247,   248,   365,   384,   365,   382,   365,
     365,     0,   357,     0,   382,     0,     0,     0,   368,   368,
       0,   409,   382,   382,   382,   382,   382,     0,   328,   329,
       0,   331,     0,     0,   310,   313,   311,   315,   382,   312,
     337,   335,   336,   343,     0,   348,   352,     0,   860,   855,
     856,   857,   858,   385,   859,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   862,   865,   864,   861,   863,   866,
     867,   868,   871,   869,   870,   872,   873,   874,   875,   876,
     877,   415,   416,   251,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,     0,    43,    44,
      45,    46,    47,    48,    49,    50,    51,     0,    41,   257,
       0,   259,   382,     0,    37,    39,     0,     0,     7,     9,
      14,    16,    15,    17,     0,   382,    13,    65,   143,   144,
      58,     0,     0,    64,     0,    67,    68,     0,   104,   105,
      92,    93,    94,   155,     0,   290,   106,     0,     0,    53,
       0,     0,   115,   848,   852,     0,   160,   120,   142,   128,
       0,   118,   119,   145,   133,     0,   290,   134,     0,   382,
     122,   132,     0,   123,     0,   130,   121,   161,   162,   467,
     496,     0,     0,    18,    18,   508,     0,     0,     0,     0,
       0,    18,     0,    18,     0,     0,   509,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,     0,     0,     0,     0,    18,     0,    18,     0,    18,
       0,    18,     0,     0,    18,    18,    18,     0,     0,   497,
     555,   566,   558,    18,   563,   560,     0,    18,   556,   568,
     691,     0,     0,     0,   692,   697,     0,     0,   145,     0,
     189,   185,   712,   710,   711,     0,     0,     0,     0,     0,
       0,     0,   698,   705,   190,   732,    18,   737,     0,   733,
     747,     0,   746,     0,   750,     0,   748,   749,   736,   753,
     145,     0,     0,   754,     0,    81,     0,    83,    18,    18,
     850,   851,   849,     0,    31,     0,   854,   853,   171,   170,
     172,   174,   175,   173,     0,   168,   264,     0,    73,    79,
      69,   262,   263,   181,   182,     0,   179,   265,   239,   242,
       0,   198,   203,   221,     0,   228,   227,     0,     0,   204,
     211,     0,     0,   219,   290,   213,   212,   216,   217,     0,
     218,   220,   206,     0,   210,   229,   237,   231,   234,   235,
     233,   236,   232,     0,   672,   672,     0,   201,   672,   201,
     672,   673,   751,     0,     0,    23,     0,     0,   361,   362,
       0,   363,   364,   359,   347,   442,   445,     0,   422,     0,
     423,   366,   367,   360,     0,     0,     0,     0,     0,     0,
     330,   382,   395,   382,   324,   322,     0,     0,   321,     0,
       0,   376,   349,   350,     0,     0,   377,   407,   405,   403,
     400,   401,   402,   404,   406,   408,   396,   397,   398,   399,
     417,     0,   418,   420,   421,   255,     0,   252,     0,    34,
       0,    33,    66,     0,     0,    60,    61,    62,    63,    10,
      11,    12,     0,   145,     0,    90,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,     0,     0,
       0,   274,   294,     0,   100,   101,     0,    95,    97,    52,
       0,   290,   116,     0,   110,   129,   149,   152,   164,   141,
     135,   125,   295,   296,   131,    18,   297,   126,   124,   117,
       0,     0,     0,     0,     0,     0,   502,    18,   537,   539,
       0,     0,     0,     0,    18,     0,     0,   553,   510,     0,
     493,     0,     0,   500,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   504,     0,   505,
       0,   535,     0,     0,     0,     0,     0,     0,     0,   516,
     517,     0,     0,     0,   541,     0,   559,     0,   564,   562,
       0,     0,   694,   695,   700,     0,   727,   709,     0,     0,
     704,   699,   701,     0,   707,   708,   706,     0,     0,     0,
       0,     0,     0,   743,    21,   745,     0,   740,   744,   771,
     755,   756,    89,    82,    84,    85,    87,    25,    24,    88,
     167,     0,    70,   178,     0,   199,   225,   224,   223,   208,
     215,   209,   207,   214,   491,   671,     0,     0,   680,   689,
       0,   687,   684,     0,     0,   682,     0,   672,     0,   672,
       0,    26,    27,     0,     0,   358,   382,   431,     0,   382,
     382,   382,   410,   382,   429,   382,     0,     0,   382,   382,
     382,   314,   309,   382,   338,   344,     0,     0,   353,   382,
      42,     6,    40,     8,    59,   102,   156,    91,   289,   288,
     290,   382,    99,    98,   299,   292,     0,   302,     0,   150,
       0,   145,     0,   140,   127,   470,   470,     0,   469,   463,
       0,     0,     0,   552,     0,   548,   547,     0,   538,   542,
     543,   544,     0,   534,   525,    22,     0,   494,   495,   498,
       0,   506,     0,     0,   529,     0,   512,   511,   524,   523,
     518,     0,     0,   536,   521,   513,   514,   515,   540,   550,
     520,   519,   551,   522,   545,     0,   561,   661,    18,     0,
      18,   579,     0,     0,   581,   571,    18,   567,     0,   585,
     662,   693,   703,   725,   730,   183,   185,   702,     0,     0,
       0,     0,     0,   187,   734,   735,   742,    21,     0,   741,
     769,   774,   169,   180,   675,   674,   688,   683,   685,   686,
       0,   676,     0,   678,     0,     0,   443,   424,     0,     0,
       0,     0,     0,   382,   380,   323,   321,     0,     0,   327,
       0,     0,     0,   291,     0,   435,     0,   436,   437,     0,
     382,   433,   434,   432,   382,   382,     0,   382,     0,     0,
     382,   305,     0,   298,     0,   146,   147,   151,   153,   138,
     136,   137,   139,   145,   464,   479,   465,   483,   466,   468,
       0,   530,   549,   532,   533,   501,   499,   528,   527,   507,
     526,   503,   546,   145,   584,   572,     0,     0,    18,     0,
     570,     0,     0,   588,   583,   587,     0,     0,   577,   569,
       0,   595,   718,   186,   190,    28,     0,     0,     0,     0,
     738,   739,   762,   677,   679,   238,   238,   428,   426,   427,
     425,   382,     0,   325,   382,     0,   419,   382,   382,   382,
     382,     0,     0,     0,   300,   306,     0,     0,   455,   438,
     440,     0,   293,     0,   148,   154,   479,   477,   482,     0,
       0,   486,     0,   664,   668,   574,   575,   573,   576,   649,
     609,   589,   649,   578,   663,   596,     0,     0,     0,     0,
     728,     0,   195,   190,   190,   190,   190,     0,     0,     0,
       0,   772,     0,     0,     0,     0,   381,     0,     0,     0,
     351,     0,     0,     0,     0,   449,   382,   354,   382,   382,
       0,   382,   454,     0,   481,     0,   484,   485,     0,   488,
     531,   662,   666,   670,     0,     0,     0,   598,     0,     0,
       0,   721,   719,    18,   713,   714,   715,   716,   717,   720,
     726,   191,   192,   193,   194,     0,   765,   763,    18,   757,
     758,   759,   760,   761,   764,   770,   240,   241,   430,   382,
     334,   382,   332,   448,   383,   452,     0,   382,   446,   307,
       0,     0,     0,     0,     0,     0,     0,     0,   441,     0,
      18,     0,     0,     0,     0,   480,     0,   487,    18,   490,
     665,     0,   580,   652,   650,   653,    18,     0,    18,   586,
       0,   582,     0,     0,     0,     0,     0,     0,   594,   608,
       0,   157,   158,   722,     0,   723,   766,     0,   767,     0,
       0,     0,     0,   382,   382,   316,   320,   318,   382,   456,
     319,   341,   339,   340,   345,     0,   451,   450,   382,   382,
     382,   382,   457,   108,   476,     0,     0,     0,   471,   475,
     478,   489,     0,     0,     0,    18,     0,     0,     0,    18,
       0,    18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   669,   648,     0,   654,     0,
       0,   592,   593,   591,   590,   602,   600,   599,     0,     0,
     603,   605,   606,   607,   597,   724,   768,   382,   333,   453,
     308,   447,   356,   321,     0,   382,     0,     0,     0,     0,
       0,     0,     0,     0,    18,   614,   656,   646,     0,     0,
       0,   641,    18,   647,     0,   630,   610,   613,   611,     0,
     628,     0,   615,   617,   618,   619,   620,   621,   622,   623,
     624,    18,   635,   636,   637,    18,   612,     0,    18,   667,
     655,   651,   601,   604,     0,   317,   382,   342,   355,   346,
       0,     0,     0,     0,   472,   473,   474,   631,     0,   639,
     638,   640,   642,   643,     0,   644,    18,   627,   616,   626,
     629,   632,   625,   326,     0,   461,   459,   460,   458,   634,
     658,   657,   645,   633,   659,     0,   660
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1298, -1298, -1298, -1298,  -154,  -206,   -47,  -254,  -894,  -141,
    -734, -1298,   -86,   -80, -1298, -1298, -1298, -1298, -1298,  1373,
    1062, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298,   527,  1535, -1298, -1298, -1298, -1298, -1298, -1298,
    -298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298,  2319,   -89,  1651, -1298,    81,  -501, -1298, -1298, -1298,
   -1298,   839,  -652, -1298, -1298, -1298, -1297, -1298, -1298, -1298,
   -1298,   -77, -1298,   910,   -58, -1298,   906, -1211,   757,  -656,
   -1298,  -911, -1298, -1298, -1298, -1298,  -137, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298,  -176, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298,  1540,  1537, -1298, -1298, -1298, -1298,  -863,   849,
     700,  1371,  1179,  1533, -1298,  1044, -1298, -1298, -1205, -1298,
     453,  -184,  1128,   381, -1041,   895, -1268, -1203,  1763, -1267,
    1138,   383, -1298, -1298,  -181,  1135, -1298, -1298, -1298, -1298,
     553,   686, -1298, -1298, -1035, -1298,    40,   733,   -17, -1298,
   -1298,   590,   926,   654, -1140, -1298, -1298, -1298, -1298,   883,
   -1298, -1298,   656, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298,  1054, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298,   643, -1298, -1298, -1298, -1298, -1298, -1298, -1298,   589,
   -1298, -1298, -1298, -1298, -1298,  -557, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,   985,
   -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298, -1298,
   -1298, -1298, -1298, -1298, -1298, -1298, -1101
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,    87,   536,   545,   537,   404,   920,   998,
     999,   775,  1276,   204,   705,    88,   524,   205,   527,   517,
     518,    89,   213,   214,    90,    91,    20,    21,    22,    23,
      24,    25,   134,   370,    92,   353,    93,   359,   354,    94,
     554,    95,   557,   878,    96,   549,    97,    98,   565,    99,
     100,   229,   255,   580,  1083,   891,   569,   101,   886,  1227,
    1080,   887,   888,  1316,   853,   854,  1391,   102,   103,   573,
     256,   364,   714,   715,   379,   725,   726,   330,   968,   331,
     332,   980,    26,    27,    28,    29,   400,    30,    31,    32,
     391,   753,    33,   388,   738,    34,   392,   381,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,   197,   520,    48,    49,    50,    51,    52,    53,    54,
      55,    56,   128,   385,   104,   555,   105,   868,   869,   870,
    1076,   893,   894,   581,   881,   872,   106,   560,  1216,  1217,
    1428,   186,   450,  1491,   805,   806,   451,  1198,   807,   452,
     453,  1494,   454,  1495,   187,   456,   457,  1430,   420,   188,
     421,   430,   189,   190,   191,  1219,   192,   491,  1196,  1220,
    1310,  1311,   786,   787,  1221,   107,  1090,   899,   108,  1233,
    1446,  1375,  1318,  1234,  1236,  1321,  1379,  1449,  1238,    57,
     109,   609,   110,   111,   112,   309,   310,   311,   960,  1148,
    1261,  1266,  1264,  1265,  1385,  1460,  1149,  1387,  1470,  1150,
    1537,  1384,  1454,  1455,  1540,  1628,  1655,  1651,  1253,  1271,
    1324,  1381,  1451,  1383,  1254,  1016,    58,    59,    60,    61,
      62,    63,    64,    65,   766,   113,   114,   115,   116,   665,
    1399,  1341,  1272,  1154,   658,   659,   117,   118,   339,   987,
     988,   340,    66,   119,   120,  1414,  1352,  1282,  1171,   690,
     691,   121,    67,   122,   123,   124,   492
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     193,   449,   641,   224,   455,   969,   966,  1195,   402,   403,
     546,  1297,  1421,   405,   406,  1012,   401,  1075,   428,   608,
     611,   298,   528,   308,   773,   316,   529,   329,   883,   338,
    -304,   345,   402,   403,   355,   360,  1218,  1484,   989,  -304,
    -304,  -304,  -304,  1204,   402,   403,  1485,  1486,  1487,  1052,
    1406,   415,   921,   931,   539,   416,   444,   445,   446,   168,
     447,   544,   544,   525,   417,  1551,  1602,   225,   444,   445,
     446,  1052,   447,     1,  1205,   921,  -303,  1305,   812,   813,
     494,   495,   496,   497,   498,  -303,  -303,  -303,  -303,  1204,
    1166,  1359,   571,   662,   499,   500,   501,   502,   503,   504,
     505,   506,   528,   194,   507,  1319,   529,   494,   495,   496,
     497,   498,   195,   630,  1206,   528,   602,  1307,   607,   529,
    1205,   499,   500,   501,   502,   503,   504,   505,   506,   947,
     538,   507,  1068,   426,   530,   531,   532,   533,   444,   445,
     446,  1052,   447,   873,  1485,  1486,  1487,  1052,   632,   644,
    1485,  1486,  1487,  1052,   528,   394,   922,   431,   529,   361,
    1206,  1456,  1492,  1493,   656,   558,   567,   443,   362,   376,
     666,   667,   585,  1369,   889,   903,  1469,   982,   377,   922,
     767,   932,  1218,  1370,   386,   598,  1207,  1208,  1209,  1210,
    1211,  1212,  1213,   528,   677,   586,    68,   529,   528,   528,
     572,  1066,   529,   529,   422,   135,  1214,   591,  1017,  1215,
    1538,  1028,   528,  1030,   215,   719,   529,  1539,  1480,   599,
    1581,   603,  1482,   528,   605,   142,  1429,   529,   874,   875,
     876,   216,  1207,  1208,  1209,  1210,  1211,  1212,  1213,   626,
    1536,   660,   530,   531,   532,   533,   200,   201,   637,   613,
     752,   143,  1214,   669,   642,  1215,   763,   645,   764,   765,
     614,   768,   770,  1426,   154,  1364,   631,   494,   495,   496,
     497,   498,   885,  1280,  1552,  1603,   884,  1553,  1604,  1562,
     155,   499,   500,   501,   502,   503,   504,   505,   506,  1069,
     394,   507,   494,   495,   496,   497,   498,  1320,  1492,  1493,
     695,  1549,   697,  1457,   592,   227,   499,   500,   501,   502,
     503,   504,   505,   506,  1591,   769,   507,   427,   494,   495,
     496,   497,   498,   199,  1629,  1630,  1458,   633,   842,   387,
     217,  1218,   499,   500,   501,   502,   503,   504,   505,   506,
     223,   218,   507,  1429,  1388,  1389,  1226,   977,   663,   664,
     916,   494,   495,   496,   497,   498,  1298,  1388,  1389,  1075,
    1618,  1452,   219,  1342,   774,   499,   500,   501,   502,   503,
     504,   505,   506,  1422,   535,   507,   429,   923,  1479,   904,
     544,  1159,   978,  1160,  1459,  1461,   844,   670,   671,   526,
     206,  -304,  1218,   852,   228,  -304,   220,  1649,  1488,   394,
    1245,   780,  1489,   418,  1256,  1257,   222,   785,   448,   419,
    1388,  1389,   723,   402,   403,   794,   795,   796,   797,   798,
    1053,   890,   814,   226,   911,   912,   979,   984,   890,   985,
     890,   808,  1401,  1402,  1403,  1404,   207,  -303,   196,  1258,
     802,  -303,   208,   839,   803,  1161,   910,  1162,  1218,  1218,
     724,   997,   221,   606,   535,   422,   924,   422,   926,   422,
     422,   929,  1440,   810,   933,   934,   811,   535,   877,   129,
    1180,  1036,  1182,   395,   231,   402,   403,   817,   818,   819,
     820,   821,   822,   823,   824,   825,   826,   827,   828,   829,
     448,  1314,   958,  1044,   363,   997,  1616,  1045,  1204,  1579,
     969,   369,  1488,   965,   378,   838,   535,   494,   495,   496,
     497,   498,  1390,   528,   973,   366,  1614,   529,   843,   367,
    1032,   499,   500,   501,   502,   503,   504,   505,   506,  1205,
    1218,   507,   530,   531,   532,   533,   396,  1580,   815,   528,
     992,   816,   993,   529,  1134,   535,   902,   905,   540,   996,
     535,   535,   125,   126,   913,   371,   915,  1336,   530,   531,
     532,   533,   896,   372,   535,  1405,   534,  1441,  1442,  1206,
     375,  1443,  1444,   935,   380,   535,  1337,   127,   940,   389,
     942,  1315,   946,   390,   948,   541,   542,   951,   952,   953,
     494,   495,   496,   497,   498,   393,   956,  1008,   943,  1433,
     959,  1323,   610,   407,   499,   500,   501,   502,   503,   504,
     505,   506,  1565,  1338,   507,  1566,  1153,   528,  1081,  1031,
      69,   529,  1073,   125,   126,   449,   449,   944,   945,   981,
    1027,  1039,  1029,  1170,   455,  1081,   530,   531,   532,   533,
    1122,  1207,  1208,  1209,  1210,  1211,  1212,  1213,   127,   528,
      70,   994,   995,   529,  1434,  1435,  1040,   365,   458,    71,
     368,  1214,    72,   897,  1215,   898,   138,   139,   530,   531,
     532,   533,   408,    73,   494,   495,   496,   497,   498,   394,
     209,  1339,  1041,   849,   850,   851,    74,   708,   499,   500,
     501,   502,   503,   504,   505,   506,    75,   409,   507,   210,
      76,   410,   397,    77,   411,    78,    79,   709,   710,  1436,
    1347,  1340,   412,    80,  1437,  1043,   413,   398,   399,  1072,
     711,   712,  1176,   713,  1445,  1177,   211,   414,  1179,  1348,
     200,   201,  1167,   423,   459,   460,   425,   461,   462,   424,
      81,    82,  1078,   618,   619,   129,  1091,  1092,   733,  1094,
     652,   653,    83,   856,   857,   858,   859,   860,   861,   862,
    1204,   863,   864,   865,   866,   867,  1349,  1107,  1108,   382,
     383,  1110,   432,  1112,  1113,    84,  1115,   433,    85,   434,
     130,   435,  1121,   436,  1046,   437,  1047,   212,   131,   132,
     133,  1205,   464,   519,   384,   521,   550,   522,   551,   552,
     523,  1135,   164,  1136,   165,   166,   167,   168,   169,   170,
    1089,   402,   403,   983,  1259,   171,   172,   804,   173,  1474,
     174,   622,   623,  1158,   543,   528,   997,   547,   734,   529,
     548,  1206,   556,  1237,  1350,   382,   383,   494,   495,   496,
     497,   498,   835,   836,   530,   531,   532,   533,  1084,   559,
    -303,   499,   500,   501,   502,   503,   504,   505,   506,   561,
    1097,   507,   562,   735,  1351,   535,   550,  1102,   551,   552,
     164,  1193,   165,   166,   167,   168,   169,   170,   736,   175,
     563,  1394,   568,   171,   172,   176,   173,   680,   174,   553,
     737,   535,   845,   846,   474,   475,   476,   477,   478,   479,
     480,   481,   482,  1207,  1208,  1209,  1210,  1211,  1212,  1213,
     564,   164,   566,   165,   166,   167,   168,   169,   170,   570,
     483,   484,   584,  1214,   171,   172,  1215,   173,   587,   174,
     588,  1395,  1396,  1397,  1398,   589,   485,   486,   487,   488,
     177,   178,   494,   495,   496,   497,   498,   175,   847,   848,
     681,   590,  1074,   176,  1291,   593,   499,   500,   501,   502,
     503,   504,   505,   506,  1330,  1409,   507,   594,   778,   535,
     779,   997,   781,   782,   595,   179,   180,   438,   439,   440,
     441,   596,   682,    86,   597,   528,   505,   506,   175,   529,
     507,   683,   181,   600,   176,   182,   200,   201,   604,   183,
     601,   535,  1263,   612,   530,   531,   532,   533,   177,   178,
     489,   490,  1000,  1001,  1281,  1410,  1411,  1412,  1413,   785,
     202,   615,  1188,  1189,  1190,   616,  1191,   617,  1192,  1003,
    1004,   203,   620,  1199,  1035,   621,  1200,   494,   495,   496,
     497,   498,  1202,   179,   180,   438,   439,   440,   441,   177,
     178,   499,   500,   501,   502,   503,   504,   505,   506,   624,
     181,   507,   625,   182,   402,   403,  1225,   183,   528,   628,
     574,   627,   529,  1229,  1277,  1278,  1279,  1062,   836,   635,
     684,   685,   686,   687,   179,   180,  1322,   530,   531,   532,
     533,  1255,   629,  1260,   582,  1230,  1231,  1232,   634,  1268,
     636,   181,  1325,  1326,   182,  1328,  1505,  1506,   183,  1353,
    1354,  1427,  1137,  1333,   791,   792,  1334,   638,   574,   639,
     643,   648,   576,   577,  1447,   164,   640,   165,   166,   167,
     168,   169,   170,   649,   646,   647,   650,  1267,   171,   172,
     804,   173,   575,   174,   651,   654,   164,   655,   165,   166,
     167,   168,   169,   170,   657,   661,   184,   668,   674,   171,
     172,   675,   173,   185,   174,   672,   676,  1138,   673,   678,
     576,   577,   243,   689,   755,   679,  1292,   535,   692,   578,
     694,   688,   696,  1392,   693,   699,   698,  1477,   700,   830,
     756,   701,   334,  1301,  1407,   703,   702,  1302,  1303,  1367,
     704,   706,   175,  1309,   707,   716,   718,   720,   176,   721,
     244,  1327,   831,   245,  1018,   722,   727,   246,   247,   728,
     832,   249,   250,   175,   184,   833,   834,   251,   729,   176,
    1019,   185,   731,   732,  1471,  1472,  1473,  1508,   579,   771,
    1377,   772,   776,  1453,   777,  1453,  1490,   783,   788,   784,
     789,  1476,   793,  1139,   757,   790,   507,   252,   799,   758,
     759,   800,   253,   177,   178,   184,  -444,   760,   801,   809,
    1634,   837,   185,   841,  1355,   855,   335,  1199,   879,   882,
    1361,   785,  1363,   880,   177,   178,   579,   895,   906,   544,
    1509,   144,   145,   900,   761,   901,   908,  1020,   179,   180,
     907,   146,  1042,  1542,   909,   914,   917,  1140,  1545,  1546,
    1547,   918,  1550,   919,   925,   181,   147,   927,   182,   179,
     180,   937,   183,   928,   930,   148,   936,   149,   150,   939,
     938,  1141,   762,  1142,   941,   949,   181,   535,  1143,   182,
    1144,   950,   954,   183,  1145,  1146,  1475,   957,   961,   962,
     955,  1431,  1432,   963,  1309,   964,   967,   970,   971,  1575,
     321,  1478,   972,   974,   975,  1624,  1625,  1626,  1586,  1587,
    1588,   976,  1590,  1592,  1650,   986,   990,   991,  1606,  1002,
    1490,  1005,  1490,  1006,  1007,  1610,  1009,  1010,  1014,  1013,
    1147,   151,  1015,  1504,  1612,  1613,   336,  1051,  1033,  1034,
    1037,  1511,  1038,  1048,  1199,  1049,  1056,  1050,  1057,  1541,
    1059,  1543,  1064,  1079,  1065,  1067,  1070,   544,   544,   544,
     535,  1071,  1082,  1093,  1631,  1632,  1633,   164,  1081,   165,
     166,   167,   168,   169,   170,  1637,  1095,  1638,  1096,  1098,
     171,   172,  1358,   173,  1021,   174,   494,   495,   496,   497,
     498,  1099,  1105,  1641,  1100,  1101,   243,  1103,  1104,   152,
     499,   500,   501,   502,   503,   504,   505,   506,  1577,  1109,
     507,  1563,  1583,  1155,  1585,  1111,  1114,  1116,  1168,   184,
    1117,  1567,  1568,  1569,  1570,   356,   185,  1118,  1119,  1120,
    1123,  1022,  1124,  1023,   244,  1125,  1126,   245,   337,  -439,
     184,   246,   247,  1127,   175,   249,   250,   185,  1024,  1128,
     176,   251,  1129,  1130,  1131,   164,  1132,   165,   166,   167,
     168,   169,   170,  1133,  1163,  1151,   357,  1627,   171,   172,
    1152,   173,  1156,   174,  1157,  1635,  1164,   153,  1165,  1025,
    1026,   252,  1174,  1178,  1175,  1181,   253,   136,   137,   140,
     141,  1183,  1184,  1185,  1639,  1201,  1187,  1222,  1640,   198,
     156,  1642,  1223,  1224,  1239,   177,   178,   494,   495,   496,
     497,   498,   321,  1593,  1594,  1595,  1596,  1597,  1598,  1599,
    1600,   499,   500,   501,   502,   503,   504,   505,   506,  1653,
    1262,   507,   175,   157,   322,   158,   159,  1275,   176,  1644,
     179,   180,  1270,  1240,  1283,  1284,  1241,  1462,  1285,  1286,
    1242,   494,   495,   496,   497,   498,  1304,   181,  1243,  1294,
     182,  1244,  1246,  1247,   183,   499,   500,   501,   502,   503,
     504,   505,   506,  1248,  1293,   507,   160,   161,   162,   163,
    1249,   494,   495,   496,   497,   498,  1250,  1251,  1252,  1295,
    1269,  1274,  1299,   177,   178,   499,   500,   501,   502,   503,
     504,   505,   506,  1300,  1308,   507,   494,   495,   496,   497,
     498,  1313,  1317,  1329,  1331,  1332,  1335,  1343,   373,   374,
     499,   500,   501,   502,   503,   504,   505,   506,   179,   180,
     507,  1344,  1345,  1346,  1357,  1360,  1368,  1372,  1373,   494,
     495,   496,   497,   498,  1371,   181,  1378,  1376,   182,  1380,
    1382,  1393,   183,   499,   500,   501,   502,   503,   504,   505,
     506,  1400,  1408,   507,   494,   495,   496,   497,   498,   508,
     509,   510,   511,   512,   513,   514,   515,   516,   499,   500,
     501,   502,   503,   504,   505,   506,  1415,  1416,   507,   494,
     495,   496,   497,   498,  1417,  1419,  1448,  1463,  1464,  1465,
    1439,  1466,  1420,   499,   500,   501,   502,   503,   504,   505,
     506,  1424,  1467,   507,  1481,   494,   495,   496,   497,   498,
    1483,   184,   358,  1498,  1499,  1500,  1501,  1502,   185,   499,
     500,   501,   502,   503,   504,   505,   506,  1503,  1507,   507,
     494,   495,   496,   497,   498,  1510,  1063,  1544,  1548,  1554,
    1555,  1556,  1557,  1558,   499,   500,   501,   502,   503,   504,
     505,   506,  1559,  1560,   507,   494,   495,   496,   497,   498,
    1564,  1571,  1572,  1573,  1574,  1578,  1468,  1576,  1582,   499,
     500,   501,   502,   503,   504,   505,   506,  1584,  1589,   507,
     856,   857,   858,   859,   860,   861,   862,  1601,   863,   864,
     865,   866,   867,   494,   495,   496,   497,   498,  1605,   184,
    1607,  1608,  1609,  1611,  1636,  1645,   185,   499,   500,   501,
     502,   503,   504,   505,   506,  1643,  1646,   507,   494,   495,
     496,   497,   498,  1647,  1652,  1654,  1648,  1656,  1060,   840,
     717,   583,   499,   500,   501,   502,   503,   504,   505,   506,
    1173,  1172,   507,  1273,   494,   495,   496,   497,   498,  1203,
    1228,   730,  1312,  1011,   754,  1077,   871,  1194,   499,   500,
     501,   502,   503,   504,   505,   506,  1561,  1054,   507,   494,
     495,   496,   497,   498,  1197,  1617,   892,   442,  1055,  1619,
    1058,  1306,  1362,   499,   500,   501,   502,   503,   504,   505,
     506,  1438,  1186,   507,   494,   495,   496,   497,   498,  1235,
    1450,  1287,  1374,  1169,  1106,  1386,   458,     0,   499,   500,
     501,   502,   503,   504,   505,   506,     0,     0,   507,   494,
     495,   496,   497,   498,     0,     0,     0,     0,     0,     0,
       0,  1288,     0,   499,   500,   501,   502,   503,   504,   505,
     506,     0,     0,   507,   494,   495,   496,   497,   498,     0,
       0,     0,     0,     0,     0,     0,  1289,     0,   499,   500,
     501,   502,   503,   504,   505,   506,     0,     0,   507,     0,
       0,     0,     0,     0,     0,     0,   494,   495,   496,   497,
     498,     0,   459,   460,     0,   461,   462,     0,     0,  1290,
     499,   500,   501,   502,   503,   504,   505,   506,     0,     0,
     507,   494,   495,   496,   497,   498,     0,     0,     0,   463,
       0,     0,     0,     0,  1296,   499,   500,   501,   502,   503,
     504,   505,   506,   243,     0,   507,   856,   857,   858,   859,
     860,   861,   862,     0,   863,   864,   865,   866,   867,  1356,
     464,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     346,   347,   348,     0,   349,   350,     0,   351,     0,     0,
       0,   244,     0,     0,   245,  1418,     0,   318,   246,   247,
       0,   243,   249,   250,     0,     0,     0,     0,   251,     0,
       0,     0,     0,   232,     0,     0,     0,     0,   233,     0,
    1615,     0,   319,   465,   466,     0,   467,   468,   469,   470,
     471,   472,   473,   234,     0,     0,     0,     0,   252,   244,
       0,     0,   245,   253,     0,  1620,   246,   247,     0,   320,
     249,   250,     0,     0,     0,   243,   251,     0,     0,     0,
       0,     0,     0,   321,     0,     0,   235,     0,     0,     0,
       0,     0,   474,   475,   476,   477,   478,   479,   480,   481,
     482,     0,     0,  1621,     0,   322,   252,     0,     0,     0,
       0,   253,     0,   244,     0,     0,   245,     0,   483,   484,
     246,   247,     0,  1085,   249,   250,     0,     0,  1622,     0,
     251,     0,     0,     0,   485,   486,   487,   488,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   243,     0,     0,  1623,     0,   259,     0,     0,     0,
     252,     0,     0,     0,     0,   253,     0,     0,     0,     0,
       0,   260,     0,   243,   236,     0,     0,  1061,     0,     0,
       0,     0,     0,     0,     0,   261,     0,     0,   243,   244,
       0,     0,   245,     0,     0,     0,   246,   247,   262,   263,
     249,   250,  1365,     0,     0,     0,   251,     0,   489,   490,
     264,   244,     0,   265,   245,   266,     0,     0,   246,   247,
       0,     0,   249,   250,     0,     0,   244,  1423,   251,   245,
       0,     0,     0,   246,   247,     0,   252,   249,   250,     0,
       0,   253,   237,   251,     0,     0,     0,     0,   243,     0,
    1086,  1087,  1425,     0,     0,   267,     0,   268,   252,     0,
       0,     0,     0,   253,     0,     0,   269,     0,     0,     0,
       0,     0,     0,   252,     0,     0,     0,     0,   253,     0,
       0,     0,     0,   323,  1496,     0,   244,     0,     0,   245,
       0,     0,   230,   246,   247,     0,   342,   249,   250,   352,
     238,   242,   270,   251,   243,   239,   257,     0,   258,  1497,
     299,     0,   312,  1512,   317,     0,   333,     0,   341,   271,
     240,   324,   325,   326,     0,   241,   272,   273,     0,   274,
     327,   275,   276,   252,   277,   278,     0,   279,   253,     0,
     280,   281,   244,   282,     0,   245,     0,   328,     0,   246,
     247,     0,   248,   249,   250,   321,   283,   284,     0,   251,
       0,   285,     0,   286,   287,     0,   288,     0,     0,     0,
       0,   289,     0,     0,     0,   290,   291,   322,   292,   293,
       0,   294,   295,   296,     0,     0,     0,     0,     0,   252,
    1513,     0,     0,   739,   253,     0,     0,     0,     0,     0,
       0,  1088,     0,     0,  1514,     0,     0,     0,   402,   403,
     300,     0,     0,     0,     0,   301,   302,   303,     0,     0,
       0,   494,   495,   496,   497,   498,     0,     0,     0,     0,
       0,     0,     0,     0,   740,   499,   500,   501,   502,   503,
     504,   505,   506,     0,     0,   507,     0,     0,     0,     0,
       0,   304,     0,     0,     0,     0,     0,     0,   305,     0,
       0,  1515,   306,     0,  1516,  1517,     0,     0,     0,     0,
       0,     0,     0,   741,     0,     0,     0,   297,   742,     0,
       0,     0,     0,   313,   314,   743,   744,     0,     0,     0,
       0,     4,     0,     0,     0,     0,   745,     0,     0,   307,
       0,     0,     0,     0,     0,   746,   747,   748,   749,     0,
     750,   751,     0,     0,   315,     0,     5,     6,     0,     0,
       0,  1518,     0,     0,  1519,     0,     7,     0,  1520,     0,
       0,     0,     0,     0,     0,     0,  1521,     0,  1522,     0,
    1523,  1524,  1525,  1526,  1527,  1528,  -445,  -445,  -445,     0,
       8,  1529,     0,     0,     0,  1530,  1531,     0,     0,  1532,
    -445,  -445,  -445,  -445,   503,   504,   505,   506,   343,     0,
     507,  1533,     0,     0,   344,     0,     0,     0,     9,  1534,
       0,    10,     0,    11,     0,     0,     0,     0,     0,     0,
      12,     0,    13,     0,     0,     0,  1366,    14,    15,    16,
       0,     0,   579,   493,    17,    18,    19,     0,   494,   495,
     496,   497,   498,     0,     0,     0,     0,     0,     0,  1535,
       0,     0,   499,   500,   501,   502,   503,   504,   505,   506,
     254,     0,   507,   494,   495,   496,   497,   498,     0,     0,
       0,     0,     0,     0,   494,   495,   496,   499,   500,   501,
     502,   503,   504,   505,   506,     0,     0,   507,   499,   500,
     501,   502,   503,   504,   505,   506,     0,     0,   507
};

static const yytype_int16 yycheck[] =
{
      17,   185,   300,    83,   185,   661,   658,  1048,    18,    19,
     216,    22,     7,   154,   155,   749,   153,   880,    15,   273,
     274,   110,    15,   112,    29,   114,    19,   116,   123,   118,
       6,   120,    18,    19,   123,   124,  1071,     6,   690,    15,
      16,    17,    18,    19,    18,    19,    15,    16,    17,    18,
    1347,    15,    41,   105,   208,    19,    15,    16,    17,    18,
      19,   215,   216,    44,    28,    97,    97,    84,    15,    16,
      17,    18,    19,     4,    50,    41,     6,  1217,    26,    27,
       8,     9,    10,    11,    12,    15,    16,    17,    18,    19,
     984,  1294,   246,    97,    22,    23,    24,    25,    26,    27,
      28,    29,    15,    35,    32,    30,    19,     8,     9,    10,
      11,    12,    44,    83,    90,    15,   149,  1218,   272,    19,
      50,    22,    23,    24,    25,    26,    27,    28,    29,   630,
     207,    32,    19,    27,    34,    35,    36,    37,    15,    16,
      17,    18,    19,    41,    15,    16,    17,    18,    92,   303,
      15,    16,    17,    18,    15,    19,   145,   174,    19,    35,
      90,   148,  1430,  1430,   318,   223,   243,   184,    44,    35,
     324,   325,   252,    22,    35,    75,  1387,   678,    44,   145,
      44,   233,  1217,    32,     3,   265,   162,   163,   164,   165,
     166,   167,   168,    15,   335,   253,     0,    19,    15,    15,
     247,   853,    19,    19,   164,     3,   182,   142,   765,   185,
     313,   768,    15,   770,    49,   369,    19,   320,  1421,   266,
    1517,   254,  1427,    15,   271,     3,  1366,    19,   126,   127,
     128,    66,   162,   163,   164,   165,   166,   167,   168,   286,
    1451,   321,    34,    35,    36,    37,    19,    20,   295,   227,
     391,   123,   182,   221,   301,   185,   393,   304,   395,   396,
     238,   398,   399,  1364,   123,  1300,   236,     8,     9,    10,
      11,    12,   570,  1167,   306,   306,   371,   309,   309,  1484,
     123,    22,    23,    24,    25,    26,    27,    28,    29,   176,
      19,    32,     8,     9,    10,    11,    12,   222,  1566,  1566,
     347,   117,   349,   290,   239,    19,    22,    23,    24,    25,
      26,    27,    28,    29,   117,    44,    32,   211,     8,     9,
      10,    11,    12,    19,   317,   318,   313,   271,   534,   148,
      80,  1366,    22,    23,    24,    25,    26,    27,    28,    29,
     113,    19,    32,  1483,   140,   141,  1080,    87,   352,   353,
     604,     8,     9,    10,    11,    12,   367,   140,   141,  1222,
    1565,   371,    87,  1274,   369,    22,    23,    24,    25,    26,
      27,    28,    29,   368,   367,    32,   373,   366,  1419,   279,
     534,   108,   122,   110,   371,   371,   540,   355,   356,   370,
      67,   367,  1427,   547,   108,   371,    19,   371,   367,    19,
     366,   418,   371,   367,   290,   291,   125,   424,   367,   373,
     140,   141,    98,    18,    19,   432,   433,   434,   435,   436,
     367,   575,   370,    19,   216,   217,   166,   681,   582,   683,
     584,   448,  1343,  1344,  1345,  1346,   113,   367,   370,   325,
     368,   371,   119,   523,   372,   108,   600,   110,  1483,  1484,
     136,   705,    83,   366,   367,   415,   610,   417,   612,   419,
     420,   615,   109,   372,   618,   619,   375,   367,   366,    20,
    1027,   372,  1029,    93,   370,    18,    19,   494,   495,   496,
     497,   498,   499,   500,   501,   502,   503,   504,   505,   506,
     367,  1225,   646,   368,   370,   749,   367,   372,    19,   295,
    1156,    66,   367,   657,   370,   522,   367,     8,     9,    10,
      11,    12,   295,    15,   668,     3,  1557,    19,   535,     3,
     774,    22,    23,    24,    25,    26,    27,    28,    29,    50,
    1565,    32,    34,    35,    36,    37,   156,   333,   372,    15,
     694,   375,   696,    19,   366,   367,   593,   594,    20,   703,
     367,   367,    19,    20,   601,     3,   603,   217,    34,    35,
      36,    37,   579,     3,   367,   295,    68,   214,   215,    90,
       3,   218,   219,   620,    83,   367,   236,    44,   625,     3,
     627,  1233,   629,     3,   631,    57,    58,   634,   635,   636,
       8,     9,    10,    11,    12,   224,   643,   738,   233,    96,
     647,  1253,   145,     3,    22,    23,    24,    25,    26,    27,
      28,    29,   369,   273,    32,   372,   370,    15,   372,   773,
       3,    19,   876,    19,    20,   809,   810,   262,   263,   676,
     767,   372,   769,   370,   815,   372,    34,    35,    36,    37,
     938,   162,   163,   164,   165,   166,   167,   168,    44,    15,
      33,   698,   699,    19,   151,   152,   372,   130,     5,    42,
     133,   182,    45,   582,   185,   584,   133,   134,    34,    35,
      36,    37,     3,    56,     8,     9,    10,    11,    12,    19,
      42,   341,   372,    38,    39,    40,    69,    78,    22,    23,
      24,    25,    26,    27,    28,    29,    79,     3,    32,    61,
      83,     3,   322,    86,     3,    88,    89,    98,    99,   206,
     217,   371,     3,    96,   211,   372,     3,   337,   338,   873,
     111,   112,  1020,   114,   371,  1023,    88,     3,  1026,   236,
      19,    20,   986,    29,    81,    82,   211,    84,    85,   367,
     123,   124,   883,   242,   243,    20,   900,   901,     3,   903,
     342,   343,   135,   169,   170,   171,   172,   173,   174,   175,
      19,   177,   178,   179,   180,   181,   273,   921,   922,    19,
      20,   925,   367,   927,   928,   158,   930,   367,   161,   367,
      55,   367,   936,   367,   801,   367,   803,   149,    63,    64,
      65,    50,   139,    15,    44,     3,    15,    22,    17,    18,
      62,   955,    13,   957,    15,    16,    17,    18,    19,    20,
     899,    18,    19,    20,   290,    26,    27,    28,    29,   217,
      31,   252,   253,   977,   366,    15,  1080,   142,    83,    19,
     131,    90,    19,  1087,   341,    19,    20,     8,     9,    10,
      11,    12,   371,   372,    34,    35,    36,    37,   895,   366,
     371,    22,    23,    24,    25,    26,    27,    28,    29,   108,
     907,    32,    19,   118,   371,   367,    15,   914,    17,    18,
      13,   372,    15,    16,    17,    18,    19,    20,   133,    90,
     370,   298,    19,    26,    27,    96,    29,   167,    31,   108,
     145,   367,    59,    60,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   162,   163,   164,   165,   166,   167,   168,
     370,    13,   370,    15,    16,    17,    18,    19,    20,    87,
     267,   268,    68,   182,    26,    27,   185,    29,   366,    31,
     366,   348,   349,   350,   351,   370,   283,   284,   285,   286,
     151,   152,     8,     9,    10,    11,    12,    90,    59,    60,
     230,   370,   368,    96,   372,   280,    22,    23,    24,    25,
      26,    27,    28,    29,  1262,   298,    32,    80,   415,   367,
     417,  1225,   419,   420,   366,   186,   187,   188,   189,   190,
     191,    20,   262,   366,   240,    15,    28,    29,    90,    19,
      32,   271,   203,    68,    96,   206,    19,    20,   128,   210,
     278,   367,  1143,    75,    34,    35,    36,    37,   151,   152,
     357,   358,   371,   372,  1168,   348,   349,   350,   351,  1036,
      43,    75,  1039,  1040,  1041,   225,  1043,   232,  1045,   371,
     372,    54,   236,  1050,   368,   238,  1053,     8,     9,    10,
      11,    12,  1059,   186,   187,   188,   189,   190,   191,   151,
     152,    22,    23,    24,    25,    26,    27,    28,    29,   252,
     203,    32,   256,   206,    18,    19,    20,   210,    15,   261,
      44,   259,    19,    80,  1160,  1161,  1162,   371,   372,   274,
     360,   361,   362,   363,   186,   187,  1240,    34,    35,    36,
      37,  1138,   266,  1140,    68,   102,   103,   104,   236,  1146,
     272,   203,  1256,  1257,   206,  1259,   216,   217,   210,  1285,
    1286,   370,    93,  1267,   428,   429,  1270,   223,    44,   366,
      86,   366,    96,    97,  1378,    13,   370,    15,    16,    17,
      18,    19,    20,   370,    87,   321,   370,   327,    26,    27,
      28,    29,    68,    31,    83,   366,    13,   370,    15,    16,
      17,    18,    19,    20,    75,   370,   367,    75,   370,    26,
      27,   370,    29,   374,    31,   366,   280,   148,   366,    83,
      96,    97,    45,   370,     3,   366,  1193,   367,    20,   105,
      83,   366,    75,  1337,   366,    80,    78,   217,   366,   171,
      19,   366,    65,  1210,  1348,    75,   366,  1214,  1215,   370,
      20,   366,    90,  1220,   366,     3,     3,     3,    96,     3,
      83,  1258,   194,    86,     3,     3,     3,    90,    91,    19,
     202,    94,    95,    90,   367,   207,   208,   100,     3,    96,
      19,   374,     3,     3,  1388,  1389,  1390,  1443,   212,     3,
    1320,     3,   143,  1384,   143,  1386,  1430,    14,    26,    15,
     211,  1405,    29,   234,    83,    27,    32,   130,    19,    88,
      89,    15,   135,   151,   152,   367,   368,    96,   372,     7,
     217,     3,   374,   366,  1291,   366,   149,  1294,   366,    19,
    1297,  1298,  1299,   367,   151,   152,   212,   106,   366,  1443,
    1444,   113,   114,    75,   123,    75,   366,    86,   186,   187,
      50,   123,   368,  1457,   366,   264,   366,   288,  1462,  1463,
    1464,   366,  1466,    30,   145,   203,   138,    75,   206,   186,
     187,   366,   210,    75,    75,   147,    75,   149,   150,   366,
      87,   312,   161,   314,   366,   366,   203,   367,   319,   206,
     321,   366,   366,   210,   325,   326,  1393,    68,    19,   366,
     145,  1368,  1369,   366,  1371,   366,   366,   366,   366,  1513,
     107,  1408,   366,   366,   366,  1571,  1572,  1573,  1522,  1523,
    1524,   366,  1526,  1527,  1628,   230,   366,   366,  1532,     3,
    1564,     3,  1566,    19,    15,  1539,    19,    19,     3,    19,
     371,   213,    20,  1440,  1548,  1549,   269,   368,    15,    15,
     368,  1448,    27,     7,  1421,   372,    15,   372,    18,  1456,
     367,  1458,    20,    27,   366,   366,   366,  1571,  1572,  1573,
     367,   370,   101,   366,  1578,  1579,  1580,    13,   372,    15,
      16,    17,    18,    19,    20,  1589,   366,  1591,   366,   366,
      26,    27,    28,    29,   233,    31,     8,     9,    10,    11,
      12,   366,    19,  1607,   366,   366,    45,   366,   366,   281,
      22,    23,    24,    25,    26,    27,    28,    29,  1515,   366,
      32,  1488,  1519,   371,  1521,   366,   366,   366,   324,   367,
     366,  1498,  1499,  1500,  1501,    74,   374,   366,   366,   366,
     366,   280,   366,   282,    83,   366,   366,    86,   371,   366,
     367,    90,    91,   366,    90,    94,    95,   374,   297,   366,
      96,   100,   366,   366,   366,    13,   366,    15,    16,    17,
      18,    19,    20,   366,   371,   366,   115,  1574,    26,    27,
     366,    29,   366,    31,   366,  1582,   366,   359,   366,   328,
     329,   130,     3,    15,     3,     3,   135,     7,     8,     9,
      10,     3,   145,   145,  1601,   372,    27,   366,  1605,    19,
      78,  1608,   368,   143,   366,   151,   152,     8,     9,    10,
      11,    12,   107,   298,   299,   300,   301,   302,   303,   304,
     305,    22,    23,    24,    25,    26,    27,    28,    29,  1636,
     280,    32,    90,   111,   129,   113,   114,    20,    96,  1616,
     186,   187,   330,   372,     3,     3,   366,   142,    15,    15,
     366,     8,     9,    10,    11,    12,   371,   203,   366,   372,
     206,   366,   366,   366,   210,    22,    23,    24,    25,    26,
      27,    28,    29,   366,   368,    32,   154,   155,   156,   157,
     366,     8,     9,    10,    11,    12,   366,   366,   366,    15,
     366,   366,    22,   151,   152,    22,    23,    24,    25,    26,
      27,    28,    29,   367,   366,    32,     8,     9,    10,    11,
      12,    15,   370,   370,   370,   370,   370,   366,   138,   139,
      22,    23,    24,    25,    26,    27,    28,    29,   186,   187,
      32,   366,   366,   366,     7,   371,    22,   366,   145,     8,
       9,    10,    11,    12,   372,   203,   223,    19,   206,   366,
     370,   274,   210,    22,    23,    24,    25,    26,    27,    28,
      29,   366,   274,    32,     8,     9,    10,    11,    12,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    22,    23,
      24,    25,    26,    27,    28,    29,   366,     3,    32,     8,
       9,    10,    11,    12,     3,   367,   220,   292,   293,   294,
      15,   296,   368,    22,    23,    24,    25,    26,    27,    28,
      29,   368,   307,    32,   368,     8,     9,    10,    11,    12,
     184,   367,   371,   367,   367,   367,   367,   366,   374,    22,
      23,    24,    25,    26,    27,    28,    29,   366,   217,    32,
       8,     9,    10,    11,    12,   366,   368,   366,   238,   366,
     315,   315,   372,   368,    22,    23,    24,    25,    26,    27,
      28,    29,   366,   371,    32,     8,     9,    10,    11,    12,
       7,   218,   218,   218,   280,   316,   371,   370,   274,    22,
      23,    24,    25,    26,    27,    28,    29,   335,   238,    32,
     169,   170,   171,   172,   173,   174,   175,   332,   177,   178,
     179,   180,   181,     8,     9,    10,    11,    12,   300,   367,
     324,   298,   366,   366,   336,   366,   374,    22,    23,    24,
      25,    26,    27,    28,    29,   368,   366,    32,     8,     9,
      10,    11,    12,   366,   315,   310,   366,   366,   836,   526,
     365,   250,    22,    23,    24,    25,    26,    27,    28,    29,
    1004,  1001,    32,  1156,     8,     9,    10,    11,    12,  1070,
    1081,   384,  1222,   744,   391,   881,   555,   368,    22,    23,
      24,    25,    26,    27,    28,    29,  1483,   809,    32,     8,
       9,    10,    11,    12,  1049,  1564,    19,   184,   810,  1566,
     815,  1218,  1298,    22,    23,    24,    25,    26,    27,    28,
      29,  1371,  1036,    32,     8,     9,    10,    11,    12,  1086,
    1381,   368,  1316,   988,   920,  1332,     5,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,     8,
       9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   368,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,    -1,    32,     8,     9,    10,    11,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   368,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,     9,    10,    11,
      12,    -1,    81,    82,    -1,    84,    85,    -1,    -1,   368,
      22,    23,    24,    25,    26,    27,    28,    29,    -1,    -1,
      32,     8,     9,    10,    11,    12,    -1,    -1,    -1,   108,
      -1,    -1,    -1,    -1,   368,    22,    23,    24,    25,    26,
      27,    28,    29,    45,    -1,    32,   169,   170,   171,   172,
     173,   174,   175,    -1,   177,   178,   179,   180,   181,   368,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    -1,    76,    77,    -1,    79,    -1,    -1,
      -1,    83,    -1,    -1,    86,   368,    -1,    41,    90,    91,
      -1,    45,    94,    95,    -1,    -1,    -1,    -1,   100,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,
     368,    -1,    66,   192,   193,    -1,   195,   196,   197,   198,
     199,   200,   201,    92,    -1,    -1,    -1,    -1,   130,    83,
      -1,    -1,    86,   135,    -1,   368,    90,    91,    -1,    93,
      94,    95,    -1,    -1,    -1,    45,   100,    -1,    -1,    -1,
      -1,    -1,    -1,   107,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,   241,   242,   243,   244,   245,   246,   247,   248,
     249,    -1,    -1,   368,    -1,   129,   130,    -1,    -1,    -1,
      -1,   135,    -1,    83,    -1,    -1,    86,    -1,   267,   268,
      90,    91,    -1,    93,    94,    95,    -1,    -1,   368,    -1,
     100,    -1,    -1,    -1,   283,   284,   285,   286,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    45,    -1,    -1,   368,    -1,    50,    -1,    -1,    -1,
     130,    -1,    -1,    -1,    -1,   135,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    45,   213,    -1,    -1,   366,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    79,    -1,    -1,    45,    83,
      -1,    -1,    86,    -1,    -1,    -1,    90,    91,    92,    93,
      94,    95,   366,    -1,    -1,    -1,   100,    -1,   357,   358,
     104,    83,    -1,   107,    86,   109,    -1,    -1,    90,    91,
      -1,    -1,    94,    95,    -1,    -1,    83,   366,   100,    86,
      -1,    -1,    -1,    90,    91,    -1,   130,    94,    95,    -1,
      -1,   135,   281,   100,    -1,    -1,    -1,    -1,    45,    -1,
     220,   221,   366,    -1,    -1,   149,    -1,   151,   130,    -1,
      -1,    -1,    -1,   135,    -1,    -1,   160,    -1,    -1,    -1,
      -1,    -1,    -1,   130,    -1,    -1,    -1,    -1,   135,    -1,
      -1,    -1,    -1,   307,   366,    -1,    83,    -1,    -1,    86,
      -1,    -1,    93,    90,    91,    -1,    93,    94,    95,   371,
     339,   102,   196,   100,    45,   344,   107,    -1,   109,   366,
     111,    -1,   113,    65,   115,    -1,   117,    -1,   119,   213,
     359,   345,   346,   347,    -1,   364,   220,   221,    -1,   223,
     354,   225,   226,   130,   228,   229,    -1,   231,   135,    -1,
     234,   235,    83,   237,    -1,    86,    -1,   371,    -1,    90,
      91,    -1,    93,    94,    95,   107,   250,   251,    -1,   100,
      -1,   255,    -1,   257,   258,    -1,   260,    -1,    -1,    -1,
      -1,   265,    -1,    -1,    -1,   269,   270,   129,   272,   273,
      -1,   275,   276,   277,    -1,    -1,    -1,    -1,    -1,   130,
     142,    -1,    -1,     3,   135,    -1,    -1,    -1,    -1,    -1,
      -1,   371,    -1,    -1,   156,    -1,    -1,    -1,    18,    19,
     282,    -1,    -1,    -1,    -1,   287,   288,   289,    -1,    -1,
      -1,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    22,    23,    24,    25,    26,
      27,    28,    29,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   323,    -1,    -1,    -1,    -1,    -1,    -1,   330,    -1,
      -1,   213,   334,    -1,   216,   217,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    -1,    -1,   371,    88,    -1,
      -1,    -1,    -1,   340,   341,    95,    96,    -1,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    -1,   106,    -1,    -1,   371,
      -1,    -1,    -1,    -1,    -1,   115,   116,   117,   118,    -1,
     120,   121,    -1,    -1,   371,    -1,    70,    71,    -1,    -1,
      -1,   273,    -1,    -1,   276,    -1,    80,    -1,   280,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,   290,    -1,
     292,   293,   294,   295,   296,   297,     8,     9,    10,    -1,
     104,   303,    -1,    -1,    -1,   307,   308,    -1,    -1,   311,
      22,    23,    24,    25,    26,    27,    28,    29,   365,    -1,
      32,   323,    -1,    -1,   371,    -1,    -1,    -1,   132,   331,
      -1,   135,    -1,   137,    -1,    -1,    -1,    -1,    -1,    -1,
     144,    -1,   146,    -1,    -1,    -1,   183,   151,   152,   153,
      -1,    -1,   212,     3,   158,   159,   160,    -1,     8,     9,
      10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,   371,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
     371,    -1,    32,     8,     9,    10,    11,    12,    -1,    -1,
      -1,    -1,    -1,    -1,     8,     9,    10,    22,    23,    24,
      25,    26,    27,    28,    29,    -1,    -1,    32,    22,    23,
      24,    25,    26,    27,    28,    29,    -1,    -1,    32
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     4,   377,   378,    45,    70,    71,    80,   104,   132,
     135,   137,   144,   146,   151,   152,   153,   158,   159,   160,
     402,   403,   404,   405,   406,   407,   458,   459,   460,   461,
     463,   464,   465,   468,   471,   474,   475,   476,   477,   478,
     479,   480,   481,   482,   483,   484,   485,   486,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   565,   602,   603,
     604,   605,   606,   607,   608,   609,   628,   638,     0,     3,
      33,    42,    45,    56,    69,    79,    83,    86,    88,    89,
      96,   123,   124,   135,   158,   161,   366,   379,   391,   397,
     400,   401,   410,   412,   415,   417,   420,   422,   423,   425,
     426,   433,   443,   444,   500,   502,   512,   551,   554,   566,
     568,   569,   570,   611,   612,   613,   614,   622,   623,   629,
     630,   637,   639,   640,   641,    19,    20,    44,   498,    20,
      55,    63,    64,    65,   408,     3,   498,   498,   133,   134,
     498,   498,     3,   123,   113,   114,   123,   138,   147,   149,
     150,   213,   281,   359,   123,   123,    78,   111,   113,   114,
     154,   155,   156,   157,    13,    15,    16,    17,    18,    19,
      20,    26,    27,    29,    31,    90,    96,   151,   152,   186,
     187,   203,   206,   210,   367,   374,   517,   530,   535,   538,
     539,   540,   542,   544,    35,    44,   370,   487,   498,    19,
      19,    20,    43,    54,   389,   393,    67,   113,   119,    42,
      61,    88,   149,   398,   399,    49,    66,    80,    19,    87,
      19,    83,   125,   113,   389,   544,    19,    19,   108,   427,
     427,   370,    72,    77,    92,   125,   213,   281,   339,   344,
     359,   364,   427,    45,    83,    86,    90,    91,    93,    94,
      95,   100,   130,   135,   371,   428,   446,   427,   427,    50,
      65,    79,    92,    93,   104,   107,   109,   149,   151,   160,
     196,   213,   220,   221,   223,   225,   226,   228,   229,   231,
     234,   235,   237,   250,   251,   255,   257,   258,   260,   265,
     269,   270,   272,   273,   275,   276,   277,   371,   428,   427,
     282,   287,   288,   289,   323,   330,   334,   371,   428,   571,
     572,   573,   427,   340,   341,   371,   428,   427,    41,    66,
      93,   107,   129,   307,   345,   346,   347,   354,   371,   428,
     453,   455,   456,   427,    65,   149,   269,   371,   428,   624,
     627,   427,    93,   365,   371,   428,    72,    73,    74,    76,
      77,    79,   371,   411,   414,   428,    74,   115,   371,   413,
     428,    35,    44,   370,   447,   408,     3,     3,   408,    66,
     409,     3,     3,   498,   498,     3,    35,    44,   370,   450,
      83,   473,    19,    20,    44,   499,     3,   148,   469,     3,
       3,   466,   472,   224,    19,    93,   156,   322,   337,   338,
     462,   462,    18,    19,   383,   385,   385,     3,     3,     3,
       3,     3,     3,     3,     3,    15,    19,    28,   367,   373,
     534,   536,   542,    29,   367,   211,    27,   211,    15,   373,
     537,   544,   367,   367,   367,   367,   367,   367,   188,   189,
     190,   191,   524,   544,    15,    16,    17,    19,   367,   517,
     518,   522,   525,   526,   528,   530,   531,   532,     5,    81,
      82,    84,    85,   108,   139,   192,   193,   195,   196,   197,
     198,   199,   200,   201,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   267,   268,   283,   284,   285,   286,   357,
     358,   543,   642,     3,     8,     9,    10,    11,    12,    22,
      23,    24,    25,    26,    27,    28,    29,    32,    45,    46,
      47,    48,    49,    50,    51,    52,    53,   395,   396,    15,
     488,     3,    22,    62,   392,    44,   370,   394,    15,    19,
      34,    35,    36,    37,    68,   367,   380,   382,   447,   380,
      20,    57,    58,   366,   380,   381,   381,   142,   131,   421,
      15,    17,    18,   108,   416,   501,    19,   418,   450,   366,
     513,   108,    19,   370,   370,   424,   370,   447,    19,   432,
      87,   380,   382,   445,    44,    68,    96,    97,   105,   212,
     429,   509,    68,   429,    68,   389,   450,   366,   366,   370,
     370,   142,   239,   280,    80,   366,    20,   240,   389,   382,
      68,   278,   149,   254,   128,   382,   366,   380,   383,   567,
     145,   383,    75,   227,   238,    75,   225,   232,   242,   243,
     236,   238,   252,   253,   252,   256,   382,   259,   261,   266,
      83,   236,    92,   271,   236,   274,   272,   382,   223,   366,
     370,   416,   382,    86,   380,   382,    87,   321,   366,   370,
     370,    83,   342,   343,   366,   370,   380,    75,   620,   621,
     389,   370,    97,   352,   353,   615,   380,   380,    75,   221,
     355,   356,   366,   366,   370,   370,   280,   385,    83,   366,
     167,   230,   262,   271,   360,   361,   362,   363,   366,   370,
     635,   636,    20,   366,    83,   382,    75,   382,    78,    80,
     366,   366,   366,    75,    20,   390,   366,   366,    78,    98,
      99,   111,   112,   114,   448,   449,     3,   409,     3,   380,
       3,     3,     3,    98,   136,   451,   452,     3,    19,     3,
     499,     3,     3,     3,    83,   118,   133,   145,   470,     3,
      44,    83,    88,    95,    96,   106,   115,   116,   117,   118,
     120,   121,   385,   467,   509,     3,    19,    83,    88,    89,
      96,   123,   161,   462,   462,   462,   610,    44,   462,    44,
     462,     3,     3,    29,   369,   387,   143,   143,   536,   536,
     544,   536,   536,    14,    15,   544,   548,   549,    26,   211,
      27,   537,   537,    29,   544,   544,   544,   544,   544,    19,
      15,   372,   368,   372,    28,   520,   521,   524,   544,     7,
     372,   375,    26,    27,   370,   372,   375,   544,   544,   544,
     544,   544,   544,   544,   544,   544,   544,   544,   544,   544,
     171,   194,   202,   207,   208,   371,   372,     3,   544,   389,
     395,   366,   381,   544,   380,    59,    60,    59,    60,    38,
      39,    40,   380,   440,   441,   366,   169,   170,   171,   172,
     173,   174,   175,   177,   178,   179,   180,   181,   503,   504,
     505,   507,   511,    41,   126,   127,   128,   366,   419,   366,
     367,   510,    19,   123,   371,   416,   434,   437,   438,    35,
     380,   431,    19,   507,   508,   106,   544,   431,   431,   553,
      75,    75,   382,    75,   279,   382,   366,    50,   366,   366,
     380,   216,   217,   382,   264,   382,   383,   366,   366,    30,
     384,    41,   145,   366,   380,   145,   380,    75,    75,   380,
      75,   105,   233,   380,   380,   382,    75,   366,    87,   366,
     382,   366,   382,   233,   262,   263,   382,   432,   382,   366,
     366,   382,   382,   382,   366,   145,   382,    68,   380,   382,
     574,    19,   366,   366,   366,   380,   438,   366,   454,   455,
     366,   366,   366,   380,   366,   366,   366,    87,   122,   166,
     457,   382,   432,    20,   383,   383,   230,   625,   626,   438,
     366,   366,   380,   380,   382,   382,   380,   383,   385,   386,
     371,   372,     3,   371,   372,     3,    19,    15,   385,    19,
      19,   508,   386,    19,     3,    20,   601,   601,     3,    19,
      86,   233,   280,   282,   297,   328,   329,   462,   601,   462,
     601,   380,   383,    15,    15,   368,   372,   368,    27,   372,
     372,   372,   368,   372,   368,   372,   544,   544,     7,   372,
     372,   368,    18,   367,   518,   526,    15,    18,   531,   367,
     396,   366,   371,   368,    20,   366,   438,   366,    19,   176,
     366,   370,   380,   383,   368,   504,   506,   511,   385,    27,
     436,   372,   101,   430,   382,    93,   220,   221,   371,   428,
     552,   380,   380,   366,   380,   366,   366,   382,   366,   366,
     366,   366,   382,   366,   366,    19,   567,   380,   380,   366,
     380,   366,   380,   380,   366,   380,   366,   366,   366,   366,
     366,   380,   416,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   366,   366,   366,   380,   380,    93,   148,   234,
     288,   312,   314,   319,   321,   325,   326,   371,   575,   582,
     585,   366,   366,   370,   619,   371,   366,   366,   380,   108,
     110,   108,   110,   371,   366,   366,   384,   383,   324,   625,
     370,   634,   449,   452,     3,     3,   416,   416,    15,   416,
     601,     3,   601,     3,   145,   145,   548,    27,   544,   544,
     544,   544,   544,   372,   368,   520,   544,   521,   523,   544,
     544,   372,   544,   505,    19,    50,    90,   162,   163,   164,
     165,   166,   167,   168,   182,   185,   514,   515,   540,   541,
     545,   550,   366,   368,   143,    20,   386,   435,   437,    80,
     102,   103,   104,   555,   559,   555,   560,   383,   564,   366,
     372,   366,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   366,   594,   600,   382,   290,   291,   325,   290,
     382,   576,   280,   385,   578,   579,   577,   327,   382,   366,
     330,   595,   618,   454,   366,    20,   388,   388,   388,   388,
     384,   380,   633,     3,     3,    15,    15,   368,   368,   368,
     368,   372,   544,   368,   372,    15,   368,    22,   367,    22,
     367,   544,   544,   544,   371,   550,   543,   642,   366,   544,
     546,   547,   506,    15,   386,   438,   439,   370,   558,    30,
     222,   561,   380,   438,   596,   380,   380,   382,   380,   370,
     416,   370,   370,   380,   380,   370,   217,   236,   273,   341,
     371,   617,   457,   366,   366,   366,   366,   217,   236,   273,
     341,   371,   632,   473,   473,   544,   368,     7,    28,   523,
     371,   544,   549,   544,   540,   366,   183,   370,    22,    22,
      32,   372,   366,   145,   558,   557,    19,   389,   223,   562,
     366,   597,   370,   599,   587,   580,   587,   583,   140,   141,
     295,   442,   380,   274,   298,   348,   349,   350,   351,   616,
     366,   457,   457,   457,   457,   295,   442,   380,   274,   298,
     348,   349,   350,   351,   631,   366,     3,     3,   368,   367,
     368,     7,   368,   366,   368,   366,   642,   370,   516,   550,
     533,   544,   544,    96,   151,   152,   206,   211,   547,    15,
     109,   214,   215,   218,   219,   371,   556,   383,   220,   563,
     595,   598,   371,   385,   588,   589,   148,   290,   313,   371,
     581,   371,   142,   292,   293,   294,   296,   307,   371,   453,
     584,   380,   380,   380,   217,   382,   380,   217,   382,   520,
     523,   368,   514,   184,     6,    15,    16,    17,   367,   371,
     517,   519,   522,   525,   527,   529,   366,   366,   367,   367,
     367,   367,   366,   366,   382,   216,   217,   217,   381,   380,
     366,   382,    65,   142,   156,   213,   216,   217,   273,   276,
     280,   288,   290,   292,   293,   294,   295,   296,   297,   303,
     307,   308,   311,   323,   331,   371,   453,   586,   313,   320,
     590,   382,   380,   382,   366,   380,   380,   380,   238,   117,
     380,    97,   306,   309,   366,   315,   315,   372,   368,   366,
     371,   516,   514,   544,     7,   369,   372,   544,   544,   544,
     544,   218,   218,   218,   280,   380,   370,   382,   316,   295,
     333,   442,   274,   382,   335,   382,   380,   380,   380,   238,
     380,   117,   380,   298,   299,   300,   301,   302,   303,   304,
     305,   332,    97,   306,   309,   300,   380,   324,   298,   366,
     380,   366,   380,   380,   520,   368,   367,   519,   514,   527,
     368,   368,   368,   368,   381,   381,   381,   382,   591,   317,
     318,   380,   380,   380,   217,   382,   336,   380,   380,   382,
     382,   380,   382,   368,   544,   366,   366,   366,   366,   371,
     383,   593,   315,   382,   310,   592,   366
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   376,   377,   377,   378,   378,   379,   380,   380,   380,
     381,   381,   381,   382,   382,   382,   382,   382,   382,   383,
     383,   384,   384,   385,   386,   386,   387,   387,   388,   389,
     389,   390,   390,   391,   392,   392,   393,   393,   393,   394,
     394,   395,   395,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   397,   397,   398,   398,   398,   398,   399,   399,
     399,   399,   399,   399,   400,   401,   401,   401,   401,   402,
     403,   404,   405,   406,   407,   408,   408,   409,   409,   409,
     410,   411,   411,   411,   411,   411,   412,   413,   413,   414,
     415,   415,   416,   416,   416,   417,   418,   418,   419,   419,
     419,   419,   420,   421,   421,   422,   423,   424,   424,   425,
     425,   426,   426,   427,   427,   427,   427,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   429,   429,   429,   429,   430,   430,   430,   430,   430,
     431,   431,   432,   433,   433,   434,   435,   435,   435,   436,
     436,   437,   438,   438,   439,   440,   441,   442,   442,   443,
     444,   444,   444,   445,   446,   447,   447,   447,   448,   448,
     449,   449,   449,   449,   449,   449,   450,   450,   450,   451,
     451,   452,   452,   453,   453,   454,   454,   455,   455,   456,
     457,   457,   457,   457,   457,   457,   458,   459,   460,   461,
     462,   462,   463,   464,   465,   466,   466,   466,   466,   466,
     466,   466,   466,   466,   466,   466,   466,   466,   467,   467,
     467,   468,   469,   469,   469,   469,   470,   470,   470,   471,
     472,   472,   472,   472,   472,   472,   472,   472,   473,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   487,   487,   488,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   497,   498,   498,   498,
     499,   499,   499,   501,   500,   502,   503,   503,   503,   503,
     503,   503,   503,   503,   503,   503,   503,   503,   503,   504,
     505,   505,   506,   506,   507,   508,   508,   509,   510,   510,
     511,   513,   512,   514,   514,   515,   515,   516,   516,   517,
     518,   518,   518,   518,   518,   518,   519,   519,   519,   519,
     519,   520,   521,   521,   521,   522,   522,   523,   524,   524,
     524,   524,   525,   525,   525,   526,   526,   526,   526,   527,
     527,   527,   527,   528,   528,   529,   529,   530,   531,   531,
     531,   531,   532,   532,   533,   533,   533,   534,   534,   535,
     535,   536,   536,   536,   536,   536,   537,   537,   537,   538,
     538,   538,   538,   538,   538,   538,   538,   538,   538,   538,
     539,   539,   540,   541,   542,   543,   543,   543,   543,   543,
     543,   543,   543,   543,   543,   544,   544,   544,   544,   544,
     544,   544,   544,   544,   544,   544,   544,   544,   544,   544,
     544,   544,   544,   544,   544,   544,   544,   544,   544,   544,
     544,   544,   544,   544,   544,   544,   544,   544,   544,   544,
     544,   544,   545,   545,   545,   545,   545,   545,   546,   547,
     547,   547,   548,   548,   549,   549,   550,   550,   550,   550,
     550,   550,   550,   550,   550,   550,   550,   550,   550,   550,
     550,   550,   551,   552,   552,   552,   552,   553,   553,   554,
     555,   556,   556,   556,   556,   556,   556,   557,   557,   558,
     558,   559,   560,   561,   561,   561,   562,   562,   563,   563,
     564,   565,   566,   567,   567,   567,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   568,   568,   568,   568,   568,   568,
     568,   568,   568,   568,   569,   570,   570,   571,   571,   571,
     571,   571,   571,   571,   571,   571,   572,   573,   574,   574,
     575,   575,   575,   575,   575,   575,   575,   575,   575,   576,
     575,   577,   575,   575,   575,   575,   578,   578,   579,   580,
     580,   581,   581,   581,   582,   582,   583,   583,   584,   584,
     584,   584,   584,   584,   584,   584,   584,   584,   584,   585,
     586,   586,   586,   586,   586,   586,   586,   586,   586,   586,
     586,   586,   586,   586,   586,   586,   586,   586,   586,   586,
     586,   586,   586,   586,   586,   586,   586,   586,   586,   586,
     586,   586,   586,   586,   586,   586,   586,   586,   586,   587,
     587,   588,   589,   590,   590,   590,   591,   591,   592,   592,
     593,   594,   595,   595,   597,   596,   598,   598,   599,   599,
     600,   601,   601,   602,   603,   604,   605,   606,   607,   608,
     609,   610,   610,   610,   610,   610,   610,   610,   610,   610,
     611,   612,   612,   612,   612,   612,   613,   614,   614,   614,
     614,   614,   614,   614,   614,   614,   614,   614,   614,   614,
     615,   615,   615,   616,   616,   616,   616,   616,   617,   617,
     617,   617,   617,   617,   617,   618,   618,   619,   619,   620,
     621,   622,   623,   623,   623,   623,   623,   624,   625,   625,
     626,   626,   627,   627,   627,   627,   627,   627,   627,   627,
     627,   628,   629,   630,   630,   630,   630,   631,   631,   631,
     631,   631,   632,   632,   632,   632,   632,   632,   632,   633,
     633,   634,   634,   635,   636,   637,   637,   637,   637,   637,
     637,   637,   637,   637,   637,   637,   637,   637,   637,   637,
     637,   638,   638,   638,   638,   638,   638,   638,   638,   638,
     638,   638,   638,   638,   638,   638,   638,   638,   638,   638,
     638,   638,   638,   638,   638,   638,   638,   638,   638,   638,
     638,   638,   638,   638,   638,   638,   638,   638,   638,   638,
     638,   638,   638,   638,   638,   638,   638,   638,   639,   639,
     639,   639,   639,   639,   639,   639,   639,   639,   640,   640,
     640,   640,   641,   641,   641,   642,   642,   642,   642,   642,
     642,   642,   642,   642,   642,   642,   642,   642,   642,   642,
     642,   642,   642,   642,   642,   642,   642,   642
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     0,     2,     5,     1,     3,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     0,     1,
       1,     0,     2,     2,     1,     1,     2,     2,     1,     1,
       1,     1,     0,     4,     2,     0,     1,     2,     1,     1,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     1,     1,     1,     1,     2,     4,
       3,     3,     3,     3,     3,     3,     4,     3,     3,     4,
       5,     3,     3,     4,     2,     0,     1,     0,     1,     2,
       2,     2,     3,     2,     3,     3,     2,     3,     3,     3,
       4,     5,     1,     1,     1,     4,     0,     2,     2,     2,
       1,     1,     5,     0,     1,     3,     3,     0,     8,     1,
       4,     1,     1,     0,     1,     2,     3,     0,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     4,     2,     3,
       2,     2,     1,     1,     1,     0,     2,     2,     2,     2,
       2,     1,     1,     3,     3,     0,     1,     1,     2,     0,
       1,     3,     1,     3,     1,     0,     2,     2,     2,     2,
       3,     3,     3,     0,     3,     1,     1,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       3,     1,     1,     4,     1,     0,     3,     4,     1,     2,
       0,     5,     5,     5,     5,     4,     3,     3,     4,     5,
       1,     0,     3,     4,     4,     0,     2,     3,     3,     3,
       2,     2,     2,     2,     3,     3,     2,     2,     1,     1,
       1,     4,     0,     3,     3,     3,     0,     1,     1,     4,
       0,     2,     2,     2,     2,     2,     2,     2,     0,     2,
       9,     9,     4,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     1,     1,     3,     0,     1,     3,     3,
       3,     3,     4,     4,     4,     4,     2,     1,     1,     1,
       1,     0,     1,     0,     4,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       0,     3,     1,     3,     1,     1,     1,     2,     3,     2,
       4,     0,     5,     0,     1,     1,     2,     1,     3,     1,
       1,     1,     1,     1,     3,     1,     1,     3,     1,     1,
       1,     1,     1,     3,     1,     5,    11,     1,     1,     1,
       2,     1,     7,     9,     7,     1,     1,     1,     3,     1,
       1,     1,     3,     1,     3,     1,     3,     3,     1,     2,
       2,     6,     1,     3,     0,     4,     3,     1,     3,     3,
       3,     2,     2,     2,     2,     0,     2,     2,     0,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     1,     1,
       5,     7,     0,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       4,     1,     1,     1,     1,     2,     2,     3,     3,     6,
       3,     3,     3,     3,     5,     6,     6,     6,     6,     4,
       8,     4,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     3,     1,     3,     0,     1,     4,     6,     4,     3,
       5,     5,     4,     6,     3,     2,     5,     5,     8,     8,
       8,     8,     2,     1,     2,     2,     2,     0,     3,     5,
       0,     2,     4,     4,     4,     2,     2,     0,     3,     0,
       3,     3,     2,     0,     2,     2,     0,     2,     0,     2,
       4,     5,     2,     0,     3,     3,     3,     3,     5,     6,
       4,     6,     4,     6,     4,     4,     5,     6,     3,     3,
       4,     5,     5,     5,     5,     5,     4,     4,     5,     5,
       5,     5,     5,     5,     5,     5,     6,     6,     6,     5,
       6,     8,     6,     6,     5,     4,     5,     4,     5,     4,
       5,     4,     5,     5,     5,     5,     6,     5,     5,     6,
       5,     5,     5,     4,     2,     3,     3,     1,     2,     3,
       2,     4,     3,     2,     3,     1,     2,     4,     0,     3,
       2,     1,     2,     3,     3,     3,     3,     2,     3,     0,
       5,     0,     5,     2,     2,     1,     4,     1,     1,     0,
       3,     2,     2,     2,     5,     2,     0,     3,     0,     2,
       2,     3,     2,     2,     3,     2,     2,     2,     1,     3,
       2,     2,     2,     2,     2,     2,     3,     2,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     2,     3,
       2,     3,     3,     4,     4,     2,     2,     2,     3,     3,
       3,     2,     3,     3,     3,     4,     2,     2,     1,     0,
       2,     3,     1,     0,     1,     2,     0,     2,     0,     1,
       3,     0,     0,     2,     0,     3,     0,     3,     0,     3,
       3,     1,     0,     4,     6,     6,     6,     7,     6,     7,
       5,     0,     2,     3,     2,     3,     3,     2,     3,     2,
       2,     3,     3,     5,     4,     4,     2,     3,     3,     4,
       4,     4,     5,     5,     4,     3,     4,     4,     4,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     2,
       2,     2,     3,     3,     4,     0,     3,     0,     3,     0,
       3,     2,     3,     3,     5,     5,     3,     2,     3,     3,
       1,     2,     4,     3,     3,     3,     2,     2,     2,     2,
       2,     4,     2,     3,     3,     4,     4,     1,     1,     1,
       1,     1,     0,     2,     2,     2,     3,     3,     4,     0,
       3,     0,     3,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     1,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
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
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
            /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
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
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
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
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
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
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 731 "cf-parse.y" /* yacc.c:1646  */
    { return 0; }
#line 3662 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 732 "cf-parse.y" /* yacc.c:1646  */
    { return 0; }
#line 3668 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 745 "cf-parse.y" /* yacc.c:1646  */
    {
     struct f_val *val = cfg_alloc(sizeof(struct f_val));
     *val = f_eval((yyvsp[-1].x), cfg_mem);
     if (val->type == T_RETURN) cf_error("Runtime error");
     cf_define_symbol((yyvsp[-3].s), SYM_CONSTANT | val->type, val);
   }
#line 3679 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 755 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = f_eval_int((yyvsp[-1].x)); }
#line 3685 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 756 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != (SYM_CONSTANT | T_INT)) cf_error("Number expected");
     (yyval.i) = SYM_VAL((yyvsp[0].s)).i; }
#line 3693 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 763 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = (u32) (yyvsp[-1].i) * 1000000; }
#line 3699 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 764 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = (u32) (yyvsp[-1].i) * 1000; }
#line 3705 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 765 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = (u32) (yyvsp[-1].i) * 1; }
#line 3711 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 773 "cf-parse.y" /* yacc.c:1646  */
    {(yyval.i) = !!(yyvsp[0].i); }
#line 3717 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 774 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 3723 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 775 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 3729 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 776 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 3735 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 777 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 3741 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 778 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 3747 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 785 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != (SYM_CONSTANT | T_IP)) cf_error("IP address expected");
     (yyval.a) = SYM_VAL((yyvsp[0].s)).px.ip;
   }
#line 3756 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 792 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.iface) = NULL; }
#line 3762 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 793 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.iface) = if_get_by_name((yyvsp[0].s)->name); }
#line 3768 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 797 "cf-parse.y" /* yacc.c:1646  */
    {
     if (!ip_is_prefix((yyvsp[-1].a), (yyvsp[0].i))) cf_error("Invalid prefix");
     (yyval.px).addr = (yyvsp[-1].a); (yyval.px).len = (yyvsp[0].i);
   }
#line 3777 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 805 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.px).addr = (yyvsp[0].a); (yyval.px).len = BITS_PER_IP_ADDRESS; }
#line 3783 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 809 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].i) < 0 || (yyvsp[0].i) > BITS_PER_IP_ADDRESS) cf_error("Invalid prefix length %d", (yyvsp[0].i));
     (yyval.i) = (yyvsp[0].i);
   }
#line 3792 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 813 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.i) = ipa_masklen((yyvsp[0].a));
     if ((yyval.i) < 0) cf_error("Invalid netmask %I", (yyvsp[0].a));
   }
#line 3801 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 820 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.time) = tm_parse_datetime((yyvsp[0].t));
     if (!(yyval.time))
       cf_error("Invalid date and time");
   }
#line 3811 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 829 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != (SYM_CONSTANT | T_STRING)) cf_error("String expected");
     (yyval.t) = SYM_VAL((yyvsp[0].s)).s;
   }
#line 3820 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 836 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = (yyvsp[0].t); }
#line 3826 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 837 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = NULL; }
#line 3832 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 843 "cf-parse.y" /* yacc.c:1646  */
    {
    struct log_config *c = cfg_allocz(sizeof(struct log_config));
    c->fh = (yyvsp[-2].g);
    c->mask = (yyvsp[-1].i);
    add_tail(&new_config->logfiles, &c->n);
  }
#line 3843 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 852 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = (yyvsp[0].t); }
#line 3849 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 853 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = bird_name; }
#line 3855 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 857 "cf-parse.y" /* yacc.c:1646  */
    {
     FILE *f = tracked_fopen(new_config->pool, (yyvsp[0].t), "a");
     if (!f) cf_error("Unable to open log file `%s': %m", (yyvsp[0].t));
     (yyval.g) = f;
   }
#line 3865 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 862 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.g) = NULL; new_config->syslog_name = (yyvsp[0].t); }
#line 3871 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 863 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.g) = stderr; }
#line 3877 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 867 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ~0; }
#line 3883 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 868 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 3889 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 872 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1 << (yyvsp[0].i); }
#line 3895 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 873 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (1 << (yyvsp[0].i)); }
#line 3901 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 877 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_DEBUG[0]; }
#line 3907 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 878 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_TRACE[0]; }
#line 3913 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 879 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_INFO[0]; }
#line 3919 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 880 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_REMOTE[0]; }
#line 3925 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 881 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_WARN[0]; }
#line 3931 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 882 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_ERR[0]; }
#line 3937 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 883 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_AUTH[0]; }
#line 3943 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 884 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_FATAL[0]; }
#line 3949 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 885 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = L_BUG[0]; }
#line 3955 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 891 "cf-parse.y" /* yacc.c:1646  */
    { new_config->proto_default_mrtdump = (yyvsp[-1].i); }
#line 3961 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 892 "cf-parse.y" /* yacc.c:1646  */
    {
     FILE *f = tracked_fopen(new_config->pool, (yyvsp[-1].t), "a");
     if (!f) cf_error("Unable to open MRTDump file '%s': %m", (yyvsp[-1].t));
     new_config->mrtdump_file = fileno(f);
   }
#line 3971 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 902 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.tf) = &new_config->tf_route; }
#line 3977 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 903 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.tf) = &new_config->tf_proto; }
#line 3983 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 904 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.tf) = &new_config->tf_base; }
#line 3989 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 905 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.tf) = &new_config->tf_log; }
#line 3995 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 908 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-1].tf) = (struct timeformat){(yyvsp[0].t), NULL, 0}; }
#line 4001 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 909 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-3].tf) = (struct timeformat){(yyvsp[-2].t), (yyvsp[0].t), (yyvsp[-1].i)}; }
#line 4007 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 910 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-2].tf) = (struct timeformat){"%T", "%F", 20*3600}; }
#line 4013 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 911 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-2].tf) = (struct timeformat){"%F %T", NULL, 0}; }
#line 4019 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 912 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-2].tf) = (struct timeformat){NULL, NULL, 0}; }
#line 4025 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 913 "cf-parse.y" /* yacc.c:1646  */
    { *(yyvsp[-2].tf) = (struct timeformat){"%d-%m-%Y %T", NULL, 0}; }
#line 4031 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 923 "cf-parse.y" /* yacc.c:1646  */
    { new_config->latency_debug = (yyvsp[0].i); }
#line 4037 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 924 "cf-parse.y" /* yacc.c:1646  */
    { new_config->latency_limit = (yyvsp[0].i32); }
#line 4043 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 925 "cf-parse.y" /* yacc.c:1646  */
    { new_config->watchdog_warning = (yyvsp[0].i32); }
#line 4049 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 926 "cf-parse.y" /* yacc.c:1646  */
    { new_config->watchdog_timeout = ((yyvsp[0].i32) + 999999) TO_S; }
#line 4055 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 935 "cf-parse.y" /* yacc.c:1646  */
    { cmd_reconfig((yyvsp[-2].t), RECONFIG_HARD, (yyvsp[-1].i)); }
#line 4061 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 938 "cf-parse.y" /* yacc.c:1646  */
    { cmd_reconfig((yyvsp[-2].t), RECONFIG_SOFT, (yyvsp[-1].i)); }
#line 4067 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 945 "cf-parse.y" /* yacc.c:1646  */
    { cmd_reconfig_confirm(); }
#line 4073 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 948 "cf-parse.y" /* yacc.c:1646  */
    { cmd_reconfig_undo(); }
#line 4079 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 951 "cf-parse.y" /* yacc.c:1646  */
    { cmd_check_config((yyvsp[-1].t)); }
#line 4085 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 954 "cf-parse.y" /* yacc.c:1646  */
    { cmd_shutdown(); }
#line 4091 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 957 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = NULL; }
#line 4097 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 962 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 4103 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 963 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = UNIX_DEFAULT_CONFIGURE_TIMEOUT; }
#line 4109 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 964 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 4115 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 972 "cf-parse.y" /* yacc.c:1646  */
    { this_proto = krt_init_config((yyvsp[-1].i)); }
#line 4121 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 977 "cf-parse.y" /* yacc.c:1646  */
    { THIS_KRT->persist = (yyvsp[0].i); }
#line 4127 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 978 "cf-parse.y" /* yacc.c:1646  */
    {
      /* Scan time of 0 means scan on startup only */
      THIS_KRT->scan_time = (yyvsp[0].i);
   }
#line 4136 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 982 "cf-parse.y" /* yacc.c:1646  */
    {
      THIS_KRT->learn = (yyvsp[0].i);
#ifndef KRT_ALLOW_LEARN
      if ((yyvsp[0].i))
	cf_error("Learning of kernel routes not supported in this configuration");
#endif
   }
#line 4148 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 989 "cf-parse.y" /* yacc.c:1646  */
    { THIS_KRT->devroutes = (yyvsp[0].i); }
#line 4154 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 990 "cf-parse.y" /* yacc.c:1646  */
    { THIS_KRT->graceful_restart = (yyvsp[0].i); }
#line 4160 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 996 "cf-parse.y" /* yacc.c:1646  */
    { this_proto = kif_init_config((yyvsp[-1].i)); }
#line 4166 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1001 "cf-parse.y" /* yacc.c:1646  */
    {
      /* Scan time of 0 means scan on startup only */
      THIS_KIF->scan_time = (yyvsp[0].i);
   }
#line 4175 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1005 "cf-parse.y" /* yacc.c:1646  */
    {
     struct kif_primary_item *kpi = cfg_alloc(sizeof (struct kif_primary_item));
     kpi->pattern = (yyvsp[-1].t);
     kpi->prefix = (yyvsp[0].px).addr;
     kpi->pxlen = (yyvsp[0].px).len;
     add_tail(&THIS_KIF->primary, &kpi->n);
   }
#line 4187 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1019 "cf-parse.y" /* yacc.c:1646  */
    {
	if ((yyvsp[0].i) <= 0 || (yyvsp[0].i) >= NL_NUM_TABLES)
	  cf_error("Kernel routing table number out of range");
	THIS_KRT->sys.table_id = (yyvsp[0].i);
   }
#line 4197 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1033 "cf-parse.y" /* yacc.c:1646  */
    { new_config->router_id = (yyvsp[-1].i32); }
#line 4203 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1034 "cf-parse.y" /* yacc.c:1646  */
    { new_config->router_id_from = this_ipatt; }
#line 4209 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1038 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = (yyvsp[0].i); }
#line 4215 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1040 "cf-parse.y" /* yacc.c:1646  */
    {
#ifndef IPV6
     (yyval.i32) = ipa_to_u32((yyvsp[0].a));
#else
     cf_error("Router IDs must be entered as hexadecimal numbers or IPv4 addresses in IPv6 version");
#endif
   }
#line 4227 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1059 "cf-parse.y" /* yacc.c:1646  */
    { new_config->listen_bgp_addr = (yyvsp[0].a); }
#line 4233 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1060 "cf-parse.y" /* yacc.c:1646  */
    { new_config->listen_bgp_port = (yyvsp[0].i); }
#line 4239 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1061 "cf-parse.y" /* yacc.c:1646  */
    { new_config->listen_bgp_flags = 0; }
#line 4245 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1062 "cf-parse.y" /* yacc.c:1646  */
    { new_config->listen_bgp_flags = 1; }
#line 4251 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1067 "cf-parse.y" /* yacc.c:1646  */
    { new_config->gr_wait = (yyvsp[-1].i); }
#line 4257 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1073 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 4263 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1074 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 4269 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1078 "cf-parse.y" /* yacc.c:1646  */
    {
   struct rtable_config *cf;
   cf = rt_new_table((yyvsp[-1].s));
   cf->sorted = (yyvsp[0].i);
   }
#line 4279 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1086 "cf-parse.y" /* yacc.c:1646  */
    {
  this_roa_table = roa_new_table_config((yyvsp[0].s));
}
#line 4287 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1092 "cf-parse.y" /* yacc.c:1646  */
    {
     roa_add_item_config(this_roa_table, (yyvsp[-5].px).addr, (yyvsp[-5].px).len, (yyvsp[-3].i), (yyvsp[-1].i));
   }
#line 4295 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1106 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = SYM_PROTO; }
#line 4301 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1107 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = SYM_TEMPLATE; }
#line 4307 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1111 "cf-parse.y" /* yacc.c:1646  */
    {
     struct symbol *s = cf_default_name(this_proto->protocol->template, &this_proto->protocol->name_counter);
     s->class = this_proto->class;
     s->def = this_proto;
     this_proto->name = s->name;
     }
#line 4318 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1117 "cf-parse.y" /* yacc.c:1646  */
    {
     cf_define_symbol((yyvsp[0].s), this_proto->class, this_proto);
     this_proto->name = (yyvsp[0].s)->name;
   }
#line 4327 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1121 "cf-parse.y" /* yacc.c:1646  */
    {
     struct symbol *s = cf_default_name(this_proto->protocol->template, &this_proto->protocol->name_counter);
     this_proto->name = s->name;
     if (((yyvsp[0].s)->class != SYM_TEMPLATE) && ((yyvsp[0].s)->class != SYM_PROTO)) cf_error("Template or protocol name expected");
     proto_copy_config(this_proto, (yyvsp[0].s)->def);
   }
#line 4338 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1127 "cf-parse.y" /* yacc.c:1646  */
    {
     if (((yyvsp[0].s)->class != SYM_TEMPLATE) && ((yyvsp[0].s)->class != SYM_PROTO)) cf_error("Template or protocol name expected");

     cf_define_symbol((yyvsp[-2].s), this_proto->class, this_proto);
     this_proto->name = (yyvsp[-2].s)->name;

     proto_copy_config(this_proto, (yyvsp[0].s)->def);
   }
#line 4351 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1139 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].i) < 0 || (yyvsp[0].i) > 0xFFFF) cf_error("Invalid preference");
     this_proto->preference = (yyvsp[0].i);
   }
#line 4360 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1143 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->disabled = (yyvsp[0].i); }
#line 4366 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1144 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->debug = (yyvsp[0].i); }
#line 4372 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1145 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->mrtdump = (yyvsp[0].i); }
#line 4378 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1146 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->in_filter = (yyvsp[0].f); }
#line 4384 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1147 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->out_filter = (yyvsp[0].f); }
#line 4390 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1148 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->rx_limit = (yyvsp[0].g); }
#line 4396 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1149 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->in_limit = (yyvsp[0].g); }
#line 4402 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1150 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->out_limit = (yyvsp[0].g); }
#line 4408 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1151 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->in_keep_filtered = (yyvsp[0].i); }
#line 4414 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1152 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->table = (yyvsp[0].r); }
#line 4420 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1153 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->router_id = (yyvsp[0].i32); }
#line 4426 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1154 "cf-parse.y" /* yacc.c:1646  */
    { this_proto->dsc = (yyvsp[0].t); }
#line 4432 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1158 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.f) = (yyvsp[0].f); }
#line 4438 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 1160 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.f) = FILTER_ACCEPT; }
#line 4444 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 1161 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.f) = FILTER_REJECT; }
#line 4450 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 1165 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = PLA_DISABLE; }
#line 4456 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 1166 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = PLA_WARN; }
#line 4462 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1167 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = PLA_BLOCK; }
#line 4468 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 1168 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = PLA_RESTART; }
#line 4474 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 1169 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = PLA_DISABLE; }
#line 4480 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 1173 "cf-parse.y" /* yacc.c:1646  */
    {
     struct proto_limit *l = cfg_allocz(sizeof(struct proto_limit));
     l->limit = (yyvsp[-1].i);
     l->action = (yyvsp[0].i);
     (yyval.g) = l;
   }
#line 4491 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 1179 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.g) = NULL; }
#line 4497 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 1183 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != SYM_TABLE) cf_error("Table name expected");
     (yyval.r) = (yyvsp[0].s)->def;
   }
#line 4506 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 1191 "cf-parse.y" /* yacc.c:1646  */
    { new_config->proto_default_debug = (yyvsp[0].i); }
#line 4512 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 1192 "cf-parse.y" /* yacc.c:1646  */
    { new_config->cli_debug = (yyvsp[0].i); }
#line 4518 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 1200 "cf-parse.y" /* yacc.c:1646  */
    {
     struct iface_patt_node *ipn = cfg_allocz(sizeof(struct iface_patt_node));
     add_tail(&this_ipatt->ipn_list, NODE ipn);
     this_ipn = ipn;
   }
#line 4528 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 1208 "cf-parse.y" /* yacc.c:1646  */
    { this_ipn->pattern = (yyvsp[0].t); this_ipn->prefix = IPA_NONE; this_ipn->pxlen = 0; }
#line 4534 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 1209 "cf-parse.y" /* yacc.c:1646  */
    { this_ipn->pattern = NULL; this_ipn->prefix = (yyvsp[0].px).addr; this_ipn->pxlen = (yyvsp[0].px).len; }
#line 4540 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 1210 "cf-parse.y" /* yacc.c:1646  */
    { this_ipn->pattern = (yyvsp[-1].t); this_ipn->prefix = (yyvsp[0].px).addr; this_ipn->pxlen = (yyvsp[0].px).len; }
#line 4546 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 1214 "cf-parse.y" /* yacc.c:1646  */
    { this_ipn->positive = 1; }
#line 4552 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 1215 "cf-parse.y" /* yacc.c:1646  */
    { this_ipn->positive = 0; }
#line 4558 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 1229 "cf-parse.y" /* yacc.c:1646  */
    { iface_patt_check(); }
#line 4564 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 1231 "cf-parse.y" /* yacc.c:1646  */
    {
   /* Generic this_ipatt init */
   this_ipatt = cfg_allocz(sizeof(struct iface_patt));
   init_list(&this_ipatt->ipn_list);
 }
#line 4574 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 1243 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i) & 0xfc;        if (((yyvsp[0].i) < 0) || ((yyvsp[0].i) > 255)) cf_error("TX class must be in range 0-255"); }
#line 4580 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 1244 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ((yyvsp[0].i) & 0x3f) << 2; if (((yyvsp[0].i) < 0) || ((yyvsp[0].i) > 63))  cf_error("TX DSCP must be in range 0-63"); }
#line 4586 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 1250 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_device, (yyvsp[-1].i));
     init_list(&DIRECT_CFG->iface_list);
   }
#line 4595 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 1263 "cf-parse.y" /* yacc.c:1646  */
    {
     this_ipatt = cfg_allocz(sizeof(struct iface_patt));
     add_tail(&DIRECT_CFG->iface_list, NODE this_ipatt);
     init_list(&this_ipatt->ipn_list);
   }
#line 4605 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 1277 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ~0; }
#line 4611 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 1278 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 4617 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 1279 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 4623 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 1284 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 4629 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 1288 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_STATES; }
#line 4635 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 1289 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_ROUTES; }
#line 4641 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 1290 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_FILTERS; }
#line 4647 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 1291 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_IFACES; }
#line 4653 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 1292 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_EVENTS; }
#line 4659 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 1293 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = D_PACKETS; }
#line 4665 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 1299 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ~0; }
#line 4671 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 1300 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 4677 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 1301 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 4683 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 1306 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-2].i) | (yyvsp[0].i); }
#line 4689 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 1310 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = MD_STATES; }
#line 4695 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 1311 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = MD_MESSAGES; }
#line 4701 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 1332 "cf-parse.y" /* yacc.c:1646  */
    {
     if (!this_p_list) {
     	this_p_list = cfg_alloc(sizeof(list));
     	init_list(this_p_list);
        password_id = 1;
     }
     this_p_item = cfg_alloc(sizeof (struct password_item));
     this_p_item->password = (yyvsp[0].t);
     this_p_item->genfrom = 0;
     this_p_item->gento = TIME_INFINITY;
     this_p_item->accfrom = 0;
     this_p_item->accto = TIME_INFINITY;
     this_p_item->id = password_id++;
     add_tail(this_p_list, &this_p_item->n);
   }
#line 4721 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 1350 "cf-parse.y" /* yacc.c:1646  */
    { }
#line 4727 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 1351 "cf-parse.y" /* yacc.c:1646  */
    { this_p_item->genfrom = (yyvsp[-2].time); }
#line 4733 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 1352 "cf-parse.y" /* yacc.c:1646  */
    { this_p_item->gento = (yyvsp[-2].time); }
#line 4739 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 1353 "cf-parse.y" /* yacc.c:1646  */
    { this_p_item->accfrom = (yyvsp[-2].time); }
#line 4745 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 1354 "cf-parse.y" /* yacc.c:1646  */
    { this_p_item->accto = (yyvsp[-2].time); }
#line 4751 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 1355 "cf-parse.y" /* yacc.c:1646  */
    { this_p_item->id = (yyvsp[-2].i); if ((yyvsp[-2].i) <= 0) cf_error("Password ID has to be greated than zero."); }
#line 4757 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 1364 "cf-parse.y" /* yacc.c:1646  */
    { cmd_show_status(); }
#line 4763 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 1367 "cf-parse.y" /* yacc.c:1646  */
    { cmd_show_memory(); }
#line 4769 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 1370 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_show, 0, 0); }
#line 4775 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 1373 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_show, 0, 1); }
#line 4781 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 1377 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.s) = NULL; }
#line 4787 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 1381 "cf-parse.y" /* yacc.c:1646  */
    { if_show(); }
#line 4793 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 1384 "cf-parse.y" /* yacc.c:1646  */
    { if_show_summary(); }
#line 4799 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 1388 "cf-parse.y" /* yacc.c:1646  */
    { rt_show((yyvsp[-1].ra)); }
#line 4805 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 1391 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = cfg_allocz(sizeof(struct rt_show_data));
     (yyval.ra)->pxlen = 256;
     (yyval.ra)->filter = FILTER_ACCEPT;
   }
#line 4815 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 1396 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     if ((yyval.ra)->pxlen != 256) cf_error("Only one prefix expected");
     (yyval.ra)->prefix = (yyvsp[0].px).addr;
     (yyval.ra)->pxlen = (yyvsp[0].px).len;
   }
#line 4826 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 1402 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-2].ra);
     if ((yyval.ra)->pxlen != 256) cf_error("Only one prefix expected");
     (yyval.ra)->prefix = (yyvsp[0].px).addr;
     (yyval.ra)->pxlen = (yyvsp[0].px).len;
     (yyval.ra)->show_for = 1;
   }
#line 4838 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 1409 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-2].ra);
     if ((yyvsp[0].s)->class != SYM_TABLE) cf_error("%s is not a table", (yyvsp[0].s)->name);
     (yyval.ra)->table = ((struct rtable_config *)(yyvsp[0].s)->def)->table;
   }
#line 4848 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 209:
#line 1414 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-2].ra);
     if ((yyval.ra)->filter != FILTER_ACCEPT) cf_error("Filter specified twice");
     (yyval.ra)->filter = (yyvsp[0].f);
   }
#line 4858 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 1419 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     if ((yyval.ra)->filter != FILTER_ACCEPT) cf_error("Filter specified twice");
     (yyval.ra)->filter = (yyvsp[0].f);
   }
#line 4868 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 1424 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     (yyval.ra)->verbose = 1;
   }
#line 4877 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 1428 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     (yyval.ra)->primary_only = 1;
   }
#line 4886 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 213:
#line 1432 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     (yyval.ra)->filtered = 1;
   }
#line 4895 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 214:
#line 1436 "cf-parse.y" /* yacc.c:1646  */
    {
     struct proto_config *c = (struct proto_config *) (yyvsp[0].s)->def;
     (yyval.ra) = (yyvsp[-2].ra);
     if ((yyval.ra)->export_mode) cf_error("Protocol specified twice");
     if ((yyvsp[0].s)->class != SYM_PROTO || !c->proto) cf_error("%s is not a protocol", (yyvsp[0].s)->name);
     (yyval.ra)->export_mode = (yyvsp[-1].i);
     (yyval.ra)->export_protocol = c->proto;
     (yyval.ra)->running_on_config = c->proto->cf->global;
   }
#line 4909 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 215:
#line 1445 "cf-parse.y" /* yacc.c:1646  */
    {
     struct proto_config *c = (struct proto_config *) (yyvsp[0].s)->def;
     (yyval.ra) = (yyvsp[-2].ra);
     if ((yyval.ra)->show_protocol) cf_error("Protocol specified twice");
     if ((yyvsp[0].s)->class != SYM_PROTO || !c->proto) cf_error("%s is not a protocol", (yyvsp[0].s)->name);
     (yyval.ra)->show_protocol = c->proto;
     (yyval.ra)->running_on_config = c->proto->cf->global;
   }
#line 4922 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 216:
#line 1453 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     (yyval.ra)->stats = 1;
   }
#line 4931 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 1457 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ra) = (yyvsp[-1].ra);
     (yyval.ra)->stats = 2;
   }
#line 4940 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 218:
#line 1464 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = RSEM_PREEXPORT; }
#line 4946 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 219:
#line 1465 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = RSEM_EXPORT; }
#line 4952 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1466 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = RSEM_NOEXPORT; }
#line 4958 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1472 "cf-parse.y" /* yacc.c:1646  */
    { roa_show((yyvsp[-1].ro)); }
#line 4964 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 222:
#line 1475 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ro) = cfg_allocz(sizeof(struct roa_show_data));
     (yyval.ro)->mode = ROA_SHOW_ALL;
     (yyval.ro)->table = roa_table_default;
     if (roa_table_default == NULL)
       cf_error("No ROA table defined");
   }
#line 4976 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 223:
#line 1482 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ro) = (yyvsp[-2].ro);
     if ((yyval.ro)->mode != ROA_SHOW_ALL) cf_error("Only one prefix expected");
     (yyval.ro)->prefix = (yyvsp[0].px).addr;
     (yyval.ro)->pxlen = (yyvsp[0].px).len;
     (yyval.ro)->mode = (yyvsp[-1].i);
   }
#line 4988 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 224:
#line 1489 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ro) = (yyvsp[-2].ro);
     (yyval.ro)->asn = (yyvsp[0].i);
   }
#line 4997 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1493 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ro) = (yyvsp[-2].ro);
     if ((yyvsp[0].s)->class != SYM_ROA) cf_error("%s is not a ROA table", (yyvsp[0].s)->name);
     (yyval.ro)->table = ((struct roa_table_config *)(yyvsp[0].s)->def)->table;
   }
#line 5007 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1501 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ROA_SHOW_PX; }
#line 5013 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 1502 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ROA_SHOW_IN; }
#line 5019 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1503 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ROA_SHOW_FOR; }
#line 5025 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1509 "cf-parse.y" /* yacc.c:1646  */
    { cmd_show_symbols((yyvsp[-1].sd)); }
#line 5031 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1512 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.sd) = cfg_allocz(sizeof(struct sym_show_data));
   }
#line 5039 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 1515 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_TABLE; }
#line 5045 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 1516 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_FUNCTION; }
#line 5051 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 1517 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_FILTER; }
#line 5057 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1518 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_PROTO; }
#line 5063 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1519 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_TEMPLATE; }
#line 5069 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1520 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->type = SYM_ROA; }
#line 5075 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1521 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.sd) = (yyvsp[-1].sd); (yyval.sd)->sym = (yyvsp[0].s); }
#line 5081 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1526 "cf-parse.y" /* yacc.c:1646  */
    { 
     if (roa_table_default == NULL)
       cf_error("No ROA table defined");
     (yyval.rot) = roa_table_default;
   }
#line 5091 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1531 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != SYM_ROA)
       cf_error("%s is not a ROA table", (yyvsp[0].s)->name);
     (yyval.rot) = ((struct roa_table_config *)(yyvsp[0].s)->def)->table;
   }
#line 5101 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1540 "cf-parse.y" /* yacc.c:1646  */
    {
  if (! cli_access_restricted())
    { roa_add_item((yyvsp[-1].rot), (yyvsp[-6].px).addr, (yyvsp[-6].px).len, (yyvsp[-4].i), (yyvsp[-2].i), ROA_SRC_DYNAMIC); cli_msg(0, ""); }
}
#line 5110 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1547 "cf-parse.y" /* yacc.c:1646  */
    {
  if (! cli_access_restricted())
    { roa_delete_item((yyvsp[-1].rot), (yyvsp[-6].px).addr, (yyvsp[-6].px).len, (yyvsp[-4].i), (yyvsp[-2].i), ROA_SRC_DYNAMIC); cli_msg(0, ""); }
}
#line 5119 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1554 "cf-parse.y" /* yacc.c:1646  */
    {
  if (! cli_access_restricted())
    { roa_flush((yyvsp[-1].rot), ROA_SRC_DYNAMIC); cli_msg(0, ""); }
}
#line 5128 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1562 "cf-parse.y" /* yacc.c:1646  */
    { rdump(&root_pool); cli_msg(0, ""); }
#line 5134 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1564 "cf-parse.y" /* yacc.c:1646  */
    { sk_dump_all(); cli_msg(0, ""); }
#line 5140 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1566 "cf-parse.y" /* yacc.c:1646  */
    { io_log_dump(); cli_msg(0, ""); }
#line 5146 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1568 "cf-parse.y" /* yacc.c:1646  */
    { if_dump_all(); cli_msg(0, ""); }
#line 5152 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1570 "cf-parse.y" /* yacc.c:1646  */
    { neigh_dump_all(); cli_msg(0, ""); }
#line 5158 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1572 "cf-parse.y" /* yacc.c:1646  */
    { rta_dump_all(); cli_msg(0, ""); }
#line 5164 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 249:
#line 1574 "cf-parse.y" /* yacc.c:1646  */
    { rt_dump_all(); cli_msg(0, ""); }
#line 5170 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1576 "cf-parse.y" /* yacc.c:1646  */
    { protos_dump_all(); cli_msg(0, ""); }
#line 5176 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1579 "cf-parse.y" /* yacc.c:1646  */
    { cmd_eval((yyvsp[-1].x)); }
#line 5182 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1582 "cf-parse.y" /* yacc.c:1646  */
    {
  cli_set_log_echo(this_cli, (yyvsp[-2].i), (yyvsp[-1].i));
  cli_msg(0, "");
}
#line 5191 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1588 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = ~0; }
#line 5197 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1589 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 5203 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 255:
#line 1590 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 5209 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1594 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 4096; }
#line 5215 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1595 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].i) < 256 || (yyvsp[0].i) > 65536) cf_error("Invalid log buffer size");
     (yyval.i) = (yyvsp[0].i);
   }
#line 5224 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1602 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_disable, 1, 0); }
#line 5230 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1604 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_enable, 1, 0); }
#line 5236 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1606 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_restart, 1, 0); }
#line 5242 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1608 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_reload, 1, CMD_RELOAD); }
#line 5248 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1610 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_reload, 1, CMD_RELOAD_IN); }
#line 5254 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1612 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-1].ps), proto_cmd_reload, 1, CMD_RELOAD_OUT); }
#line 5260 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1616 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-2].ps), proto_cmd_debug, 1, (yyvsp[-1].i)); }
#line 5266 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1620 "cf-parse.y" /* yacc.c:1646  */
    { proto_apply_cmd((yyvsp[-2].ps), proto_cmd_mrtdump, 1, (yyvsp[-1].i)); }
#line 5272 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1623 "cf-parse.y" /* yacc.c:1646  */
    { this_cli->restricted = 1; cli_msg(16, "Access restricted"); }
#line 5278 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1626 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = (yyvsp[0].s); (yyval.ps).patt = 0; }
#line 5284 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 268:
#line 1627 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = NULL; (yyval.ps).patt = 1; }
#line 5290 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 269:
#line 1628 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = (yyvsp[0].t); (yyval.ps).patt = 1; }
#line 5296 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 270:
#line 1632 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = (yyvsp[0].s); (yyval.ps).patt = 0; }
#line 5302 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1633 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = NULL; (yyval.ps).patt = 1; }
#line 5308 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1634 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ps).ptr = (yyvsp[0].t); (yyval.ps).patt = 1; }
#line 5314 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1642 "cf-parse.y" /* yacc.c:1646  */
    { (yyvsp[0].s) = cf_define_symbol((yyvsp[0].s), SYM_FILTER, NULL); cf_push_scope( (yyvsp[0].s) ); }
#line 5320 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 274:
#line 1643 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyvsp[-2].s)->def = (yyvsp[0].f);
     (yyvsp[0].f)->name = (yyvsp[-2].s)->name;
     DBG( "We have new filter defined (%s)\n", (yyvsp[-2].s)->name );
     cf_pop_scope();
   }
#line 5331 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1652 "cf-parse.y" /* yacc.c:1646  */
    { f_eval_int((yyvsp[0].x)); }
#line 5337 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 276:
#line 1656 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_INT; }
#line 5343 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 277:
#line 1657 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_BOOL; }
#line 5349 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 278:
#line 1658 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_IP; }
#line 5355 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1659 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_PREFIX; }
#line 5361 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 280:
#line 1660 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_PAIR; }
#line 5367 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1661 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_QUAD; }
#line 5373 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 282:
#line 1662 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_EC; }
#line 5379 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 283:
#line 1663 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_STRING; }
#line 5385 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 284:
#line 1664 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_PATH_MASK; }
#line 5391 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 285:
#line 1665 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_PATH; }
#line 5397 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 286:
#line 1666 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_CLIST; }
#line 5403 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 287:
#line 1667 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = T_ECLIST; }
#line 5409 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 288:
#line 1668 "cf-parse.y" /* yacc.c:1646  */
    { 
	switch ((yyvsp[-1].i)) {
	  case T_INT:
	  case T_PAIR:
	  case T_QUAD:
	  case T_EC:
	  case T_IP:
	       (yyval.i) = T_SET;
	       break;

	  case T_PREFIX:
	       (yyval.i) = T_PREFIX_SET;
	    break;

	  default:
		cf_error( "You can't create sets of this type." );
	}
   }
#line 5432 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 289:
#line 1689 "cf-parse.y" /* yacc.c:1646  */
    {
     struct f_val * val = cfg_alloc(sizeof(struct f_val));
     val->type = T_VOID;
     (yyvsp[0].s) = cf_define_symbol((yyvsp[0].s), SYM_VARIABLE | (yyvsp[-1].i), val);
     DBG( "New variable %s type %x\n", (yyvsp[0].s)->name, (yyvsp[-1].i) );
     (yyvsp[0].s)->aux2 = NULL;
     (yyval.s)=(yyvsp[0].s);
   }
#line 5445 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 290:
#line 1700 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.s) = NULL; }
#line 5451 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 291:
#line 1701 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.s) = (yyvsp[-2].s);
     (yyval.s)->aux2 = (yyvsp[0].s);
   }
#line 5460 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 292:
#line 1708 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.s) = (yyvsp[0].s); }
#line 5466 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 293:
#line 1709 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.s) = (yyvsp[-2].s);
     (yyval.s)->aux2 = (yyvsp[0].s);
   }
#line 5475 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 294:
#line 1716 "cf-parse.y" /* yacc.c:1646  */
    {
     struct filter *f = cfg_alloc(sizeof(struct filter));
     f->name = NULL;
     f->root = (yyvsp[0].x);
     (yyval.f) = f;
   }
#line 5486 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 295:
#line 1725 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[0].s)->class != SYM_FILTER) cf_error("No such filter.");
     (yyval.f) = (yyvsp[0].s)->def;
   }
#line 5495 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 297:
#line 1733 "cf-parse.y" /* yacc.c:1646  */
    {
     /* Construct 'IF term THEN ACCEPT; REJECT;' */
     struct filter *f = cfg_alloc(sizeof(struct filter));
     struct f_inst *i, *acc, *rej;
     acc = f_new_inst();		/* ACCEPT */
     acc->code = P('p',',');
     acc->a1.p = NULL;
     acc->a2.i = F_ACCEPT;
     rej = f_new_inst();		/* REJECT */
     rej->code = P('p',',');
     rej->a1.p = NULL;
     rej->a2.i = F_REJECT;
     i = f_new_inst();			/* IF */
     i->code = '?';
     i->a1.p = (yyvsp[0].x);
     i->a2.p = acc;
     i->next = rej;
     f->name = NULL;
     f->root = i;
     (yyval.f) = f;
  }
#line 5521 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 298:
#line 1757 "cf-parse.y" /* yacc.c:1646  */
    { DBG( "Have function parameters\n" ); (yyval.s)=(yyvsp[-1].s); }
#line 5527 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 299:
#line 1758 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.s)=NULL; }
#line 5533 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 300:
#line 1762 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[-3].s)) {
       /* Prepend instruction to clear local variables */
       (yyval.x) = f_new_inst();
       (yyval.x)->code = P('c','v');
       (yyval.x)->a1.p = (yyvsp[-3].s);
       (yyval.x)->next = (yyvsp[-1].x);
     } else
       (yyval.x) = (yyvsp[-1].x);
   }
#line 5548 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 301:
#line 1775 "cf-parse.y" /* yacc.c:1646  */
    { DBG( "Beginning of function %s\n", (yyvsp[0].s)->name );
     (yyvsp[0].s) = cf_define_symbol((yyvsp[0].s), SYM_FUNCTION, NULL);
     cf_push_scope((yyvsp[0].s));
   }
#line 5557 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 302:
#line 1778 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyvsp[-3].s)->def = (yyvsp[0].x);
     (yyvsp[-3].s)->aux2 = (yyvsp[-1].s);
     DBG("Hmm, we've got one function here - %s\n", (yyvsp[-3].s)->name); 
     cf_pop_scope();
   }
#line 5568 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 303:
#line 1791 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = NULL; }
#line 5574 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 304:
#line 1792 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x)->next; (yyvsp[0].x)->next = NULL; }
#line 5580 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 305:
#line 1795 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); (yyvsp[0].x)->next = (yyvsp[0].x); }
#line 5586 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 306:
#line 1796 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); (yyvsp[0].x)->next = (yyvsp[-1].x)->next ; (yyvsp[-1].x)->next = (yyvsp[0].x); }
#line 5592 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 307:
#line 1800 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x)=(yyvsp[0].x);
   }
#line 5600 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 308:
#line 1803 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x)=(yyvsp[-1].x);
   }
#line 5608 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 309:
#line 1812 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_IP; (yyval.v).val.px.ip = (yyvsp[0].a); }
#line 5614 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 310:
#line 1825 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_INT; (yyval.v).val.i = (yyvsp[0].i); }
#line 5620 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1826 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_QUAD; (yyval.v).val.i = (yyvsp[0].i32); }
#line 5626 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1827 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 5632 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1828 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = pair_a((yyvsp[0].i)); (yyval.v).val.i = pair_b((yyvsp[0].i)); }
#line 5638 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1829 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.v) = f_eval((yyvsp[-1].x), cfg_mem);
     if (!f_valid_set_type((yyval.v).type)) cf_error("Set-incompatible type");
   }
#line 5647 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 315:
#line 1833 "cf-parse.y" /* yacc.c:1646  */
    {
     if (!cf_symbol_is_constant((yyvsp[0].s))) cf_error("%s: constant expected", (yyvsp[0].s)->name);
     if (!f_valid_set_type(SYM_TYPE((yyvsp[0].s)))) cf_error("%s: set-incompatible type", (yyvsp[0].s)->name);
     (yyval.v) = *(struct f_val *)((yyvsp[0].s)->def);
   }
#line 5657 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 316:
#line 1841 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_INT; (yyval.v).val.i = (yyvsp[0].i); }
#line 5663 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 317:
#line 1842 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_INT; (yyval.v).val.i = f_eval_int((yyvsp[-1].x)); }
#line 5669 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1843 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = T_QUAD; (yyval.v).val.i = (yyvsp[0].i32); }
#line 5675 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1844 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 5681 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1845 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v).type = pair_a((yyvsp[0].i)); (yyval.v).val.i = pair_b((yyvsp[0].i)); }
#line 5687 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1849 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = f_eval_int((yyvsp[0].x)); check_u16((yyval.i)); }
#line 5693 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 322:
#line 1852 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = pair((yyvsp[0].i), (yyvsp[0].i)); }
#line 5699 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 323:
#line 1853 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = pair((yyvsp[-2].i), (yyvsp[0].i)); }
#line 5705 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 324:
#line 1854 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = 0xFFFF; }
#line 5711 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1858 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.e) = f_new_pair_set(pair_a((yyvsp[-3].i32)), pair_b((yyvsp[-3].i32)), pair_a((yyvsp[-1].i32)), pair_b((yyvsp[-1].i32)));
   }
#line 5719 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1861 "cf-parse.y" /* yacc.c:1646  */
    {
     /* Hack: $2 and $4 should be pair_expr, but that would cause shift/reduce conflict */
     if ((pair_a((yyvsp[-9].i32)) != pair_b((yyvsp[-9].i32))) || (pair_a((yyvsp[-7].i32)) != pair_b((yyvsp[-7].i32))))
       cf_error("syntax error");
     (yyval.e) = f_new_pair_item(pair_b((yyvsp[-9].i32)), (yyvsp[-3].i), pair_b((yyvsp[-7].i32)), (yyvsp[-1].i)); 
   }
#line 5730 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1870 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i32) = f_eval_int((yyvsp[0].x)); }
#line 5736 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1873 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = EC_RT; }
#line 5742 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1874 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = EC_RO; }
#line 5748 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1875 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 5754 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1876 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = EC_GENERIC; }
#line 5760 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1880 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_ec_item((yyvsp[-5].i), 0, (yyvsp[-3].i32), (yyvsp[-1].i32), (yyvsp[-1].i32)); }
#line 5766 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 333:
#line 1881 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_ec_item((yyvsp[-7].i), 0, (yyvsp[-5].i32), (yyvsp[-3].i32), (yyvsp[-1].i32)); }
#line 5772 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 334:
#line 1882 "cf-parse.y" /* yacc.c:1646  */
    {  (yyval.e) = f_new_ec_item((yyvsp[-5].i), 0, (yyvsp[-3].i32), 0, EC_ALL); }
#line 5778 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 337:
#line 1888 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_item((yyvsp[0].v), (yyvsp[0].v)); }
#line 5784 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 338:
#line 1889 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_item((yyvsp[-2].v), (yyvsp[0].v)); }
#line 5790 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 341:
#line 1895 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_item((yyvsp[0].v), (yyvsp[0].v)); }
#line 5796 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 342:
#line 1896 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_new_item((yyvsp[-2].v), (yyvsp[0].v)); }
#line 5802 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 344:
#line 1901 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_merge_items((yyvsp[-2].e), (yyvsp[0].e)); }
#line 5808 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 346:
#line 1906 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = f_merge_items((yyvsp[-2].e), (yyvsp[0].e)); }
#line 5814 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1910 "cf-parse.y" /* yacc.c:1646  */
    {
     if (((yyvsp[0].i) < 0) || ((yyvsp[0].i) > MAX_PREFIX_LENGTH) || !ip_is_prefix((yyvsp[-2].a), (yyvsp[0].i))) cf_error("Invalid network prefix: %I/%d.", (yyvsp[-2].a), (yyvsp[0].i));
     (yyval.v).type = T_PREFIX; (yyval.v).val.px.ip = (yyvsp[-2].a); (yyval.v).val.px.len = (yyvsp[0].i);
   }
#line 5823 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1917 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[0].v); }
#line 5829 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 349:
#line 1918 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[-1].v); (yyval.v).val.px.len |= LEN_PLUS; }
#line 5835 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 350:
#line 1919 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.v) = (yyvsp[-1].v); (yyval.v).val.px.len |= LEN_MINUS; }
#line 5841 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1920 "cf-parse.y" /* yacc.c:1646  */
    { 
     if (! ((0 <= (yyvsp[-3].i)) && ((yyvsp[-3].i) <= (yyvsp[-1].i)) && ((yyvsp[-1].i) <= MAX_PREFIX_LENGTH))) cf_error("Invalid prefix pattern range: {%d, %d}.", (yyvsp[-3].i), (yyvsp[-1].i));
     (yyval.v) = (yyvsp[-5].v); (yyval.v).val.px.len |= LEN_RANGE | ((yyvsp[-3].i) << 16) | ((yyvsp[-1].i) << 8);
   }
#line 5850 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1927 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.trie) = f_new_trie(cfg_mem, sizeof(struct f_trie_node)); trie_add_fprefix((yyval.trie), &((yyvsp[0].v).val.px)); }
#line 5856 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 353:
#line 1928 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.trie) = (yyvsp[-2].trie); trie_add_fprefix((yyval.trie), &((yyvsp[0].v).val.px)); }
#line 5862 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 354:
#line 1931 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.e) = NULL; }
#line 5868 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 355:
#line 1932 "cf-parse.y" /* yacc.c:1646  */
    {
     /* Fill data fields */
     struct f_tree *t;
     for (t = (yyvsp[-2].e); t; t = t->left)
       t->data = (yyvsp[0].x);
     (yyval.e) = f_merge_items((yyvsp[-3].e), (yyvsp[-2].e));
   }
#line 5880 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1939 "cf-parse.y" /* yacc.c:1646  */
    { 
     struct f_tree *t = f_new_tree();
     t->from.type = t->to.type = T_VOID;
     t->right = t;
     t->data = (yyvsp[0].x);
     (yyval.e) = f_merge_items((yyvsp[-2].e), t);
 }
#line 5892 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 357:
#line 1951 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 5898 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 358:
#line 1952 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[-1].x); }
#line 5904 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 359:
#line 1956 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = (yyvsp[-1].h); }
#line 5910 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1957 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = (yyvsp[-1].h); }
#line 5916 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1961 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_ASN;      (yyval.h)->val = (yyvsp[-1].i); }
#line 5922 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1962 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_ASTERISK; (yyval.h)->val  = 0; }
#line 5928 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 363:
#line 1963 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_QUESTION; (yyval.h)->val  = 0; }
#line 5934 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1964 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_ASN_EXPR; (yyval.h)->val = (uintptr_t) (yyvsp[-1].x); }
#line 5940 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1965 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = NULL; }
#line 5946 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1969 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_ASN;      (yyval.h)->val = (yyvsp[-1].i); }
#line 5952 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 367:
#line 1970 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = cfg_alloc(sizeof(struct f_path_mask)); (yyval.h)->next = (yyvsp[0].h); (yyval.h)->kind = PM_ASTERISK; (yyval.h)->val  = 0; }
#line 5958 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 368:
#line 1971 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.h) = NULL; }
#line 5964 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 369:
#line 1975 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_INT;  (yyval.x)->a2.i = (yyvsp[0].i); }
#line 5970 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 370:
#line 1976 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_BOOL; (yyval.x)->a2.i = 1;  }
#line 5976 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 371:
#line 1977 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_BOOL; (yyval.x)->a2.i = 0;  }
#line 5982 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 372:
#line 1978 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_STRING; (yyval.x)->a2.p = (yyvsp[0].t); }
#line 5988 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 373:
#line 1979 "cf-parse.y" /* yacc.c:1646  */
    { NEW_F_VAL; (yyval.x) = f_new_inst(); (yyval.x)->code = 'C'; (yyval.x)->a1.p = val; *val = (yyvsp[0].v); }
#line 5994 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 374:
#line 1980 "cf-parse.y" /* yacc.c:1646  */
    {NEW_F_VAL; (yyval.x) = f_new_inst(); (yyval.x)->code = 'C'; (yyval.x)->a1.p = val; *val = (yyvsp[0].v); }
#line 6000 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 375:
#line 1981 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_QUAD;  (yyval.x)->a2.i = (yyvsp[0].i32); }
#line 6006 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 376:
#line 1982 "cf-parse.y" /* yacc.c:1646  */
    { DBG( "We've got a set here..." ); (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_SET; (yyval.x)->a2.p = build_tree((yyvsp[-1].e)); DBG( "ook\n" ); }
#line 6012 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 377:
#line 1983 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = T_PREFIX_SET;  (yyval.x)->a2.p = (yyvsp[-1].trie); }
#line 6018 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 378:
#line 1984 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'c'; (yyval.x)->aux = (yyvsp[0].i) >> 16; (yyval.x)->a2.i = (yyvsp[0].i) & 0xffff; }
#line 6024 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 379:
#line 1985 "cf-parse.y" /* yacc.c:1646  */
    { NEW_F_VAL; (yyval.x) = f_new_inst(); (yyval.x)->code = 'C'; val->type = T_PATH_MASK; val->val.path_mask = (yyvsp[0].h); (yyval.x)->a1.p = val; }
#line 6030 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 380:
#line 1989 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_dpair((yyvsp[-3].x), (yyvsp[-1].x)); }
#line 6036 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 381:
#line 1990 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_ec((yyvsp[-5].i), (yyvsp[-3].x), (yyvsp[-1].x)); }
#line 6042 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 383:
#line 2004 "cf-parse.y" /* yacc.c:1646  */
    {
     struct symbol *sym;
     struct f_inst *inst = (yyvsp[-1].x);
     if ((yyvsp[-3].s)->class != SYM_FUNCTION)
       cf_error("You can't call something which is not a function. Really.");
     DBG("You are calling function %s\n", (yyvsp[-3].s)->name);
     (yyval.x) = f_new_inst();
     (yyval.x)->code = P('c','a');
     (yyval.x)->a1.p = inst;
     (yyval.x)->a2.p = (yyvsp[-3].s)->def;
     sym = (yyvsp[-3].s)->aux2;
     while (sym || inst) {
       if (!sym || !inst)
	 cf_error("Wrong number of arguments for function %s.", (yyvsp[-3].s)->name);
       DBG( "You should pass parameter called %s\n", sym->name);
       inst->a1.p = sym;
       sym = sym->aux2;
       inst = inst->next;
     }
   }
#line 6067 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 384:
#line 2027 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();

     switch ((yyvsp[0].s)->class & 0xff00) {
       case SYM_CONSTANT: (yyval.x)->code = 'C'; break;
       case SYM_VARIABLE: (yyval.x)->code = 'V'; break;
       default: cf_error("%s: variable expected.", (yyvsp[0].s)->name);
     }

     (yyval.x)->a1.p = (yyvsp[0].s)->def;
     (yyval.x)->a2.p = (yyvsp[0].s)->name;
   }
#line 6084 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 385:
#line 2041 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_IP;         (yyval.x)->a2.i = SA_FROM;	(yyval.x)->a1.i = 1; }
#line 6090 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 386:
#line 2042 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_IP;         (yyval.x)->a2.i = SA_GW;	(yyval.x)->a1.i = 1; }
#line 6096 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 387:
#line 2043 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_PREFIX;     (yyval.x)->a2.i = SA_NET; }
#line 6102 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 388:
#line 2044 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_STRING;     (yyval.x)->a2.i = SA_PROTO; }
#line 6108 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 389:
#line 2045 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_ENUM_RTS;   (yyval.x)->a2.i = SA_SOURCE; }
#line 6114 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 390:
#line 2046 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_ENUM_SCOPE; (yyval.x)->a2.i = SA_SCOPE;	(yyval.x)->a1.i = 1; }
#line 6120 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 391:
#line 2047 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_ENUM_RTC;   (yyval.x)->a2.i = SA_CAST; }
#line 6126 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 392:
#line 2048 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_ENUM_RTD;   (yyval.x)->a2.i = SA_DEST;	(yyval.x)->a1.i = 1; }
#line 6132 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 393:
#line 2049 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_STRING;     (yyval.x)->a2.i = SA_IFNAME; }
#line 6138 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 394:
#line 2050 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->aux = T_INT;        (yyval.x)->a2.i = SA_IFINDEX; }
#line 6144 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 395:
#line 2054 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[-1].x); }
#line 6150 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 396:
#line 2055 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '+';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6156 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 397:
#line 2056 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '-';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6162 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 398:
#line 2057 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '*';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6168 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 399:
#line 2058 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '/';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6174 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 400:
#line 2059 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '&';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6180 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 401:
#line 2060 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '|';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6186 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 402:
#line 2061 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('=','='); (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6192 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 403:
#line 2062 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('!','=');     (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6198 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 404:
#line 2063 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '<';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6204 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 405:
#line 2064 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('<','=');     (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6210 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 406:
#line 2065 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '<';        (yyval.x)->a1.p = (yyvsp[0].x); (yyval.x)->a2.p = (yyvsp[-2].x); }
#line 6216 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 407:
#line 2066 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('<','=');     (yyval.x)->a1.p = (yyvsp[0].x); (yyval.x)->a2.p = (yyvsp[-2].x); }
#line 6222 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 408:
#line 2067 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '~';        (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->a2.p = (yyvsp[0].x); }
#line 6228 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 409:
#line 2068 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = '!'; (yyval.x)->a1.p = (yyvsp[0].x); }
#line 6234 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 410:
#line 2069 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('d','e');  (yyval.x)->a1.p = (yyvsp[-1].x); }
#line 6240 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 411:
#line 2071 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6246 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 412:
#line 2072 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6252 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 413:
#line 2073 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6258 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 414:
#line 2075 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'P'; }
#line 6264 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 415:
#line 2077 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); (yyval.x)->code = 'a'; }
#line 6270 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 416:
#line 2079 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); (yyval.x)->code = P('e','a'); }
#line 6276 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 417:
#line 2081 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('c','p'); (yyval.x)->a1.p = (yyvsp[-2].x); (yyval.x)->aux = T_IP; }
#line 6282 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 418:
#line 2082 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'L'; (yyval.x)->a1.p = (yyvsp[-2].x); }
#line 6288 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 419:
#line 2083 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('i','M'); (yyval.x)->a1.p = (yyvsp[-5].x); (yyval.x)->a2.p = (yyvsp[-1].x); }
#line 6294 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 420:
#line 2084 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('a','f'); (yyval.x)->a1.p = (yyvsp[-2].x); }
#line 6300 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 421:
#line 2085 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('a','l'); (yyval.x)->a1.p = (yyvsp[-2].x); }
#line 6306 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 422:
#line 2095 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'E'; (yyval.x)->aux = T_PATH; }
#line 6312 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 423:
#line 2096 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'E'; (yyval.x)->aux = T_CLIST; }
#line 6318 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 424:
#line 2097 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'E'; (yyval.x)->aux = T_ECLIST; }
#line 6324 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 425:
#line 2098 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('A','p'); (yyval.x)->a1.p = (yyvsp[-3].x); (yyval.x)->a2.p = (yyvsp[-1].x); }
#line 6330 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 426:
#line 2099 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('C','a'); (yyval.x)->a1.p = (yyvsp[-3].x); (yyval.x)->a2.p = (yyvsp[-1].x); (yyval.x)->aux = 'a'; }
#line 6336 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 427:
#line 2100 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('C','a'); (yyval.x)->a1.p = (yyvsp[-3].x); (yyval.x)->a2.p = (yyvsp[-1].x); (yyval.x)->aux = 'd'; }
#line 6342 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 428:
#line 2101 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('C','a'); (yyval.x)->a1.p = (yyvsp[-3].x); (yyval.x)->a2.p = (yyvsp[-1].x); (yyval.x)->aux = 'f'; }
#line 6348 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 429:
#line 2103 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_roa_check((yyvsp[-1].s), NULL, NULL); }
#line 6354 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 430:
#line 2104 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_roa_check((yyvsp[-5].s), (yyvsp[-3].x), (yyvsp[-1].x)); }
#line 6360 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 431:
#line 2109 "cf-parse.y" /* yacc.c:1646  */
    {
     struct symbol *sym;
     struct f_inst *inst = (yyvsp[-1].x);
     if ((yyvsp[-3].s)->class != SYM_FUNCTION)
       cf_error("You can't call something which is not a function. Really.");
     DBG("You are calling function %s\n", (yyvsp[-3].s)->name);
     (yyval.x) = f_new_inst();
     (yyval.x)->code = P('c','a');
     (yyval.x)->a1.p = inst;
     (yyval.x)->a2.p = (yyvsp[-3].s)->def;
     sym = (yyvsp[-3].s)->aux2;
     while (sym || inst) {
       if (!sym || !inst)
	 cf_error("Wrong number of arguments for function %s.", (yyvsp[-3].s)->name);
       DBG( "You should pass parameter called %s\n", sym->name);
       inst->a1.p = sym;
       sym = sym->aux2;
       inst = inst->next;
     }
   }
#line 6385 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 432:
#line 2132 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_QUITBIRD; }
#line 6391 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 433:
#line 2133 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_ACCEPT; }
#line 6397 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 434:
#line 2134 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_REJECT; }
#line 6403 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 435:
#line 2135 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_ERROR; }
#line 6409 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 436:
#line 2136 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_NOP; }
#line 6415 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 437:
#line 2137 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = F_NONL; }
#line 6421 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 438:
#line 2141 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = 'p'; (yyval.x)->a1.p = (yyvsp[0].x); (yyval.x)->a2.p = NULL; }
#line 6427 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 439:
#line 2144 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = NULL; }
#line 6433 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 440:
#line 2145 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6439 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 441:
#line 2146 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[-2].x)) {
       (yyvsp[-2].x)->next = (yyvsp[0].x);
       (yyval.x) = (yyvsp[-2].x);
     } else (yyval.x) = (yyvsp[0].x);
   }
#line 6450 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 442:
#line 2154 "cf-parse.y" /* yacc.c:1646  */
    { 
     (yyval.x) = f_new_inst();
     (yyval.x)->code = 's';
     (yyval.x)->a1.p = NULL;
     (yyval.x)->a2.p = (yyvsp[0].x);
     (yyval.x)->next = NULL;
   }
#line 6462 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 443:
#line 2161 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();
     (yyval.x)->code = 's';
     (yyval.x)->a1.p = NULL;
     (yyval.x)->a2.p = (yyvsp[-2].x);
     (yyval.x)->next = (yyvsp[0].x);
   }
#line 6474 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 444:
#line 2170 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = NULL; }
#line 6480 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 445:
#line 2171 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[0].x); }
#line 6486 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 446:
#line 2175 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();
     (yyval.x)->code = '?';
     (yyval.x)->a1.p = (yyvsp[-2].x);
     (yyval.x)->a2.p = (yyvsp[0].x);
   }
#line 6497 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 447:
#line 2181 "cf-parse.y" /* yacc.c:1646  */
    {
     struct f_inst *i = f_new_inst();
     i->code = '?';
     i->a1.p = (yyvsp[-4].x);
     i->a2.p = (yyvsp[-2].x);
     (yyval.x) = f_new_inst();
     (yyval.x)->code = '?';
     (yyval.x)->a1.p = i;
     (yyval.x)->a2.p = (yyvsp[0].x);
   }
#line 6512 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 448:
#line 2191 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();
     DBG( "Ook, we'll set value\n" );
     if (((yyvsp[-3].s)->class & ~T_MASK) != SYM_VARIABLE)
       cf_error( "You may set only variables." );
     (yyval.x)->code = 's';
     (yyval.x)->a1.p = (yyvsp[-3].s);
     (yyval.x)->a2.p = (yyvsp[-1].x);
   }
#line 6526 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 449:
#line 2200 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();
     DBG( "Ook, we'll return the value\n" );
     (yyval.x)->code = 'r';
     (yyval.x)->a1.p = (yyvsp[-1].x);
   }
#line 6537 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 450:
#line 2206 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = (yyvsp[-3].x);
     (yyval.x)->code = P('e','S');
     (yyval.x)->a1.p = (yyvsp[-1].x);
   }
#line 6547 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 451:
#line 2211 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = (yyvsp[-3].x);
     if (!(yyval.x)->a1.i)
       cf_error( "This static attribute is read-only.");
     (yyval.x)->code = P('a','S');
     (yyval.x)->a1.p = (yyvsp[-1].x);
   }
#line 6559 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 452:
#line 2218 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = f_new_inst();
     (yyval.x)->code = P('P','S');
     (yyval.x)->a1.p = (yyvsp[-1].x);
   }
#line 6569 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 453:
#line 2223 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.x) = (yyvsp[-2].x);
     (yyval.x)->aux = EAF_TYPE_UNDEF | EAF_TEMP;
     (yyval.x)->code = P('e','S');
     (yyval.x)->a1.p = NULL;
   }
#line 6580 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 454:
#line 2229 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_inst(); (yyval.x)->code = P('p',','); (yyval.x)->a1.p = (yyvsp[-1].x); (yyval.x)->a2.i = (yyvsp[-2].i); }
#line 6586 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 455:
#line 2230 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = (yyvsp[-1].x); }
#line 6592 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 456:
#line 2231 "cf-parse.y" /* yacc.c:1646  */
    {
      (yyval.x) = f_new_inst();
      (yyval.x)->code = P('S','W');
      (yyval.x)->a1.p = (yyvsp[-3].x);
      (yyval.x)->a2.p = build_tree( (yyvsp[-1].e) );
   }
#line 6603 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 457:
#line 2239 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_empty((yyvsp[-3].x)); }
#line 6609 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 458:
#line 2240 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_complex( P('A','p'), 'x', (yyvsp[-6].x), (yyvsp[-2].x) ); }
#line 6615 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 459:
#line 2241 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_complex( P('C','a'), 'a', (yyvsp[-6].x), (yyvsp[-2].x) ); }
#line 6621 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 460:
#line 2242 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_complex( P('C','a'), 'd', (yyvsp[-6].x), (yyvsp[-2].x) ); }
#line 6627 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 461:
#line 2243 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_generate_complex( P('C','a'), 'f', (yyvsp[-6].x), (yyvsp[-2].x) ); }
#line 6633 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 462:
#line 2250 "cf-parse.y" /* yacc.c:1646  */
    {
  this_proto = proto_config_new(&proto_bfd, (yyvsp[-1].i));
  init_list(&BFD_CFG->patt_list);
  init_list(&BFD_CFG->neigh_list);

  if (bfd_cf)
    cf_error("Only one BFD instance allowed");
  bfd_cf = BFD_CFG;
}
#line 6647 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 470:
#line 2277 "cf-parse.y" /* yacc.c:1646  */
    {
  this_ipatt = cfg_allocz(sizeof(struct bfd_iface_config));
  init_list(&this_ipatt->ipn_list);

  BFD_IFACE->min_rx_int = BFD_DEFAULT_MIN_RX_INT;
  BFD_IFACE->min_tx_int = BFD_DEFAULT_MIN_TX_INT;
  BFD_IFACE->idle_tx_int = BFD_DEFAULT_IDLE_TX_INT;
  BFD_IFACE->multiplier = BFD_DEFAULT_MULTIPLIER;
}
#line 6661 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 471:
#line 2288 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->min_rx_int = BFD_IFACE->min_tx_int = (yyvsp[0].i32); }
#line 6667 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 472:
#line 2289 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->min_rx_int = (yyvsp[0].i32); }
#line 6673 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 473:
#line 2290 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->min_tx_int = (yyvsp[0].i32); }
#line 6679 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 474:
#line 2291 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->idle_tx_int = (yyvsp[0].i32); }
#line 6685 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 475:
#line 2292 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->multiplier = (yyvsp[0].i); }
#line 6691 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 476:
#line 2293 "cf-parse.y" /* yacc.c:1646  */
    { BFD_IFACE->passive = (yyvsp[0].i); }
#line 6697 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 481:
#line 2307 "cf-parse.y" /* yacc.c:1646  */
    { add_tail(&BFD_CFG->patt_list, NODE this_ipatt); }
#line 6703 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 482:
#line 2310 "cf-parse.y" /* yacc.c:1646  */
    { BFD_CFG->multihop = BFD_IFACE; }
#line 6709 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 483:
#line 2314 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.iface) = NULL; }
#line 6715 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 484:
#line 2315 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.iface) = if_get_by_name((yyvsp[0].s)->name); }
#line 6721 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 485:
#line 2316 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.iface) = if_get_by_name((yyvsp[0].t)); }
#line 6727 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 486:
#line 2320 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.a) = IPA_NONE; }
#line 6733 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 487:
#line 2321 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.a) = (yyvsp[0].a); }
#line 6739 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 488:
#line 2325 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 6745 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 489:
#line 2326 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 6751 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 490:
#line 2330 "cf-parse.y" /* yacc.c:1646  */
    {
  this_bfd_neighbor = cfg_allocz(sizeof(struct bfd_neighbor));
  add_tail(&BFD_CFG->neigh_list, NODE this_bfd_neighbor);

  BFD_NEIGHBOR->addr = (yyvsp[-3].a);
  BFD_NEIGHBOR->local = (yyvsp[-1].a);
  BFD_NEIGHBOR->iface = (yyvsp[-2].iface);
  BFD_NEIGHBOR->multihop = (yyvsp[0].i);

  if ((yyvsp[0].i) && (yyvsp[-2].iface))
    cf_error("Neighbor cannot set both interface and multihop");

  if ((yyvsp[0].i) && ipa_zero((yyvsp[-1].a)))
    cf_error("Multihop neighbor requires specified local address");
}
#line 6771 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 491:
#line 2349 "cf-parse.y" /* yacc.c:1646  */
    { bfd_show_sessions(proto_get_named((yyvsp[-1].s), &proto_bfd)); }
#line 6777 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 492:
#line 2354 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_bgp, (yyvsp[-1].i));
     BGP_CFG->remote_port = BGP_PORT;
     BGP_CFG->multihop = -1;	/* undefined */
     BGP_CFG->hold_time = 240;
     BGP_CFG->initial_hold_time = 240;
     BGP_CFG->compare_path_lengths = 1;
     BGP_CFG->igp_metric = 1;
     BGP_CFG->connect_delay_time = 5;
     BGP_CFG->connect_retry_time = 120;
     BGP_CFG->error_amnesia_time = 300;
     BGP_CFG->error_delay_time_min = 60;
     BGP_CFG->error_delay_time_max = 300;
     BGP_CFG->enable_refresh = 1;
     BGP_CFG->enable_as4 = 1;
     BGP_CFG->capabilities = 2;
     BGP_CFG->advertise_ipv4 = 1;
     BGP_CFG->interpret_communities = 1;
     BGP_CFG->default_local_pref = 100;
     BGP_CFG->gr_mode = BGP_GR_AWARE;
     BGP_CFG->gr_time = 120;
 }
#line 6804 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 494:
#line 2380 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->remote_port = (yyvsp[0].i); if (((yyvsp[0].i)<1) || ((yyvsp[0].i)>65535)) cf_error("Invalid port number");  }
#line 6810 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 495:
#line 2381 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->remote_as = (yyvsp[0].i); }
#line 6816 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 498:
#line 2387 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->local_as = (yyvsp[-1].i); }
#line 6822 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 499:
#line 2388 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->source_addr = (yyvsp[-3].a); BGP_CFG->local_as = (yyvsp[-1].i); }
#line 6828 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 501:
#line 2390 "cf-parse.y" /* yacc.c:1646  */
    {
     if (ipa_nonzero(BGP_CFG->remote_ip))
       cf_error("Only one neighbor per BGP instance is allowed");
     BGP_CFG->remote_ip = (yyvsp[-3].a);
     if ((yyvsp[-2].iface)) BGP_CFG->iface = (yyvsp[-2].iface);
   }
#line 6839 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 502:
#line 2396 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->iface = if_get_by_name((yyvsp[-1].t)); }
#line 6845 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 503:
#line 2397 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->rr_cluster_id = (yyvsp[-1].i32); }
#line 6851 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 504:
#line 2398 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->rr_client = 1; }
#line 6857 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 505:
#line 2399 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->rs_client = 1; }
#line 6863 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 506:
#line 2400 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->hold_time = (yyvsp[-1].i); }
#line 6869 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 507:
#line 2401 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->initial_hold_time = (yyvsp[-1].i); }
#line 6875 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 508:
#line 2402 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->multihop = 0; }
#line 6881 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 509:
#line 2403 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->multihop = 64; }
#line 6887 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 510:
#line 2404 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->multihop = (yyvsp[-1].i); if (((yyvsp[-1].i)<1) || ((yyvsp[-1].i)>255)) cf_error("Multihop must be in range 1-255"); }
#line 6893 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 511:
#line 2405 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->next_hop_self = 1; BGP_CFG->next_hop_keep = 0; }
#line 6899 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 512:
#line 2406 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->next_hop_keep = 1; BGP_CFG->next_hop_self = 0; }
#line 6905 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 513:
#line 2407 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->missing_lladdr = MLL_SELF; }
#line 6911 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 514:
#line 2408 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->missing_lladdr = MLL_DROP; }
#line 6917 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 515:
#line 2409 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->missing_lladdr = MLL_IGNORE; }
#line 6923 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 516:
#line 2410 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->gw_mode = GW_DIRECT; }
#line 6929 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 517:
#line 2411 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->gw_mode = GW_RECURSIVE; }
#line 6935 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 518:
#line 2412 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->compare_path_lengths = (yyvsp[-1].i); }
#line 6941 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 519:
#line 2413 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->med_metric = (yyvsp[-1].i); }
#line 6947 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 520:
#line 2414 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->igp_metric = (yyvsp[-1].i); }
#line 6953 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 521:
#line 2415 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->prefer_older = (yyvsp[-1].i); }
#line 6959 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 522:
#line 2416 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->deterministic_med = (yyvsp[-1].i); }
#line 6965 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 523:
#line 2417 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->default_med = (yyvsp[-1].i); }
#line 6971 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 524:
#line 2418 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->default_local_pref = (yyvsp[-1].i); }
#line 6977 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 525:
#line 2419 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->source_addr = (yyvsp[-1].a); }
#line 6983 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 526:
#line 2420 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->connect_delay_time = (yyvsp[-1].i); log(L_WARN "%s: Start delay time option is deprecated, use connect delay time", this_proto->name); }
#line 6989 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 527:
#line 2421 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->connect_delay_time = (yyvsp[-1].i); }
#line 6995 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 528:
#line 2422 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->connect_retry_time = (yyvsp[-1].i); }
#line 7001 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 529:
#line 2423 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->keepalive_time = (yyvsp[-1].i); }
#line 7007 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 530:
#line 2424 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->error_amnesia_time = (yyvsp[-1].i); }
#line 7013 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 531:
#line 2425 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->error_delay_time_min = (yyvsp[-3].i); BGP_CFG->error_delay_time_max = (yyvsp[-1].i); }
#line 7019 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 532:
#line 2426 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->disable_after_error = (yyvsp[-1].i); }
#line 7025 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 533:
#line 2427 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->enable_refresh = (yyvsp[-1].i); }
#line 7031 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 534:
#line 2428 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->enable_as4 = (yyvsp[-1].i); }
#line 7037 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 535:
#line 2429 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->capabilities = (yyvsp[-1].i); }
#line 7043 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 536:
#line 2430 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->advertise_ipv4 = (yyvsp[-1].i); }
#line 7049 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 537:
#line 2431 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->password = (yyvsp[-1].t); }
#line 7055 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 538:
#line 2432 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto->in_limit = cfg_allocz(sizeof(struct proto_limit));
     this_proto->in_limit->limit = (yyvsp[-1].i);
     this_proto->in_limit->action = PLA_RESTART;
     log(L_WARN "%s: Route limit option is deprecated, use import limit", this_proto->name);
   }
#line 7066 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 539:
#line 2438 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->passive = (yyvsp[-1].i); }
#line 7072 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 540:
#line 2439 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->interpret_communities = (yyvsp[-1].i); }
#line 7078 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 541:
#line 2440 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->secondary = (yyvsp[-1].i); }
#line 7084 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 542:
#line 2441 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->add_path = ADD_PATH_RX; }
#line 7090 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 543:
#line 2442 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->add_path = ADD_PATH_TX; }
#line 7096 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 544:
#line 2443 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->add_path = (yyvsp[-1].i) ? ADD_PATH_FULL : 0; }
#line 7102 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 545:
#line 2444 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->allow_local_as = -1; }
#line 7108 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 546:
#line 2445 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->allow_local_as = (yyvsp[-1].i); }
#line 7114 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 547:
#line 2446 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->gr_mode = (yyvsp[-1].i); }
#line 7120 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 548:
#line 2447 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->gr_mode = BGP_GR_AWARE; }
#line 7126 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 549:
#line 2448 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->gr_time = (yyvsp[-1].i); }
#line 7132 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 550:
#line 2449 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->igp_table = (yyvsp[-1].r); }
#line 7138 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 551:
#line 2450 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->ttl_security = (yyvsp[-1].i); }
#line 7144 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 552:
#line 2451 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->check_link = (yyvsp[-1].i); }
#line 7150 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 553:
#line 2452 "cf-parse.y" /* yacc.c:1646  */
    { BGP_CFG->bfd = (yyvsp[-1].i); cf_check_bfd((yyvsp[-1].i)); }
#line 7156 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 554:
#line 2463 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_ospf, (yyvsp[-1].i));
     init_list(&OSPF_CFG->area_list);
     init_list(&OSPF_CFG->vlink_list);
     OSPF_CFG->tick = OSPF_DEFAULT_TICK;
     OSPF_CFG->ospf2 = OSPF_IS_V2;
  }
#line 7168 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 558:
#line 2479 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->rfc1583 = (yyvsp[0].i); }
#line 7174 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 559:
#line 2480 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->stub_router = (yyvsp[0].i); }
#line 7180 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 560:
#line 2481 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->ecmp = (yyvsp[0].i) ? OSPF_DEFAULT_ECMP_LIMIT : 0; }
#line 7186 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 561:
#line 2482 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->ecmp = (yyvsp[-2].i) ? (yyvsp[0].i) : 0; if ((yyvsp[0].i) < 0) cf_error("ECMP limit cannot be negative"); }
#line 7192 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 562:
#line 2483 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->merge_external = (yyvsp[0].i); }
#line 7198 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 563:
#line 2484 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->tick = (yyvsp[0].i); if((yyvsp[0].i)<=0) cf_error("Tick must be greater than zero"); }
#line 7204 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 564:
#line 2485 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_CFG->instance_id = (yyvsp[0].i); if (((yyvsp[0].i)<0) || ((yyvsp[0].i)>255)) cf_error("Instance ID must be in range 0-255"); }
#line 7210 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 566:
#line 2489 "cf-parse.y" /* yacc.c:1646  */
    {
  this_area = cfg_allocz(sizeof(struct ospf_area_config));
  add_tail(&OSPF_CFG->area_list, NODE this_area);
  this_area->areaid = (yyvsp[0].i32);
  this_area->default_cost = OSPF_DEFAULT_STUB_COST;
  this_area->type = OPT_E;
  this_area->transint = OSPF_DEFAULT_TRANSINT;

  init_list(&this_area->patt_list);
  init_list(&this_area->net_list);
  init_list(&this_area->enet_list);
  init_list(&this_area->stubnet_list);
 }
#line 7228 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 567:
#line 2504 "cf-parse.y" /* yacc.c:1646  */
    { ospf_area_finish(); }
#line 7234 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 570:
#line 2513 "cf-parse.y" /* yacc.c:1646  */
    { this_area->type = (yyvsp[0].i) ? 0 : OPT_E; /* We should remove the option */ }
#line 7240 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 571:
#line 2514 "cf-parse.y" /* yacc.c:1646  */
    { this_area->type = OPT_N; }
#line 7246 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 572:
#line 2515 "cf-parse.y" /* yacc.c:1646  */
    { this_area->summary = (yyvsp[0].i); }
#line 7252 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 573:
#line 2516 "cf-parse.y" /* yacc.c:1646  */
    { this_area->default_nssa = (yyvsp[0].i); }
#line 7258 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 574:
#line 2517 "cf-parse.y" /* yacc.c:1646  */
    { this_area->default_cost = (yyvsp[0].i); ospf_check_defcost((yyvsp[0].i)); }
#line 7264 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 575:
#line 2518 "cf-parse.y" /* yacc.c:1646  */
    { this_area->default_cost = (yyvsp[0].i) | LSA_EXT3_EBIT; ospf_check_defcost((yyvsp[0].i)); }
#line 7270 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 576:
#line 2519 "cf-parse.y" /* yacc.c:1646  */
    { this_area->default_cost = (yyvsp[0].i); ospf_check_defcost((yyvsp[0].i)); }
#line 7276 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 577:
#line 2520 "cf-parse.y" /* yacc.c:1646  */
    { this_area->translator = (yyvsp[0].i); }
#line 7282 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 578:
#line 2521 "cf-parse.y" /* yacc.c:1646  */
    { this_area->transint = (yyvsp[0].i); }
#line 7288 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 579:
#line 2522 "cf-parse.y" /* yacc.c:1646  */
    { this_nets = &this_area->net_list; }
#line 7294 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 581:
#line 2523 "cf-parse.y" /* yacc.c:1646  */
    { this_nets = &this_area->enet_list; }
#line 7300 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 588:
#line 2535 "cf-parse.y" /* yacc.c:1646  */
    {
     this_stubnet = cfg_allocz(sizeof(struct ospf_stubnet_config));
     add_tail(&this_area->stubnet_list, NODE this_stubnet);
     this_stubnet->px = (yyvsp[0].px);
     this_stubnet->cost = COST_D;
   }
#line 7311 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 591:
#line 2549 "cf-parse.y" /* yacc.c:1646  */
    { this_stubnet->hidden = (yyvsp[0].i); }
#line 7317 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 592:
#line 2550 "cf-parse.y" /* yacc.c:1646  */
    { this_stubnet->summary = (yyvsp[0].i); }
#line 7323 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 593:
#line 2551 "cf-parse.y" /* yacc.c:1646  */
    { this_stubnet->cost = (yyvsp[0].i); }
#line 7329 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 594:
#line 2555 "cf-parse.y" /* yacc.c:1646  */
    { ospf_iface_finish(); }
#line 7335 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 595:
#line 2556 "cf-parse.y" /* yacc.c:1646  */
    { ospf_iface_finish(); }
#line 7341 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 599:
#line 2565 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->helloint = (yyvsp[0].i) ; if (((yyvsp[0].i)<=0) || ((yyvsp[0].i)>65535)) cf_error("Hello interval must be in range 1-65535"); }
#line 7347 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 600:
#line 2566 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->rxmtint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Retransmit int must be greater than one"); }
#line 7353 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 601:
#line 2567 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->inftransdelay = (yyvsp[0].i) ; if (((yyvsp[0].i)<=0) || ((yyvsp[0].i)>65535)) cf_error("Transmit delay must be in range 1-65535"); }
#line 7359 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 602:
#line 2568 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->waitint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Wait interval must be greater than one"); }
#line 7365 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 603:
#line 2569 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->deadint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Dead interval must be greater than one"); }
#line 7371 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 604:
#line 2570 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->deadc = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Dead count must be greater than one"); }
#line 7377 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 605:
#line 2571 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_NONE;  }
#line 7383 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 606:
#line 2572 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_SIMPLE; ospf_check_auth(); }
#line 7389 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 607:
#line 2573 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_CRYPT; ospf_check_auth(); }
#line 7395 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 608:
#line 2574 "cf-parse.y" /* yacc.c:1646  */
    { ospf_check_auth(); }
#line 7401 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 609:
#line 2578 "cf-parse.y" /* yacc.c:1646  */
    {
  if (this_area->areaid == 0) cf_error("Virtual link cannot be in backbone");
  this_ipatt = cfg_allocz(sizeof(struct ospf_iface_patt));
  add_tail(&OSPF_CFG->vlink_list, NODE this_ipatt);
  init_list(&this_ipatt->ipn_list);
  OSPF_PATT->voa = this_area->areaid;
  OSPF_PATT->vid = (yyvsp[0].i32);
  OSPF_PATT->helloint = HELLOINT_D;
  OSPF_PATT->rxmtint = RXMTINT_D;
  OSPF_PATT->inftransdelay = INFTRANSDELAY_D;
  OSPF_PATT->deadc = DEADC_D;
  OSPF_PATT->type = OSPF_IT_VLINK;
  OSPF_PATT->instance_id = OSPF_CFG->instance_id;
  init_list(&OSPF_PATT->nbma_list);
  reset_passwords();
 }
#line 7422 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 610:
#line 2597 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->cost = (yyvsp[0].i) ; if (((yyvsp[0].i)<=0) || ((yyvsp[0].i)>65535)) cf_error("Cost must be in range 1-65535"); }
#line 7428 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 611:
#line 2598 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->helloint = (yyvsp[0].i) ; if (((yyvsp[0].i)<=0) || ((yyvsp[0].i)>65535)) cf_error("Hello interval must be in range 1-65535"); }
#line 7434 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 612:
#line 2599 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->pollint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=0) cf_error("Poll int must be greater than zero"); }
#line 7440 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 613:
#line 2600 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->rxmtint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Retransmit int must be greater than one"); }
#line 7446 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 614:
#line 2601 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->waitint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Wait interval must be greater than one"); }
#line 7452 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 615:
#line 2602 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->deadint = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Dead interval must be greater than one"); }
#line 7458 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 616:
#line 2603 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->deadc = (yyvsp[0].i) ; if ((yyvsp[0].i)<=1) cf_error("Dead count must be greater than one"); }
#line 7464 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 617:
#line 2604 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_BCAST ; }
#line 7470 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 618:
#line 2605 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_BCAST ; }
#line 7476 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 619:
#line 2606 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_NBMA ; }
#line 7482 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 620:
#line 2607 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_NBMA ; }
#line 7488 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 621:
#line 2608 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_PTP ; }
#line 7494 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 622:
#line 2609 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_PTP ; }
#line 7500 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 623:
#line 2610 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_PTMP ; }
#line 7506 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 624:
#line 2611 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->type = OSPF_IT_PTMP ; }
#line 7512 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 625:
#line 2612 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->real_bcast = (yyvsp[0].i); if (!ospf_cfg_is_v2()) cf_error("Real broadcast option requires OSPFv2"); }
#line 7518 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 626:
#line 2613 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->ptp_netmask = (yyvsp[0].i); if (!ospf_cfg_is_v2()) cf_error("PtP netmask option requires OSPFv2"); }
#line 7524 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 627:
#line 2614 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->inftransdelay = (yyvsp[0].i) ; if (((yyvsp[0].i)<=0) || ((yyvsp[0].i)>65535)) cf_error("Transmit delay must be in range 1-65535"); }
#line 7530 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 628:
#line 2615 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->priority = (yyvsp[0].i) ; if (((yyvsp[0].i)<0) || ((yyvsp[0].i)>255)) cf_error("Priority must be in range 0-255"); }
#line 7536 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 629:
#line 2616 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->strictnbma = (yyvsp[0].i) ; }
#line 7542 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 630:
#line 2617 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->stub = (yyvsp[0].i) ; }
#line 7548 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 631:
#line 2618 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->check_link = (yyvsp[0].i); }
#line 7554 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 632:
#line 2619 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->ecmp_weight = (yyvsp[0].i) - 1; if (((yyvsp[0].i)<1) || ((yyvsp[0].i)>256)) cf_error("ECMP weight must be in range 1-256"); }
#line 7560 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 633:
#line 2620 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->link_lsa_suppression = (yyvsp[0].i); if (!ospf_cfg_is_v3()) cf_error("Link LSA suppression option requires OSPFv3"); }
#line 7566 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 635:
#line 2622 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_NONE; }
#line 7572 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 636:
#line 2623 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_SIMPLE; ospf_check_auth(); }
#line 7578 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 637:
#line 2624 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->autype = OSPF_AUTH_CRYPT; ospf_check_auth(); }
#line 7584 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 638:
#line 2625 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->rx_buffer = 0; }
#line 7590 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 639:
#line 2626 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->rx_buffer = OSPF_MAX_PKT_SIZE; }
#line 7596 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 640:
#line 2627 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->rx_buffer = (yyvsp[0].i); if (((yyvsp[0].i) < OSPF_MIN_PKT_SIZE) || ((yyvsp[0].i) > OSPF_MAX_PKT_SIZE)) cf_error("Buffer size must be in range 256-65535"); }
#line 7602 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 641:
#line 2628 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->tx_tos = (yyvsp[0].i); }
#line 7608 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 642:
#line 2629 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->tx_priority = (yyvsp[0].i); }
#line 7614 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 643:
#line 2630 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->tx_length = (yyvsp[0].i); if (((yyvsp[0].i) < OSPF_MIN_PKT_SIZE) || ((yyvsp[0].i) > OSPF_MAX_PKT_SIZE)) cf_error("TX length must be in range 256-65535"); }
#line 7620 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 644:
#line 2631 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->ttl_security = (yyvsp[0].i); }
#line 7626 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 645:
#line 2632 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->ttl_security = 2; }
#line 7632 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 646:
#line 2633 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->bfd = (yyvsp[0].i); cf_check_bfd((yyvsp[0].i)); }
#line 7638 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 647:
#line 2634 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->bsd_secondary = (yyvsp[0].i); }
#line 7644 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 648:
#line 2635 "cf-parse.y" /* yacc.c:1646  */
    { ospf_check_auth(); }
#line 7650 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 652:
#line 2646 "cf-parse.y" /* yacc.c:1646  */
    {
   this_pref = cfg_allocz(sizeof(struct area_net_config));
   add_tail(this_nets, NODE this_pref);
   this_pref->px.addr = (yyvsp[0].px).addr;
   this_pref->px.len = (yyvsp[0].px).len;
 }
#line 7661 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 654:
#line 2656 "cf-parse.y" /* yacc.c:1646  */
    { this_pref->hidden = 1; }
#line 7667 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 655:
#line 2657 "cf-parse.y" /* yacc.c:1646  */
    { this_pref->tag = (yyvsp[0].i); }
#line 7673 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 658:
#line 2666 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 7679 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 659:
#line 2667 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 7685 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 660:
#line 2671 "cf-parse.y" /* yacc.c:1646  */
    {
   this_nbma = cfg_allocz(sizeof(struct nbma_node));
   add_tail(&OSPF_PATT->nbma_list, NODE this_nbma);
   this_nbma->ip=(yyvsp[-2].a);
   this_nbma->eligible=(yyvsp[-1].i);
 }
#line 7696 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 661:
#line 2680 "cf-parse.y" /* yacc.c:1646  */
    {
  this_ipatt = cfg_allocz(sizeof(struct ospf_iface_patt));
  add_tail(&this_area->patt_list, NODE this_ipatt);
  init_list(&this_ipatt->ipn_list);
  OSPF_PATT->cost = COST_D;
  OSPF_PATT->helloint = HELLOINT_D;
  OSPF_PATT->pollint = POLLINT_D;
  OSPF_PATT->rxmtint = RXMTINT_D;
  OSPF_PATT->inftransdelay = INFTRANSDELAY_D;
  OSPF_PATT->priority = PRIORITY_D;
  OSPF_PATT->deadc = DEADC_D;
  OSPF_PATT->type = OSPF_IT_UNDEF;
  OSPF_PATT->instance_id = OSPF_CFG->instance_id;
  init_list(&OSPF_PATT->nbma_list);
  OSPF_PATT->ptp_netmask = 2; /* not specified */
  OSPF_PATT->tx_tos = IP_PREC_INTERNET_CONTROL;
  OSPF_PATT->tx_priority = sk_priority_control;
  reset_passwords();
 }
#line 7720 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 663:
#line 2703 "cf-parse.y" /* yacc.c:1646  */
    { OSPF_PATT->instance_id = (yyvsp[0].i); if (((yyvsp[0].i)<0) || ((yyvsp[0].i)>255)) cf_error("Instance ID must be in range 0-255"); }
#line 7726 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 664:
#line 2707 "cf-parse.y" /* yacc.c:1646  */
    { if (ospf_cfg_is_v3()) iface_patt_check(); }
#line 7732 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 670:
#line 2721 "cf-parse.y" /* yacc.c:1646  */
    { ospf_iface_finish(); }
#line 7738 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 672:
#line 2726 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.t) = NULL; }
#line 7744 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 673:
#line 2732 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh(proto_get_named((yyvsp[-1].s), &proto_ospf)); }
#line 7750 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 674:
#line 2735 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_neigh(proto_get_named((yyvsp[-2].s), &proto_ospf), (yyvsp[-1].t)); }
#line 7756 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 675:
#line 2738 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_iface(proto_get_named((yyvsp[-2].s), &proto_ospf), (yyvsp[-1].t)); }
#line 7762 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 676:
#line 2743 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_state(proto_get_named((yyvsp[-2].s), &proto_ospf), 0, 1); }
#line 7768 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 677:
#line 2746 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_state(proto_get_named((yyvsp[-2].s), &proto_ospf), 0, 0); }
#line 7774 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 678:
#line 2751 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_state(proto_get_named((yyvsp[-2].s), &proto_ospf), 1, 1); }
#line 7780 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 679:
#line 2754 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_state(proto_get_named((yyvsp[-2].s), &proto_ospf), 1, 0); }
#line 7786 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 680:
#line 2758 "cf-parse.y" /* yacc.c:1646  */
    { ospf_sh_lsadb((yyvsp[-1].ld)); }
#line 7792 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 681:
#line 2761 "cf-parse.y" /* yacc.c:1646  */
    {
     (yyval.ld) = cfg_allocz(sizeof(struct lsadb_show_data));
   }
#line 7800 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 682:
#line 2764 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-1].ld); (yyval.ld)->scope = LSA_SCOPE_AS; }
#line 7806 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 683:
#line 2765 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-2].ld); (yyval.ld)->scope = LSA_SCOPE_AREA; (yyval.ld)->area = (yyvsp[0].i32); }
#line 7812 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 684:
#line 2766 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-1].ld); (yyval.ld)->scope = 1; /* hack, 0 is no filter */ }
#line 7818 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 685:
#line 2767 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-2].ld); (yyval.ld)->type = (yyvsp[0].i); }
#line 7824 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 686:
#line 2768 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-2].ld); (yyval.ld)->lsid = (yyvsp[0].i32); }
#line 7830 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 687:
#line 2769 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-1].ld); (yyval.ld)->router = SH_ROUTER_SELF; }
#line 7836 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 688:
#line 2770 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-2].ld); (yyval.ld)->router = (yyvsp[0].i32); }
#line 7842 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 689:
#line 2771 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.ld) = (yyvsp[-1].ld); (yyval.ld)->name = (yyvsp[0].s); }
#line 7848 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 690:
#line 2777 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_pipe, (yyvsp[-1].i));
     PIPE_CFG->mode = PIPE_TRANSPARENT;
  }
#line 7857 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 693:
#line 2786 "cf-parse.y" /* yacc.c:1646  */
    {
     if ((yyvsp[-1].s)->class != SYM_TABLE)
       cf_error("Routing table name expected");
     PIPE_CFG->peer = (yyvsp[-1].s)->def;
   }
#line 7867 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 694:
#line 2791 "cf-parse.y" /* yacc.c:1646  */
    { PIPE_CFG->mode = PIPE_OPAQUE; }
#line 7873 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 695:
#line 2792 "cf-parse.y" /* yacc.c:1646  */
    { PIPE_CFG->mode = PIPE_TRANSPARENT; }
#line 7879 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 696:
#line 2798 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_rip, (yyvsp[-1].i));
     rip_init_config(RIP_CFG);
   }
#line 7888 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 699:
#line 2807 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->infinity = (yyvsp[-1].i); }
#line 7894 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 700:
#line 2808 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->port = (yyvsp[-1].i); }
#line 7900 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 701:
#line 2809 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->period = (yyvsp[-1].i); }
#line 7906 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 702:
#line 2810 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->garbage_time = (yyvsp[-1].i); }
#line 7912 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 703:
#line 2811 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->timeout_time = (yyvsp[-1].i); }
#line 7918 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 704:
#line 2812 "cf-parse.y" /* yacc.c:1646  */
    {RIP_CFG->authtype = (yyvsp[-1].i); }
#line 7924 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 706:
#line 2814 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->honor = HO_ALWAYS; }
#line 7930 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 707:
#line 2815 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->honor = HO_NEIGHBOR; }
#line 7936 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 708:
#line 2816 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->honor = HO_NEVER; }
#line 7942 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 710:
#line 2821 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=AT_PLAINTEXT; }
#line 7948 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 711:
#line 2822 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=AT_MD5; }
#line 7954 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 712:
#line 2823 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=AT_NONE; }
#line 7960 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 713:
#line 2828 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_BROADCAST; }
#line 7966 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 714:
#line 2829 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=0; }
#line 7972 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 715:
#line 2830 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_QUIET; }
#line 7978 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 716:
#line 2831 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_NOLISTEN; }
#line 7984 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 717:
#line 2832 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_VERSION1 | IM_BROADCAST; }
#line 7990 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 719:
#line 2836 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->metric = (yyvsp[0].i); }
#line 7996 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 720:
#line 2837 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->mode |= (yyvsp[0].i); }
#line 8002 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 721:
#line 2838 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->tx_tos = (yyvsp[0].i); }
#line 8008 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 722:
#line 2839 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->tx_priority = (yyvsp[0].i); }
#line 8014 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 723:
#line 2840 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->ttl_security = (yyvsp[0].i); }
#line 8020 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 724:
#line 2841 "cf-parse.y" /* yacc.c:1646  */
    { RIP_IPATT->ttl_security = 2; }
#line 8026 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 729:
#line 2855 "cf-parse.y" /* yacc.c:1646  */
    {
     this_ipatt = cfg_allocz(sizeof(struct rip_patt));
     add_tail(&RIP_CFG->iface_list, NODE this_ipatt);
     init_list(&this_ipatt->ipn_list);
     RIP_IPATT->metric = 1;
     RIP_IPATT->tx_tos = IP_PREC_INTERNET_CONTROL;
     RIP_IPATT->tx_priority = sk_priority_control;
     RIP_IPATT->ttl_security = RIP_DEFAULT_TTL_SECURITY;
   }
#line 8040 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 731:
#line 2874 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_static, (yyvsp[-1].i));
     static_init_config((struct static_config *) this_proto);
  }
#line 8049 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 734:
#line 2883 "cf-parse.y" /* yacc.c:1646  */
    { STATIC_CFG->check_link = (yyvsp[-1].i); }
#line 8055 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 735:
#line 2884 "cf-parse.y" /* yacc.c:1646  */
    { STATIC_CFG->igp_table = (yyvsp[-1].r); }
#line 8061 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 737:
#line 2888 "cf-parse.y" /* yacc.c:1646  */
    {
     this_srt = cfg_allocz(sizeof(struct static_route));
     add_tail(&STATIC_CFG->other_routes, &this_srt->n);
     this_srt->net = (yyvsp[0].px).addr;
     this_srt->masklen = (yyvsp[0].px).len;
  }
#line 8072 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 738:
#line 2897 "cf-parse.y" /* yacc.c:1646  */
    {
     last_srt_nh = this_srt_nh;
     this_srt_nh = cfg_allocz(sizeof(struct static_route));
     this_srt_nh->dest = RTD_NONE;
     this_srt_nh->via = (yyvsp[-1].a);
     this_srt_nh->via_if = (yyvsp[0].iface);
     this_srt_nh->if_name = (void *) this_srt; /* really */
   }
#line 8085 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 739:
#line 2905 "cf-parse.y" /* yacc.c:1646  */
    {
     this_srt_nh->masklen = (yyvsp[0].i) - 1; /* really */
     if (((yyvsp[0].i)<1) || ((yyvsp[0].i)>256)) cf_error("Weight must be in range 1-256"); 
   }
#line 8094 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 740:
#line 2912 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->mp_next = this_srt_nh; }
#line 8100 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 741:
#line 2913 "cf-parse.y" /* yacc.c:1646  */
    { last_srt_nh->mp_next = this_srt_nh; }
#line 8106 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 742:
#line 2917 "cf-parse.y" /* yacc.c:1646  */
    {
      this_srt->dest = RTD_ROUTER;
      this_srt->via = (yyvsp[-1].a);
      this_srt->via_if = (yyvsp[0].iface);
   }
#line 8116 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 743:
#line 2922 "cf-parse.y" /* yacc.c:1646  */
    {
      this_srt->dest = RTD_DEVICE;
      this_srt->if_name = (yyvsp[0].t);
      rem_node(&this_srt->n);
      add_tail(&STATIC_CFG->iface_routes, &this_srt->n);
   }
#line 8127 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 744:
#line 2928 "cf-parse.y" /* yacc.c:1646  */
    {
      this_srt->dest = RTD_MULTIPATH;
   }
#line 8135 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 745:
#line 2931 "cf-parse.y" /* yacc.c:1646  */
    {
      this_srt->dest = RTDX_RECURSIVE;
      this_srt->via = (yyvsp[0].a);
   }
#line 8144 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 746:
#line 2936 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->dest = RTD_BLACKHOLE; }
#line 8150 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 747:
#line 2937 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->dest = RTD_UNREACHABLE; }
#line 8156 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 748:
#line 2938 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->dest = RTD_BLACKHOLE; }
#line 8162 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 749:
#line 2939 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->dest = RTD_UNREACHABLE; }
#line 8168 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 750:
#line 2940 "cf-parse.y" /* yacc.c:1646  */
    { this_srt->dest = RTD_PROHIBIT; }
#line 8174 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 751:
#line 2944 "cf-parse.y" /* yacc.c:1646  */
    { static_show(proto_get_named((yyvsp[-1].s), &proto_static)); }
#line 8180 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 752:
#line 2949 "cf-parse.y" /* yacc.c:1646  */
    {
     this_proto = proto_config_new(&proto_sdn, (yyvsp[-1].i));
     sdn_init_config(SDN_CFG);
   }
#line 8189 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 756:
#line 2959 "cf-parse.y" /* yacc.c:1646  */
    { SDN_CFG->unixsocket = (yyvsp[-1].t); }
#line 8195 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 757:
#line 2963 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_BROADCAST; }
#line 8201 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 758:
#line 2964 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=0; }
#line 8207 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 759:
#line 2965 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_QUIET; }
#line 8213 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 760:
#line 2966 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_NOLISTEN; }
#line 8219 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 761:
#line 2967 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.i)=IM_VERSION1 | IM_BROADCAST; }
#line 8225 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 763:
#line 2971 "cf-parse.y" /* yacc.c:1646  */
    { SDN_IPATT->metric = (yyvsp[0].i); }
#line 8231 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 764:
#line 2972 "cf-parse.y" /* yacc.c:1646  */
    { SDN_IPATT->mode |= (yyvsp[0].i); }
#line 8237 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 765:
#line 2973 "cf-parse.y" /* yacc.c:1646  */
    { SDN_IPATT->tx_tos = (yyvsp[0].i); }
#line 8243 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 766:
#line 2974 "cf-parse.y" /* yacc.c:1646  */
    { SDN_IPATT->tx_priority = (yyvsp[0].i); }
#line 8249 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 767:
#line 2975 "cf-parse.y" /* yacc.c:1646  */
    { SDN_IPATT->ttl_security = (yyvsp[0].i); }
#line 8255 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 768:
#line 2976 "cf-parse.y" /* yacc.c:1646  */
    { SDN_IPATT->ttl_security = 2; }
#line 8261 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 773:
#line 2990 "cf-parse.y" /* yacc.c:1646  */
    {
     this_ipatt = cfg_allocz(sizeof(struct sdn_patt));
     add_tail(&SDN_CFG->iface_list, NODE this_ipatt);
     init_list(&this_ipatt->ipn_list);
     SDN_IPATT->metric = 1;
     SDN_IPATT->tx_tos = IP_PREC_INTERNET_CONTROL;
     SDN_IPATT->tx_priority = sk_priority_control;
     SDN_IPATT->ttl_security = SDN_DEFAULT_TTL_SECURITY;
   }
#line 8275 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 842:
#line 3011 "cf-parse.y" /* yacc.c:1646  */
    { bgp_check_config(BGP_CFG); }
#line 8281 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 843:
#line 3011 "cf-parse.y" /* yacc.c:1646  */
    { ospf_proto_finish(); }
#line 8287 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 845:
#line 3011 "cf-parse.y" /* yacc.c:1646  */
    { RIP_CFG->passwords = get_passwords(); }
#line 8293 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 855:
#line 3014 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_KRT_SOURCE); }
#line 8299 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 856:
#line 3014 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_KRT_METRIC); }
#line 8305 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 857:
#line 3014 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_IP_ADDRESS, T_IP, EA_KRT_PREFSRC); }
#line 8311 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 858:
#line 3014 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_KRT_REALM); }
#line 8317 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 859:
#line 3015 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_GEN_IGP_METRIC); }
#line 8323 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 860:
#line 3015 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = NULL; }
#line 8329 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 861:
#line 3016 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_ENUM_BGP_ORIGIN, EA_CODE(EAP_BGP, BA_ORIGIN)); }
#line 8335 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 862:
#line 3017 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_AS_PATH, T_PATH, EA_CODE(EAP_BGP, BA_AS_PATH)); }
#line 8341 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 863:
#line 3018 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_IP_ADDRESS, T_IP, EA_CODE(EAP_BGP, BA_NEXT_HOP)); }
#line 8347 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 864:
#line 3019 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_CODE(EAP_BGP, BA_MULTI_EXIT_DISC)); }
#line 8353 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 865:
#line 3020 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_CODE(EAP_BGP, BA_LOCAL_PREF)); }
#line 8359 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 866:
#line 3021 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_OPAQUE, T_ENUM_EMPTY, EA_CODE(EAP_BGP, BA_ATOMIC_AGGR)); }
#line 8365 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 867:
#line 3022 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT, T_INT, EA_CODE(EAP_BGP, BA_AGGREGATOR)); }
#line 8371 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 868:
#line 3023 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT_SET, T_CLIST, EA_CODE(EAP_BGP, BA_COMMUNITY)); }
#line 8377 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 869:
#line 3024 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_ROUTER_ID, T_QUAD, EA_CODE(EAP_BGP, BA_ORIGINATOR_ID)); }
#line 8383 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 870:
#line 3025 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT_SET, T_CLIST, EA_CODE(EAP_BGP, BA_CLUSTER_LIST)); }
#line 8389 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 871:
#line 3026 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_EC_SET, T_ECLIST, EA_CODE(EAP_BGP, BA_EXT_COMMUNITY)); }
#line 8395 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 872:
#line 3026 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_OSPF_METRIC1); }
#line 8401 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 873:
#line 3026 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_OSPF_METRIC2); }
#line 8407 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 874:
#line 3026 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_OSPF_TAG); }
#line 8413 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 875:
#line 3026 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_ROUTER_ID | EAF_TEMP, T_QUAD, EA_OSPF_ROUTER_ID); }
#line 8419 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 876:
#line 3026 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_RIP_METRIC); }
#line 8425 "cf-parse.tab.c" /* yacc.c:1646  */
    break;

  case 877:
#line 3026 "cf-parse.y" /* yacc.c:1646  */
    { (yyval.x) = f_new_dynamic_attr(EAF_TYPE_INT | EAF_TEMP, T_INT, EA_RIP_TAG); }
#line 8431 "cf-parse.tab.c" /* yacc.c:1646  */
    break;


#line 8435 "cf-parse.tab.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



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
                      yytoken, &yylval);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 3028 "cf-parse.y" /* yacc.c:1906  */

/* C Code from ../../conf/confbase.Y */

/* C Code from ../../sysdep/unix/config.Y */

/* C Code from ../../sysdep/unix/krt.Y */

/* C Code from ../../sysdep/linux/netlink.Y */

/* C Code from ../../nest/config.Y */

/* C Code from ../../proto/bfd/config.Y */

/* C Code from ../../proto/bgp/config.Y */

/* C Code from ../../proto/ospf/config.Y */

/* C Code from ../../proto/pipe/config.Y */

/* C Code from ../../proto/rip/config.Y */

/* C Code from ../../proto/static/config.Y */

/* C Code from ../../proto/sdn/config.Y */

