#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Scanning.h"
#include "Ast.h"
static struct _tAstNde_ *_tAstNde_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = calloc(1, sizeof(struct _tAstNde_));
	if (nde == NULL) fprintf(stderr, "Err: Could not allocate memory for _tAstNde_.\n");
	else
	{
		nde->ln = ast->crrnt->ln;
		nde->pos = ast->crrnt->pos;
	}
	return nde;
}
static struct _tArrStr_ _tArrStr_construct_(void)
{
	return (struct _tArrStr_)
	{
		.strs = NULL, .cpcty = 0LLU, .len = 0LLU
	};
}
static void _tArrStr_destruct_(struct _tArrStr_ *strs)
{
	if (strs == NULL) return;
	for (long long unsigned idx = 0LLU; idx < strs->len; ++idx) free(strs->strs[idx]);
	if (strs->strs != NULL)
	{
		free((void *)strs->strs);
		strs->strs = NULL;
	}
	strs->cpcty = 0LLU;
	strs->len = 0LLU;
}
static bool _tArrStr_addStr_(struct _tArrStr_ *strs, char const *txt)
{
	if (strs->cpcty == 0LLU)
	{
		strs->strs = (char **)calloc(++strs->cpcty, sizeof(char *));
		if (strs->strs == NULL)
		{
			fprintf(stderr, "Err: Could not allocate memory for _tArrStr_.\n");
			return true;
		}
	}
	else if (strs->len == strs->cpcty)
	{
		char **newPtr = (char **)realloc(strs->strs, sizeof(char *) * (strs->cpcty << 1));
		if (newPtr == NULL)
		{
			fprintf(stderr, "Err: Could not reallocate memory for _tArrStr_.\n");
			return true;
		}
		strs->strs = newPtr;
		strs->cpcty <<= 1;
	}
	long long unsigned strLen = strlen(txt);
	strs->strs[strs->len] = (char *)calloc(strLen, sizeof(char));
	if (strs->strs[strs->len] == NULL)
	{
		fprintf(stderr, "Err: Could not allocate memory for _tArrStr_[%llu].\n", strs->len);
		_tArrStr_destruct_(strs);
		return true;
	}
	strncpy(strs->strs[strs->len], txt, strLen);
	++strs->len;
	return false;
}
static bool _tArrStr_srch_(struct _tArrStr_ *strs, char const *txt)
{
	for (long long unsigned idx = 0LLU; idx < strs->len; ++idx) if (strcmp(strs->strs[idx], txt) == 0) return true;
	return false;
}
static void _tAstNd_destruct_(struct _tAstNde_ *nde)
{
	if (nde == NULL) return;
	else if (nde->type == eN_Cnst) nde->dat.cnst.Llu = 0LLU;
	else if (nde->type == eN_Addtv ||
		nde->type == eN_Multv)
	{
		_tAstNd_destruct_(nde->dat.unry.lhs);
		_tAstNd_destruct_(nde->dat.unry.rhs);
	}
	else if (nde->type == eN_Expr) _tAstNd_destruct_(nde->dat.expr);
	else if (nde->type == eN_Assgn ||
		nde->type == eN_Rssgn)
	{
		_tAstNd_destruct_(nde->dat.assgn.lhs);
		_tAstNd_destruct_(nde->dat.assgn.rhs);
	}
	else if (nde->type == eN_Idntf) nde->dat.idntf = NULL;
	else if (nde->type == eN_Stmnt) _tAstNd_destruct_(nde->dat.stmnt);
	//TODO(Me): Maybe add a memset here to zero everything.
	nde->info = eNI_Err;
	nde->type = eN_Err;
	nde->pos = 0LLU;
	nde->ln = 0LLU;
	free(nde);
}
tAst _tAst_construct_(tScnnr *scnnr)
{
	tAst ast = (tAst)
	{
		.rt = NULL, .tkns = &scnnr->tkns, .idx = 0LLU, .crrnt = NULL, .fileName = scnnr->fileName, .knwnTypes = _tArrStr_construct_()
	};
	_tArrStr_addStr_(&ast.knwnTypes, "tTxt");
	_tArrStr_addStr_(&ast.knwnTypes, "tBln");
	_tArrStr_addStr_(&ast.knwnTypes, "tU8");
	_tArrStr_addStr_(&ast.knwnTypes, "tU16");
	_tArrStr_addStr_(&ast.knwnTypes, "tU32");
	_tArrStr_addStr_(&ast.knwnTypes, "tU64");
	_tArrStr_addStr_(&ast.knwnTypes, "tS8");
	_tArrStr_addStr_(&ast.knwnTypes, "tS16");
	_tArrStr_addStr_(&ast.knwnTypes, "tS32");
	_tArrStr_addStr_(&ast.knwnTypes, "tS64");
	_tArrStr_addStr_(&ast.knwnTypes, "tF32");
	_tArrStr_addStr_(&ast.knwnTypes, "tF64");
	return ast;
}
void _tAst_destruct_(tAst *ast)
{
	_tArrStr_destruct_(&ast->knwnTypes);
	_tAstNd_destruct_(ast->rt);
	ast->rt = NULL;
	ast->idx = 0LLU;
	ast->tkns = NULL;
	ast->fileName = NULL;
}
static void _tAst_printErr_(tAst const *ast, char const *hnt, struct _tAstNde_ const *nde)
{
	fprintf(stderr, "Err: Failed whilst building Ast in file '%s' on line %llu at pos %llu.\nTkn: Enumerated as token type '%d'", ast->fileName, ast->crrnt->ln ? ast->crrnt->ln : 1, ast->crrnt->pos ? ast->crrnt->pos : 1, ast->crrnt->type);
	if (nde == NULL) fprintf(stderr, ".\n");
	else fprintf(stderr, " and enumerated as Ast node type '%d', '%d'.\n", nde->type, nde->info);
	if (hnt != NULL) fprintf(stderr, "Exp: %s.\n", hnt);
}
static struct _tTkn_ *_tAst_ahd_(tAst *ast, long long unsigned offst)
{
	if (ast->idx + offst >= ast->tkns->len)
	{
		_tAst_printErr_(ast, "Ran out of tokens", NULL);
		return NULL;
	}
	return &ast->tkns->tkns[ast->idx + offst];
}
static bool _tAst_nxt_(tAst *ast)
{
	if (ast->idx > ast->tkns->len) return true;
	ast->crrnt = &ast->tkns->tkns[ast->idx++];
	return false;
}
#define tAst_nxt_s(ast, nde) \
do \
{ \
	if (_tAst_nxt_(ast)) \
	{ \
		_tAst_printErr_(ast, "Ran out of tokens", nde); \
		_tAstNd_destruct_(nde); \
		return NULL; \
	} \
} \
while(0)
static struct _tAstNde_ *_tAst_tIdntf_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_Idntf;
	nde->info = eNI_Err;
	if (ast->crrnt->type == eT_Id) nde->dat.idntf = ast->crrnt->bffr.bffr;
	else
	{
		_tAst_printErr_(ast, "Not an identifier", nde);
		_tAstNd_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	return nde;
}
static struct _tAstNde_ *_tAst_uCnst_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_Cnst;
	nde->info = eNI_Int;
	if (ast->crrnt->type == eT_B2) nde->dat.cnst.Llu = strtoull(ast->crrnt->bffr.bffr + 2, NULL, 2);
	else if (ast->crrnt->type == eT_B8) nde->dat.cnst.Llu = strtoull(ast->crrnt->bffr.bffr + 2, NULL, 8);
	else if (ast->crrnt->type == eT_B10) nde->dat.cnst.Llu = strtoull(ast->crrnt->bffr.bffr, NULL, 10);
	else if (ast->crrnt->type == eT_B16) nde->dat.cnst.Llu = strtoull(ast->crrnt->bffr.bffr + 2, NULL, 16);
	else if (ast->crrnt->type == eT_True ||
		ast->crrnt->type == eT_False)
	{
		nde->dat.cnst.Bln = ast->crrnt->type == eT_True;
		nde->info = eNI_Bln;
	}
	else if (ast->crrnt->type == eT_Flt)
	{
		nde->info = eNI_Flt;
		nde->dat.cnst.Flt = strtof(ast->crrnt->bffr.bffr, NULL);
	}
	else if (ast->crrnt->type == eT_Dbl)
	{
		nde->info = eNI_Dbl;
		nde->dat.cnst.Dbl = strtold(ast->crrnt->bffr.bffr, NULL);
	}
	else if (ast->crrnt->type == eT_Txt)
	{
		nde->info = eNI_Txt;
		nde->dat.cnst.Txt = ast->crrnt->bffr.bffr;
	}
	else return _tAst_tIdntf_construct_(ast);
	tAst_nxt_s(ast, nde);
	return nde;
}
static struct _tAstNde_ *_tAst_tMultv_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAst_uCnst_construct_(ast);
	if (nde == NULL) return NULL;
	while (ast->crrnt->type == eT_Mult ||
		ast->crrnt->type == eT_Div)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.unry.lhs = nde;
		nde = tmp;
		nde->type = eN_Multv;
		if (ast->crrnt->type == eT_Mult) nde->info = eNI_Mult;
		else if (ast->crrnt->type == eT_Div) nde->info = eNI_Div;
		else
		{
			_tAst_printErr_(ast, "Invalid token", nde);
			_tAstNd_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, nde);
		nde->dat.unry.rhs = _tAst_uCnst_construct_(ast);
		if (nde->dat.unry.rhs == NULL)
		{
			_tAstNd_destruct_(nde);
			return NULL;
		}
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tAddtv_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAst_tMultv_construct_(ast);
	if (nde == NULL) return NULL;
	while (ast->crrnt->type == eT_Add ||
		ast->crrnt->type == eT_Sub)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.unry.lhs = nde;
		nde = tmp;
		nde->type = eN_Addtv;
		if (ast->crrnt->type == eT_Add) nde->info = eNI_Add;
		else if (ast->crrnt->type == eT_Sub) nde->info = eNI_Sub;
		else
		{
			_tAst_printErr_(ast, "Invalid token", nde);
			_tAstNd_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, nde);
		nde->dat.unry.rhs = _tAst_tMultv_construct_(ast);
		if (nde->dat.unry.rhs == NULL)
		{
			_tAstNd_destruct_(nde);
			return NULL;
		}
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tExpr_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_Expr;
	nde->info = eNI_Err;
	nde->dat.expr = _tAst_tAddtv_construct_(ast);
	return nde;
}
static struct _tAstNde_ *_tAst_tAssgn_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_Assgn;
	nde->info = eNI_Err;
	nde->dat.assgn.lhs = _tAst_tIdntf_construct_(ast);
	if (nde->dat.assgn.lhs == NULL)
	{
		_tAstNd_destruct_(nde);
		return NULL;
	}
	if (ast->crrnt->type != eT_Cln)
	{
		_tAst_printErr_(ast, "Expected ':'", nde);
		_tAstNd_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	if (ast->crrnt->type == eT_Id) nde->dat.assgn.datType = ast->crrnt->bffr.bffr;
	else if (ast->crrnt->type == eT_Infr) nde->dat.assgn.datType = NULL;
	else
	{
		_tAst_printErr_(ast, "Expected identifier to denote variable type", nde);
		_tAstNd_destruct_(nde);
		return NULL;
	}
	if (_tArrStr_srch_(&ast->knwnTypes, ast->crrnt->bffr.bffr) == false)
	{
		_tAst_printErr_(ast, "Identifier is not a valid type", nde);
		_tAstNd_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	if (ast->crrnt->type == eT_Eq) nde->info = eNI_Eq;
	else
	{
		_tAst_printErr_(ast, "Unexpected token", nde);
		_tAstNd_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	nde->dat.assgn.rhs = _tAst_tExpr_construct_(ast);
	if (nde->dat.assgn.rhs == NULL)
	{
		_tAstNd_destruct_(nde);
		return NULL;
	};
	return nde;
}
static struct _tAstNde_ *_tAst_tRssgn_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_Rssgn;
	nde->info = eNI_Err;
	nde->dat.assgn.lhs = _tAst_tIdntf_construct_(ast);
	if (nde->dat.assgn.lhs == NULL)
	{
		_tAstNd_destruct_(nde);
		return NULL;
	}
	if (ast->crrnt->type == eT_Eq) nde->info = eNI_Eq;
	else if (ast->crrnt->type == eT_AddEq) nde->info = eNI_AddEq;
	else if (ast->crrnt->type == eT_SubEq) nde->info = eNI_SubEq;
	else if (ast->crrnt->type == eT_MultEq) nde->info = eNI_MultEq;
	else if (ast->crrnt->type == eT_DivEq) nde->info = eNI_DivEq;
	else if (ast->crrnt->type == eT_LssLssEq) nde->info = eNI_LssLssEq;
	else if (ast->crrnt->type == eT_GrtGrtEq) nde->info = eNI_GrtGrtEq;
	else if (ast->crrnt->type == eT_BOrEq) nde->info = eNI_BOrEq;
	else if (ast->crrnt->type == eT_BAndEq) nde->info = eNI_BAndEq;
	else if (ast->crrnt->type == eT_BNotEq) nde->info = eNI_BNotEq;
	else if (ast->crrnt->type == eT_PrcntEq) nde->info = eNI_PrcntEq;
	else
	{
		_tAst_printErr_(ast, "Unexpected token", nde);
		_tAstNd_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	nde->dat.assgn.rhs = _tAst_tExpr_construct_(ast);
	if (nde->dat.assgn.rhs == NULL)
	{
		_tAstNd_destruct_(nde);
		return NULL;
	};
	return nde;
}
static struct _tAstNde_ *_tAst_tStmnt_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_Stmnt;
	nde->info = eNI_Err;
	struct _tTkn_ *ahd = _tAst_ahd_(ast, 0);
	if (ahd == NULL)
	{
		_tAstNd_destruct_(nde);
		return NULL;
	}
	else if (ahd->type == eT_Cln)
	{
		nde->dat.stmnt = _tAst_tAssgn_construct_(ast);
		if (nde->dat.stmnt == NULL)
		{
			_tAstNd_destruct_(nde);
			return NULL;
		}
	}
	else if (ahd->type == eT_Eq ||
		ahd->type == eT_AddEq ||
		ahd->type == eT_SubEq ||
		ahd->type == eT_MultEq ||
		ahd->type == eT_DivEq ||
		ahd->type == eT_LssLssEq ||
		ahd->type == eT_GrtGrtEq ||
		ahd->type == eT_BOrEq ||
		ahd->type == eT_BAndEq ||
		ahd->type == eT_BNotEq ||
		ahd->type == eT_PrcntEq)
	{
		nde->dat.stmnt = _tAst_tRssgn_construct_(ast);
		if (nde->dat.stmnt == NULL)
		{
			_tAstNd_destruct_(nde);
			return NULL;
		}
	}
	else
	{
		nde->dat.stmnt = _tAst_tExpr_construct_(ast);
		if (nde->dat.stmnt == NULL)
		{
			_tAstNd_destruct_(nde);
			return NULL;
		}
	}
	if (ast->crrnt->type != eT_Smicln)
	{
		_tAst_printErr_(ast, "Expected ';'", NULL);
		_tAstNd_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	return nde;
}
void _tAst_print_(struct _tAstNde_ *nde, long long unsigned indnt)
{
	if (nde == NULL) return;
	for (long long unsigned idx = 0; idx < indnt; idx++) printf(" ");
	switch (nde->type)
	{
	case eN_Cnst:
		printf("eN_Cnst: ");
		switch (nde->info)
		{
		case eNI_Bln:
			printf("(tBln) %s", nde->dat.cnst.Bln ? "True" : "False");
			break;
		case eNI_Int:
			printf("(tInt) %llu", nde->dat.cnst.Llu);
			break;
		case eNI_Flt:
			printf("(tFlt) %f", nde->dat.cnst.Flt);
			break;
		case eNI_Dbl:
			printf("(tDbl) %lf", nde->dat.cnst.Dbl);
			break;
		case eNI_Txt:
			printf("(tTxt) %s", nde->dat.cnst.Txt);
			break;
		default:
			printf("Unknown eN_Cnst (%d)\n", nde->info);
		}
		printf("\n");
		break;
	case eN_Multv:
		printf("eN_Multv: ");
		switch (nde->info)
		{
		case eNI_Mult:
			printf("*\n");
			break;
		case eNI_Div:
			printf("/\n");
			break;
		default:
			printf("Unknown eN_Multv (%d)\n", nde->info);
		}
		_tAst_print_(nde->dat.unry.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.unry.rhs, indnt + 1LLU);
		break;
	case eN_Addtv:
		printf("eN_Addtv: ");
		switch (nde->info)
		{
		case eNI_Add:
			printf("+\n");
			break;
		case eNI_Sub:
			printf("-\n");
			break;
		default:
			printf("Unknown eN_Addtv (%d)\n", nde->info);
		}
		_tAst_print_(nde->dat.unry.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.unry.rhs, indnt + 1LLU);
		break;
	case eN_Expr:
		printf("eN_Expr:\n");
		_tAst_print_(nde->dat.expr, indnt + 1LLU);
		break;
	case eN_Assgn:
		printf("eN_Assgn: %s =\n", nde->dat.assgn.datType);
		_tAst_print_(nde->dat.assgn.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.assgn.rhs, indnt + 1LLU);
		break;
	case eN_Rssgn:
		printf("eN_Rssgn: ");
		switch (nde->info)
		{
		case eNI_Eq:
			printf("=\n");
			break;
		case eNI_AddEq:
			printf("+=\n");
			break;
		case eNI_SubEq:
			printf("-=\n");
			break;
		case eNI_MultEq:
			printf("*=\n");
			break;
		case eNI_DivEq:
			printf("/=\n");
			break;
		case eNI_LssLssEq:
			printf("<<=\n");
			break;
		case eNI_GrtGrtEq:
			printf(">>=\n");
			break;
		case eNI_BOrEq:
			printf("|=\n");
			break;
		case eNI_BAndEq:
			printf("&=\n");
			break;
		case eNI_BNotEq:
			printf("~=\n");
			break;
		case eNI_PrcntEq:
			printf("%%=\n");
			break;
		default:
			printf("Unknown eN_Assgn (%d)\n", nde->info);
			break;
		}
		_tAst_print_(nde->dat.assgn.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.assgn.rhs, indnt + 1LLU);
		break;
	case eN_Idntf:
		printf("eN_Idntf: %s\n", nde->dat.idntf);
		break;
	case eN_Stmnt:
		printf("eN_Stmnt:\n");
		_tAst_print_(nde->dat.stmnt, indnt + 1LLU);
		break;
	case eN_Err:
		fprintf(stderr, "Err: Unknown Ast node.\n");
		break;
	default:
		fprintf(stderr, "Err: Unknown Ast node type: %d\n", nde->type);
	}
}
bool _Ast_parse_(tAst *ast)
{
	if (_tAst_nxt_(ast))
	{
		_tAst_printErr_(ast, "No tokens provided in input file", NULL);
		_tAst_destruct_(ast);
		return true;
	}
	ast->rt = _tAst_tStmnt_construct_(ast);
	if (ast->rt == NULL)
	{
		_tAst_destruct_(ast);
		return true;
	}
	else if (ast->tkns->len >= ast->idx)
	{
		_tAst_printErr_(ast, "Unparsed tokens remaining", NULL);
		_tAst_destruct_(ast);
		return true;
	}
	_tAst_print_(ast->rt, 0);
	return false;
}