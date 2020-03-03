#define MZ 91.1876
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
using namespace std;

class cut{
	private:
	/*data to cut*/
	TChain *data;
	/*tree to store information*/
	TTree *tr;
	/*file to store the tree*/
	TFile *f;
	/*variables read from data*/

        vector<int>* svs_nTrks;
	vector<float>* svs_anglePV;
	vector<float>* svs_distXYval;
	vector<float>* svs_dist3Dsig;

	int isData;
	int run;
	int evt;
	float purewgt;
	int nVert;
	float evt_scale1fb;

	
	int nVlep;
	int nLlep;

	vector<float> *lep_MVA;
	vector<int> *lep_pass_VVV_fo;
	vector<int> *lep_pass_VVV_3l_fo;
	vector<float> *lep_Iso;
	vector<int> *lep_pdgId;
	vector<LorentzVector> *jet_p4;
	vector<LorentzVector> *lep_p4;
	int nb;
	int njet30;

	int nisoTrack_mt2_cleaned_VVV_cutbased_veto;
	float met_pt;
	float MTmax;
	float MjjL;
	float DetajjL;

	int firstgoodvertex;
        int Flag_AllEventFilters;
        int vetophoton;
        int evt_passgoodrunlist;

	/*variables saved in tree*/
	/*other variables*/
        TH1F *hCut;//histogram for number of events pass every cut	
	bool is2jet;
	vector<long long> checkDuplicates;
	vector<int> nSS,n3l;	
	/*fucntion for cuts applied on data*/
	bool preSelectionCut();
	bool DuplicateCut();
	bool lepNumberCut();
	bool jetNumberCut();
	bool isoTrackCut();
      	bool mSFOSCut();
	bool bjetCut();
	bool mllCut();
        bool mjjLCut();
        bool DetajjLCut();
	bool DRljminCut();
	bool pTmissCut();
	bool mTmaxCut();

	/*other function*/
	void storeInfo();//function used to store info in tree
	float getRawMVA(float notraw);
	public:
	cut();
	~cut();
	bool initialize();
	bool execute();
	bool finalize();
};

cut::cut(){
}

cut::~cut(){
}

bool cut::initialize() {
	/*initialize the data for cut*/
	data=new TChain("t_ss");
	data->Add("/home/yulunmiao/Documents/CMS_data/WZ/data/*");
	/*initialize the variable read from data*/
	svs_nTrks=0;
        svs_anglePV=0;
        svs_distXYval=0;
        svs_dist3Dsig=0;
	lep_pass_VVV_fo=0;
	lep_pass_VVV_3l_fo=0;
	lep_pdgId=0;
	jet_p4=0;
	lep_p4=0;
	lep_Iso=0;
	lep_MVA=0;

	data->SetBranchAddress("isData",&isData);
	data->SetBranchAddress("evt",&evt);
	data->SetBranchAddress("run",&run);

	data->SetBranchAddress("evt_scale1fb",&evt_scale1fb);
	data->SetBranchAddress("purewgt",&purewgt);
        data->SetBranchAddress("nVert",&nVert);

        data->SetBranchAddress("firstgoodvertex",&firstgoodvertex);
        data->SetBranchAddress("Flag_AllEventFilters",&Flag_AllEventFilters);
        data->SetBranchAddress("vetophoton",&vetophoton);
        data->SetBranchAddress("evt_passgoodrunlist",&evt_passgoodrunlist);

        data->SetBranchAddress("nVlep",&nVlep);
        data->SetBranchAddress("nLlep",&nLlep);

        data->SetBranchAddress("svs_nTrks",&svs_nTrks);
        data->SetBranchAddress("svs_anglePV",&svs_anglePV);
        data->SetBranchAddress("svs_distXYval",&svs_distXYval);
        data->SetBranchAddress("svs_dist3Dsig",&svs_dist3Dsig);

        data->SetBranchAddress("lep_pass_VVV_fo",&lep_pass_VVV_fo);
        data->SetBranchAddress("lep_pass_VVV_3l_fo",&lep_pass_VVV_3l_fo);
        data->SetBranchAddress("lep_MVA",&lep_MVA);
        data->SetBranchAddress("lep_pdgId",&lep_pdgId);
        data->SetBranchAddress("lep_relIso03EALep",&lep_Iso);
        data->SetBranchAddress("jets_p4",&jet_p4);
        data->SetBranchAddress("lep_p4",&lep_p4);
        data->SetBranchAddress("nisoTrack_mt2_cleaned_VVV_cutbased_veto",&nisoTrack_mt2_cleaned_VVV_cutbased_veto);
        data->SetBranchAddress("nj30",&njet30);

        data->SetBranchAddress("nb",&nb);
        data->SetBranchAddress("MjjL",&MjjL);
        data->SetBranchAddress("DetajjL",&DetajjL);
        data->SetBranchAddress("met_pt",&met_pt);
        data->SetBranchAddress("MTmax",&MTmax);
        /*initialize the file to store tree*/
        f=TFile::Open("data_result.root","recreate");
        /*initialize the tree to store data*/
        tr=new TTree("result","result");
        /*initialize the variables saved in tree*/

        tr->Branch("purewgt",&purewgt,"purewgt/F");
        tr->Branch("nVert",&nVert,"nVert/I");
	tr->Branch("scale",&evt_scale1fb,"scale/F");

        /*initial other variables*/
        hCut=new TH1F("hCut","hCut",12,0,12);
	checkDuplicates.clear();
	return true;
}

bool cut::execute(){
	int total=data->GetEntries();
	cout<<"total #="<<total<<endl;
	for(int i=0;i<data->GetEntries();i++){
		if((i+1)%1000==0) cout<<(i+1)<<"/"<<total<<endl;
		data->GetEntry(i);
		if(!preSelectionCut()) continue;
		if(!DuplicateCut()) continue;
		hCut->AddBinContent(1);	
		if(!lepNumberCut()) continue;
		hCut->AddBinContent(2);
		if(!isoTrackCut()) continue;
		hCut->AddBinContent(3);
		if(!mSFOSCut()) continue;
		hCut->AddBinContent(4);
		if(!jetNumberCut()) continue;
		hCut->AddBinContent(5);
		if(!bjetCut()) continue;
		hCut->AddBinContent(6);
		if(!mllCut()) continue;
		hCut->AddBinContent(7);
		if(!pTmissCut()) continue;
		hCut->AddBinContent(8);
		storeInfo();
	}
	return true;
}

bool cut::finalize(){
	/*store the tree in file*/
	tr->Write();
	/*store other variables in file*/
	hCut->Write();
	f->Close();
	return true;
}

float cut::getRawMVA(float notraw){
    	if (notraw >  1.0-1.e-6) notraw =  1.0-1.e-6; // protect against inf, -inf due to FP rounding issues
   	if (notraw < -1.0+1.e-6) notraw = -1.0+1.e-6;
   	return -0.5*log((2.0/(notraw+1))-1.0);
}

void cut::storeInfo(){
	tr->Fill();
}

bool cut::preSelectionCut(){
	bool presel = true;
        presel &= (firstgoodvertex      == 0);
        presel &= (Flag_AllEventFilters >  0);
        presel &= (vetophoton           == 0);
        presel &= (evt_passgoodrunlist  >  0);
        presel &= (nVlep                >= 2);
        presel &= (nLlep                >= 2);
	return presel;
}

bool cut::DuplicateCut(){
	long long RUNPREF=1000*1000;
	long long dupCheck = run*RUNPREF + evt;
        for (unsigned int uid = 0; uid < checkDuplicates.size(); uid++){
        	if (checkDuplicates[uid] == dupCheck){
			return false;
		}
	}
	checkDuplicates.push_back(dupCheck);
	return true;
}

bool cut::lepNumberCut(){
       	nSS.clear();
	n3l.clear();
	
	for(int i=0;i<lep_pdgId->size();i++){
		if(fabs(lep_pdgId->at(i))==11){
                	if(getRawMVA(fabs(lep_MVA->at(0)))>7&&lep_p4->at(i).pt()>25&&lep_pass_VVV_fo->at(i)&&lep_Iso->at(i)<0.05) nSS.push_back(i);
			else if(getRawMVA(fabs(lep_MVA->at(0)))>7&&lep_p4->at(i).pt()>20&&lep_pass_VVV_3l_fo->at(i)&&lep_Iso->at(i)<0.10) n3l.push_back(i);
		}

                if(fabs(lep_pdgId->at(i))==13){
                        if(getRawMVA(fabs(lep_MVA->at(0)))>7&&lep_p4->at(i).pt()>25&&lep_pass_VVV_fo->at(i)&&lep_Iso->at(i)<0.04) nSS.push_back(i);
			else if(getRawMVA(fabs(lep_MVA->at(0)))>7&&lep_p4->at(i).pt()>20&&lep_pass_VVV_fo->at(i)&&lep_Iso->at(i)<0.15) n3l.push_back(i);

                }
        }

	if(!(nSS.size()==2&&n3l.size()==1)) return false;
	if(lep_pdgId->at(nSS.at(0))*lep_pdgId->at(nSS.at(1))<0) return true;
	return true;
	
}

bool cut::isoTrackCut(){
	if(nisoTrack_mt2_cleaned_VVV_cutbased_veto==0) return true;
	return false;
}

bool cut::mSFOSCut(){
	double mSFOS;
	for(int i=0;i<nSS.size();i++){
		if(lep_pdgId->at(nSS.at(i))+lep_pdgId->at(n3l.at(0))==0){
			mSFOS=(lep_p4->at(nSS.at(i))+lep_p4->at(n3l.at(0))).M();
			if(fabs(mSFOS-MZ)<20.) return true;
		}
	}
	return false;
}

bool cut::jetNumberCut(){
	if(njet30>=2){ 
		is2jet=true;
	       	return true;
	}
	if(njet30==1){
		is2jet=false;
		return true;
	}
	return false;
}
bool cut::bjetCut(){
	int softb=0;

	if(nb!=0) return false;
	for (unsigned int i = 0; i<svs_nTrks->size(); i++)
        {
            bool passID = true;
            if (svs_nTrks->at(i) < 3)            passID = false;
            if (svs_distXYval->at(i) >= 3.0)     passID = false;
            if (svs_dist3Dsig->at(i) <= 4.0)     passID = false;
            if (cos(svs_anglePV->at(i)) <= 0.98) passID = false;
            if (passID) softb++;
        }
	
	if(softb!=0) return false;
	return true;
}

bool cut::mllCut(){
	float Mll;
	Mll=(lep_p4->at(nSS.at(0))+lep_p4->at(nSS.at(1))).M();
	if(!(Mll>20)) return false;
	if(lep_pdgId->at(nSS.at(0))*lep_pdgId->at(nSS.at(1))==121){
		if(!(fabs(Mll-MZ)>20)) return false;
	}
	return true;
}

bool cut::pTmissCut(){
	if(!is2jet) return true;
	if(met_pt>45) return true;
	return false;
}

bool cut::mjjLCut(){
	if(!is2jet) return true;
        if(MjjL<500) return true;
        return false;
}

bool cut::DetajjLCut(){
        if(fabs(DetajjL)<2.5) return true;
        return false;
}


bool cut::DRljminCut(){
	float DRljmin=9999;
	float DRljtemp;
	if(is2jet) return true;
	for(int i=0;i<lep_p4->size();i++){
		DRljtemp=fabs(ROOT::Math::VectorUtil::DeltaR(lep_p4->at(i), jet_p4->at(0)));
		if(DRljtemp<DRljmin) DRljmin=DRljtemp;
	}
	if(DRljmin<1.5) return true;
	return false;
}

bool cut::mTmaxCut(){
	if(is2jet){
		if(lep_pdgId->at(nSS.at(0))*lep_pdgId->at(nSS.at(1))==169) return true;
		if(MTmax>90) return true;
	}
	else{
		if(MTmax>90) return true;
	}
	return false;
}

int cutForWZ(){
	cut c;
	cout<<"----------Initializing------------"<<endl;
	c.initialize();
	cout<<"-----------Executing--------------"<<endl;
	c.execute();
	cout<<"-----------Finalizing-------------"<<endl;
	c.finalize();
	return 0;
}
