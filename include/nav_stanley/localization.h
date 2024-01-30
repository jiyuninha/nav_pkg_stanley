#pragma once
#include <master.h>
#include <fstream>
#include <sstream>

class Local
{
private:
    struct Waypoint {
        double x;
        double y;
        double index;
    };

    double x = 0;
    double y = 0;
    double z = 0;

    vector<vector<double>> global_map;

public:
    void readCSV();
    void findClosestWaypoint(Scout*);
    vector<vector<double>> getGP();
    vector <vector <double>> run();
};