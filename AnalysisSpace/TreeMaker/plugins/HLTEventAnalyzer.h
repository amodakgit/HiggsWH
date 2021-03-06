#ifndef AnalysisSpace_TreeMaker_HLTEventAnalyzerAOD_h
#define AnalysisSpace_TreeMaker_HLTEventAnalyzerAOD_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

class HLTEventAnalyzer : public edm::EDAnalyzer {
public:
  explicit HLTEventAnalyzer(const edm::ParameterSet&);
  ~HLTEventAnalyzer();
 
  virtual void beginRun(edm::Run const &, edm::EventSetup const&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void analyzeTrigger(const edm::Event&, const edm::EventSetup&, const std::string& triggerName);
 
private:

  /// module config parameters
  std::string   processName_;
  std::string   triggerName_;
  edm::InputTag triggerResultsTag_;
  edm::InputTag triggerEventTag_;
 
  /// additional class data memebers
  edm::Handle<edm::TriggerResults>   triggerResultsHandle_;
  edm::Handle<trigger::TriggerEvent> triggerEventHandle_;
  HLTConfigProvider hltConfig_;
  HLTPrescaleProvider hltPrescaleProvider_;

  edm::EDGetTokenT<edm::TriggerResults> _srcTriggerResultsToken;
  edm::EDGetTokenT<trigger::TriggerEvent> _srcTriggerEventToken;
};
#endif
