#ifndef __AnalysisSpace_TreeMaker_METBlock_h
#define __AnalysisSpace_TreeMaker_METBlock_h

#include <string>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

namespace vhtm {
  class MET;
}
class METBlock : public edm::EDAnalyzer
{
private:
  virtual void beginJob();
  virtual void beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup) {}
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  virtual void endJob() {}

public:
  explicit METBlock(const edm::ParameterSet& iConfig);
  virtual ~METBlock() {}

  enum {
    kMaxMET_ = 5
  };

  void fillMET(const edm::Event& iEvent,
               const edm::EventSetup& iSetup,
               std::vector<vhtm::MET>* list,
               int& fnMET,
               const edm::InputTag& iTag,  
               const edm::EDGetTokenT<pat::METCollection>& token);
  void fillMET(const edm::Event& iEvent,
               const edm::EventSetup& iSetup,
               std::vector<vhtm::MET>* list,
               int& fnMET,
               const edm::InputTag& iTag,  
               const edm::EDGetTokenT<reco::PFMETCollection>& token);

private:
  std::vector<vhtm::MET>* pfList_;
  int fnPFMET_;

  std::vector<vhtm::MET>* corrList_;
  int fnCorrMET_;

  std::vector<vhtm::MET>* puppiList_;
  int fnPuppiMET_;

  std::vector<vhtm::MET>* mvaList_;
  int fnMVAMET_;

  const int verbosity_;
  const edm::InputTag pfMETTag_;
  const edm::InputTag corrMETTag_;
  const edm::InputTag puppiMETTag_;
  const edm::InputTag mvaMETTag_;

  const edm::EDGetTokenT<pat::METCollection> pfMETToken_;
  const edm::EDGetTokenT<pat::METCollection> corrMETToken_;
  const edm::EDGetTokenT<pat::METCollection> puppiMETToken_;
  const edm::EDGetTokenT<reco::PFMETCollection> mvaMETToken_;
};
#endif
