/**
 * @file Backend.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Base classes for the backends.
 * @version 0.5.1
 * @date 2021-07-20
 *
 *
 */

#pragma once
#include "OpaqueShadow.hpp"
#include "Utils.hpp"
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace insane {

/**
 * @brief FP comparisons
 *
 */
enum FCmpOpcode {
  OrderedFCmp,
  FCmp_oeq,
  FCmp_one,
  FCmp_ogt,
  FCmp_oge,
  FCmp_olt,
  FCmp_ole,
  UnorderedFCmp,
  FCmp_ueq,
  FCmp_une,
  FCmp_ugt,
  FCmp_uge,
  FCmp_ult,
  FCmp_ule
};

// Forward declaration to avoid circular inclusion
class InsaneContext;

/**
 * @brief Performs runtime initialization. Should use printf-like functions
 *
 * Must be implemented in the runtime library
 * Should atleast set the runtime name in the context and check for shadowscale
 *
 * @param Context
 */
void BackendInit(InsaneContext &Context) noexcept;

/**
 * @brief Perfom runtime finalization before program ends
 *
 * @param Context
 */
void BackendFinalize(InsaneContext &Context) noexcept;

class WarningRecorder {
public:
  void Record();

  virtual ~WarningRecorder() = default;
  virtual void print(std::string const &BackendName, std::ostream &out) = 0;

private:
  virtual void RecordImpl() = 0;
  size_t WarningCount = 0;
};

/**
 * @brief Helper class for registering error location
 *
 * Registers stacktraces that will be printed at the end of the program if
 * warnings are enabled. Uses clang stacktrace library
 *
 * Should be replaced by the std stacktrace library in the future
 */
class StacktraceRecorder : public WarningRecorder {
public:
  virtual ~StacktraceRecorder() = default;

  /**
   * @brief Pretty print every recorded stacktrace to the given stream.
   * Also outputs the number of time a stacktrace was recorded
   *
   * @param BackendName
   * @param out Output stream object
   */
  void print(std::string const &BackendName, std::ostream &out);

private:
  /**
   * @brief Records a stacktrace and store its Id for later printing
   *
   */
  void RecordImpl() {
    // Clang stack parsing should be thread safe
    size_t SId = utils::SaveStackTrace();

    std::scoped_lock<std::mutex> lock(Mutex);
    Map[SId]++;
  }

  // FIXME: We should probably store an error message along the stacktrace
  std::unordered_map<uint32_t, int> Map;
  std::mutex Mutex;
};

/**
 * @brief InsaneRuntime declaration.
 * Should be implemented in the runtime library
 *
 * @tparam MetaFP Describes the floating point type used for this backend
 * includes scalar type, vector size, shadow type, etc. Refer to MetaFloat
 */
template <typename MetaFP> class InsaneRuntime {
public:
  using FPType = typename MetaFP::FPType;
  using ShadowType = typename MetaFP::ShadowType;
  static constexpr size_t VectorSize = MetaFP::VectorSize;

  // This class is well documented for the runtime-developers
  // Hence some comments might be redundant

  /**
   * @brief Should return -a and store -sa in res
   *
   * @param a FP parameter
   * @param sa Shadow of a
   * @param res Return shadow
   * @return FPType Should be -a
   */
  FPType Neg(FPType a, ShadowType **sa, ShadowType **res);

  /**
   * @brief FP Addition. Should return a + b and store sa + sb in res
   *
   * @param a First FP operand
   * @param sa Shadow of a
   * @param b Second FP operand
   * @param sb shadow of b
   * @param res Return shadow
   * @return FPType Should be a + b
   */
  FPType Add(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);

  /**
   * @brief FP Substract. Should return a + b and store sa + sb in res
   *
   * @param a First FP operand
   * @param sa Shadow of a
   * @param b Second FP operand
   * @param sb shadow of b
   * @param res Return shadow
   * @return FPType Should be a + b
   */
  FPType Sub(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);

  /**
   * @brief FP Multiply. Should return a + b and store sa + sb in res
   *
   * @param a First FP operand
   * @param sa Shadow of a
   * @param b Second FP operand
   * @param sb shadow of b
   * @param res Return shadow
   * @return FPType Should be a + b
   */
  FPType Mul(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);

  /**
   * @brief FP Division. Should return a + b and store sa + sb in res
   *
   * @param a First FP operand
   * @param sa Shadow of a
   * @param b Second FP operand
   * @param sb shadow of b
   * @param res Return shadow
   * @return FPType Should be a + b
   */
  FPType Div(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);

  /**
   * @brief Performs an accuracy check using a FP and its shadow
   *
   * @param a FP Operand
   * @param sa Shadow of a
   * @return true if an error is detected
   * @return false if there's no error
   */
  bool Check(FPType a, ShadowType **sa);

  /**
   * @brief Check the accuracy of an FP comparison
   *
   * @param Opcode Comparisons opcode
   * @param LeftOperand FP first operand
   * @param LeftShadowOperand Shadow first operand
   * @param RightOperand FP second operand
   * @param RightShadowOperand Shadow second operand
   * @param Value Truth value of LeftOperand (OPCODE) RightOperand
   * @return true if the comparisons is correct
   * @return false if the comparisons is incorrect
   */
  bool CheckFCmp(FCmpOpcode Opcode, FPType LeftOperand,
                 ShadowType **LeftShadowOperand, FPType RightOperand,
                 ShadowType **RightShadowOperand, bool Value);

  /**
   * @brief Shadow constructor
   *
   * Note that there's no shadow destructor, so memory allocation is not
   * advised or an external memory manager should be used. Using a smart pointer
   * as it will also never get destroyed
   *
   * @param a FP Operand
   * @param res Pointer to the new shadow
   */
  void MakeShadow(FPType a, ShadowType **res);

  /**
   * @brief Cast a FP to a another FP type. The FP cast is performed in the
   * instrumentation, only the shadow cast is left to the runtime
   *
   * @param a FP operand
   * @param sa Shadow of a
   * @param res Casted return shadow
   */
  void CastToFloat(FPType a, ShadowType **sa, OpaqueShadow **res);

  /**
   * @brief Cast a FP to a another FP type. The FP cast is performed in the
   * instrumentation, only the shadow cast is left to the runtime
   *
   * @param a FP operand
   * @param sa Shadow of a
   * @param res Casted return shadow
   */
  void CastToDouble(FPType a, ShadowType **sa, OpaqueLargeShadow **res);

  /**
   * @brief Cast a FP to a another FP type. The FP cast is performed in the
   * instrumentation, only the shadow cast is left to the runtime
   *
   * @param a FP operand
   * @param sa Shadow of a
   * @param res Casted return shadow
   */
  void CastToLongdouble(FPType a, ShadowType **sa, OpaqueLargeShadow **res);
};
} // namespace insane
