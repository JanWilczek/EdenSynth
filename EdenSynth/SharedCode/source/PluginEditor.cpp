#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <filesystem>

//==============================================================================
EdenSynthAudioProcessorEditor::EdenSynthAudioProcessorEditor(EdenSynthAudioProcessor& p, AudioProcessorValueTreeState& vts)
	: AudioProcessorEditor(&p)
	, _processor(p)
	, _generatorComponent(vts)
	, _modifierComponent(vts)
{
	setSize(1000, 500);

	_title.setFont(_titleFont);
	addAndMakeVisible(&_title);

	const auto path = _processor.getAssetsPath() / "wavetables";
	auto i = 0;
	for (auto& file : std::experimental::filesystem::directory_iterator(path))
	{
		++i;
		_waveTables.addItem(file.path().filename().string(), i);
	}

	_waveTables.onChange = [this] { waveTableChanged(); };
	_waveTables.setSelectedId(1);

	addAndMakeVisible(&_waveTables);

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
	_title.setBounds(450, 450, 100, 20);
	
	_waveTables.setBounds(100, 100, 100, 100);

	_generatorComponent.setBounds(120, 0, 380, 500);
	_modifierComponent.setBounds(500, 0, 380, 500);
}

void EdenSynthAudioProcessorEditor::waveTableChanged()
{
	_processor.setWaveTable(_waveTables.getText().toStdString());
}