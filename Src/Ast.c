#include <stdlib.h>
#include <stdbool.h>
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
static void _tAstNd_destruct_(struct _tAstNde_ *nde)
{
	if (nde == NULL) return;
	else if (nde->type == eN_Cnst) nde->dat.cnst.Llu = 0LLU;
	else if (nde->type == eN_Addtv || nde->type == eN_Multv)
	{
		_tAstNd_destruct_(nde->dat.unry.lhs);
		_tAstNd_destruct_(nde->dat.unry.rhs);
	}
	nde->info = eNT_Err;
	nde->type = eN_Err;
	nde->pos = 0LLU;
	nde->ln = 0LLU;
	free(nde);
}
tAst _tAst_construct_(tScnnr *scnnr)
{
	return (tAst)
	{
		.rt = NULL, .tkns = &scnnr->tkns, .idx = 0LLU, .crrnt = NULL, .fileName = scnnr->fileName
	};
}
void _tAst_destruct_(tAst *ast)
{
	//Descend
	_tAstNd_destruct_(ast->rt);
	ast->rt = NULL;
	ast->idx = 0LLU;
	ast->tkns = NULL;
	ast->fileName = NULL;
}
static void _tAst_printErr_(tAst const *ast, char const *hnt, struct _tAstNde_ const *nde)
{
	fprintf(stderr, "Err: Failed whilst building Ast in file '%s' on line %llu at pos %llu.\nTkn: token enumerated as token type '%d'", ast->fileName, ast->crrnt->ln, ast->crrnt->pos, ast->crrnt->type);
	if (nde == NULL) fprintf(stderr, ".\n");
	else fprintf(stderr, " and enumerated as Ast node type '%d', '%d'.\n", nde->type, nde->info);
	if (hnt != NULL) fprintf(stderr, "Exp: %s.\n", hnt);
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
static struct _tAstNde_ *_tAst_uCnst_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAstNde_construct_(ast);
	if (nde == NULL) return NULL;
	nde->type = eN_Cnst;
	nde->info = eNT_Int;
	if (ast->crrnt->type == eT_B2) nde->dat.cnst.Llu = strtoull(ast->crrnt->bffr.bffr + 2, NULL, 2);
	else if (ast->crrnt->type == eT_B8) nde->dat.cnst.Llu = strtoull(ast->crrnt->bffr.bffr + 2, NULL, 8);
	else if (ast->crrnt->type == eT_B10) nde->dat.cnst.Llu = strtoull(ast->crrnt->bffr.bffr, NULL, 10);
	else if (ast->crrnt->type == eT_B16) nde->dat.cnst.Llu = strtoull(ast->crrnt->bffr.bffr + 2, NULL, 16);
	else if (ast->crrnt->type == eT_Flt)
	{
		nde->info = eNT_Flt;
		nde->dat.cnst.Flt = strtof(ast->crrnt->bffr.bffr, NULL);
	}
	else if (ast->crrnt->type == eT_Dbl)
	{
		nde->info = eNT_Dbl;
		nde->dat.cnst.Dbl = strtold(ast->crrnt->bffr.bffr, NULL);
	}
	else if (ast->crrnt->type == eT_Txt)
	{
		nde->info = eNT_Txt;
		nde->dat.cnst.Txt = ast->crrnt->bffr.bffr;
	}
	else
	{
		_tAst_printErr_(ast, "Invalid token", nde);
		_tAstNd_destruct_(nde);
		return NULL;
	}
	tAst_nxt_s(ast, nde);
	return nde;
}
static struct _tAstNde_ *_tAst_tMultv_construct_(tAst *ast)
{
	struct _tAstNde_ *nde = _tAst_uCnst_construct_(ast);
	if (nde == NULL) return NULL;
	while (ast->crrnt->type == eT_Mult || ast->crrnt->type == eT_Div)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.unry.lhs = nde;
		nde = tmp;
		nde->type = eN_Multv;
		if (ast->crrnt->type == eT_Mult) nde->info = eNT_Mult;
		else if (ast->crrnt->type == eT_Div) nde->info = eNT_Div;
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
	while (ast->crrnt->type == eT_Add || ast->crrnt->type == eT_Sub)
	{
		struct _tAstNde_ *tmp = _tAstNde_construct_(ast);
		if (tmp == NULL) return NULL;
		tmp->dat.unry.lhs = nde;
		nde = tmp;
		nde->type = eN_Addtv;
		if (ast->crrnt->type == eT_Add) nde->info = eNT_Add;
		else if (ast->crrnt->type == eT_Sub) nde->info = eNT_Sub;
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
void _tAst_print_(struct _tAstNde_ *nd, long long unsigned indent)
{
	if (nd == NULL) return;

	for (long long unsigned i = 0; i < indent; i++)
	{
		printf("  ");
	}

	switch (nd->type)
	{
	case eN_Cnst:
		switch (nd->info)
		{
		case eNT_Int:
			printf("eNT_Int: %llu\n", nd->dat.cnst.Llu);
			break;
		case eNT_Flt:
			printf("eNT_Flt: %f\n", nd->dat.cnst.Flt);
			break;
		case eNT_Dbl:
			printf("eNT_Dbl: %lf\n", nd->dat.cnst.Dbl);
			break;
		case eNT_Txt:
			printf("eNT_Txt: %s\n", nd->dat.cnst.Txt);
			break;
		default:
			printf("Unknown eN_Cnst\n");
		}
		break;
	case eN_Multv:
		printf("eN_Multv: ");
		switch (nd->info)
		{
		case eNT_Mult:
			printf("*\n");
			break;
		case eNT_Div:
			printf("/\n");
			break;
		default:
			printf("Unknown eN_Multv\n");
		}
		_tAst_print_(nd->dat.unry.lhs, indent + 1);
		_tAst_print_(nd->dat.unry.rhs, indent + 1);
		break;
	case eN_Addtv:
		printf("eN_Addtv: ");
		switch (nd->info)
		{
		case eNT_Add:
			printf("+\n");
			break;
		case eNT_Sub:
			printf("-\n");
			break;
		default:
			printf("Unknown eN_Addtv\n");
		}
		_tAst_print_(nd->dat.unry.lhs, indent + 1);
		_tAst_print_(nd->dat.unry.rhs, indent + 1);
		break;
	case eN_Err:
		fprintf(stderr, "Err: Unknown Ast node.\n");
		break;
	default:
		fprintf(stderr, "Err: Unknown Ast node type: %d\n", nd->type);
	}
}
bool _Ast_parse_(tAst *ast)
{
	if (_tAst_nxt_(ast))
	{
		_tAst_printErr_(ast, "No tokens provided in input file", NULL);
		return true;
	}
	ast->rt = _tAst_tAddtv_construct_(ast);
	if (ast->rt == NULL) return true;
	if (ast->tkns->len >= ast->idx)
	{
		_tAst_printErr_(ast, "Unparsed tokens remaining", NULL);
		return true;
	}
	_tAst_print_(ast->rt, 0);
	return false;
}
