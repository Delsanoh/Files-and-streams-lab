#include "sparse_vector.h"
#include <iostream>
#include <fstream>

int main()
{
    std::setlocale(LC_ALL, "Russian");
    SparseVector v1;
    SparseVector v2;
    initVector(v1);
    initVector(v2);

    int choice;
    std::cout << "Скалярное умножение разреженных векторов" << std::endl;

    while (true) {
        std::cout << "\nВыберите способ ввода:" << std::endl;
        std::cout << "1 - Ввод с консоли" << std::endl;
        std::cout << "2 - Чтение из файла in.txt" << std::endl;
        std::cout << "3 - Выход" << std::endl;
        std::cout << "Ваш выбор: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Неверный ввод. Попробуйте снова." << std::endl;
            continue;
        }
        std::cin.ignore();

        if (choice == 3) {
            break;
        }

        bool success = false;

        try {
            switch (choice) {
            case 1:
                readVectorFromConsole(v1, "Ввод первого вектора:");
                readVectorFromConsole(v2, "Ввод второго вектора:");
                success = true;
                break;

            case 2:
                if (readVectorsFromFile("in.txt", v1, v2)) {
                    success = true;
                }
                break;

            default:
                std::cout << "Неверный выбор. Попробуйте снова." << std::endl;
                continue;
            }
        }
        catch (const std::exception& e) {
            std::cout << "\nОшибка: " << e.what() << std::endl;
            clearVector(v1);
            clearVector(v2);
            continue;
        }

        if (success) {
            try {
                long result = scalarProduct(v1, v2);
                std::cout << std::format("\nРезультат скалярного умножения: {}", result) << std::endl;

                std::cout << "\nНенулевые координаты первого вектора:" << std::endl;
                printVector(v1, std::cout);

                std::cout << "\nНенулевые координаты второго вектора:" << std::endl;
                printVector(v2, std::cout);

                std::ofstream outFile("out.txt");
                if (outFile.is_open()) {
                    outFile << std::format("Результат скалярного умножения: {}", result) << std::endl;
                    outFile.close();
                    std::cout << "Результат также записан в файл out.txt" << std::endl;
                }
                else {
                    std::cout << "Не удалось открыть файл out.txt для записи" << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cout << "\nОшибка при вычислении: " << e.what() << std::endl;
            }
        }

        clearVector(v1);
        clearVector(v2);
    }

    std::cout << "Программа завершена." << std::endl;
    return 0;
}