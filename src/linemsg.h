#ifndef RCHK_LINEMSG_H
#define RCHK_LINEMSG_H

#include "common.h"

#include <set>

#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/LLVMContext.h>

#include <llvm/Support/raw_ostream.h>

using namespace llvm;

struct LineInfoTy {
  std::string kind;
  std::string message;
  std::string path;
  unsigned line;
  
  public:
  LineInfoTy(std::string kind, std::string message, std::string path, unsigned line): 
    kind(kind), message(message), path(path), line(line) {}
    
  void print() const;

};

struct LineInfoTy_compare {
  bool operator() (const LineInfoTy& lhs, const LineInfoTy& rhs) const;
};

typedef std::set<LineInfoTy, LineInfoTy_compare> LineBufferTy;

class LineMessenger {

  const bool DEBUG;
  const bool TRACE;
  const bool UNIQUE_MSG;
  
  LineBufferTy lineBuffer;
  Function *lastFunction;
  
  private:
    void lineInfo(std::string kind, std::string message, Instruction *in, Function *func, LLVMContext& context);
  
  public:
    LineMessenger(bool DEBUG = false, bool TRACE = false, bool UNIQUE_MSG = false):
      DEBUG(DEBUG), TRACE(TRACE), UNIQUE_MSG(UNIQUE_MSG) {};
      
    void flush();
    void clearForFunction(Function *func);
    void trace(std::string msg, Instruction *in, Function *func, LLVMContext& context);
    void debug(std::string msg, Instruction *in, Function *func, LLVMContext& context);
    void info(std::string msg, Instruction *in, Function *func, LLVMContext& context);
    void error(std::string msg, Instruction *in, Function *func, LLVMContext& context);
};

#endif
