
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

#include "../external/nlohmann_json/json.hpp"
#include "../core/global.h"
#include "../core/bsearch.h"
#include "../core/rand.h"
#include "../core/elo.h"
#include "../core/fancymath.h"
#include "../core/hash.h"
#include "../game/board.h"
#include "../game/rules.h"
#include "../game/boardhistory.h"
#include "../neuralnet/nninputs.h"
#include "../tests/tests.h"
#include "../main.h"
#include "../neuralnet/nneval.h"

using namespace std;
using json = nlohmann::json;

int MainCmds::runtests(int argc, const char* const* argv) {
  (void)argc;
  (void)argv;
  testAssert(sizeof(size_t) == 8);
  Board::initHash();
  ScoreValue::initTables();

  BSearch::runTests();
  Rand::runTests();
  FancyMath::runTests();
  ComputeElos::runTests();


  Tests::runBoardIOTests();
  Tests::runBoardBasicTests();
  Tests::runBoardAreaTests();

  Tests::runRulesTests();

  Tests::runBoardUndoTest();
  Tests::runBoardHandicapTest();
  Tests::runBoardStressTest();

  Tests::runSgfTests();

  ScoreValue::freeTables();

  cout << "All tests passed" << endl;
  return 0;
}

int MainCmds::runoutputtests(int argc, const char* const* argv) {
  (void)argc;
  (void)argv;
  Board::initHash();
  ScoreValue::initTables();

  Tests::runNNInputsV3V4Tests();
  Tests::runNNLessSearchTests();
  Tests::runTrainingWriteTests();
  Tests::runTimeControlsTests();
  Tests::runScoreTests();

  ScoreValue::freeTables();

  return 0;
}

int MainCmds::runsearchtests(int argc, const char* const* argv) {
  Board::initHash();
  ScoreValue::initTables();

  if(argc != 6) {
    cerr << "Must supply exactly five arguments: MODEL_FILE INPUTSNHWC CUDANHWC SYMMETRY FP16" << endl;
    return 1;
  }
  Tests::runSearchTests(
    string(argv[1]),
    Global::stringToBool(argv[2]),
    Global::stringToBool(argv[3]),
    Global::stringToInt(argv[4]),
    Global::stringToBool(argv[5])
  );

  ScoreValue::freeTables();

  return 0;
}

int MainCmds::runsearchtestsv3(int argc, const char* const* argv) {
  Board::initHash();
  ScoreValue::initTables();

  if(argc != 6) {
    cerr << "Must supply exactly five arguments: MODEL_FILE INPUTSNHWC CUDANHWC SYMMETRY FP16" << endl;
    return 1;
  }
  Tests::runSearchTestsV3(
    string(argv[1]),
    Global::stringToBool(argv[2]),
    Global::stringToBool(argv[3]),
    Global::stringToInt(argv[4]),
    Global::stringToBool(argv[5])
  );

  ScoreValue::freeTables();

  return 0;
}

int MainCmds::runsearchtestsv8(int argc, const char* const* argv) {
  Board::initHash();
  ScoreValue::initTables();

  if(argc != 5) {
    cerr << "Must supply exactly four arguments: MODEL_FILE INPUTSNHWC CUDANHWC FP16" << endl;
    return 1;
  }
  Tests::runSearchTestsV8(
    string(argv[1]),
    Global::stringToBool(argv[2]),
    Global::stringToBool(argv[3]),
    Global::stringToBool(argv[4])
  );

  ScoreValue::freeTables();

  return 0;
}

int MainCmds::runmmctstests(int argc, const char *const *argv) {
  Board::initHash();
  ScoreValue::initTables();

  const string &modelFile = std::string(argv[1]);
  const string &seed = "seed";
  int nnXLen = 9;
  int nnYLen = 9;
  int defaultSymmetry = 0;
  bool inputsUseNHWC = false;
  bool useNHWC = false;
  bool useFP16 = false;
  bool debugSkipNeuralNet = false;
  bool requireExactNNLen = false;

  vector<int> gpuIdxByServerThread = {0};
  int maxBatchSize = 16;
  //bool inputsUseNHWC = true;
  int nnCacheSizePowerOfTwo = 16;
  int nnMutexPoolSizePowerOfTwo = 12;
  int maxConcurrentEvals = 1024;
  //bool debugSkipNeuralNet = false;
  bool openCLReTunePerBoardSize = false;
  const string &modelName = modelFile;
  const string openCLTunerFile = "";
  int numNNServerThreadsPerModel = 1;
  bool nnRandomize = false;
  string nnRandSeed = "runSearchTestsRandSeed" + seed;

  defaultSymmetry = 0;

  NNEvaluator *nnEval = new NNEvaluator(
      modelName,
      modelFile,
      nullptr,
      maxBatchSize,
      maxConcurrentEvals,
      nnXLen,
      nnYLen,
      requireExactNNLen,
      inputsUseNHWC,
      nnCacheSizePowerOfTwo,
      nnMutexPoolSizePowerOfTwo,
      debugSkipNeuralNet,
      openCLTunerFile,
      openCLReTunePerBoardSize,
      useFP16 ? enabled_t::True : enabled_t::False,
      useNHWC ? enabled_t::True : enabled_t::False,
      numNNServerThreadsPerModel,
      gpuIdxByServerThread,
      nnRandSeed,
      nnRandomize,
      defaultSymmetry
  );
  (void) inputsUseNHWC;

  nnEval->spawnServerThreads();
  std::this_thread::sleep_for(std::chrono::duration<double>(0.1));

  std::ifstream infile;
  infile.open(std::string(argv[2]));
  std::ofstream outfile;
  outfile.open(std::string(argv[3]));

  std::string line;
  while (std::getline(infile, line)) {
    std::string suffix = "JSON_DATA";
    auto foundPos = line.find(suffix);
    if (foundPos != std::string::npos) {
      auto rest = line.substr(foundPos + suffix.size() + 1);
      auto parsed = json::parse(rest);
      auto locs = parsed["history"]["locs"];
      auto players = parsed["history"]["players"];

      Board board{nnXLen, nnYLen};

      Rules rules;
      auto parseRulesResult = Rules::tryParseRules(parsed["history"]["rules"], rules);
      if (!parseRulesResult) {
        throw std::runtime_error("Could not parse");
      }

      BoardHistory history;
      history.clear(board, P_BLACK, rules, 0);

      for (size_t i = 0; i < locs.size(); ++i) {
        history.makeBoardMoveAssumeLegal(board, locs[i], players[i], nullptr);
      }

      MiscNNInputParams nnInputParams;
      NNResultBuf buf;

      Color nextPla = P_BLACK;
      if (players.size()) {
        nextPla = getOpp(players[players.size() - 1]);
      }
      nnEval->evaluate(
          board,
          history,
          nextPla,
          nnInputParams,
          buf,
          false,
          true
      );

      while (!buf.hasResult) {
        std::cout << "waiting result" << std::endl;
        std::this_thread::sleep_for(std::chrono::duration<double>(0.03));
      }

      json outData;
      outData["orig"]["whiteOwnerMap"] = parsed["whiteOwnerMap"];
      outData["orig"]["whiteWinProb"] = parsed["whiteWinProb"];
      outData["history"] = parsed["history"];
      outData["new"]["whiteWinProb"] = buf.result->whiteWinProb;
      int ownerMapSize = buf.result->nnXLen* buf.result->nnYLen;
      float *whiteOwnerMap = buf.result->whiteOwnerMap;
      outData["new"]["whiteOwnerMap"] = std::vector<float>(whiteOwnerMap, whiteOwnerMap + ownerMapSize);
      outfile << outData.dump() << std::endl;
    }
  }

  outfile.close();

  return 0;
}

int MainCmds::runselfplayinittests(int argc, const char* const* argv) {
  if(argc != 2) {
    cerr << "Must supply exactly one argument: MODEL_FILE" << endl;
    return 1;
  }

  Board::initHash();
  ScoreValue::initTables();

  Tests::runSelfplayInitTestsWithNN(
    string(argv[1])
  );
  Tests::runMoreSelfplayTestsWithNN(
    string(argv[1])
  );

  ScoreValue::freeTables();

  return 0;
}

int MainCmds::runsekitrainwritetests(int argc, const char* const* argv) {
  if(argc != 2) {
    cerr << "Must supply exactly one argument: MODEL_FILE" << endl;
    return 1;
  }

  Board::initHash();
  ScoreValue::initTables();

  Tests::runSekiTrainWriteTests(
    string(argv[1])
  );

  ScoreValue::freeTables();

  return 0;
}

int MainCmds::runnnlayertests(int argc, const char* const* argv) {
  (void)argc;
  (void)argv;
  Tests::runNNLayerTests();
  return 0;
}

int MainCmds::runnnontinyboardtest(int argc, const char* const* argv) {
  if(argc != 6) {
    cerr << "Must supply exactly five arguments: MODEL_FILE INPUTSNHWC CUDANHWC SYMMETRY FP16" << endl;
    return 1;
  }
  Board::initHash();
  ScoreValue::initTables();

  Tests::runNNOnTinyBoard(
    string(argv[1]),
    Global::stringToBool(argv[2]),
    Global::stringToBool(argv[3]),
    Global::stringToInt(argv[4]),
    Global::stringToBool(argv[5])
  );

  ScoreValue::freeTables();

  return 0;
}

int MainCmds::runnnsymmetriestest(int argc, const char* const* argv) {
  if(argc != 5) {
    cerr << "Must supply exactly four arguments: MODEL_FILE INPUTSNHWC CUDANHWC FP16" << endl;
    return 1;
  }
  Board::initHash();
  ScoreValue::initTables();

  Tests::runNNSymmetries(
    string(argv[1]),
    Global::stringToBool(argv[2]),
    Global::stringToBool(argv[3]),
    Global::stringToBool(argv[4])
  );

  ScoreValue::freeTables();

  return 0;
}

int MainCmds::runnnonmanyposestest(int argc, const char* const* argv) {
  if(argc != 6 && argc != 7) {
    cerr << "Must supply five or six arguments: MODEL_FILE INPUTSNHWC CUDANHWC SYMMETRY FP16 [COMPARISONFILE]" << endl;
    return 1;
  }
  Board::initHash();
  ScoreValue::initTables();

  if(argc == 6) {
    Tests::runNNOnManyPoses(
      string(argv[1]),
      Global::stringToBool(argv[2]),
      Global::stringToBool(argv[3]),
      Global::stringToInt(argv[4]),
      Global::stringToBool(argv[5]),
      ""
    );
  }
  else if(argc == 7) {
    Tests::runNNOnManyPoses(
      string(argv[1]),
      Global::stringToBool(argv[2]),
      Global::stringToBool(argv[3]),
      Global::stringToInt(argv[4]),
      Global::stringToBool(argv[5]),
      string(argv[6])
    );
  }

  ScoreValue::freeTables();

  return 0;
}

int MainCmds::runownershiptests(int argc, const char* const* argv) {
  if(argc != 3) {
    cerr << "Must supply exactly two arguments: GTP_CONFIG MODEL_FILE" << endl;
    return 1;
  }
  Board::initHash();
  ScoreValue::initTables();

  Tests::runOwnershipTests(
    string(argv[1]),
    string(argv[2])
  );

  ScoreValue::freeTables();
  return 0;
}
