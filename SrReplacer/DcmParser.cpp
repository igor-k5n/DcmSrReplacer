#include "DcmParser.h"

#include "DebugLog.h"

bool DcmParser::loadDcm(const std::string& file)
{
    auto dcmTemplate = std::make_unique<DcmFileFormat>();
    OFCondition status = dcmTemplate->loadFile(file.c_str(), EXS_Unknown);
    if (!status.good())
    {
        PRINT_ERROR("load file", file);
        return false;
    }

    m_metaInfo = dcmTemplate->getMetaInfo();
    m_dataset = dcmTemplate->getDataset();

    if (m_metaInfo == nullptr || m_dataset == nullptr)
    {
        PRINT_ERROR("meta info is null");
        return false;
    }

    m_dcm = std::move(dcmTemplate);

    return true;
}

std::string DcmParser::getStudyInstanceUid()
{
    return findString(DCM_StudyInstanceUID, m_dataset);
}

std::string DcmParser::getSeriesInstanceUid()
{
    return findString(DCM_SeriesInstanceUID, m_dataset);
}

std::string DcmParser::getPatientId()
{
    return findString(DCM_PatientID, m_dataset);
}

std::string DcmParser::getPatientName()
{
    return findString(DCM_PatientName, m_dataset);
}

std::string DcmParser::getStudyId()
{
    return findString(DCM_StudyID, m_dataset);
}

std::string DcmParser::getSeriesNumber()
{
    return findString(DCM_SeriesNumber, m_dataset);
}

std::string DcmParser::getInstanceNumber()
{
    return findString(DCM_InstanceNumber, m_dataset);
}

std::string DcmParser::findString(const DcmTagKey& tagKey, DcmItem* dcm)
{
    if (dcm == nullptr)
    {
        PRINT_ERROR("dcm info is null");
        return "";
    }

    OFString result;

    if (m_dataset->findAndGetOFString(tagKey, result).bad())
    {
        PRINT_ERROR("can't find", tagKey);
    }

    return result.c_str();
}
