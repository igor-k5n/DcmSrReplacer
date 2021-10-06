#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>

#include "DcmFile.h"

class SRMaker : public DcmFile
{
    using base = DcmFile;

public:

    SRMaker();

    bool loadTemplate(const std::string& fileTemplate);
    bool saveFile(const std::string& file);

    bool replacePolyline(const std::unordered_map<std::string, std::vector<float>> &);
    bool replaceStudyInstanceUid(const std::string&);
    bool replaceSeriesInstanceUid(const std::string&);
    bool replacePatientId(const std::string&);
    bool replacePatientName(const std::string&);
    bool replaceStudyId(const std::string&);
    bool replaceSeriesNumber(const std::string&);
    bool replaceInstanceNumber(const std::string&);
private:

    using TSequences = std::vector<DcmSequenceOfItems*>;

    TSequences searchSequenceInSequence(const std::string& valueType, const std::string& codeValue, DcmSequenceOfItems*);
    DcmItem* searchItemInSequence(const std::string& valueType, const std::string& codeValue, DcmSequenceOfItems*);

    template<typename T, typename R>
    bool replace(const R&, const DcmTagKey& , DcmItem* );
 
    bool replaceValue(DcmElement* el, const std::vector<float>& value);
    bool replaceValue(DcmElement* el, const std::string& value);
    bool replaceValue(DcmElement* el, const uint32_t& value);

    std::string genString(uint32_t length);

    DcmItem* m_measurementItem = nullptr;
    DcmSequenceOfItems* m_imagingMeasurements = nullptr;

    std::default_random_engine m_random;
};
