#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "Scanning.h"
#include "Ast.h"
static struct _tAstNde_ *_tAst_tExpr_construct_(tAst *ast);
static struct _tAstNde_ *_tAst_Prmry_construct_(tAst *ast);
void _tAst_print_(struct _tAstNde_ *nde, long long unsigned indnt)
{
	if (nde == NULL) return;
	for (long long unsigned idx = 0; idx < indnt; idx++) printf("\t");
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
	case eN_Unry:
		printf("eN_Unry: ");
		switch (nde->info)
		{
		case eNI_Add:
			printf("+\n");
			break;
		case eNI_Sub:
			printf("-\n");
			break;
		case eNI_Bng:
			printf("!\n");
			break;
		case eNI_BNot:
			printf("~\n");
			break;
		default:
			printf("Unknown eN_Unry (%d)\n", nde->info);
		}
		_tAst_print_(nde->dat.unry, indnt + 1LLU);
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
		case eNI_Prcnt:
			printf("%%\n");
			break;
		default:
			printf("Unknown eN_Multv (%d)\n", nde->info);
		}
		_tAst_print_(nde->dat.arthmtc.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.arthmtc.rhs, indnt + 1LLU);
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
		_tAst_print_(nde->dat.arthmtc.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.arthmtc.rhs, indnt + 1LLU);
		break;
	case eN_BShft:
		printf("eN_BShft: ");
		switch (nde->info)
		{
		case eNI_LShft:
			printf("<<\n");
			break;
		case eNI_RShft:
			printf(">>\n");
			break;
		default:
			printf("Unkown eN_BShft (%d)\n", nde->info);
		}
		_tAst_print_(nde->dat.btws.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.btws.rhs, indnt + 1LLU);
		break;
	case eN_BAnd:
		printf("eN_BAnd: &\n");
		_tAst_print_(nde->dat.btws.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.btws.rhs, indnt + 1LLU);
		break;
	case eN_BXor:
		printf("eN_BXor: *|\n");
		_tAst_print_(nde->dat.btws.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.btws.rhs, indnt + 1LLU);
		break;
	case eN_BOr:
		printf("eN_BOr: |\n");
		_tAst_print_(nde->dat.btws.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.btws.rhs, indnt + 1LLU);
		break;
	case eN_Rltnl:
		printf("eN_Rltnl: ");
		switch (nde->info)
		{
		case eNI_Lss:
			printf("<\n");
			break;
		case eNI_Grt:
			printf(">\n");
			break;
		case eNI_LssEq:
			printf("<=\n");
			break;
		case eNI_GrtEq:
			printf(">=\n");
			break;
		case eNI_EqEq:
			printf("==\n");
			break;
		case eNI_BngEq:
			printf("!=\n");
			break;
		default:
			printf("Unkown eN_Rltnl (%d)\n", nde->info);
		}
		_tAst_print_(nde->dat.btws.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.btws.rhs, indnt + 1LLU);
		break;
	case eN_Lgcl:
		printf("eN_Lgcl: ");
		switch (nde->info)
		{
		case eNI_And:
			printf("and\n");
			break;
		case eNI_Or:
			printf("or\n");
			break;
		default:
			printf("Unkown eN_Lgcl (%d)\n", nde->info);
		}
		_tAst_print_(nde->dat.btws.lhs, indnt + 1LLU);
		_tAst_print_(nde->dat.btws.rhs, indnt + 1LLU);
		break;
	case eN_Trn:
		printf("eN_Trn:(\n");
		_tAst_print_(nde->dat.trn.cnd, indnt + 1LLU);
		for (long long unsigned idx = 0; idx < indnt; idx++) printf("\t");
		printf("?\n");
		_tAst_print_(nde->dat.trn.tru, indnt + 1LLU);
		for (long long unsigned idx = 0; idx < indnt; idx++) printf("\t");
		printf(",\n");
		_tAst_print_(nde->dat.trn.fls, indnt + 1LLU);
		for (long long unsigned idx = 0; idx < indnt; idx++) printf("\t");
		printf(")\n");
		break;
	case eN_Expr:
		printf("eN_Expr:(\n");
		_tAst_print_(nde->dat.expr, indnt + 1LLU);
		for (long long unsigned idx = 0; idx < indnt; idx++) printf("\t");
		printf(")\n");
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
		case eNI_BXorEq:
			printf("*|=\n");
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
		for (long long unsigned idx = 0; idx < indnt; idx++) printf("\t");
		printf(");\n");
		break;
	case eN_StmntLst:
		printf("eN_StmntLst:(\n");
		for (long long unsigned idx = 0LLU; idx < nde->dat.stmntLst.lng; ++idx) _tAst_print_(nde->dat.stmntLst.stmnts[idx], indnt + 1LLU);
		for (long long unsigned idx = 0; idx < indnt; idx++) printf("\t");
		printf(")\n");
		break;
	case eN_Err:
		fprintf(stderr, "Err: Unknown Ast node.\n");
		break;
	default:
		fprintf(stderr, "Err: Unknown Ast node type: %d\n", nde->type);
	}
}
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
		.strs = NULL, .cpcty = 0LLU, .lng = 0LLU
	};
}
static void _tArrStr_destruct_(struct _tArrStr_ *strs)
{
	if (strs == NULL) return;
	for (long long unsigned idx = 0LLU; idx < strs->lng; ++idx) free(strs->strs[idx]);
	if (strs->strs != NULL)
	{
		free((void *)strs->strs);
		strs->strs = NULL;
	}
	strs->cpcty = 0LLU;
	strs->lng = 0LLU;
}
static bool _tArrStr_addStr_(struct _tArrStr_ *strs, char const *txt)
{
	if (strs == NULL) return true;
	else if (strs->cpcty == 0LLU)
	{
		strs->strs = (char **)calloc(strs->cpcty, sizeof(char *));
		if (strs->strs == NULL)
		{
			fprintf(stderr, "Err: Could not allocate memory for _tArrStr_.\n");
			return true;
		}
		++strs->cpcty;
	}
	else if (strs->lng == strs->cpcty)
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
	strs->strs[strs->lng] = (char *)calloc(strLen, sizeof(char));
	if (strs->strs[strs->lng] == NULL)
	{
		fprintf(stderr, "Err: Could not allocate memory for _tArrStr_[%llu].\n", strs->lng);
		_tArrStr_destruct_(strs);
		return true;
	}
	strncpy(strs->strs[strs->lng++], txt, strLen);
	return false;
}
static bool _tArrStr_srch_(struct _tArrStr_ *strs, char const *txt)
{
	for (long long unsigned idx = 0LLU; idx < strs->lng; ++idx) if (strcmp(strs->strs[idx], txt) == 0) return true;
	return false;
}
static void _tAstNde_destruct_(struct _tAstNde_ *nde)
{
	if (nde == NULL) return;
	else if (nde->type == eN_Cnst) nde->dat.cnst.Llu = 0LLU;
	else if (nde->type == eN_Addtv ||
		nde->type == eN_Multv)
	{
		_tAstNde_destruct_(nde->dat.arthmtc.lhs);
		_tAstNde_destruct_(nde->dat.arthmtc.rhs);
	}
	else if (nde->type == eN_Expr) _tAstNde_destruct_(nde->dat.expr);
	else if (nde->type == eN_Assgn ||
		nde->type == eN_Rssgn)
	{
		_tAstNde_destruct_(nde->dat.assgn.lhs);
		_tAstNde_destruct_(nde->dat.assgn.rhs);
	}
	else if (nde->type == eN_Idntf) nde->dat.idntf = NULL;
	else if (nde->type == eN_Stmnt) _tAstNde_destruct_(nde->dat.stmnt);
	else if (nde->type == eN_Unry) _tAstNde_destruct_(nde->dat.unry);
	else if (nde->type == eN_BShft ||
		nde->type == eN_BAnd ||
		nde->type == eN_BXor ||
		nde->type == eN_BOr)
	{
		_tAstNde_destruct_(nde->dat.btws.lhs);
		_tAstNde_destruct_(nde->dat.btws.rhs);
	}
	else if (nde->type == eN_Rltnl)
	{
		_tAstNde_destruct_(nde->dat.btws.lhs);
		_tAstNde_destruct_(nde->dat.btws.rhs);
	}
	else if (nde->type == eN_Lgcl)
	{
		_tAstNde_destruct_(nde->dat.btws.lhs);
		_tAstNde_destruct_(nde->dat.btws.rhs);
	}
	else if (nde->type == eN_Trn)
	{
		_tAstNde_destruct_(nde->dat.trn.cnd);
		_tAstNde_destruct_(nde->dat.trn.tru);
		_tAstNde_destruct_(nde->dat.trn.fls);
	}
	else if (nde->type == eN_StmntLst) for (long long unsigned idx = 0LLU; idx < nde->dat.stmntLst.lng; ++idx) _tAstNde_destruct_(nde->dat.stmntLst.stmnts[idx]);
	else if (nde->type == eN_Blck) _tAstNde_destruct_(nde->dat.blck.stmnts);
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
	_tArrStr_addStr_(&ast.knwnTypes, "tWrd");
	return ast;
}
void _tAst_destruct_(tAst *ast)
{
	_tArrStr_destruct_(&ast->knwnTypes);
	_tAstNde_destruct_(ast->rt);
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
	if (ast->idx + offst > ast->tkns->lng)
	{
		_tAst_printErr_(ast, "Ran out of tokens", NULL);
		return NULL;
	}
	return &ast->tkns->tkns[ast->idx + offst];
}
static bool _tAst_nxt_(tAst *ast)
{
	if (ast->idx > ast->tkns->lng) return true;
	ast->crrnt = &ast->tkns->tkns[ast->idx++];
	return false;
}
#define tAst_nxt_s(ast, nde) \
do \
{ \
	if (_tAst_nxt_(ast)) \
	{ \
		_tAst_printErr_(ast, "Ran out of tokens", nde); \
		_tAstNde_destruct_(nde); \
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
		_tAstNde_destruct_(nde);
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
		nde->dat.cnst.Bln = (ast->crrnt->type == eT_True);
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
	else
	{
		_tAstNde_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	return nde;
}
static struct _tAstNde_ *_tAst_tUnry_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_Unry;
	nde->info = eNI_Err;
	if (ast->crrnt->type == eT_Add) nde->info = eNI_Add;
	else if (ast->crrnt->type == eT_Sub) nde->info = eNI_Sub;
	else if (ast->crrnt->type == eT_Bng) nde->info = eNI_Bng;
	else if (ast->crrnt->type == eT_BNot) nde->info = eNI_BNot;
	else
	{
		_tAstNde_destruct_(nde);
		nde = _tAst_Prmry_construct_(ast);
		return nde;
	}
	tAst_nxt_s(ast, nde);
	nde->dat.unry = _tAst_Prmry_construct_(ast);
	if (nde->dat.unry == NULL)
	{
		_tAstNde_destruct_(nde);
		return NULL;
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tMultv_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAst_tUnry_construct_(ast);
	if (nde == NULL) return NULL;
	while (ast->crrnt->type == eT_Mult ||
		ast->crrnt->type == eT_Div)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.arthmtc.lhs = nde;
		nde = tmp;
		nde->type = eN_Multv;
		if (ast->crrnt->type == eT_Mult) nde->info = eNI_Mult;
		else if (ast->crrnt->type == eT_Div) nde->info = eNI_Div;
		else if (ast->crrnt->type == eT_Prcnt) nde->info = eNI_Prcnt;
		else
		{
			_tAst_printErr_(ast, "Invalid token", nde);
			_tAstNde_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, nde);
		nde->dat.arthmtc.rhs = _tAst_tUnry_construct_(ast);
		if (nde->dat.arthmtc.rhs == NULL)
		{
			_tAstNde_destruct_(nde);
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
		tmp->dat.arthmtc.lhs = nde;
		nde = tmp;
		nde->type = eN_Addtv;
		if (ast->crrnt->type == eT_Add) nde->info = eNI_Add;
		else if (ast->crrnt->type == eT_Sub) nde->info = eNI_Sub;
		else
		{
			_tAst_printErr_(ast, "Invalid token", nde);
			_tAstNde_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, nde);
		nde->dat.arthmtc.rhs = _tAst_tMultv_construct_(ast);
		if (nde->dat.arthmtc.rhs == NULL)
		{
			_tAstNde_destruct_(nde);
			return NULL;
		}
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tShft_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAst_tAddtv_construct_(ast);
	if (nde == NULL) return NULL;
	while (ast->crrnt->type == eT_LssLss || ast->crrnt->type == eT_GrtGrt)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.btws.lhs = nde;
		nde = tmp;
		nde->type = eN_BShft;
		if (ast->crrnt->type == eT_LssLss) nde->info = eNI_LShft;
		else if (ast->crrnt->type == eT_GrtGrt) nde->info = eNI_RShft;
		else
		{
			_tAst_printErr_(ast, "Invalid token", nde);
			_tAstNde_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, nde);
		nde->dat.btws.rhs = _tAst_tAddtv_construct_(ast);
		if (nde->dat.btws.rhs == NULL)
		{
			_tAstNde_destruct_(nde);
			return NULL;
		}
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tBAnd_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAst_tShft_construct_(ast);
	if (nde == NULL) return NULL;
	while (ast->crrnt->type == eT_BAnd)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.btws.lhs = nde;
		nde = tmp;
		nde->type = eN_BAnd;
		if (ast->crrnt->type == eT_BAnd) nde->info = eNI_Err;
		else
		{
			_tAst_printErr_(ast, "Invalid token", nde);
			_tAstNde_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, nde);
		nde->dat.btws.rhs = _tAst_tShft_construct_(ast);
		if (nde->dat.btws.rhs == NULL)
		{
			_tAstNde_destruct_(nde);
			return NULL;
		}
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tBXor_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAst_tBAnd_construct_(ast);
	if (nde == NULL) return NULL;
	while (ast->crrnt->type == eT_MultBOr)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.btws.lhs = nde;
		nde = tmp;
		nde->type = eN_BXor;
		if (ast->crrnt->type == eT_MultBOr) nde->info = eNI_Err;
		else
		{
			_tAst_printErr_(ast, "Invalid token", nde);
			_tAstNde_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, nde);
		nde->dat.btws.rhs = _tAst_tBAnd_construct_(ast);
		if (nde->dat.btws.rhs == NULL)
		{
			_tAstNde_destruct_(nde);
			return NULL;
		}
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tBOr_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAst_tBXor_construct_(ast);
	if (nde == NULL) return NULL;
	while (ast->crrnt->type == eT_BOr)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.btws.lhs = nde;
		nde = tmp;
		nde->type = eN_BOr;
		if (ast->crrnt->type == eT_BOr) nde->info = eNI_Err;
		else
		{
			_tAst_printErr_(ast, "Invalid token", nde);
			_tAstNde_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, nde);
		nde->dat.btws.rhs = _tAst_tBXor_construct_(ast);
		if (nde->dat.btws.rhs == NULL)
		{
			_tAstNde_destruct_(nde);
			return NULL;
		}
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tRltnl_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAst_tBOr_construct_(ast);
	if (nde == NULL) return NULL;
	while (ast->crrnt->type == eT_Lss ||
		ast->crrnt->type == eT_Grt ||
		ast->crrnt->type == eT_LssEq ||
		ast->crrnt->type == eT_GrtEq ||
		ast->crrnt->type == eT_EqEq ||
		ast->crrnt->type == eT_BngEq)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.btws.lhs = nde;
		nde = tmp;
		nde->type = eN_Rltnl;
		if (ast->crrnt->type == eT_Lss) nde->info = eNI_Lss;
		else if (ast->crrnt->type == eT_Grt) nde->info = eNI_Grt;
		else if (ast->crrnt->type == eT_LssEq) nde->info = eNI_LssEq;
		else if (ast->crrnt->type == eT_GrtEq) nde->info = eNI_GrtEq;
		else if (ast->crrnt->type == eT_EqEq) nde->info = eNI_EqEq;
		else if (ast->crrnt->type == eT_BngEq) nde->info = eNI_BngEq;
		else
		{
			_tAst_printErr_(ast, "Invalid token", nde);
			_tAstNde_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, nde);
		nde->dat.btws.rhs = _tAst_tBOr_construct_(ast);
		if (nde->dat.btws.rhs == NULL)
		{
			_tAstNde_destruct_(nde);
			return NULL;
		}
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tLgcl_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAst_tRltnl_construct_(ast);
	if (nde == NULL) return NULL;
	while (ast->crrnt->type == eT_And || ast->crrnt->type == eT_Or)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.btws.lhs = nde;
		nde = tmp;
		nde->type = eN_Lgcl;
		if (ast->crrnt->type == eT_And) nde->info = eNI_And;
		else if (ast->crrnt->type == eT_Or) nde->info = eNI_Or;
		else
		{
			_tAst_printErr_(ast, "Invalid token", nde);
			_tAstNde_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, nde);
		nde->dat.btws.rhs = _tAst_tRltnl_construct_(ast);
		if (nde->dat.btws.rhs == NULL)
		{
			_tAstNde_destruct_(nde);
			return NULL;
		}
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tTrn_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_Trn;
	nde->info = eNI_Err;
	nde->dat.trn.cnd = _tAst_tLgcl_construct_(ast);
	if (nde->dat.trn.cnd == NULL)
	{
		_tAstNde_destruct_(nde);
		return NULL;
	}
	else if (ast->crrnt->type != eT_Qstn)
	{
		struct _tAstNde_ *tmp = nde->dat.trn.cnd;
		nde->dat.trn.cnd = NULL;
		_tAstNde_destruct_(nde);
		return tmp;
	}
	tAst_nxt_s(ast, nde);
	nde->dat.trn.tru = _tAst_tExpr_construct_(ast);
	if (nde->dat.trn.tru == NULL)
	{
		_tAst_printErr_(ast, "Expected expression for when condition is True in ternary operator", nde->dat.trn.tru);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	else if (ast->crrnt->type != eT_Cmma)
	{
		_tAst_printErr_(ast, "Expected ',' for ternary operator", nde->dat.trn.fls);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	nde->dat.trn.fls = _tAst_tExpr_construct_(ast);
	if (nde->dat.trn.fls == NULL)
	{
		_tAst_printErr_(ast, "Expected expression for when condition is False in ternary operator", nde->dat.trn.fls);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	return nde;
}
static struct _tAstNde_ *_tAst_tExpr_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_Expr;
	nde->info = eNI_Err;
	nde->dat.expr = _tAst_tTrn_construct_(ast);
	if (nde->dat.expr == NULL)
	{
		_tAstNde_destruct_(nde);
		return NULL;
	}
	return nde;
}
static struct _tAstNde_ *_tAst_Prmry_construct_(tAst *ast)
{
	if (ast->crrnt->type == eT_Id) return _tAst_tIdntf_construct_(ast);
	else if (ast->crrnt->type == eT_LPrnth)
	{
		tAst_nxt_s(ast, NULL);
		struct _tAstNde_ *nde = _tAst_tExpr_construct_(ast);
		if (ast->crrnt->type != eT_RPrnth)
		{
			_tAst_printErr_(ast, "Missing right parenthesis", nde);
			_tAstNde_destruct_(nde);
			return NULL;
		}
		tAst_nxt_s(ast, NULL);
		return nde;
	}
	else return _tAst_uCnst_construct_(ast);
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
		_tAstNde_destruct_(nde);
		return NULL;
	}
	else if (ast->crrnt->type != eT_Cln)
	{
		_tAst_printErr_(ast, "Expected ':'", nde);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	if (ast->crrnt->type == eT_Id) nde->dat.assgn.datType = ast->crrnt->bffr.bffr;
	else if (ast->crrnt->type == eT_Infr) nde->dat.assgn.datType = NULL;
	else
	{
		_tAst_printErr_(ast, "Expected identifier to denote variable type", nde);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	if (_tArrStr_srch_(&ast->knwnTypes, ast->crrnt->bffr.bffr) == false)
	{
		_tAst_printErr_(ast, "Identifier is not a valid type", nde);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	if (ast->crrnt->type == eT_Eq) nde->info = eNI_Eq;
	else
	{
		_tAst_printErr_(ast, "Unexpected token", nde);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	nde->dat.assgn.rhs = _tAst_tExpr_construct_(ast);
	if (nde->dat.assgn.rhs == NULL)
	{
		_tAstNde_destruct_(nde);
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
		_tAstNde_destruct_(nde);
		return NULL;
	}
	else if (ast->crrnt->type == eT_Eq) nde->info = eNI_Eq;
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
	else if (ast->crrnt->type == eT_MultBOrEq) nde->info = eNI_BXorEq;
	else
	{
		_tAst_printErr_(ast, "Unexpected token", nde);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	nde->dat.assgn.rhs = _tAst_tExpr_construct_(ast);
	if (nde->dat.assgn.rhs == NULL)
	{
		_tAstNde_destruct_(nde);
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
		_tAstNde_destruct_(nde);
		return NULL;
	}
	else if (ahd->type == eT_Cln)
	{
		nde->dat.stmnt = _tAst_tAssgn_construct_(ast);
		if (nde->dat.stmnt == NULL)
		{
			_tAstNde_destruct_(nde);
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
		ahd->type == eT_PrcntEq ||
		ahd->type == eT_MultBOrEq)
	{
		nde->dat.stmnt = _tAst_tRssgn_construct_(ast);
		if (nde->dat.stmnt == NULL)
		{
			_tAstNde_destruct_(nde);
			return NULL;
		}
	}
	else
	{
		nde->dat.stmnt = _tAst_tExpr_construct_(ast);
		if (nde->dat.stmnt == NULL)
		{
			_tAstNde_destruct_(nde);
			return NULL;
		}
	}
	if (ast->crrnt->type != eT_Smicln)
	{
		_tAst_printErr_(ast, "Expected ';'", NULL);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	return nde;
}
static bool _tStmntLst_addStmnt_(struct _tAstNde_ *nde, struct _tAstNde_ *chld)
{
	if (nde == NULL) return true;
	else if (nde->dat.stmntLst.cpcty == 0LLU)
	{
		nde->dat.stmntLst.stmnts = (struct _tAstNde_ **)calloc(1LLU, sizeof(struct _tAstNde_ *));
		if (nde->dat.stmntLst.stmnts == NULL)
		{
			fprintf(stderr, "Err: Could not allocate memory for _tStmntLst_.\n");
			return true;
		}
		++nde->dat.stmntLst.cpcty;
	}
	else if (nde->dat.stmntLst.lng == nde->dat.stmntLst.cpcty)
	{
		struct _tAstNde_ **newPtr = (struct _tAstNde_ **)realloc(nde->dat.stmntLst.stmnts, sizeof(struct _tAstNde_ *) * (nde->dat.stmntLst.cpcty << 1));
		if (newPtr == NULL)
		{
			fprintf(stderr, "Err: Could not reallocate memory for _tStmntLst_.\n");
			return true;
		}
		nde->dat.stmntLst.stmnts = newPtr;
		nde->dat.stmntLst.cpcty <<= 1;
	}
	nde->dat.stmntLst.stmnts[nde->dat.stmntLst.lng++] = chld;
	return false;
}
static struct _tAstNde_ *_tAst_tStmntLst_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_StmntLst;
	nde->info = eNI_Err;
	while (ast->idx < ast->tkns->lng && ast->crrnt->type != eT_RCrly)
	{
		struct _tAstNde_ *stmnt = _tAst_tStmnt_construct_(ast);
		if (stmnt == NULL)
		{
			_tAstNde_destruct_(nde);
			return NULL;
		}
		else if (_tStmntLst_addStmnt_(nde, stmnt))
		{
			_tAstNde_destruct_(stmnt);
			_tAstNde_destruct_(nde);
			return NULL;
		}
	}
	return nde;
}
/*static struct _tAstNde_ *_tAst_tBlck_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	if (ast->crrnt->type != eT_LCrly)
	{
		_tAst_printErr_(ast, "Expected '{' to open block", nde);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	nde->type = eN_Blck;
	nde->info = eNI_Err;
	nde->dat.blck.stmnts = _tAst_tStmntLst_construct_(ast);
	if (nde->dat.blck.stmnts == NULL)
	{
		_tAstNde_destruct_(nde);
		return NULL;
	}
	else if (ast->crrnt->type != eT_RCrly)
	{
		_tAst_printErr_(ast, "Expected '}' to close block", nde);
		_tAstNde_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	return nde;
}*/
bool _Ast_parse_(tAst *ast)
{
	if (_tAst_nxt_(ast))
	{
		_tAst_printErr_(ast, "No tokens provided in input file", NULL);
		_tAst_destruct_(ast);
		return true;
	}
	ast->rt = _tAst_tStmntLst_construct_(ast);
	if (ast->rt == NULL)
	{
		_tAst_destruct_(ast);
		return true;
	}
	else if (ast->tkns->lng >= ast->idx)
	{
		_tAst_printErr_(ast, "Unparsed tokens remaining", NULL);
		_tAst_destruct_(ast);
		return true;
	}
	_tAst_print_(ast->rt, 0LLU);
	return false;
}