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

        vector<float>* svs_nTrks;
	vector<float>* svs_anglePV;
	vector<float>* svs_distXYval;
	vector<float>* svs_dist3Dsig;
	float evt_scale1fb;
	int nVlep;
	int nTlep;
	int nLlep;
	vector<float> *lep_MVA;
	vector<int> *lep_pass_VVV_fo;
	vector<float> *lep_Iso;
	vector<int> *lep_pdgId;
	vector<LorentzVector> *jet_p4;
	vector<LorentzVector> *lep_p4;
	float Mjj;
	int nb;
	float met_pt;
	float MjjL;
	float DetajjL;
	float MTmax;

	int firstgoodvertex;
        int Flag_AllEventFilters;
        int vetophoton;
        int evt_passgoodrunlist;

	/*variables saved in tree*/
	int lep1_pdgId;
	float lep1_px;
	float lep1_py;
	float lep1_pz;
	float lep1_E;
	float lep1_pT;

	int lep2_pdgId;
	float lep2_px;
        float lep2_py;
        float lep2_pz;
        float lep2_E;
        float lep2_pT;

        float jet1_px;
	float jet1_py;
	float jet1_pz;
	float jet1_E;
	float jet1_pT;

	float jet2_px;
        float jet2_py;
        float jet2_pz;
        float jet2_E;
        float jet2_pT;

	float Mjjc;

	/*other variables*/
        TH1I *hCut;//histogram for number of events pass every cut	
	LorentzVector jet1p4,jet2p4;
	TH1F *hCount;
	/*fucntion for cuts applied on data*/
	bool preSelectionCut();
	bool lepNumberCut();
	bool jetNumberCut();
	bool bjetCut();
	bool mllCut();
	bool pTmissCut();
	bool mjjLCut();
	bool detajjLCut();
	bool mTmaxCut();

	/*other function*/
	void storeInfo();//function used to store info in tree
	void mjjSelection();//function used to select the dijet with minimum dR
	void counting();
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

bool cut::initialize(){
	/*initialize the data for cut*/
	data=new TChain("t_ss");
	data->Add("/home/yulunmiao/Documents/CMS_data/WWWto2lFatJet/data/www_amcatnlo_dilepfilter_1.root");
	/*initialize the variable read from data*/
	svs_nTrks=0;
        svs_anglePV=0;
        svs_distXYval=0;
        svs_dist3Dsig=0;
	lep_pass_VVV_fo=0;
	lep_pdgId=0;
	jet_p4=0;
	lep_p4=0;
	lep_Iso=0;
	lep_MVA=0;

        data->SetBranchAddress("firstgoodvertex",&firstgoodvertex);
	data->SetBranchAddress("Flag_AllEventFilters",&Flag_AllEventFilters);
        data->SetBranchAddress("vetophoton",&vetophoton);
        data->SetBranchAddress("evt_passgoodrunlist",&evt_passgoodrunlist);


	data->SetBranchAddress("svs_nTrks",&svs_nTrks);
	data->SetBranchAddress("svs_anglePV",&svs_anglePV);
        data->SetBranchAddress("svs_distXYval",&svs_distXYval);
        data->SetBranchAddress("svs_dist3Dsig",&svs_dist3Dsig);

	data->SetBranchAddress("lep_pass_VVV_fo",&lep_pass_VVV_fo);
	data->SetBranchAddress("nVlep",&nVlep);
	data->SetBranchAddress("nTlep",&nTlep);
	data->SetBranchAddress("nLlep",&nLlep);
	data->SetBranchAddress("lep_MVA",&lep_MVA);
	data->SetBranchAddress("evt_scale1fb",&evt_scale1fb);
	data->SetBranchAddress("lep_pdgId",&lep_pdgId);
	data->SetBranchAddress("lep_relIso03EALep",&lep_Iso);
        data->SetBranchAddress("jets_p4",&jet_p4);
	data->SetBranchAddress("lep_p4",&lep_p4);
        data->SetBranchAddress("nb",&nb);
	data->SetBranchAddress("met_pt",&met_pt);
        data->SetBranchAddress("MjjL",&MjjL);
	data->SetBranchAddress("Mjj",&Mjj);
	data->SetBranchAddress("DetajjL",&DetajjL);
	data->SetBranchAddress("MTmax",&MTmax);
	/*initialize the file to store tree*/
	f=TFile::Open("result.root","recreate");
	/*initialize the tree to store data*/
	tr=new TTree("result","result");
	/*initialize the variables saved in tree*/

	tr->Branch("scale",&evt_scale1fb,"scale/F");

	tr->Branch("lep1_pdgId",&lep1_pdgId,"lep1_pdgId/I");
        tr->Branch("lep1_px",&lep1_px,"lep1_px/F");
	tr->Branch("lep1_py",&lep1_py,"lep1_py/F");
        tr->Branch("lep1_pz",&lep1_pz,"lep1_pz/F");
        tr->Branch("lep1_E",&lep1_E,"lep1_E/F");
	tr->Branch("lep1_pT",&lep1_pT,"lep1_pT/F");

	tr->Branch("lep2_pdgId",&lep2_pdgId,"lep2_pdgId/I");
	tr->Branch("lep2_px",&lep2_px,"lep2_px/F");
        tr->Branch("lep2_py",&lep2_py,"lep2_py/F");
        tr->Branch("lep2_pz",&lep2_pz,"lep2_pz/F");
        tr->Branch("lep2_E",&lep2_E,"lep2_E/F");
        tr->Branch("lep2_pT",&lep2_pT,"lep2_pT/F");

	tr->Branch("jet1_px",&jet1_px,"jet1_px/F");
        tr->Branch("jet1_py",&jet1_py,"jet1_py/F");
        tr->Branch("jet1_pz",&jet1_pz,"jet1_pz/F");
        tr->Branch("jet1_E",&jet1_E,"jet1_E/F");
        tr->Branch("jet1_pT",&jet1_pT,"jet1_pT/F");

	tr->Branch("jet2_px",&jet2_px,"jet2_px/F");
        tr->Branch("jet2_py",&jet2_py,"jet2_py/F");
        tr->Branch("jet2_pz",&jet2_pz,"jet2_pz/F");
        tr->Branch("jet2_E",&jet2_E,"jet2_E/F");
        tr->Branch("jet2_pT",&jet2_pT,"jet2_pT/F");

	tr->Branch("Mjj",&Mjj,"Mjj/F");
	tr->Branch("Mjjc",&Mjjc,"Mjjc/F");

	//tr->Branch("jet1_p4",&lep1_p4,"jet1_p4/LV");
        //tr->Branch("jet2_p4",&lep2_p4,"jet2_p4/LV");


	/*initial other variables*/
	hCut=new TH1I("hCut","hCut",10,0,10);
	hCount=new TH1F("hCount","hCount",4,0,4);
	return true;

}

bool cut::execute(){
	for(int i=0;i<data->GetEntries();i++){
		data->GetEntry(i);
		if(!preSelectionCut()) continue;
		hCut->AddBinContent(1);	
		if(!lepNumberCut()) continue;
		hCut->AddBinContent(2);
		if(!jetNumberCut()) continue;
		hCut->AddBinContent(3);
		if(!bjetCut()) continue;
		hCut->AddBinContent(4);
		if(!mllCut()) continue;
		hCut->AddBinContent(5);
		if(!mjjLCut()) continue;
		hCut->AddBinContent(6);
		if(!detajjLCut()) continue;
		hCut->AddBinContent(7);
		if(!mTmaxCut()) continue;
		hCut->AddBinContent(8);
		counting();
		mjjSelection();
		storeInfo();
	}
	return true;
}

bool cut::finalize(){
	/*store the tree in file*/
	tr->Write();
	/*store other variables in file*/
	hCut->Write();
	hCount->Write();
	f->Close();
	return true;
}

float cut::getRawMVA(float notraw){
    	if (notraw >  1.0-1.e-6) notraw =  1.0-1.e-6; // protect against inf, -inf due to FP rounding issues
   	if (notraw < -1.0+1.e-6) notraw = -1.0+1.e-6;
   	return -0.5*log((2.0/(notraw+1))-1.0);
}

void cut::storeInfo(){
	lep1_pdgId=lep_pdgId->at(0);
	lep1_px=lep_p4->at(0).Px();
        lep1_py=lep_p4->at(0).Py();
        lep1_pz=lep_p4->at(0).Pz();
	lep1_E=lep_p4->at(0).E();
	lep1_pT=lep_p4->at(0).Pt();
	
	lep2_pdgId=lep_pdgId->at(1);
        lep2_px=lep_p4->at(1).Px();
        lep2_py=lep_p4->at(1).Py();
        lep2_pz=lep_p4->at(1).Pz();
        lep2_E=lep_p4->at(1).E();
        lep2_pT=lep_p4->at(1).Pt();

	jet1_px=jet1p4.Px();
        jet1_py=jet1p4.Py();
	jet1_pz=jet1p4.Pz();
        jet1_E=jet1p4.E();
        jet1_pT=jet1p4.Pt();
	
	jet2_px=jet2p4.Px();
        jet2_py=jet2p4.Py();
        jet2_pz=jet2p4.Pz();
        jet2_E=jet2p4.E();
        jet2_pT=jet2p4.Pt();

	LorentzVector p=jet1p4+jet2p4;
	Mjjc=p.M();


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

bool cut::lepNumberCut(){
	
	int nloose=0,ntight=0;
	
	if(nLlep!=2) return false;
	if(nVlep!=2) return false;
	if(lep_p4->at(0).pt()<25) return false;
	if(lep_p4->at(1).pt()<25) return false;
	if(!((getRawMVA(fabs(lep_MVA->at(0))) > 7) && (getRawMVA(fabs(lep_MVA->at(1))) > 7))) return false;
	for(int i=0;i<lep_p4->size();i++){
		if(lep_Iso->at(i)<0.05&&lep_pass_VVV_fo->at(i)) ntight++;
		if(lep_Iso->at(i)<0.40&&lep_pass_VVV_fo->at(i)) nloose++;
	}
	if(ntight==2 && nloose==2) return true;
	return false;
	
}

bool cut::jetNumberCut(){
	if(jet_p4->size()>=2){
		if(jet_p4->at(0).Pt()>30&&jet_p4->at(1).Pt()>30) return true;
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
	LorentzVector p;
	p=lep_p4->at(0)+lep_p4->at(1);
	if(p.M()>20){
		if(lep_pdgId->at(0)*lep_pdgId->at(1)==121){
	       		if(fabs(p.M()-MZ)>20)	return true;
			else return false;
		}
		return true;
	}
	return false;
}

bool cut::pTmissCut(){
	if(met_pt>45) return true;
	return false;
}

bool cut::mjjLCut(){
	if(MjjL<500) return true;
	return false;
}

bool cut::detajjLCut(){
	if(fabs(DetajjL)<2.5) return true;
	return false;
}

void cut::mjjSelection(){
	float dRmin=9999;
	float dRtemp;
	for(int i=0;i<jet_p4->size();i++){
		if (jet_p4->at(i).Pt()<=30) break;
		for(int j=i+1;j<jet_p4->size();j++){
			if(jet_p4->at(j).Pt()<=30) break;
			if (fabs(jet_p4->at(i).eta()) >= 2.5 || fabs(jet_p4->at(j).eta()) >= 2.5) break;
			dRtemp=ROOT::Math::VectorUtil::DeltaR(jet_p4->at(i),jet_p4->at(j));
			if(dRtemp<dRmin){
				dRmin=dRtemp;
				jet1p4=jet_p4->at(i);
				jet2p4=jet_p4->at(j);
			}
		}
	}
	return;
}

bool cut::mTmaxCut(){
	if(lep_pdgId->at(0)*lep_pdgId->at(1)==169) return true;
	if(MTmax>90) return true;
	return false;
}

void cut::counting(){
	switch(lep_pdgId->at(0)*lep_pdgId->at(1)){
		case 121:hCount->Fill(1,evt_scale1fb*137);break;
		case 143:hCount->Fill(2,evt_scale1fb*137);break;
		case 169:hCount->Fill(3,evt_scale1fb*137);break;
		default:hCount->AddBinContent(4);break;
	}
	return;
}

int cutFor2l2j_debug(){
	cut c;
	cout<<"----------Initializing------------"<<endl;
	c.initialize();
	cout<<"-----------Executing--------------"<<endl;
	c.execute();
	cout<<"----------Finalizeing-------------"<<endl;
	c.finalize();
	return 0;
}
