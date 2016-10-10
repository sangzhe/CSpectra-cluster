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
        pool/PointerPool.h Cluster/SpectrumHolderListener.h IPointer.h engine/IncrementalClusteringEngineFactory.h similarity/IntensityRankCorrelation.cpp similarity/IntensityRankCorrelation.h similarity/IPeakMatches.h similarity/PeakMatches.cpp similarity/PeakMatches.h similarity/PeakMatchesUtilities.cpp similarity/PeakMatchesUtilities.h similarity/HypergeometricScore.cpp similarity/HypergeometricScore.h similarity/FisherExactTest.cpp similarity/FisherExactTest.h similarity/KendallCorrelation.cpp similarity/KendallCorrelation.h similarity/CombinedFisherIntensityTest.cpp similarity/CombinedFisherIntensityTest.h engine/IncrementalClusteringEngine.cpp engine/IncrementalClusteringEngine.h engine/GreedyIncrementalClusteringEngine.cpp engine/GreedyIncrementalClusteringEngine.h cdf/CumulativeDistributionFunction.cpp cdf/CumulativeDistributionFunction.h cdf/CumulativeDistributionFunctionFactory.cpp cdf/CumulativeDistributionFunctionFactory.h similarity/FrankEtAlDotProduct.cpp similarity/FrankEtAlDotProduct.h util/Function/peak/FractionTICPeakFunction.cpp util/Function/peak/FractionTICPeakFunction.h util/Function/spectrum/SpectrumFunctoin.h util/Function/peak/PeakFunction.h util/Predicate/cluster_comparison/ClusterPredicate.h util/Predicate/cluster_comparison/IsKnownComparisonsPredicate.cpp util/Predicate/cluster_comparison/IsKnownComparisonsPredicate.h util/Predicate/cluster_comparison/ClusterShareMajorPeakPredicate.cpp util/Predicate/cluster_comparison/ClusterShareMajorPeakPredicate.h util/Predicate/spectrum_comparison/SpectrumPredicate.h util/Predicate/spectrum_comparison/ShareMajorPeaksPredicate.cpp util/Predicate/spectrum_comparison/ShareMajorPeaksPredicate.h IPointer.cpp pool/PointerPool.cpp pool/PoolFactory.h pool/PoolFactory.cpp)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_LIBRARIES "/usr/local/include/boost")
#find_package(Boost 1.61.0 COMPONENTS  algorithm  regex REQUIRED)

add_executable(CSpectra_cluster ${SOURCE_FILES})

target_link_libraries(CSpectra_cluster ${Boost_LIBRARIES})



