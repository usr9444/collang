#include <stdio.h>
#include "../Brng/Optprcss.h"
#include "Scanning.h"
int main(int argc, char *argv[])
{
	tOptPrcss optPrcss = tOptPrcss_construct("collang", "<Compiler options> [Input files]");
	//tOptPrcss_addOpt(optPrcss, "Arch", "Specifies the target architecture (x86, ARM, x86_64, ARM64)", false, 1, tOT_T, "--arch");
	tOptPrcss_process_s(optPrcss, argc, argv);
	if (optPrcss.argc == 0LLU)
	{
		fprintf(stderr, "Err: No input files provided.\n");
		tOptPrcss_destruct(optPrcss);
		return 1;
	}
	else
	{
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
				fprintf(stderr, "Err: Error parsing file '%s'.\n", optPrcss.argv[idx]);
				tOptPrcss_destruct(optPrcss);
				return 1;
			}
			fclose(inputFile);
		}
	}
	tOptPrcss_destruct(optPrcss);
	return 0;
}
