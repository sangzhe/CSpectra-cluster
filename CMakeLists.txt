cmake_minimum_required(VERSION 3.6)
project(CSpectra_cluster)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -stdlib=libc++")

set(SOURCE_FILES
        main.cpp
        util/ParserUtilities.cpp
        util/ParserUtilities.h
        Cluster/ICluster.h
        Spectrum/ISpectrum.h
        Consensus/IConsensusSpectrumBuilder.h
        Property/Properties.cpp
        Property/Properties.h
        Spectrum/IPeak.h
        Spectrum/ISpectrumQuality.h
        quality/IQualityScorer.h
        Spectrum/Spectrum.cpp
        Spectrum/Spectrum.h
        util/Defaults.cpp
        util/Defaults.h
        Spectrum/KnownProperties.h
        Spectrum/KnownProperties.cpp
        util/IOUtilities.cpp
        util/IOUtilities.h
        Spectrum/Peak.cpp
        Spectrum/Peak.h
        util/ComparisonMatch.cpp
        util/ComparisonMatch.h
        Cluster/SpectralClusrer.cpp
        Cluster/SpectralClusrer.h
        Cluster/ISpectrumHolder.h
        Consensus/ConsensusSpectrumFactory.cpp
        Consensus/ConsensusSpectrumFactory.h
        Consensus/ConsensusSpectrum.cpp
        Consensus/ConsensusSpectrum.h
        Consensus/ConsensusSpectrumBuilderFactory.h
        quality/SignalToNoiseChecker.cpp
        quality/SignalToNoiseChecker.h
        util/MZIntensityUtilities.cpp
        util/MZIntensityUtilities.h
        util/Function/spectrum/RemoveImpossiblyHighPeaksFunction.cpp
        util/Function/spectrum/RemoveImpossiblyHighPeaksFunction.h
        Spectrum/Mass.cpp
        Spectrum/Mass.h
        util/Function/spectrum/RemovePrecursorPeaksFunction.cpp
        util/Function/spectrum/RemovePrecursorPeaksFunction.h
        util/Function/peak/HighestNPeakFunction.cpp
        util/Function/peak/HighestNPeakFunction.h
        util/Function/peak/BinnedHighestNPeakFunction.cpp
        util/Function/peak/BinnedHighestNPeakFunction.h
        engine/IClusteringEngine.h
        similarity/ISimilarityChecker.h
        engine/IIncrementalClusteringEngine.h
        Cluster/GreedySpectralCluster.cpp
        Cluster/GreedySpectralCluster.h
        Consensus/GreedyConsensusSpectrum.cpp
        Consensus/GreedyConsensusSpectrum.h
        Consensus/GreedyConsensusSpectrumFactory.cpp
        Consensus/GreedyConsensusSpectrumFactory.h
        )

set(Boost_USE_STATIC_LIBS ON)
set(Boost_LIBRARIES "/usr/local/include/boost")

#find_package(Boost 1.61.0 COMPONENTS  algorithm  regex REQUIRED)

add_executable(CSpectra_cluster ${SOURCE_FILES})

target_link_libraries(CSpectra_cluster ${Boost_LIBRARIES})



