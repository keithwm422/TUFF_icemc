// arguments the function needs: list in conv file in github TUFF_icemc
#ifdef ANITA_UTIL_EXISTS
void Anita::readTUFFResponseDigitizer(Settings *settings1, long_int time){
// Linda thinks the if statements should go in ChanTrigger.C
//The read response function I write should load the correct response, then either return that response or convolve it?
  TString indir;

  if(settings1->WHICH==10)
  {

    if(time <= config_a_time) // config A //folder notches_260_0_0
    {
       indir= "notches_260_0_0";
    }
    else if(time <= config_a_time) // config B notches_260_375_0
    {
       indir= "notches_260_375_0";
    }
    else if(time <= config_a_time) // config C notches_260_0_460
    {
       indir= "notches_260_0_460";
    }
    else if(time <= config_a_time) // config G notches_260_385_0
    {
       indir= "notches_260_385_0";
    }
    else if(time <= config_a_time) // config J notches_250_375_0
    {
       indir= "notches_250_375_0";
    }
    else if(time <= config_a_time) // config O notches_260_365_0
    {
       indir= "notches_260_365_0";
    }
    else if(time <= config_a_time) // config P notches_260_375_460
    {
       indir= "notches_260_375_460";
    }
    else
    {
       continue;
    }
  else
  {
    continue;
  }
//keith added file outdir declared here for testing
  //Add if statements to compare the time given to this fucntion with the time for which configuration is on.

//      mkdir(Form("data/responses/TUFFs/%s", outdir), 0777); from andrew code
  string filenameH;
  string filenameV;
  if(ant < 10)
        {
           filenameH = Form("%s/data/%s/0%d%cH.imp",getenv("ICEMC_BUILD_DIR"), indir, ant, tmb);
           filenameV = Form("%s/data/%s/0%d%cV.imp",getenv("ICEMC_BUILD_DIR"), indir, ant, tmb);
        }
        else
        {
           filenameH = Form("%s/data/%s/%d%cH.imp",getenv("ICEMC_BUILD_DIR"), indir, ant, tmb);
           filenameV = Form("%s/data/%s/%d%cH.imp",getenv("ICEMC_BUILD_DIR"), indir, ant, tmb);
        }
  TGraph graphTUFFH(filenameH);
  TGraph graphTUFFV(filenameV);
  
  // input in time domain. FFT. check if analysis waveform is included? should fucntion return the TGraph or the waveform?
  AnalysisWaveform awfTUFFH(graphTUFFH.GetN(), graphTUFFH.GetY(), graphTUFFH.GetX()[1] - graphTUFFH.GetX()[0], 0);
  (void) awfTUFFH.freq();
  // 


  }










//Below this is for the older function for reference when coding

/*------------------------------------------------------------------*/
  // Set deltaT to be used in the convolution
  deltaT = 1/(2.6*16);
  string graphNames[2][3][16];
  string fileName;
  double norm=1;
 
  // For ANITA-2 we have 1 impulse response for VPOL and 1 for HPOL
  // For ANITA-3 we have 3 impulse responses (Top, Middle, Bottom ring) for VPOL and 3 for HPOL.
  // Set Graph names for ANITA-2 and ANITA-3
  if (settings1->WHICH==8){
    fileName = ICEMC_DATA_DIR+"/sumPicoImpulse.root";
    
    for (int iring=0;iring<3;iring++){
      for (int iphi=1;iphi<17;iphi++){
	graphNames[0][iring][iphi]="grImpRespV";
	graphNames[1][iring][iphi]="grImpRespH";
      }
    }
    //Now need to scale our impulse response from unit areas to the area of kronecker-delta (i.e dt)
    norm*=0.1;
  } else if(settings1->WHICH==9 || settings1->WHICH==10){

    fileName = ICEMC_DATA_DIR+"/Anita3_ImpulseResponseDigitizer.root";

    string spol[2] ={"V", "H"};
    string sring[3]={"T", "M", "B"};
    
    for (int ipol=0;ipol<2;ipol++){
      for (int iring=0;iring<3;iring++){
	for (int iphi=0;iphi<16;iphi++){
	  graphNames[ipol][iring][iphi] = Form("g%02d%s%s", iphi+1, sring[iring].c_str(), spol[ipol].c_str() ) ;
	}
      }
    }

    // // Impulse response already accounts for trigger/digitizer splitter
    // norm *= sqrt(2);

  }

  // Read in input file
  TFile fImpulse(fileName.c_str());
  
  if(!fImpulse.IsOpen()) {
    std::cerr << "Couldn't read siganl chain impulse response from " << fileName << "\n";
    exit(0);
  } else {

    for (int ipol=0;ipol<2;ipol++){
      for (int iring=0;iring<3;iring++){
	for (int iphi=0;iphi<16;iphi++){
	  // Read graph
	  TGraph *grTemp = (TGraph*) fImpulse.Get(graphNames[ipol][iring][iphi].c_str());
	  if(!grTemp) {
	    std::cerr << "Couldn't read signal chain impulse response" << graphNames[ipol][iring][iphi] << " from file " << fileName << "\n";
	    exit(0);
	  }
	  // Interpolate to high sampling rate that will be used for the convolution
	  TGraph *grInt = Tools::getInterpolatedGraph(grTemp,deltaT); 
	  Int_t nPoints  = grInt->GetN();
	  Double_t *newx = grInt->GetX();
	  Double_t *newy = grInt->GetY();
	  // Normalise
	  for (int i=0;i<nPoints;i++){
	    newy[i]=newy[i]*norm;
	    // change time axis from ns to s
	    newx[i]=newx[i]*1E-9;
	  }
	  // Pave to 0
	  int paveNum = 8533;
	  grTemp = new TGraph(nPoints,  newx, newy);
	  
	  fSignalChainResponseDigitizer[ipol][iring][iphi] = new RFSignal(FFTtools::padWaveToLength(grTemp, paveNum));
	  
	  delete grInt;
	  delete grTemp;

	  TGraph *gDig  = fSignalChainResponseDigitizer[ipol][iring][iphi]->getFreqMagGraph();
	  for(int i=0;i<numFreqs;i++) {
	    fSignalChainResponseDigitizerFreqDomain[ipol][iring][iphi][i]  = gDig->Eval(freqs[i]*1e6);
	    // cout <<  i <<  " " << ipol << " " << iring << " " << iphi << " " << freqs[i] << " " << fSignalChainResponseDigitizerFreqDomain[ipol][iring][iphi][i]<< endl;
	  }
	  delete gDig;
	  
	}
      }
    }
  }

}

