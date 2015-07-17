//
//  SomeData.h
//  MCM
//
//  Created by slgu on 14-2-8.
//  Copyright (c) 2014年 gsl. All rights reserved.
//

#ifndef MCM_SomeData_h
#define MCM_SomeData_h
#include <string>
#include <vector>
#include <utility>
#include <cmath>
#define maxdata  1e9
#define mindata  -1e9
using namespace std;
struct Record;
struct TeamData{
    string coachname;
    string teamname;
    int total;
    double  win_percent;
    int win;
    int loss;
    int total_competition_team;
    int year;
};
class Team{
public:
    Team(){
        
    }
    Team(string _teamname){
        teamname = _teamname;
        gamedata.clear();
    }
    string teamname;
    vector <TeamData> gamedata;
    vector <string> memberdata[2020];

};
class Coach{
public:
    Coach(){
        glory = 0;
        coachname = "fuck";
    }
    Coach(string _coachname){
        coachname = _coachname;
        gamedata.clear();
        glory = 0;
    }
    void push_back(TeamData & teamdata){
        gamedata.push_back(teamdata);
    }
    void analyze(){
        
    }
    string coachname;
    vector <TeamData> gamedata;
    int glory;
};
struct Win_Rank_Cat{
    int year;
    double win_percent;
    double rank_percent;
    Win_Rank_Cat(){
        win_percent = rank_percent = -1;
    }
    Win_Rank_Cat(int _year, double _win_percent, double _rank_percent):year(_year), win_percent(_win_percent), rank_percent(_rank_percent){
    }
    friend bool operator < (Win_Rank_Cat a, Win_Rank_Cat b){
        return a.year < b.year;
    }
};
struct Player_record{
    string playername;
    string teamname;
    int year;
    double rating;
};
class Analyze_coach{
public:
    void setcoach(Coach _coach){
        coach = _coach;
    }
    vector <Win_Rank_Cat> win_rank_percent_catalog; //win_percent, rank_percent
    vector <Win_Rank_Cat> ex_win_rank_percent_catalog;
    Coach coach;
    double win_percent_cat, rank_percent_cat, year_cat,glory_cat;
    double first_team_change_cat, first_player_change_cat, continue_team_change_cat, continue_player_change_cat;
    double finalvalue;
    
    double win_ability;
    double affect_ability;
    double lead_team_ability;
    double lead_player_ability;
    
    void norm(){
        sort(win_rank_percent_catalog.begin(), win_rank_percent_catalog.end());
        sort(ex_win_rank_percent_catalog.begin(), ex_win_rank_percent_catalog.end());
        win_percent_cat = 0;
        rank_percent_cat = 0;
        continue_team_change_cat = 0;
        first_team_change_cat = 0;
        int first_team_num = 0;
        int continue_team_num = 0;
        int year = win_rank_percent_catalog.size();
        int st = 0;
        double continue_team_change = 0;
        double coe = 0;
        for(int i = 0; i < win_rank_percent_catalog.size(); ++i){
            if(ex_win_rank_percent_catalog[i].win_percent == -1 || ex_win_rank_percent_catalog[i].rank_percent == -1)
                continue;
            double change =
            change = (win_rank_percent_catalog[i].win_percent + win_rank_percent_catalog[i].rank_percent);
            change -= (ex_win_rank_percent_catalog[i].win_percent + ex_win_rank_percent_catalog[i].rank_percent);
            change /= 2;
            double p = (ex_win_rank_percent_catalog[i].win_percent + ex_win_rank_percent_catalog[i].rank_percent) / 2;
            if(i < 5) coe = 0.1;
            else
                if(i < 10) coe = 0.3;
            else
                if(i < 15) coe = 0.7;
            else
                coe = 1;
            if(i != 0 &&
               coach.gamedata[i].teamname == coach.gamedata[i - 1].teamname && coach.gamedata[i].year == coach.gamedata[i - 1].year + 1){
                ++st;
                ++continue_team_num;
                if(change  < 0){
                    continue_team_change_cat += 0.1 * (10 + 1.0 / sqrt(i + 1.0)) * change;
                }
                else
                    continue_team_change_cat  += (10 + 1.0 / sqrt(i + 1.0)) * change;
            }
            else{
                continue_team_change += st * st;
                st = 1;
                ++first_team_num;
                if(change  < 0){
                    first_team_change_cat += 0.1 * (10 + 1.0 / sqrt(i + 1.0)) * change;
                }
                else
                    first_team_change_cat += (10 + 1.0 / sqrt(i + 1.0)) * change;
            }
            
            if(i < 5)
                win_percent_cat += (10 + 1.0 / sqrt((i + 1.0))) * win_rank_percent_catalog[i].win_percent / 2;
            else
                if(i < 10)
                    win_percent_cat += (10 + 1.0 / sqrt((i + 1.0))) * win_rank_percent_catalog[i].win_percent * 2/ 3;
            else
                win_percent_cat += (10 + 1.0 / sqrt((i + 1.0))) * win_rank_percent_catalog[i].win_percent;
            if(i < 5)
                rank_percent_cat += (10 + 1.0 / sqrt((i + 1.0))) * win_rank_percent_catalog[i].rank_percent / 2;
            else
                if(i < 10)
                    rank_percent_cat += (10 + 1.0 / sqrt((i + 1.0))) * win_rank_percent_catalog[i].rank_percent * 2/ 3;
            else
                rank_percent_cat += (10 + 1.0 / sqrt((i + 1.0))) * win_rank_percent_catalog[i].rank_percent;
        }
        continue_team_change += st * st;
        if(first_team_num == 0){
            first_team_change_cat = mindata;
        }
        else
            first_team_change_cat /= first_team_num;
      //  continue_team_change_cat =  continue_team_change * continue_team_change * continue_team_change_cat;
        
        if(continue_team_num == 0)
            continue_team_change_cat = mindata;
        else
            continue_team_change_cat /= continue_team_num;
        
        win_percent_cat /= win_rank_percent_catalog.size();
        rank_percent_cat /= win_rank_percent_catalog.size();
        glory_cat = coach.glory;
        year_cat = year * sqrt(year);
    }
    friend bool operator < (Analyze_coach a, Analyze_coach b){
        return a.finalvalue > b.finalvalue;
    }
};
#endif
