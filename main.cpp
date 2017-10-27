#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

long max(long a, long b) { return (a > b) ? a : b; }

long **alloc2D(long h, long w)
{
	long i;
	long *array = new long[h * w];
	if(array == 0) return 0;
	long **array2D = new long*[h];
	if(array2D == 0) { delete array; return NULL; }
	for (i = 0; i < h; i++)
		array2D[i] = &array[i * w];
	return array2D;
}

void free2D(long **array)
{
	if(array != NULL) delete[] *array;
	delete[] array;

	return;
}

// knapsack algorithm

long knapSack(long max_weight, long weight[], long profit[], long n, vector<pair<long, long> > &backtrace)
{
	long **dynamic = alloc2D(n + 1, max_weight + 1);

	for (long i = 0; i <= n; i++) {
		for (long w = 0; w <= max_weight; w++) {
			if (i == 0 || w == 0) {
				dynamic[i][w] = 0;
			} else if (weight[i - 1] <= w) {
				dynamic[i][w] = 
					max(profit[i - 1] + dynamic[i - 1][w - weight[i - 1]], 
						dynamic[i - 1][w]);
			} else {
				dynamic[i][w] = dynamic[i-1][w];
			}
		}
	}
	
	int row = n;
	int column = max_weight;
	
	while (column > 0) {
		if (dynamic[row][column] != dynamic[row - 1][column]) {
			backtrace.push_back(make_pair(weight[row - 1], profit[row - 1]));
			row--;
			column -= weight[row - 1];
		} else {
			row--;
		}
	}
	
	
	long result = dynamic[n][max_weight];
	
	free2D(dynamic);
	return result;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		cerr << "wrong arguments" << endl;  // incorrect argument
		return -1;
	}
	
	ifstream in(argv[1]);
	ofstream out(argv[2]);
	
	if (!in || !out) {
		cerr << "can't open file" << endl;  // missing file
		return -1;
	}
	
	
	long n, W;
	long *w, *p;
	time_t start, finish;
	
	while (in >> n >> W) {
		w = new long[n];
		p = new long[n];
		
		for (long i = 0; i < n; i++) {
			in >> w[i] >> p[i];
		}
		
		vector<pair<long, long> > backtrace;  
		start = time(0);
		long result = knapSack(W, w, p, n, backtrace);
		finish = time(0);
		
		out << n << ' ' << result << ' ' 
			<< (double) finish - start << "s" << endl;
		for (unsigned i = 0; i < backtrace.size(); i++) {
			out << backtrace[i].first << ' ' << backtrace[i].second << endl;
		}
	}
	
	
	return 0;
}
