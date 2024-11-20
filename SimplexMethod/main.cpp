#include "SimplexMethod.h"

int main() {
    setlocale(LC_ALL, "Russian");
    // Задача:
    // Максимизировать F = 68x1 + 57x2 + 45x3
    // при ограничениях:
    // 0.6x1 + 0.15x2 <= 2000
    // 0.2x1 + 0.6x2 + 0.5x3 <= 1200
    // x1, x2, x3 >= 0

    int numVariables = 3;  // x1, x2, x3

    vector<vector<double>> table1 = {
        {0.6, 0.15, 0, 1, 0, 2000},
        {0.2, 0.6, 0.5, 0, 1, 1200},
        {-68, -57, -45, 0, 0, 0}
    };

    vector<vector<double>> table3 = {
        {1, 3, 1, 0, 0, 0, 18},
        {2, 1, 0, 1, 0, 0, 16},
        {0, 1, 0, 0, 1, 0, 5},
        {3, 0, 0, 0, 0, 1, 21},
        {-2, -3, 0, 0, 0, 0, 0}
    };
    
    // Создаем объект класса SimplexSolver
    SimplexMethod solver(table1, numVariables);

    try
    {
        // Решаем задачу
        solver.Solve();
    }
    catch (const char * err)
    {
        cout << err;
        exit(-1);
    }
    

    // Получаем результат
    solver.GetResult();

    return 0;
}