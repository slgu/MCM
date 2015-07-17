#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;
const int MAXN = 10;
double mindata = -1e9;
double maxdata = 1e9;
int n;
double checkmax(double a[], int n, int & index){
	double maxn = a[0];
	index = 0;
	for(int i = 1; i < n; ++i){
		if(fabs(maxn) < fabs(a[i])){
			maxn = a[i];
			index = i;
		}
	}
	return maxn;
}
void mul(double x[], double xx[], double  a[][MAXN], int n){
	for(int i = 0; i < n; ++i){
		xx[i] = 0;
		for(int j = 0; j < n; ++j){
			xx[i] += a[i][j] * x[j];
		}
	}
}
double itr_cal_vect(double x[], int n, double  a[][MAXN]){		
	int maxstep = 200;
    double maxn;
    double err = 1e-7;
    for(int i = 0; i < n; ++i)
        x[i] = 1;
    double xx[MAXN];
    int index = 0;
    while(maxstep--){
        mul(x, xx, a, n);
        double ret = xx[index] / x[index];
        maxn = checkmax(xx, n, index);
        for(int i = 0; i < n; ++i)
            xx[i] /= maxn;
        bool flag = true;
        for(int i = 0; i < n; ++i){
            if(fabs(x[i] - xx[i]) > err){
                flag = false;
                break;
            }
        }
        for(int i = 0; i < n; ++i)
            x[i] = xx[i];
        if(flag){
        	return ret;
        }
    }
    return -1;
}
double a[MAXN][MAXN];
double mina[MAXN][MAXN];
double maxa[MAXN][MAXN];

double mina_consist[MAXN][MAXN];
double maxa_consist[MAXN][MAXN];
double final_mina[MAXN][MAXN];
double final_maxa[MAXN][MAXN];
//考虑上三角 i <= j
bool cml(double a, double b){
	return a > b;
}
void pre(){
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j){
			maxa[i][j] = maxdata;
			mina[i][j] = 0;
		}
}
double ri[] = {
	0,  0,0,0.58,0.9,1.12,1.24,1.32,1.41,1.45,1.49,1.51
};
void print(double a[][MAXN], int n){
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			printf("%.3f ", a[i][j]);
		}
		puts("");
	}
}
int main(){
	scanf("%d", &n);
	pre();
	for(int i = 0; i < n; ++i)
		for(int j = 0; j < n; ++j)
			scanf("%lf", &a[i][j]);
	double x[MAXN];
	double ret_val = itr_cal_vect(x, n, a);
	printf("egivalue:%.3f\n", ret_val);
	for(int i = 0; i < n; ++i){
		printf("%.3f ", x[i]);
	}
	puts("");
	for(int i = 0; i < n; ++i)
		for(int j = i + 1; j < n; ++j){
			if(j != i + 1){
				mina[i][j] = a[i][j] * max(pow(x[i + 1] / x[i], n), pow(x[j] / x[j - 1], n));
				if(i != 0 && j != n - 1)
					maxa[i][j] = a[i][j] * min(pow(x[j] / x[j + 1], n),pow(x[i - 1] / x[i], n));
				if(i == 0 && j != n - 1)
					maxa[i][j] = a[i][j] * (pow(x[j] / x[j + 1], n));
				if(i != 0 && j == n - 1)
					maxa[i][j] = a[i][j] * (pow(x[i - 1] / x[i], n));
			}
			else{
				mina[i][j] = a[i][j] * pow(x[i + 1] / x[i], 1.0 * n / 2);
				if(i != 0 && j != n - 1)
					maxa[i][j] = a[i][j] * min(pow(x[j] / x[j + 1], n),pow(x[i - 1] / x[i], n));
				if(i == 0 && j != n - 1)
					maxa[i][j] = a[i][j] * (pow(x[j] / x[j + 1], n));
				if(i != 0 && j == n - 1)
					maxa[i][j] = a[i][j] * (pow(x[i - 1] / x[i], n));
			}
		}
	for(int i = 0; i < n; ++i)
		for(int j = i + 1; j < n; ++j){
			double save = a[i][j];
			while(true){
				a[i][j] += 0.01;
				a[j][i] = 1 / a[i][j];
				ret_val = itr_cal_vect(x, n, a);
				double ci = (ret_val - n) / (n - 1);
				if(ci / ri[n] > 0.1)
					break;
			}
			maxa_consist[i][j] = a[i][j] - 0.01;
			a[i][j] = save;
			a[j][i] = 1 / a[i][j];
			while(true){
				a[i][j] -= 0.01;
				a[j][i] = 1/ a[i][j];
				if(a[i][j] < 0)break;
				ret_val = itr_cal_vect(x, n, a);
				double ci = (ret_val - n) / (n - 1);
				if(i == 2 && j == 3){
					printf("egivalue:%.3f\n", ret_val);
					for(int v1 = 0; v1 < n; ++ v1){
						for(int v2 =0; v2 < n; ++ v2)
							printf("%.2f ", a[v1][v2]);
						puts("");
					}
				}
				if(ci / ri[n] > 0.1)
					break;
			}
			mina_consist[i][j] = a[i][j] + 0.01;
			a[i][j] = save;
			a[j][i] = 1 / a[i][j];
		}
	for(int i = 0; i < n; ++i)
		for(int j = i + 1; j < n; ++j){
			final_mina[i][j] = max(mina[i][j], mina_consist[i][j]);
			final_maxa[i][j] = min(maxa[i][j], maxa_consist[i][j]);
		}
			puts("\n\n\n");
	
	print(mina, n);
	puts("--------");
	print(maxa, n);

	puts("\n\n\n");
	print(mina_consist, n);
	puts("------------");
	print(maxa_consist, n);
		puts("\n\n\n");
	print(final_mina, n);
	puts("------------");
	print(final_maxa, n);
	
	return 0;
}






