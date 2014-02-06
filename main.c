//----------------------------------------------------
//													//
//			Cryptage RSA							//
//			v1.0									//
//													//
//          Rémi Vion - Jean-Baptiste Arzounian     //
//			2010-2011		                        //
//                                                  //
//----------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>

#include <string.h>
#include <time.h>

void mpz_min(mpz_t* v, mpz_t x, mpz_t y);				// renvoi le minimum entre x, et y
void mpz_max(mpz_t* v, mpz_t x, mpz_t y);				// renvoi le maximum entre x, et y
void mpz_inverseModulo(mpz_t* v, mpz_t a, mpz_t b); 	// renvoi l'inverse de a mod b ou 0 si pas inversible
int mpz_premier(mpz_t a);						        // renvoi si un nombre est premier

int miller_rabin_pass(mpz_t a, mpz_t n);
int miller_rabin(mpz_t n, gmp_randstate_t rand_state, int nb_bases_test);

int min(int x, int y);					// renvoi le minimum entre x, et y
int max(int x, int y);					// renvoi le maximum entre x, et y
int inverseModulo(int a, int b); 		// renvoi l'inverse de a mod b ou 0 si pas inversible
int premier(int a);						// renvoi si un nombre est premier

int main()
{
                                        // VARIABLES DECLARATION
        //Clées Générées
    mpz_t clee_privee;      mpz_init(clee_privee);
    mpz_t clee_publique;    mpz_init(clee_publique);
    mpz_t clee_modulo;      mpz_init(clee_modulo);


        // Auto GMP + test  prima
    mpz_t __n;      mpz_init(__n);
    mpz_t __max;    mpz_init(__max);
    mpz_t __two;    mpz_init_set_ui(__two, 2);
    mpz_t __p;      mpz_init(__p);
    mpz_t __q;      mpz_init(__q);

    gmp_randstate_t __rand_state;
    gmp_randinit_default(__rand_state);
    gmp_randseed_ui (__rand_state, time(NULL));

        // Manu GMP
    mpz_t _p, _q;   mpz_init (_p); mpz_init (_q);
	mpz_t _n;       mpz_init (_n);
	mpz_t _phi;     mpz_init(_phi);
	mpz_t _e,_d;    mpz_init(_e); mpz_init(_d);
	mpz_t _temp1;    mpz_init(_temp1);
	mpz_t _temp2;   mpz_init(_temp2);

        //manu Small
	int p,q;
	int n;
	int phi;
	int e,d;

        // In / Out
    char s_temp[1000]="";
    char s_in[1000]  = "ref.txt";
    char s_out[1000] = "out.txt";
    FILE * f_in ;
    FILE * f_out;

        // Some vars
    int longeur_nombres=100;
    int clees_generees=0;
    int i,j;
 	char c = ' ';
	int menu;
	int continuer =1;
	int base_cryptage=10;
	char separator='\n';

    while (continuer ==1 )
    {
        if (clees_generees==1) {printf("\n\n\n\nLes clees ont ete generees");}
        else  {printf("\n\n\nLes clees n'ont  pas encore ete generees");}
            //Menu
        printf("\n\t-> 1: generer des cles RSA manuellement (petits nombres ; sans GMP).");
        printf("\n\t-> 2: generer des cles RSA manuellement (grands nombres ; avec GMP).");
        printf("\n\t-> 3: generer des cles RSA automatiquement (avec GMP ).");
        printf("\n\t-> 8: Entrer manuellement les clees.");
        printf("\n\t-> 9: Charger les clees a partir d'un fichier.");

        if (clees_generees==1)
        {
            printf("\n\n4: Cripter un fichier.");
            printf("\n5: Decripter un fichier.");
            printf("\n10: Enregistrer les clees dans un fichier");
        }
        printf("\n\n6: Test de primalite ( Miller Rabin ).");
        printf("\n\n0: Quitter.");

        printf("\n\nChoix : ");
        scanf("%d",&menu);

		switch (menu)
		{
			case 0: // quiter
				continuer=0;
			break;

			case 1: // setup RSA
				printf("\nEntrez deux nombres premiers p et q, : \n"); // On entre deux nbres premiers p, q
				do { printf("\tp = ");           scanf("%d",&p); } while( premier(p)!= 1 );
				do { printf("\tp = %d, q = ",p); scanf("%d",&q); } while( q==p || premier(q)!= 1 );

				n = p*q;		    // on calcul la première partie de la clé publique
				phi=(p-1)*(q-1);	// on calcule l'indicatrice d'euler de n

				printf("\tphi(%d) = %d\n",n,phi);
				printf("\nEntrez un nombre e, tel que Pgcd(%d,e) = 0 :\n",phi);
				do { printf("\te = "); scanf("%d",&e);printf("\n"); d= inverseModulo(e,phi); } while( d==0 );
				printf("\n\td = %d , car (%d * %d = 1 mod %d )\n",d,d,e,phi);

                while (d<0) {d+=n;}
				printf("\n\tCle Publique : { %d, %d }",e,n);
				printf("\n\tCle Privee   : { %d, %d }\n\n",d,n);

				mpz_set_ui(clee_privee,d);
				mpz_set_ui(clee_publique,e);
				mpz_set_ui(clee_modulo,n);
				clees_generees=1;
			break;

			case 2: // setup RSA
				printf("\nEntrez deux nombres premiers p et q, : \n"); // On entre deux nbres premiers p, q
				do { printf("\tp = ");                gmp_scanf("%Zd",_p);  } while( mpz_premier(_p)!= 1 );
				do { gmp_printf("\tp = %Zd, q = ",_p); gmp_scanf("%Zd",_q); } while( mpz_cmp(_q,_p)==0 || mpz_premier(_q)!= 1 );

				mpz_mul(_n,_p,_q);              // on calcul la première partie de la clé publique

				mpz_sub_ui(_temp1,_p,1);
				mpz_sub_ui(_temp2,_q,1);
				mpz_mul(_phi,_temp1,_temp2);    // on calcule l'indicatrice d'euler de n

				gmp_printf("\tphi(%Zd) = %Zd\n",_n,_phi);
				gmp_printf("\nEntrez un nombre e, tel que Pgcd(%Zd,e) = 0 :\n",_phi);
                do { printf("\te = "); gmp_scanf("%Zd",_e);printf("\n"); mpz_inverseModulo(&_d,_e,_phi); } while( mpz_cmp_ui(_d,0)==0 );

				gmp_printf("%Zd",_e);
				gmp_printf("%Zd",_phi);

				gmp_printf("\n\td = %Zd , car (%Zd * %Zd = 1 mod %Zd )\n",_d,_d,_e,_phi);

				gmp_printf("\n\tCle Publique : { %Zd, %Zd }",_e,_n);
				gmp_printf("\n\tCle Privee   : { %Zd, %Zd }\n\n",_d,_n);

                mpz_set(clee_privee,_d);
				mpz_set(clee_publique,_e);
				mpz_set(clee_modulo,_n);
				clees_generees=1;
			break;

            case 8:
                printf("\n\n\t  * ENTREZ LES CLEES *\n");
                printf("\n\tClee publique = "); gmp_scanf("%Zd",clee_publique);
                printf("\tClee privee = ");   gmp_scanf("%Zd",clee_privee);
                printf("\tClee Modulo = ");   gmp_scanf("%Zd",clee_modulo);
                printf("\n\n");
                clees_generees=1;

			break;

            case 9: // Charger les clees a partir d'un fichier
                {
                    j=0;
                    do
                    {
                        printf("\n\n\t\t * CHARGER *\n( clee publique / privee / modulo)\n ");

                        printf("\n\t1 : Input  : %s",s_in);
                        printf("\n\t2 : separateur input  : ");
                            if (separator == '\n'){printf("retour a la ligne");}
                            if (separator == '\t'){printf("tabulation");}
                            if (separator == ' '){printf("espace");}
                        printf("\n\n\t0 : Charger !!! :");

                        scanf("%d",&menu);
                        if (menu == 1) {printf("\n\tnouveau fichier : "); scanf("%s",s_in);}
                        if (menu == 2)
                        {
                                printf("\n\tnouveau caractere de separation :");
                                printf("\n\t\t1 : retour a la ligne");
                                printf("\n\t\t2 : tabulation ");
                                printf("\n\t\t3 : espace ");
                                printf("\n\n\t\t0 : annuler ");
                                scanf("%d",&menu);
                                if (menu==1) { separator='\n';}
                                if (menu==2) { separator='\t';}
                                if (menu==3) { separator=' ';}
                        }

                    }while (menu!=0);

                    f_in = fopen(s_in,"r");

                    c = '0';
                    while(c != EOF)
                        {
                            i=0;
                            c = '0';
                            while(c != EOF && c!= separator)
                            {
                                c = fgetc(f_in);
                                if (c != EOF && c!= separator)
                                {
                                    s_temp[i]=c;
                                    i++;
                                }
                            }
                            if (c!=EOF)
                            {
                                s_temp[i]='\0';
                                if (j==0) {mpz_set_str(clee_publique,s_temp,10); }
                                if (j==1) {mpz_set_str(clee_privee  ,s_temp,10); }
                                if (j==2) {mpz_set_str(clee_modulo  ,s_temp,10); }
                                j++;
                            }
                        }

                    fclose(f_in);
                    printf("\nappuyez sur une touche pour continuer");
                    clees_generees=1;
                    getchar(); getchar();
                    j=0;
                }

			break;


			case 3: // auto crypte rabin
                mpz_mul_2exp(__max, __two, longeur_nombres );
                printf("\nGeneration de 2 nombres premiers p et q, : \n");
                do
                {
                    mpz_urandomm(__p, __rand_state, __max);

                    // a remplacer par une boucle propre sur les 100 plus petits premiers par exemple
                    if (mpz_even_p(__p)) continue;
                    if (mpz_fdiv_ui(__p, 3) == 0) continue;
                    if (mpz_fdiv_ui(__p, 5) == 0) continue;
                    if (mpz_fdiv_ui(__p, 7) == 0) continue;
                } while (miller_rabin(__p, __rand_state, 100) == 0);
                do
                {
                    mpz_urandomm(__q, __rand_state, __max);

                    // a remplacer par une boucle propre sur les 100 plus petits premiers par exemple
                    if (mpz_even_p(__q)) continue;
                    if (mpz_fdiv_ui(__q, 3) == 0) continue;
                    if (mpz_fdiv_ui(__q, 5) == 0) continue;
                    if (mpz_fdiv_ui(__q, 7) == 0) continue;
                } while (miller_rabin(__q, __rand_state, 100) == 0);
                do
                {
                    mpz_urandomm(_e, __rand_state, __max);

                    // a remplacer par une boucle propre sur les 100 plus petits premiers par exemple
                    if (mpz_even_p(_e)) continue;
                    if (mpz_fdiv_ui(_e, 3) == 0) continue;
                    if (mpz_fdiv_ui(_e, 5) == 0) continue;
                    if (mpz_fdiv_ui(_e, 7) == 0) continue;
                } while (miller_rabin(_e, __rand_state, 100) == 0);
                gmp_printf("\n\tp = %Zd,\n\n\tq = %Zd",__p,__q);

                mpz_mul(_n,__p,__q);            // on calcul la première partie de la clé publique
				mpz_sub_ui(_temp1,__p,1);
				mpz_sub_ui(_temp2,__q,1);
				mpz_mul(_phi,_temp1,_temp2);    // on calcule l'indicatrice d'euler de n

				gmp_printf("\n\n\tphi(%Zd) \n\t= %Zd\n",_n,_phi);
				mpz_inverseModulo(&_d,_e,_phi);

				gmp_printf("\n\tCle Publique : \n\t{ %Zd,\n\t%Zd }",_e,_n);
				gmp_printf("\n\n\tCle Privee   : \n\t{ %Zd,\n\t%Zd }\n\n",_d,_n);

                mpz_set(clee_privee,_d);
				mpz_set(clee_publique,_e);
				mpz_set(clee_modulo,_n);
				clees_generees=1;
            break;

            case 4: // crypte
                if (clees_generees==1)
                {
                    do
                    {
                        printf("\n\n\t * CRYPTAGE *\n");

                        printf("\n\t1 : Input  : %s",s_in);
                        printf("\n\t2 : Output : %s",s_out);
                        printf("\n\t3 : Base   : %d",base_cryptage);
                        printf("\n\t4 : separateur : ");
                            if (separator == '\n'){printf("retour a la ligne");}
                            if (separator == '\t'){printf("tabulation");}
                            if (separator == ' '){printf("espace");}
                        printf("\n\n\t0 : CRYPTER !!! :");

                        scanf("%d",&menu);
                        if (menu == 1) {printf("\n\tnouveau fichier : "); scanf("%s",s_in);}
                        if (menu == 2) {printf("\n\tnouveau fichier : "); scanf("%s",s_out);}
                        if (menu == 3) {printf("\n\tnouvelle base ( entre 2 et 36) : "); scanf("%d",&base_cryptage);
                                        if (base_cryptage<2 || base_cryptage>36) {base_cryptage=10;}}
                        if (menu == 4)
                        {
                                printf("\n\tnouveau caractere de separation :");
                                printf("\n\t\t1 : retour a la ligne");
                                printf("\n\t\t2 : tabulation ");
                                printf("\n\t\t3 : espace ");
                                printf("\n\n\t\t0 : annuler ");
                                scanf("%d",&menu);
                                if (menu==1) { separator='\n';}
                                if (menu==2) { separator='\t';}
                                if (menu==3) { separator=' ';}
                        }
                    }while (menu!=0);

                    f_in = fopen(s_in,"r");
                    f_out = fopen(s_out,"w+");

                    c = ' ';
                    while(c != EOF)
                    {
                        c = fgetc(f_in);
                        if (c!=EOF)
                        {
                            mpz_set_ui(_temp1,(unsigned int)c);
                            mpz_powm(_temp1,_temp1,clee_publique,clee_modulo);
                            mpz_out_str (f_out, base_cryptage, _temp1 );
                            fputc(separator,f_out);
                        }
                    }

                    fclose(f_in);
                    fclose(f_out);
                    printf("\nappuyez sur une touche pour continuer");
                    getchar(); getchar();
                }
                else
                {
                    printf("\nVous devez d'abord generer un set de Clees");
                    printf("\nappuyez sur une touche pour continuer");
                    getchar(); getchar();
                }
			break;

			case 5: // Décrypte
                if (clees_generees==1)
                {
                    do
                    {
                        printf("\n\n\t * DECRYPTAGE *\n");

                        printf("\n\t1 : Input  : %s",s_in);
                        printf("\n\t2 : Output : %s",s_out);
                        printf("\n\t3 : Base   : %d",base_cryptage);
                        printf("\n\t4 : separateur input  : ");
                            if (separator == '\n'){printf("retour a la ligne");}
                            if (separator == '\t'){printf("tabulation");}
                            if (separator == ' '){printf("espace");}
                        printf("\n\n\t0 : DECRYPTER !!! :");

                        scanf("%d",&menu);
                        if (menu == 1) {printf("\n\tnouveau fichier : "); scanf("%s",s_in);}
                        if (menu == 2) {printf("\n\tnouveau fichier : "); scanf("%s",s_out);}
                        if (menu == 3) {printf("\n\tnouvelle base ( entre 2 et 36) : "); scanf("%d",&base_cryptage);
                                        if (base_cryptage<2 || base_cryptage>36) {base_cryptage=10;}}
                        if (menu == 4)
                        {
                                printf("\n\tnouveau caractere de separation :");
                                printf("\n\t\t1 : retour a la ligne");
                                printf("\n\t\t2 : tabulation ");
                                printf("\n\t\t3 : espace ");
                                printf("\n\n\t\t0 : annuler ");
                                scanf("%d",&menu);
                                if (menu==1) { separator='\n';}
                                if (menu==2) { separator='\t';}
                                if (menu==3) { separator=' ';}
                        }

                    }while (menu!=0);

                    f_in = fopen(s_in,"r");
                    f_out = fopen(s_out,"w+");

                    c = '0';
                    while(c != EOF)
                        {
                            i=0;
                            c = '0';
                            while(c != EOF && c!= separator)
                            {
                                c = fgetc(f_in);
                                if (c != EOF && c!= separator)
                                {
                                    s_temp[i]=c;
                                    i++;
                                }
                            }
                            if (c!=EOF)
                            {
                                s_temp[i]='\0';
                                mpz_set_str(_temp1,s_temp,base_cryptage);
                                mpz_powm(_temp1,_temp1,clee_privee,clee_modulo);
                                fputc((char)(mpz_get_ui(_temp1)),f_out);
                            }
                        }

                    fclose(f_in);
                    fclose(f_out);
                    printf("\nappuyez sur une touche pour continuer");
                    getchar(); getchar();
                }
                else
                {
                    printf("\nVous devez d'abord generer un set de Clees");
                    printf("\nappuyez sur une touche pour continuer");
                    getchar(); getchar();
                }
			break;

			case 10: // enregistre les clees
                if (clees_generees==1)
                {
                    do
                    {
                        printf("\n\n\t * SAUVER  *\n");

                        printf("\n\t1 : Output : %s",s_out);
                        printf("\n\t2 : separateur : ");
                            if (separator == '\n'){printf("retour a la ligne");}
                            if (separator == '\t'){printf("tabulation");}
                            if (separator == ' '){printf("espace");}
                        printf("\n\n\t0 : CRYPTER !!! :");

                        scanf("%d",&menu);
                        if (menu == 1) {printf("\n\tnouveau fichier : "); scanf("%s",s_out);}
                        if (menu == 2)
                        {
                                printf("\n\tnouveau caractere de separation :");
                                printf("\n\t\t1 : retour a la ligne");
                                printf("\n\t\t2 : tabulation ");
                                printf("\n\t\t3 : espace ");
                                printf("\n\n\t\t0 : annuler ");
                                scanf("%d",&menu);
                                if (menu==1) { separator='\n';}
                                if (menu==2) { separator='\t';}
                                if (menu==3) { separator=' ';}
                        }
                    }while (menu!=0);

                    f_out = fopen(s_out,"w+");

                    mpz_out_str (f_out, 10, clee_publique );
                    fputc(separator,f_out);

                    mpz_out_str (f_out, 10, clee_privee );
                    putc(separator,f_out);

                    mpz_out_str (f_out, 10, clee_modulo );
                    fputc(separator,f_out);

                    fclose(f_out);
                    printf("\nappuyez sur une touche pour continuer");
                    getchar(); getchar();
                }
                else
                {
                    printf("\nVous devez d'abord generer un set de Clees");
                    printf("\nappuyez sur une touche pour continuer");
                    getchar(); getchar();
                }
			break;

            case 6: // Test de primalité
                printf("Entrez un nombre, et nous vous dirrons s'il est premier ou pas :\n\t n = ");
                gmp_scanf("%Zd",__n);
                if (miller_rabin(__n, __rand_state, 100) == 1)
                        { printf("\n PROBABLEMENT, ce nombre est probablement premier ."); }
                else    { printf("\n NON, ce nombre n'est pas premier !"); }
			break;
			default : printf("  -> Choix non reconnu !"); break;
		}
    }

	return 0;
}

int min(int x, int y) {	return ( x > y ) ? y : x; }  // sans GMP
int max(int x, int y) {	return ( x < y ) ? y : x; }  // sans GMP

void mpz_min(mpz_t *v, mpz_t x, mpz_t y) {	mpz_set(*v,(mpz_cmp(x,y)> 0) ? y : x); }  // Avec GMP
void mpz_max(mpz_t *v, mpz_t x, mpz_t y) {	mpz_set(*v,(mpz_cmp(x,y)> 0) ? y : x); }  // Avec GMP



int inverseModulo(int a, int b) // renvoi l'inverse de a mod b ou 0 si pas inversible
{
    int r;
    int u2 = 1;
    int u1 = 0;
    int u =0;

    while (b > 0)
    {
        u1=u;
        r = a % b;
				printf("\t( %d mod %d = %d )",a, b, r); 			//ligne de débeug

        u=u2-u1*(a-r)/b;
        u2=u1;
				printf("(%d,%d,%d)",u2, u1, u);						//ligne de débeug
        		printf("  -->  (%d = (%d * 120) + b*22)\n",r, u);	//ligne de débeug

		if (r == 1) {printf("\n"); return u;}
		else		{ a = b; b = r; }
    }

	printf("\t\t-> Ces deux nombres ne sont pas premiers entre eux\n");
    return (0); // Les deux nombres ne sont pas inversibles, donc pas premiers entr eeux
}
void mpz_inverseModulo(mpz_t *v, mpz_t _aa, mpz_t _bb) // renvoi l'inverse de a mod b ou 0 si pas inversible
{
    mpz_t _temp; mpz_init(_temp);
    mpz_t _r;    mpz_init(_r);

    mpz_t _a;   mpz_init(_a); mpz_set(_a,_aa);
    mpz_t _b;   mpz_init(_b); mpz_set(_b,_bb);

    mpz_t _u2;   mpz_init(_u2); mpz_set_ui(_u2,1);
    mpz_t _u1;   mpz_init(_u1); mpz_set_ui(_u1,0);
    mpz_t _u;    mpz_init(_u);  mpz_set_ui(_u,0);

    while (mpz_cmp_ui(_b,0) > 0 )
    {
        mpz_set(_u1,_u);
        mpz_mod(_r,_a,_b);

        mpz_tdiv_q(_temp,_a ,_b);

        mpz_mul   (_temp,_u1,_temp);
        mpz_sub   (_u   ,_u2,_temp);
        mpz_set   (_u2  ,_u1);

		if (mpz_cmp_ui(_r,1)==0) {printf("\n"); mpz_set(*v,_u); return ; }
		else		{ mpz_set(_a,_b);  mpz_set(_b,_r); }

    }

    mpz_set_ui(*v,0); // Les deux nombres ne sont pas inversibles, donc pas premiers entr eeux
}


int premier(int a)
{
    int i;
    for(i=2; i</*sqrt(a)*/a-1; i++) { if (a%i==0) {return 0;} }
    return 1;
}
int mpz_premier(mpz_t _a)
{
    mpz_t _root_temp;
        mpz_init(_root_temp);
        mpz_root(_root_temp,_a,2);
        mpz_add_ui(_root_temp,_root_temp,1);
    mpz_t _i; mpz_init(_i);
    mpz_t _k; mpz_init(_k);
    mpz_set_ui(_i,2);
    while (mpz_cmp(_i,_root_temp)< 0)
        { mpz_mod(_k,_a,_i); mpz_add_ui(_i,_i,1); if (mpz_cmp_ui(_k,0)==0) {return 0;} }
    return 1;
}


int miller_rabin_pass(mpz_t a, mpz_t n)
{
    int i, s;
    mpz_t a_to_power, d, n_minus_one;

    mpz_init(n_minus_one);
    mpz_sub_ui(n_minus_one, n, 1);

    //on calcule s et d
    s = 0;
    mpz_init_set(d, n_minus_one);
    while (mpz_even_p(d))
    {
        mpz_fdiv_q_2exp(d, d, 1);
        s++;
    }

    mpz_init(a_to_power);
    mpz_powm(a_to_power, a, d, n);
    if (mpz_cmp_ui(a_to_power, 1) == 0)
    {
        mpz_clear(a_to_power);
        mpz_clear(d);
        mpz_clear(n_minus_one);
        return 1;
    }

    for(i=0; i < s-1; i++)
    {
        if (mpz_cmp(a_to_power, n_minus_one) == 0)
        {
            mpz_clear(a_to_power);
            mpz_clear(d);
            mpz_clear(n_minus_one);
            return 1;
        }
        mpz_powm_ui (a_to_power, a_to_power, 2, n);
    }

    if (mpz_cmp(a_to_power, n_minus_one) == 0)
    {
        mpz_clear(a_to_power);
        mpz_clear(d);
        mpz_clear(n_minus_one);
        return 1;
    }
    return 0;
}
int miller_rabin(mpz_t n, gmp_randstate_t rand_state,int nb_bases_test)
{
    mpz_t a;
    int i;
    mpz_init(a);
    for(i=0; i<nb_bases_test; i++)
    {
        do { mpz_urandomm(a, rand_state, n); } while (mpz_sgn(a) == 0);
        if (miller_rabin_pass(a, n) == 0)
            { return 0;}
    }
    return 1;
}
