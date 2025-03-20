default: all

all: Setup bin/MakeWeights bin/ClosureTest bin/GenSpectra

Setup:
	mkdir -p bin
	mkdir -p img
	mkdir -p Weights


bin/MakeWeights: source/MakeWeights.cpp
	g++ source/MakeWeights.cpp -O3 -Iinclude -I$(ProjectBase)/WeightHandler/20250305_DzeroUPC_GptGyWeight/include -I$(ProjectBase)/CommonCode/include -o bin/MakeWeights `root-config \
	--cflags --glibs`

bin/ClosureTest: source/ClosureTest.cpp
	g++ source/ClosureTest.cpp -O3 -Iinclude -I$(ProjectBase)/WeightHandler/20250305_DzeroUPC_GptGyWeight/include -I$(ProjectBase)/CommonCode/include -o bin/ClosureTest `root-config --cflags --glibs`

bin/GenSpectra: source/GenSpectra.cpp
	g++ source/GenSpectra.cpp -O3 -Iinclude -I$(ProjectBase)/WeightHandler/20250305_DzeroUPC_GptGyWeight/include -I$(ProjectBase)/CommonCode/include -o bin/GenSpectra `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

test: bin/MakeWeights
	bin/MakeWeights \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/Data.root \
	--treeName ntReweighting \
	--weightFileName Weights/pt2-5_y-2-2_IsGammaN1.root

	bin/MakeWeights \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/Data.root \
	--treeName ntReweighting \
	--weightFileName Weights/pt2-5_y-2--1_IsGammaN1.root

	bin/MakeWeights \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN1/Data.root \
	--treeName ntReweighting \
	--weightFileName Weights/pt2-5_y-1-0_IsGammaN1.root

	bin/MakeWeights \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN1/Data.root \
	--treeName ntReweighting \
	--weightFileName Weights/pt2-5_y0-1_IsGammaN1.root

	bin/MakeWeights \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN1/Data.root \
	--treeName ntReweighting \
	--weightFileName Weights/pt2-5_y1-2_IsGammaN1.root

	bin/MakeWeights \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/Data.root \
	--treeName ntReweighting \
	--weightFileName Weights/pt2-5_y-2-2_IsGammaN0.root

	bin/MakeWeights \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/Data.root \
	--treeName ntReweighting \
	--weightFileName Weights/pt2-5_y-2--1_IsGammaN0.root

	bin/MakeWeights \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN0/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN0/Data.root \
	--treeName ntReweighting \
	--weightFileName Weights/pt2-5_y-1-0_IsGammaN0.root

	bin/MakeWeights \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN0/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN0/Data.root \
	--treeName ntReweighting \
	--weightFileName Weights/pt2-5_y0-1_IsGammaN0.root

	bin/MakeWeights \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN0/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN0/Data.root \
	--treeName ntReweighting \
	--weightFileName Weights/pt2-5_y1-2_IsGammaN0.root

	bin/ClosureTest \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/Data.root \
	--weightFileName Weights/pt2-5_y-2--1_IsGammaN1.root; /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload img/ClosureTest*pdf /tmp/pt2-5_y-2--1_IsGammaN1/

	bin/ClosureTest \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN1/Data.root \
	--weightFileName Weights/pt2-5_y-1-0_IsGammaN1.root; /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload img/ClosureTest*pdf /tmp/pt2-5_y-1-0_IsGammaN1/

	bin/ClosureTest \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN1/Data.root \
	--weightFileName Weights/pt2-5_y0-1_IsGammaN1.root; /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload img/ClosureTest*pdf /tmp/pt2-5_y0-1_IsGammaN1/

	bin/ClosureTest \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN1/Data.root \
	--weightFileName Weights/pt2-5_y1-2_IsGammaN1.root; /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload img/ClosureTest*pdf /tmp/pt2-5_y1-2_IsGammaN1/


	bin/ClosureTest \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN0/Data.root \
	--weightFileName Weights/pt2-5_y-2--1_IsGammaN0.root; /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload img/ClosureTest*pdf /tmp/pt2-5_y-2--1_IsGammaN0/

	bin/ClosureTest \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN0/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-1-0_IsGammaN0/Data.root \
	--weightFileName Weights/pt2-5_y-1-0_IsGammaN0.root; /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload img/ClosureTest*pdf /tmp/pt2-5_y-1-0_IsGammaN0/

	bin/ClosureTest \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN0/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y0-1_IsGammaN0/Data.root \
	--weightFileName Weights/pt2-5_y0-1_IsGammaN0.root; /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload img/ClosureTest*pdf /tmp/pt2-5_y0-1_IsGammaN0/

	bin/ClosureTest \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN0/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y1-2_IsGammaN0/Data.root \
	--weightFileName Weights/pt2-5_y1-2_IsGammaN0.root; /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload img/ClosureTest*pdf /tmp/pt2-5_y1-2_IsGammaN0/

	bin/GenSpectra \
  --unweightedFileName /data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250213_v4_Pthat0_ForcedD0DecayD0Filtered_BeamA/mergedfile.root \
  --weightFileName Weights/pt2-5_y-2-2_IsGammaN1.root; /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload img/ClosureTest*pdf /tmp/pt2-5_y-2-2_IsGammaN1/

	bin/GenSpectra \
  --unweightedFileName /data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250213_v4_Pthat0_ForcedD0DecayD0Filtered_BeamA/mergedfile.root \
  --weightFileName Weights/pt2-5_y-2-2_IsGammaN1.root \
  --doGptGyReweighting false; /afs/cern.ch/user/y/yuchenc/Dropbox-Uploader/dropbox_uploader.sh upload img/ClosureTest*pdf /tmp/pt2-5_y-2-2_IsGammaN1_noGptGyReweighting/
