#include "SRMaker.h"

#include <array>
#include <functional>

#include "SrTemplates.h"
#include "DebugLog.h"

SRMaker::SRMaker()
    : m_random(std::random_device{}())
{

}

bool SRMaker::loadTemplate(const std::string& fileTemplate)
{
    if (!base::loadFile(fileTemplate))
    {
        return false;
    }

    auto imagingMeasurements = searchSequenceInSequence("CONTAINER", "126010",
        search<DcmSequenceOfItems>(DCM_ContentSequence, m_dataset));

    if (imagingMeasurements.empty())
    {
        return false;
    }

    m_imagingMeasurements = imagingMeasurements[0];

    m_measurementItem = searchItemInSequence("CONTAINER", "125007", m_imagingMeasurements);
    return m_measurementItem != nullptr;
}

SRMaker::TSequences SRMaker::searchSequenceInSequence(const std::string& valueType, const std::string& codeValue,
    DcmSequenceOfItems* sequence)
{
    TSequences result;
    auto count = sequence->getNumberOfValues();

    for (uint32_t i = 0; i < count; i++)
    {
        auto item = sequence->getItem(i);
        auto relationship = *getValue<base::TCodeString>(DCM_RelationshipType, item);
        auto value = *getValue<base::TCodeString>(DCM_ValueType, item);
        auto conceptNameCode = search<DcmSequenceOfItems>(DCM_ConceptNameCodeSequence, item);

        if ("CONTAINS" == relationship && valueType == value && conceptNameCode != nullptr)
        {
            auto code = *getValue<base::TShortString>(DCM_CodeValue, conceptNameCode->getItem(0));

            if (code == codeValue)
            {
                auto resultSequence = search<DcmSequenceOfItems>(DCM_ContentSequence, item);
                if (resultSequence != nullptr)
                {
                    result.push_back(resultSequence);
                }
            }
        }
    }

    return result;
}

DcmItem* SRMaker::searchItemInSequence(const std::string& valueType, const std::string& codeValue,
    DcmSequenceOfItems* sequence)
{
    auto count = sequence->getNumberOfValues();

    for (uint32_t i = 0; i < count; i++)
    {
        auto item = sequence->getItem(i);

        auto value = *getValue<base::TCodeString>(DCM_ValueType, item);
        auto conceptNameCode = search<DcmSequenceOfItems>(DCM_ConceptNameCodeSequence, item);

        bool sameCodeValue = false;
        if (!codeValue.empty())
        {
            if (conceptNameCode)
            {
                auto code = *getValue<base::TShortString>(DCM_CodeValue, conceptNameCode->getItem(0));
                sameCodeValue = code == codeValue;
            }
        }
        else
        {
            sameCodeValue = true;
        }

        if (valueType == value && sameCodeValue)
        {
            return item;
        }
    }

    return nullptr;
}

void SRMaker::removeItemInSequence(const std::string& valueType, const std::string& codeValue, DcmSequenceOfItems *sequence)
{
    auto count = sequence->getNumberOfValues();

    for (uint32_t i = 0; i < count; i++)
    {
        auto item = sequence->getItem(i);

        auto value = *getValue<base::TCodeString>(DCM_ValueType, item);
        auto conceptNameCode = search<DcmSequenceOfItems>(DCM_ConceptNameCodeSequence, item);

        bool sameCodeValue = false;

        if (conceptNameCode)
        {
            auto code = *getValue<base::TShortString>(DCM_CodeValue, conceptNameCode->getItem(0));
            sameCodeValue = code == codeValue;
        }

        if (valueType == value && sameCodeValue)
        {
            sequence->remove(item);
            break;
        }
    }
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

bool SRMaker::insertMeasurements(const TMeasurements &measurements)
{
    if (m_measurementItem == nullptr)
    {
        return false;
    }

    m_measurementItem->remove(DCM_ContentSequence);

    auto kmtSequnce = new DcmSequenceOfItems(DCM_ContentSequence);
    auto kmtItem = new DcmItem();
    auto kmtRelType = new DcmCodeString(DCM_RelationshipType);
    auto kmtValueType = new DcmCodeString(DCM_ValueType);

    kmtRelType->putString("CONTAINS");
    kmtValueType->putString("CONTAINER");

    kmtItem->insert(kmtRelType);
    kmtItem->insert(kmtValueType);

    auto kmtNameSequnce = new DcmSequenceOfItems(DCM_ConceptNameCodeSequence);
    auto kmtNameItem = new DcmItem();
    auto kmtCodeValue = new DcmShortString(DCM_CodeValue);
    auto kmtSchemeDesignator = new DcmShortString(DCM_CodingSchemeDesignator);
    auto kmtCodeMeaning = new DcmLongString(DCM_CodeMeaning);

    kmtCodeValue->putString("LNGMS");
    kmtSchemeDesignator->putString("KMT");
    kmtCodeMeaning->putString("Lungs measurement");

    kmtNameItem->insert(kmtCodeValue);
    kmtNameItem->insert(kmtSchemeDesignator);
    kmtNameItem->insert(kmtCodeMeaning);

    kmtNameSequnce->insert(kmtNameItem);

    kmtItem->insert(kmtNameSequnce);

    auto measurementSequnce = new DcmSequenceOfItems(DCM_ContentSequence);

    auto measurementTemplateItem = SrTemplates::createMeasurement();

    for (auto& measurement : measurements)
    {
        auto item = dynamic_cast<DcmItem*>(measurementTemplateItem->clone());
        if (!replace<DcmFloatingPointDouble>(measurement.Value, DCM_FloatingPointValue, item))
        {
            return false;
        }

        if (!replace<DcmUnlimitedText>(measurement.Description, DCM_TextValue, item))
        {
            return false;
        }

        measurementSequnce->insert(item);
    }

    kmtItem->insert(measurementSequnce);

    kmtSequnce->insert(kmtItem);
    return m_measurementItem->insert(kmtSequnce).good();
}

bool SRMaker::replacePolyline(const std::unordered_map<std::string, std::vector<float>>& polylines)
{
    if (m_measurementItem == nullptr)
    {
        return false;
    }

    auto item = m_measurementItem;
    for (const auto& polyline : polylines)
    {
        auto group = search<DcmSequenceOfItems>(DCM_ContentSequence, item);
        auto coronaryRoute = searchSequenceInSequence("NUM", "G-D17C", group);
        if (!coronaryRoute.empty())
        {
            auto uid = searchItemInSequence("UIDREF", "112040", group);
            if (uid != nullptr)
            {
                replace<DcmUniqueIdentifier>(std::string("2.25.") + genString(38), DCM_UID, uid);
            }
            
            auto scoord3d = searchItemInSequence("SCOORD3D", "", coronaryRoute[0]);
            if (scoord3d == nullptr)
            {
                return false;
            }

            auto coronaryIdentifier = searchItemInSequence("TEXT", "125010", coronaryRoute[0]);
            if (coronaryIdentifier == nullptr)
            {
                return false;
            }

            if (!replace<DcmFloatingPointSingle>(polyline.second, DCM_GraphicData, scoord3d))
            {
                return false;
            }

            if (!replace<DcmUnlimitedText>(polyline.first, DCM_TextValue, coronaryIdentifier))
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        if (item != m_measurementItem)
        {
            m_imagingMeasurements->insert(item);
        }

        item = dynamic_cast<DcmItem*>(m_measurementItem->clone());
    }

    delete item;

    return true;
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

bool SRMaker::replaceSeriesDescription(const std::string& description)
{
    return replace<DcmLongString>(description, DCM_SeriesDescription, m_dataset);
}

bool SRMaker::replaceInstanceNumber(const std::string& number)
{
    return replace<DcmIntegerString>(number, DCM_InstanceNumber, m_dataset);
}

template<typename T, typename R>
bool SRMaker::replace(const R& value, const DcmTagKey& tag, DcmItem* item)
{
    auto dcmValue = base::search<T>(tag, item);
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

bool SRMaker::replaceValue(DcmElement* el, const double& value)
{
    return el->putFloat64(value).good();
}

std::string SRMaker::genString(uint32_t length)
{
    std::vector<char> charSet{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    std::uniform_int_distribution<> dist(0, charSet.size() - 1);
    auto randChar = [&charSet, &dist, this]() 
    {
        return charSet[dist(m_random)];
    };

    std::string randomString(length, 0);
    std::generate_n(randomString.begin(), length, randChar);
    return randomString;
}