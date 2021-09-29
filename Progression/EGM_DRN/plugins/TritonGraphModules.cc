#include "HeterogeneousCore/SonicTriton/interface/TritonEDProducer.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <sstream>
#include <string>
#include <vector>
#include <random>

class TritonGraphHelper {
public:
  TritonGraphHelper(edm::ParameterSet const& cfg)
      : nodeMin_(cfg.getParameter<unsigned>("nodeMin")),
        nodeMax_(cfg.getParameter<unsigned>("nodeMax")),
        brief_(cfg.getParameter<bool>("brief")) {}
  void makeInput(edm::Event const& iEvent, TritonInputMap& iInput, const std::string& debugName) const {
    
    //get event-based seed for RNG
    unsigned int runNum_uint = static_cast<unsigned int>(iEvent.id().run());
    unsigned int lumiNum_uint = static_cast<unsigned int>(iEvent.id().luminosityBlock());
    unsigned int evNum_uint = static_cast<unsigned int>(iEvent.id().event());
    std::uint32_t seed = (lumiNum_uint << 10) + (runNum_uint << 20) + evNum_uint;
    std::mt19937 rng(seed);

    std::uniform_int_distribution<int> randint1(nodeMin_, nodeMax_);
    int nnodes = randint1(rng);

    //set shapes
    auto& input1 = iInput.at("x__0");
    input1.setShape(0, nnodes);
    auto data1 = input1.allocate<float>();
    auto& vdata1 = (*data1)[0];

    auto& input2 = iInput.at("batch__1");
    input2.setShape(0, nnodes);
    auto data2 = input2.allocate<int64_t>();
    auto& vdata2 = (*data2)[0];

    auto& input3 = iInput.at("graphx__2");
    input3.setShape(0, 2);
    auto data3 = input3.allocate<float>();
    auto& vdata3 = (*data3)[0];

    //fill
    std::normal_distribution<float> randx(-10, 4);
    for (unsigned i = 0; i < input1.sizeShape(); ++i) {
        vdata1.push_back(randx(rng));
    }

    for (unsigned i = 0; i < input2.sizeShape(); ++i) {
        vdata2.push_back(0);
    }

    for (unsigned i = 0; i < input3.sizeShape(); ++i) {
        vdata3.push_back(randx(rng));
    }

    // convert to server format
    input1.toServer(data1);
    input2.toServer(data2);
    input3.toServer(data3);

    edm::LogInfo(debugName) << "input X shape: " << input1.shape()[0] << ", " << input1.shape()[1];
    edm::LogInfo(debugName) << "input batch shape: " << input2.shape()[0];
    edm::LogInfo(debugName) << "input graphx shape: " << input3.shape()[0];
  }
  void makeOutput(const TritonOutputMap& iOutput, const std::string& debugName) const {
    edm::LogInfo (debugName) << "top of output ";
    //check the results
    const auto& output1 = iOutput.begin()->second;
    // convert from server format
    const auto& tmp = output1.fromServer<float>();
    //if (brief_)
    edm::LogInfo(debugName) << "output shape: " << output1.shape()[0] << "," << output1.shape()[1];
    //else {
    //  edm::LogInfo msg(debugName);
    //  for (int i = 0; i < output1.shape()[0]; ++i) {
    //    msg << "output " << i << ": ";
    //    for (int j = 0; j < output1.shape()[1]; ++j) {
    //      msg << tmp[0][output1.shape()[1] * i + j] << " ";
    //    }
    //    msg << "\n";
    //  }
    //}
  }
  static void fillPSetDescription(edm::ParameterSetDescription& desc) {
    desc.add<unsigned>("nodeMin", 1);
    desc.add<unsigned>("nodeMax", 200);
    desc.add<bool>("brief", false);
  }

private:
  //members
  unsigned nodeMin_, nodeMax_;
  bool brief_;
};

class DRNProducer : public TritonEDProducer<> {
public:
  explicit DRNProducer(edm::ParameterSet const& cfg)
      : TritonEDProducer<>(cfg, "DRNProducer"), helper_(cfg) {}
  void acquire(edm::Event const& iEvent, edm::EventSetup const& iSetup, Input& iInput) override {
    helper_.makeInput(iEvent, iInput, debugName_);
  }
  void produce(edm::Event& iEvent, edm::EventSetup const& iSetup, Output const& iOutput) override {
    helper_.makeOutput(iOutput, debugName_);
  }
  ~DRNProducer() override = default;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    TritonClient::fillPSetDescription(desc);
    TritonGraphHelper::fillPSetDescription(desc);
    //to ensure distinct cfi names
    descriptions.addWithDefaultLabel(desc);
  }

private:
  //member
  TritonGraphHelper helper_;
};

DEFINE_FWK_MODULE(DRNProducer);
