# Overview

This directory is a **self-contained UPC D⁰ data vs. Monte Carlo validation** workflow. At run time you only need **`ExecuteDzeroUPC`**, **`MassFit`**, **`SidebandSubtraction`**, **`DataMCComp`**, the shell drivers, and JSON cards **in this folder**—no other `MainAnalysis` package has to be run in parallel.

It shares the usual repository **`CommonCode`** (`Messenger`, etc.) like any analysis, but **operationally it does not depend on** `20241210_DzeroUPC` or any cross-section plotting step.

### Essential source files (and built executables)

| Source | Executable | Role |
|--------|------------|------|
| `DzeroUPC.cpp` | `ExecuteDzeroUPC` | Read skim `Tree`, apply event and D selections, write per-bin `Data.root` / `MC.root` (histograms + `TNtuple` for fits). |
| `massfit.C` | `MassFit` | RooFit 1D mass fit; reads micro-tree outputs and template MC; writes `fit.root` and fit diagnostics. |
| `sidebandSubtraction.C` | `SidebandSubtraction` | For a chosen variable expression, build sideband-subtracted shapes using the mass-fit workspace/weights; writes ROOT with `hist_SR_sub_norm`, etc., for comparison. |
| `dataMCComp.C` | `DataMCComp` | Overlay data vs MC: reads sideband-subtracted histograms and MC gen-matched histograms, draws ratios, writes a comparison PDF. |

Other `.C` files in this directory are **not** part of the default `make` targets:

- `SidebandSubtract.C` — standalone RooWorkspace-driven sideband helper (see comments at top of file for `root -l -q` usage).
- `sidebandSubtraction_org.C` — earlier / backup variant of the sideband machinery.

### Preparing micro trees for analysis
- **`DzeroUPC.cpp`** (`ExecuteDzeroUPC`)
	- Applies event selections (trigger, background/vtx filters, rapidity-gap logic, `nVtx`, etc.) and D selections (`DpassCut23PAS` and syst variants, optional track filters when branches exist).
	- Fills `hDmass`, efficiency histograms, optional HF vs. multiplicity 2D maps when `DoSystRapGap > 9`, and a **`TNtuple`** (`Dmass`, `Dgen`, kinematics, topology) for mass fits.
	- This driver is intentionally **slim**: it does not expose every switch (e.g. BDT / `HFMaxRapDefn`) that other UPC D⁰ drivers in the repo may add—extend `DzeroUPC.cpp` / `include/parameter.h` here if you need them.

### Mass fit
- **`massfit.C`** (`MassFit`)
	- Performs the 1D mass fit (RooFit).
	- Controlled by `massfit.sh` and a JSON card listing `dataInput`, `fitmcInputs` (and optional template inputs, `RstDir`, etc.) per `(pt, y)` bin.

### Data vs. MC shape comparison
- **`dataMCComp.sh`**
	- Reads a JSON (e.g. `fullAnalysis/dataMCComp.json`): global sideband settings, a list of **variables** (`Dpt`, `DsvpvDistance`, …), and **tasks** pairing Data/MC `sampleInput` + `massFitResult` paths under `MicroTreeDir`.
	- For each variable and task, runs **`SidebandSubtraction`** on data and on MC, then **`DataMCComp`** to produce overlay PDFs (e.g. `sidebandSubtraction_<var>_comp.pdf`) under `.../dataMCComp/`.
	- **Note:** the script may call a personal upload helper at the end; comment that line out if you do not use it.

### Controlling shell scripts
- Simple pattern:
	```bash
	make
	bash makeMicroTree.sh fullAnalysis/sampleConfig.json
	### Creates <MicroTreeDir>/ and copies the card as sampleConfig.json
	bash massfit.sh pt2-5_fitSettings/fullAnalysis_useGammaNForNgammaForFitFunc.json
	### Writes per-bin MassFit/MassFit_MC output and fitConfig.json under each bin
	bash dataMCComp.sh fullAnalysis/dataMCComp.json
	### Or: bash dataMCComp.sh pt2-5_dataMCCompSettings/example20250219_useGammaNForNgammaForFitFunc.json
	```
- Output layout for a given study (names are illustrative; `MicroTreeDir` and `FitDir` are user-defined tags in the JSON):
	```
	<MicroTreeDir>/
	 ├── sampleConfig.json          # copy of the micro-tree card
	 ├── dataMCComp.json            # optional: local copy when the card lives under pt2-5_dataMCCompSettings/
	 ├── MassFit.json               # optional: copy from fitSettings when used
	 ├── pt<pt>-<pt>_y<y>-<y>_IsGammaN<0|1>/
	 │	├── Data.root, MC.root, MC_inclusive.root (per sampleConfig)
	 │	├── MassFit/                 # data fit: fit.root, fit.log, fitConfig.json, …
	 │	├── MassFit_MC/              # MC fit when configured in the fit JSON
	 │	├── dataMCComp/              # sideband outputs + data/MC comparison PDFs
	 │	└── dataMCComp_MC/
	 └── ...
	```

# Steps for full analysis

Slides (general framework): https://www.dropbox.com/scl/fi/srm11gxdaivbvrkz5jbpo/20250121_newAnalysisFramework.pdf?rlkey=02y94bcpxoha4zod0oog6bz7x&dl=0

### Setup environment
From the repository root, build **`CommonCode`** (for `Messenger.o`), then build this folder:

```bash
source SetupAnalysis.sh
cd CommonCode && make && cd ../MainAnalysis/20260121_DzeroUPC_dataMC && make
```

The local **`makefile`** builds `ExecuteDzeroUPC`, `MassFit`, `SidebandSubtraction`, and `DataMCComp`. **`ProjectBase`** must be set by `SetupAnalysis.sh` (or equivalent).

**Note:** **`clean.sh`** in this directory rebuilds **`CommonCode`** and then runs **`make`** here (same four executables as above).

### 1. Make micro trees
```bash
bash makeMicroTree.sh fullAnalysis/sampleConfig.json
### Or systematic / alternate skims, e.g.:
bash makeMicroTree.sh pt2-5_sampleSettings/fullAnalysis_skim2025.json
bash makeMicroTree.sh pt2-5_sampleSettings/systDsvpv_skimV4.json
bash makeMicroTree.sh pt2-5_sampleSettings/systDtrkPt_skimV4.json
bash makeMicroTree.sh pt2-5_sampleSettings/systRapGapLoose_skimV4.json
bash makeMicroTree.sh pt2-5_sampleSettings/systRapGapTight_skimV4.json
bash makeMicroTree.sh pt2-5_sampleSettings/systDalpha_skimV4.json
bash makeMicroTree.sh pt2-5_sampleSettings/systDchi2cl_skimV4.json
```

### 2. Run mass fits
```bash
# Nominal and systematics (paths are under pt2-5_fitSettings/ in this directory)
bash massfit.sh pt2-5_fitSettings/fullAnalysis_useGammaNForNgammaForFitFunc.json
bash massfit.sh pt2-5_fitSettings/systFitSigMean_useGammaNForNgammaForFitFunc.json
bash massfit.sh pt2-5_fitSettings/systFitSigAlpha_useGammaNForNgammaForFitFunc.json
bash massfit.sh pt2-5_fitSettings/systFitComb_useGammaNForNgammaForFitFunc.json
bash massfit.sh pt2-5_fitSettings/systFitPkBg_useGammaNForNgammaForFitFunc.json
bash massfit.sh pt2-5_fitSettings/systDsvpv_useGammaNForNgammaForFitFunc.json
bash massfit.sh pt2-5_fitSettings/systDtrkPt_useGammaNForNgammaForFitFunc.json
bash massfit.sh pt2-5_fitSettings/systRapGapLoose_useGammaNForNgammaForFitFunc.json
bash massfit.sh pt2-5_fitSettings/systRapGapTight_useGammaNForNgammaForFitFunc.json
bash massfit.sh pt2-5_fitSettings/systDalpha_useGammaNForNgammaForFitFunc.json
bash massfit.sh pt2-5_fitSettings/systDchi2cl_useGammaNForNgammaForFitFunc.json
```

Nominal fit plots: `fullAnalysis/pt*-*_y*-*_IsGammaN*/MassFit/` (and `MassFit_MC/` where configured).

### 3. Data vs. MC comparison
```bash
bash dataMCComp.sh fullAnalysis/dataMCComp.json
### or
bash dataMCComp.sh pt2-5_dataMCCompSettings/example20250219_useGammaNForNgammaForFitFunc.json
```

PDFs and logs: `fullAnalysis/.../dataMCComp/` (and `dataMCComp_MC/` for MC-only sideband steps).

### 4. Example: 2025 skim–style chain
```bash
bash makeMicroTree.sh pt2-5_sampleSettings/fullAnalysis_skim2025.json
bash massfit.sh pt2-5_fitSettings/fullAnalysis_useGammaNForNgammaForFitFunc.json
bash dataMCComp.sh fullAnalysis/dataMCComp.json
### Mass fit results: fullAnalysis/*/MassFit/fit_result_full_param.pdf (typical naming from massfit.C)
### Data/MC overlays: fullAnalysis/*/dataMCComp/sidebandSubtraction_*_comp.pdf
```
