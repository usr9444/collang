#ifndef SCANNING_H_
#define SCANNING_H_
enum eTokens
{
	eT_Err,

	/*Brackets*/
	eT_LPrnth,
	eT_RPrnth,
	eT_LCrly,
	eT_RCrly,
	eT_LSqr,
	eT_RSqr,
	eT_Lss,
	eT_Grt,
	eT_Cmma,
	eT_Dot,
	//eT_Apos,

	/*Equations*/
	eT_Add,
	eT_AddEq,
	eT_Sub,
	eT_SubEq,
	eT_Mult,
	eT_MultEq,
	eT_MultBOr,
	eT_Div,
	eT_DivEq,
	eT_Smicln,
	eT_Cln,
	eT_Bng,
	eT_BngEq,
	eT_Eq,
	eT_EqEq,
	eT_LssEq,
	eT_LssLss,
	eT_LssLssEq,
	eT_GrtEq,
	eT_GrtGrt,
	eT_GrtGrtEq,
	eT_BOr,
	eT_BOrEq,
	eT_BAnd,
	eT_BAndEq,
	eT_BNot,
	eT_BNotEq,
	eT_Prcnt,
	eT_PrcntEq,
	eT_Qstn,
	eT_Crt,
	eT_At,
	eT_She,
	eT_Dlr,
	eT_BSlsh,
	eT_Quot,
	/*Flow control*/
	eT_And,
	eT_Or,
	eT_If,
	eT_Else,
	eT_For,
	eT_While,
	eT_Ret,
	eT_Brk,

	/*Types*/
	eT_None,
	eT_Id,
	eT_Strng,
	eT_B2,
	eT_B8,
	eT_B10,
	eT_B16,
	eT_Flt,
	eT_Dbl,
};
struct _tArrBffr_
{
	char *bffr;
	long long unsigned cpcty, len;
};
struct _tTkn_
{
	enum eTokens type;
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
	long long unsigned strt, curr, ln;
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
#endif/*SCANNING_H_*/