# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <omp.h>

#define MSIZE	1000

int main(void)

{
  double a[MSIZE][MSIZE], b[MSIZE][MSIZE], c[MSIZE][MSIZE];
  int i, j, k;
  int n = MSIZE;
  double pi = 3.141592653589793;
  double s, angle;
  double wtime, wtime1, wtime2, wtime3;


  printf ( "\n" );
  printf ( "  Compute matrix product C = A * B\n" );

  printf ( "\n" );
  printf ( "  The number of processors available = %d\n", omp_get_num_procs());
  printf ( "  The number of threads available    = %d\n", omp_get_max_threads());

  printf ( "  The matrix order N                 = %d\n", n );

  
  /*
  Loop 1: Evaluate A.
  */
  s = 1.0 / sqrt ( ( double ) ( n ) );

  wtime = omp_get_wtime();
  wtime1 = omp_get_wtime();

  #pragma omp parallel
  for (i = 0; i < n; i++)
  {
    #pragma omp for
    for (j = 0; j < n; j++)
    {
      angle = 2.0 * pi * i * j / (double) n;
      a[i][j] = s * (sin (angle) + cos (angle));
    }
  }

  wtime1 = omp_get_wtime ( ) - wtime1;
	  
  /*
    Loop 2: Copy A into B.
  */

  wtime2 = omp_get_wtime();

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      b[i][j] = a[i][j];
    }
  }

  wtime2 = omp_get_wtime ( ) - wtime2;

  /*
    Loop 3: Compute C = A * B.
  */

  wtime3 = omp_get_wtime();

  for (i = 0; i < n; i++)
  {
    for ( j = 0; j < i; j++ ) // n
    {
      c[i][j] = 0.0;
      for (k = 0; k < j; k++) // n
      {
        c[i][j] += sin(a[i][k] * b[k][j]);
      }
    }
  }
  
  wtime3 = omp_get_wtime ( ) - wtime3;
  wtime = omp_get_wtime() - wtime;
  
  printf ( "\n" );
  printf ( "  Elapsed time loop 1 = %g seconds\n", wtime1 );
  printf ( "  Elapsed time loop 2 = %g seconds\n", wtime2 );
  printf ( "  Elapsed time loop 3 = %g seconds\n", wtime3 );

  printf ( "\n" );
  printf ( "  Total lapsed time = %g seconds\n", wtime );

  printf ( "\n" );
  printf ( "  Normal end of execution.\n" );

  return 0;
}
/******************************************************************************/

