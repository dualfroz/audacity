/*
 * Audacity: A Digital Audio Editor
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "global/settings.h"

#include "importexport/export/internal/exportconfiguration.h"
#include "importexport/export/view/customffmpegpreferencesmodel.h"

#include "importexport/export/tests/mocks/ffmpegoptionsaccessormock.h"

using ::testing::NiceMock;
using ::testing::Return;

namespace au::importexport {
class CustomFFmpegPreferencesModelTests : public ::testing::Test
{
public:
    void SetUp() override
    {
        //! NOTE: the export dialog keeps a settings transaction open while the custom FFmpeg dialog is shown
        muse::settings()->beginTransaction(false);

        m_configuration = std::make_shared<ExportConfiguration>();
        m_optionsAccessor = std::make_shared<NiceMock<FFmpegOptionsAccessorMock> >();
        muse::modularity::globalIoc()->registerExport<IExportConfiguration>("utests", m_configuration);
        muse::modularity::globalIoc()->registerExport<IFFmpegOptionsAccessor>("utests", m_optionsAccessor);

        ON_CALL(*m_optionsAccessor, profileList())
        .WillByDefault(Return(std::vector<std::string> { "aac_low", "aac_main" }));

        m_configuration->setFFmpegFormat("matroska");
        m_configuration->setFFmpegCodec("flac");
        m_configuration->setFFmpegLanguage("eng");
        m_configuration->setFFmpegTag("fLaC");
        m_configuration->setFFmpegQuality(5);
        m_configuration->setFFmpegCutoff(16000);
        m_configuration->setFFmpegBitrate(128000);
        m_configuration->setFFmpegSampleRate(44100);
        m_configuration->setFFmpegProfile("aac_low");
        m_configuration->setFFmpegBitReservoir(true);
        m_configuration->setFFmpegVbl(true);
        m_configuration->setFFmpegCompression(5);
        m_configuration->setFFmpegLpcCoeffPrecision(10);
        m_configuration->setFFmpegFrameSize(4096);
        m_configuration->setFFmpegPredictionOrderMethod(4);
        m_configuration->setFFmpegMinPredictionOrder(1);
        m_configuration->setFFmpegMaxPredictionOrder(8);
        m_configuration->setFFmpegMinPartitionOrder(0);
        m_configuration->setFFmpegMaxPartitionOrder(6);
        m_configuration->setFFmpegUseLpc(true);
        m_configuration->setFFmpegMuxRate(10080000);
        m_configuration->setFFmpegPacketSize(2048);

        m_model = std::make_unique<CustomFFmpegPreferencesModel>();
        m_model->init();
    }

    void TearDown() override
    {
        m_model.reset();
        muse::modularity::globalIoc()->unregister<IFFmpegOptionsAccessor>("utests");
        muse::modularity::globalIoc()->unregister<IExportConfiguration>("utests");
        muse::settings()->rollbackTransaction(false);
    }

    std::unique_ptr<CustomFFmpegPreferencesModel> m_model;
    std::shared_ptr<ExportConfiguration> m_configuration;
    std::shared_ptr<NiceMock<FFmpegOptionsAccessorMock> > m_optionsAccessor;
};

TEST_F(CustomFFmpegPreferencesModelTests, Cancel_RestoresOptionsFromWhenTheDialogWasOpened)
{
    m_model->setFFmpegFormat("ogg");
    m_model->setFFmpegCodec("opus");
    m_model->setLanguage("pol");
    m_model->setTag("Opus");
    m_model->setQuality(9);
    m_model->setCutoff(20000);
    m_model->setBitrate(320000);
    m_model->setSampleRate(48000);
    m_model->setProfile("aac_main");
    m_model->setBitReservoir(false);
    m_model->setVbl(false);
    m_model->setCompression(8);
    m_model->setLpc(15);
    m_model->setFrameSize(1152);
    m_model->setPdOMethod(2);
    m_model->setMinPdO(2);
    m_model->setMaxPdO(12);
    m_model->setMinPtO(1);
    m_model->setMaxPtO(8);
    m_model->setUseLpc(false);
    m_model->setMuxRate(0);
    m_model->setPacketSize(4096);

    m_model->cancel();

    EXPECT_EQ(m_configuration->ffmpegFormat(), "matroska");
    EXPECT_EQ(m_configuration->ffmpegCodec(), "flac");
    EXPECT_EQ(m_configuration->ffmpegLanguage(), "eng");
    EXPECT_EQ(m_configuration->ffmpegTag(), "fLaC");
    EXPECT_EQ(m_configuration->ffmpegQuality(), 5);
    EXPECT_EQ(m_configuration->ffmpegCutoff(), 16000);
    EXPECT_EQ(m_configuration->ffmpegBitrate(), 128000);
    EXPECT_EQ(m_configuration->ffmpegSampleRate(), 44100);
    EXPECT_EQ(m_configuration->ffmpegProfile(), "aac_low");
    EXPECT_TRUE(m_configuration->ffmpegBitReservoir());
    EXPECT_TRUE(m_configuration->ffmpegVbl());
    EXPECT_EQ(m_configuration->ffmpegCompression(), 5);
    EXPECT_EQ(m_configuration->ffmpegLpcCoeffPrecision(), 10);
    EXPECT_EQ(m_configuration->ffmpegFrameSize(), 4096);
    EXPECT_EQ(m_configuration->ffmpegPredictionOrderMethod(), 4);
    EXPECT_EQ(m_configuration->ffmpegMinPredictionOrder(), 1);
    EXPECT_EQ(m_configuration->ffmpegMaxPredictionOrder(), 8);
    EXPECT_EQ(m_configuration->ffmpegMinPartitionOrder(), 0);
    EXPECT_EQ(m_configuration->ffmpegMaxPartitionOrder(), 6);
    EXPECT_TRUE(m_configuration->ffmpegUseLpc());
    EXPECT_EQ(m_configuration->ffmpegMuxRate(), 10080000);
    EXPECT_EQ(m_configuration->ffmpegPacketSize(), 2048);
}

TEST_F(CustomFFmpegPreferencesModelTests, Cancel_ShowsAllFormatsAndCodecsAgain)
{
    m_model->setFFmpegCodec("opus");

    EXPECT_CALL(*m_optionsAccessor, fetchAllFormats()).Times(1);
    EXPECT_CALL(*m_optionsAccessor, fetchAllCodecs()).Times(1);

    m_model->cancel();
}
}
