#include <stdio.h>
#include "../Brng/Optprcss.h"
#include "Scanning.h"
#include "Ast.h"
int main(int argc, char *argv[])
{
	tOptPrcss optPrcss = tOptPrcss_construct("collang", "<Compiler options> [Input files]");
	//tOptPrcss_addOpt(optPrcss, "Arch", "Specifies the target architecture (x86, ARM, x86_64, ARM64)", false, 1, tOT_T, "--arch");
	tOptPrcss_addOpt(optPrcss, "Output", "Specifies the target output file name", false, 1, tOT_T, "--output", "-o");
	tOptPrcss_process_s(optPrcss, argc, argv);
	if (optPrcss.argc == 0LLU)
	{
		fprintf(stderr, "Err: No input files provided.\n");
		tOptPrcss_destruct(optPrcss);
		return 1;
	}
	for (long long unsigned idx = 0LLU; idx < optPrcss.argc; ++idx)
	{
		FILE *inputFile = fopen(optPrcss.argv[idx], "rb");
		if (inputFile == NULL)
		{
			fprintf(stderr, "Err: Invalid input file '%s'.\n", optPrcss.argv[idx]);
			tOptPrcss_destruct(optPrcss);
			return 1;
		}
		tScnnr fileScanner = tScnnr_construct(optPrcss.argv[idx], inputFile);
		if (tScnnr_parse(fileScanner))
		{
			tScnnr_destruct(fileScanner);
			tOptPrcss_destruct(optPrcss);
			return 1;
		}
		for (long long unsigned idx = 0; idx < fileScanner.tkns.len; ++idx)
		{
			if (fileScanner.tkns.tkns[idx].bffr.cpcty == 0) fprintf(stdout, "_tTkn_(%d, %llu, %llu)\n", fileScanner.tkns.tkns[idx].type, fileScanner.tkns.tkns[idx].ln, fileScanner.tkns.tkns[idx].pos);
			else fprintf(stdout, "_tTkn_(%d, %llu, %llu, %s)\n", fileScanner.tkns.tkns[idx].type, fileScanner.tkns.tkns[idx].ln, fileScanner.tkns.tkns[idx].pos, fileScanner.tkns.tkns[idx].bffr.bffr);
		}
		tAst ast = tAst_construct(fileScanner);
		if (!tAst_parse(ast))
		{
			printf("_tAstNd_(%u, %llu, %llu, %llu)\n", ast.rt->type, ast.rt->ln, ast.rt->pos, ast.rt->dat.cnst.dat.Llu);
		}
		tAst_destruct(ast);
		tScnnr_destruct(fileScanner);
		fclose(inputFile);
	}
	tOptPrcss_destruct(optPrcss);
	return 0;
}