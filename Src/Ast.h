#ifndef Ast_H_
#define Ast_H_
#include "Scanning.h"
enum _eNd_
{
	eN_Err,
	/*Terminal*/
	eN_Cnst,
	eN_Multv,
	eN_Addtv,
};
enum _eNdType_
{
	eNT_Err,
	/*Constant types*/
	eNT_Int,
	eNT_Flt,
	eNT_Dbl,
	eNT_Txt,
	/*Additive*/
	eNT_Mult,
	eNT_Div,
	eNT_Add,
	eNT_Sub,
};
struct _tAstNde_
{
	enum _eNd_ type;
	enum _eNdType_ info;
	long long unsigned ln, pos;
	union _uDat_
	{
		struct _tUnry_
		{
			struct _tAstNde_ *lhs;
			struct _tAstNde_ *rhs;
		}
		unry;
		union _uConst_
		{
			long long unsigned Llu;
			double Dbl;
			char *Txt;
			float Flt;
		}
		cnst;
	}
	dat;
};
typedef struct
{
	struct _tTkns_ *tkns;
	struct _tAstNde_ *rt;
	long long unsigned idx;
	struct _tTkn_ *crrnt;
	char const *fileName;
}
tAst;
tAst _tAst_construct_(tScnnr *scnnr);
#define tAst_construct(scnnr) _tAst_construct_(&scnnr)
void _tAst_destruct_(tAst *ast);
#define tAst_destruct(ast) _tAst_destruct_(&ast)
bool _Ast_parse_(tAst *ast);
#define tAst_parse(ast) _Ast_parse_(&ast)
#endif/*Ast_H_*/