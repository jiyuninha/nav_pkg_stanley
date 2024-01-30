#include <control.h>

double Control::StanleyController(vector<vector <double>> gp, Scout* scout){
    double k = 0.6;
    int closestIndex = 0;
    double minDistance = -1.0;
    int save_index = 0;
    int next_index = 0;
    Position* target = new Position();
    for (int i = 0; i < gp.size(); i++) { // gp에서 가장 가까운 point 찾기
        double distance = sqrt(pow(scout->getPosition_x() - gp[i][0], 2) + pow(scout->getPosition_y() - gp[i][1], 2));
        if ((minDistance > distance || minDistance == -1.0) && save_index <= i) {
            minDistance = distance;
            closestIndex = i;
            save_index = i;
        }
    }
    target->x = gp[closestIndex][0];
    target->y = gp[closestIndex][1]; // target position
    cout << "scout" <<scout->getPosition_x() << ", " <<scout->getPosition_y() << endl;
    cout << "target" <<target->x << ", " <<target->y << endl;
    target->idx = closestIndex;
    next_index = closestIndex + 1; // 다음 
    if (next_index >= gp.size()) next_index = 0; // 
    double cte = calculate_cte(gp, target, scout);
    double next_angle = atan2(gp[next_index][1] - gp[closestIndex][1], gp[next_index][0] - gp[closestIndex][0])*(180.0/M_PI); 
    if(next_angle < 0)next_angle+=360;
    double heading_error = next_angle - scout->getHeading();
    double scoutVel = 0.8;
    double steering_angle = fmod(atan2(k * cte, scoutVel)*(180.0/M_PI),360.0) + heading_error; // deg
    if(steering_angle<0)steering_angle+=360;
    return ((steering_angle-scout->getHeading())*(M_PI/180.0))/0.02;
}

double Control::calculate_cte(vector<vector <double>> gp , Position* target, Scout* scout){
    double next_slope = (gp[target->idx + 1][1] - gp[target->idx][1]) / (gp[target->idx + 1][0] - gp[target->idx][0]); // target과 다음 target의 기울기
    double cur_slope = atan2(target->y - scout->getPosition_y(), target->x - scout->getPosition_x()); // 현재 스카우트와 target의 기울기
    double diff = fmod(next_slope - cur_slope,2*M_PI)*(M_PI/180.0);
    if (diff < 0) diff+=360;
    double distance = sqrt(pow(scout->getPosition_x() - gp[target->idx][0], 2) + pow(scout->getPosition_y() - gp[target->idx][1], 2)); // 스카우트와 target point까지의 거리 
    double cte = distance * sin(diff*(M_PI/180.0));
    return cte;
}

double Control::run(vector < vector <double> > gp, Scout* scout){
    return this->StanleyController(gp,scout);
    
}