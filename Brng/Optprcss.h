#ifndef OPTPRCSS_H_
#define OPTPRCSS_H_
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef enum
{
	tOT_N,
	tOT_LLU,
	tOT_LL,
	tOT_D,
	tOT_IU,
	tOT_I,
	tOT_F,
	tOT_SU,
	tOT_S,
	tOT_CU,
	tOT_C,
	tOT_B,
	tOT_T
}
_tOptTypes;
char const *_tOptTypeStrs[] = {
	"None",
	"long long unsigned",
	"long long",
	"double",
	"int unsigned",
	"int",
	"float",
	"short unsigned",
	"short",
	"char unsigned",
	"char",
	"bool",
	"char const *"
};
typedef union
{
	long long unsigned LLU;
	long long LL;
	char *T;
	double D;
	int unsigned IU;
	int I;
	float F;
	short unsigned SU;
	short S;
	char unsigned CU;
	char C;
	bool B;
}
tOptDat;
typedef struct
{
	char const *idntf;
	char const **nam;
	long long unsigned namLen;
	bool req;
	tOptDat *dat;
	long long unsigned datLen;
	_tOptTypes datType;
	char const *dsc;
	bool prs;
}
_tCmdOpt, *tOpt;
void _tCmdOpt_destruct_(_tCmdOpt *cmdOpt)
{
	if (cmdOpt->idntf != NULL)
	{
		free((void *)cmdOpt->idntf);
		cmdOpt->idntf = NULL;
	}
	if (cmdOpt->nam != NULL)
	{
		for (long long unsigned idx = 0LLU; idx < cmdOpt->namLen; ++idx)
		{
			if (cmdOpt->nam[idx] != NULL)
			{
				free((void *)cmdOpt->nam[idx]);
				cmdOpt->nam[idx] = NULL;
			}
		}
		cmdOpt->namLen = 0LLU;
		free((void *)cmdOpt->nam);
		cmdOpt->nam = NULL;
	}
	cmdOpt->req = 0;
	if (cmdOpt->datType != tOT_N)
	{
		if (cmdOpt->dat != NULL)
		{
			for (long long unsigned idx = 0LLU; idx < cmdOpt->datLen; ++idx)
			{
				cmdOpt->dat[idx].LLU = 0LLU;
			}
			cmdOpt->datLen = 0LLU;
			free((void *)cmdOpt->dat);
		}
		cmdOpt->datType = tOT_N;
	}
	cmdOpt->dat = NULL;
	if (cmdOpt->dsc != NULL)
	{
		free((void *)cmdOpt->dsc);
		cmdOpt->dsc = NULL;
	}
}
#define _tCmdOpt_destruct(cmdOpt) _tCmdOpt_destruct_(&cmdOpt)
typedef struct _tUsgLl_
{
	struct _tUsgLl_ *nxt;
	char const *usg;
}
*_tUsgLl;
_tUsgLl _tUsgLl_construct(char const *usg)
{
	_tUsgLl usgN = calloc(1, sizeof(struct _tUsgLl_));
	if (usgN == NULL)
	{
		fprintf(stderr, "Err: Could not allocate memory for '_tUsgLl'.\n");
		return NULL;
	}
	usgN->usg = calloc(strlen(usg) + 1LLU, sizeof(char const));
	if (usgN->usg == NULL)
	{
		fprintf(stderr, "Err: Could not allocate memory for '_tUsgLl.usg'.\n");
		free((void *)usgN);
		return NULL;
	}
	strcpy((char *)usgN->usg, usg);
	usgN->nxt = NULL;
	return usgN;
}
typedef struct
{
	char const *progNam;
	_tCmdOpt *opts;
	long long unsigned cpcty;
	long long unsigned len;
	_tUsgLl usg;
	long long unsigned minArg;
	long long unsigned argc;
	char const **argv;
	long long unsigned argCpcty;
}
tOptPrcss;
void _tOptPrcss_destruct_(tOptPrcss *optPrcss)
{
	for (long long unsigned idx = 0LLU; idx < optPrcss->len; ++idx)
	{
		_tCmdOpt_destruct(optPrcss->opts[idx]);
	}
	if (optPrcss->opts != NULL)
	{
		free((void *)optPrcss->opts);
		optPrcss->opts = NULL;
	}
	optPrcss->cpcty = 0LLU;
	optPrcss->len = 0LLU;
	if (optPrcss->progNam != NULL)
	{
		free((void *)optPrcss->progNam);
		optPrcss->progNam = NULL;
	}
	if (optPrcss->usg != NULL)
	{
		do
		{
			_tUsgLl tmpUsg = optPrcss->usg;
			optPrcss->usg = optPrcss->usg->nxt;
			if (tmpUsg->nxt != NULL)
			{
				tmpUsg->nxt = NULL;
			}
			if (tmpUsg->usg != NULL)
			{
				free((void *)tmpUsg->usg);
				tmpUsg->usg = NULL;
			}
			free((void *)tmpUsg);
		}
		while (optPrcss->usg != NULL);
		free((void *)optPrcss->usg);
		optPrcss->usg = NULL;
	}
	if (optPrcss->argv != NULL)
	{
		free((void *)optPrcss->argv);
		optPrcss->argv = NULL;
	}
	optPrcss->argc = 0LLU;
	optPrcss->argCpcty = 0LLU;
}
#define tOptPrcss_destruct(optPrcss) _tOptPrcss_destruct_(&optPrcss)
void _tOptPrcss_addOpt_(tOptPrcss *optPrcss, char const *idntf, char const *dsc, bool req, long long unsigned argCnt, _tOptTypes datType, char const *nams, ...)
{
	if (optPrcss->cpcty == 0LLU)
	{
		optPrcss->opts = calloc(++optPrcss->cpcty, sizeof(_tCmdOpt));
		if (optPrcss->opts == NULL)
		{
			fprintf(stderr, "Err: Could not allocate memory for first opt.\n");
			_tOptPrcss_destruct_(optPrcss);
			return;
		}
	}
	else if (optPrcss->cpcty == optPrcss->len)
	{
		optPrcss->cpcty <<= 1LLU;
		void *newPtr = realloc(optPrcss->opts, optPrcss->cpcty * sizeof(_tCmdOpt));
		if (newPtr == NULL)
		{

			fprintf(stderr, "Err: Could not reallocate memory for opt.\n");
			_tOptPrcss_destruct_(optPrcss);
			return;
		}
		optPrcss->opts = newPtr;
	}
	_tCmdOpt cmdOpt = (_tCmdOpt){0};
	cmdOpt.idntf = (char const *)calloc(strlen(idntf) + 1LLU, sizeof(char const));
	if (cmdOpt.idntf == NULL)
	{
		fprintf(stderr, "Err: Could not allocate memory for 'cmdOpt.idntf'.\n");
		_tCmdOpt_destruct(cmdOpt);
		return;
	}
	strcpy((char *)cmdOpt.idntf, idntf);
	cmdOpt.nam = NULL;
	cmdOpt.namLen = 0LLU;
	cmdOpt.req = req;
	cmdOpt.datType = datType;
	cmdOpt.datLen = argCnt;
	if (cmdOpt.datLen == 0LLU && cmdOpt.datType != tOT_N)
	{
		fprintf(stderr, "Err: Need at least a value of '1' for 'cmdOpt.datLen'.\n");
		_tCmdOpt_destruct(cmdOpt);
		return;
	}
	else if (cmdOpt.datLen != 0LLU && cmdOpt.datType == tOT_N)
	{
		fprintf(stderr, "Err: Need a value of '0' for 'cmdOpt.datLen' when type is None.\n");
		_tCmdOpt_destruct(cmdOpt);
		return;
	}
	else if (cmdOpt.datLen > 0)
	{
		cmdOpt.dat = (tOptDat *)calloc(cmdOpt.datLen, sizeof(tOptDat));
		if (cmdOpt.dat == NULL)
		{
			fprintf(stderr, "Err: Could not allocate memory for 'cmdOpt.dat'.\n");
			_tCmdOpt_destruct(cmdOpt);
			return;
		}
	}
	else cmdOpt.dat = NULL;
	cmdOpt.dsc = (char const *)calloc(strlen(dsc) + 1LLU, sizeof(char const));
	if (cmdOpt.dsc == NULL)
	{
		fprintf(stderr, "Err: Could not allocate memory for 'cmdOpt.dsc'.\n");
		_tCmdOpt_destruct(cmdOpt);
		return;
	}
	strcpy((char *)cmdOpt.dsc, dsc);
	va_list param;
	va_start(param, nams);
	while (nams != NULL)
	{
		char const **newPtr = NULL;
		if (cmdOpt.nam == NULL)
		{
			newPtr = (char const **)calloc((cmdOpt.namLen + 1LLU), sizeof(char const *));
		}
		else
		{
			newPtr = (char const **)realloc(cmdOpt.nam, (cmdOpt.namLen + 1LLU) * sizeof(char const *));
		}
		if (newPtr == NULL)
		{
			fprintf(stderr, "Err: Could not allocate memory for 'cmdOpt.nam'.\n");
			va_end(param);
			_tCmdOpt_destruct(cmdOpt);
			return;
		}
		cmdOpt.nam = newPtr;
		cmdOpt.nam[cmdOpt.namLen] = (char const *)calloc(strlen(nams) + 1LLU, sizeof(char const));
		if (cmdOpt.nam[cmdOpt.namLen] == NULL)
		{
			fprintf(stderr, "Err: Could not allocate memory for 'cmdOpt.nam[%llu]'.\n", cmdOpt.namLen);
			va_end(param);
			_tCmdOpt_destruct(cmdOpt);
			return;
		}
		strcpy((char *)cmdOpt.nam[cmdOpt.namLen++], nams);
		nams = va_arg(param, char const *);
	}
	va_end(param);
	if (cmdOpt.namLen == 0LLU)
	{
		fprintf(stderr, "Err: Need at least one name for a command line argument.\n");
		_tCmdOpt_destruct(cmdOpt);
		return;
	}
	optPrcss->opts[optPrcss->len] = cmdOpt;
	++optPrcss->len;
	if (req == 1)
	{
		optPrcss->minArg += argCnt + 1LLU;
	}
}
#define tOptPrcss_addOpt(optPrcss, idntf, dsc, req, argCnt, datType, ...) _tOptPrcss_addOpt_(&optPrcss, idntf, dsc, req, argCnt, datType, __VA_ARGS__, NULL)
tOptPrcss _tOptPrcss_construct_(char const *progNam, char const *usgs, ...)
{
	tOptPrcss optPrcss = (tOptPrcss){.progNam = NULL, .opts = NULL, .cpcty = 0LLU, .len = 0LLU, .usg = NULL, .minArg = 0LLU, .argc = 0LLU, .argv = NULL, .argCpcty = 0LLU};
	optPrcss.progNam = calloc(strlen(progNam) + 1LLU, sizeof(char const));
	if (optPrcss.progNam == NULL)
	{
		fprintf(stderr, "Err: Could not allocate memory for 'optPrcss.progNam'.\n");
		return optPrcss;
	}
	strcpy((char *)optPrcss.progNam, progNam);
	va_list param;
	va_start(param, usgs);
	while (usgs != NULL)
	{
		_tUsgLl newUsg = _tUsgLl_construct(usgs);
		if (newUsg == NULL)
		{
			fprintf(stderr, "Err: Could not allocate memory for 'optPrcss.usg'.\n");
			_tOptPrcss_destruct_(&optPrcss);
			return optPrcss;
		}
		newUsg->nxt = optPrcss.usg;
		optPrcss.usg = newUsg;
		usgs = va_arg(param, char const *);
	}
	_tOptPrcss_addOpt_(&optPrcss, "Help", "Prints the usage for the program.", false, 0LLU, tOT_N, "--help", NULL);
	return optPrcss;
}
#define tOptPrcss_construct(progNam, ...) _tOptPrcss_construct_(progNam, __VA_ARGS__, NULL)
void _tOptPrcss_printUsage_(tOptPrcss *optPrcss)
{
	printf("Usage: ");
	_tUsgLl tmpUsg = optPrcss->usg;
	while (tmpUsg != NULL)
	{
		printf("%s %s\n", optPrcss->progNam, tmpUsg->usg);
		if (tmpUsg->nxt != NULL)
		{
			printf("   Or: ");
		}
		tmpUsg = tmpUsg->nxt;
	}
	for (long long unsigned idx = 0LLU; idx < optPrcss->len; ++idx)
	{
		printf("\t%s:\t%s", optPrcss->opts[idx].idntf, optPrcss->opts[idx].nam[0]);
		for (long long unsigned jdx = 1LLU; jdx < optPrcss->opts[idx].namLen; ++jdx)
		{
			printf(", %s", optPrcss->opts[idx].nam[jdx]);
		}
		printf("\n\t\t%s\n", optPrcss->opts[idx].dsc);
		printf("\t\t\tRequired: %s\n", optPrcss->opts[idx].req == 0 ? "False" : "True");
		printf("\t\t\tType: %s\n", _tOptTypeStrs[optPrcss->opts[idx].datType]);
		printf("\t\t\tCount: %llu\n", optPrcss->opts[idx].datLen);
	}
}
#define tOptPrcss_printUsage(optPrcss) _tOptPrcss_printUsage_(&optPrcss)
int _tOptPrcss_process_(tOptPrcss *optPrcss, int cnt, char *val[])
{
	if ((long long unsigned)(cnt - 1) < optPrcss->minArg)
	{
		_tOptPrcss_printUsage_(optPrcss);
		_tOptPrcss_destruct_(optPrcss);
		fprintf(stderr, "Err: Missing required arguments.\n");
		return 1;
	}
	for (int idx = 1; idx < cnt; ++idx)
	{
		bool brk = 0;
		long long unsigned optIdx = (long long unsigned) - 1;
		for (long long unsigned jdx = 0LLU; jdx < optPrcss->len; ++jdx)
		{
			for (long long unsigned kdx = 0LLU; kdx < optPrcss->opts[jdx].namLen; ++kdx)
			{
				if (strcmp(optPrcss->opts[jdx].nam[kdx], val[idx]) == 0)
				{
					optIdx = jdx;
					break;
				}
			}
			if (optIdx != (long long unsigned) - 1)
			{
				if (optIdx == 0)
				{
					_tOptPrcss_printUsage_(optPrcss);
					_tOptPrcss_destruct_(optPrcss);
					return 1;
				}
				brk = 1;
				break;
			}
		}
		if (brk == 0)
		{
			if (val[idx][0] == '-')
			{
				fprintf(stderr, "Err: Unknown argument '%s' at position '%d'.\n", val[idx], idx);
				_tOptPrcss_destruct_(optPrcss);
				return 1;
			}
			if (optPrcss->argCpcty == 0LLU)
			{

				optPrcss->argv = calloc(++optPrcss->argCpcty, sizeof(char const *));
				if (optPrcss->argv == NULL)
				{
					fprintf(stderr, "Err: Could not allocate memory for argv.\n");
					_tOptPrcss_destruct_(optPrcss);
					return 1;
				}
			}
			else if (optPrcss->argc == optPrcss->argCpcty)
			{
				optPrcss->argCpcty <<= 1LLU;
				char const **newPtr = realloc(optPrcss->argv, optPrcss->argCpcty * sizeof(char const *));
				if (newPtr == NULL)
				{
					fprintf(stderr, "Err: Could not reallocate memory for argv.\n");
					_tOptPrcss_destruct_(optPrcss);
					return 1;
				}
				optPrcss->argv = newPtr;
			}
			optPrcss->argv[optPrcss->argc] = val[idx];
			optPrcss->argc++;
			continue;
		}
		if (optPrcss->opts[optIdx].prs == true)
		{
			fprintf(stderr, "Err: Redefition of opt '%s' at position %d.\n", optPrcss->opts[optIdx].idntf, idx);
			_tOptPrcss_destruct_(optPrcss);
			return 1;
		}
		if (optPrcss->opts[optIdx].datType == tOT_N)
		{
			optPrcss->opts[optIdx].prs = true;
		}
		else
		{
			for (long long unsigned jdx = 0LLU; jdx < optPrcss->opts[optIdx].datLen; ++jdx)
			{
				char tmp = '\0';
				char *invalidChar = &tmp;
				switch (optPrcss->opts[optIdx].datType)
				{
				case tOT_N: break;
				case tOT_LLU:
					optPrcss->opts[optIdx].dat[jdx].LLU = strtoull(val[idx + jdx + 1LLU], &invalidChar, 10);
					break;
				case tOT_LL:
					optPrcss->opts[optIdx].dat[jdx].LL = strtoll(val[idx + jdx + 1LLU], &invalidChar, 10);
					break;
				case tOT_D:
					optPrcss->opts[optIdx].dat[jdx].D = strtod(val[idx + jdx + 1LLU], &invalidChar);
					break;
				case tOT_IU:
					optPrcss->opts[optIdx].dat[jdx].IU = (int unsigned)strtoul(val[idx + jdx + 1LLU], &invalidChar, 10);
					break;
				case tOT_I:
					optPrcss->opts[optIdx].dat[jdx].I = (int)strtol(val[idx + jdx + 1LLU], &invalidChar, 10);
					break;
				case tOT_F:
					optPrcss->opts[optIdx].dat[jdx].F = strtof(val[idx + jdx + 1LLU], &invalidChar);
					break;
				case tOT_SU:
					optPrcss->opts[optIdx].dat[jdx].SU = (short unsigned)strtoul(val[idx + jdx + 1LLU], &invalidChar, 10);
					break;
				case tOT_S:
					optPrcss->opts[optIdx].dat[jdx].S = (short)strtol(val[idx + jdx + 1LLU], &invalidChar, 10);
					break;
				case tOT_CU:
					optPrcss->opts[optIdx].dat[jdx].CU = (char unsigned)val[idx + jdx + 1LLU][0];
					break;
				case tOT_C:
					optPrcss->opts[optIdx].dat[jdx].C = val[idx + jdx + 1LLU][0];
					break;
				case tOT_B:
					if (strncmp(val[idx + jdx + 1LLU], "True", 5LLU) == 0) optPrcss->opts[optIdx].dat[jdx].B = 1;
					else if (strncmp(val[idx + jdx + 1LLU], "False", 6LLU) == 0) optPrcss->opts[optIdx].dat[jdx].B = 0;
					else invalidChar = val[0];
					break;
				case tOT_T:
					optPrcss->opts[optIdx].dat[jdx].T = val[idx + jdx + 1LLU];
					break;
				default:
					fprintf(stderr, "Err: datType not implemented.\n");
					_tOptPrcss_destruct_(optPrcss);
					return 1;
				}
				if (invalidChar[0] != '\0')
				{
					fprintf(stderr, "Err: Invalid argument at position %llu.\n", idx + jdx + 1LLU);
					_tOptPrcss_printUsage_(optPrcss);
					_tOptPrcss_destruct_(optPrcss);
					return 1;
				}
			}
		}
		idx += optPrcss->opts[optIdx].datLen;
		optPrcss->opts[optIdx].prs = true;
		optIdx = (long long unsigned) - 1;
		brk = 0;
	}
	return 0;
}
#define tOptPrcss_process(optPrcss, argc, argv) _tOptPrcss_process_(&optPrcss, argc, argv)
#define tOptPrcss_process_s(optPrcss, argc, argv) do{long long unsigned err=tOptPrcss_process(optPrcss,argc,argv);if(err!=0LLU){tOptPrcss_destruct(optPrcss);return err;}}while(0)
_tCmdOpt *_tOptPrcss_opt_(tOptPrcss *optPrcss, char const *idntf)
{
	for (long long unsigned idx = 0LLU; idx < optPrcss->len; ++idx)
	{
		if (strcmp(optPrcss->opts[idx].idntf, idntf) == 0)
		{
			return &optPrcss->opts[idx];
		}
	}
	fprintf(stderr, "Err: Could not find _tCmdOpt with idntf '%s'.\n", idntf);
	return NULL;
}
#define tOptPrcss_opt(optPrcss, idntf) _tOptPrcss_opt_(&optPrcss, idntf)
#endif/*OPTPRCSS_H_*/