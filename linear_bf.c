#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <assert.h>


//GSL
#include <gsl/gsl_rng.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>


#define DELTA  1.0/6.0
#define OMEGA  1.0/5.0
#define PI  3.141592653



extern  double  gsl_ran_gaussian(gsl_rng *r, double sigma);
extern  double  gsl_ran_flat (const gsl_rng * r, double a, double b);
extern  unsigned int gsl_ran_binomial (const gsl_rng * r, double p, unsigned int n);


extern  double  gsl_ran_gaussian_pdf (double x, double sigma);
extern  double gsl_ran_hypergeometric_pdf (unsigned int k, unsigned int n1, unsigned int n2, unsigned int t);



double capF(double p, double a){
	double out;
	out=DELTA*(sqrt(PI)/2)*erf(a*p/DELTA);

	return out;
}

double func_a(double a, int n, int k){

	double out;
	double phat= (double)k/(double)n;
	
	double temp3 = a*phat;

	double temp4 = (1-OMEGA)+capF(phat-(1-OMEGA)/a, a);
  	
  	if (phat<(1-OMEGA)/a) {
  		out=temp3;
  	}
  	else{
  		out=temp4;
  	}
 	
 	return out;
 }


double func_b(double a, int n, int k){
	double out;
	double a1=func_a(a, n, k);
	double C=pow(a,2)*sqrt(2.0)/( DELTA*sqrt(expf(1)) );
	out=a1+C/(double)(2*n);

	return out;
}


int bernfactory(double a, double ptrue, gsl_rng *RRR){

	int W;

    int j;
    //i=1;
    //int give_log = 0;
    int n=2; //2^i 
    double lhh = 0.0;
    double uhh = 1.0;
    double lh = 0.0;
    double uh = 1.0;
    double lhhpr = 0.0;
    double uhhpr = 1.0;

    double lstar,ustar;

    while (func_b(a, n, n) > 1){
        n = 2*n;
    }
    
    
    int nstar = n;
    
    
    double G0;
  	G0 = gsl_ran_flat(RRR, 0.0, 1.0);

    double H,Hpr;	
    Hpr = gsl_ran_binomial(RRR, ptrue, n/2);

    int k=0;
    while ((G0 <= uhh) && (G0 > lhh)){ 
    	
    	k = k + 1;
    	
        H = Hpr + gsl_ran_binomial(RRR, ptrue, n/2);
        
    	lh = func_a(a, n, H);
    	uh = func_b(a, n, H);
    	
        
    	
    	if (n == nstar){ 
            lstar = 0.0;
            ustar = 1.0;
        }
    	else{ 
            lstar = 0.0;
            ustar = 0.0;
            for(j=0;j<(H+1);j++){
                //prob = hygepdf(j, n, H, n/2);
            	double prob = gsl_ran_hypergeometric_pdf(j, H, n-H, n/2);
                lstar = lstar + prob * func_a(a, n/2, j);
                ustar = ustar + prob * func_b(a, n/2, j);
            }
        }
     
     
     	lhh = lhhpr + (lh - lstar)*(uhhpr - lhhpr)/(ustar - lstar);
     	uhh = uhhpr + (uh - ustar)*(uhhpr - lhhpr)/(ustar - lstar);
     	Hpr = H;
     	lhhpr = lhh;
     	uhhpr = uhh;
     	n = 2*n;
    }	
    
    if (G0 <= lhh){ 
        W = 1;
    }
    if (G0>uhh){
    	W=0;
    }

    
    return W;
}





int main(void){

	double ptrue = 0.2342;
	double a = 2.53;

	//------------------------------------------------------------------------
	// initial random number setup
	const gsl_rng_type * TTT;
  	gsl_rng *RRR;
  	gsl_rng_env_setup();
  	if (!getenv("GSL_RNG_SEED")) gsl_rng_default_seed = time(0);

	TTT = gsl_rng_default;
  	RRR = gsl_rng_alloc (TTT);	
	//------------------------------------------------------------------------

	int b,B=10000;
  	int *Wkeep;
  	Wkeep = (int*)malloc(B*sizeof(int));

  	int SS=0;

	for(b=0;b<B;b++){
		printf("%d \n", b);
		Wkeep[b] = bernfactory(a, ptrue, RRR);
		SS = SS + Wkeep[b];
	}



	printf("%.5f \n", (double)SS/(double)B);
	printf("%.5f \n", a*ptrue);




	return 1;
}