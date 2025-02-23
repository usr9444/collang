#ifndef Scanning_H_
#define Scanning_H_
enum eTkns
{
	eT_Err,
	/*Brackets*/
	eT_LPrnth,	// (
	eT_RPrnth,	// )
	eT_LCrly,	// {
	eT_RCrly,	// }
	eT_LSqr,	// [
	eT_RSqr,	// ]
	eT_Lss,		// <
	eT_Grt,		// >
	eT_Cmma,	// ,
	eT_Dot,		// .
	/*Equations*/
	eT_Add,		// +
	eT_AddEq,	// +=
	eT_Sub,		// -
	eT_SubEq,	// -=
	eT_Mult,	// *
	eT_MultEq,	// *=
	eT_MultBOr,	// *|
	eT_Div,		// /
	eT_DivEq,	// /=
	eT_Smicln,	// ;
	eT_Cln,		// :
	eT_Bng,		// !
	eT_BngEq,	// !=
	eT_Eq,		// =
	eT_EqEq,	// ==
	eT_LssEq,	// <=
	eT_LssLss,	// <<
	eT_LssLssEq,// <<=
	eT_GrtEq,	// >=
	eT_GrtGrt,	// >>
	eT_GrtGrtEq,// >>=
	eT_BOr,		// |
	eT_BOrEq,	// |=
	eT_BAnd,	// &
	eT_BAndEq,	// &=
	eT_BNot,	// ~
	eT_BNotEq,	// ~=
	eT_Prcnt,	// %
	eT_PrcntEq,	// %=
	eT_Qstn,	// ?
	eT_Crt,		// ^
	eT_At,		// @
	eT_She,		// #
	eT_Dlr,		// $
	eT_BSlsh,	// (Backslash)
	/*Flow control*/
	eT_And,		// and
	eT_Or,		// or
	eT_If,		// if
	eT_Else,	// else
	eT_For,		// for
	eT_While,	// while
	eT_Ret,		// ret
	eT_Brk,		// brk
	eT_Fnc,		// fnc
	eT_Infr,	// infr
	eT_Rmbr,	// rmbr
	/*Types*/
	eT_None,	// None
	eT_True,	// True
	eT_False,	// False
	eT_Id,		// <?>		e.g. idntf
	eT_Strng,	// '<?>'	e.g. 'Strng'
	eT_B2,		// 0B0<?>	e,g, 0B001010101
	eT_B8,		// 0O0<?>	e.g. 0O0777
	eT_B10,		// <?>		e.g. 255
	eT_B16,		// 0H0<?>	e.g. 0H0FF
	eT_Flt,		// <?>.<?>F	e.g. 1.0F
	eT_Dbl,		// <?>.<?>	e.g. 1.0
};
struct _tArrBffr_
{
	char *bffr;
	long long unsigned cpcty, len;
};
struct _tTkn_
{
	enum eTkns type;
	long long unsigned ln, pos;
	struct _tArrBffr_ bffr;
};
struct _tTkns_
{
	struct _tTkn_ *tkns;
	long long unsigned cpcty;
	long long unsigned len;
};
typedef struct
{
	char const *fileName;
	long long unsigned strt, crrnt, ln;
	char ch;
	struct _tTkns_ tkns;
	FILE *filePtr;
}
tScnnr;
tScnnr tScnnr_construct(char const *fileName, FILE *filePtr);
void _tScnnr_destruct_(tScnnr *scnnr);
#define tScnnr_destruct(scnnr) _tScnnr_destruct_(&scnnr)
int _tScnnr_parse_(tScnnr *scnnr);
#define tScnnr_parse(scnnr) _tScnnr_parse_(&scnnr)
#endif/*Scanning_H_*/