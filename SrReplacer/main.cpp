#include "DebugLog.h"

#include "SRMaker.h"
#include "TextFileParser.h"
#include "DcmParser.h"
#include "JsonParser.h"

int main(int argc, char** argv)
{
    if (argc < 6)
    {
        std::cout << "<in dcm template file> <in text|json file> <in dcm file> <out dcm file> <c|m>" << std::endl;
        return 0;
    }

    SRMaker srMaker;
    DcmParser dcmParser;

    std::string inTemplateFile = argv[1];
    std::string inFile = argv[2];
    std::string inDcmFile = argv[3];
    std::string outDcmFile = argv[4];
    std::string mode = argv[5];

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

    if (mode == "c")
    {
        TextFileParser textParser;

        if (!textParser.load(inFile))
        {
            PRINT_ERROR("load text file");
            return -1;
        }

        auto polylines = textParser.getPolyline();
        if (polylines.empty())
        {
            PRINT_ERROR("polyline is empty in json");
            return -1;
        }

        if (!srMaker.replacePolyline(polylines))
        {
            PRINT_ERROR("replace polyline");
            return -1;
        }
    }
    else
    {
        JsonParser jsonParser;

        if (!jsonParser.loadJson(inFile))
        {
            PRINT_ERROR("load json file");
            return -1;
        }

        auto measurements = jsonParser.getMeasurments();

        if (!measurements.empty())
        {
            srMaker.insertMeasurements(measurements);
        }
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

    if (!srMaker.replaceSeriesDescription(""))
    {
        PRINT_ERROR("replce series description");
        return -1;
    }

    if (!srMaker.replaceInstanceNumber(dcmParser.getInstanceNumber()))
    {
        PRINT_ERROR("replace patient name");
        return -1;
    }

    return srMaker.saveFile(outDcmFile) ? 0 : -1;
}
