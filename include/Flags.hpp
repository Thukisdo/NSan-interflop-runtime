/**
 * @file Flags.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Stores global runtime flags.
 * @version 0.2.0
 * @date 2021-08-31
 *
 *
 */
#pragma once
#include <iostream>

namespace insane {

// Storage class for the runtime flags
// We could use a struct to avoid setter/getters, but this is both easier to
// read and to use in external code (i.e defining specific flags for testing)
class RuntimeFlags {
public:
  RuntimeFlags() = default;

  void setExitOnError(bool const Value) { ExitOnError = Value; }
  bool getExitOnError() const { return ExitOnError; }

  void setPrintStatsOnExit(bool const value) { PrintStatsOnExit = value; }
  bool getPrintStatsOnExit() const { return PrintStatsOnExit; }

  void setWarningEnabled(bool const value) { WarningEnabled = value; }
  bool getWarningEnabled() const { return WarningEnabled; }

  void setStackRecording(bool const value) { StackRecording = value; }
  bool getStackRecording() const { return StackRecording; }

  void setWarningLimit(size_t const value) { WarningLimit = value; }
  size_t getWarningLimit() const { return WarningLimit; }

  void setVerbose(bool const value) { Verbose = value; }
  bool getVerbose() const { return Verbose; }

  void setUseColor(bool const value) { UseColor = value; }
  bool getUseColor() const { return UseColor; }

  /**
   * @brief Read the environnement var "INSANE_OPTIONS" and parse it for flags
   * 
   * @return true if the environnement var was found
   * @return false
   */
  bool LoadFromEnvironnement();

  /**
   * @brief Load flags from a string
   * 
   * @param Flags Input string containing flags
   * @return true 
   * @return false 
   */
  bool LoadFromString(const std::string &Flags);

  /**
   * @brief Load flags from a path
   * 
   * @param FileName defaults to "insane_flags.cfg"
   * @return true 
   * @return false 
   */
  bool LoadFromFile(const std::string &FileName = "insane_flags.cfg");

private:
  // Parse a string generated from LoadFromEnvironnement() or LoadFile() and set
  // appropriate flags
  void ParseFlag(std::string const &Str);

  // If true, the program will exit on the first error.
  // FIXME : calling conventions
  bool ExitOnError = true;
  bool PrintStatsOnExit = true;
  bool WarningEnabled = true;
  bool StackRecording = true;
  bool UseColor = true;

  // Maximum number of warnings before exiting.
  // 0 means no limit.
  // FIXME : not used yet
  size_t WarningLimit = 20;
  bool Verbose = false;
};

} // namespace insane
