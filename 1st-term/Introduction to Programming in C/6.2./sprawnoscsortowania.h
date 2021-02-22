void swap(double *a, double *b);

void insertSort(double t[], int n, int(*comparator)(const void*, const void*));

void quicksort(double t[], int n, int(*comparator)(const void*, const void*));

void print(const double *t, const int n);

double randDouble();

void assignRand(double t[], int n);

int less(const void* a, const void*b);

_Bool sortPerformsCorrectly(double *t, int n, void(*sort)(double t[], int n, int(*comparator)(const void*, const void*)));

int countRandomSortTime(int n, void(*sort)(double t[], int n, int(*comparator)(const void*, const void*)));

void printRandomSortStats(int length, int iterations, void(*sort)(double t[], int n, int(*comparator)(const void*, const void*)));
