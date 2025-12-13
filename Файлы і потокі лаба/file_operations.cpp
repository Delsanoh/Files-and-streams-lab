#include "sparse_vector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <format>

namespace fs = std::filesystem;


void checkFile(const std::string& filename) {
    // Проверка существования файла
    if (!fs::exists(filename)) {
        throw std::runtime_error(std::format("Ошибка: файл '{}' не существует", filename));
    }

    // Проверка, что это файл, а не директория
    if (!fs::is_regular_file(filename)) {
        throw std::runtime_error(std::format("Ошибка: '{}' не является обычным файлом", filename));
    }

    // Проверка, что файл не пустой
    if (fs::file_size(filename) == 0) {
        throw std::runtime_error(std::format("Ошибка: файл '{}' пуст", filename));
    }
}

bool readVectorsFromFile(const std::string& filename, SparseVector& v1, SparseVector& v2)
{
    try {
        // Проверяем файл
        checkFile(filename);
    }
    catch (const std::exception& e) {
        throw; // Перебрасываем исключение дальше
    }

    // Открытие файла
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error(std::format("Ошибка: невозможно открыть файл '{}'", filename));
    }

    // Дополнительная проверка на пустоту
    if (file.peek() == std::ifstream::traits_type::eof()) {
        file.close();
        throw std::runtime_error(std::format("Ошибка: файл '{}' пуст", filename));
    }

    clearVector(v1);
    clearVector(v2);
    std::string line;
    bool readingFirstVector = true;
    bool foundSeparator = false;
    int lineNumber = 0;
    bool anyDataRead = false;

    while (std::getline(file, line))
    {
        lineNumber++;
        std::string trimmedLine = trim(line);

        // Пропускаем пустые строки
        if (trimmedLine.empty()) {
            continue;
        }

        anyDataRead = true;

        if (trimmedLine == "---" || trimmedLine == "***" || trimmedLine == "###")
        {
            if (readingFirstVector) {
                readingFirstVector = false;
                foundSeparator = true;
            }
            else {
                file.close();
                throw std::runtime_error(
                    std::format("Ошибка в строке {}: обнаружено несколько разделителей", lineNumber)
                );
            }
            continue;
        }

        std::istringstream iss(trimmedLine);
        size_t index;
        int value;

        if (iss >> index >> value) {
            std::string remaining;
            if (iss >> remaining) {
                file.close();
                throw std::runtime_error(
                    std::format("Ошибка в строке {}: лишние символы в строке: '{}'", lineNumber, remaining)
                );
            }
            if (readingFirstVector) {
                setCoordinate(v1, index, value);
            }
            else {
                setCoordinate(v2, index, value);
            }
        }
        else {
            file.close();
            throw std::runtime_error(
                std::format("Ошибка в строке {}: неверный формат строки: '{}'", lineNumber, trimmedLine)
            );
        }
    }

    file.close();

    if (!anyDataRead) {
        throw std::runtime_error("Ошибка: файл не содержит данных");
    }

    if (!foundSeparator && getNonZeroCount(v2) > 0) {
        throw std::runtime_error("Ошибка: не найден разделитель между векторами");
    }

    std::cout << std::format("Успешно прочитано: {} элементов в первом векторе, {} элементов во втором векторе",
        getNonZeroCount(v1), getNonZeroCount(v2)) << std::endl;
    return true;
}

bool readVectorFromStream(std::istream& is, SparseVector& vector)
{
    clearVector(vector);
    std::string line;

    while (std::getline(is, line)) {
        std::string trimmedLine = trim(line);
        if (trimmedLine.empty()) {
            continue;
        }
        if (trimmedLine == "end" || trimmedLine == "END") {
            break;
        }

        std::istringstream iss(trimmedLine);
        size_t index;
        int value;

        if (iss >> index >> value) {
            std::string remaining;
            if (iss >> remaining) {
                throw std::runtime_error(
                    std::format("Ошибка: лишние символы в строке: '{}'", remaining)
                );
            }
            setCoordinate(vector, index, value);
        }
        else {
            throw std::runtime_error(
                std::format("Ошибка: неверный формат строки: '{}'", trimmedLine)
            );
        }
    }
    return true;
}

void readVectorFromConsole(SparseVector& vector, const std::string& prompt)
{
    std::cout << prompt << std::endl;
    std::cout << "Введите координаты в формате: индекс значение" << std::endl;
    std::cout << "Для завершения ввода введите 'end'" << std::endl;

    try {
        readVectorFromStream(std::cin, vector);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        throw; // Перебрасываем исключение
    }
}