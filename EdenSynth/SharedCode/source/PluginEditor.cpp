#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WaveTablePathProvider.h"

//==============================================================================
EdenSynthAudioProcessorEditor::EdenSynthAudioProcessorEditor(EdenSynthAudioProcessor& p, AudioProcessorValueTreeState& vts, const eden_vst::WaveTablePathProvider& pathProvider)
	: AudioProcessorEditor(&p)
	, _processor(p)
	, _generatorComponent(vts, pathProvider)
	, _modifierComponent(vts)
{
	setSize(1000, 500);

	/*const auto path = _processor.getAssetsPath() / "wavetables";
	auto i = 0;
	for (auto& file : std::experimental::filesystem::directory_iterator(path))
	{
		++i;
		_waveTables.addItem(file.path().filename().string(), i);
	}

	_waveTables.onChange = [this] { waveTableChanged(); };
	_waveTables.setSelectedId(1);

	addAndMakeVisible(&_waveTables);*/

	addAndMakeVisible(_generatorComponent);
	addAndMakeVisible(_modifierComponent);
}

EdenSynthAudioProcessorEditor::~EdenSynthAudioProcessorEditor()
{
}

//==============================================================================
void EdenSynthAudioProcessorEditor::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void EdenSynthAudioProcessorEditor::resized()
{
	//_waveTables.setBounds(100, 100, 100, 100);

	_generatorComponent.setBounds(120, 0, 380, 500);
	_modifierComponent.setBounds(500, 0, 380, 500);
}

void EdenSynthAudioProcessorEditor::waveTableChanged()
{
	//_processor.setWaveTable(_waveTables.getText().toStdString());
}
