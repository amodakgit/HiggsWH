#ifndef __AnalysisSpace_TreeMaker_PhysicsObjects_h
#define __AnalysisSpace_TreeMaker_PhysicsObjects_h

#include <vector>
#include <map>
#include <string>

#include "TObject.h"

namespace vhtm {
  class Candidate: public TObject {
  public:
    Candidate();
    Candidate(float pt, float eta, float phi);
    virtual ~Candidate() {}

    float pt;
    float eta;
    float phi;

    ClassDef(Candidate,1)
  };
  class Event: public TObject {
  public:
    Event();
    ~Event() {}
  
    unsigned long int run;
    unsigned long int event;
    unsigned long int lumis;
    unsigned long int bunch;
    unsigned long int orbit;
    double time;
    bool isdata;
  
    bool isPhysDeclared;
    bool isBPTX0;
    bool isBSCMinBias;
    bool isBSCBeamHalo;
    bool isPrimaryVertex;
    //bool isBeamScraping;
    bool passHBHENoiseFilter;
  
    double rho;
    double rhoNeutral;
    int nvtx; 

    std::vector<int> nPU;
    std::vector<int> bunchCrossing;
    std::vector<int> trueNInt;
  
    ClassDef(Event,1)
  };
  class GenEvent: public TObject {
  public:
    GenEvent();
    ~GenEvent() {}
  
    double weightevt;
    unsigned int processID;
    double ptHat;
    std::vector<double> pdfWeights;
  
    ClassDef(GenEvent,1)
  };
  
  class Electron: public TObject {
  public:
    Electron();
    ~Electron() {}
    double eta;
    double phi;
    double pt;
    bool ecalDriven;
    bool hasGsfTrack;
    double trackPt;
    double energy;
    double caloEnergy;
    int charge;
    int pixHits;
    int trkHits;
    int nValidHits;
    double trkD0;
    double trkDz;
    // ID variables
    double hoe;
    double eop;
    double sigmaEtaEta;
    double sigmaIEtaIEta;
    double deltaPhiTrkSC;
    double deltaEtaTrkSC;
    int classif;
  
    // Vertex
    double vx;
    double vy;
    double vz;

    // SC associated with electron
    double scEn;
    double scEta;
    double scPhi;
    double scET;
    double scRawEnergy;
  
    // Vertex association variables
    double dxyPV;
    double dzPV;
    double vtxDist3D;
    int vtxIndex;
    double vtxDistZ;
    float relIso;
    float pfRelIso;
    // PFlow isolation variable
    float chargedHadronIso;
    float neutralHadronIso;
    float photonIso;

    float sumChargedHadronPt;
    float sumPUPt;

    int missingHits;

    double dB;
    int nBrems;
    float fbrem;
  
    float hasMatchedConv;
    bool mvaPreselection;
    bool isTriggerElectron;
    int fidFlag;
    std::map<std::string, float> idmap;
    int selbit;
    int isPassMedium_MVAIDWP90;
    int isPassTight_MVAIDWP80;
    float mvaId;
    float mvaId_def;

    ClassDef(Electron, 1)
  };
  class GenParticle: public TObject {
  public:
    GenParticle();
    ~GenParticle() {}
  
    double eta;
    double phi;
    double p;
    double px;
    double py;
    double pz;
    double pt;
    double energy;
    int pdgId;
    double vx;
    double vy;
    double vz;
    int status;
    double charge;
    int numDaught;
    int numMother;
    int motherIndex;
    std::vector<int> motherIndices;
    std::vector<int> daughtIndices;
  
    ClassDef(GenParticle,1)
  };
  class GenJet: public TObject {
  public:
    GenJet();
    ~GenJet() {}
  
    double eta;
    double phi;
    double p;
    double pt;
    double energy;
    double emf;
    double hadf;
  
    ClassDef(GenJet,1)
  };
  class MET: public TObject {
  public:
    MET();
    ~MET() {}
  
    double met;
    double metphi;
    double sumet;
    double metuncorr;
    double metphiuncorr;
    double sumetuncorr;
  
    ClassDef(MET,1)
  };
  class Tau: public TObject {
  public:
    Tau();
    ~Tau() {}
  
    double eta;
    double phi;
    double pt;
    double energy;
    int charge;
    double mass;
  
    double leadTrkPt;
    double dxyPV;
    double dzPV;
    int vtxIndex;
    double vtxDxy;
    double vtxDz;

    // Leading particle pT
    double leadChargedParticlePt;
    double leadNeutralParticlePt;
    double leadParticlePt;

    std::vector<vhtm::Candidate> sigChHadList;
    std::vector<vhtm::Candidate> sigNeHadList;
    std::vector<vhtm::Candidate> sigGammaList;
    std::vector<vhtm::Candidate> isoChHadList;
    std::vector<vhtm::Candidate> isoNeHadList;
    std::vector<vhtm::Candidate> isoGammaList;

    float ptSumChargedHadronsIsoCone;
    float ptSumNeutralHadronsIsoCone;
    float ptSumPhotonsIsoCone;

     // tau id. discriminators
     float decayModeFinding;
     float decayModeFindingNewDMs;
     float decayModeFindingOldDMs;

     // discriminators against electrons/muons
     float againstMuonLoose;
     float againstMuonMedium;
     float againstMuonTight;

     float againstMuonLoose3;
     float againstMuonTight3;

     float againstElectronLoose;
     float againstElectronMedium;
     float againstElectronTight;
     //float againstElectronMVA;
  
     float againstElectronVLooseMVA6;
     float againstElectronLooseMVA6;
     float againstElectronMediumMVA6;
     float againstElectronTightMVA6;
     float againstElectronVTightMVA6;

     float byLooseCombinedIsolationDeltaBetaCorr3Hits;
     float byMediumCombinedIsolationDeltaBetaCorr3Hits;
     float byTightCombinedIsolationDeltaBetaCorr3Hits;
     float byCombinedIsolationDeltaBetaCorrRaw3Hits;
     float byVLooseIsolationMVArun2v1DBoldDMwLT;
     float byLooseIsolationMVArun2v1DBoldDMwLT;
     float byMediumIsolationMVArun2v1DBoldDMwLT;
     float byTightIsolationMVArun2v1DBoldDMwLT;
     float byVTightIsolationMVArun2v1DBoldDMwLT;
     float chargedIsoPtSum;
     float neutralIsoPtSum;
     float puCorrPtSum;
#if 0
     // kinematic variables for PFJet associated to PFTau
    double jetPt;
    double jetEta;
    double jetPhi;
  
     float emFraction;
#endif
    double vx;
    double vy;
    double vz;
  
    double zvertex;
#if 0
    double ltsipt;
#endif  
       int selbit;
  
    ClassDef(Tau, 1)
  };
  class Muon: public TObject {
  public:
    Muon();
    ~Muon() {}
    bool isTrackerMuon;
    bool isPFMuon;
    double eta;
    double phi;
    double pt;

    double p;
    double energy;
    int charge;
    double trkD0;

    double trkDz;

    double globalChi2;
    float trkIso;
    float ecalIso;
    float hcalIso;
    float hoIso;
    float chargedParticleIso;
    float chargedHadronIso;
    float neutralHadronIso;
    float photonIso;
    float puIso;
    float pfRelIsoDB03;
    float pfRelIsoDB04;
    int passID;
    double dxyPV;
    double dzPV;
    double vtxDist3D;
    int vtxIndex;
    double vtxDistZ;
    int pixHits;
    int trkHits;
    int muoHits;
    int matches;
    int trackerLayersWithMeasurement;

    double vx;
    double vy;
    double vz;

    double dB; // PV2D
    double edB; // PV2D
  
    // UW Recommendation
    bool isGlobalMuonPromptTight;
    bool isAllArbitrated;
    int nChambers;
    int nMatches;
    int nMatchedStations;

    unsigned int stationMask;
    unsigned int stationGapMaskDistance;
    unsigned int stationGapMaskPull;

    bool muonMedID;
    bool isLooseMuon;
    bool isMediumMuon;
    bool isTightMuon;
    int selbit;

    ClassDef(Muon, 1)
  };
  class Jet: public TObject {
  public:
    Jet();
    ~Jet() {}
    double eta;
    double phi;
    double pt;
    double pt_raw;
    double energy;
    double energy_raw;
    double jecUnc;
    double resJEC;
    int partonFlavour;

    float chargedEmEnergyFraction;
    float chargedHadronEnergyFraction;
    float chargedMuEnergyFraction;
    float electronEnergyFraction;
    float muonEnergyFraction;
    float neutralEmEnergyFraction;
    float neutralHadronEnergyFraction;
    float photonEnergyFraction;
    int chargedHadronMultiplicity;
    int chargedMultiplicity;
    int electronMultiplicity;
    int muonMultiplicity;
    int neutralHadronMultiplicity;
    int neutralMultiplicity;
    int photonMultiplicity;
    int nConstituents;

    //float simpleSecondaryVertexHighEffBTag;
    //float simpleSecondaryVertexHighPurBTag;

    //float combinedSecondaryVertexBTag;
    //double combinedSecondaryVertexMVABTag;
    float combinedInclusiveSecondaryVertexV2BJetTags;
    float pfCombinedInclusiveSecondaryVertexV2BJetTags;
    //double combinedMVABTag;
    std::map<std::string, float> discrimap;

    int passLooseID;
    int passTightID;
  
    int selbit;

    ClassDef(Jet, 1)
  };
  class Vertex: public TObject {
  public:
    Vertex();
    ~Vertex() {}
  
    double x;
    double y;
    double z;
    double xErr;
    double yErr;
    double zErr;
    double rho;
    double chi2;
    double ndf;
    int ntracks;
    int ntracksw05;
    bool isfake;
    bool isvalid;
    double sumPt; // vector sum
  
    int selbit;

    ClassDef(Vertex, 1)
  };
  class GenMET: public TObject {
  public:
    GenMET();
    ~GenMET() {}
  
    double met;
    double metphi;
    double sumet;
  
    ClassDef(GenMET, 1)
  };
  class TriggerObject: public TObject {
  public:
    TriggerObject();
    ~TriggerObject() {}
  
    double energy;
    double pt;
    double eta;
    double phi;
  
    std::map<std::string, unsigned int> pathList;
  
    ClassDef(TriggerObject, 1)
  };
}
#endif
