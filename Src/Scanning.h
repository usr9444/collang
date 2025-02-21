#ifndef SCANNING_H_
#define SCANNING_H_
//TODO(Me): Move all of this into a .c file, Only include what we need in header.
enum eTokens
{
	eT_Err,

	/*Brackets*/
	eT_LPrnth,
	eT_RPrnth,
	eT_LCrly,
	eT_RCrly,
	eT_LSqr,
	eT_RSqr,
	eT_Lss,
	eT_Grt,
	eT_Cmma,
	eT_Dot,
	//eT_Apos,

	/*Equations*/
	eT_Add,
	eT_AddEq,
	eT_Sub,
	eT_SubEq,
	eT_Mult,
	eT_MultEq,
	eT_MultBOr,
	eT_Div,
	eT_DivEq,
	eT_Smicln,
	eT_Cln,
	eT_Bng,
	eT_BngEq,
	eT_Eq,
	eT_EqEq,
	eT_LssEq,
	eT_LssLss,
	eT_LssLssEq,
	eT_GrtEq,
	eT_GrtGrt,
	eT_GrtGrtEq,
	eT_BOr,
	eT_BOrEq,
	eT_BAnd,
	eT_BAndEq,
	eT_BNot,
	eT_BNotEq,
	eT_Prcnt,
	eT_PrcntEq,
	eT_Qstn,
	eT_Crt,
	eT_At,
	eT_She,
	eT_Dlr,
	eT_BSlsh,
	eT_Quot,
	/*Flow control*/
	eT_And,
	eT_Or,
	eT_If,
	eT_Else,
	eT_For,
	eT_While,
	eT_Ret,
	eT_Brk,

	/*Types*/
	eT_None,
	eT_Id,
	eT_Strng,
	eT_B2,
	eT_B8,
	eT_B10,
	eT_B16,
	eT_Flt,
	eT_Dbl,
};
struct _tArrBffr_
{
	char *bffr;
	long long unsigned cpcty, len;
};
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
struct _tTkn_
{
	enum eTokens type;
	long long unsigned ln, pos;
	struct _tArrBffr_ bffr;
};
struct _tTkns_
{
	struct _tTkn_ *tkns;
	long long unsigned cpcty;
	long long unsigned len;
};
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
static int _tTkns_addTkn_(struct _tTkns_ *tkns, enum eTokens type, long long unsigned ln, long long unsigned pos, struct _tArrBffr_ *bffr)
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
typedef struct
{
	char const *fileName;
	long long unsigned strt, curr, ln;
	char ch;
	struct _tTkns_ tkns;
	FILE *filePtr;
}
tScnnr;
tScnnr tScnnr_construct(char const *fileName, FILE *filePtr)
{
	tScnnr scnnr;
	scnnr.ch = EOF;
	scnnr.curr = 0;
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
#define tScnnr_destruct(scnnr) _tScnnr_destruct_(&scnnr)
static int _tScnnr_addTkn_(tScnnr *scnnr, enum eTokens type, long long unsigned ln, long long unsigned pos, struct _tArrBffr_ *bffr)
{
	int err = _tTkns_addTkn_(&scnnr->tkns, type, ln, pos, bffr);
	if (err != 0) fprintf(stderr, "Err: Failed to add _tTkn_ in file '%s' on line %llu at pos %llu.\n", scnnr->fileName, scnnr->ln, scnnr->curr);
	return err;
}
#define tScnnr_addTkn_s(scnnr, type, ln, pos, lit) \
	{ \
		int err = _tScnnr_addTkn_(scnnr, type, ln, pos, lit); \
		if (err != 0) \
		{ \
			fprintf(stderr, "Err: Failed while parsing token in '%s'.\n", scnnr->fileName); \
			_tScnnr_destruct_(scnnr); \
			return err; \
		} \
	}
static char _tScnnr_nxt_(tScnnr *scnnr)
{
	++scnnr->curr;
	scnnr->ch = getc(scnnr->filePtr);
	return scnnr->ch;
}
static char _tScnnr_ahd_(tScnnr *scnnr)
{
	char const ch = getc(scnnr->filePtr);
	ungetc(ch, scnnr->filePtr);
	return ch;
}
static char _tScnnr_ahd2_(tScnnr *scnnr)
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
	enum eTokens tknType = eT_Err;
	if (strncmp(bffr.bffr, "and", (4 >= bffr.len) ? 4 : bffr.len) == 0) tknType = eT_And;
	else if (strncmp(bffr.bffr, "for", (4 >= bffr.len) ? 4 : bffr.len) == 0) tknType = eT_For;
	else if (strncmp(bffr.bffr, "or", (3 >= bffr.len) ? 3 : bffr.len) == 0) tknType = eT_Or;
	else if (strncmp(bffr.bffr, "if", (3 >= bffr.len) ? 3 : bffr.len) == 0) tknType = eT_If;
	else if (strncmp(bffr.bffr, "else", (5 >= bffr.len) ? 5 : bffr.len) == 0) tknType = eT_Else;
	else if (strncmp(bffr.bffr, "while", (6 >= bffr.len) ? 6 : bffr.len) == 0) tknType = eT_While;
	else if (strncmp(bffr.bffr, "ret", (4 >= bffr.len) ? 4 : bffr.len) == 0) tknType = eT_Ret;
	else if (strncmp(bffr.bffr, "brk", (4 >= bffr.len) ? 4 : bffr.len) == 0) tknType = eT_Brk;
	else if (strncmp(bffr.bffr, "None", (4 >= bffr.len) ? 4 : bffr.len) == 0) tknType = eT_None;
	else tknType = eT_Id;
	return _tScnnr_addTkn_(scnnr, tknType, scnnr->ln, scnnr->strt, &bffr);
}
static int _tScnnr_parseStrng_(tScnnr *scnnr)
{
	struct _tArrBffr_ bffr = _tArrBffr_construct_();
	for (char ch = _tScnnr_ahd_(scnnr); ch != '\''; ch = _tScnnr_ahd_(scnnr))
	{
		if (ch == '\n' || ch == '\r' || ch == EOF)
		{
			fprintf(stderr, "Err: Unterminated string in file '%s' on line %llu at pos %llu.\n", scnnr->fileName, scnnr->ln, scnnr->strt);
			_tArrBffr_destruct_(&bffr);
			return 1;
		}
		int err = _tArrBffr_addCh_(&bffr, _tScnnr_nxt_(scnnr));
		if (err != 0)
		{
			_tArrBffr_destruct_(&bffr);
			return err;
		}
	}
	return _tScnnr_addTkn_(scnnr, eT_Strng, scnnr->ln, scnnr->strt, &bffr);
}
static int _tScnnr_parseNum_(tScnnr *scnnr)
{

	struct _tArrBffr_ bffr = _tArrBffr_construct_();
	bool hasDot = scnnr->ch == '.';
	enum eTokens tknType = hasDot == true ? eT_Dbl : eT_Err;
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
			fprintf(stderr, "Err: Multiple floating type declarations ('F') present in floating point number in file '%s' on line %llu at pos %llu.\n", scnnr->fileName, scnnr->ln, scnnr->curr);
			_tArrBffr_destruct_(&bffr);
			return 1;
		}
		else if (ch == '.')
		{
			if (hasDot == true)
			{
				fprintf(stderr, "Err: Multiple decimal points present in floating point number in file '%s' on line %llu at pos %llu.\n", scnnr->fileName, scnnr->ln, scnnr->curr);
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
				fprintf(stderr, "Err: Invalid digit '%c' (not of base 2) in number '%s' from file '%s' on line %llu at pos %llu.\n", bffr.bffr[idx], bffr.bffr, scnnr->fileName, scnnr->ln, scnnr->curr);
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
		else if (tknType == eT_B8)
		{
			if (_isB8Digit_(bffr.bffr[idx]) == false)
			{
				fprintf(stderr, "Err: Invalid digit '%c' (not of base 8) in number '%s' from file '%s' on line %llu at pos %llu.\n", bffr.bffr[idx], bffr.bffr, scnnr->fileName, scnnr->ln, scnnr->curr);
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
		else if (tknType == eT_B10)
		{
			if (_isB10Digit_(bffr.bffr[idx]) == false)
			{
				fprintf(stderr, "Err: Invalid digit '%c' (not of base 10) in number '%s' from file '%s' on line %llu at pos %llu.\n", bffr.bffr[idx], bffr.bffr, scnnr->fileName, scnnr->ln, scnnr->curr);
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
		else if (tknType == eT_B16)
		{
			if (_isB16Digit_(bffr.bffr[idx]) == false)
			{
				fprintf(stderr, "Err: Invalid digit '%c' (not of base 16) in number '%s' from file '%s' on line %llu at pos %llu.\n", bffr.bffr[idx], bffr.bffr, scnnr->fileName, scnnr->ln, scnnr->curr);
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
		else if (tknType == eT_Dbl)
		{
			if (_isB10Digit_(bffr.bffr[idx]) == false && bffr.bffr[idx] != '.')
			{
				fprintf(stderr, "Err: Invalid digit '%c' (not of double floating point type) in number '%s' from file '%s' on line %llu at pos %llu.\n", bffr.bffr[idx], bffr.bffr, scnnr->fileName, scnnr->ln, scnnr->curr);
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
		else if (tknType == eT_Flt)
		{
			if (_isB10Digit_(bffr.bffr[idx]) == false && bffr.bffr[idx] != '.')
			{
				if ((idx == bffr.len - 1) && (bffr.bffr[idx] == 'F' || bffr.bffr[idx] == 'f')) continue;
				fprintf(stderr, "Err: Invalid digit '%c' (not of floating point type) in file '%s' on line %llu at pos %llu.\n", bffr.bffr[idx], scnnr->fileName, scnnr->ln, scnnr->curr);
				_tArrBffr_destruct_(&bffr);
				return 1;
			}
		}
	}
	if (tknType == eT_Err)
	{
		fprintf(stderr, "Err: Invalid number in file '%s' on line %llu at pos %llu. This point should be unreachable.\n", scnnr->fileName, scnnr->ln, scnnr->curr);
		_tArrBffr_destruct_(&bffr);
		return 1;
	}
	return _tScnnr_addTkn_(scnnr, tknType, scnnr->ln, scnnr->strt, &bffr);
}
int _tScnnr_parse_(tScnnr *scnnr)
{
	for (_tScnnr_nxt_(scnnr); scnnr->ch != EOF; _tScnnr_nxt_(scnnr))
	{
		scnnr->strt = scnnr->curr;
		enum eTokens tknType = eT_Err;
		if (scnnr->ch == '\n')
		{
			++scnnr->ln;
			scnnr->curr = 0;
			continue;
		}
		else if (scnnr->ch == '\t') continue;
		else if (scnnr->ch == ' ') continue;
		else if (scnnr->ch == '\r')
		{
			scnnr->curr = 0;
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
				fprintf(stderr, "Err: Invalid string in file '%s' on line %llu at pos %llu.\n", scnnr->fileName, scnnr->ln, scnnr->curr);
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
		else if (scnnr->ch == '"') tknType = eT_Quot;
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
			tScnnr_addTkn_s(scnnr, tknType, scnnr->ln, scnnr->strt, NULL);
		}
		else
		{
			fprintf(stderr, "Err: Unrecognized token '%c' in file '%s' on line %llu at pos %llu.\n", scnnr->ch, scnnr->fileName, scnnr->ln, scnnr->curr);
			_tScnnr_destruct_(scnnr);
			return 1;
		}
	}
	for (long long unsigned idx = 0; idx < scnnr->tkns.len; ++idx)
	{
		if (scnnr->tkns.tkns[idx].bffr.cpcty == 0) fprintf(stdout, "_tTkn_(%d, %llu, %llu)\n", scnnr->tkns.tkns[idx].type, scnnr->tkns.tkns[idx].ln, scnnr->tkns.tkns[idx].pos);
		else fprintf(stdout, "_tTkn_(%d, %llu, %llu, %s)\n", scnnr->tkns.tkns[idx].type, scnnr->tkns.tkns[idx].ln, scnnr->tkns.tkns[idx].pos, scnnr->tkns.tkns[idx].bffr.bffr);
	}
	return 0;
}
#undef tScnnr_addTkn_s
#define tScnnr_parse(scnnr) _tScnnr_parse_(&scnnr)
#endif/*SCANNING_H_*/