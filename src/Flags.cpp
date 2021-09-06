/**
 * @file Flags.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Stores global runtime flags.
 * @version 0.2.0
 * @date 2021-08-31
 *
 *
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>

#include "Flags.hpp"
#include "Utils.hpp"

namespace insane {

// Can only use printf during initialization
bool RuntimeFlags::LoadFromEnvironnement() {
  char *env_var = std::getenv("INSANE_OPTIONS");
  if (env_var == nullptr)
    return false;

  fprintf(stderr, "[INSanE] Loading config from environnement\n");
  ParseFlag(env_var);
  return true;
}

// Can only use printf during initialization
bool RuntimeFlags::LoadFromString(const std::string &Flags) {
  fprintf(stderr, "[INSanE] Loading config from string\n");
  ParseFlag(Flags);
  return true;
}

// Can only use printf during initialization
bool RuntimeFlags::LoadFromFile(const std::string &Filename) {

  std::ifstream ConfigFile(Filename);
  
  if (not ConfigFile.is_open()) {
    return false;
  }

  fprintf(stderr, "[INSanE] Found insane's config file\n");

  ConfigFile.seekg(0, ConfigFile.end);
  size_t Size = ConfigFile.tellg();
  ConfigFile.seekg(0, ConfigFile.beg);

  if (not ConfigFile.is_open())
    return false;

  std::string Str;
  Str.reserve(Size);
  Str.assign(std::istreambuf_iterator<char>(ConfigFile),
             std::istreambuf_iterator<char>());
  ParseFlag(Str);

  ConfigFile.close();
  return true;
}

// Can only use printf during initialization
void RuntimeFlags::ParseFlag(std::string const &Str) {

  std::regex reg("(\\w*)\\s*=\\s*(\\w*)",
                 std::regex::ECMAScript | std::regex::icase);

  auto FlagBegin = std::sregex_iterator(Str.begin(), Str.end(), reg);

  size_t RecognizedFlag = 0;
  for (auto It = FlagBegin; It != std::sregex_iterator(); It++) {
    std::string FlagName = (*It)[1].str();
    std::string Value = (*It)[2].str();

    if (FlagName == "exit_on_error")
      ExitOnError = (Value == "true");
    else if (FlagName == "warning_enabled")
      WarningEnabled = (Value == "true");
    else if (FlagName == "stack_recording")
      StackRecording = (Value == "true");
    else if (FlagName == "verbose")
      Verbose = (Value == "true");
    else if (FlagName == "use_color")
      UseColor = (Value == "true");
    else if (FlagName == "print_stats_on_exit")
      PrintStatsOnExit = (Value == "true");
    else if (FlagName == "warning_limit") {
      WarningLimit = std::stoi(Value);
      if (WarningLimit < 0) {
        fprintf(stderr, "[INSanE] Invalid value for warning limit : \'%lu\'",
                WarningLimit);
        WarningLimit = 0;
      }
    } else
      continue;
    RecognizedFlag++;
  }

  printf("[INSanE] %lu flag(s) loaded\n", RecognizedFlag);
}

} // namespace insane