cmake_minimum_required(VERSION 3.6)
project(CSpectra_cluster)



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        main.cpp
        ParserUtilities.cpp
        ParserUtilities.h
        ICluster.h
        ISpectrum.h
        IConsensusSpectrumBuilder.h
        Properties.cpp
        Properties.h IPeak.h
        ISpectrumQuality.h
        IQualityScorer.h
        Spectrum.cpp
        Spectrum.h
        Defaults.cpp
        Defaults.h
        KnownProperties.cpp
        KnownProperties.h
        Functions.cpp
        Functions.h
        Peak.cpp
        Peak.h
        ComparisonMatch.cpp
        ComparisonMatch.h
        SpectralClusrer.cpp
        SpectralClusrer.h
        ISpectrumHolder.h
        ConsensusSpectrum.cpp
        ConsensusSpectrum.h
        ConsensusSpectrumBuilderFactory.h
        SignalToNoiseChecker.cpp
        SignalToNoiseChecker.h
        ConsensusSpectrumFactory.cpp
        ConsensusSpectrumFactory.h)
add_executable(CSpectra_cluster ${SOURCE_FILES})