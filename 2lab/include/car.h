#define N 10
typedef struct {
    char brand[16];
    int year;
    int power;
    int price;

}car;

void fillCars(car arr[], int n);
void printCars(car arr[], int n);
void SortByPrice(car arr[], int n);
void sortByYear(car arr[], int n);