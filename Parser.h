#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

class Parser
{
protected:
    std::unordered_map<std::string, int> categoryMap;
    std::vector<std::vector<std::string>> dataList;
    
public:
    virtual void initialize(const std::string&) = 0;
    
    int getIntData(int row, std::string column)
    {
        if (categoryMap.find(column) == categoryMap.end())
        {
            throw(std::runtime_error("Invalid column"));
        }

        return std::stoi(dataList[row][categoryMap[column]]);
    }

    std::string getStringData(int row, std::string column)
    {
        if (categoryMap.find(column) == categoryMap.end())
        {
            throw(std::runtime_error("Invalid column"));
        }
        
        return dataList[row][categoryMap[column]];
    }

    float getFloatData(int row, std::string column)
    {
        if (categoryMap.find(column) == categoryMap.end())
        {
            throw(std::runtime_error("Invalid column"));
        }

        return std::stof(dataList[row][categoryMap[column]]);
    }

    long long getLongLongData(int row, std::string column)
    {
        if (categoryMap.find(column) == categoryMap.end())
        {
            throw(std::runtime_error("Invalid column"));
        }

        return std::stoll(dataList[row][categoryMap[column]]);
    }

    double getDoubleData(int row, std::string column)
    {
        if (categoryMap.find(column) == categoryMap.end())
        {
            throw(std::runtime_error("Invalid column"));
        }

        return std::stod(dataList[row][categoryMap[column]]);
    }

    int getLine()
    {
        return dataList.size();
    }
};

class CSVParser : public Parser
{
private:
    std::vector<std::string> getLine(const std::string& line)
    {
        std::string s = "";
        std::vector<std::string> datas;
        for (int i = 0; i < line.size(); ++i)
        {
            char c = line[i];
            if (c == ',')
            {
                datas.push_back(s);
                s.clear();

                continue;
            }
            
            s += line[i];
        }

        datas.push_back(s);

        return datas;
    }
    
public:
    virtual void initialize(const std::string& filename) override
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw(std::runtime_error("File not found"));
        }

        categoryMap.clear();
        dataList.clear();

        std::vector<std::string> header;
        
        std::string line;
        std::getline(file, line);

        header = getLine(line);
        for (int i = 0; i < header.size(); ++i)
        {
            categoryMap[header[i]] = i;
        }

        std::string dataLine;
        while (std::getline(file, dataLine))
        {
            if (dataLine.empty())
            {
                continue;
            }

            dataList.emplace_back(getLine(dataLine));
            dataLine.clear();
        }

        file.close();
    }
};

class JSONParser : public Parser
{
private:
        
    
public:
};
