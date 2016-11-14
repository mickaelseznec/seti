#include <stdio.h>

#define N 1000		// la taille des matrices/vecteurs
#define M 10		// nombre d'itérations de chaque fonction
#define TYPE float	// ou double ou int, etc. On peut essayer les
			// différents types de vecteurs


// Permet de lire le compteur interne du nombre de cycles du
// processeur (TSC timestamp counter).
// NB codé sur 64 bits (long long)
unsigned long long read_cycles() 
{ 
  unsigned int hi = 0, lo = 0; 
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi) : : 
			"memory" ); 
  unsigned long long that =   (unsigned long long)((lo)|
						   (unsigned long long)(hi)<<32);

  return that; 
} 

// convertit le TSC en flottant
double dtime()  
{ 
  unsigned long long ll = read_cycles();
  double dd = (double) ll;
  return (double) dd; 
}


void exemple(){
  double t1,t2, dt ;//déclaration des variables 
  t1 = dtime(); 
  //Partie du programme dont on mesure le temps d'exécution.   
  t2 = dtime(); 
  dt = t2-t1; // Nombre de cycles d'horloge processeur 
}


TYPE AF[N][N], YF[N][N], XF[N][N], YT[N][N],	// matrices [N][N]
  BF[N*N], CF[N*N],				// vecteurs N^2
  SF;						// accumulateur

#define STR1(x) #x
#define STR(x) STR1(x)
#define N3inv (1/((double)N*N*N))
			// pour éviter les débordements si N^3>2^32

double begin, benchtime;


int min (int a,int b)
{
  if (a<b) return a;
  else return b; 
}

double begin, benchtime;

//Mise à zéro d'un vecteur
void zero(){

  int i, j, m ;
  
  for (m=0;m<M;m++)
    {
      begin=dtime();
      for (i=0;i<(N*N);i++)
	BF[i]=0.0;
      benchtime=dtime() - begin;
      printf ("ZERO\t%g\n", (double) benchtime/(N*N));
    }
  printf ("\n");
}

//copie de matrices
void copy_ij(){

  int i, j, m ;
  
  for (m=0;m<M;m++)
    {
      begin=dtime();
      for (i=0;i<N;i++)
	for (j=0;j<N;j++)
	  AF[i][j]=YF[i][j];
      benchtime=dtime() - begin;
      printf ("COPY_ij\t%g\n", (double) benchtime/(N*N));
    }
  printf ("\n");
}

void copy_ji(){

  int i, j, m ;
  
  for (m=0;m<M;m++)
    {
      begin=dtime();
      for (j=0;j<N;j++)
	for (i=0;i<N;i++)
	  AF[i][j]=YF[i][j];
      benchtime=dtime() - begin;
      printf ("COPY_ji\t%g\n", (double) benchtime/(N*N));
    }
  printf ("\n");
}

// addition de deux matrices
void add_ij(){

  int i, j, m ;

  for (m=0;m<M;m++)
    {
      begin=dtime();
      for (i=0;i<N;i++)
	for (j=0;j<N;j++)
	  AF[i][j]+=YF[i][j];
      benchtime=dtime() - begin;
      printf ("ADD_ij\t%g\n", (double) benchtime/(N*N));
    }
  printf ("\n");
}

void add_ji(){

  int i, j, m ;

  for (m=0;m<M;m++)
    {
      begin=dtime();
      for (j=0;j<N;j++)
	for (i=0;i<N;i++)
	  AF[i][j]+=YF[i][j];
      benchtime=dtime() - begin;
      printf ("ADD_ji\t%g\n", (double) benchtime/(N*N));
    }
  printf ("\n");
}


// Produit scalaire 
void ps()
{
  int i, j, k, m;
  
  for (m=0;m<M;m++)
    {
      begin=dtime();
      SF=0.0;
      for (i=0;i<(N*N);i++)
	SF+=BF[i]*CF[i]; 
      benchtime=dtime() - begin;
      printf ("PS \t%g\n", (double) benchtime/(N*N));
    }
  printf ("\n");
}


// Multiplication de matrices ijk
void mm_ijk()
{
  int i, j, k, m;
  
  for (m=0;m<M;m++)
    {
      begin=dtime();
      for (i=0;i<N;i++)
	for (j=0;j<N;j++)
	  {
	    SF=0.0;
	    for (k=0; k<N; k++)
	      SF+=AF[i][k]*XF[k][j];
	    YF[i][j]=SF;
	  }
      benchtime=dtime() - begin;
      printf ("MM_ijk\t%g\n", (double) benchtime*N3inv);
    }
  printf ("\n");
}


// Multiplication de matrices ikj
void mm_ikj()
{
  int i, j, k, m;
  for (m=0;m<M;m++)
    {
      begin=dtime();
      for (i=0;i<N;i++)
	for (k=0;k<N;k++)
	  {
	    SF=AF[i][k];
	    for (j=0; j<N; j++)
	      YF[i][j]+=SF*XF[k][j];
	  }
      benchtime=dtime() - begin;
      printf ("MM_ijk\t%g\n", (double) benchtime*N3inv);
    }
  printf ("\n");
}

#define BL 16

// Multiplication de matrices par blocs
void mm_b_ijk(){
  int i, j, k, m, ii, jj, kk;
  
  for (m=0;m<M;m++)
    {
      begin=dtime();
      for (jj=0;jj<N;jj+=BL)
	for (kk=0;kk<N;kk+=BL)
	  for (i=0;i<N;i++)
	    {
	      for (j=jj;j<min(jj+BL-1,N);j++)
		{
		  SF=0.0;
		  for (k=kk;k<min(kk+BL-1,N);k++)
		      SF += AF[i][k]*XF[k][j];
		  YF[i][j]=SF;
		}
	    }
      benchtime=dtime() - begin;
      printf ("MM_B_ijk\t%g\n", (double) benchtime*N3inv);
    }
  printf ("\n");
}



void main()
{
  printf("Evaluation : N=%d, type="STR(TYPE)"\n",N);
  zero();
//  copy_ij();
//  copy_ji();
//  add_ij();
//  add_ji();
//  ps();
//  mm_ijk();
//  mm_ikj();
//  mm_b_ijk();
} 
