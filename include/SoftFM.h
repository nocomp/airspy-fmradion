// airspy-fmradion
// Software decoder for FM broadcast radio with Airspy
//
// Copyright (C) 2015 Edouard Griffiths, F4EXB
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef SOFTFM_H
#define SOFTFM_H

#include <complex>
#include <vector>

typedef std::complex<float> IQSample;
typedef std::vector<IQSample> IQSampleVector;

typedef double Sample;
typedef std::vector<Sample> SampleVector;

typedef std::vector<IQSample::value_type> IQSampleCoeff;
typedef std::vector<SampleVector::value_type> SampleCoeff;

enum class FilterType { Default, Medium, Narrow };
enum class DevType { Airspy, AirspyHF, RTLSDR };
enum class ModType { FM, AM, DSB, USB, LSB, CW, NBFM };
enum class OutputMode { RAW_INT16, RAW_FLOAT32, WAV, ALSA };

/** Compute mean and RMS over a sample vector. */
inline void samples_mean_rms(const SampleVector &samples, double &mean,
                             double &rms) {
  Sample vsum = 0;
  Sample vsumsq = 0;

  unsigned int n = samples.size();
  for (unsigned int i = 0; i < n; i++) {
    Sample v = samples[i];
    vsum += v;
    vsumsq += v * v;
  }

  mean = vsum / n;
  rms = sqrt(vsumsq / n);
}

#endif
