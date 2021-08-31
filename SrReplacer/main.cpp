#include "DebugLog.h"

#include "SRMaker.h"

#include "JsonParser.h"
#include "DcmParser.h"

int main(int argc, char** argv)
{
    if (argc < 5)
    {
        std::cout << "<in dcm template file> <in json file> <in dcm file> <out dcm file>";
        return 0;
    }

    SRMaker srMaker;
    DcmParser dcmParser;
    JsonParser jsonParser;

    std::string inTemplateFile = argv[1];
    std::string inJsonFile = argv[2];
    std::string inDcmFile = argv[3];
    std::string outDcmFile = argv[4];

    if (!srMaker.loadTemplate(inTemplateFile))
    {
        PRINT_ERROR("load template file");
        return -1;
    }

    if (!dcmParser.loadDcm(inDcmFile))
    {
        PRINT_ERROR("load dcm file");
        return -1;
    }

    if (!jsonParser.loadJson(inJsonFile))
    {
        PRINT_ERROR("load json file");
        return -1;
    }

    auto polyline = jsonParser.getPolyline();
    if (polyline.empty())
    {
        PRINT_ERROR("polyline is empty in json");
        return -1;
    }

    if (!srMaker.replaceStudyInstanceUid(dcmParser.getStudyInstanceUid()))
    {
        PRINT_ERROR("replace study instance uid");
        return -1;
    }

    if (!srMaker.replaceSeriesInstanceUid(dcmParser.getSeriesInstanceUid()))
    {
        PRINT_ERROR("replace series instance uid");
        return -1;
    }

    if (!srMaker.replacePatientId(dcmParser.getPatientId()))
    {
        PRINT_ERROR("replace patient id");
        return -1;
    }

    if (!srMaker.replacePatientName(dcmParser.getPatientName()))
    {
        PRINT_ERROR("replace patient name");
        return -1;
    }

    if (!srMaker.replaceStudyId(dcmParser.getStudyId()))
    {
        PRINT_ERROR("replace patient name");
        return -1;
    }
    
    if (!srMaker.replaceSeriesNumber(dcmParser.getSeriesNumber()))
    {
        PRINT_ERROR("replace patient name");
        return -1;
    }

    if (!srMaker.replaceInstanceNumber(dcmParser.getInstanceNumber()))
    {
        PRINT_ERROR("replace patient name");
        return -1;
    }

    if (!srMaker.replacePolyline(polyline))
    {
        PRINT_ERROR("replace polyline");
        return -1;
    }

    return srMaker.saveFile(outDcmFile) ? 0 : -1;
}