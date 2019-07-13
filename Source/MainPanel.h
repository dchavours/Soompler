#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SampleViewer.h"
#include "PluginProcessor.h"
#include "PianoRoll.h"
#include "GuiEditor.h"

class MainPanel  : public Component,
                   public Slider::Listener,
                   public Button::Listener,
                   public TransportStateListener

{
public:
    MainPanel (SoomplerAudioProcessor& processor);
    ~MainPanel();

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void transportStateChanged(TransportState state);

    float getVolume() const;

private:
    std::unique_ptr<Slider> volumeKnob;
    std::unique_ptr<Label> volumeKnobLabel;
    std::unique_ptr<ImageButton> openFileButton;
    std::unique_ptr<ImageButton> aboutButton;
    std::shared_ptr<SampleViewer> sampleViewer;
    std::unique_ptr<Label> loadSampleTip;
    std::unique_ptr<PianoRoll> pianoRoll;
    std::unique_ptr<ImageButton> playButton;
    std::unique_ptr<ImageButton> stopButton;

    Image backgroundImage;

    SoomplerAudioProcessor& processor;

    // ability to edit components for this panel
    GuiEditor editor;

    void openFileButtonClicked();
    void aboutButtonClicked();
    void playSampleButtonClicked();
    void stopSampleButtonClicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainPanel)
};

