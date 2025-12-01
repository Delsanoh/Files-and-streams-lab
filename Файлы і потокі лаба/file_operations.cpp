#include "sparse_vector.h"

bool readVectorsFromFile(const std::string& filename, SparseVector& v1, SparseVector& v2)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << std::format("Ошибка: невозможно открыть файл {}", filename) << std::endl;
        return false;
    }
    clearVector(v1);
    clearVector(v2);
    std::string line;
    bool readingFirstVector = true;
    bool foundSeparator = false;
    int lineNumber = 0;
    while (std::getline(file, line))
    {
        lineNumber++;
        std::string trimmedLine = trim(line);
        if (trimmedLine.empty())
        {
            continue;
        }
        if (trimmedLine == "---" || trimmedLine == "***" || trimmedLine == "###")
        {
            if (readingFirstVector)
            {
                readingFirstVector = false;
                foundSeparator = true;
            }
            else
            {
                std::cerr << std::format("Ошибка в строке {}: обнаружено несколько разделителей", lineNumber) << std::endl;
                file.close();
                return false;
            }
            continue;
        }
        std::istringstream iss(trimmedLine);
        size_t index;
        int value;
        if (iss >> index >> value)
        {
            std::string remaining;
            if (iss >> remaining)
            {
                std::cerr << std::format("Ошибка в строке {}: лишние символы в строке: '{}'", lineNumber, remaining) << std::endl;
                file.close();
                return false;
            }
            if (readingFirstVector)
            {
                setCoordinate(v1, index, value);
            }
            else
            {
                setCoordinate(v2, index, value);
            }
        }
        else
        {
            std::cerr << std::format("Ошибка в строке {}: неверный формат строки: '{}'", lineNumber, trimmedLine) << std::endl;
            file.close();
            return false;
        }
    }
    file.close();
    if (!foundSeparator && getNonZeroCount(v2) > 0)
    {
        std::cerr << "Ошибка: не найден разделитель между векторами" << std::endl;
        return false;
    }
    std::cout << std::format("Успешно прочитано: {} элементов в первом векторе, {} элементов во втором векторе", getNonZeroCount(v1), getNonZeroCount(v2)) << std::endl;
    return true;
}

bool readVectorFromStream(std::istream& is, SparseVector& vector)
{
    clearVector(vector);
    std::string line;
    while (std::getline(is, line))
    {
        std::string trimmedLine = trim(line);
        if (trimmedLine.empty())
        {
            continue;
        }
        if (trimmedLine == "end" || trimmedLine == "END")
        {
            break;
        }
        std::istringstream iss(trimmedLine);
        size_t index;
        int value;
        if (iss >> index >> value)
        {
            std::string remaining;
            if (iss >> remaining)
            {
                std::cerr << std::format("Ошибка: лишние символы в строке: '{}'", remaining) << std::endl;
                return false;
            }
            setCoordinate(vector, index, value);
        }
        else
        {
            std::cerr << std::format("Ошибка: неверный формат строки: '{}'", trimmedLine) << std::endl;
            return false;
        }
    }
    return true;
}

void readVectorFromConsole(SparseVector& vector, const std::string& prompt)
{
    std::cout << prompt << std::endl;
    std::cout << "Введите координаты в формате: индекс значение" << std::endl;
    std::cout << "Для завершения ввода введите 'end'" << std::endl;
    readVectorFromStream(std::cin, vector);
}