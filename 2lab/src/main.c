#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "car.h"

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