#include "SRMaker.h"

#include <array>

#include "DebugLog.h"

bool SRMaker::loadTemplate(const std::string& fileTemplate)
{
    auto dcmTemplate = std::make_unique<DcmFileFormat>();
    OFCondition status = dcmTemplate->loadFile(fileTemplate.c_str(), EXS_Unknown);
    if (!status.good())
    {
        PRINT_ERROR("load file", fileTemplate);
        return false;
    }

    m_metaInfo = dcmTemplate->getMetaInfo();
    m_dataset = dcmTemplate->getDataset();

    if (m_metaInfo == nullptr || m_dataset == nullptr)
    {
        PRINT_ERROR("meta info or dataset is null");
        return false;
    }

    m_dcmTemplate = std::move(dcmTemplate);

    return true;
}

bool SRMaker::saveFile(const std::string& file)
{
    if (!m_dcmTemplate)
    {
        PRINT_ERROR("template file not loading");
        return false;
    }

    return m_dcmTemplate->saveFile(file.c_str()).good();
}

bool SRMaker::replacePolyline(const std::vector<float>& polyline)
{
    return replace<DcmFloatingPointSingle>(polyline, DCM_GraphicData, m_dataset);
}

bool SRMaker::replaceStudyInstanceUid(const std::string& uid)
{
    return replace<DcmUniqueIdentifier>(uid, DCM_StudyInstanceUID, m_dataset);
}

bool SRMaker::replaceSeriesInstanceUid(const std::string& uid)
{
    return replace<DcmUniqueIdentifier>(uid, DCM_SeriesInstanceUID, m_dataset);
}

bool SRMaker::replacePatientId(const std::string& id)
{
    return replace<DcmLongString>(id, DCM_PatientID, m_dataset);
}

bool SRMaker::replacePatientName(const std::string& name)
{
    return replace<DcmPersonName>(name, DCM_PatientName, m_dataset);
}

bool SRMaker::replaceStudyId(const std::string& id)
{
    return replace<DcmShortString>(id, DCM_StudyID, m_dataset);
}

bool SRMaker::replaceSeriesNumber(const std::string& number)
{
    return replace<DcmIntegerString>(number, DCM_SeriesNumber, m_dataset);
}

bool SRMaker::replaceInstanceNumber(const std::string& number)
{
    return replace<DcmIntegerString>(number, DCM_InstanceNumber, m_dataset);
}

template<typename T>
T* SRMaker::search(const DcmTagKey& tag, DcmItem* item)
{
    if (!m_dcmTemplate || item == nullptr)
    {
        PRINT_ERROR("template file not loading or item is null");
        return nullptr;
    }

    T dcmValue(tag);
    auto status = item->search(dcmValue.getTag(), m_stack);

    if (!status.good())
    {
        PRINT_ERROR("can't find", tag);
        return nullptr;
    }

    auto dcmValuePoint = dynamic_cast<T*>(m_stack.top());
    if (dcmValuePoint == nullptr)
    {
        PRINT_VARIABLE_ERROR(dcmValuePoint, "is null");
        return nullptr;
    }

    dcmValuePoint->clear();

    return dcmValuePoint;
}

template<typename T, typename R>
bool SRMaker::replace(const R& value, const DcmTagKey& tag, DcmItem* item)
{
    auto dcmValue = search<T>(tag, item);
    if (dcmValue == nullptr)
    {
        PRINT_VARIABLE_ERROR(value, "is null");
        return false;
    }

    return replaceValue(dcmValue, value);
}

bool SRMaker::replaceValue(DcmElement* el, const std::vector<float>& value)
{
    return el->putFloat32Array(value.data(), value.size()).good();
}

bool SRMaker::replaceValue(DcmElement* el, const std::string& value)
{
    return el->putString(value.c_str()).good();
}