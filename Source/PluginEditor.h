#pragma once

#include "JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/

class DickoderAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener, public juce::Timer, public juce::Slider::Listener

{
public:
    DickoderAudioProcessorEditor (DickoderAudioProcessor&);
    ~DickoderAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    
private:
    
    //========================== GUI ELEMENTS (INPUTS) ======================================
    
    juce::ImageComponent bgImageComponent;
    
    juce::Slider midGainSlider;
    juce::Slider sideGainSlider;
    
    juce::ImageButton midSoloButton;
    juce::ImageButton sideSoloButton;
    juce::ImageButton leftSoloButton;
    juce::ImageButton rightSoloButton;
    juce::ImageButton monoCheckButton;
    juce::ImageButton sideSpreadButton;
    
    juce::Slider outLeftTrimSlider;
    juce::Slider outRightTrimSlider;
    
    juce::ImageButton linkButton;
    
    //========================== LABELS =====================
    float maxLeftPeakValue = -100.f;
    float maxRightPeakValue = -100.f;
    
    std::string toTwoDecString (float);
    
    juce::Font labelFont;
    std::unique_ptr<juce::BorderSize<int>> borderSize;
    std::unique_ptr<juce::Label> leftPeakLabelPtr;
    std::unique_ptr<juce::Label> rightPeakLabelPtr;
    
    juce::ImageButton meterResetButton;

    
    //========================== ATTACHMENTS FOR APVTS ======================================
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midGainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sideGainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midSoloButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> sideSoloButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> leftSoloButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> rightSoloButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> monoCheckButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> sideSpreadButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outLeftTrimSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outRightTrimSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> linkButtonAttachment;

    
    //========================== FUNCTIONS FOR DEDICATED FUNCTIONALITY =============================

    void buttonClicked (juce::Button *) override;
    void sliderDragStarted (juce::Slider *) override;
    
    void sliderValueChanged (juce::Slider *) override;
    double oldLeftOutValue;

    //=================== WHAT WE NEED TO READ FROM PROCESSOR VIA THE CALLBACK ======================
    
    void timerCallback() override;
    
    //FOR OUTPUT PEAKING INDICATORS
    int peakedOutputChannels;

    //FOR PEAK METERS
    juce::ImageComponent leftPeakMeter;
    juce::ImageComponent rightPeakMeter;
    float oldLeftPeakValue = 0.f;
    float newLeftPeakValue;
    float oldRightPeakValue = 0.f;
    float newRightPeakValue;

    //FOR RMS METERS
    juce::ImageComponent leftRmsMeter;
    juce::ImageComponent rightRmsMeter;
    float oldLeftRmsValue = 0.f;
    float oldRightRmsValue = 0.f;
    float newLeftRmsValue;
    float newRightRmsValue;
    
    //================================================================
    
    DickoderAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DickoderAudioProcessorEditor)
};
