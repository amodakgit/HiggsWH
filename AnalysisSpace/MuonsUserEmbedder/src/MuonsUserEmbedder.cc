// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"
#include "DataFormats/MuonReco/interface/MuonIsolation.h"
#include "DataFormats/MuonReco/interface/MuonPFIsolation.h"

using namespace std;
using namespace reco;
//
// class declaration
//

class MuonsUserEmbedder : public edm::EDProducer {
   public:
      explicit MuonsUserEmbedder(const edm::ParameterSet&);
      ~MuonsUserEmbedder();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   enum {
     kMaxMuon_ = 100
   };

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------

      bool bsCorr_;
      bool trigMode_;
      const edm::InputTag bsTag_;
      const edm::InputTag vertexTag_;
      const edm::InputTag muonTag_;

      const edm::EDGetTokenT<reco::BeamSpot> bsToken_;
      const edm::EDGetTokenT<reco::VertexCollection> vertexToken_;
      const edm::EDGetTokenT<pat::MuonCollection> muonToken_;

};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
MuonsUserEmbedder::MuonsUserEmbedder(const edm::ParameterSet& iConfig) :

  bsCorr_(iConfig.getUntrackedParameter<bool>("beamSpotCorr", false)),
  trigMode_(iConfig.getUntrackedParameter<bool>("useTrigMode", false)),
  bsTag_(iConfig.getUntrackedParameter<edm::InputTag>("offlineBeamSpot", edm::InputTag("offlineBeamSpot"))),
  vertexTag_(iConfig.getUntrackedParameter<edm::InputTag>("vertexSrc", edm::InputTag("goodOfflinePrimaryVertices"))),
  muonTag_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc", edm::InputTag("selectedPatMuons"))),
  bsToken_(consumes<reco::BeamSpot>(bsTag_)),
  vertexToken_(consumes<reco::VertexCollection>(vertexTag_)),
  muonToken_(consumes<pat::MuonCollection>(muonTag_))
{
  produces<pat::MuonCollection>("");
}

MuonsUserEmbedder::~MuonsUserEmbedder()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
MuonsUserEmbedder::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  std::auto_ptr< pat::MuonCollection > muonsUserEmbeddedColl( new pat::MuonCollection() ) ;

  edm::Handle<pat::MuonCollection> muons;
  bool found = iEvent.getByToken(muonToken_, muons);

  if (found && muons.isValid()) {
    edm::Handle<reco::BeamSpot> beamSpot;
    if (bsCorr_) iEvent.getByToken(bsToken_, beamSpot);

    edm::Handle<reco::VertexCollection> primaryVertices;
    iEvent.getByToken(vertexToken_, primaryVertices);

    for (const pat::Muon& v: *muons) {
      //if (v.size() >= kMaxElectron_) break;

      pat::Muon userMuon (v);
      
      // PF Isolation
      float cpIso = v.pfIsolationR04().sumChargedParticlePt;      
      float chIso = v.pfIsolationR04().sumChargedHadronPt;
      float nhIso = v.pfIsolationR04().sumNeutralHadronEt;
      float phoIso = v.pfIsolationR04().sumPhotonEt;
      float puIso = v.pfIsolationR04().sumPUPt;
      float pfRelIsoDB04 = (chIso + std::max(0.0, nhIso + phoIso - 0.5*puIso))/v.pt();
      float pfRelIsoDB03 = (v.pfIsolationR03().sumChargedHadronPt + 
                            std::max(0.0, v.pfIsolationR03().sumNeutralHadronEt + 
                                          v.pfIsolationR03().sumPhotonEt - 
                                          0.5*v.pfIsolationR03().sumPUPt)
                           )/v.pt();
      userMuon.addUserFloat("chargedParticleIso", cpIso);
      userMuon.addUserFloat("chargedHadronIso", chIso);
      userMuon.addUserFloat("neutralHadronIso", nhIso);
      userMuon.addUserFloat("photonIso", phoIso);
      userMuon.addUserFloat("puIso", puIso);
      userMuon.addUserFloat("pfRelIsoDB03", pfRelIsoDB03);
      userMuon.addUserFloat("pfRelIsoDB04", pfRelIsoDB04);

      muonsUserEmbeddedColl->push_back(userMuon);
    }
  }

  iEvent.put( muonsUserEmbeddedColl );
  return;
}

// ------------ method called once each job just before starting event loop  ------------
void 
MuonsUserEmbedder::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MuonsUserEmbedder::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
MuonsUserEmbedder::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
MuonsUserEmbedder::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
MuonsUserEmbedder::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
MuonsUserEmbedder::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MuonsUserEmbedder::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MuonsUserEmbedder);
