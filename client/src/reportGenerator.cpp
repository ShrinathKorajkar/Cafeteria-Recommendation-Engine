#include "reportGenerator.h"

#include <fstream>

std::string ReportGenerator::generateReport(const std::vector<ReportData> &reportData)
{
    std::ofstream reportFile(filename);
    if (reportFile.is_open())
    {
        writeHeader(reportFile);
        writeData(reportData, reportFile);
        reportFile.close();
        return filename;
    }
    return "";
}

void ReportGenerator::writeHeader(std::ofstream &reportFile)
{
    reportFile << "name,totalOrders\n";
}

void ReportGenerator::writeData(const std::vector<ReportData> &reportData, std::ofstream &reportFile)
{
    for (const auto &data : reportData)
    {
        reportFile << data.name << "," << data.totalOrders << "\n";
    }
}
