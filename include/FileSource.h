// NGSoftFM - Software decoder for FM broadcast radio with RTL-SDR
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

#ifndef SOFTFM_FILESOURCE_H
#define SOFTFM_FILESOURCE_H

#include <cstdint>
#include <string>
#include <thread>
#include <vector>

#include <sndfile.h>

#include "Source.h"

class FileSource : public Source {
public:
  static constexpr int default_block_length = 2048;
  static constexpr std::uint32_t default_sample_rate = 384000;
  static constexpr std::int32_t default_frequency = 825000000;

  /** Open file. */
  FileSource(int dev_index);

  /** Close RTL-SDR device. */
  virtual ~FileSource();

  /** Configure device and prepare for streaming. */
  virtual bool configure(std::string configuration);

  /** Return current sample frequency in Hz. */
  virtual std::uint32_t get_sample_rate();

  /** Return device current center frequency in Hz. */
  virtual std::uint32_t get_frequency();

  /** Return if device is using Low-IF. */
  virtual bool is_low_if();

  /** Print current parameters specific to device type */
  virtual void print_specific_parms();

  virtual bool start(DataBuffer<IQSample> *samples,
                     std::atomic_bool *stop_flag);
  virtual bool stop();

  /** Return true if the device is OK, return false if there is an error. */
  virtual operator bool() const { return m_error.empty(); }

  /** Return a list of supported devices. */
  static void get_device_names(std::vector<std::string> &devices);

private:
  /**
   * Configure IQ file for streaming.
   *
   * fname        :: file to read.
   * sample_rate  :: desired sample rate in Hz.
   * frequency    :: desired center frequency in Hz.
   * zero_offset  :: true if sample contain zero offset.
   * block_length :: preferred number of samples per block.
   *
   * Return true.
   */
  bool configure(std::string fname,
                 std::uint32_t sample_rate = default_sample_rate,
                 std::uint32_t frequency = default_frequency,
                 bool zero_offset = false,
                 int block_length = default_block_length);

  /**
   * Fetch a bunch of samples from the device.
   *
   * This function must be called regularly to maintain streaming.
   * Return true for success, false if an error occurred.
   */
  static bool get_samples(IQSampleVector *samples);

  static void run();

  std::uint32_t m_sample_rate;
  std::uint32_t m_frequency;
  bool m_zero_offset;
  int m_block_length;

  SNDFILE *m_sfp;
  SF_INFO m_sfinfo;

  double m_sample_rate_per_us;

  std::thread *m_thread;
  static FileSource *m_this;
};

#endif
