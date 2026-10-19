#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define N 10
typedef struct {
    char brand[16];
    int year;
    int power;
    int price;

}car;

const char *brands[] = {
        "Toyota", "BMW", "Kia", "Lada", "Hyundai",
    "Audi",   "Ford", "Skoda", "Nissan", "Mazda"
};
const int BRANDS_COUNT = sizeof(brands) / sizeof(brands[0]);

void fillCars(car arr[], int n) {
    for(int i = 0; i < n; i++) {
        strcpy(arr[i].brand, brands[rand() % BRANDS_COUNT]);
        arr[i].year = 1990 + rand() % 32;
        arr[i].power = 50 + rand() % 400;
        arr[i].price = 100000 + rand() % 1000000;
    }
}

void printCars(car arr[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("| %-8s | %4d | %5d   | %10d |\n",
               arr[i].brand,
               arr[i].year,
               arr[i].power,
               arr[i].price);
    }
}

void sortByYear(car arr[], int n){
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++){
            if (arr[j].year < arr[min].year){
                min = j;
            }
        }if(min != i){
            car temp = arr[min];
            arr[min] = arr[i];
            arr[i] = temp;
        }
    }
}

int main() {
    car garage[N];
    srand(time(NULL));
    fillCars(garage, N);
    printf("=== BEFORE SORT ===\n");
    printCars(garage, N);
    sortByYear(garage, N);
    
    printf("=== AFTER SORT BY YEAR ===\n");
    printCars(garage, N);
    return 0;
}