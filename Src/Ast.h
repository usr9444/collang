#ifndef Ast_H_
#define Ast_H_
#include "Scanning.h"
struct _tArrStr_
{
	char **strs;
	long long unsigned cpcty, lng;
};
enum _eNd_
{
	eN_Err,
	/*Terminal*/
	eN_Cnst,
	eN_Idntf,
	/*Arithmetic*/
	eN_Unry,
	eN_Multv,
	eN_Addtv,
	eN_BShft,
	eN_BAnd,
	eN_BXor,
	eN_BOr,
	eN_Rltnl,
	eN_Lgcl,
	/*Expression*/
	eN_Trn,
	eN_Assgn,
	eN_Rssgn,
	eN_Expr,
	eN_Stmnt,
	eN_StmntLst,
	/*Flow control*/
	eN_Blck,
	eN_Cndtl,
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
	/*Unary*/
	eNI_Bng,
	eNI_BNot,
	/*Multiplicative*/
	eNI_Mult,
	eNI_Div,
	eNI_Prcnt,
	/*Additive*/
	eNI_Add,
	eNI_Sub,
	/*Bitwise*/
	eNI_LShft,
	eNI_RShft,
	/*Relational*/
	eNI_Lss,
	eNI_Grt,
	eNI_LssEq,
	eNI_GrtEq,
	eNI_EqEq,
	eNI_BngEq,
	/*Logical*/
	eNI_And,
	eNI_Or,
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
	eNI_BXorEq,
	/*Flow control*/
	eNI_If,
	eNI_While,
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
		struct _tBtws_
		{
			struct _tAstNde_ *lhs;
			struct _tAstNde_ *rhs;
		}
		btws;
		struct _tRltnl_
		{
			struct _tAstNde_ *lhs;
			struct _tAstNde_ *rhs;
		}
		rltnl;
		struct _tLgcl_
		{
			struct _tAstNde_ *lhs;
			struct _tAstNde_ *rhs;
		}
		lgcl;
		struct _tTrn_
		{
			struct _tAstNde_ *cnd;
			struct _tAstNde_ *tru;
			struct _tAstNde_ *fls;
		}
		trn;
		struct _tStmntLst_
		{
			struct _tAstNde_ **stmnts;
			long long unsigned cpcty, lng;
		}
		stmntLst;
		struct _tBlck_
		{
			struct _tAstNde_ *stmnts;
		}
		blck;
		struct _tCndtnl_
		{
			struct _tAstNde_ *cnd;
			struct _tAstNde_ *stmntOrBlck;
		}
		cndtnl;
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