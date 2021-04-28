#include <iostream>
#include <limits>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <stdio.h>

using namespace std;

const int INF = numeric_limits<int>::max();
int N = 0;
int* queens;

int* colCount;
int* upperDiagCount;
int* lowerDiagCount;

int getNumberOfConflicts(int excludeRow) {
	int conflicts=0;

	for(int i=0; i<2*N-1; i++) {
		if(i<N) {
			colCount[i]=0;
		}
		upperDiagCount[i]=lowerDiagCount[i]=0;
	}

	for(int i=0; i<N; i++) {
		if(i != excludeRow) {
			colCount[queens[i]] += 1;
			upperDiagCount[queens[i]+i] += 1;
			lowerDiagCount[(N-queens[i])+i-1] += 1;
		}
	}

	for(int i=0; i<2*N-1; i++) {
		if(i<N) {
			conflicts += ((colCount[i]-1)*colCount[i])/2;
		}
		conflicts += ((upperDiagCount[i]-1)*upperDiagCount[i])/2;
		conflicts += ((lowerDiagCount[i]-1)*lowerDiagCount[i])/2;
	}
	return conflicts;
}

int getNumberOfConflicts() {
	return getNumberOfConflicts(-1);
}

void initBoard() {
	queens = new int[N];

	colCount = new int[N];
	upperDiagCount = new int[(2*N)-1];
	lowerDiagCount = new int[(2*N)-1];

	for(int i=0; i<2*N-1; i++) {
		if(i<N) {
			colCount[i]=0;
		}
		upperDiagCount[i]=lowerDiagCount[i]=0;
	}

	vector<int> minConflictCols;
	int minConflicts=INF;
	int tempConflicts;

	queens[0] = rand()%N;

	colCount[queens[0]] += 1;
	upperDiagCount[queens[0]+0] += 1;
	lowerDiagCount[(N-queens[0])+0-1] += 1;


	for(int i=1; i<N; i++) {
		minConflictCols.clear();
		minConflicts = INF;

		for(int j=0; j<N; j++) {
			tempConflicts = ((colCount[j]+1)*colCount[j])/2;
			tempConflicts += ((upperDiagCount[j+i]+1)*upperDiagCount[j+i])/2;
			tempConflicts += ((lowerDiagCount[(N-j)+i-1]+1)*lowerDiagCount[(N-j)+i-1])/2;

			if(tempConflicts < minConflicts) {
				minConflicts=tempConflicts;
				minConflictCols.clear();
				minConflictCols.push_back(j);
			} else if(tempConflicts == minConflicts) {
				minConflictCols.push_back(j);
			}
		}
		queens[i] = minConflictCols[rand()%minConflictCols.size()];

		colCount[queens[i]] += 1;
		upperDiagCount[queens[i]+i] += 1;
		lowerDiagCount[(N-queens[i])+i-1] += 1;
	}
}

void printBoard() {
	for(int i=0; i<N; i++) {
		for(int j=0; j<queens[i]; j++) {
			cout << "| ";
		}
		cout << "|Q";
		for(int j=0; j<N-queens[i]-1; j++) {
			cout << "| ";
		}
		cout << "|\n";
	}
	cout << "Conflicts: " << getNumberOfConflicts() << "\n\n";
}

int rowWithMostConflicts() {
	int rowConflicts=0;
	int tempConflicts;
	vector<int> maxConflictRows;

	for(int i=0; i<N; i++) {
		tempConflicts = ((colCount[queens[i]]-1)*colCount[queens[i]])/2;
		tempConflicts += ((upperDiagCount[queens[i]+i]-1)*upperDiagCount[queens[i]+i])/2;
		tempConflicts += ((lowerDiagCount[(N-queens[i])+i-1]-1)*lowerDiagCount[(N-queens[i])+i-1])/2;

		if(tempConflicts > rowConflicts) {
			rowConflicts=tempConflicts;
			maxConflictRows.clear();
			maxConflictRows.push_back(i);
		} else if(tempConflicts == rowConflicts) {
			maxConflictRows.push_back(i);
		}
	}
	return maxConflictRows[rand()%maxConflictRows.size()];
}

void minimumConflicts() {
	int highestConflictRow = rowWithMostConflicts();

	int minConflicts=INF;
	int tempConflicts;

	vector<int> minConflictCols;

	getNumberOfConflicts(highestConflictRow);

	for(int i=0; i<N; i++) {
		tempConflicts = ((colCount[i]+1)*colCount[i])/2;
		tempConflicts += ((upperDiagCount[i+highestConflictRow]+1)*upperDiagCount[i+highestConflictRow])/2;
		tempConflicts += ((lowerDiagCount[(N-i)+highestConflictRow-1]+1)*lowerDiagCount[(N-i)+highestConflictRow-1])/2;

		if(tempConflicts < minConflicts) {
			minConflicts=tempConflicts;
			minConflictCols.clear();
			minConflictCols.push_back(i);
		} else if(tempConflicts == minConflicts) {
			minConflictCols.push_back(i);
		}
	}
	queens[highestConflictRow]=minConflictCols[rand()%minConflictCols.size()];
}

int main(int argc, const char *argv[]) {
    N = atoi(argv[1]);

    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

	srand(time(0));

	initBoard();

    // The initial conflicts
	int previousConflicts = getNumberOfConflicts();
	int newConflicts;

	int count = 0;

	cout << "Solving: ";

    // Go through until we have zero conflicts
	while(previousConflicts != 0)	{
		minimumConflicts();
		newConflicts = getNumberOfConflicts();
		if(previousConflicts == newConflicts) {
			count++;
			if(count>1) {
				queens[rand()%N] = rand()%N;
				count = 0;
			}
		}
		previousConflicts = newConflicts;
	}

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    printf("Time measured: %.3f seconds.\n", elapsed);

	return 0;
}