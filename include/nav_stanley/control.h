#pragma once
#include <master.h>
#define _USE_MATH_DEFINES

class Control {
private:
    double lookahead = 0;
    double alpha = 0;
    int target_index = 0;
    double angle = 0;
    double targetHeading = 0;
    struct Position{
        double x;
        double y;
        int idx;
    };
public:
    double StanleyController(vector<vector <double>>, Scout*);
    double calculate_cte(vector<vector<double>> , Position* , Scout*);
    double run(vector < vector <double> >, Scout*);
};