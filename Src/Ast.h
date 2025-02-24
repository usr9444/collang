#ifndef Ast_H_
#define Ast_H_
#include "Scanning.h"
enum _eNd_
{
	eN_Err,
	/*Terminal*/
	eN_Cnst,
	eN_Idntf,
	/*Expression*/
	eN_Assgn,
	eN_Rssgn,
	eN_Expr,
	eN_Stmnt,
	/*Arithmetic*/
	eN_Multv,
	eN_Unry,
	eN_Addtv,
};
enum _eNdInfo_
{
	eNI_Err,
	/*Constant types*/
	eNI_Bln,
	eNI_Int,
	eNI_Flt,
	eNI_Dbl,
	eNI_Txt,
	/*Multiplicative*/
	eNI_Mult,
	eNI_Div,
	eNI_Prcnt,
	/*Unary*/
	eNI_Bng,
	eNI_BNot,
	/*Additive*/
	eNI_Add,
	eNI_Sub,
	/*Assignments*/
	eNI_Eq,
	eNI_AddEq,
	eNI_SubEq,
	eNI_MultEq,
	eNI_DivEq,
	eNI_LssLssEq,
	eNI_GrtGrtEq,
	eNI_BOrEq,
	eNI_BAndEq,
	eNI_BNotEq,
	eNI_PrcntEq,
};
struct _tAstNde_
{
	enum _eNd_ type;
	enum _eNdInfo_ info;
	long long unsigned ln, pos;
	union _uDat_
	{
		struct _tArthmtc_
		{
			struct _tAstNde_ *lhs;
			struct _tAstNde_ *rhs;
		}
		arthmtc;
		union _uConst_
		{
			bool Bln;
			long long unsigned Llu;
			double Dbl;
			char *Txt;
			float Flt;
		}
		cnst;
		struct _tAstNde_ *expr;
		struct _tAssgn_
		{
			struct _tAstNde_ *lhs;
			struct _tAstNde_ *rhs;
			char *datType;
		}
		assgn;
		struct _tRssgn_
		{
			struct _tAstNde_ *lhs;
			struct _tAstNde_ *rhs;
		}
		rssgn;
		char *idntf;
		struct _tAstNde_ *stmnt;
		struct _tAstNde_ *unry;
	}
	dat;
};
struct _tArrStr_
{
	char **strs;
	long long unsigned cpcty, len;
};
typedef struct
{
	struct _tTkns_ *tkns;
	struct _tAstNde_ *rt;
	long long unsigned idx;
	struct _tTkn_ *crrnt;
	char const *fileName;
	struct _tArrStr_ knwnTypes;
}
tAst;
tAst _tAst_construct_(tScnnr *scnnr);
#define tAst_construct(scnnr) _tAst_construct_(&scnnr)
void _tAst_destruct_(tAst *ast);
#define tAst_destruct(ast) _tAst_destruct_(&ast)
bool _Ast_parse_(tAst *ast);
#define tAst_parse(ast) _Ast_parse_(&ast)
#endif/*Ast_H_*/