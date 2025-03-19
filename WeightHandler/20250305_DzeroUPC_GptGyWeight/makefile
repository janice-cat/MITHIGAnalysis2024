default: all

all: Setup bin/MakeWeights bin/ClosureTest

Setup:
	mkdir -p bin
	mkdir -p img
	mkdir -p Weights


bin/MakeWeights: source/MakeWeights.cpp
	g++ source/MakeWeights.cpp -O3 -Iinclude -I$(ProjectBase)/CommonCode/include -o bin/MakeWeights `root-config --cflags --glibs`

bin/ClosureTest: source/ClosureTest.cpp
	g++ source/ClosureTest.cpp -O3 -Iinclude -I$(ProjectBase)/CommonCode/include -o bin/ClosureTest `root-config --cflags --glibs`

test: bin/MakeWeights bin/ClosureTest
	bin/MakeWeights \
	--numFileName /data00/cbaldene/forJanice/correctedcross_0nAn_PL.root \
	--numHistName hCrossSection \
	--denFileName /data00/cbaldene/forJanice/correctedcross_AnAn_PYTHIA8.root \
	--denHistName hCrossSection

	bin/ClosureTest \
	--unweightedFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/MC.root \
	--targetFileName ../../MainAnalysis/20250313_DzeroUPC_dataMC/fullAnalysis/pt2-5_y-2--1_IsGammaN1/Data.root
