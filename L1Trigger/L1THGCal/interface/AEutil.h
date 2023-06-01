#ifndef L1Trigger_L1THGCal_AEutil_H
#define L1Trigger_L1THGCal_AEutil_H


static constexpr int nTriggerCells = 48;
static constexpr int cellUVSize = 8;
static constexpr int latentSize = 16;
static constexpr int cellUVremap[cellUVSize][cellUVSize] = {{47, 46, 45, 44, -1, -1, -1, -1},
                                                            {16, 43, 42, 41, 40, -1, -1, -1},
                                                            {20, 17, 39, 38, 37, 36, -1, -1},
                                                            {24, 21, 18, 35, 34, 33, 32, -1},
                                                            {28, 25, 22, 19, 3, 7, 11, 15},
                                                            {-1, 29, 26, 23, 2, 6, 10, 14},
                                                            {-1, -1, 30, 27, 1, 5, 9, 13},
                                                            {-1, -1, -1, 31, 0, 4, 8, 12}};
static constexpr int ae_outputCellU[nTriggerCells] = {7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4,
                                                      1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 6, 4, 5, 6, 7,
                                                      3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0};
static constexpr int ae_outputCellV[nTriggerCells] = {4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7,
                                                      0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
                                                      6, 5, 4, 3, 5, 4, 3, 2, 4, 3, 2, 1, 3, 2, 1, 0};

using AEMap=std::unordered_map<unsigned int, std::array<float, nTriggerCells>>;
using ECONMap=std::unordered_map<unsigned int, std::array<float, latentSize>>;

enum InputType {
  ADC, //hwpt
  ADCT, //hwpt/cosh(eta)
  MIP, //mipT * cosh(eta)
  MIPT, //mipT
  E, //energy
  ET //energy/cosh(eta)
};

inline std::string inputTypeEnumToStr(InputType en){
  switch (en){
    case ADC:
      return "ADC";
    case ADCT:
      return "ADCT";
    case MIP:
      return "MIP";
    case MIPT:
      return "MIPT";
    case E:
      return "E";
    case ET:
      return "ET";
    default:
      throw cms::Exception("InputTypeEnumToStr") << "Invalid InputType";
  }
}

inline InputType inputTypeStrToEnum(std::string inputTypeStr){
  if(inputTypeStr == "ADC"){
    return InputType::ADC;
  } else if(inputTypeStr == "ADCT"){
    return InputType::ADCT;
  } else if(inputTypeStr == "MIP"){
    return InputType::MIP;
  } else if(inputTypeStr == "MIPT"){
    return InputType::MIPT;
  } else if(inputTypeStr == "E"){
    return InputType::E;
  } else if(inputTypeStr == "ET"){
    return InputType::ET;
  } else{
    throw cms::Exception("InputTypeStrToEnum") << "Invalid InputType in constructor";
  }
}

enum NormType{
  None,
  Floating,
  BitShift
};

inline NormType normTypeStrToEnum(std::string str){
  if(str == "None"){
    return NormType::None;
  } else if(str == "Floating"){
    return NormType::Floating;
  } else if(str == "BitShift"){
    return NormType::BitShift;
  } else {
    throw cms::Exception("NormTypeStrToEnum") << "Invalid NormType string";
  }
}

#endif
