#include <iostream>
#include <vector>
#include <climits>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

void printArray(const vector<int>& arr) {
    cout << "\nArray Elements: ";
    for (int x : arr)
        cout << x << " ";
    cout << endl;
}

void generateRandomArray(vector<int>& arr, int N) {
    srand(time(0));
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 10000;  // Random integers between 0 and 9999
    }
}

void min(const vector<int>& arr, double& min_seq_time, double& min_par_time) {
    int min_seq = INT_MAX;
    double start = omp_get_wtime();
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] < min_seq)
            min_seq = arr[i];
    }
    double end = omp_get_wtime();
    min_seq_time = end - start;

    int min_par = INT_MAX;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(min:min_par) schedule(static)
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] < min_par)
            min_par = arr[i];
    }
    end = omp_get_wtime();
    min_par_time = end - start;

    cout << "Minimum (Seq): " << min_seq << ", Parallel: " << min_par << endl;
}

void max(const vector<int>& arr, double& max_seq_time, double& max_par_time) {
    int max_seq = INT_MIN;
    double start = omp_get_wtime();
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] > max_seq)
            max_seq = arr[i];
    }
    double end = omp_get_wtime();
    max_seq_time = end - start;

    int max_par = INT_MIN;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(max:max_par) schedule(static)
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] > max_par)
            max_par = arr[i];
    }
    end = omp_get_wtime();
    max_par_time = end - start;

    cout << "Maximum (Seq): " << max_seq << ", Parallel: " << max_par << endl;
}

void sum(const vector<int>& arr, double& sum_seq_time, double& sum_par_time) {
    int sum_seq = 0;
    double start = omp_get_wtime();
    for (int i = 0; i < arr.size(); i++)
        sum_seq += arr[i];
    double end = omp_get_wtime();
    sum_seq_time = end - start;

    int sum_par = 0;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum_par) schedule(static)
    for (int i = 0; i < arr.size(); i++)
        sum_par += arr[i];
    end = omp_get_wtime();
    sum_par_time = end - start;

    cout << "Sum (Seq): " << sum_seq << ", Parallel: " << sum_par << endl;
}

void average(const vector<int>& arr, double& avg_seq_time, double& avg_par_time) {
    float avg_seq = 0;
    double start = omp_get_wtime();
    for (int i = 0; i < arr.size(); i++)
        avg_seq += arr[i];
    avg_seq /= arr.size();
    double end = omp_get_wtime();
    avg_seq_time = end - start;

    float avg_par = 0;
    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:avg_par) schedule(static)
    for (int i = 0; i < arr.size(); i++)
        avg_par += arr[i];
    avg_par /= arr.size();
    end = omp_get_wtime();
    avg_par_time = end - start;

    cout << "Average (Seq): " << avg_seq << ", Parallel: " << avg_par << endl;
}

int main() {
    int N;
    cout << "Enter number of elements in array: ";
    cin >> N;

    vector<int> arr(N);
    generateRandomArray(arr, N);  // Generate a random array
    printArray(arr);

    // Set the number of threads
    omp_set_num_threads(4); // Adjust according to your CPU cores

    // Time variables
    double min_seq_time, min_par_time, max_seq_time, max_par_time;
    double sum_seq_time, sum_par_time, avg_seq_time, avg_par_time;

    // Call functions
    min(arr, min_seq_time, min_par_time);
    max(arr, max_seq_time, max_par_time);
    sum(arr, sum_seq_time, sum_par_time);
    average(arr, avg_seq_time, avg_par_time);

    // Final Table
    cout << "\n------------------------------------------------------------\n";
    cout << "Operation      Seq Time (s)  Par Time (s)  Speedup\n";
    cout << "------------------------------------------------------------\n";

    // Check if times are non-zero before calculating speedup
    auto safe_speedup = [](double seq_time, double par_time) {
        if (seq_time > 0 && par_time > 0)
            return seq_time / par_time;
        return 0.0; // Avoid division by zero
    };

    cout << "Minimum        " << min_seq_time << "      " << min_par_time << "      " << safe_speedup(min_seq_time, min_par_time) << "x\n";
    cout << "Maximum        " << max_seq_time << "      " << max_par_time << "      " << safe_speedup(max_seq_time, max_par_time) << "x\n";
    cout << "Sum            " << sum_seq_time << "      " << sum_par_time << "      " << safe_speedup(sum_seq_time, sum_par_time) << "x\n";
    cout << "Average        " << avg_seq_time << "      " << avg_par_time << "      " << safe_speedup(avg_seq_time, avg_par_time) << "x\n";
    cout << "------------------------------------------------------------\n";

    return 0;
}
// Exe: g++ -fopenmp red.cpp -o a 
//./a
