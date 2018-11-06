#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <filesystem>

//==============================================================================
EdenSynthAudioProcessorEditor::EdenSynthAudioProcessorEditor(EdenSynthAudioProcessor& p, AudioProcessorValueTreeState& vts)
	: AudioProcessorEditor(&p), _processor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
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
}

EdenSynthAudioProcessorEditor::~EdenSynthAudioProcessorEditor()
{
}

//==============================================================================
void EdenSynthAudioProcessorEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void EdenSynthAudioProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..
	_title.setBounds(40, 200, 100, 20);
	_waveTables.setBounds(100, 100, 100, 100);
}

void EdenSynthAudioProcessorEditor::waveTableChanged()
{
	_processor.setWaveTable(_waveTables.getText().toStdString());
}
