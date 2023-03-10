

/* variables globales */

# define MAXMAC 2000
# define MAXMEM 20000
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

/* allocation dynamique de la mem. pour stocker le corps d'une macro */
putmacro (c)
{
	if (memp >= MAXMEM + mem)
		nonmem ();
		*memp++ = c;
}
/* fonction delimitant un champ */
char *champ ()
{
	char *ptd = pts;
	while (*ptc != '\n')
	{
		switch (*ptc) {
		case '<':
			while (*++ptc!='>' && *ptc!=' ')
				*pts++ = *ptc;
			++ptc;
			break;
		case '\t':
		case ' ':
			/* remplacer les blancs et les tabulation
			   par des 0 */
			*pts++ = ' ';
			while (*++ptc == ' ' || *ptc == '\t')
				;
			return (ptd);
		default:
			*pts++ = *ptc++;
		}
	}
	*pts++ = ' ';
	return (ptd);
}

/* decomposition dune ligne en champs */
decompligne(ligne, tchampp, copie)
char *ligne, *copie;
struct stligne *tchampp;
{	int i;
	char *pit;
	char *pat;
	ptc = ligne;
	pts = copie;
	tchampp->etiquette = champ ();
	tchampp->codop = champ ();
	pit = champ ();
	/* detecter les . argumens */
	if (pat = index(tchampp->codop, '.'))
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

/* fonction recherchant le nom d'une macro dans le descrip. des macros */
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


/* message d'erreur pour signaler des parametres de macro incorrects */
merr ()
{
	fprintf (stderr, "parametre incorrect \n");
	exit (1);
}

/* teste s'il y a au moins un \@, le remplace par l'etiq. courante */
getetiq (macp)
struct descripm *macp;
{
	static int etiqcount;
	char c;
	char *car;
	car = macp->corps;
	while ((c = *car) != ' ')
	{
		if ((c = *car++) == '\')
		{
			if ((c = *car++) == '@')
				return (++etiqcount);

		}
	}
	return (0);
}




expandmacro (macp, tchampp) /* remplace le nom d'une macro par son corps */
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
	if (*tchampp->etiquette)
		printf("%s\t", tchampp->etiquette);
	while ((c = *corps) != ' ')
		{
		if ((c = *corps++) == '\')
			 /* remplacer le nom des parametres par leur valeur */
			{
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
					merr (); /* message parametre incorrect */
			++corps;
			}
		else
			*chaine++ = c;
		if (c == '\n')
		{
			*chaine++ = ' ';
			dansif = recurmacro(ligne, dansif);
			chaine = ligne;
		}
	}
}

/* definition d'une macro */
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
		decompligne(ligne, &tchamp, copie);
		if ( strcmp ( tchamp.codop, "ENDM" ) == 0 )
		break;
		p = ligne;
		while( c = *p++ )
		{
			putmacro (c);
		}
	}
	entc++;
}


/* allocation dynamique de la memoire */
char *salloc (N)
{
	char *p;
	p = memp;
	if ( (memp += N) <= mem + MAXMEM)
		return (p);
	nonmem ();
}


/* message de debordement memoire */
nonmem ()
{
	fprintf (stderr, " plus d'espace memoire disponible \n");
	exit (1);
}

/* programme principal */
recurmacro(chaine,dansif)
char *chaine;
{
	struct stligne tchamp;
	char copie[120];
	struct descripm *macp;
	if (*chaine =='*') {
		if (!dansif)
			fputs(chaine, stdout);
		return(dansif);
	}
	decompligne(chaine, &tchamp, copie);
	if (strcmp(tchamp.codop, "ENDC") == 0)
		return(0);
	if (strcmp(tchamp.codop, "IFEQ")==0 || strcmp(tchamp.codop, "IFNE")==0)
		return ( ifeqne(equal(tchamp.codop, "IFEQ"),
				 tchamp.paramv[1]) );
	if (strcmp(tchamp.codop, "IFC")==0 || strcmp(tchamp.codop, "IFNC")==0 )
		return (equal(tchamp.paramv[1], tchamp.paramv[2]) !=
			equal(tchamp.codop, "IFC") );
	if (dansif)
		return(dansif);
	if ( (macp = ismacro (&tchamp)) != NULL)
		expandmacro (macp, &tchamp);
	else
		if (strcmp (tchamp.codop, "MACRO") == 0)
			defmacro (&tchamp);
		else
			fputs (chaine, stdout);
	return(dansif);
}

/* programme principal */
main ()
{
	int dansif;
	char ligne[120];
	dansif = 0;
	while (fgets(ligne, sizeof ligne, stdin) != NULL)
		dansif = recurmacro(ligne, dansif);
}

equal(a,b)
{
	return (!strcmp(a,b));
}

ifeqne(iseqne, string)
{
	return( iseqne==0 );
}

