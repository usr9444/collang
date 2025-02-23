#ifndef Ast_H_
#define Ast_H_
#include "stdbool.h"
#include "Scanning.h"
enum _eNd_
{
	eN_Err,
	/*Basic*/
	eN_Int,
	eN_Flt,
	eN_Dbl,
	eN_Bln,
	eN_Str,
	eN_Type,
	/*Control flow*/
	eN_For,		// "for" "(" eN_Expr ";" eN_Expr ";" eN_Expr ")" "{" eN_StmntLst "}"
	eN_While, 	// "while" "(" eN_Expr ")" "{" eN_StmntLst "}"
	/*Definitions*/
	eN_FncDec, 	// "fnc" eN_Idnt "(" ")" ":" eN_Type ";"
	eN_FncDef, 	// "fnc" eN_Idnt "(" ")" ":" eN_Type "{" eN_StmntLst "}"
	eN_Call, 	// eN_Idnt "(" ")"
};
struct _AstNd_
{
	enum _eNd_ type;
	union _uDat_
	{
		struct _tBOp_
		{
			enum eTkns op;
			struct _AstNd_ *lhs;
			struct _AstNd_ *rhs;
		}
		bOp;
		struct _tUOp_
		{
			enum eTkns op;
			struct _AstNd_ *oprnd;
		}
		uOp;
		struct _tAssgn_
		{
			char *idntf;
			struct _AstNd_ *expr;
		}
		assgn;
		struct _tCall_
		{
			char *idntf;
			// Args
		}
		call;
		struct _tRet_
		{
			struct _AstNd_ *expr;
		}
		ret;
		struct _tId_
		{
			char *name;
		}
		id;
		struct _tConst_
		{
			enum eTkns type;
			union _uDat_
			{
				long long unsigned Llu;
				double Dbl;
				float Flt;
				char *Txt;
				bool Bln;
			}
			val;
		}
		cnst;
		struct _tFncDec_
		{
			char *name;
			enum eTkns retType;
			// Params
		}
		fncDec;
		struct _tFncDef_
		{
			char *name;
			// Params
			struct _AstNd_ *bdy;
		}
		fncDef;
		struct _tBlck_
		{
			// Statements
		}
		blck;
		struct tLoop
		{
			struct _AstNd_ *cnd;
			struct _AstNd_ *bdy;
			struct _AstNd_ *init;	// Only for eN_For
			struct _AstNd_ *incr;	// Only for eN_For
		}
		loop;
		struct _tType_
		{
			enum eTkns type;
		}
		typeNd;
	}
	dat;
};
#endif/*Ast_H_*/