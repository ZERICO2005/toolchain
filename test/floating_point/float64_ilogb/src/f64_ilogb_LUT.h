#ifndef F64_ILOGB_LUT_H
#define F64_ILOGB_LUT_H

#include <stdint.h>
#include <limits.h>
#include <math.h>

/* Generated 2025-03-01T22:43:31Z */

typedef uint64_t input_type;

typedef int output_type;

static const input_type f64_ilogb_LUT_input[256] = {
/*    0 */ UINT64_C(0x0000000000000000),
/*    1 */ UINT64_C(0x0000000000000001),
/*    2 */ UINT64_C(0x0010000000000000),
/*    3 */ UINT64_C(0x3FE0000000000000),
/*    4 */ UINT64_C(0x3FF0000000000000),
/*    5 */ UINT64_C(0x4000000000000000),
/*    6 */ UINT64_C(0x7FEFFFFFFFFFFFFF),
/*    7 */ UINT64_C(0x7FF0000000000000),
/*    8 */ UINT64_C(0x7FF8000000000000),
/*    9 */ UINT64_C(0x7FF4000000000000),
/*   10 */ UINT64_C(0x3FE0000000000001),
/*   11 */ UINT64_C(0x3FF0000000000001),
/*   12 */ UINT64_C(0x4000000000000001),
/*   13 */ UINT64_C(0x3FDFFFFFFFFFFFFF),
/*   14 */ UINT64_C(0x3FEFFFFFFFFFFFFF),
/*   15 */ UINT64_C(0x3FFFFFFFFFFFFFFF),
/*   16 */ UINT64_C(0x0000000000000002),
/*   17 */ UINT64_C(0x0010000000000001),
/*   18 */ UINT64_C(0x000FFFFFFFFFFFFF),
/*   19 */ UINT64_C(0x7FEFFFFFFFFFFFFE),
/*   20 */ UINT64_C(0x8000000000000000),
/*   21 */ UINT64_C(0x8000000000000001),
/*   22 */ UINT64_C(0x8010000000000000),
/*   23 */ UINT64_C(0xBFE0000000000000),
/*   24 */ UINT64_C(0xBFF0000000000000),
/*   25 */ UINT64_C(0xC000000000000000),
/*   26 */ UINT64_C(0xFFEFFFFFFFFFFFFF),
/*   27 */ UINT64_C(0xFFF0000000000000),
/*   28 */ UINT64_C(0xFFF8000000000000),
/*   29 */ UINT64_C(0xFFF4000000000000),
/*   30 */ UINT64_C(0xBFE0000000000001),
/*   31 */ UINT64_C(0xBFF0000000000001),
/*   32 */ UINT64_C(0xC000000000000001),
/*   33 */ UINT64_C(0xBFDFFFFFFFFFFFFF),
/*   34 */ UINT64_C(0xBFEFFFFFFFFFFFFF),
/*   35 */ UINT64_C(0xBFFFFFFFFFFFFFFF),
/*   36 */ UINT64_C(0x8000000000000002),
/*   37 */ UINT64_C(0x8010000000000001),
/*   38 */ UINT64_C(0x800FFFFFFFFFFFFF),
/*   39 */ UINT64_C(0xFFEFFFFFFFFFFFFE),
/*   40 */ UINT64_C(0x61849A4FEC44D477),
/*   41 */ UINT64_C(0xDA4469E0A0C32B9F),
/*   42 */ UINT64_C(0x5327FDBCECE03F47),
/*   43 */ UINT64_C(0xE769447C51F95389),
/*   44 */ UINT64_C(0x15FA6C6E5A2CCF6E),
/*   45 */ UINT64_C(0x1A9ECBDFE22CF854),
/*   46 */ UINT64_C(0x94328FCB36B5B0F4),
/*   47 */ UINT64_C(0x2ACC511C4502BD79),
/*   48 */ UINT64_C(0x0FB44F4ADB47CE27),
/*   49 */ UINT64_C(0xCB2B7FC051B8F3F3),
/*   50 */ UINT64_C(0x31D29D2E781EBB56),
/*   51 */ UINT64_C(0x23133B480BE8DC1B),
/*   52 */ UINT64_C(0x9D33BA00AE31CDEE),
/*   53 */ UINT64_C(0x5A5DA9735CE2E708),
/*   54 */ UINT64_C(0xC35ED568D973698E),
/*   55 */ UINT64_C(0x9F99144DB49055EC),
/*   56 */ UINT64_C(0x105FB02570E69E47),
/*   57 */ UINT64_C(0x6DEFDAEF9752EC11),
/*   58 */ UINT64_C(0x67A60D486E06028A),
/*   59 */ UINT64_C(0xC205ADF1E0385E69),
/*   60 */ UINT64_C(0x4FA1961E087F9295),
/*   61 */ UINT64_C(0x3500706C773CD38E),
/*   62 */ UINT64_C(0xCDB0B54F6A2FCBDD),
/*   63 */ UINT64_C(0x6302E0C0835301BB),
/*   64 */ UINT64_C(0x3B337E74DD52CBC6),
/*   65 */ UINT64_C(0xA5E10E75E7ED7C0E),
/*   66 */ UINT64_C(0x339FC2B8080AC38F),
/*   67 */ UINT64_C(0xCD9413DDF59D8737),
/*   68 */ UINT64_C(0x8C1F99959668E5F9),
/*   69 */ UINT64_C(0x05B0C6B07F87A2AA),
/*   70 */ UINT64_C(0x662CDFC6B3CA6321),
/*   71 */ UINT64_C(0xA8B2D440B2B090A9),
/*   72 */ UINT64_C(0x90AF770B4E439E34),
/*   73 */ UINT64_C(0x81F31945D69C866C),
/*   74 */ UINT64_C(0x2FB0AA5357F234AE),
/*   75 */ UINT64_C(0xD9EEC79CEF93AD46),
/*   76 */ UINT64_C(0xD980CA71E7BE0A2D),
/*   77 */ UINT64_C(0x7155389F4084AD21),
/*   78 */ UINT64_C(0x2C0226F8C9FAED67),
/*   79 */ UINT64_C(0x002C118807762503),
/*   80 */ UINT64_C(0x606DE82AD56E3CA3),
/*   81 */ UINT64_C(0x8F1D7684561454A3),
/*   82 */ UINT64_C(0x405D686FCBE59C1D),
/*   83 */ UINT64_C(0x2A86A495D4F19553),
/*   84 */ UINT64_C(0xC854AFDB0530D211),
/*   85 */ UINT64_C(0x2BBF5DC574814771),
/*   86 */ UINT64_C(0xBB4C294785B5D658),
/*   87 */ UINT64_C(0xC745C4F37F07D636),
/*   88 */ UINT64_C(0x8DE715AFC78A5213),
/*   89 */ UINT64_C(0x0CDA30FB334F2D87),
/*   90 */ UINT64_C(0xF07BDC02DE07655F),
/*   91 */ UINT64_C(0x128D12FD52473BA1),
/*   92 */ UINT64_C(0xBCA40C45BD694861),
/*   93 */ UINT64_C(0x2D0B47028AE2C5FD),
/*   94 */ UINT64_C(0xF89C1E0D0B9FD9AB),
/*   95 */ UINT64_C(0x0B1E3EAC26D485E1),
/*   96 */ UINT64_C(0xE697629216733E03),
/*   97 */ UINT64_C(0x974131B6B70BA6F2),
/*   98 */ UINT64_C(0x75E0AC9EF5ACB977),
/*   99 */ UINT64_C(0xDD55348C7058B8FB),
/*  100 */ UINT64_C(0x8DE9ECF6B3B5C2D4),
/*  101 */ UINT64_C(0x42BD909B7330F8DA),
/*  102 */ UINT64_C(0xB0F4FA93E848E1D5),
/*  103 */ UINT64_C(0x9BE07E4510C6A508),
/*  104 */ UINT64_C(0x540AF17B9BACE73D),
/*  105 */ UINT64_C(0xADD95D77F35380A3),
/*  106 */ UINT64_C(0xD4718D1C62175277),
/*  107 */ UINT64_C(0xF2537146F67CAAB5),
/*  108 */ UINT64_C(0xED3E56A68F995845),
/*  109 */ UINT64_C(0x15BAB2307BE69D78),
/*  110 */ UINT64_C(0x902418754DFCACA7),
/*  111 */ UINT64_C(0x9D7348CD7515D8AA),
/*  112 */ UINT64_C(0xAE3BB4BB085D6FAC),
/*  113 */ UINT64_C(0xEA780829310190FC),
/*  114 */ UINT64_C(0x51B5A76A2B66716B),
/*  115 */ UINT64_C(0x15778651CBFCECDC),
/*  116 */ UINT64_C(0x9717930B1C8E8DFE),
/*  117 */ UINT64_C(0x179B1E1B2898A416),
/*  118 */ UINT64_C(0x7B5E4954A7694178),
/*  119 */ UINT64_C(0x3753EFF558E9172E),
/*  120 */ UINT64_C(0xCAF901A80945CD01),
/*  121 */ UINT64_C(0x1A8CF2F598EB9F70),
/*  122 */ UINT64_C(0x6EE8D914937F1438),
/*  123 */ UINT64_C(0xCCCC2A0722D5175F),
/*  124 */ UINT64_C(0xEE327337A5DDF5A5),
/*  125 */ UINT64_C(0x0369E9BC820F9BAD),
/*  126 */ UINT64_C(0x42731B021C385EAB),
/*  127 */ UINT64_C(0x72BD61DC480B157B),
/*  128 */ UINT64_C(0x35F6A19300383505),
/*  129 */ UINT64_C(0x651C7D551E3FCF0D),
/*  130 */ UINT64_C(0x6F935DCB515C014B),
/*  131 */ UINT64_C(0x5DFA40DAA40D4B69),
/*  132 */ UINT64_C(0x5655071D2FAD0E31),
/*  133 */ UINT64_C(0xF5D07270E9CED7DA),
/*  134 */ UINT64_C(0xC70615CA847C4AC3),
/*  135 */ UINT64_C(0xADDC4EA791944F41),
/*  136 */ UINT64_C(0xE4A7F569DF8B4FD6),
/*  137 */ UINT64_C(0xA7C6ED69075A874C),
/*  138 */ UINT64_C(0x4623BB85F5081A70),
/*  139 */ UINT64_C(0x451D07F424629E23),
/*  140 */ UINT64_C(0x806A2B62C74E4366),
/*  141 */ UINT64_C(0x78FC5E653186238E),
/*  142 */ UINT64_C(0x7CA7A8B2AB666A47),
/*  143 */ UINT64_C(0xD4F2FA8C334B74A9),
/*  144 */ UINT64_C(0xD3D64AB329A40830),
/*  145 */ UINT64_C(0xD5FBDE6386E745F3),
/*  146 */ UINT64_C(0xB0D245421057F908),
/*  147 */ UINT64_C(0x57B539EEC6C20971),
/*  148 */ UINT64_C(0x1582F5250F68BD11),
/*  149 */ UINT64_C(0xD1A5740F566DD12C),
/*  150 */ UINT64_C(0x9FD30114F2C7C2FD),
/*  151 */ UINT64_C(0x0D00F76A1E04EFE5),
/*  152 */ UINT64_C(0x560A65107DD79AA6),
/*  153 */ UINT64_C(0x455C4D1EED49FC2A),
/*  154 */ UINT64_C(0x56DCCC1E534B5C74),
/*  155 */ UINT64_C(0x27B278C3C1B88C03),
/*  156 */ UINT64_C(0x1B116FFE5CC652CA),
/*  157 */ UINT64_C(0x6C29B2A08472AEE1),
/*  158 */ UINT64_C(0xED25C0244198974F),
/*  159 */ UINT64_C(0xD540515781691432),
/*  160 */ UINT64_C(0xBD14D09862587DA5),
/*  161 */ UINT64_C(0x546C051CBD07BA91),
/*  162 */ UINT64_C(0x369ABD67D7582373),
/*  163 */ UINT64_C(0x74F270BE2472574D),
/*  164 */ UINT64_C(0x46B9995675A84DBA),
/*  165 */ UINT64_C(0xE92C702D086D6D46),
/*  166 */ UINT64_C(0x9ECBE4C380B71923),
/*  167 */ UINT64_C(0x1E2DF72DB09765B4),
/*  168 */ UINT64_C(0xB22D51024742559A),
/*  169 */ UINT64_C(0x4E139B2A7D637405),
/*  170 */ UINT64_C(0x6FA706C722206191),
/*  171 */ UINT64_C(0xA570DCF7F9608904),
/*  172 */ UINT64_C(0xD9B6C7D4CDCF9A44),
/*  173 */ UINT64_C(0x68C87B93E66B991A),
/*  174 */ UINT64_C(0x95470C287536B959),
/*  175 */ UINT64_C(0xECC511561C552F5E),
/*  176 */ UINT64_C(0x2536D6E7F81E6C6F),
/*  177 */ UINT64_C(0x352C28AC2CC38053),
/*  178 */ UINT64_C(0xC438CF67DCF389B3),
/*  179 */ UINT64_C(0xC8272F196AF20845),
/*  180 */ UINT64_C(0x85DDFA847589B574),
/*  181 */ UINT64_C(0x8BCE3B6997402284),
/*  182 */ UINT64_C(0x99AC45DA50C55BF2),
/*  183 */ UINT64_C(0x8258151B5C5B6AF9),
/*  184 */ UINT64_C(0xB2D07B5C061F07E0),
/*  185 */ UINT64_C(0x1F4D32FF89F9B1D5),
/*  186 */ UINT64_C(0x17E58B67B9D767F7),
/*  187 */ UINT64_C(0x46AA79A88E1D785D),
/*  188 */ UINT64_C(0x60BCB910EFDC5C21),
/*  189 */ UINT64_C(0x985C3FE2F2427388),
/*  190 */ UINT64_C(0xC324EC441DF65D2E),
/*  191 */ UINT64_C(0x03818B94D35C8823),
/*  192 */ UINT64_C(0xEA8EAB1E55EB9194),
/*  193 */ UINT64_C(0x8F26952A164C34FD),
/*  194 */ UINT64_C(0xA910BD0BE22D6F7E),
/*  195 */ UINT64_C(0xFAD206C3D8B4794F),
/*  196 */ UINT64_C(0x58CA7D47E52FCEB9),
/*  197 */ UINT64_C(0x9833FAB9E204B40D),
/*  198 */ UINT64_C(0x247D7B3FAAD3DADC),
/*  199 */ UINT64_C(0xE73FEB2058CAE6D4),
/*  200 */ UINT64_C(0xA8796827D725EBE5),
/*  201 */ UINT64_C(0x45476EBDC685E844),
/*  202 */ UINT64_C(0x28B3C432A828BAE9),
/*  203 */ UINT64_C(0x8A6A4B41ABC6D0F9),
/*  204 */ UINT64_C(0xC54EE61774B1A296),
/*  205 */ UINT64_C(0x1C8D51160223DB59),
/*  206 */ UINT64_C(0x91B6E9FF404C485F),
/*  207 */ UINT64_C(0x6D24DDFD02726ADD),
/*  208 */ UINT64_C(0xDA87BDD9DCCEC7E8),
/*  209 */ UINT64_C(0xAD2941F73F3C5C4A),
/*  210 */ UINT64_C(0xC699B462544CA3B4),
/*  211 */ UINT64_C(0x340CC6412C7DAD27),
/*  212 */ UINT64_C(0xEA16E650EA38F1C8),
/*  213 */ UINT64_C(0x3E3C2EBF27852886),
/*  214 */ UINT64_C(0x6CA19BF889E54CE4),
/*  215 */ UINT64_C(0x019FA56C460094DD),
/*  216 */ UINT64_C(0x716AC0597E7D67FE),
/*  217 */ UINT64_C(0x61EDF2810231CA49),
/*  218 */ UINT64_C(0xCEAB3778B0EEAAFE),
/*  219 */ UINT64_C(0xC9D28F27BB540851),
/*  220 */ UINT64_C(0x2C472BB7DD03CA91),
/*  221 */ UINT64_C(0xDDF0C627E65683CC),
/*  222 */ UINT64_C(0x097BF116E84BEDFF),
/*  223 */ UINT64_C(0x97CB3D81AA8A4A43),
/*  224 */ UINT64_C(0xB4707EC88099F977),
/*  225 */ UINT64_C(0x047BDF02B9967BC9),
/*  226 */ UINT64_C(0x87799094CEA6ECDE),
/*  227 */ UINT64_C(0x333B8DF284B73653),
/*  228 */ UINT64_C(0xDD9BFF5B87D7ED57),
/*  229 */ UINT64_C(0xCF836121DA6F04B8),
/*  230 */ UINT64_C(0xE540B715B08BC7F6),
/*  231 */ UINT64_C(0x54505CF98B029F86),
/*  232 */ UINT64_C(0xDAA19AB547D06459),
/*  233 */ UINT64_C(0x3C926F6AFCF203A7),
/*  234 */ UINT64_C(0xDD77825571CE2B3D),
/*  235 */ UINT64_C(0x7D795F98D458B709),
/*  236 */ UINT64_C(0x784BA2638CD96DFE),
/*  237 */ UINT64_C(0xACCCB3BEC9F91AE3),
/*  238 */ UINT64_C(0x877DC64549BED5F9),
/*  239 */ UINT64_C(0x271B602D75D7549D),
/*  240 */ UINT64_C(0xA24CEC8671F73BA7),
/*  241 */ UINT64_C(0xF1F44C42D3D8CBE9),
/*  242 */ UINT64_C(0xDDB0227B11A38C04),
/*  243 */ UINT64_C(0xB95023AB6D1FBC00),
/*  244 */ UINT64_C(0xA0B49C49D9A9BD84),
/*  245 */ UINT64_C(0x194C5E31ABA59C71),
/*  246 */ UINT64_C(0x1863A0AD2EE9A031),
/*  247 */ UINT64_C(0x0D05FECEFD06F147),
/*  248 */ UINT64_C(0x3273752F9A0C751E),
/*  249 */ UINT64_C(0x42EB7F2AEFB26A8D),
/*  250 */ UINT64_C(0x9F1E8CDDB95D7DA6),
/*  251 */ UINT64_C(0xD520815EF22C4502),
/*  252 */ UINT64_C(0xBE5C8CD1FDB94A03),
/*  253 */ UINT64_C(0x63711520B0E78A71),
/*  254 */ UINT64_C(0x5FB551D1FD32679C),
/*  255 */ UINT64_C(0xC47B64B868541B25),
};

const output_type f64_ilogb_LUT_output[256] = {
/*    0 */ FP_ILOGB0,
/*    1 */ -1074,
/*    2 */ -1022,
/*    3 */ -1,
/*    4 */ 0,
/*    5 */ 1,
/*    6 */ 1023,
/*    7 */ INT_MAX,
/*    8 */ FP_ILOGBNAN,
/*    9 */ FP_ILOGBNAN,
/*   10 */ -1,
/*   11 */ 0,
/*   12 */ 1,
/*   13 */ -2,
/*   14 */ -1,
/*   15 */ 0,
/*   16 */ -1073,
/*   17 */ -1022,
/*   18 */ -1023,
/*   19 */ 1023,
/*   20 */ FP_ILOGB0,
/*   21 */ -1074,
/*   22 */ -1022,
/*   23 */ -1,
/*   24 */ 0,
/*   25 */ 1,
/*   26 */ 1023,
/*   27 */ INT_MAX,
/*   28 */ FP_ILOGBNAN,
/*   29 */ FP_ILOGBNAN,
/*   30 */ -1,
/*   31 */ 0,
/*   32 */ 1,
/*   33 */ -2,
/*   34 */ -1,
/*   35 */ 0,
/*   36 */ -1073,
/*   37 */ -1022,
/*   38 */ -1023,
/*   39 */ 1023,
/*   40 */ 537,
/*   41 */ 421,
/*   42 */ 307,
/*   43 */ 631,
/*   44 */ -672,
/*   45 */ -598,
/*   46 */ -700,
/*   47 */ -339,
/*   48 */ -772,
/*   49 */ 179,
/*   50 */ -226,
/*   51 */ -462,
/*   52 */ -556,
/*   53 */ 422,
/*   54 */ 54,
/*   55 */ -518,
/*   56 */ -762,
/*   57 */ 735,
/*   58 */ 635,
/*   59 */ 33,
/*   60 */ 251,
/*   61 */ -175,
/*   62 */ 220,
/*   63 */ 561,
/*   64 */ -76,
/*   65 */ -417,
/*   66 */ -198,
/*   67 */ 218,
/*   68 */ -830,
/*   69 */ -932,
/*   70 */ 611,
/*   71 */ -372,
/*   72 */ -757,
/*   73 */ -992,
/*   74 */ -260,
/*   75 */ 415,
/*   76 */ 409,
/*   77 */ 790,
/*   78 */ -319,
/*   79 */ -1021,
/*   80 */ 519,
/*   81 */ -782,
/*   82 */ 6,
/*   83 */ -343,
/*   84 */ 134,
/*   85 */ -324,
/*   86 */ -75,
/*   87 */ 117,
/*   88 */ -801,
/*   89 */ -818,
/*   90 */ 776,
/*   91 */ -727,
/*   92 */ -53,
/*   93 */ -303,
/*   94 */ 906,
/*   95 */ -846,
/*   96 */ 618,
/*   97 */ -651,
/*   98 */ 863,
/*   99 */ 470,
/*  100 */ -801,
/*  101 */ 44,
/*  102 */ -240,
/*  103 */ -577,
/*  104 */ 321,
/*  105 */ -290,
/*  106 */ 328,
/*  107 */ 806,
/*  108 */ 724,
/*  109 */ -676,
/*  110 */ -765,
/*  111 */ -552,
/*  112 */ -284,
/*  113 */ 680,
/*  114 */ 284,
/*  115 */ -680,
/*  116 */ -654,
/*  117 */ -646,
/*  118 */ 950,
/*  119 */ -138,
/*  120 */ 176,
/*  121 */ -599,
/*  122 */ 751,
/*  123 */ 205,
/*  124 */ 740,
/*  125 */ -969,
/*  126 */ 40,
/*  127 */ 812,
/*  128 */ -160,
/*  129 */ 594,
/*  130 */ 762,
/*  131 */ 480,
/*  132 */ 358,
/*  133 */ 862,
/*  134 */ 113,
/*  135 */ -290,
/*  136 */ 587,
/*  137 */ -387,
/*  138 */ 99,
/*  139 */ 82,
/*  140 */ -1017,
/*  141 */ 912,
/*  142 */ 971,
/*  143 */ 336,
/*  144 */ 318,
/*  145 */ 352,
/*  146 */ -242,
/*  147 */ 380,
/*  148 */ -679,
/*  149 */ 283,
/*  150 */ -514,
/*  151 */ -815,
/*  152 */ 353,
/*  153 */ 86,
/*  154 */ 366,
/*  155 */ -388,
/*  156 */ -590,
/*  157 */ 707,
/*  158 */ 723,
/*  159 */ 341,
/*  160 */ -46,
/*  161 */ 327,
/*  162 */ -150,
/*  163 */ 848,
/*  164 */ 108,
/*  165 */ 659,
/*  166 */ -531,
/*  167 */ -541,
/*  168 */ -221,
/*  169 */ 226,
/*  170 */ 763,
/*  171 */ -424,
/*  172 */ 412,
/*  173 */ 653,
/*  174 */ -683,
/*  175 */ 717,
/*  176 */ -428,
/*  177 */ -173,
/*  178 */ 68,
/*  179 */ 131,
/*  180 */ -930,
/*  181 */ -835,
/*  182 */ -613,
/*  183 */ -986,
/*  184 */ -210,
/*  185 */ -523,
/*  186 */ -641,
/*  187 */ 107,
/*  188 */ 524,
/*  189 */ -634,
/*  190 */ 51,
/*  191 */ -967,
/*  192 */ 681,
/*  193 */ -781,
/*  194 */ -366,
/*  195 */ 942,
/*  196 */ 397,
/*  197 */ -636,
/*  198 */ -440,
/*  199 */ 628,
/*  200 */ -376,
/*  201 */ 85,
/*  202 */ -372,
/*  203 */ -857,
/*  204 */ 85,
/*  205 */ -567,
/*  206 */ -740,
/*  207 */ 723,
/*  208 */ 425,
/*  209 */ -301,
/*  210 */ 106,
/*  211 */ -191,
/*  212 */ 674,
/*  213 */ -28,
/*  214 */ 715,
/*  215 */ -998,
/*  216 */ 791,
/*  217 */ 543,
/*  218 */ 235,
/*  219 */ 158,
/*  220 */ -315,
/*  221 */ 480,
/*  222 */ -872,
/*  223 */ -643,
/*  224 */ -184,
/*  225 */ -952,
/*  226 */ -904,
/*  227 */ -204,
/*  228 */ 474,
/*  229 */ 249,
/*  230 */ 597,
/*  231 */ 326,
/*  232 */ 427,
/*  233 */ -54,
/*  234 */ 472,
/*  235 */ 984,
/*  236 */ 901,
/*  237 */ -307,
/*  238 */ -904,
/*  239 */ -398,
/*  240 */ -475,
/*  241 */ 800,
/*  242 */ 476,
/*  243 */ -106,
/*  244 */ -500,
/*  245 */ -619,
/*  246 */ -633,
/*  247 */ -815,
/*  248 */ -216,
/*  249 */ 47,
/*  250 */ -526,
/*  251 */ 339,
/*  252 */ -26,
/*  253 */ 568,
/*  254 */ 508,
/*  255 */ 72,
};

#endif /* F64_ILOGB_LUT_H */
