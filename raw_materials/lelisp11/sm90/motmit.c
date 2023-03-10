
/* variables globales */

# define MAXPAR 10
# include <stdio.h>
# define strend(s) ( (s) + strlen(s) )
		/* massive kludjj for kludjy motorola & jerome  */
#define hexdigit(c) ( (c>='0' && c<='9') || c=='F')

struct stligne
{
	char	*etiquette;
	char	*codop;
	char	*comment;
	int	nparam;
	char	*paramv [MAXPAR];
};

/* special directives */
int xop(), dcop(), dsop(), equop(), multop(), sectop();
int condop(), errop(), arg1op(), dcbop(), bclrop();

struct descripm
{
	char	*nom;
	int	(*func)();

} pmac[] = {
	"xref", xop,
	"xdef", xop,
	"dc.b", dcop,
	"dc.w", dcop,
	"dc.l", dcop,
	"ds.b", dsop,
	"ds.w", dsop,
	"ds.l", dsop,
	"equ",	equop,
	"set",	equop,
	"movem", multop,
	"movem.l", multop,
	"movem.w", multop,
	"section", sectop,
	"idnt",	NULL,
	"list", NULL,
	"nolist", NULL,
	"nol", NULL,
	"opt", NULL,
	"format", NULL,
	"page", NULL,
	"org", errop,
	"org.l", errop,
	"rorg", errop,
	"mexit", errop,
	"spc", NULL,
	"noobj", NULL,
	"fail", errop,
	"nopage", NULL,
	"ttl", NULL,
	"end", NULL,
	"ifeq", errop,
	"ifne", errop,
	"rts", arg1op,
	"nop", arg1op,
	"dcb.b", dcbop,
	"dcb.w", dcbop,
	"dcb.l", dcbop,
	"bclr.b", bclrop,
	"bclr.l", bclrop,
	"bset.b", bclrop,
	"bset.l", bclrop,
	"btst.b", bclrop,
	"btst.l", bclrop,
	NULL
};

struct stligne tchamp ;
char ligne [120], copie [120];
char *ptc, *pts;
int lnno;

char *readln(), *champ();
struct descripm *isspcl();


/* programme principal */
main ()
{
	int i;
	struct descripm *macp;

	while( readln() ) {
		if(*ligne == '*') { /* whole line is a comment */
			printf("|%s", ligne+1);
			continue;
		}
		if ( (macp = isspcl ()) != NULL) {
			if (macp->func)
				(*macp->func)();
			else { /* null pseudo op -- echo as comment */
				printf("|%s", ligne);
				continue;
			}
		}
		else {
			label();
			if (*tchamp.codop)
				outop(tchamp.codop);
			printargs();
		}
		if(*tchamp.comment!='\n')
			printf("\t\t|\t%s", tchamp.comment);
		else
			printf("\n");
	}
}


/*  lit une ligne et la decompose en champs */
char *readln ()
{	register i, brac;
	register char *pit, c;
	/* lit une ligne d'un fichier */

	if (fgets (ligne, sizeof ligne, stdin) == NULL) return (NULL);

	lnno++;
	if (ligne[0]=='*')
		return (ligne);
	ptc = ligne;
	pts = copie;
	tchamp.etiquette = champ ();
	tchamp.codop = champ ();
	pit = champ ();
	tchamp.comment = ptc;
	tchamp.nparam = *pit != ' ';
	for (i = 0; i < MAXPAR; i++) /* remplacer les , par des   */
	{
		tchamp.paramv[i] = pit;
		brac = 0;
		while (c = *pit) {
			switch (c) {
			case ',':
				if (!brac)
					goto out;
				break;
			case '(':
				brac++; break;
			case ')':
				brac--; break;
			}
			pit++;
		}
	out:
		if (c == ',') {
			*pit++ = ' ';
			tchamp.nparam++;
		}
	}
	return (ligne);
}

char *champ ()
{
	char c, *ptd = pts;
	while (*ptc != '\n')
	{
		if (*ptc == '\t' || *ptc == ' ')
		{
			/* remplacer les blancs et les tabulation
			   par des 0 */
			*pts++ = ' ';
			while (*++ptc == ' ' || *ptc == '\t')
				;
			return (ptd);
		}
		if ((c = *ptc++) == '\'') {
			*pts++ = '\'';
			while ( (c = getquote()) )
				*pts++ = c;
			*pts++ = '\'';
		} else {
			if (c=='$' && hexdigit(*ptc) ) {
				*pts++ = '0';
				*pts++ = 'x';
			} else {
				if (c>='A' && c<='Z')
					c += 'a' - 'A';
				*pts++ = c;
			}
		}
	}
	*pts++ = ' ';
	return (ptd);
}

getquote()
{
	char c;
	if ((c = *ptc++) == '\'') {
		if( *ptc=='\'' ) {
			ptc++;
			return (c);
		}
		return(NULL);
	}
	if (!c || c=='\n') {
		werr("newline or EOF in string");
		return(NULL);
	}
	return(c);
}

/* fonction recherchant le nom d'une macro dans le descrip. des macros */
struct descripm *isspcl ()
{
	struct descripm *macp;
	for (macp = pmac; macp->nom; macp++) {
		if (strcmp (tchamp.codop, macp->nom) == 0)
			return (macp);

	}
	return (NULL);
}

char *defops[] =
{
	"add", "addx", "and", "asl", "asr", "cmp", "clr", "ext", "lsl",
	"lsl", "move", "neg", "negx", "not", "or", "rol", "ror", "roxl",
	"roxr", "sub", "subq", "tst",
	"addq", "subx", /* forgot these ones !!! */
	NULL
};
outop(op)
register char *op;
{
	register char **opp;
	putchar('\t');
	opp = defops;
	while( *opp )
		if (strcmp(*(opp++), op)==0) {
			printf("%sw", strcmp(op, "move") ?op : "mov");
			return;
		}
	if (strncmp(op, "move", 4) == 0)
		printf("mov");
	else
		begop(op);
	restop(op);
}


substarg(arg)
register char *arg;
{
	/* first off, find the index register */
	register char *areg, *dreg, *offs, *offe, disp, incrdecr;
	char *end;
	register brac;

	end = strend(arg) - 1;
	areg = dreg = 0;
	disp = 0;
	if (*end == '+') {
		incrdecr = '+';
		end--;
	}
	if (*arg == '-' )
		incrdecr = '-';

	if( end>=arg && *end == ')') {
		brac = 1;
		while( brac && --end >= arg ) {
			if (*end == '(')
				brac--;
			else if (*end == ')')
				brac++;
		}
		offs = arg;
		offe = end;
		areg = end + 1;
		if (areg[2] == ',')  {
			dreg = areg + 3;
			if (areg[2+3] == '.')
				disp = areg[3+3];
		}
	} else {
		outoff(arg, strend(arg));
		return;
	}
	printf("%c%c@", areg[0], areg[1]);
	if (incrdecr)
		putchar(incrdecr);
	else if (offs!=offe || dreg) {
		putchar('(');
		if (offs==offe)
			putchar('0');
		else
			outoff(offs, offe);
		if( dreg )
			printf(",%c%c", dreg[0], dreg[1]);
		if( disp )
			printf(":%c", disp);
		putchar(')');
	}
}

outoff(offs, offe)
register char *offs, *offe;
{
	register char c;
	register inquote = 0;
	int state;
	state = 0;
	while (offs<offe) {
		if ((c = *offs++) == '\'')
			inquote = !inquote;
		if (c=='*' && state==0)
			putchar('.');
		else
			putchar(c);
		state = alphanum(c);
	}
}

alphanum(c)
{
	if ((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' & c<='9') )
		return(1);
	return(0);
}

struct reg
{
	char	*rname;
	int	rno;
} regnames[] = {
	"d0", 0,
	"d1", 1,
	"d2", 2,
	"d3", 3,
	"d4", 4,
	"d5", 5,
	"d6", 6,
	"d7", 7,
	"a0", 8,
	"a1", 9,
	"a2", 10,
	"a3", 11,
	"a4", 12,
	"a5", 13,
	"a6", 14,
	"sp", 15,
	"a7", 15,
	"pc", -1,
	NULL
} ;

isreg(s)
{
	register struct reg *rp;
	for (rp=regnames; rp->rname; rp++)
		if (strncmp(rp->rname, s, 2) == 0)
			return(rp->rno);
	return(-1);
}


sectop()
{
	label();
	if( !strcmp(tchamp.paramv[0], "10") || !strcmp(tchamp.paramv[0],"9"))
		printf("\t.text");
	else if (strcmp(tchamp.paramv[0], "15") == 0)
		printf("\t.data");
	else if ( strcmp(tchamp.paramv[0], "14") == 0)
		printf("\t.bss");
	else
		werr("bad argument to section directive");
}

equop()
{
	printf("%s = ", tchamp.etiquette);
	substarg(tchamp.paramv[0]);
}

xop()
{
	printf("\t.globl");
	printargs();
}



dcop()
{
	register i;
	label();
	for (i=0; i<tchamp.nparam; i++) {
		if (*tchamp.paramv[i] == '\'') {
			strop(tchamp.paramv[i]);
		} else {
			switch(tchamp.codop[3]) { /* DC.[BWD] */
			case 'b':
				printf("\t.byte\t");
				break;
			case 'l':
				printf("\t.long\t");
				break;
			case 'w':
			default:
				printf("\t.word\t");
				break;
			}
			substarg(tchamp.paramv[i]);
		}
		putchar('\n');
	}
}

dcbop()
{
	register char *w, *init;
	register count;
	label();
	switch(tchamp.codop[4]) {
	case 'b':
		w = ".byte"; break;
	case 'l':
		w = ".long"; break;
	default: case 'w':
		w = ".word"; break;
	}
	init = tchamp.nparam<2 ? "0" : tchamp.paramv[1];
	if (!strcmp(init, "0") && tchamp.codop[3]=='l')
		printf("\t.zerol\t%s\n", tchamp.paramv[0]);
	count = atoi(tchamp.paramv[0]);
	while(count-- >0)
		printf("\t%s\t%s\n", w, init);
}


strop(s)
register char *s;
{
	register char *start, *end, c;
	align(tchamp.codop[3]);
	printf("\t.ascii\t");
	start = s + 1;
	end = strend(start) - 1;
	putchar('"');
	while (start < end) {
		if ( (c = *start++) == '"' || c == '\' )
			putchar('\');
		putchar(c);
	}
	printf("\"\n");
	align(tchamp.codop[3]);
}

dsop()
{
	label();
	align(tchamp.codop[3]);
	printf(". = ");
	switch(tchamp.codop[3]) {
	default:
	case 'b': printf("1"); break;
	case 'w': printf("2"); break;
	case 'l': printf("4"); break;
	}
	printf("*");
	substarg(tchamp.paramv[0]);
	printf(" + .");
}

multop()
{
	unsigned short mask, flipbits(), multbits();
	register regs, mem;
	label();
	if (strcmp(tchamp.codop, "movem.l")==0)
		printf("\tmoveml");
	else
		printf("\tmovemw");
	printf("\t");
	regs = isreg(tchamp.paramv[1]) >= 0;
	mem = !regs;
	mask = multbits(tchamp.paramv[regs]);
	if (tchamp.paramv[mem][0] == '-' )
		mask = flipbits(mask);
	if (regs==0)
		printf("#0x%04x", mask);
	else
		substarg(tchamp.paramv[0]);
	putchar(',');
	if (regs==0)
		substarg(tchamp.paramv[1]);
	else
		printf("#0x%04x", mask);
}

unsigned short  multbits(s)
register char *s;
{
	register unsigned short mask;
	register rn, r2n;
	int temp;

	mask = 0;
	while ((rn = isreg(s)) >= 0) {
		if (s[2]=='-') {
			if ((r2n = isreg(s+=3)) < 0)
				break;
			if (r2n<rn ) {
				temp =rn;
				rn = r2n;
				r2n = temp;
			}
			mask |= ((1<<(r2n+1)) - 1) - ((1<<rn) - 1);
		} else
			mask |= 1 << rn;
		if ( *(s+=2) != '/' )
			break;
		s++;
	}
	return(mask);
}

unsigned short flipbits(word)
{
	register unsigned short test, rev, val;
	val = 0;
	for(test=1, rev=0x8000; rev; test<<=1, rev >>= 1)
		if (word&test)
			val |= rev;
	return (val);
}



errop()
{
	char s[80];
	sprintf(s, "unknown directive %s", tchamp.codop);
	werr(s);
}

werr(s)
char *s;
{
	fprintf(stderr, "line %d: %s\n", lnno, s);
}


begop(op)
register char *op;
{
	register char c;
	while ( (c = *op++) && c!='.' )
		putchar(c);
}

restop(op)
register char *op;
{
	register char c;
	while ( (c = *op++) && c!='.' )
		;
	if (c)
		while( c = *op++ )
			putchar(c);
}

align(c)
{
	if (c != 'b')
		printf("\t.even\n");
}

printargs()
{
	register i;
	if (tchamp.nparam==0)
		return;
	putchar('\t');
	for(i=0; i<tchamp.nparam; i++) {
		substarg(tchamp.paramv[i]);
		if(i<tchamp.nparam-1)
			printf(",");
	}
}

label()
{
	if (*tchamp.etiquette)
		printf("%s:", tchamp.etiquette);
}


arg1op()
{
	label();
	putchar('\t');
	printf("%s", tchamp.codop);
}

bclrop()
{
	label();
	begop(tchamp.codop);
	printargs();
}
