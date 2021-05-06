/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#ifndef SRSRAN_SSB_H
#define SRSRAN_SSB_H

#include "srsran/config.h"
#include "srsran/phy/common/phy_common_nr.h"
#include "srsran/phy/dft/dft.h"
#include "srsran/phy/phch/pbch_nr.h"
#include <inttypes.h>

/**
 * @brief Default SSB maximum sampling rate
 */
#define SRSRAN_SSB_DEFAULT_MAX_SRATE_HZ 61.44e6

/**
 * @brief Default SSB minimum subcarrier spacing
 */
#define SRSRAN_SSB_DEFAULT_MIN_SCS srsran_subcarrier_spacing_15kHz

/**
 * @brief Default beta value, used in case they are set to zero
 */
#define SRSRAN_SSB_DEFAULT_BETA 1.0f

/**
 * @brief Describes SSB object initializatoion arguments
 */
typedef struct SRSRAN_API {
  double max_srate_hz; ///< Maximum sampling rate in Hz (common for gNb and UE), set to zero to use default
  srsran_subcarrier_spacing_t min_scs;          ///< Minimum subcarrier spacing
  bool                        enable_correlate; ///< Enables PSS/SSS correlation and peak search (UE cell search)
  bool                        enable_encode;    ///< Enables PBCH Encoder (intended for gNb)
  bool                        enable_decode;    ///< Enables PBCH Decoder (intented for UE)
  bool                        enable_measure;   ///< Enables PSS/SSS CSI measurements
} srsran_ssb_args_t;

/**
 * @brief Describes SSB configuration arguments
 */
typedef struct SRSRAN_API {
  double                      srate_hz;       ///< Current sampling rate in Hz
  double                      freq_offset_hz; ///< SSB base-band frequency offset
  srsran_subcarrier_spacing_t scs;            ///< SSB configured Subcarrier spacing
  float                       beta_pss;       ////< PSS power allocation
  float                       beta_sss;       ////< SSS power allocation
  float                       beta_pbch;      ////< PBCH power allocation
  float                       beta_pbch_dmrs; ////< PBCH DMRS power allocation
} srsran_ssb_cfg_t;

/**
 * @brief Describes SSB object
 */
typedef struct SRSRAN_API {
  srsran_ssb_args_t args; ///< Stores initialization arguments
  srsran_ssb_cfg_t  cfg;  ///< Stores last configuration

  /// Sampling rate dependent parameters
  float    scs_hz;        ///< Subcarrier spacing in Hz
  uint32_t max_symbol_sz; ///< Maximum symbol size given the minimum supported SCS and sampling rate
  uint32_t symbol_sz;     ///< Current SSB symbol size (for the given base-band sampling rate)
  int32_t  offset;        ///< Current SSB integer offset (multiple of SCS)
  uint32_t cp0_sz;        ///< First symbol cyclic prefix size
  uint32_t cp_sz;         ///< Other symbol cyclic prefix size

  /// Internal Objects
  // srsran_pbch_nr_t pbch; ///< PBCH object for encoding/decoding
  // srsran_dmrs_pbch_nr_t dmrs; ///< PBCH DMRS object for channel estimation
  srsran_dft_plan_t ifft; ///< IFFT object for modulating the SSB
  srsran_dft_plan_t fft;  ///< FFT object for demodulate the SSB.

  /// Frequency/Time domain temporal data
  cf_t* tmp_freq;
  cf_t* tmp_time;

  /// Time domain sequences
  //  cf_t* pss[SRSRAN_NOF_NID_2_NR]; ///< PSS signal for each possible N_id_2
  //  cf_t* sss[SRSRAN_NOF_NID_1_NR]; ///< SSS signal for each possible N_id_1

} srsran_ssb_t;

/**
 * @brief Initialises configures NR SSB with the given arguments
 * @param q SSB object
 * @param args NR PSS initialization arguments
 * @return SRSLTE_SUCCESS if the parameters are valid, SRSLTE_ERROR code otherwise
 */
SRSRAN_API int srsran_ssb_init(srsran_ssb_t* q, const srsran_ssb_args_t* args);

/**
 * @brief Frees NR SSB object
 * @param q SSB object
 */
SRSRAN_API void srsran_ssb_free(srsran_ssb_t* q);

/**
 * @brief Sets SSB configuration with the current SSB configuration
 * @param q SSB object
 * @param cfg Current SSB configuration
 * @return SRSLTE_SUCCESS if the parameters are valid, SRSLTE_ERROR code otherwise
 */
SRSRAN_API int srsran_ssb_set_cfg(srsran_ssb_t* q, const srsran_ssb_cfg_t* cfg);
/**
 * @brief Decodes PBCH in the given time domain signal
 * @param q SSB object
 * @return SRSLTE_SUCCESS if the parameters are valid, SRSLTE_ERROR code otherwise
 */
SRSRAN_API int srsran_ssb_decode_pbch(srsran_ssb_t* q, const cf_t* in, srsran_pbch_msg_nr_t* msg);

/**
 * @brief Adds SSB to a given signal in time domain
 * @param q SSB object
 * @param N_id Physical Cell Identifier
 * @param msg NR PBCH message to transmit
 * @return SRSLTE_SUCCESS if the parameters are valid, SRSLTE_ERROR code otherwise
 */
SRSRAN_API int
srsran_ssb_add(srsran_ssb_t* q, uint32_t N_id, const srsran_pbch_msg_nr_t* msg, const cf_t* in, cf_t* out);

/**
 * @brief Perform Channel State Information (CSI) measurement from the SSB
 * @param q NR PSS object
 * @param N_id Physical Cell Identifier
 * @param in Base-band signal
 * @param meas SSB-based CSI measurement
 * @return SRSLTE_SUCCESS if the parameters are valid, SRSLTE_ERROR code otherwise
 */
SRSRAN_API int
srsran_ssb_csi_measure(srsran_ssb_t* q, uint32_t N_id, const cf_t* in, srsran_csi_trs_measurements_t* meas);

#endif // SRSRAN_SSB_H
