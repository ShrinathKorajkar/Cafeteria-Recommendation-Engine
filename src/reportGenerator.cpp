#include "reportGenerator.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::string ReportGenerator::generateReport(const std::vector<ReportData> &reportData)
{
    std::ofstream outFile(filename);
    if (outFile.is_open())
    {
        outFile << "name,totalOrders\n";
        for (const auto &data : reportData)
        {
            outFile << data.name << "," << data.totalOrders << "\n";
        }
        outFile.close();
        return filename;
    }
    return "";
}
