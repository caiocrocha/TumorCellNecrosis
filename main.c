#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Funcoes auxiliares

//Distancia r da origem
double get_distance(double x, double y)
{
    return sqrt(x*x + y*y);
}

//Media harmonica para calcular k_{i+1/2} ou k_{i-1/2}
double harmonic_mean(double kp, double km)
{
    return 2.0*kp*km/(kp+km);
}

//Variaveis dependentes de r
//se r > 0.01m, k do tecido saudavel, se não, k do tumor
double get_k(double r)
{
    return (r > 0.01) ? 0.5 : 0.55;
}

//se r > 0.01m, w_b do tecido saudavel, se não, w_b do tumor
double get_wb(double r)
{
    return (r > 0.01) ? 0.0005 : 0.00125;
}

//funcao do calor externo Qr para um ponto de aplicacao da injecao
double get_q(double r)
{
    return 1300000.0 * exp(-r*r/(0.0031*0.0031));
}

//generate a range of evenly spaced numbers within [start, end) just like numpy's arange function
void arange(double *x, double start, double end, double step)
{
    int i;
    for(i = 0; i < (int) ceil((end-start)/step); i++)
    {
        x[i] = start + i * step;
    }
} 

int main(int argc, char *argv[])
{
    //define space discretization
    double h_x=0.001;
    double start_x=-0.05;
    double end_x=0.05;

    //define time discretization
    double h_t=0.01;
    double end_t = 1000.0;
    int sol_step = 1; //print solution every 1s of simulation

    //number of iterations
    int size = (int) ceil((end_x-start_x+h_x)/h_x);
    int steps = (int) ceil((end_t+h_t)/h_t);
    int sol_size = (int) ceil((end_t+1)/sol_step);
    
    //declare arrays in space
    double x[size]; 
    double y[size]; 
    arange(x, start_x, end_x+h_x, h_x);
    arange(y, start_x, end_x+h_x, h_x);

    //declare array in time
    double t[steps];
    arange(t, 0, end_t+h_t, h_t);

    //initial condition
    double u_0=37.0;

    //diffusion constants
    double pb=1000.0;
    double cb=4200.0;
    double kappa = 1.0/(pb*cb);
    double r_dif=kappa*h_t/(h_x*h_x);

    //advection constants
    double a_x=0, a_y=0;
    if(argc == 2)
    {
        if(strcmp(argv[1], "true") == 0)
        {
            a_x=1e-4;
            a_y=1e-4;
        }
        else if(strcmp(argv[1], "false"))
        {
            printf("Invalid argument: %s\n", argv[1]);
            exit(1);
        }
    }
    else
    {
        printf("One argument expected: true (with advection) or false (without advection).\n");
        exit(1);
    }
    
    double r_adv_x=a_x*h_t/h_x;
    double r_adv_y=a_y*h_t/h_x;

    //boundary conditions
    double u_a = 37.0; //dirichlet on left
    double u_b = 0.0; //neumann on top
    double u_c = 0.0; //neumann on right
    double u_d = 0.0; //neumann on bottom

    //declare matrices
    double u[size][size];
    double K[size][size];
    double Sigma[size][size];
    double Q[size][size];

    int i, j;
    for(i = 0; i < size; i++)
    {
        for(j = 0; j < size; j++)
        {
            u[i][j] = u_0;
            double r = get_distance(x[i],y[j]);
            K[i][j] = get_k(r);
            Sigma[i][j] = get_wb(r)*pb*cb;
            Q[i][j] = get_q(r);
        }
    }

    double u_new[size][size];

    int k, i_aux, j_aux;
    for(k = 0; k < steps; k++)
    {
        for(i = 0; i < size; i++)
        {
            for(j = 0; j < size; j++)
            {
                double kijp = j==size-1 ? harmonic_mean(K[i][size-2], K[i][j]) : harmonic_mean(K[i][j+1], K[i][j]);
                double kijm = j==0 ? harmonic_mean(K[i][1], K[i][j]) : harmonic_mean(K[i][j-1], K[i][j]);
                double kipj = i==size-1 ? harmonic_mean(K[size-2][j], K[i][j]) : harmonic_mean(K[i+1][j], K[i][j]);
                double kimj = i==0 ? harmonic_mean(K[1][j], K[i][j]) : harmonic_mean(K[i-1][j], K[i][j]);
                //para tratar do tipo Dirichlet, devemos atribuir diretamente no ponto (x_i,y_j)
                if (j==0)
                {
                    u_new[i][j] = u_a;
                }
                else
                {
                    //Tratando condicoes de contorno do tipo Neumann
                    double uijp = j==size-1 ? u_new[i][size-2] : u_new[i][j+1];
                    double uipj = i==size-1 ? u_new[size-2][j] : u_new[i+1][j];
                    double uimj = i == 0 ? u_new[1][j] : u_new[i-1][j];
                    double uijm = u_new[i][j-1];

                    double f = Sigma[i][j]*(u_0-u[i][j]) + Q[i][j]; //funçao do lado direito f

                    double phi_x, phi_y;
                    if(a_x > 0)
                    {
                        phi_x=r_adv_x*(u[i][j]-uimj);
                    }
                    else
                    {
                        phi_x=r_adv_x*(uipj-u[i][j]);
                    }
                    if(a_y > 0)
                    {
                        phi_y=r_adv_y*(u[i][j]-uijm);
                    }
                    else
                    {
                        phi_y=r_adv_y*(uijp-u[i][j]);
                    }

                    u_new[i][j] = u[i][j] + f*kappa*h_t - phi_x - phi_y + r_dif*(kipj*uipj + kimj*uimj + kijp*uijp + kijm*uijm - (kipj + kimj + kijp + kijm)*u[i][j]);
                }
            }
        }
        for(i_aux = 0; i_aux < size; i_aux++)
        {
            for(j_aux = 0; j_aux < size; j_aux++)
            {
                u[i_aux][j_aux] = u_new[i_aux][j_aux];
                if(k%((int) ceil(sol_step/h_t))==0)
                {
                    printf("%.15f ", u[i_aux][j_aux]);
                }
            }
        }
        if(k%((int) ceil(sol_step/h_t))==0)
        {
            printf("\n");
        }
    }

    return 0;
}
