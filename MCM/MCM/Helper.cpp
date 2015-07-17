//
//  help.cpp
//  MCM
//
//  Created by slgu on 14-2-9.
//  Copyright (c) 2014年 gsl. All rights reserved.
//
#include <cstdio>
#include <cmath>
#include <cstring>
int  cal_glory(int regular_champ, int tourment_champ, int enter_ncaa, int ncaa_ff, int ncaa_champ){
    return ncaa_champ * 20 + ncaa_ff * 10 + 5 * enter_ncaa + 10 * tourment_champ + 8 * regular_champ;
}
double checkmax(double a[], int n, int & index){
    index = 0;
    double maxn = a[0];
    for(int i  = 1; i < n; ++i){
        if(a[i] > maxn)
            maxn = a[i], index = i;
    }
    return maxn;
}
void mul(const double a[][4], double x[4], double xx[4]){
    for(int i = 0; i < 4; ++i)
        xx[i] = 0;
    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            xx[i] += x[j] * a[i][j];
}
void itr_cal_vector(const double  a[][4], double  x[]){
    int maxstep = 200;
    double maxn;
    double err = 1e-7;
    for(int i = 0; i < 4; ++i)
        x[i] = 1;
    double xx[4];
    while(maxstep--){
        mul(a, x, xx);
        int index;
        maxn = checkmax(xx, 4, index);
        for(int i = 0; i < 4; ++i)
            xx[i] /= maxn;
        bool flag = true;
        for(int i = 0; i < 4; ++i){
            if(fabs(x[i] - xx[i]) > err){
                flag = false;
                break;
            }
        }
        for(int i = 0; i < 4; ++i)
            x[i] = xx[i];
        if(flag)return;
    }
}

void print(double x[], int n){
    for(int i = 0; i < n; ++i)
        printf("%.2f ",x[i]);
    puts("");
}
void norm_vect(double x[], int n){
    double total = 0;
    for(int i = 0; i < n; ++i)
        total += x[i];
    for(int i = 0; i < n; ++i)
        x[i] /= total;
}