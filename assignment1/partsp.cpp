#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <climits>
#include <omp.h>
using namespace std;

#define V 7
#define UNAVAILABLE 73

// Declarations of functions
int travllingSalesmanProblem(int graph[][V], int start, vector<vector<int>>& optimalTours, int num_processors);
int factorial(int n);
vector<int> getPermutation(const vector<int>& elements, int index);

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <input_file> <num_processors>" << endl;
        return 1;
    }

    int num_processors = strtol(argv[2], NULL, 10);

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

    double start_time = omp_get_wtime();

    int start_city = 0;

    vector<vector<int>> optimalTours;
    int minCost = travllingSalesmanProblem(graph, start_city, optimalTours, num_processors);

    double end_time = omp_get_wtime();

    // Write to file
    ofstream outputFile("res.log.par");

    if (outputFile.is_open()) {
        outputFile << "Price: " << minCost << endl;
        outputFile << "Paths: " << endl;

        for (const auto& tour : optimalTours) {
            for (int city : tour) {
                outputFile << city << " ";
            }
            outputFile << "\n";
        }

        outputFile << "Elapsed time: " << (end_time - start_time) << " seconds\n";

        outputFile.close();
        cout << "Optimal tours written to res.log" << endl;
    } else {
        cerr << "Unable to open file: res.log" << endl;
    }

    printf("Execution time: %f seconds\n", (end_time - start_time));
    cout << "Minimum cost for TSP (Travelling Salesman Problem): " << minCost << endl;

    return 0;
}

int travllingSalesmanProblem(int graph[][V], int start, vector<vector<int>>& optimalTours, int num_processors)
{
    vector<int> vertex;
    for (int i = 0; i < V; i++)
        if (i != start)
            vertex.push_back(i);

    int min_path = INT_MAX;

#pragma omp parallel for num_threads(num_processors) shared(min_path, optimalTours)
    for (int permIndex = 0; permIndex < factorial(vertex.size()); permIndex++) {
        vector<int> current_permutation = getPermutation(vertex, permIndex);
        int current_pathweight = 0;
        int k = start;
        bool invalidPath = false;
        for (int i = 0; i < current_permutation.size(); i++) {
            if (graph[k][current_permutation[i]] == UNAVAILABLE) {
                invalidPath = true;
                break;
            }
            current_pathweight += graph[k][current_permutation[i]];
            k = current_permutation[i];
        }
        if (invalidPath) {
            continue; // Skip unavailable paths
        }
        current_pathweight += graph[k][start];

#pragma omp critical
        {
            if (current_pathweight < min_path) {
                min_path = current_pathweight;
                optimalTours.clear();
                optimalTours.push_back({start});
                for (int i = 0; i < current_permutation.size(); i++) {
                    optimalTours.back().push_back(current_permutation[i]);
                }
            } else if (current_pathweight == min_path) {
                optimalTours.push_back({start});
                for (int i = 0; i < current_permutation.size(); i++) {
                    optimalTours.back().push_back(current_permutation[i]);
                }
            }
        }
    }

    return min_path;
}

// Implement the function that calculates factorial
int factorial(int n)
{
    int result = 1;
    for (int i = 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

// Implement the function that returns a specific permutation
std::vector<int> getPermutation(const std::vector<int>& elements, int index)
{
    std::vector<int> permutation(elements);
    for (int i = elements.size() - 1; i > 0; i--)
    {
        int j = index % (i + 1);
        std::swap(permutation[i], permutation[j]);
        index /= (i + 1);
    }
    return permutation;
}

