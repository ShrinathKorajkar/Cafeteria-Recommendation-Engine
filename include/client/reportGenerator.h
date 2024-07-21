#pragma once

#include <vector>
#include <string>

struct ReportData
{
    std::string name;
    int totalOrders;

    ReportData(const std::string &name, int totalOrders)
        : name(name), totalOrders(totalOrders) {}
};

class ReportGenerator
{
private:
    const std::string filename = "./assets/food_report.csv";

    void writeHeader(std::ofstream &outFile);
    void writeData(const std::vector<ReportData> &reportData, std::ofstream &outFile);

public:
    ReportGenerator();
    std::string generateReport(const std::vector<ReportData> &reportData);
};
