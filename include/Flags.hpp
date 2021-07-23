/**
 * @file Flags.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Store global runtime flags.
 * @version 0.1.0
 * @date 2021-07-20
 *
 *
 */
#pragma once

namespace interflop {

// Storage class for the runtime flags
// We could use a struct to avoid setter/getters, but this is both easier to
// read and to use in external code (i.e defining specific flags for testing)
class RuntimeFlags {
public:
  // FIXME : should load the config from a file or from the environnement
  RuntimeFlags() {}
  
  void setExitOnError(bool const Value) { ExitOnError = Value; }
  bool getExitOnError() const { return ExitOnError; }

  void setPrintStatsOnExit(bool const value) { PrintStatsOnExit = value; }
  bool getPrintStatsOnExit() const { return PrintStatsOnExit; }

  void setWarningEnabled(bool const value) { WarningEnabled = value; }
  bool getWarningEnabled() const { return WarningEnabled; }

  void setWarningLimit(size_t const value) { WarningLimit = value; }
  size_t getWarningLimit() const { return WarningLimit; }

  void setVerbose(bool const value) { Verbose = value; }
  bool getVerbose() const { return Verbose; }

private:
  // If true, the program will exit on the first error.
  bool ExitOnError = false;
  bool PrintStatsOnExit = true;
  bool WarningEnabled = false;
  // Maximum number of warnings before exiting.
  // 0 means no limit.
  size_t WarningLimit = 20;
  bool Verbose = false;
};

} // namespace interflop
