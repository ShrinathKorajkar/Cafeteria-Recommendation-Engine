#ifndef GENERATEREPORT_H
#define GENERATEREPORT_H

#include "utility.h"

#include <vector>
#include <string>

class ReportGenerator
{
    const std::string filename = "./assets/food_report.csv";

public:
    std::string generateReport(const std::vector<ReportData> &reportData);
};

#endif // GENERATEREPORT_H
