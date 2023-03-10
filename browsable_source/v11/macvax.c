/*
 *	Macro expanseur de macros "a la Motorola"
 *	ecrit par Zohra Bellahsene pour le transport du
 *	systeme Le_Lisp 68K sur SM90.
 *	[adapte pour VAX par Jerome Chailloux 15-Fev-83]
 */


/* variables globales */

# define MAXMAC 2000
# define MAXMEM 30000
# define MAXPAR 20
# include <stdio.h>

struct stligne {
	char *etiquette;
	char *codop;
	char *paramv [MAXPAR];
		};

struct descripm {
	char *nom;
	char *corps;
		};

struct descripm pmac [MAXMAC];
struct descripm *entc = pmac;
char *ptc, *pts;
char *index();
char mem [MAXMEM];
char *memp = mem;
char *salloc ();


/*
 *	allocation dynamique de la mem. 
 *	pour y stocker le corps d'une macro
 */
putmacro (c)
{
	if (memp >= MAXMEM + mem) nonmem ();
	*memp++ = c;
}


/*
 *	fonction delimitant un champ 
 */
char *champ (firstexp)
int firstexp;
{
	char c, *ptd = pts;

        while (*ptc != '\n' && *ptc != '\t' && *ptc != ' ') {
		/* decodage du type de l'operande */
		if (firstexp == 0) {
			switch (*ptc) {
			case '#':
				/* argument immediat */
				*pts++ = '$';
				ptc++;
				continue;
			case '$':
				/* constante hexadecimale */
				if ((*++ptc>='0' && *ptc<='9') ||
				    (*ptc>='A' && *ptc<='F')) {
					*pts++ = '0';
					*pts++ = 'x';
					--ptc;
					while ((*++ptc>='0' && *ptc<='9') ||
					       (*ptc>='A' && *ptc<='F'))
						*pts++ = *ptc;
				} else {
					*pts++ = '$';
					*pts++ = *ptc++;
				}
				continue;
			}
		};
		switch (*ptc) {
		case ',':
			/* argument suivant */
			*pts++ = *ptc++;
			break;
		case '*':
			/* compteur d'assemblage */
			*pts++ = '*';
			ptc++;
			break;
		case '(':
			*pts++ = *ptc++;
			break;
		case ')':
			*pts++ = *ptc++;
			break;
		case '<':
			/* argument completement quote */
			while (*++ptc!='>') {
				if (*ptc == '\n') merr(5);
				*pts++ = *ptc;
			}
			++ptc;
			break;
		case '\'':
			/* constante ASCII (sans doublement!) */
			*pts++ = '"';
			ptc++;
			while (*ptc != '\'') *pts++ = *ptc++;
			*pts++ = '"';
			ptc++;
			break;
		case 'A':
			/* accumulateurs de LLM3 */
			ptc++;
			if (*ptc == '1' || *ptc == '2' ||
			   *ptc == '3' || *ptc == '4') {
				*pts++ = 'r';
				*pts++ = *ptc++;
			} else 	*pts++ = 'a';
			break;
		default:
			while (*ptc != '\n' && *ptc != ' ' &&
			       *ptc != '\t' && *ptc != ',' &&
			       *ptc != '(' && *ptc!= ')' ) {
				if (*ptc>='A' && *ptc<='Z') {
					*pts++ = *ptc++ + 'a' - 'A'; 
				} else {
					*pts++ = *ptc++;
				}
			}
		}
	}
        /* saute tous les espaces, toutes les tabulations
	   pret pour l'US suivante */
	while (*ptc == ' ' || *ptc == '\t')
		ptc++;
	/* termine la chaine champ */
	*pts++ = ' ';
	return (ptd);
}


/*
 *	recupere le caractere suivant d'une chaine ASCII
 *	dont les ' sont doubles
 */
getquote ()
{
	char c;
	c = *++ptc;
	if (c == '\'') {
		if ( *++ptc == '\'') {
			return (c);
		}
		return(NULL);
	}
	if (!c || c=='\n') {
		merr(6);
		return(NULL);
	}
	return(c);
}


/*
 *	decomposition d'une ligne en champs
 */
decompligne(ligne, tchampp, copie, firstexp)
char *ligne, *copie;
struct stligne *tchampp;
int firstexp;
{	int i;
	char *pit;
	char *pat;
	ptc = ligne;
	pts = copie;
	tchampp->etiquette = champ (firstexp);
	tchampp->codop = champ (firstexp);
	pit = champ (firstexp);
	/* detecter les . arguments mais pas en 1ere position! */
	if ((pat = index(tchampp->codop, '.')) && pat != tchampp->codop)
	{
		tchampp->paramv[0] = pat + 1;
		*pat = ' ';
	} else
		tchampp->paramv[0] = "";
	for (i = 1; i < MAXPAR; i++) /* remplacer les , par des   */
	{
		tchampp->paramv[i] = pit;
		while (*pit != ',' && *pit != ' ')
			pit++;
		if (*pit == ',')  *pit++ = ' ';
	}
}


/*
 *	fonction recherchant le nom d'une macro
 *	dans le descrip. des macros
 */
struct descripm *ismacro (tchampp)
struct stligne *tchampp;
{
struct descripm *macp;
for (macp = pmac; macp < entc; macp++)
	{
		if (strcmp (tchampp->codop, macp->nom) == 0)
			return (macp);

	}
	return (NULL);
}


/*
 *	message d'erreur pour signaler
 *	des parametres de macro incorrects
 */
merr (n)
int n;
{
	fprintf (stderr, "erreur no %d parametre incorrect \n",n);
	exit (1);
}

/*
 *	teste s'il y a au moins un \@,
 *	le remplace par l'etiq. courante
 */
getetiq (macp)
struct descripm *macp;
{
	static int etiqcount;
	char c;
	char *car;
	car = macp->corps;
	while ((c = *car) != ' ')
	{
		if ((c = *car++) == '\\')
		{
			if ((c = *car++) == '@')
				return (++etiqcount);

		}
	}
	return (0);
}

/*
 *	expanse un appel de macro
 */
expandmacro (macp, tchampp)
struct descripm *macp;
struct stligne *tchampp;
{
	char c, ligne[120], *chaine;
	int dansif;
	char *corps;
	int etiq;
	corps = macp->corps;
	etiq = getetiq (macp);
	chaine = ligne;
	dansif = 0;
	while ((c = *corps) != ' ')
		{
		if ((c = *corps++) == '\\') {
			 /* remplacer le nom des parametres par leur valeur */
			if ((c = *corps) >= '0' && c <= '9') {
				sprintf(chaine, "%s", tchampp->paramv[c-'0']);
				chaine += strlen(chaine);   
			} else
				if ((c = *corps) == '@' )  {
			 /* remplacer les \@ par l'etiquette courante */
					sprintf(chaine, ".%d", etiq);
					chaine += strlen(chaine);
				}
				else
					/* parametre incorrect */
					merr (7);
			++corps;
			}
		else
			*chaine++ = c;
		if (c == '\n')
		{
			*chaine++ = ' ';
			dansif = recurmacro(ligne, dansif, 1);
			chaine = ligne;
		}
	}
}

/*
 *	definition d'une macro
 */
defmacro(tchampp)
struct stligne *tchampp;
{
	struct stligne tchamp;
	char *p, c;
	char copie[120], ligne[120];
	strcpy (entc->nom = salloc(strlen(tchampp->etiquette) + 1),
		tchampp->etiquette);
	entc->corps = memp;
	/* memoriser le corps de la macro */
	while( fgets(ligne, sizeof ligne, stdin) )
	{
		decompligne(ligne, &tchamp, copie, 0);
		if ( strcmp ( tchamp.codop, "endm" ) == 0 )
		break;
		p = ligne;
		while( c = *p++ )
		{
			putmacro (c);
		}
	}
	entc++;
}


/*
 *	allocation dynamique de la memoire
 */
char *salloc (N)
{
	char *p;
	p = memp;
	if ( (memp += N) <= mem + MAXMEM)
		return (p);
	nonmem ();
}


/*
 *	message de debordement memoire
 */
nonmem ()
{
	fprintf (stderr, " plus d'espace memoire disponible \n");
	exit (1);
}

/*
 *	procedure recursive principale
 */
recurmacro(chaine,dansif,firstexp)
char *chaine;
{
	int i;
	struct stligne tchamp;
	char copie[120];
	struct descripm *macp;
	if (*chaine =='*' || *chaine =='\n') {
		return(dansif);
	}
	decompligne(chaine, &tchamp, copie, firstexp);
	if (strcmp(tchamp.codop, "equ") == 0) {
		if (strcmp(tchamp.paramv[1], "*") == 0)
			printf("%s:\n",tchamp.etiquette); else
			printf("\t.set\t%s,%s\n",tchamp.etiquette,
				tchamp.paramv[1]);
		return(dansif);
	}
	if (strcmp(tchamp.codop, "endc") == 0)
		return(0);
	if (strcmp(tchamp.codop, "ifeq")==0 || 
            strcmp(tchamp.codop, "ifne")==0)
		return ( ifeqne(equal(tchamp.codop, "ifeq"),
				 tchamp.paramv[1]) );
	if (strcmp(tchamp.codop, "ifc")==0 || 
            strcmp(tchamp.codop, "ifnc")==0 )
		return (equal(tchamp.paramv[1], tchamp.paramv[2]) !=
			equal(tchamp.codop, "ifc") );
	if (dansif)
		return(dansif);
	if (strcmp (tchamp.codop, "macro") == 0) {
		defmacro (&tchamp);
		return(dansif);
	}
	if (*tchamp.etiquette)
		printf("%s:\n",tchamp.etiquette); 
	/* La recursion est enlevee temporairement !!! */
	if (((macp = ismacro (&tchamp)) != NULL) && (firstexp == 0)) {
		expandmacro (macp, &tchamp);
	 } else {
		if (!*tchamp.codop)
			return (dansif); else {
			if (!firstexp) {
				fprintf (stderr,
					"***** ce n'est pas une macro %s\n", 
					tchamp.codop);}
			printf("\t%s",tchamp.codop);
			for (i = 1; i < MAXPAR; i++) {
				if (*tchamp.paramv[i]) {
					if (i != 1) printf(","); 
						else printf("\t");
					printf("%s",tchamp.paramv[i]);
				}
			}
		}
		printf("\n");
	}
	return(dansif);
}

/*
 *	programme principal
 */
main ()
{
	int dansif;
	int firstexp;
	char ligne[120];
	dansif = 0;
	firstexp = 0;
	while (fgets(ligne, sizeof ligne, stdin) != NULL) {
/*
		printf("#%s",ligne);
 */
		dansif = recurmacro(ligne, dansif, firstexp);
	}
}

equal(a,b)
{
	return (!strcmp(a,b));
}

ifeqne(iseqne, string)
{
	return( iseqne==0 );
}

