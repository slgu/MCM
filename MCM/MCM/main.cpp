//
//  main.cpp
//  MCM
//
//  Created by slgu on 14-2-8.
//  Copyright (c) 2014年 gsl. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include "DataStruct.h"
#include "Data.cpp"
#include <set>
#include <map>
using namespace std;
map <string, Coach> coach_data;
map <string, Team> team_data;
vector < pair <string, double> > rank_year[2020];// win percent for team per year
map < pair <string, int>, pair <string, double> > search_ply_rating;//get rating and of team
//Season	School 	Conf	G	W	L	W-L%	SRS	SOS	AP Pre	AP High	AP Final	Notes
//extern data
//-----------begin
extern const string teamrecordtxt;
extern const string playerrecordtxt;
extern const string gloryrecordtxt;
extern int  cal_glory(int regular_champ, int tourment_champ, int enter_ncaa, int ncaa_ff, int ncaa_champ);
extern void print(double x[], int n);
extern double win_ci;
extern double psy_ci;
extern double team_change_ci;
extern double player_change_ci;
extern string standard_first_10[10];
//-------------end
void norm_vect(double x[], int n);
vector <Analyze_coach> analizer;
void readcompetitiondata(){
    ifstream in(teamrecordtxt);
    TeamData tmp_record;
    string coachname;
    string year;
    string note;
    while(in >> tmp_record.coachname){
        coachname = tmp_record.coachname;
        in >> year;
        tmp_record.year = stoi(year);
        in >> tmp_record.teamname;
        in >> year;
        in >> tmp_record.total;
        in >> tmp_record.win;
        in >> tmp_record.loss;
        in >> year;
        for(int i = 0; i < 5; ++i)
            in >> year;
        in >> note;
        if(tmp_record.total == 0)
            continue;
        tmp_record.win_percent = tmp_record.win * 1.0 / tmp_record.total;
        if(coach_data.find(coachname) ==  coach_data.end()){
            coach_data[coachname] = Coach(coachname);
        }
        coach_data[coachname].push_back(tmp_record);
        if(team_data.find(tmp_record.teamname) == team_data.end())
            team_data[tmp_record.teamname] = Team(tmp_record.teamname);
        team_data[tmp_record.teamname].gamedata.push_back(tmp_record);
        rank_year[tmp_record.year].push_back(make_pair(tmp_record.teamname, tmp_record.win_percent));
    }
    in.close();
}
void readplayerdata(){
    ifstream in(playerrecordtxt);
    Player_record plyrecord;
    string year;
    vector <Player_record> vt;
    double minn = 10000;
    while(in >> plyrecord.playername){
        in >> year;
        plyrecord.year = stoi(year);
        in >> plyrecord.teamname;
        in >> plyrecord.rating;
        vt.push_back(plyrecord);
        minn = min(minn, plyrecord.rating);
    }
    in.close();
    for(int i = 0; i < vt.size(); ++i){
        vt[i].rating -= minn;
        vt[i].rating += 0.1;
    }
    for(Player_record plyrecord :vt){
        if(team_data.find(plyrecord.teamname) != team_data.end()){
            team_data[plyrecord.teamname].memberdata[plyrecord.year].push_back(plyrecord.playername);
            search_ply_rating[make_pair(plyrecord.playername, plyrecord.year)] = make_pair(plyrecord.teamname,plyrecord.rating);
        }
    }
}
void readglorydata(){
    ifstream in(gloryrecordtxt);
    string name;
    int rc, tc, enter_nc, nc_ff, nc_champ;
    while(in >> name){
        if(name[0] >= 'A' && name[0] <= 'Z'){
            name[0] = name[0] + 'a' - 'A';
        }
        int index = name.find("-");
        if(index != -1){
            if(name[index + 1] >= 'A' && name[index + 1] <= 'Z'){
                name[index + 1] = name[index + 1] + 'a' - 'A';
            }
        }
        in >> rc >> tc >> enter_nc >> nc_ff >> nc_champ;
        if(coach_data.find(name) != coach_data.end()){
            coach_data[name].glory = cal_glory(rc, tc, enter_nc, nc_ff, nc_champ);
        }
    }
    in.close();
}
bool cmp(pair <string, double> a, pair <string, double> b){
    if(a.second == b.second)
        return a.first < b.first;
    else
        return a.second < b.second;
}
inline void sortrank_percent(){
    for(int i = 1900; i < 2015; ++i)
        sort(rank_year[i].begin(), rank_year[i].end(), cmp);
}
inline double getrank_percent(int year, string teamname){
    int st = 0;
    for(auto i = rank_year[year].begin(); i != rank_year[year].end(); ++i){
        if(i -> first == teamname){
            int x = rank_year[year].size();
            return 1.0 * (st + 1) / x;
        }
        ++st;
    }
    return -1;
}
inline double getwin_percent(int year, string teamname){
    for(auto i = rank_year[year].begin(); i != rank_year[year].end(); ++i){
        if(i -> first == teamname)
            return i -> second;
    }
    return -1;
}
inline double get_player_eff(string player, int year){
    if(search_ply_rating.find(make_pair(player, year)) != search_ply_rating.end()){
        return search_ply_rating[make_pair(player, year)].second;
    }
    return -1;
}
inline string get_player_team(string player, int year){
    if(search_ply_rating.find(make_pair(player, year)) != search_ply_rating.end()){
        return search_ply_rating[make_pair(player, year)].first;
    }
    return "fuck";
}
void get_analyze_data(){
    sortrank_percent();
    for(auto i = coach_data.begin(); i != coach_data.end(); ++i){
        Analyze_coach tmp;
        tmp.setcoach(i -> second);
        for(auto i = tmp.coach.gamedata.begin(); i != tmp.coach.gamedata.end(); ++i){
            tmp.win_rank_percent_catalog.push_back(Win_Rank_Cat(i -> year, i -> win_percent, getrank_percent(i -> year,
                                                                                                             i -> teamname)));
            tmp.ex_win_rank_percent_catalog.push_back(Win_Rank_Cat(i -> year - 1, getwin_percent(i -> year - 1, i -> teamname)
                                                                   , getrank_percent(i -> year - 1, i -> teamname)));
        }
        analizer.push_back(tmp);
    }
}
void pre(){
    norm_vect(psy_cho_vect, 4);
    norm_vect(team_change_vect, 5);
    norm_vect(player_change_vect, 4);
    norm_vect(win_vect, 3);
    norm_vect(total_vect, 4);
}
void cal_value(){
    for(int i = 0; i < analizer.size(); ++i){
        analizer[i].norm();
        //printf("%.2f %.2f\n", analizer[i].first_team_change_cat, analizer[i].continue_team_change_cat);
    }
    for(int i = 0; i < analizer.size(); ++i){
       // per coach
        double continue_player_value = 0;
        double  first_player_value = 0;
        int continue_player_num = 0;
        int first_player_num = 0;
        for(int j = 0; j < analizer[i].coach.gamedata.size(); ++j){
            string teamname = analizer[i].coach.gamedata[j].teamname;
            int year = analizer[i].coach.gamedata[j].year;
            Team & team = team_data[teamname];
            for(int k = 0; k < team.memberdata[year].size(); ++k){
                string membername = team.memberdata[year][k];
                int last = year;
                while(last--){
                    if(get_player_team(membername, last) != teamname)
                        break;
                }
                int continue_num = year - last;
                if(get_player_team(membername, year - 1) == "fuck")
                    continue;
                double p = get_player_eff(membername, year - 1);
                double change = get_player_eff(membername, year) - get_player_eff(membername, year - 1);
                if(continue_num == 1){
                    ++first_player_num;
                    if(change < 0)
                        first_player_value += -(10 + 1.0 / sqrt(1.0 + j)) * (change / p) * (change / p);
                    else
                        first_player_value += (10 + 1.0 / sqrt(1.0 + j)) * (change * p) * (change * p);
                }
                else{
                    ++continue_player_num;
                    if(change < 0)
                        continue_player_value += -(10 + 1.0 / sqrt(1.0 + j)) * (change / p) * (change / p);
                    else
                        continue_player_value += (10 + 1.0 / sqrt(1.0 + j)) * (change * p) * (change * p);
                    
                }
            }
        }
        if(continue_player_num == 0)
            analizer[i].continue_player_change_cat = mindata;
        else
            analizer[i].continue_player_change_cat =  continue_player_value / continue_player_num;
        if(first_player_num == 0)
            analizer[i].first_player_change_cat = mindata;
        else
            analizer[i].first_player_change_cat = first_player_value / first_player_num;
    }
    
}
void norm(){
    // analyzer_coach's analyzing data: double win_cat, team_change_cat, player_change_cat, glory;
    double min_win_cat = maxdata, max_win_cat = mindata;
    double min_rank_cat = maxdata, max_rank_cat = mindata;
    double min_continue_team_change_cat = maxdata, max_continue_team_change_cat = mindata;
    double min_first_team_change_cat = maxdata, max_first_team_change_cat = mindata;
    double min_continue_player_change_cat = maxdata, max_continue_player_change_cat = mindata;
    double min_first_player_change_cat = maxdata, max_first_player_change_cat = mindata;
    double min_glory_cat = maxdata, max_glory_cat = mindata;
    double min_year_cat = maxdata, max_year_cat = mindata;
    for(int i = 0; i < analizer.size(); ++i){
        min_win_cat = min(min_win_cat, analizer[i].win_percent_cat);
        max_win_cat = max(max_win_cat, analizer[i].win_percent_cat);
        
        min_rank_cat = min(min_rank_cat, analizer[i].rank_percent_cat);
        max_rank_cat = max(max_rank_cat, analizer[i].rank_percent_cat);
        
        min_glory_cat = min(min_glory_cat, analizer[i].glory_cat);
        max_glory_cat = max(max_glory_cat, analizer[i].glory_cat);
        
        min_year_cat = min(min_year_cat, analizer[i].year_cat);
        max_year_cat = max(max_year_cat, analizer[i].year_cat);
        
        min_first_player_change_cat = min(min_first_player_change_cat, analizer[i].first_player_change_cat);
        max_first_player_change_cat = max(max_first_player_change_cat, analizer[i].first_player_change_cat);
        
        min_first_team_change_cat = min(min_first_team_change_cat, analizer[i].first_team_change_cat);
        max_first_team_change_cat = max(max_first_team_change_cat, analizer[i].first_team_change_cat);
        
        min_continue_player_change_cat = min(min_continue_player_change_cat, analizer[i].continue_player_change_cat);
        max_continue_player_change_cat = max(max_continue_player_change_cat, analizer[i].continue_player_change_cat);
        
        min_continue_team_change_cat = min(min_continue_team_change_cat, analizer[i].continue_team_change_cat);
        max_continue_team_change_cat = max(max_continue_team_change_cat, analizer[i].continue_team_change_cat);
    }
    for(int i = 0; i < analizer.size(); ++i){
        analizer[i].win_percent_cat = (analizer[i].win_percent_cat - min_win_cat) / (max_win_cat - min_win_cat);
        analizer[i].rank_percent_cat = (analizer[i].rank_percent_cat - min_rank_cat) / (max_rank_cat - min_rank_cat);
        analizer[i].glory_cat = (analizer[i].glory_cat - min_glory_cat) / (max_glory_cat - min_glory_cat);
        analizer[i].year_cat = (analizer[i].year_cat - min_year_cat) / (max_year_cat - min_year_cat);
        analizer[i].first_team_change_cat = (analizer[i].first_team_change_cat - min_first_team_change_cat) / (max_first_team_change_cat - min_first_team_change_cat);
        analizer[i].first_player_change_cat = (analizer[i].first_player_change_cat - min_first_player_change_cat) /
        (max_first_player_change_cat - min_first_player_change_cat);
        analizer[i].continue_team_change_cat = (analizer[i].continue_team_change_cat - min_continue_team_change_cat)/
        (max_continue_team_change_cat - min_continue_team_change_cat);
        analizer[i].continue_player_change_cat = (analizer[i].continue_player_change_cat  - min_continue_player_change_cat)/
        (max_continue_player_change_cat - min_continue_player_change_cat);
    }
}
void step_analyze_alogorithm(){
    
    //1:win_percent 2.rank_percent 3:year 4:glory
    //5:first_team_change 6.first_player_change 7.continue_team_change 8.continue_player_change

    //psy_cho_pass: 1 2 3 4
    //zhanji :1 2 4
    //improve player:  3 4 6 8
    //improve team: 1 2 4 5 7
    norm_vect(total_vect, 4);
    double weight_win_cat = 0;
    double weight_rank_cat = 0;
    double weight_year_cat = 0;
    double weight_glory_cat = 0;
    
    double weight_first_player_cat = 0;
    double weight_first_team_cat = 0;
    double weight_continue_player_cat = 0;
    double weight_continue_team_cat = 0;
    double * a[] = {0, &weight_win_cat, &weight_rank_cat, &weight_year_cat, &weight_glory_cat,
        &weight_first_team_cat, &weight_first_player_cat, &weight_continue_team_cat, &weight_continue_player_cat};
    int psy_cho[] = {1, 2, 3, 4};
    int zhanji[] = {1, 2, 4};
    int im_player[] = {3, 4, 6, 8};
    int im_team[] = {1, 2, 4, 5, 7};
    
    for(int i = 0; i < 4; ++i){
        *a[psy_cho[i]] += total_vect[0] * psy_cho_vect[i];
    }
    for(int i = 0; i < 4; ++i){
        *a[im_player[i]] += total_vect[1] * player_change_vect[i];
    }
    for(int i = 0; i < 3; ++i){
        *a[zhanji[i]] += total_vect[2] * win_vect[i];
    }
    for(int i = 0; i < 5; ++i){
        *a[im_team[i]] += total_vect[3] * team_change_vect[i];
    }
    /*
    cout << "win" << weight_win_cat << endl;
    cout << "rank" << weight_rank_cat << endl;
    cout << "year" << weight_year_cat << endl;
    cout << "f pl" << weight_first_player_cat << endl;
    cout << "c pl" << weight_continue_player_cat << endl;
     */
    cout << "win_cat" << weight_win_cat << endl;
    cout << "rank_cat" << weight_rank_cat << endl;
    cout << "year_cat" << weight_year_cat << endl;
    cout << "glory_cat" << weight_glory_cat << endl;
    cout << "first_pl" << weight_first_player_cat << endl;
    cout << "continue_pl" << weight_continue_player_cat << endl;
    cout << "first_team" << weight_first_team_cat << endl;
    cout << "continue_team" << weight_continue_team_cat << endl;
    
    for(int i = 0; i < analizer.size(); ++i){
        analizer[i].finalvalue =
        weight_win_cat * analizer[i].win_percent_cat +
        weight_rank_cat * analizer[i].rank_percent_cat +
        weight_year_cat * analizer[i].year_cat +
        weight_glory_cat * analizer[i].glory_cat +
        weight_first_player_cat * analizer[i].first_player_change_cat +
        weight_first_team_cat * analizer[i].first_team_change_cat +
        weight_continue_player_cat * analizer[i].continue_player_change_cat +
        weight_continue_team_cat * analizer[i].continue_team_change_cat;
        analizer[i].affect_ability = psy_cho_vect[0] * analizer[i].win_percent_cat +
            psy_cho_vect[1] * analizer[i].rank_percent_cat +
            psy_cho_vect[2] * analizer[i].year_cat +
            psy_cho_vect[3] * analizer[i].glory_cat;
        
        analizer[i].win_ability = win_vect[0] * analizer[i].win_percent_cat +
            win_vect[1] * analizer[i].rank_percent_cat +
            win_vect[2] * analizer[i].glory_cat;
        
        analizer[i].lead_player_ability = player_change_vect[0] * analizer[i].year_cat +
            player_change_vect[1] * analizer[i].glory_cat + player_change_vect[2] * analizer[i].first_player_change_cat +
        player_change_vect[3] * analizer[i].continue_player_change_cat;
        
        analizer[i].lead_team_ability = team_change_vect[0] * analizer[i].win_percent_cat +
        team_change_vect[1] * analizer[i].rank_percent_cat + team_change_vect[2] * analizer[i].glory_cat +
        team_change_vect[3] * analizer[i].first_team_change_cat + team_change_vect[4] * analizer[i].continue_team_change_cat;
        //    cout << analizer[i].finalvalue << endl;
    }
    sort(analizer.begin(), analizer.end());
}
void deal_nil_data(){
    int l = analizer.size();
    
    
    sort(analizer.begin(), analizer.end(), [](Analyze_coach x, Analyze_coach y) -> bool {
        return x.continue_player_change_cat < y.continue_player_change_cat;});
    int index = 0;
    int middle;
    while(analizer[index++].continue_player_change_cat == mindata);
    middle = index  + (l - index + 1) / 2;
    for(int i = 0; i < index; ++i)
        analizer[i].continue_player_change_cat = analizer[middle].continue_player_change_cat;
    
    
    sort(analizer.begin(), analizer.end(), [](Analyze_coach x, Analyze_coach y) -> bool{
        return x.first_player_change_cat < y.first_player_change_cat;});
    index = 0;
    while(analizer[index++].first_player_change_cat == mindata);
    middle = index  + (l - index + 1) / 2;
    for(int i = 0; i < index; ++i)
        analizer[i].first_player_change_cat = analizer[middle].first_player_change_cat;
    
    
    sort(analizer.begin(), analizer.end(),[](Analyze_coach x, Analyze_coach y) -> bool{
        return x.continue_team_change_cat < y.continue_team_change_cat;});
    index = 0;
    while(analizer[index++].continue_team_change_cat == mindata);
    middle = index  + (l - index + 1) / 2;
    for(int i = 0; i < index; ++i)
        analizer[i].continue_team_change_cat = analizer[middle].continue_team_change_cat;
    
    
    sort(analizer.begin(), analizer.end(),[](Analyze_coach x, Analyze_coach y) -> bool{
        return x.first_team_change_cat < y.first_team_change_cat;});
    index = 0;
    while(analizer[index++].first_team_change_cat == mindata);
    middle = index  + (l - index + 1) / 2;
    for(int i = 0; i < index; ++i)
        analizer[i].first_team_change_cat = analizer[middle].first_team_change_cat;
}
int cal_leastsquare(){
    int f = 0;
    int l = analizer.size();
    for(int i = 0; i < 10; ++i){
        if(i == 1 || i == 9 || i == 8)
            continue;
        int j = 0;
        while(j < l && analizer[j].coach.coachname != standard_first_10[i]) ++j;
        f += (j - i) * (j - i);
    }
    return f;
}
void pso_algorithm(){
    double c1 = 1, c2 = 1;
    double w = 0.8;
    const int max_point_num = 200;
    double random_point[max_point_num][4];
    double speed[max_point_num][4];
    memset(speed, 0, sizeof(speed));
    double global_best[4];
    double local_best[max_point_num][4];
    int local_best_value[max_point_num];
    int global_best_value;
    srand(time(NULL));
    //1,1,1,1
    {
        for(int i = 0; i < 4; ++i)
            random_point[0][i] = 0.25;
    }
    for(int i = 1; i < max_point_num; ++i){
        double a , b, c, d;
        a = fabs(rand());
        b = fabs(rand());
        c = fabs(rand());
        d = fabs(rand());
        double maxn = a + b + c + d;
        random_point[i][0] = a / maxn,
        random_point[i][1] = b / maxn,
        random_point[i][2] = c / maxn,
        random_point[i][3] = d / maxn;
        for(int j = 0; j < 4; ++j)
            local_best[i][j] = random_point[i][j];
    }
    //initial generation
    int f = -1;
    for(int i = 0; i < max_point_num; ++i){
        for(int j = 0; j < 4; ++j)
            total_vect[j] = random_point[i][j];
        step_analyze_alogorithm();
        int tmp_f = cal_leastsquare();
        local_best_value[i] = tmp_f;
        if(f > tmp_f || f == -1){
            f = tmp_f;
            for(int j = 0; j < 4; ++j)
                global_best[j] = random_point[i][j];
        }
    }
    global_best_value = f;
    cout << global_best_value << endl;
    //begin maxstep steps;
    int maxstep = 200;
    while(maxstep--){
        printf("%d\n", maxstep);
        cout << global_best_value << endl;
        for (int i = 0; i < 4; ++i)
            printf("%.2f ", global_best[i]);
        puts("");
        
        for(int i = 0; i < max_point_num; ++i){
            for(int j = 0; j < 4; ++j)
                speed[i][j] = w * speed[i][j] + c1 * (local_best[i][j] - random_point[i][j]) + c2 * (global_best[j] - random_point[i][j]);
            for(int j = 0; j < 4; ++j){
                random_point[i][j] += speed[i][j];
            }
            for(int j = 0; j < 4; ++j){
                total_vect[j] = random_point[i][j];
            }
            step_analyze_alogorithm();
            int f = cal_leastsquare();
            int neg_coe = 0;
            for(int j = 0; j < 4; ++j){
                if(random_point[i][j] < 0)
                    ++neg_coe;
            }
            f += neg_coe * 100000; //penalty
            if(f < global_best_value){
                global_best_value = f;
                for(int j = 0; j < 4; ++j)
                    global_best[j] = random_point[i][j];
            }
            if(f < local_best_value[i]){
                local_best_value[i] = f;
                for(int j = 0; j < 4; ++j)
                    local_best[i][j] = random_point[i][j];
            }
        }
    }
    
    for(int i = 0; i < 4; ++i)
        total_vect[i] = global_best[i];
    step_analyze_alogorithm();
    f = cal_leastsquare();
    printf("%d\n",f);
    norm_vect(global_best, 4);
    for(int i = 0; i < 4; i++)
        printf("%.3f ", global_best[i]);
    puts("");
    for(int i = 0; i < 9; ++i){
        cout << analizer[i].coach.coachname << " " << analizer[i].win_ability << " " << analizer[i].affect_ability << " " <<
        analizer[i].lead_team_ability << " " << analizer[i].lead_player_ability << endl;
    }
}
int main(int argc, const char * argv[])
{
    pre();
    readcompetitiondata();
    readglorydata();
    readplayerdata();
    get_analyze_data();
    cal_value();
    deal_nil_data();
    norm();
    //pso_algorithm();
    step_analyze_alogorithm();
    /*for(int i = 0; i < analizer.size(); ++i){
        cout << analizer[i].win_percent_cat << " " <<   analizer[i].rank_percent_cat << " " << analizer[i].glory_cat << " " <<
        analizer[i].year_cat << " " << analizer[i].first_team_change_cat << " " << analizer[i].first_player_change_cat << " " <<
        analizer[i].continue_team_change_cat << " " << analizer[i].continue_player_change_cat << endl;
    }*/
    /*
    cout << "win- ---------\n";
    sort(analizer.begin(), analizer.end(), [](Analyze_coach x, Analyze_coach y) -> bool {
        return x.win_percent_cat > y.win_percent_cat;});
    for(int i = 0; i < 10; ++i){
        if(i == 1)
            continue;
        for(int j = 0; j < analizer.size(); ++j)
            if(analizer[j].coach.coachname == standard_first_10[i]){
                printf("%d\n", j);
                continue;
            }
    }
    cout << "rank-----------\n";
    sort(analizer.begin(), analizer.end(), [](Analyze_coach x, Analyze_coach y) -> bool {
        return x.rank_percent_cat > y.rank_percent_cat;});
    for(int i = 0; i < 10; ++i){
        if(i == 1)
            continue;
        for(int j = 0; j < analizer.size(); ++j)
            if(analizer[j].coach.coachname == standard_first_10[i]){
                printf("%d\n", j);
                continue;
            }
    }
    cout << analizer.size() << endl;
    cout << "team change\n";
    sort(analizer.begin(), analizer.end(), [](Analyze_coach x, Analyze_coach y) -> bool {
        return x.continue_team_change_cat > y.continue_team_change_cat;});
    for(int i = 0; i < 10; ++i){
        if(i == 1)
            continue;
        for(int j = 0; j < analizer.size(); ++j)
            if(analizer[j].coach.coachname == standard_first_10[i]){
                printf("%d\n", j);
                continue;
            }
    }
    cout << "year \n";
    sort(analizer.begin(), analizer.end(), [](Analyze_coach x, Analyze_coach y) -> bool {
        return x.year_cat > y.year_cat;});
    for(int i = 0; i < 10; ++i){
        if(i == 1)
            continue;
        for(int j = 0; j < analizer.size(); ++j)
            if(analizer[j].coach.coachname == standard_first_10[i]){
                printf("%d\n", j);
                continue;
            }
    }
    cout << "glory \n";
    sort(analizer.begin(), analizer.end(), [](Analyze_coach x, Analyze_coach y) -> bool {
        return x.glory_cat > y.glory_cat;});
    for(int i = 0; i < 10; ++i){
        if(i == 1)
            continue;
        for(int j = 0; j < analizer.size(); ++j)
            if(analizer[j].coach.coachname == standard_first_10[i]){
                printf("%d\n", j);
                continue;
            }
    }
     */
   // for(int i = 0; i < analizer.size(); ++i)    cout << analizer[i].coach.coachname << " " << analizer[i].finalvalue << endl;
    
    /*
    for(auto i = team_data.begin(); i != team_data.end(); ++i){
        int total = 0;
        for(int j = 1900; j < 2020; j++)
            total += i -> second.memberdata[j].size();
        cout << i -> second.teamname << " " << total << endl;
    }*/
    return 0;
}


//f:26  0.44 0.15 0.41 0.01

/*
 0.405 0.165 0.426 0.004
 adolph-rupp 0.984921 0.92309 0.338499 0.171745
 mike-krzyzewski 0.973346 0.876432 0.370018 0.165726
 dean-smith 0.981913 0.844713 0.32115 0.155557
 john-wooden 0.93948 0.762479 0.374316 0.141009
 jim-calhoun 0.844095 0.779105 0.305736 0.158846
 jim-boeheim 0.916463 0.682744 0.332289 0.148347
 roy-williams 0.916181 0.635832 0.307467 0.151598
 bob-knight 0.832849 0.692551 0.339104 0.123688
 lute-olson 0.864464 0.644103 0.29368 0.114331
 */

