#pragma once

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmsr/dsrdoc.h"       /* for main interface class DSRDocument */

#include "dcmtk/dcmdata/dctk.h"

#include "dcmtk/ofstd/ofstream.h"
#include "dcmtk/ofstd/ofconapp.h"

class DcmParser
{
public:

    bool loadDcm(const std::string& file);

    std::string getStudyInstanceUid();
    std::string getSeriesInstanceUid();
    std::string getPatientId();
    std::string getPatientName();
    std::string getStudyId();
    std::string getSeriesNumber();
    std::string getInstanceNumber();

private:

    std::string findString(const DcmTagKey& , DcmItem *);

    std::unique_ptr<DcmFileFormat> m_dcm;
    DcmMetaInfo* m_metaInfo = nullptr;
    DcmDataset* m_dataset = nullptr;
};

