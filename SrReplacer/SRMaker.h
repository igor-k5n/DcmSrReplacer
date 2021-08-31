#pragma once

#include <memory>
#include <string>
#include <vector>

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmsr/dsrdoc.h"       /* for main interface class DSRDocument */

#include "dcmtk/dcmdata/dctk.h"

#include "dcmtk/ofstd/ofstream.h"
#include "dcmtk/ofstd/ofconapp.h"

class SRMaker
{

public:

    SRMaker() = default;

    bool loadTemplate(const std::string& fileTemplate);
    bool saveFile(const std::string& file);

    bool replacePolyline(const std::vector<float> &);
    bool replaceStudyInstanceUid(const std::string&);
    bool replaceSeriesInstanceUid(const std::string&);
    bool replacePatientId(const std::string&);
    bool replacePatientName(const std::string&);
    bool replaceStudyId(const std::string&);
    bool replaceSeriesNumber(const std::string&);
    bool replaceInstanceNumber(const std::string&);
private:

    template<typename T>
    T* search(const DcmTagKey&, DcmItem* );

    template<typename T, typename R>
    bool replace(const R&, const DcmTagKey& , DcmItem* );
 
    bool replaceValue(DcmElement* el, const std::vector<float>& value);
    bool replaceValue(DcmElement* el, const std::string& value);
    bool replaceValue(DcmElement* el, const uint32_t& value);

    std::unique_ptr<DcmFileFormat> m_dcmTemplate;
    DcmMetaInfo* m_metaInfo = nullptr;
    DcmDataset* m_dataset = nullptr;
    DcmStack m_stack;
};
