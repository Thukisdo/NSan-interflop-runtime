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
  
  void ExitOnError(bool const Value) { _ExitOnError = Value; }
  bool ExitOnError() const { return _ExitOnError; }

  void PrintStatsOnExit(bool const value) { _PrintStatsOnExit = value; }
  bool PrintStatsOnExit() const { return _PrintStatsOnExit; }

  void WarningEnabled(bool const value) { _WarningEnabled = value; }
  bool WarningEnabled() const { return _WarningEnabled; }

  void WarningLimit(size_t const value) { _WarningLimit = value; }
  size_t WarningLimit() const { return _WarningLimit; }

  void Verbose(bool const value) { _Verbose = value; }
  bool Verbose() const { return _Verbose; }

  bool UseColor() const {return _UseColor;}
  void UseColor(bool const value) { _UseColor = value; }

private:
  // If true, the program will exit on the first error.
  bool _ExitOnError = false;
  bool _PrintStatsOnExit = true;
  bool _WarningEnabled = false;
  bool _UseColor = true;
  // Maximum number of warnings before exiting.
  // 0 means no limit.
  // FIXME : not used yet 
  size_t _WarningLimit = 20;
  bool _Verbose = false;
};

} // namespace interflop
