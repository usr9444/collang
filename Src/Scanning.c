#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Scanning.h"
static struct _tArrBffr_ _tArrBffr_construct_(void)
{
	return (struct _tArrBffr_)
	{
		.bffr = NULL, .cpcty = 0LLU, .len = 0LLU
	};
}
static void _tArrBffr_destruct_(struct _tArrBffr_ *bffr)
{
	if (bffr->bffr != NULL) free((void *)bffr->bffr);
	bffr->cpcty = 0LLU;
	bffr->len = 0LLU;
	bffr->bffr = NULL;
}
static int _tArrBffr_addCh_(struct _tArrBffr_ *bffr, char const ch)
{
	if (bffr->cpcty == 0LLU)
	{
		bffr->bffr = (char *)calloc((++bffr->cpcty) + 1, sizeof(char));
		if (bffr->bffr == NULL)
		{
			fprintf(stderr, "Err: Could not allocate memory for _tArrBffr_.\n");
			return 1;
		}
	}
	else if (bffr->len == bffr->cpcty)
	{
		char *newPtr = (char *)realloc(bffr->bffr, sizeof(char) * ((bffr->cpcty << 1) + 1));
		if (newPtr == NULL)
		{
			fprintf(stderr, "Err: Could not reallocate memory for _tArrBffr_.\n");
			return 1;
		}
		bffr->bffr = newPtr;
		bffr->cpcty <<= 1;
	}
	bffr->bffr[bffr->len++] = ch;
	bffr->bffr[bffr->len] = '\0';
	return 0;
}
static struct _tTkns_ _tTkns_construct_(void)
{
	return (struct _tTkns_)
	{
		.tkns = NULL, .cpcty = 0LLU, .len = 0LLU
	};
}
static void _tTkns_destruct_(struct _tTkns_ *tkns)
{
	if (tkns->tkns != NULL)
	{
		for (long long unsigned idx = 0; idx < tkns->len; ++idx)
		{
			if (tkns->tkns[idx].bffr.cpcty != 0LLU)
			{
				_tArrBffr_destruct_(&tkns->tkns[idx].bffr);
			}
		}
		free((void *)tkns->tkns);
	}
}
static int _tTkns_addTkn_(struct _tTkns_ *tkns, enum eTkns type, long long unsigned const ln, long long unsigned const pos, struct _tArrBffr_ *bffr)
{
	struct _tTkn_ tkn = (struct _tTkn_){.pos = pos, .ln = ln, .type = type};
	tkn.bffr = (struct _tArrBffr_){.bffr = NULL, .cpcty = 0LLU, .len = 0LLU};
	if (bffr != NULL)
	{
		tkn.bffr = *bffr;
	}
	if (tkns->cpcty == 0LLU)
	{
		tkns->tkns = (struct _tTkn_ *)calloc(++tkns->cpcty, sizeof(struct _tTkn_));
		if (tkns->tkns == NULL)
		{
			fprintf(stderr, "Err: Could not allocate memory for _tTkns_.\n");
			return 1;
		}
	}
	else if (tkns->len == tkns->cpcty)
	{
		struct _tTkn_ *newPtr = (struct _tTkn_ *)realloc(tkns->tkns, sizeof(struct _tTkn_) * (tkns->cpcty << 1));
		if (newPtr == NULL)
		{
			fprintf(stderr, "Err: Could not reallocate memory for _tTkns_.\n");
			return 1;
		}
		tkns->tkns = newPtr;
		tkns->cpcty <<= 1;
	}
	tkns->tkns[tkns->len++] = tkn;
	return 0;
}
tScnnr tScnnr_construct(char const *fileName, FILE *filePtr)
{
	tScnnr scnnr;
	scnnr.ch = EOF;
	scnnr.crrnt = 0;
	scnnr.fileName = fileName;
	scnnr.ln = 1;
	scnnr.strt = 0;
	scnnr.tkns = _tTkns_construct_();
	scnnr.filePtr = filePtr;
	return scnnr;
}
void _tScnnr_destruct_(tScnnr *scnnr)
{
	_tTkns_destruct_(&scnnr->tkns);
}
static void _tScnnr_printErr_(tScnnr const *scnnr, char const *hnt)
{
	fprintf(stderr, "Err: Failed while parsing token in file '%s' on line %llu at pos %llu (started at pos %llu).\n", scnnr->fileName, scnnr->ln, scnnr->crrnt, scnnr->strt);
	if (hnt != NULL) fprintf(stderr, "Exp: %s.\n", hnt);
}
static int _tScnnr_addTkn_(tScnnr *scnnr, enum eTkns type, struct _tArrBffr_ *bffr)
{
	int err = _tTkns_addTkn_(&scnnr->tkns, type, scnnr->ln, scnnr->strt, bffr);
	if (err != 0) _tScnnr_printErr_(scnnr, "Could not allocate memory for token");
	return err;
}
#define tScnnr_addTkn_s(scnnr, type, lit) \
	{ \
		int err = _tScnnr_addTkn_(scnnr, type, lit); \
		if (err != 0) \
		{ \
			_tScnnr_destruct_(scnnr); \
			return err; \
		} \
	}
static char _tScnnr_nxt_(tScnnr *scnnr)
{
	++scnnr->crrnt;
	scnnr->ch = getc(scnnr->filePtr);
	return scnnr->ch;
}
static char _tScnnr_ahd_(tScnnr const *scnnr)
{
	char const ch = getc(scnnr->filePtr);
	ungetc(ch, scnnr->filePtr);
	return ch;
}
static char _tScnnr_ahd2_(tScnnr const *scnnr)
{
	char const ch1 = getc(scnnr->filePtr);
	char ch2 = EOF;
	if (ch1 != EOF)
	{
		ch2 = getc(scnnr->filePtr);
		ungetc(ch2, scnnr->filePtr);
	}
	ungetc(ch1, scnnr->filePtr);
	return ch2;
}
inline static bool _isAlpha_(char const ch)
{
	return (ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a');
}
inline static bool _isB2Digit_(char const ch)
{
	return ch == '1' || ch == '0';
}
inline static bool _isB8Digit_(char const ch)
{
	return ch <= '7' && ch >= '0';
}
inline static bool _isB10Digit_(char const ch)
{
	return ch <= '9' && ch >= '0';
}
inline static bool _isB16Digit_(char const ch)
{
	return ((ch <= 'F' && ch >= 'A') || (ch <= 'f' && ch >= 'a')) || _isB10Digit_(ch);
}
inline static char _dgtsToChr_(char *dgts, const int unsigned bas)
{
	static char *invalidChr = NULL;
	return (char)strtoul(dgts, &invalidChr, bas);
}
inline static bool _isNumTypeHint_(char const ch)
{
	return ch == 'B' || ch == 'b' || ch == 'O' || ch == 'o' || ch == 'H' || ch == 'h' || ch == 'F' || ch == 'f' || ch == '.';
}
inline static bool _isIdChar_(char const ch)
{
	return _isAlpha_(ch) || ch == '_';
}
static int _tScnnr_parseWord_(tScnnr *scnnr)
{
	struct _tArrBffr_ bffr = _tArrBffr_construct_();
	if (_tArrBffr_addCh_(&bffr, scnnr->ch) != 0)
	{
		return 1;
	}
	for (char ch = _tScnnr_ahd_(scnnr); _isIdChar_(ch) || _isB10Digit_(ch); ch = _tScnnr_ahd_(scnnr))
	{
		int err = _tArrBffr_addCh_(&bffr, _tScnnr_nxt_(scnnr));
		if (err != 0)
		{
			_tArrBffr_destruct_(&bffr);
			return err;
		}
	}
	enum eTkns tknType = eT_Err;
	if (strncmp(bffr.bffr, "and", (4 >= bffr.len) ? 4 : bffr.len) == 0) tknType = eT_And;
	else if (strncmp(bffr.bffr, "for", (4 >= bffr.len) ? 4 : bffr.len) == 0) tknType = eT_For;
	else if (strncmp(bffr.bffr, "or", (3 >= bffr.len) ? 3 : bffr.len) == 0) tknType = eT_Or;
	else if (strncmp(bffr.bffr, "if", (3 >= bffr.len) ? 3 : bffr.len) == 0) tknType = eT_If;
	else if (strncmp(bffr.bffr, "else", (5 >= bffr.len) ? 5 : bffr.len) == 0) tknType = eT_Else;
	else if (strncmp(bffr.bffr, "while", (6 >= bffr.len) ? 6 : bffr.len) == 0) tknType = eT_While;
	else if (strncmp(bffr.bffr, "ret", (4 >= bffr.len) ? 4 : bffr.len) == 0) tknType = eT_Ret;
	else if (strncmp(bffr.bffr, "brk", (4 >= bffr.len) ? 4 : bffr.len) == 0) tknType = eT_Brk;
	else if (strncmp(bffr.bffr, "None", (5 >= bffr.len) ? 5 : bffr.len) == 0) tknType = eT_None;
	else if (strncmp(bffr.bffr, "True", (5 >= bffr.len) ? 5 : bffr.len) == 0) tknType = eT_True;
	else if (strncmp(bffr.bffr, "False", (6 >= bffr.len) ? 6 : bffr.len) == 0) tknType = eT_False;
	else if (strncmp(bffr.bffr, "fnc", (4 >= bffr.len) ? 4 : bffr.len) == 0) tknType = eT_Fnc;
	else if (strncmp(bffr.bffr, "infr", (5 >= bffr.len) ? 5 : bffr.len) == 0) tknType = eT_Infr;
	else if (strncmp(bffr.bffr, "rmbr", (5 >= bffr.len) ? 5 : bffr.len) == 0) tknType = eT_Rmbr;
	else tknType = eT_Id;
	if (tknType == eT_Id) return _tScnnr_addTkn_(scnnr, tknType, &bffr);
	else
	{
		_tArrBffr_destruct_(&bffr);
		return _tScnnr_addTkn_(scnnr, tknType, &bffr);
	}
}
static int _tScnnr_parseStrng_(tScnnr *scnnr)
{
	struct _tArrBffr_ bffr = _tArrBffr_construct_();
	for (char ch = _tScnnr_ahd_(scnnr); ch != '\''; ch = _tScnnr_ahd_(scnnr))
	{
		if (ch == '\n' || ch == '\r' || ch == EOF)
		{
			_tScnnr_printErr_(scnnr, "Unterminated string");
			_tArrBffr_destruct_(&bffr);
			return 1;
		}
		if (ch == '\\')
		{
			_tScnnr_nxt_(scnnr);
			char const chck = _tScnnr_ahd_(scnnr);
			if (chck == '0') ch = '\0';
			else if (chck == 'a') ch = '\a';
			else if (chck == 'b') ch = '\b';
			else if (chck == 't') ch = '\t';
			else if (chck == 'n') ch = '\n';
			else if (chck == 'v') ch = '\v';
			else if (chck == 'f') ch = '\f';
			else if (chck == 'r') ch = '\r';
			else if (chck == '\\' || chck == '\"' || chck == '\'') ch = chck;
			else if (chck == 'H' || chck == 'h')
			{
				_tScnnr_nxt_(scnnr);
				char dig[3LLU] = {0, 0, 0};
				for (long long unsigned idx = 0LLU; idx < 2LLU; ++idx)
				{
					char const chck2 = _tScnnr_nxt_(scnnr);
					if (_isB16Digit_(chck2) == false)
					{
						_tScnnr_printErr_(scnnr, "Invalid escaped hex digit");
						_tArrBffr_destruct_(&bffr);
						return 1;
					}
					dig[idx] = chck2;
				}
				ch = _dgtsToChr_(dig, 16LLU);
				goto Skp;
			}
			else if (chck == 'O' || chck == 'o')
			{
				_tScnnr_nxt_(scnnr);
				char dig[4LLU] = {0, 0, 0, 0};
				for (long long unsigned idx = 0LLU; idx < 3LLU; ++idx)
				{
					char const chck2 = _tScnnr_nxt_(scnnr);
					if (_isB8Digit_(chck2) == false)
					{
						_tScnnr_printErr_(scnnr, "Invalid escaped octal digit");
						_tArrBffr_destruct_(&bffr);
						return 1;
					}
					dig[idx] = chck2;
				}
				ch = _dgtsToChr_(dig, 8U);
				goto Skp;
			}
			//TODO(Me): Add unicode '\uHHHH' and '\UHHHHHHHH' support.
			else
			{
				_tScnnr_printErr_(scnnr, "Unknown escape character");
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
			_tScnnr_nxt_(scnnr);
Skp:	(void)0;
		}
		else ch = _tScnnr_nxt_(scnnr);
		int err = _tArrBffr_addCh_(&bffr, ch);
		if (err != 0)
		{
			_tArrBffr_destruct_(&bffr);
			return err;
		}
	}
	return _tScnnr_addTkn_(scnnr, eT_Txt, &bffr);
}
static int _tScnnr_parseNum_(tScnnr *scnnr)
{
	//TODO(Me): Add exponents.
	struct _tArrBffr_ bffr = _tArrBffr_construct_();
	bool hasDot = scnnr->ch == '.';
	enum eTkns tknType = hasDot == true ? eT_Dbl : eT_Err;
	int err = _tArrBffr_addCh_(&bffr, scnnr->ch);
	if (err != 0)
	{
		_tArrBffr_destruct_(&bffr);
		return err;
	}
	for (char ch = _tScnnr_ahd_(scnnr); _isB16Digit_(ch) || _isNumTypeHint_(ch); ch = _tScnnr_ahd_(scnnr))
	{
		if (tknType == eT_Flt && (ch == 'F' || ch == 'f'))
		{
			_tScnnr_printErr_(scnnr, "Multiple floating type declarations ('F') present in floating point number");
			_tArrBffr_destruct_(&bffr);
			return 1;
		}
		else if (ch == '.')
		{
			if (hasDot == true)
			{
				_tScnnr_printErr_(scnnr, "Multiple decimal points present in floating point number");
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
			hasDot = true;
			tknType = eT_Dbl;
		}
		else if (tknType == eT_Dbl && (ch == 'F' || ch == 'f')) tknType = eT_Flt;
		int err = _tArrBffr_addCh_(&bffr, _tScnnr_nxt_(scnnr));
		if (err != 0)
		{
			_tArrBffr_destruct_(&bffr);
			return err;
		}
	}
	if (tknType == eT_Err)
	{
		if (bffr.len < 3) tknType = eT_B10;
		else if (bffr.bffr[0] == '0')
		{
			if (bffr.bffr[1] == 'H' || bffr.bffr[1] == 'h') tknType = eT_B16;
			else if (bffr.bffr[1] == 'O' || bffr.bffr[1] == 'o') tknType = eT_B8;
			else if (bffr.bffr[1] == 'B' || bffr.bffr[1] == 'b') tknType = eT_B2;
			else tknType = eT_B10;
		}
	}
	for (long long unsigned idx = (tknType == eT_B16 || tknType == eT_B8 || tknType == eT_B2) ? 2LLU : 0LLU; idx < bffr.len; ++idx)
	{
		if (tknType == eT_B2)
		{
			if (_isB2Digit_(bffr.bffr[idx]) == false)
			{
				_tScnnr_printErr_(scnnr, "Invalid digit (not of base 2) in number");
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
		else if (tknType == eT_B8)
		{
			if (_isB8Digit_(bffr.bffr[idx]) == false)
			{
				_tScnnr_printErr_(scnnr, "Invalid digit (not of base 8) in number");
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
		else if (tknType == eT_B10)
		{
			if (_isB10Digit_(bffr.bffr[idx]) == false)
			{
				_tScnnr_printErr_(scnnr, "Invalid digit (not of base 10) in number");
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
		else if (tknType == eT_B16)
		{
			if (_isB16Digit_(bffr.bffr[idx]) == false)
			{
				_tScnnr_printErr_(scnnr, "Invalid digit (not of base 16) in number");
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
		else if (tknType == eT_Dbl)
		{
			if (_isB10Digit_(bffr.bffr[idx]) == false && bffr.bffr[idx] != '.')
			{
				_tScnnr_printErr_(scnnr, "Invalid digit (not of base 10) in floating point number");
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
		else if (tknType == eT_Flt)
		{
			if (_isB10Digit_(bffr.bffr[idx]) == false && bffr.bffr[idx] != '.')
			{
				if ((idx == bffr.len - 1) && (bffr.bffr[idx] == 'F' || bffr.bffr[idx] == 'f')) continue;
				_tScnnr_printErr_(scnnr, "Invalid digit (not of base 10) in floating point number");
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
	}
	if (tknType == eT_Err)
	{
		_tScnnr_printErr_(scnnr, "Invalid number");
		_tArrBffr_destruct_(&bffr);
		return 1;
	}
	return _tScnnr_addTkn_(scnnr, tknType, &bffr);
}
int _tScnnr_parse_(tScnnr *scnnr)
{
	for (_tScnnr_nxt_(scnnr); scnnr->ch != EOF; _tScnnr_nxt_(scnnr))
	{
		scnnr->strt = scnnr->crrnt;
		enum eTkns tknType = eT_Err;
		if (scnnr->ch == '\n')
		{
			++scnnr->ln;
			scnnr->crrnt = 0;
			continue;
		}
		else if (scnnr->ch == '\t') continue;
		else if (scnnr->ch == ' ') continue;
		else if (scnnr->ch == '\r')
		{
			scnnr->crrnt = 0;
			continue;
		}
		else if (scnnr->ch == '(') tknType = eT_LPrnth;
		else if (scnnr->ch == ')') tknType = eT_RPrnth;
		else if (scnnr->ch == '{') tknType = eT_LCrly;
		else if (scnnr->ch == '}') tknType = eT_RCrly;
		else if (scnnr->ch == '[') tknType = eT_LSqr;
		else if (scnnr->ch == ']') tknType = eT_RSqr;
		else if (scnnr->ch == '<')
		{
			char ahd = _tScnnr_ahd_(scnnr);
			if (ahd == '=')
			{
				tknType = eT_LssEq;
				_tScnnr_nxt_(scnnr);
			}
			else if (ahd == '<')
			{
				if (_tScnnr_ahd2_(scnnr) == '=')
				{
					tknType = eT_LssLssEq;
					_tScnnr_nxt_(scnnr);
				}
				else tknType = eT_LssLss;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_Lss;
		}
		else if (scnnr->ch == '>')
		{
			char ahd = _tScnnr_ahd_(scnnr);
			if (ahd == '=')
			{
				tknType = eT_GrtEq;
				_tScnnr_nxt_(scnnr);
			}
			else if (ahd == '>')
			{
				if (_tScnnr_ahd2_(scnnr) == '=')
				{
					tknType = eT_GrtGrtEq;
					_tScnnr_nxt_(scnnr);
				}
				else tknType = eT_GrtGrt;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_Grt;
		}
		else if (scnnr->ch == ',') tknType = eT_Cmma;
		else if (scnnr->ch == '.')
		{
			if (_isB10Digit_(_tScnnr_ahd_(scnnr)))
			{
				if (_tScnnr_parseNum_(scnnr) != 0)
				{
					_tScnnr_destruct_(scnnr);
					return 1;
				}
				continue;
			}
			else tknType = eT_Dot;
		}
		else if (scnnr->ch == '\'')
		{
			if (_tScnnr_parseStrng_(scnnr) != 0)
			{
				_tScnnr_destruct_(scnnr);
				return 1;
			}
			_tScnnr_nxt_(scnnr);
			continue;
		}
		else if (scnnr->ch == '+')
		{
			if (_tScnnr_ahd_(scnnr) == '=')
			{
				tknType = eT_AddEq;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_Add;
		}
		else if (scnnr->ch == '-')
		{
			if (_tScnnr_ahd_(scnnr) == '=')
			{
				tknType = eT_SubEq;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_Sub;
		}
		else if (scnnr->ch == '*')
		{
			char ahd = _tScnnr_ahd_(scnnr);
			if (ahd == '=')
			{
				tknType = eT_MultEq;
				_tScnnr_nxt_(scnnr);
			}
			else if (ahd == '|')
			{
				tknType = eT_MultBOr;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_Mult;
		}
		else if (scnnr->ch == '/')
		{
			if (_tScnnr_ahd_(scnnr) == '=')
			{
				tknType = eT_DivEq;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_Div;
		}
		else if (scnnr->ch == ';') tknType = eT_Smicln;
		else if (scnnr->ch == ':') tknType = eT_Cln;
		else if (scnnr->ch == '!')
		{
			if (_tScnnr_ahd_(scnnr) == '=')
			{
				tknType = eT_BngEq;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_Bng;
		}
		else if (scnnr->ch == '=')
		{
			if (_tScnnr_ahd_(scnnr) == '=')
			{
				tknType = eT_EqEq;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_Eq;
		}
		else if (scnnr->ch == '|')
		{
			if (_tScnnr_ahd_(scnnr) == '=')
			{
				tknType = eT_BOrEq;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_BOr;
		}
		else if (scnnr->ch == '&')
		{
			if (_tScnnr_ahd_(scnnr) == '=')
			{
				tknType = eT_BAndEq;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_BAnd;
		}
		else if (scnnr->ch == '~')
		{
			if (_tScnnr_ahd_(scnnr) == '=')
			{
				tknType = eT_BNotEq;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_BNot;
		}
		else if (scnnr->ch == '%')
		{
			if (_tScnnr_ahd_(scnnr) == '=')
			{
				tknType = eT_PrcntEq;
				_tScnnr_nxt_(scnnr);
			}
			else tknType = eT_Prcnt;
		}
		else if (scnnr->ch == '?') tknType = eT_Qstn;
		else if (scnnr->ch == '^') tknType = eT_Crt;
		else if (scnnr->ch == '@') tknType = eT_At;
		else if (scnnr->ch == '#') tknType = eT_She;
		else if (scnnr->ch == '$') tknType = eT_Dlr;
		else if (scnnr->ch == '\\') tknType = eT_BSlsh;
		else if (scnnr->ch == '"')
		{
			long long unsigned ln = scnnr->ln, crrnt = scnnr->crrnt;
			do
			{
				_tScnnr_nxt_(scnnr);
				if (scnnr->ch == EOF)
				{
					fprintf(stderr, "Err: Failed while parsing token in file '%s' on line %llu at pos %llu.\nExp: Unterminated comment.\n", scnnr->fileName, ln, crrnt);
					_tScnnr_destruct_(scnnr);
					return 1;
				}
				else if (scnnr->ch == '\n')
				{
					++scnnr->ln;
					scnnr->crrnt = 0;
				}
				else if (scnnr->ch == '\r')
				{
					scnnr->crrnt = 0;
				}
			}
			while (scnnr->ch != '"');
			continue;
		}
		else if (_isIdChar_(scnnr->ch))
		{
			if (_tScnnr_parseWord_(scnnr) != 0)
			{
				_tScnnr_destruct_(scnnr);
				return 1;
			}
			continue;
		}
		else if (_isB10Digit_(scnnr->ch))
		{
			if (_tScnnr_parseNum_(scnnr) != 0)
			{
				_tScnnr_destruct_(scnnr);
				return 1;
			}
			continue;
		}
		if (tknType != eT_Err)
		{
			tScnnr_addTkn_s(scnnr, tknType, NULL);
		}
		else
		{
			_tScnnr_destruct_(scnnr);
			return 1;
		}
	}
	return 0;
}
#undef tScnnr_addTkn_s