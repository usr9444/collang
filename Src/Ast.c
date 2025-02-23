#include <stdlib.h>
#include <stdbool.h>
#include "Scanning.h"
#include "Ast.h"
tAst _tAst_construct_(tScnnr *scnnr)
{
	return (tAst)
	{
		.rt = NULL, .tkns = &scnnr->tkns, .idx = 0LLU, .crrnt = NULL
	};
}
void _tAst_destruct_(tAst *ast)
{
	//Descend
	if (ast->rt != NULL)
	{
		free(ast->rt);
	}
	ast->rt = NULL;
	ast->tkns = NULL;
	ast->idx = 0LLU;
}
void _tAst_nxt_(tAst *ast)
{
	if (ast->idx == ast->tkns->len) return;
	ast->crrnt = &ast->tkns->tkns[ast->idx++];
}
struct _tAstNd_ *_tAst_tCnst_(tAst *ast)
{
	struct _tAstNd_ *nd = calloc(1, sizeof(struct _tAstNd_));
	if (nd == NULL)
	{
		fprintf(stderr, "Err: Could not allocate memory for _tAstNd_.\n");
		return NULL;
	}
	if (ast->crrnt->type == eT_B10)
	{
		nd->type = eN_Int;
		nd->dat.cnst.dat.Llu = strtoull(ast->crrnt->bffr.bffr, NULL, 10);
	}
	nd->ln = ast->crrnt->ln;
	nd->pos = ast->crrnt->pos;
	_tAst_nxt_(ast);
	return nd;
}
bool _Ast_parse_(tAst *ast)
{
	_tAst_nxt_(ast);
	ast->rt = _tAst_tCnst_(ast);
	return false;
}
