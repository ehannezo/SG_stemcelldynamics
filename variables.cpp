// key biological parameters for the results
double divrate_homeo=0.2;       // division rate at homeostasis (from Fig. XX)
double divrate_P2=0.45;    // division rate from P2 to P7  (from Fig. XX)
double d_P=0.3;            // imbalance towards duplication during SG growth (fitted, see SI Note)
double r_P=0.5;             // here, no asymetric division (see SI Note)
double ratio_SB=2.25;       // ratio of basal to suprabasal cells in an SG (from Fig. XX)
double initial_p=12;        // number of precursors for a SG (from Fig. XX, see SI Note)

double initial_t=3.0;   // time of induction (taking into account one day of delay from clonal induction at P2)
double homeo_t=7.0;     // time of homeostasis (from Fig. XX)


// important parameters for simulation
double runs=10000; // number of clones simulated
double dt=0.2;    //time step
int T=400; // total time of simulation
double t;   // time



// variables usefull for storing info during simulation
const int N=20;
double divrate_cell;
int countp;
int countd;

int ch1;
int ch2;
int tot_cells;
int size_y;
int size_x;

double distrib[100][10000];
int basal_grid[N][N];
double mean_p[1000];
double mean_d[1000];
double pcount[1000];
int supra_grid[N][N];
double cumul[100][1000];


