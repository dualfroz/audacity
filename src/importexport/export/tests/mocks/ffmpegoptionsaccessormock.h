/*
* Audacity: A Digital Audio Editor
*/
#pragma once

#include <gmock/gmock.h>

#include "importexport/export/iffmpegoptionsaccessor.h"

namespace au::importexport {
class FFmpegOptionsAccessorMock : public IFFmpegOptionsAccessor
{
public:
    MOCK_METHOD(std::vector<std::string>, formatList, (), (const, override));
    MOCK_METHOD(void, fetchCompatibleFormatList, (const std::string& format, const std::string& codec), (override));
    MOCK_METHOD(void, fetchAllFormats, (), (override));

    MOCK_METHOD(std::vector<std::string>, codecList, (), (const, override));
    MOCK_METHOD(void, fetchCompatibleCodecList, (const std::string& format, const std::string& codec), (override));
    MOCK_METHOD(void, fetchAllCodecs, (), (override));

    MOCK_METHOD(std::vector<std::string>, profileList, (), (const, override));
    MOCK_METHOD(std::vector<std::string>, predictionOrderMethodList, (), (const, override));

    MOCK_METHOD(std::string, ffmpegVersion, (), (const, override));
    MOCK_METHOD(std::string, ffmpegLibraryPath, (), (const, override));
    MOCK_METHOD(bool, setFFmpegLibraryPath, (const muse::io::path_t& path), (override));
};
}
