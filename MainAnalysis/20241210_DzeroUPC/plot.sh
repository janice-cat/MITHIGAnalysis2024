PlotSettingCard=${1}
PlotDir=$(jq -r '.PlotDir' $PlotSettingCard)
mkdir -p $PlotDir
cp $PlotSettingCard $PlotDir/plotConfig.json
PlotSettingCard=$PlotDir/plotConfig.json

echo "" > $PlotDir/plot.log

jq -c '.Plots[]' $PlotSettingCard | while read Plot; do
	MinDzeroPT=$(echo $Plot | jq -r '.MinDzeroPT')
	MaxDzeroPT=$(echo $Plot | jq -r '.MaxDzeroPT')
	IsGammaN=$(echo $Plot | jq -r '.IsGammaN')
	InputPoints=$(echo $Plot | jq -r '.InputPoints')
	wSystLumi=$(echo $Plot | jq -r '.wSystLumi')
	wSystTrk=$(echo $Plot | jq -r '.wSystTrk')
	wSystBR=$(echo $Plot | jq -r '.wSystBR')
	wSystEvtSel=$(echo $Plot | jq -r '.wSystEvtSel')
	wSystRapGapSel=$(echo $Plot | jq -r '.wSystRapGapSel')
	wSystDsvpv=$(echo $Plot | jq -r '.wSystDsvpv')
	wSystDtrkPt=$(echo $Plot | jq -r '.wSystDtrkPt')
	wSystFitSig=$(echo $Plot | jq -r '.wSystFitSig')
	wSystFitComb=$(echo $Plot | jq -r '.wSystFitComb')
	nominalSampleRST=$(echo $Plot | jq -r '.nominalSampleRST')
	nominalFitRST=$(echo $Plot | jq -r '.nominalFitRST')

  cmd="./PlotCrossSection --PlotDir $PlotDir --MinDzeroPT $MinDzeroPT --MaxDzeroPT $MaxDzeroPT --IsGammaN $IsGammaN --InputPoints $InputPoints"
			[ "$wSystLumi" != "null" ] && cms="$cmd --wSystLumi $wSystLumi"
			[ "$wSystTrk" != "null" ] && cms="$cmd --wSystTrk $wSystTrk"
			[ "$wSystBR" != "null" ] && cms="$cmd --wSystBR $wSystBR"
			[ "$wSystEvtSel" != "null" ] && cms="$cmd --wSystEvtSel $wSystEvtSel"
			[ "$wSystRapGapSel" != "null" ] && cms="$cmd --wSystRapGapSel $wSystRapGapSel"
			[ "$wSystDsvpv" != "null" ] && cms="$cmd --wSystDsvpv $wSystDsvpv"
			[ "$wSystDtrkPt" != "null" ] && cms="$cmd --wSystDtrkPt $wSystDtrkPt"
			[ "$wSystFitSig" != "null" ] && cms="$cmd --wSystFitSig $wSystFitSig"
			[ "$wSystFitComb" != "null" ] && cms="$cmd --wSystFitComb $wSystFitComb"
			[ "$nominalSampleRST" != "null" ] && cms="$cmd --nominalSampleRST $nominalSampleRST"
			[ "$nominalFitRST" != "null" ] && cms="$cmd --nominalFitRST $nominalFitRST"

	$cmd >> $PlotDir/plot.log

done