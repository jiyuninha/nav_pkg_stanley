#include <localization.h>

void Local::readCSV() {
    vector<Waypoint> waypoints;
    const string filename = "../../../src/scout_nav/nav_stanley/map/global_path.csv";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open CSV file." << endl;
    }

    string line;
    int i = 0;
    while (getline(file, line)) {
        istringstream iss(line);
        Waypoint waypoint;
        char delimiter = ',';
        if (iss >> waypoint.x >> delimiter >> waypoint.y >> delimiter >> waypoint.index ) {
            waypoints.push_back(waypoint);
            this->global_map.push_back(std::vector<double>(3));
            this->global_map[i][0] = double(waypoint.x);
            this->global_map[i][1] = double(waypoint.y);
            this->global_map[i][2] = double(waypoint.index);
        }
        else {
            cerr << "Error: Invalid line in CSV file: " << line << endl;
        }
        i++;
    }
    file.close();
}

void Local::findClosestWaypoint(Scout* scout) {
    int closestIndex = 0;
    double minDistance = -1.0;
    int save_index = 0;
    for (int i = 0; i < this->global_map.size(); i++) {
        double distance = sqrt(pow(scout->getPosition_x() - this->global_map[i][0], 2) + pow(scout->getPosition_y() - this->global_map[i][1], 2));
        if ((minDistance > distance || minDistance == -1.0) && save_index <= i) {
            minDistance = distance;
            closestIndex = i;
            save_index = i;
        }
    }
    scout->setPosition_idx(closestIndex);
}
vector<vector<double>> Local::getGP(){
    return this->global_map;
}

vector<vector<double>> Local::run(){
    this->readCSV();
    return this->getGP();
}
