#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtFdlWord.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "TTree.h"

#include "AnalysisSpace/TreeMaker/interface/PhysicsObjects.h"
#include "AnalysisSpace/TreeMaker/plugins/EventBlock.h"
#include "AnalysisSpace/TreeMaker/interface/Utility.h"

EventBlock::EventBlock(const edm::ParameterSet& iConfig):
  verbosity_(iConfig.getUntrackedParameter<int>("verbosity", 0)),
  l1Tag_(iConfig.getUntrackedParameter<edm::InputTag>("l1Tag", edm::InputTag("gtDigis"))),
  vertexTag_(iConfig.getUntrackedParameter<edm::InputTag>("vertexTag", edm::InputTag("goodOfflinePrimaryVertices"))),
  //trackTag_(iConfig.getUntrackedParameter<edm::InputTag>("trkTag", edm::InputTag("generalTracks"))),
  selectedVertexTag_(iConfig.getUntrackedParameter<edm::InputTag>("selectedVtxTag", edm::InputTag("selectedPrimaryVertices"))),
  puSummaryTag_(iConfig.getUntrackedParameter<edm::InputTag>("puSummaryInputTag", edm::InputTag("slimmedAddPileupInfo"))),
  rhoTag_(iConfig.getUntrackedParameter<edm::InputTag>("rhoTag", edm::InputTag("kt6PFJets","rho"))),
  rhoNeutralTag_(iConfig.getUntrackedParameter<edm::InputTag>("rhoNeutralTag", edm::InputTag("kt6PFNeutralJetsForVtxMultReweighting", "rho"))),
  vtxMinNDOF_(iConfig.getUntrackedParameter<unsigned int>("vertexMinimumNDOF", 4)),
  vtxMaxAbsZ_(iConfig.getUntrackedParameter<double>("vertexMaxAbsZ", 24.)),
  vtxMaxd0_(iConfig.getUntrackedParameter<double>("vertexMaxd0", 2.0)),
  numTracks_(iConfig.getUntrackedParameter<unsigned int>("numTracks", 10)),
  hpTrackThreshold_(iConfig.getUntrackedParameter<double>("hpTrackThreshold", 0.25)),
  l1Token_(consumes<L1GlobalTriggerReadoutRecord>(l1Tag_)),
  vertexToken_(consumes<reco::VertexCollection>(vertexTag_)),
  //trackToken_(consumes<reco::TrackCollection>(trackTag_)),
  selectedVertexToken_(consumes<reco::VertexCollection>(selectedVertexTag_)),
  puSummaryToken_(consumes<std::vector<PileupSummaryInfo> >(puSummaryTag_)),
  rhoToken_(consumes<double>(rhoTag_)),
  rhoNeutralToken_(consumes<double>(rhoNeutralTag_))
{
}
EventBlock::~EventBlock() {
  delete nPU_;
  delete bunchCrossing_;
  delete trueNInt_;
  delete list_;
}
void EventBlock::beginJob() {

  // Get TTree pointer
  TTree* tree = vhtm::Utility::getTree("vhtree");
  list_ = new std::vector<vhtm::Event>();
  tree->Branch("Event", "std::vector<vhtm::Event>", &list_, 32000, -1);

  nPU_ = new std::vector<int>();
  tree->Branch("nPU", "std::vector<int>", &nPU_);

  bunchCrossing_ = new std::vector<int>();
  tree->Branch("bunchCrossing", "std::vector<int>", &bunchCrossing_);

  trueNInt_ = new std::vector<int>();
  tree->Branch("trueNInt", "std::vector<int>", &trueNInt_);
}
void EventBlock::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup) {
  // Reset the vector
  list_->clear();

  // Clear the independent vectors
  nPU_->clear();
  bunchCrossing_->clear();
  trueNInt_->clear();

  // Create Event Object
  vhtm::Event ev;
  ev.run   = iEvent.id().run();
  ev.event = iEvent.id().event();
  ev.lumis = iEvent.id().luminosityBlock();
  ev.bunch = iEvent.bunchCrossing();
  ev.orbit = iEvent.orbitNumber();

  double sec = iEvent.time().value() >> 32 ;
  double usec = 0xFFFFFFFF & iEvent.time().value();
  double conv = 1e6;
  ev.time   = sec+usec/conv;
  ev.isdata = iEvent.isRealData();
 
  edm::Handle<L1GlobalTriggerReadoutRecord> l1GtReadoutRecord;
  bool found = iEvent.getByToken(l1Token_, l1GtReadoutRecord);

/*
  // Technical Trigger Part
  if (found && l1GtReadoutRecord.isValid()) {
    edm::LogInfo("EventBlock") << "Successfully obtained L1GlobalTriggerReadoutRecord for label: "
                               << l1Tag_;

    L1GtFdlWord fdlWord = l1GtReadoutRecord->gtFdlWord();
    if (fdlWord.physicsDeclared() == 1)
      ev.isPhysDeclared = true;

    // BPTX0
    if (l1GtReadoutRecord->technicalTriggerWord()[0])
      ev.isBPTX0 = true;

    // MinBias
    if (l1GtReadoutRecord->technicalTriggerWord()[40] || l1GtReadoutRecord->technicalTriggerWord()[41])
      ev.isBSCMinBias = true;

    // BeamHalo
    if ( (l1GtReadoutRecord->technicalTriggerWord()[36] || l1GtReadoutRecord->technicalTriggerWord()[37] ||
          l1GtReadoutRecord->technicalTriggerWord()[38] || l1GtReadoutRecord->technicalTriggerWord()[39]) ||
         ((l1GtReadoutRecord->technicalTriggerWord()[42] && !l1GtReadoutRecord->technicalTriggerWord()[43]) ||
          (l1GtReadoutRecord->technicalTriggerWord()[43] && !l1GtReadoutRecord->technicalTriggerWord()[42])) )
      ev.isBSCBeamHalo = true;
  }
  else {
    edm::LogError("EventBlock") << "Failed to get L1GlobalTriggerReadoutRecord for label: "
                                << l1Tag_;
  }
*/
  // Good Primary Vertex Part
  edm::Handle<reco::VertexCollection> primaryVertices;
  found = iEvent.getByToken(vertexToken_, primaryVertices);

  if (found && primaryVertices.isValid()) {
    edm::LogInfo("EventBlock") << "Total # Primary Vertices: " << primaryVertices->size();
    for (const reco::Vertex& v: *primaryVertices) {
      if (!v.isFake() &&
           v.ndof() > vtxMinNDOF_ &&
	   std::abs(v.z()) <= vtxMaxAbsZ_ &&
	   std::abs(v.position().rho()) <= vtxMaxd0_)
      {
        ev.isPrimaryVertex = true;
        break;
      }
    }
  }
  else {
    edm::LogError("EventBlock") << "Error! Failed to get VertexCollection for label: "
                                << vertexTag_;
  }
#if 0
  // Scraping Events Part
  edm::Handle<reco::TrackCollection> tracks;
  found = iEvent.getByToken(trackToken_, tracks);

  if (found && tracks.isValid()) {
    edm::LogInfo("EventBlock") << "Total # Tracks: " << tracks->size();

    int numhighpurity = 0;
    double fraction = 1.;
    reco::TrackBase::TrackQuality trackQuality = reco::TrackBase::qualityByName("highPurity");
    if (tracks->size() > numTracks_) {
      for (const reco::Track& v: *tracks)
        if (v.quality(trackQuality)) numhighpurity++;
      fraction = static_cast<double>(numhighpurity)/static_cast<double>(tracks->size());
      if (fraction < hpTrackThreshold_)
        ev.isBeamScraping = true;
    }
  }
  else {
    edm::LogError("EventBlock") << "Error! Failed to get TrackCollection for label: "
                                << trackTag_;
  }
#endif
  // Access PU information
  if (!iEvent.isRealData()) {
    edm::Handle<std::vector<PileupSummaryInfo> > puInfo;
    found = iEvent.getByToken(puSummaryToken_, puInfo);
    if (found && puInfo.isValid()) {
      for (const PileupSummaryInfo& v: *puInfo) {
	ev.bunchCrossing.push_back(v.getBunchCrossing()); // in-time and out-of-time indices
	bunchCrossing_->push_back(v.getBunchCrossing());

	ev.nPU.push_back(v.getPU_NumInteractions());
	nPU_->push_back(v.getPU_NumInteractions());

	ev.trueNInt.push_back(v.getTrueNumInteractions());
	trueNInt_->push_back(v.getTrueNumInteractions());
      }
    }
    // More info about PU is here:
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupInformation#Accessing_PileupSummaryInfo_in_r
  }
#if 0
  // Access rho
  edm::Handle<double> rho;
  found = iEvent.getByToken(rhoToken_, rho);
  if (found)
  ev.rho = *rho;
#endif
  // Access rhoNeutral
  edm::Handle<double> rhoNeutral;
  found = iEvent.getByToken(rhoNeutralToken_, rhoNeutral);
  if (found) ev.rhoNeutral = *rhoNeutral;
  
  // Vertex Container
  edm::Handle<reco::VertexCollection> spVertices;
  found = iEvent.getByToken(selectedVertexToken_, spVertices);
  if (found) ev.nvtx = spVertices->size();

  list_->push_back(ev);
}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EventBlock);
