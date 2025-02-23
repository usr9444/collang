#ifndef Ast_H_
#define Ast_H_
#include "Scanning.h"
enum _eNd_
{
	eN_Err,
	/*Basic*/
	eN_Int,
};
struct _tAstNd_
{
	enum _eNd_ type;
	long long unsigned ln, pos;
	union _uDat_
	{
		struct _tUOp_
		{
			enum eTkns type;
			struct _tAstNd_ *oprnd;
		}
		uOp;
		struct _tConst_
		{
			enum eTkns type;
			struct _tDat_
			{
				long long unsigned Llu;
				double Dbl;
				char *Txt;
				float Flt;
			}
			dat;
		}
		cnst;
	}
	dat;
};
typedef struct
{
	struct _tTkns_ *tkns;
	struct _tAstNd_ *rt;
	long long unsigned idx;
	struct _tTkn_ *crrnt;
}
tAst;
tAst _tAst_construct_(tScnnr *scnnr);
#define tAst_construct(scnnr) _tAst_construct_(&scnnr)
void _tAst_destruct_(tAst *ast);
#define tAst_destruct(ast) _tAst_destruct_(&ast)
bool _Ast_parse_(tAst *ast);
#define tAst_parse(ast) _Ast_parse_(&ast)
#endif/*Ast_H_*/