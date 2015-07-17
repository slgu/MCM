//
//  ConstData.cpp
//  MCM
//
//  Created by slgu on 14-2-8.
//  Copyright (c) 2014年 gsl. All rights reserved.
//
//
//1:win_percent 2.rank_percent 3:year 4:glory
//5:first_team_change 6.first_player_change 7.continue_team_change 8.continue_player_change
const string teamrecordtxt = "/Users/gsl/Documents/MCM/team_record.txt";
const string playerrecordtxt = "/Users/gsl/Documents/MCM/player_record.txt";
const string gloryrecordtxt = "/Users/gsl/Documents/MCM/glory_record.txt";

double psy_cho_vect[4] = {0.0937, 0.1874, 0.5103, 0.8341};
double win_vect[3] = {0.4364, 0.8729, 0.2182};
double player_change_vect[4] = {0.0937, 0.1874, 0.5103, 0.8341};
double team_change_vect[5] = {0.0799, 0.1598, 0.2466, 0.5125, 0.8029};
double total_vect[4] = {0.405, 0.165, 0.426, 0.004 };

double win_ci = 0;
double psy_ci = 0.0069;
double team_change_ci = 0.0244;
double player_change_ci = 0.0069;


string standard_first_10[10] = {
    "john-wooden",
    "bobby-knight",
    "mike-krzyzewski",
    "adolph-rupp",
    "dean-smith",
    "jim-calhoun",
    "jim-boeheim",
    "lute-olson",
    "eddie-sutton",
    "jim-phelan"
};

//26 0.41 0.17 0.43 0.00