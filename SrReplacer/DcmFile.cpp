#include "DcmFile.h"

bool DcmFile::loadFile(const std::string& file)
{
    auto dcmTemplate = std::make_unique<DcmFileFormat>();
    OFCondition status = dcmTemplate->loadFile(file.c_str(), EXS_Unknown);
    if (!status.good())
    {
        return false;
    }

    m_metaInfo = dcmTemplate->getMetaInfo();
    m_dataset = dcmTemplate->getDataset();

    if (m_metaInfo == nullptr || m_dataset == nullptr)
    {
        return false;
    }

    m_dcmTemplate = std::move(dcmTemplate);

    return true;
}


