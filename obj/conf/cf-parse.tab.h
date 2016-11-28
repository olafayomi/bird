/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 535 "cf-parse.y" /* yacc.c:1909  */

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

#line 436 "cf-parse.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE cf_lval;

int cf_parse (void);

#endif /* !YY_CF_CF_PARSE_TAB_H_INCLUDED  */
