// relations.c�: d�finit le point d'entr�e pour l'application .
// gcc -o relations relations.c -Wall

typedef int bool;
#define false 0
#define true -1

#include "stdlib.h"
#include "memory.h"
#include "stdio.h"
#include "string.h"


////////////////////////////////////////
// Exercice 1: Classement des Relations

/*
valeur 0: NULL
valeur 1: relation inconnue, on sait pas si les deux entités ont une relation ou non
*/
typedef enum {
	FRERE = 2, COUSIN, PARENT, ONCLE, EPOUX, AMI, VIT, CONNAIT,
	CHEF, COLLEGUE, LOCATAIRE, TRAVAILLE, PROPRIETAIRE, SITUE, DECOUVERT
} rtype;

#define NB_RTYPE 15


bool est_lien_parente(rtype id) { 
	return id == 2 || id == 3 || id == 4 || id == 5 || id == 6; 
}

bool est_lien_professionel(rtype id) { 
	return id == 10  || id == 11;
}

bool est_lien_connaissance(rtype id) {
	return id == 7  || id == 8 || id == 9; 
}

char* toStringRelation(rtype id) { 
	char* t[] = {
		"null",
		"relation inconnue",
		"frère ou sœur de",
		"cousin ou cousine de",
		"père ou mère de",
		"oncle ou tante de",
		"époux ou épouse de",
		"ami de",
		"vit avec",
		"connait",
		"supérieur de",
		"collègue de",
		"locataire",
		"traivaille à",
		"proprietaire de",
		"situé à",
		"découvert à"
	};

	return t[id];
}

////////////////////////////////////////
// Exercice 2: Liste de pointeurs

typedef struct s_node {
	void *val;  // pointeur vers objet quelconque
	struct s_node *suiv;
} *listeg;


listeg listegnouv() { 
	return NULL; 
}

// Il n’est pas nécessaire de cloner les objets lorsqu’ils sont ajoutés. 
listeg adjtete(listeg lst, void *x) {
	listeg new = malloc(sizeof(struct s_node));
	new->val = x;
	new->suiv = lst;
	return new;
}

// Il n’est pas nécessaire de cloner les objets lorsqu’ils sont ajoutés. 
listeg adjqueue(listeg lst, void *x) {
	if (lst == NULL) return adjtete(lst, x);
	listeg new = malloc(sizeof(struct s_node));
	new->val = x;
	new->suiv = NULL;

	listeg curser = lst;
	while(curser->suiv != NULL) curser = curser->suiv;
	curser->suiv = new;

	return lst;
}


listeg suptete(listeg lst) {
	if (lst == NULL) return lst;
	listeg new = lst->suiv;
	free(lst);
	return new;
}

void *tete(listeg lst) {
	return lst->val;
}


int longueur(listeg lst) {
	if (lst == NULL) return 0;
	int n = 0;
	listeg curser = lst;
	while(curser != NULL) {
		curser = curser->suiv;
		n++;
	}
	return n;
}


bool estvide(listeg lst) {
	return lst == NULL;
}


void detruire(listeg lst) {
	if (estvide(lst)) return;
	while(lst != NULL) {
		listeg dummy = lst;
		lst = lst->suiv;
		free(dummy->val);
		free(dummy);
	}
}


listeg rech(listeg lst, void *x, int(*comp)(void *, void *)) {
	if (estvide(lst)) return lst;
	listeg res = listegnouv();
	listeg curser = lst;
	while(curser != NULL) {
		if (comp(curser->val, x) == 0) {
			res = adjtete(res, curser->val);
		}
		curser = curser->suiv;
	}

	return res;
}



////////////////////////////////////////
// Exercice 3: Construction du graphe

#define LONG_NOM_MAX 64
typedef enum { PERSONNE=1, OBJET, ADRESSE, VILLE } etype;
typedef struct s_entite {
	char nom[LONG_NOM_MAX]; // le nom de l�entit� p.ex � Peugeot 106 �
	etype ident; // l�identifiant associ�, p.ex OBJET
} *Entite;

//3.1 les structures de donn�es
typedef struct s_sommet {
	struct s_node *larcs; 
	struct s_entite *x;
} *Sommet;

typedef struct s_arc {
	rtype t;
	struct s_entite* x;
} *Arc;

typedef struct s_relations {
	struct s_node *sommets;
} *Relations;

//3.2 les constructeurs
Entite creerEntite(char *s, etype e) {
	Entite entite = (Entite)malloc(sizeof(struct s_entite));
	entite->ident = e;
	strcpy(entite->nom, s);
	return entite;
}

// va etre stocké dans un s_node *val
Sommet nouvSommet(Entite e) {
	Sommet sommet = (Sommet)malloc(sizeof(struct s_sommet));
	sommet->larcs = listegnouv();
	sommet->x = e;
	return sommet;
}

// va etre stocké dans un s_node *val
Arc nouvArc(Entite e, rtype type) {
	Arc arc = (Arc)malloc(sizeof(struct s_arc));
	arc->t = type;
	arc->x = e;
	return arc;
}


void relationInit(Relations *g) {
	(*g) = (Relations)malloc(sizeof(struct s_relations));
	(*g)->sommets = listegnouv();
}


void relationFree(Relations *g) {
	listeg sommets = (*g)->sommets;
	while(sommets!=NULL) {
		listeg arcs = ((Sommet)sommets->val)->larcs;
		detruire(arcs);
		sommets = sommets->suiv;
	}
	detruire((*g)->sommets);
	free(*g);
}



//3.3 les comparaisons
int compEntite(void *e, void *string) {
	return strcmp(((Entite)e)->nom, string);
}

int compSommet(void *s, void *string) {
	return compEntite(((Sommet)s)->x, string);
}

int compArc(void *a, void *string) {
	return compEntite(((Arc)a)->x, string);
}


//3.4 ajout d'entites et de relations
void adjEntite(Relations g, char *nom, etype t) {
	if (rech(g->sommets, nom, compSommet) == NULL) {
		Entite entite = creerEntite(nom, t);
		Sommet sommet = nouvSommet(entite);
		g->sommets = adjqueue(g->sommets, sommet);
	}
}


// fonction auxilaire
Sommet rechSommet(Relations g, char *nom, int(*comp)(void *, void *)) {
    listeg curser = g->sommets;
    while(curser != NULL) {
        if (comp(curser->val, nom) == 0) {
            return curser->val;
        }
        curser = curser->suiv;
    }
    return NULL;
}

// fonction auxilaire
Arc rechArc(struct s_node* larcs, char *nom, int(*comp)(void *, void *)) {
    listeg curser = larcs;
    while(curser != NULL) {
        if (comp(curser->val, nom) == 0) {
            return curser->val;
        }
        curser = curser->suiv;
    }
    return NULL;
}


// PRE CONDITION: id doit �tre coh�rent avec les types des sommets correspondants � x et y
//                p.ex si x est de type OBJET, id ne peut pas etre une relation de parente
// PRE CONDITION: strcmp(nom1,nom2)!=0
void adjRelation(Relations g, char *nom1, char *nom2, rtype id) {
    Sommet sommet1 = rechSommet(g, nom1, compSommet);
    Sommet sommet2 = rechSommet(g, nom2, compSommet);
    Arc arc1 = rechArc(sommet1->larcs, nom2, compArc);
    if (arc1 != NULL) {
        arc1->t = id; // si l’arc existe déjà, son « type » est mis-à-jour et remplacé par id.
        Arc arc2 = rechArc(sommet2->larcs, nom1, compArc);
        arc2->t = id;
    } else {
        arc1 = nouvArc(sommet2->x, id);
        sommet1->larcs = adjqueue(sommet1->larcs, arc1);
        Arc arc2 = nouvArc(sommet1->x, id);
        sommet2->larcs = adjqueue(sommet2->larcs, arc2);
    }
}
////////////////////////////////////////
// Exercice 4: Explorer les relations entre personnes

// 4.1 listes de relations
listeg en_relation(Relations g, char *x) {
	listeg sommet = rech(g->sommets, x, compSommet);
	return ((Sommet)sommet->val)->larcs;
}


listeg chemin2(Relations g, char *x, char *y) {
	listeg res = listegnouv();
	listeg arcs_de_x = en_relation(g, x); 
	while(arcs_de_x != NULL) {
		Entite entite = ((Arc)arcs_de_x->val)->x; 
		listeg arcs_de_entite = en_relation(g, entite->nom);
		if (rech(arcs_de_entite, y, compArc) != NULL){
			Entite temp = creerEntite(entite->nom, entite->ident);
			res = adjtete(res, temp);
		}
		arcs_de_x = arcs_de_x->suiv;
	}
	return res;
}

// 4.2 verifier un lien de parente
// PRE CONDITION: strcmp(x,y)!=0
bool ont_lien_parente(Relations g, char *x, char *y) {
	listeg larcs = en_relation(g, x);  // tous les relations de x
	if (larcs != NULL) {
		listeg res = rech(larcs, y, compArc);
		if (res != NULL && est_lien_parente(((Arc)res->val)->t)) {
			return true;
		}
	}
	return false;
}

bool ont_lien_professionnel(Relations g, char *x, char *y) {
	listeg larcs = en_relation(g, x); 
	if (larcs != NULL) {
		listeg res = rech(larcs, y, compArc);
		if (res != NULL && est_lien_professionel(((Arc)res->val)->t)) {
			return true;
		}
	}
	return false;
}

bool ont_lien_connaissance(Relations g, char *x, char *y) {
	listeg larcs = en_relation(g, x); 
	if (larcs != NULL) {
		listeg res = rech(larcs, y, compArc);
		if (res != NULL && est_lien_connaissance(((Arc)res->val)->t)) {
			return true;
		}
	}
	return false;
}

// 4.3 tester connaissances
// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent(Relations g, char *x, char *y) {
	if(ont_lien_parente(g, x, y) || ont_lien_professionnel(g, x, y) || ont_lien_connaissance(g, x, y)) return true;
	listeg chemin = chemin2(g, x, y);
	while (chemin != NULL) {
		if (ont_lien_parente(g, x, ((Entite)chemin->val)->nom) && ont_lien_parente(g, y, ((Entite)chemin->val)->nom))
			return true;
		chemin = chemin->suiv;
	}
	return false;
}

// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_proba(Relations g, char *x, char *y) {
	if (se_connaissent(g, x, y)) return false;
	listeg chemin = chemin2(g, x, y);
	while (chemin != NULL) {
		if ((ont_lien_parente(g, x, ((Entite)chemin->val)->nom) && !ont_lien_parente(g, ((Entite)chemin->val)->nom, y))
			|| (!ont_lien_parente(g, x, ((Entite)chemin->val)->nom) && ont_lien_parente(g, ((Entite)chemin->val)->nom, y)))
			return true;
		chemin = chemin->suiv;
	}
	return false;
}

// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_peutetre(Relations g, char *x, char *y) {
	if (se_connaissent_proba(g, x, y)) return false;
	listeg chemin = chemin2(g, x, y);
	while (chemin != NULL) {
		if (ont_lien_connaissance(g, x, ((Entite)chemin->val)->nom) && ont_lien_connaissance(g, y, ((Entite)chemin->val)->nom))
			return true;
		chemin = chemin->suiv;
	}
	return false;
}

////////////////////////////////////////
// Exercice 5: Affichages

char* toStringEntite(etype t) {
	char* strings[] = { "PERSONNE", "OBJET", "ADRESSE", "VILLE" };
	return strings[t-1];
}

void affichelg(listeg l, void(*aff)(void *)) {
	listeg temp = l;
	while(temp != NULL) {
		aff(temp->val);
		temp = temp->suiv;
	}
}

void afficheEntite(void *x) {
	printf("%s: %s\n", ((Entite)x)->nom, toStringEntite(((Entite)x)->ident));
}

void afficheSommet(void *x) {
	afficheEntite(((Sommet)x)->x);
}

void afficheArc(void *x) {
	printf("--%s-->", toStringRelation(((Arc)x)->t));
	afficheEntite(((Arc)x)->x);
}


listeg adjqEns(listeg file, listeg liens, listeg visited) {
	while(liens != NULL) {
		if (rech(file, ((Arc)liens->val)->x, compEntite) == NULL && rech(visited, ((Arc)liens->val)->x, compEntite) == NULL)
			file = adjqueue(file, ((Arc)liens->val)->x);
		liens = liens->suiv;
	}
	return file;
}

////////////////////////////////////////
// Exercice 6: Parcours
void affiche_degre_relations(Relations r, char *x) {
	Entite entite = ((Sommet)rech(r->sommets, x, compSommet)->val)->x;
	listeg queue = adjqueue(listegnouv(), entite);
	listeg visited = listegnouv();
	
	int i = 1;
	while(!estvide(queue)) {
		int size = longueur(queue); // Chloe size = 1   [Gildas, Karl] size = 2
		for (int i = 0; i < size; i++) {
			Entite elem = tete(queue); // Chloe, Gildas
			queue = suptete(queue); // queue = vide | queue = [Karl]
			listeg voisins = en_relation(r, elem->nom); // Gildas, Karl | Chloe
			if (rech(visited, elem, compEntite) == NULL) {
				visited = adjqueue(visited, elem); // visited = [Chloe] | [Chloe, Gildas]
				queue = adjqEns(queue, voisins, visited); // queue = [Gildas, Karl]
			}
		}
		if (!estvide(queue)) {
			printf("-- %d\n", i);
			affichelg(queue, afficheEntite); // [Gildas, Karl]
			i++;
		}

	}
}


int main()
{

	int i,j;
	Relations r; relationInit(&r);
	// ajouter les entites de l'exemple
	char *tabe[] = {"KARL","LUDOVIC","CELINE","CHLOE","GILDAS","CEDRIC","SEVERINE",
		"PEUGEOT 106" ,"1, RUE DE LA RUE","STRASBOURG" };
	for (i = 0; i < 7; i++) adjEntite(r, tabe[i], PERSONNE);
	adjEntite(r, tabe[7], OBJET);
	adjEntite(r, tabe[8], ADRESSE);
	adjEntite(r, tabe[9], VILLE);

	affichelg(r->sommets, afficheSommet);



	// ajouter les relations de l'exemple
	adjRelation(r, tabe[0], tabe[1], FRERE);
	adjRelation(r, tabe[0], tabe[2], AMI);
	adjRelation(r, tabe[0], tabe[3], CONNAIT);
	adjRelation(r, tabe[0], tabe[5], COUSIN);
	adjRelation(r, tabe[0], tabe[7], PROPRIETAIRE);
	adjRelation(r, tabe[0], tabe[8], PROPRIETAIRE);
	adjRelation(r, tabe[3], tabe[4], VIT);
	adjRelation(r, tabe[5], tabe[6], EPOUX);
	adjRelation(r, tabe[5], tabe[8], LOCATAIRE);
	adjRelation(r, tabe[7], tabe[8], DECOUVERT);
	adjRelation(r, tabe[8], tabe[9], SITUE);



	// explorer les relations
	printf("%s est en relation avec:\n", tabe[0]);
	affichelg(en_relation(r, tabe[0]), afficheArc);
	printf("\n");




	for (i = 0; i < 7; i++) for (j = i + 1; j < 10; j++)
	{
		printf("<%s> et <%s> ont les relations communes:\n", tabe[i], tabe[j]);
		listeg ch = chemin2(r, tabe[i], tabe[j]);
		affichelg(ch, afficheEntite);
		printf("\n");
		detruire(ch);
	}
	printf("\n\n");



	for (i = 0; i < 10; i++) for (j = i + 1; j < 10; j++)
	{
		printf("<%s> et <%s> ont lien de parente: %s\n",
			tabe[i], tabe[j], ont_lien_parente(r, tabe[i], tabe[j]) ? "vrai" : "faux");
	}
	printf("\n");


	for (i = 0; i < 7; i++)
	{
		for (j = i + 1; j < 7; j++)
		{
			printf("<%s> et <%s> se connaissent: %s\n",
				tabe[i], tabe[j], se_connaissent(r, tabe[i], tabe[j]) ? "vrai" : "faux");
			printf("<%s> et <%s> se connaissent tres probablement: %s\n",
				tabe[i], tabe[j], se_connaissent_proba(r, tabe[i], tabe[j]) ? "vrai" : "faux");
			printf("<%s> et <%s> se connaissent peut etre: %s\n",
				tabe[i], tabe[j], se_connaissent_peutetre(r, tabe[i], tabe[j]) ? "vrai" : "faux");
		}
		printf("\n");
	}


	affiche_degre_relations(r, tabe[3]);

	relationFree(&r);

	printf("\nPRESS RETURN\n");
	char buff[64]; fscanf(stdin, "%s", buff);
    return 0;


}
