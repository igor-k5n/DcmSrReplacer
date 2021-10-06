#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <optional>

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmsr/dsrdoc.h>

#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcvrut.h>

#include <dcmtk/ofstd/ofstream.h>
#include <dcmtk/ofstd/ofconapp.h>

class DcmFile
{
public:
    DcmFile() = default;
    virtual ~DcmFile() = default;

    virtual bool loadFile(const std::string& file);

    template<typename T>
    T* search(const DcmTagKey& tag, DcmItem* item)
    {
        if (!m_dcmTemplate || item == nullptr)
        {
            return nullptr;
        }

        T dcmValue(tag);
        auto status = item->search(dcmValue.getTag(), m_stack);

        if (!status.good())
        {
            return nullptr;
        }

        auto dcmValuePoint = dynamic_cast<T*>(m_stack.top());
        if (dcmValuePoint == nullptr)
        {
            return nullptr;
        }

        return dcmValuePoint;
    }

    template<typename T, typename R>
    struct TValue
    {
        using TDcm = T;
        using TReturn = R;
    };

    using TCodeString = TValue<DcmCodeString, std::string>;
    using TShortString = TValue<DcmShortString, std::string>;
    using TUnlimitedText = TValue<DcmUnlimitedText, std::string>;
    using TIntegerString = TValue<DcmIntegerString, int32_t>;
    using TUnsignedShort = TValue<DcmUnsignedShort, uint16_t>;
    using TUnsignedLongVec = TValue<DcmUnsignedLong, std::vector<uint32_t>>;
    using TDecimalStringVec = TValue<DcmDecimalString, std::vector<double>>;
    using TByteArray = TValue<DcmOtherByteOtherWord, std::pair<uint8_t*, uint32_t>>;

    template<typename T>
    std::optional<typename T::TReturn> getValue(const DcmTagKey& tag, DcmItem* item)
    {
        return getValue<typename T::TDcm, typename T::TReturn>(tag, item);
    }

    template<typename T, typename R>
    std::optional<R> getValue(const DcmTagKey& tag, DcmItem* item)
    {
        auto dcmValue = search<T>(tag, item);
        if (dcmValue == nullptr)
        {
            return std::nullopt;
        }

        return convertValue<R>(dcmValue, std::true_type());
    }

    std::unique_ptr<DcmFileFormat> m_dcmTemplate;

    DcmMetaInfo* m_metaInfo = nullptr;
    DcmDataset* m_dataset = nullptr;
    DcmStack m_stack;

private:

    template<typename T>
    std::optional<std::string> convertValue(DcmElement* dcm,
        typename std::enable_if<std::is_same_v<T, std::string>, std::true_type>::type)
    {
        OFString value;
        if (!dcm->getOFString(value, 0).good())
        {
            return std::nullopt;
        }

        return value.c_str();
    }

    template<typename T>
    std::optional<T> convertValue(DcmElement* dcm,
        typename std::enable_if<std::is_same_v<T, int32_t>, std::true_type>::type)
    {
        T value;
        if (!dcm->getSint32(value, 0).good())
        {
            return std::nullopt;
        }

        return value;
    }

    template<typename T>
    std::optional<T> convertValue(DcmElement* dcm,
        typename std::enable_if<std::is_same_v<T, uint32_t>, std::true_type>::type)
    {
        T value;
        if (!dcm->getUint32(value, 0).good())
        {
            return std::nullopt;
        }

        return value;
    }

    template<typename T>
    std::optional<T> convertValue(DcmElement* dcm,
        typename std::enable_if<std::is_same_v<T, uint16_t>, std::true_type>::type)
    {
        T value;
        if (!dcm->getUint16(value, 0).good())
        {
            return std::nullopt;
        }

        return value;
    }

    template<typename T>
    std::optional<T> convertValue(DcmElement* dcm,
        typename std::enable_if<std::is_same_v<T, std::vector<double>>, std::true_type>::type)
    {
        auto dcmString = dynamic_cast<DcmDecimalString*>(dcm);
        if (dcmString != nullptr) {
            T value;
            if (dcmString->getFloat64Vector(value).good())
            {
                return value;
            }
        }

        return std::nullopt;
    }

    template<typename T>
    std::optional<T> convertValue(DcmElement* dcm,
        typename std::enable_if<std::is_same_v<T, std::vector<uint32_t>>, std::true_type>::type)
    {
        Uint32* dcmArray = nullptr;
        if (!dcm->getUint32Array(dcmArray).good())
        {
            return std::nullopt;
        }

        if (dcmArray == nullptr)
        {
            return std::nullopt;
        }

        T value;
        std::copy(dcmArray, dcmArray + dcm->getVM(), std::back_inserter(value));

        return value;
    }

    template<typename T>
    std::optional<T> convertValue(DcmElement* dcm,
        typename std::enable_if<std::is_same_v<T, std::pair<uint8_t*, uint32_t>>, std::true_type>::type)
    {
        Uint8* dcmArray = nullptr;
        uint32_t size = 0;
        if (!dcm->getUint8Array(dcmArray).good())
        {
            return std::nullopt;
        }

        if (dcmArray == nullptr)
        {
            return std::nullopt;
        }

        size = dcm->getLength();

        return std::make_pair(dcmArray, size);
    }
};

