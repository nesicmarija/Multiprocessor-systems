#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>
#include <ctime>
using namespace std;

#define V 7
#define UNAVAILABLE 73

int travllingSalesmanProblem(int graph[][V], int start, vector<vector<int>>& optimalTours)
{
    vector<int> vertex;
    for (int i = 0; i < V; i++)
        if (i != start)
            vertex.push_back(i);

    int min_path = INT_MAX;
    do {
        int current_pathweight = 0;
        int k = start;
        bool invalidPath = false;
        for (int i = 0; i < vertex.size(); i++) {
            if (graph[k][vertex[i]] == UNAVAILABLE) {
                invalidPath = true;
                break;
            }
            current_pathweight += graph[k][vertex[i]];
            k = vertex[i];
        }
        if (invalidPath) {
            continue; // Skip unavailable paths
        }
        current_pathweight += graph[k][start];

        if (current_pathweight < min_path) {
            min_path = current_pathweight;
            optimalTours.clear();
            optimalTours.push_back({start});
            for (int i = 0; i < vertex.size(); i++) {
                optimalTours.back().push_back(vertex[i]);
            }
        } else if (current_pathweight == min_path) {
            optimalTours.push_back({start});
            for (int i = 0; i < vertex.size(); i++) {
                optimalTours.back().push_back(vertex[i]);
            }
        }

    } while (
        next_permutation(vertex.begin(), vertex.end()));

    return min_path;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }
    
    ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        cerr << "Unable to open file: " << argv[1] << endl;
        return 1;
    }

    int graph[V][V];
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            char value;
            inputFile >> value;
            graph[i][j] = (value == UNAVAILABLE) ? UNAVAILABLE : (value - '0');
        }
    }

    inputFile.close();

    clock_t start_time = clock();

    int start_city = 0;

    vector<vector<int>> optimalTours;
    int minCost = travllingSalesmanProblem(graph, start_city, optimalTours);

    clock_t end_time = clock();

    // Write to file
    ofstream outputFile("res.log.seq");

    if (outputFile.is_open()) {
        outputFile << "Price: " << minCost << endl;
        outputFile << "Paths: " << endl;

        for (const auto& tour : optimalTours) {
            for (int city : tour) {
                outputFile << city << " ";
            }
            outputFile << "\n";
        }

        outputFile << "Elapsed time: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << " seconds\n";

        outputFile.close();
        cout << "Optimal tours written to res.log" << endl;
    } else {
        cerr << "Unable to open file: res.log" << endl;
    }

    printf("Execution time: %f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    cout << "Minimum cost for TSP (Travelling Salesman Problem): " << minCost << endl;

    return 0;
}

