default: all

all: Setup bin/MakeWeights bin/ClosureTest bin/DataMC_ClosureTest

Setup:
	mkdir -p bin
	mkdir -p img
	mkdir -p Weights


bin/MakeWeights: source/MakeWeights.cpp
	g++ source/MakeWeights.cpp -O3 -Iinclude -I$(ProjectBase)/CommonCode/include -o bin/MakeWeights `root-config --cflags --glibs`

bin/ClosureTest: source/ClosureTest.cpp
	g++ source/ClosureTest.cpp -O3 -Iinclude -I$(ProjectBase)/CommonCode/include -o bin/ClosureTest `root-config --cflags --glibs` \
	$(ProjectBase)/CommonCode/library/Messenger.o

bin/DataMC_ClosureTest: source/DataMC_ClosureTest.cpp
	g++ source/DataMC_ClosureTest.cpp -O3 -Iinclude -I$(ProjectBase)/CommonCode/include -o bin/DataMC_ClosureTest `root-config --cflags --glibs`

test: bin/MakeWeights bin/ClosureTest
	bin/MakeWeights \
	--numFileName /data00/cbaldene/forJanice/correctedcross_0nAn_PL.root \
	--numHistName hCrossSection \
	--denFileName /data00/cbaldene/forJanice/correctedcross_AnAn_PYTHIA8.root \
	--denHistName hCrossSection

	bin/ClosureTest \
	--unweightedFileName /data00/UPCD0LowPtAnalysis_2023ZDCORData_2023reco/SkimsMC/20250213_v4_Pthat0_ForcedD0DecayD0Filtered_BeamA/mergedfile.root

	bin/DataMC_ClosureTest \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2-2_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2-2_IsGammaN1/Data.root
