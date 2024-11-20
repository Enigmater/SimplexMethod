#include "SimplexMethod.h"
#include <string>

int SimplexMethod::GetPivotColumn()
{
    int pivotCol = -1;
    double minValue = 0;

    for (int j = 0; j < table[table.size() - 1].size(); j++) {
        if (table[table.size() - 1][j] < minValue) {
            minValue = table[table.size() - 1][j];
            pivotCol = j;
        }
    }

    return pivotCol;
}

int SimplexMethod::GetPivotRow(int pivotCol)
{
    int pivotRow = -1;
    double minRatio = numeric_limits<double>::infinity();

    for (int i = 0; i < table.size() - 1; i++) {
        double b = table[i][table[i].size() - 1];
        // Ограничения для каждой строки: 
        if (table[i][pivotCol] > 0 && b > 0 || table[i][pivotCol] < 0 && b < 0) {
            double ratio = b / table[i][pivotCol];
            if (ratio < minRatio) {
                minRatio = ratio;
                pivotRow = i;
            }
        }
    }

    return pivotRow;
}

void SimplexMethod::Pivot(int pivotRow, int pivotCol)
{
    double pivotValue = table[pivotRow][pivotCol];
    vector<vector<double>> newTable(table.size(), vector<double>(table[0].size()));
    // б) Проставляем 1 в столбце новой основной переменной (pivotCol) и 0 в столбцах старых переменных
    for (int i = 0; i < table.size(); ++i) {
        if (i == pivotRow) newTable[i][pivotCol] = 1;  // новая основная переменная
        else newTable[i][pivotCol] = 0;  // все остальные строки, кроме pivotRow, получат 0 в столбце новой переменной       
    }

    // в) Делим всю строку pivotRow на элемент pivotValue
    for (int j = 0; j < table[0].size(); ++j) {
        newTable[pivotRow][j] = table[pivotRow][j] / pivotValue;
    }

    // г) Обновляем остальные строки
    for (int i = 0; i < table.size(); ++i) {
        if (i != pivotRow) {
            for (int j = 0; j < table[i].size(); ++j) {
                newTable[i][j] = table[i][j] - table[i][pivotCol] * table[pivotRow][j] / pivotValue;
            }
        }
    }
    table = newTable;

    // Обновляем список основных и неосновных переменных
    swap(basicVariables[pivotRow], *find(nonBasicVariables.begin(), nonBasicVariables.end(), pivotCol));
}

SimplexMethod::SimplexMethod(vector<vector<double>> table, int numVariables)
{
    this->table = table;
    this->numVariables = numVariables;

    // Изначально все переменные неосновные
    for (int i = 0; i < numVariables; ++i) {
        nonBasicVariables.push_back(i);
    }
    // Основные переменные — это дополнительные переменные для ограничений
    for (int i = numVariables; i < table[0].size() - 1; ++i) {
        basicVariables.push_back(i);
    }
}

void SimplexMethod::Solve()
{
    // Итерируем, пока не получим оптимальное решение
    while (true) {
        PrintTable();
        int pivotCol = GetPivotColumn();

        // Если все коэффициенты в целевой функции неотрицательны, мы нашли оптимальное решение
        if (pivotCol < 0) break;

        int pivotRow = GetPivotRow(pivotCol);

        if (pivotRow < 0) throw "Нет решения! Отсутствует конечный оптимум!";     // Если не найдено, выходим

        Pivot(pivotRow, pivotCol);   // Если найдено допустимое решение, проводим элементарное преобразование
            
    }
}

void SimplexMethod::GetResult()
{
    // Выводим оптимальное значение целевой функции
    double optimalValue = table[table.size() - 1][table[0].size() - 1]; // Последний элемент в последней строке
    cout << "Оптимальное значение целевой функции: " << optimalValue << endl;

    // Проходимся по всем переменным от 0 до numVariables
    for (int i = 0; i < numVariables; ++i) {
        cout << "Значение переменной x" << i + 1 << ": ";
        auto it = find(basicVariables.begin(), basicVariables.end(), i);
            // Если переменная основная, то выводим ее значение из столбца b
        if (it != basicVariables.end()) {
            // Вычисляем индекс элемента, на который указывает итератор
            int index = std::distance(basicVariables.begin(), it);
            cout << table[index][table[index].size() - 1] << endl;  // Берем значение свободного члена (последний столбец)
        }
        else {
            // Если переменнаяносновная, то выводим 0
            cout << 0 << endl;
        }
    }
}

void SimplexMethod::PrintTable()
{
    int columnWidth = 10;

    // Начало таблицы
    cout << "+";
    int numColumns = table[0].size();
    for (int j = 0; j < numColumns; ++j) cout << string(columnWidth, '-') << "+";
    cout << string(columnWidth, '-') << "+" << endl;

    // Заголовок: сначала основные переменные, затем неосновные
    cout << "|";

    // Пустая ячейка в верхнем левом углу (0,0)
    cout << " " << setw(columnWidth - 1) << left << " " << "|";

    // Выводим основные переменные (если они есть)
    for (int j : nonBasicVariables) cout << " " << setw(columnWidth - 1) << left << "x" + to_string(j + 1) << "|";
    
    // Выводим неосновные переменные (если они есть)
    for (int j : basicVariables) cout << " " << setw(columnWidth - 1) << left << "x" + to_string(j + 1) << "|";

    // Выводим столбец b
    cout << " " << setw(columnWidth - 1) << left << "b" << "|";
    cout << endl;

    // Разделительная линия после заголовков
    cout << "+";
    for (int j = 0; j < numColumns; ++j) cout << string(columnWidth, '-') << "+";
    cout << string(columnWidth, '-') << "+" << endl;

    // Выводим строки таблицы
    for (int i = 0; i < table.size(); ++i) {
        // Нулевой столбец: выводим основную переменную или "F" в последней строке
        cout << "|";

        if (i == table.size() - 1) cout << " " << setw(columnWidth - 1) << left << "F" << "|";              // Последняя строка - целевая функция, в нулевом столбце выводим "F" 
        else cout << " " << setw(columnWidth - 1) << left << "x" + to_string(basicVariables[i] + 1) << "|"; // Для остальных строк выводим основную переменную 
        

        // Выводим остальные столбцы таблицы (значения)
        for (int j = 0; j < numColumns - 1; ++j) cout << " " << setw(columnWidth - 1) << left << fixed << setprecision(2) << table[i][j] << "|";

        // Выводим столбец "b" для всех строк
        cout << " " << setw(columnWidth - 1) << left << fixed << setprecision(2) << table[i][numColumns - 1] << "|";
        cout << endl;

        // Разделительная линия после строки
        cout << "+";
        for (int j = 0; j < numColumns; ++j) cout << string(columnWidth, '-') << "+";
        cout << string(columnWidth, '-') << "+" << endl;
    }
    cout << endl;
}
