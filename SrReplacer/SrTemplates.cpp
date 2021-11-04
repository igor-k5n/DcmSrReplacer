#include "SrTemplates.h"

DcmItem * SrTemplates::createMeasurement()
{
    auto item = new DcmItem();
    auto relType = new DcmCodeString(DCM_RelationshipType);
    auto valueType = new DcmCodeString(DCM_ValueType);

    relType->putString("CONTAINS");
    valueType->putString("NUM");

    item->insert(relType);
    item->insert(valueType);

    auto quantitySequnce = new DcmSequenceOfItems(DCM_ConceptNameCodeSequence);
    auto quantityItem = new DcmItem();
    auto quantityCodeValue = new DcmShortString(DCM_CodeValue);
    auto quantitySchemeDesignator = new DcmShortString(DCM_CodingSchemeDesignator);
    auto quantityCodeMeaning = new DcmLongString(DCM_CodeMeaning);

    quantityCodeValue->putString("110855");
    quantitySchemeDesignator->putString("DCM");
    quantityCodeMeaning->putString("Fractional Occupancy Segmentation");

    quantityItem->insert(quantityCodeValue);
    quantityItem->insert(quantitySchemeDesignator);
    quantityItem->insert(quantityCodeMeaning);

    quantitySequnce->insert(quantityItem);

    item->insert(quantitySequnce);

    auto unitsSequnce = new DcmSequenceOfItems(DCM_MeasuredValueSequence);
    auto unitsItem = new DcmItem();
    auto unitsValue = new DcmShortString(DCM_CodeValue);
    auto unitsSchemeDesignator = new DcmShortString(DCM_CodingSchemeDesignator);
    auto unitsCodeMeaning = new DcmLongString(DCM_CodeMeaning);

    unitsValue->putString("%");
    unitsSchemeDesignator->putString("UCUM");
    unitsCodeMeaning->putString("percent");

    unitsItem->insert(unitsValue);
    unitsItem->insert(unitsSchemeDesignator);
    unitsItem->insert(unitsCodeMeaning);

    unitsSequnce->insert(unitsItem);

    item->insert(unitsSequnce);

    auto valueSequnce = new DcmSequenceOfItems(DCM_ContentSequence);
    auto valueItem = new DcmItem();

    auto valueRelType = new DcmCodeString(DCM_RelationshipType);
    auto valueValueType = new DcmCodeString(DCM_ValueType);
    auto value = new DcmFloatingPointDouble(DCM_FloatingPointValue);

    valueRelType->putString("INFERREDED FROM");
    valueValueType->putString("NUM");

    valueItem->insert(valueRelType);
    valueItem->insert(valueValueType);
    valueItem->insert(value);

    valueSequnce->insert(valueItem);

    auto valueDescItem = new DcmItem();
    auto valueDescRelType = new DcmCodeString(DCM_RelationshipType);
    auto valueDescValueType = new DcmCodeString(DCM_ValueType);
    auto valueDesc = new DcmUnlimitedText(DCM_TextValue);

    valueDescRelType->putString("HAS CONCEPT MOD");
    valueDescValueType->putString("TEXT");

    valueDescItem->insert(valueDescRelType);
    valueDescItem->insert(valueDescValueType);
    valueDescItem->insert(valueDesc);

    valueSequnce->insert(valueDescItem);

    item->insert(valueSequnce);

    return item;
}

