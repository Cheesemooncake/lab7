#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

#define INF INT_MAX

int* Dijkstra(int** mat, int size, int start)
{
    int* d = new int[size];
    bool* used = new bool[size];
    int min;
    int min_index;

    for (int i = 0; i < size; i++)
    {
        d[i] = INF;
        used[i] = false;
    }

    d[start] = 0;

    for (int i = 0; i < size - 1; i++)
    {
        min = INF;

        for (int j = 0; j < size; j++)
            if (!used[j] && d[j] <= min)
            {
                min = d[j];
                min_index = j;
            }

        used[min_index] = true;

        for (int j = 0; j < size; j++)
            if (!used[j] && mat[min_index][j] && d[min_index] != INF && d[min_index] + mat[min_index][j] < d[j])
                d[j] = d[min_index] + mat[min_index][j];
    }

    delete[] used;

    return d;
}

stack<int> FindWay(int** mat, int size, int* d, int start, int finish)
{
    stack<int> way;
    way.push(finish + 1);
    while (finish != start)
    {
        for (int i = 0; i < size; i++)
            if (mat[i][finish] != 0)
                if (d[finish] - mat[i][finish] == d[i])
                {
                    finish = i;
                    way.push(i + 1);
                }
    }

    return way;
}

int** ReadMatrixFromFile(char* filename, int& n, int& m)
{
    ifstream fin;
    fin.open(filename, ios_base::in);

    if (!fin)
        return nullptr;

    fin >> n >> m;

    int** arr = new int* [n];
    for (int i = 0; i < n; i++)
        arr[i] = new int[m];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fin >> arr[i][j];

    fin.close();

    return arr;
}

int main() {

    setlocale(LC_ALL, "Russian");

    ofstream fout;
    fout.open("output.txt", ios_base::trunc);

    int n, m;
    int start, finish;
    int** matrix = ReadMatrixFromFile((char*)"input.txt", n, m);
    int* distance;
    stack <int> way;

    if (!matrix)
    {
        cout << "Невозможно открыть файл или он пуст!\n";
        fout << "Невозможно открыть файл или он пуст!\n";
        return 0;
    }

    if (n != m)
    {
        cout << "Матрица не является квадратной!\n";
        fout << "Матрица не является квадратной!\n";
        return 0;
    }

    cout << "Введите номер вершины от которой нужно посчитать расстояние: ";
    cin >> start;
    distance = Dijkstra(matrix, n, start - 1);

    for (int i = 0; i < n; i++)
    {
        cout << "Расстояние от вершины " << start << " до вершины " << i + 1 << " = " << distance[i] << endl;
        fout << "Расстояние от вершины " << start << " до вершины " << i + 1 << " = " << distance[i] << endl;
    }

    cout << "Введите вершину, в которую нужно построить маршрут: ";
    cin >> finish;

    way = FindWay(matrix, n, distance, start - 1, finish - 1);
    cout << "Путь: ";
    fout << "Путь: ";
    while (way.size() > 1) {

        cout << way.top() << "->";
        fout << way.top() << "->";
        way.pop();
    }
    cout << way.top() << endl;
    fout << way.top() << endl;
    way.pop();

    fout.close();

    for (int i = 0; i < n; i++)
        delete[] matrix[i];
    delete[] matrix;
    delete[] distance;

    return 0;
}