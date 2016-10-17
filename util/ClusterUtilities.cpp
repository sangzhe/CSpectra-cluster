//
// Created by SangZhe on 2016/10/12.
//

#include "ClusterUtilities.h"


ICluster* ClusterUtilities::asCluster(ISpectrum *spectrum) {
        ICluster *ret = new SpectralCluster("", Defaults::getDefaultConsensusSpectrumBuilder());
        ret->addSpectra(spectrum);
        return ret;
}