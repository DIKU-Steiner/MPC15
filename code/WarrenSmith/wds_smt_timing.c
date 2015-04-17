/* Simplified Steiner tree backtrack code. Warren D. Smith Jan 1989
 * Program contains about 341 C instructions. See main() for I/O details; for an
 * input & output example, see table 1 of the text. */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef XWINDOWS
#include <LEDA/window.h>
#include <LEDA/panel.h>
window W("wds_smt");
#endif

#define REAL double
#define SQROOT(a) sqrt(a)
#define RANDM() drand48() /* uniform random deviates in (0,1) */
#define INITRAND() srand48(57731)
#define DoForever while(1)

#define MAXDIM 12  /* max space dimension permitted */
#define NMAX 100   /* max number of sites permitted */
/* Global variables: */
REAL STUB, SCALE;
int NUMSITES, DIMENSION, N;
static int BESTVEC[NMAX], STACK[NMAX*NMAX], adj[NMAX-2][3], edge[2*NMAX][2];
static REAL XX[NMAX*2][MAXDIM], LEN[NMAX], EL[NMAX][3];

/* SMT representation: Steiner point i is adjacent to points adj[i][0..2].
 * (If neighbor is Steiner - NUMSITES added!)
 * There are N-2 Steiner points and N regular sites.
 * The coordinates of Steiner point i are XX[i+NUMSITES][0..DIMENSION-1], and
 * The coordinates of regular site i are XX[i][DIMENSION-1], i=1,2,..
 * Edge i has endpoints edge[i][0] < edge[i][1] (sam NUMSITES-add convention).
 */

void buildtree(int k, int topvec[])
{ /* builds tree represented by topvec[1..k]. Initial location of new Steiner pts
   * is a slightly random perturbation of the centroid of its neighbors */
	register int i, e, m, j, sn, ea, eb, en;
	/* First build the tree corresponding to the null vector */
	N = 3; m = NUMSITES+1;
	adj[1][0] = 1; adj[1][1] = 2; adj[1][2] = 3;
	edge[1][0] = 1; edge[2][0] = 2; edge[3][0] = 3;
	edge[1][1] = m; edge[2][1] = m; edge[3][1] = m;
	for (i = 0; i < DIMENSION; i++)
		XX[m][i] = (XX[1][i]+XX[2][i]+XX[3][i])/3.0 + 0.001*SCALE*RANDM(); 

	for (i = 1; i <= k; i++){ /* Now: do vector element topvec[i] */
		en = i+3; m = i+1; sn = m+NUMSITES;
		e = topvec[i]; ea = edge[e][0]; eb = edge[e][1];
		adj[m][0] = ea; adj[m][1] = eb; adj[m][2] = en;
		m = ea - NUMSITES;
		if (m > 0) for (j = 0; j < 3; j++) if (adj[m][j] == eb) { adj[m][j] = sn; break; }
		m = eb - NUMSITES;
		if (m > 0) for (j = 0; j < 3; j++) if (adj[m][j] == ea) { adj[m][j] = sn; break; }
		edge[e][1] = sn; e = en+en-4; edge[e][0] = en; edge[e][1] = sn;
		e++; edge[e][0] = eb; edge[e][1] = sn;
		for (j = 0; j < DIMENSION; j++)
			XX[sn][j] = (XX[ea][j] + XX[eb][j] + XX[en][j])/3.0 + 0.001*SCALE*RANDM();
	}
	N = k + 3; /* Tree is now built. Initial coords in general position. */ return;
}

REAL length()
{ /* Stores edge lengths of tree T in array EL[1..k1][0..2] and returns total length. */
#define dist(a,b) t = 0.0; for (m = 0; m < DIMENSION; m++) { r = XX[a][m] - XX[b][m]; t += r*r; } \
	t = SQROOT(t);
	register int m, i2, i, j; int n0, n1, n2, k1; REAL leng, t, r;
	leng = 0.0; k1 = N - 2;
	for (i = 1; i <= k1; i++) {
		i2 = i + NUMSITES;
		n0 = adj[i][0]; n1 = adj[i][1]; n2 = adj[i][2];
		if (n0 < i2) {
			dist(n0, i2); leng += t; EL[i][0] = t; n0 -= NUMSITES;
			if (n0 > 0) for (j = 0; j < 3; j++) if (adj[n0][j] == i2) { EL[n0][j] = t; break; }
		}
		if (n1 < i2) {
			dist(n1, i2); leng += t; EL[i][1] = t; n1 -= NUMSITES;
			if (n1 > 0) for (j = 0; j < 3; j++) if (adj[n1][j] == i2) { EL[n1][j] = t; break; }
		}
		if (n2 < i2) {
			dist(n2, i2); leng += t; EL[i][2] = t; n2 -= NUMSITES;
			if (n2 > 0) for (j = 0; j < 3; j++) if (adj[n2][j] == i2) { EL[n2][j] = t; break; }
		}
	} /* Have now figured out distance EL[i][00.3] from Steiner pt. i to neighbors. */
	return (leng);
}

void optimize(REAL tol)
	/* tol: a small positive number */
{ /* finds better coordinates XX[NUMSITES+1..NUMSITES+k1][] for the k1 Steiner points
   * of tree T by: doing a relaxation iteration. Assumes that edge lengths of old tree
   * have been per-stored in array EL[][] */
#define prep(a,b,c) if (b > NUMSITES) { val[i]++; B[i][a] = c; } \
	else for (m = 0; m < DIMENSION; m++) C[i][m] += XX[b][m]*c;
	static REAL B[NMAX][3], C[NMAX][MAXDIM];
	static int eqnstack[NMAX], leafQ[NMAX], val[NMAX];
	register int i, m, j, i2;  int n0, n1, n2, lqp, eqp, k1;  REAL q0, q1, q2, t;
	lqp = eqp = 0; k1 = N - 2;
	/* First: compute B array, C array, and valences. Set up leafQ. */
	for (i = k1; i > 0; i--) {
		n0 = adj[i][0]; n1 = adj[i][1]; n2 = adj[i][2];
		q0 = 1.0 / (EL[i][0] + tol); q1 = 1.0 / (EL[i][1] + tol); q2 = 1.0 / (EL[i][2] + tol);

		/* printf("q: %20.20g %20.20g %20.20g\n", q0, q1,  q2); */

		/* Have now figured out reciprocal distances q0, q1, q2 from
		 * Steiner pt. i to neighbors n0, n1, n2 */
		t = q0 + q1 + q2; q0 /= t; q1 /= t; q2 /= t;
		val[i] = 0; B[i][0] = B[i][1] = B[i][2] = 0.0;
		for (m = 0; m < DIMENSION; m++) { C[i][m] = 0.0; }

		prep(0, n0, q0); prep(1, n1, q1); prep(2, n2, q2);

		/* printf("SP: %20.20g %20.20g\n", XX[i + NUMSITES][0], XX[i + NUMSITES][1]); */
		/* printf("C: %20.20g %20.20g\n", C[i][0], C[i][1]); */

		/* Now: Steiner point i has Steiner valence val[i];
		 * coords obey eqns XX[i+NUMSITES][] = sum(j)of B[i][j]*XX[nj][] + C[i][] */
		if (val[i] <= 1) { leafQ[lqp] = i; lqp++; } /* puts leafs on leafQ */
	} /* Have set up equations - now to solve them. */
	/* Second: eliminate leaves */
	while(lqp > 1) {
		lqp--; i = leafQ[lqp]; val[i]--; i2 = i + NUMSITES;
		/* now to eliminate leaf i */
		eqnstack[eqp] = i; eqp++; /* push i onto stack */
		for (j = 0; j < 3; j++) if (B[i][j] != 0.0) break; /* neighbor is #j */
		q0 = B[i][j];
		j = adj[i][j] - NUMSITES; /* neighbor is j */
		val[j]--; if (val[j] == 1) { leafQ[lqp] = j; lqp++; } /* new leaf? */
		for (m = 0; m < 3; m++) if (adj[j][m] == i2) break;
		q1 = B[j][m]; B[j][m] = 0.0;
		t = 1.0 - q1*q0; t = 1.0 / t;
		for (m = 0; m < 3; m++) B[j][m] *= t;
		for (m = 0; m < DIMENSION; m++) { C[j][m] += q1*C[i][m]; C[j][m] *= t; }
	}
	/* Third: Solve 1-vertex tree! */
	i = leafQ[0]; i2 = i + NUMSITES;
	for (m = 0; m < DIMENSION; m++) XX[i2][m] = C[i][m];

	/* printf("SP(1): %20.20g %20.20g\n", XX[i + NUMSITES][0], XX[i + NUMSITES][1]); */

	/* Fourth: backsolve */
	while (eqp > 0) {
		eqp--; i = eqnstack[eqp]; i2 = i + NUMSITES;
		for (j = 0; j < 3; j++) if (B[i][j] != 0.0) break; /* neighbor is #j */
		q0 = B[i][j];
		j = adj[i][j]; /* neighbor is j */
		for (m = 0; m < DIMENSION; m++) XX[i2][m] = C[i][m] + q0*XX[j][m];
	}

	/* printf("SP(2): %20.20g %20.20g\n", XX[i + NUMSITES][0], XX[i + NUMSITES][1]); */

	return;
}

REAL error()
{ /* Returns the error figure of tree T with Steiner coords in XX[][].
   * Assumes edge lengths have been pre-stored in array EL[][]. */
	register int i, m, i2, n0, n1, n2;  int k1;  REAL r, s, t, efig, d01, d12, d02;
	k1 = N - 2; efig = 0.0;
	for (i = 1; i <= k1; i++) {
		i2 = i + NUMSITES;
		n0 = adj[i][0]; n1 = adj[i][1]; n2 = adj[i][2];

		d12 = d01 = d02 = 0.0;
		for (m = 0; m < DIMENSION; m++) {
			t = XX[i2][m];
			r = XX[n0][m] - t; s = XX[n1][m] - t; t = XX[n2][m] - t;
			d12 += s*t; d01 += r*s; d02 += r*t;
		}
		/* only angles < 120 cause error */
		t = d12 + d12 + EL[i][1]*EL[i][2]; if (t > 0.0) efig += t;
		t = d01 + d01 + EL[i][0]*EL[i][1]; if (t > 0.0) efig += t;
		t = d02 + d02 + EL[i][0]*EL[i][2]; if (t > 0.0) efig += t;
	}
	efig = SQROOT(efig); return (efig);
}

void output_tree()
{
	int i, j;
	printf("topology-describing vector:");
	for (i = 1; i <= NUMSITES-3; i++) printf(" %d", BESTVEC[i]);
	printf("\nsteiner point coords\n");
	for (i = NUMSITES+1; i <= 2*NUMSITES-2; i++) {
		for (j = 0; j < DIMENSION; j++) printf(" %20.20g", XX[i][j]);
		printf("\n");
	}
	printf("edges\n");
	for (i = 1; i <= 2*NUMSITES-3; i++) printf("%d-%d;", edge[i][0], edge[i][1]);
	printf("\n");
	fflush(stdout);
}

#ifdef XWINDOWS
void display_tree()
{
	int i;
	if (DIMENSION != 2) return;
	W.clear();
	for (i = 1; i <= NUMSITES; i++) {
		point p( XX[i][0], XX[i][1] );
		W.draw_filled_node(p);
	}
	for (i = 1; i <= 2*NUMSITES-3; i++) 
		W.draw_edge(point(XX[edge[i][0]][0], XX[edge[i][0]][1]),
				point(XX[edge[i][1]][0], XX[edge[i][1]][1]));
	W.read_mouse();
}
#endif

void output_tree_to_maple()
{
	int i;
	if (DIMENSION != 3) return;
	printf("PLOT3D(\n");
	for (i = 1; i <= NUMSITES; i++)
		printf("POLYGONS([[%g, %g, %g]], STYLE(POINT), SYMBOL(CIRCLE)),\n",
				XX[i][0], XX[i][1], XX[i][2]);
	for (i = 1; i <= 2*NUMSITES-3; i++) 
		printf("POLYGONS([[%g, %g, %g], [%g, %g, %g]]),\n",
				XX[edge[i][0]][0], XX[edge[i][0]][1], XX[edge[i][0]][2],
				XX[edge[i][1]][0], XX[edge[i][1]][1], XX[edge[i][1]][2]);
	printf("AXESSTYLE(FRAME));\n");
}


int main(int argc, char *argv[])
{ /* Inputs NUMSITES, DIMENSION, sites; outputs succesive best Steiner
   * trees found. Best tree's topology-vector is stored in BESTVEC. */

	time_t start_t = clock();
	int i, j, x, k, m, nc, ct;  REAL q, r; int A[NMAX];
	INITRAND();

	/* (1) Input */
	scanf("%d", &NUMSITES);
	if (NUMSITES < 3 || NUMSITES > NMAX) {
		fprintf(stderr, "NUMSITES=%d out of range\n", NUMSITES); exit(0); 
	}
	printf("NUMSITES = %d\n", NUMSITES);

	scanf("%d", &DIMENSION);
	if (DIMENSION < 2 || DIMENSION > MAXDIM) {
		fprintf(stderr, "DIMENSION=%d out of range\n", DIMENSION); exit(0);
	}
	printf("DIMENSION = %d\n", DIMENSION);

	for (i = 1; i <= NUMSITES; i++) for (j = 0; j < DIMENSION; j++) scanf("%lf", &(XX[i][j]));
	SCALE = 0.0;
	for (i = 1; i <= NUMSITES; i++) {
		for (j = 0; j < DIMENSION; j++) {
			q = XX[i][j] - XX[1][j]; if (q < 0.0) q = -q; if (q > SCALE) SCALE = q;
			printf(" %g", XX[i][j]); 
		}
		printf("\n");
	}
	printf("SCALE = %g\n", SCALE);
	fflush(stdout);

#ifdef XWINDOWS
	W.display();
	W.init( 0.0, 1.0, 0.0 );
	W.set_node_width(1);
#endif

	if (NUMSITES == 3) { /* Deal with special case of 3 sites */
		buildtree(0, A);
		q = length(); r = error();
		do { 
			/* printf("%20.20g %20.20g\n", q, r); */
			optimize(0.0001*r/NUMSITES); 
			q = length(); r = error();
		} while ( r > q *0.0001);
		printf("\nnew record length %20.20g\n", q);
		output_tree();
		printf("done\n"); fflush(stdout); 
		
		time_t t = clock() - start_t;
		printf ("Took %d clocks = %f seconds.\n",t,((double)t)/CLOCKS_PER_SEC);
		exit(0);
	}

	/* (2) Preprocessing and initialization */
	/* Optionally, sort sites in some nice order here. */
	/* STUB = any upper bound ont the length of the SMT. */
	STUB = HUGE; k = 1; m = 0; ct = 0; /*  ct counts backtrack iters. Unused at present */
	double stopSecs = 12.0*60.0*60.0;

	DoForever{ /* 3: candidate leaf generation and backtracking */
		nc = 0; ct++;
		if (ct % 10000 == 0) { 
			printf("still running (%ld)!\n",ct); fflush(stdout); 
			time_t t = clock() - start_t;
			if( ((double)t)/CLOCKS_PER_SEC > stopSecs){
				printf ("Stopped prematurely after %d clocks = %f seconds.\n",t,((double)t)/CLOCKS_PER_SEC);
				exit(0);
			}
		}
		for ( x = 2*k + 1; x > 0 && k <= NUMSITES-3; x--) {
			A[k] = x;

			/* for (i = 1; i <= k; i++) printf(" %d", A[i]); printf("\n"); */

			buildtree(k, A); /* Build the tree represented by the topol. vector A[1..k] */
ITER: /* .. and optimize it until either obviously bad or small error figure happens */
			q = length();
			r = error();

			/* if ((A[1] == 3) && (A[2] == 5) && (A[3] == 4) && (A[4] == 8) && (k >= NUMSITES-3))
			   { printf("Still optimizing... (q = %20.20g   r = %20.20g   LB = %20.20g)\n", q, r, q - r); display_tree(); } */

			if (r > 0.005*q) { optimize(0.0001*r/NUMSITES); goto ITER; }
			if ( q - r < STUB) { /* moved behind previous line!!!! */
				if (k >= NUMSITES-3) {
					do { optimize(0.0001*r/NUMSITES); q = length(); r = error(); 
						/* printf("%20.20g %20.20g\n", q, r); */
					} while (r > q*0.0001);

					/* if (q < 1.7) { printf("Length = %20.20g\n", q); display_tree(); } */

					if (q < STUB) {
						printf("\nnew record length %20.20g\n", q);
						for (i = 1; i <= k; i++) BESTVEC[i] = A[i];
						/* if (q < STUB*0.99999) output_tree(); */
						output_tree();
						output_tree_to_maple();
#ifdef XWINDOWS
						display_tree(); 
#endif
						STUB = q;
					}
				}
				else {
					i = nc; nc++;
					while (i > 0 && LEN[i] < q) {
						STACK[m+i+1] = STACK[m+i]; LEN[i+1] = LEN[i]; i--;
					}
					i = i + 1; STACK[m+i] = x; LEN[i] = q;
				}
			} 
		}
		m = m + nc;

		/* printf("k = %d  nc = %d  m = %d\n", k, nc, m); */

		while (nc <= 0) {
			k--;
			if (k <= 0) { printf("done\n"); fflush(stdout); 
				
				time_t t = clock() - start_t;
				printf ("Took %d clocks = %f seconds.\n",t,((double)t)/CLOCKS_PER_SEC);
				exit(0); }
			nc = STACK[m]; m--;
		}

		A[k] = STACK[m]; STACK[m] = nc - 1;
		if (k < NUMSITES-3) k++; else m--;
	}

	return 0;

} /* End of Steiner tree program. */







