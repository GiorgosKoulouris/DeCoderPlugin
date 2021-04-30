#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DickoderAudioProcessor::DickoderAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                    apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    bufferHistoryIndex = 0;
}

DickoderAudioProcessor::~DickoderAudioProcessor()
{
}

//==============================================================================
const juce::String DickoderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DickoderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DickoderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DickoderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DickoderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DickoderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DickoderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DickoderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DickoderAudioProcessor::getProgramName (int index)
{
    return {};
}

void DickoderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DickoderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
//============CLEARING THE BUFFER HISTORY SO WE DONT INCLUDE PREVIUS DATA INTO RMS CALCULATION==========
    bufferHistoryIndex = 0;
    for (int i = 0; i < 2048; ++i)
    {
        bufferHistory[0][i] = 0.f;
        bufferHistory[1][i] = 0.f;
    }
}

void DickoderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DickoderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    
    //THIS PLUGIN IS USED ONLY IN STEREO CHANNELS
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DickoderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    //============= CLEARING DATA IN THE CHANNELS THAT ARE NOT USED BY THE PLUGIN ============
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    
    //THE ACTUAL DATA THAT IS GOING TO BE PROCESSED
    auto outL = buffer.getWritePointer(0);
    auto outR = buffer.getWritePointer(1);
    
    //A COPY OF OUR BUFFER DATA
    float inL[buffer.getNumSamples()];
    float inR[buffer.getNumSamples()];
    
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        inL[sample] = outL[sample];
        inR[sample] = outR[sample];
    }

    
    //READING AND STORING THE VALUES OF THE PARAMETERS WE USE DURING THE PROCESSING
    float midGain = *apvts.getRawParameterValue("MID GAIN");
    float sideGain = *apvts.getRawParameterValue("SIDE GAIN");
    float lOutTrim = *apvts.getRawParameterValue("LOUT TRIM");
    float rOutTrim = *apvts.getRawParameterValue("ROUT TRIM");
    
    
    
    //DATA PROCESSING BASED ON THE MODE SELECTED BY THE USER - ALSO UPDATING THE BUFFER HISTORY
    if (*apvts.getRawParameterValue("SOLO MID"))
    {
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            outL[sample] = (inL[sample] + inR[sample]) *  0.5f;
            outR[sample] = outL[sample];
            
            outL[sample] *= lOutTrim;
            outR[sample] *= rOutTrim;
            
            if (bufferHistoryIndex < bufferHistorySize)
            {
                bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                bufferHistoryIndex += 1;
            }
            else
            {
                bufferHistoryIndex = 0;
                bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                bufferHistoryIndex += 1;
            }
        }
    }
    
    else if (*apvts.getRawParameterValue("SOLO SIDE"))
    {
        if (*apvts.getRawParameterValue("COLLAPSE SIDE"))
        {
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                outL[sample] = (inL[sample] - inR[sample]) * 0.5f;
                outR[sample] = outL[sample];
                
                outL[sample] *= lOutTrim;
                outR[sample] *= rOutTrim;
                
                if (bufferHistoryIndex < bufferHistorySize)
                {
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
                else
                {
                    bufferHistoryIndex = 0;
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
            }
        }
        else
        {
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                outL[sample] = (inL[sample] - inR[sample]) * 0.5f;
                outR[sample] = (inR[sample] - inL[sample]) * 0.5f;
                
                outL[sample] *= lOutTrim;
                outR[sample] *= rOutTrim;
                
                if (bufferHistoryIndex < bufferHistorySize)
                {
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
                else
                {
                    bufferHistoryIndex = 0;
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
            }
        }
    }
    
    else if (*apvts.getRawParameterValue("SOLO LEFT"))
    {
        if (*apvts.getRawParameterValue("MONO CHECK"))
        {
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                outR[sample] = outL[sample] * rOutTrim * 0.5f;
                outL[sample] *= lOutTrim * 0.5f;
                
                if (bufferHistoryIndex < bufferHistorySize)
                {
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
                else
                {
                    bufferHistoryIndex = 0;
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
            }
        }
        else
        {
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                outR[sample] = 0.f;
                outL[sample] *= lOutTrim;
                
                if (bufferHistoryIndex < bufferHistorySize)
                {
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
                else
                {
                    bufferHistoryIndex = 0;
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
            }
        }
    }
    
    else if (*apvts.getRawParameterValue("SOLO RIGHT"))
    {
        if (*apvts.getRawParameterValue("MONO CHECK"))
        {
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                outL[sample] = outR[sample] * lOutTrim * 0.5f;
                outR[sample] *= rOutTrim * 0.5f;
                
                if (bufferHistoryIndex < bufferHistorySize)
                {
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
                else
                {
                    bufferHistoryIndex = 0;
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
            }
        }
        else
        {
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                outL[sample] = 0.f;
                outR[sample] *= rOutTrim;
                
                if (bufferHistoryIndex < bufferHistorySize)
                {
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
                else
                {
                    bufferHistoryIndex = 0;
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
            }
        }
    }
    
    else
    {
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                outL[sample] = (inL[sample] + inR[sample]) * midGain;
                outL[sample] += (inL[sample] - inR[sample]) * sideGain;
                
                
                outR[sample] = (inL[sample] + inR[sample]) * midGain;
                outR[sample] += (inR[sample] - inL[sample]) * sideGain;
                
                outL[sample] *= lOutTrim;
                outR[sample] *= rOutTrim;
                
                if (bufferHistoryIndex < bufferHistorySize)
                {
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
                else
                {
                    bufferHistoryIndex = 0;
                    bufferHistory[0][bufferHistoryIndex] = outL[sample] * outL[sample];
                    bufferHistory[1][bufferHistoryIndex] = outR[sample] * outR[sample];
                    bufferHistoryIndex += 1;
                }
            }
        }
    
    
    //CALL FUNCTION TO CHECK WHICH CHANNELS HAVE PEAKED (IF ANY)
    int a = channelsWithPeaks(buffer);
    if (a > hasPeaksValue)
    {
        hasPeaksValue = a;
    }

    
    //STORE THE MAX AMPLITUDE OF THE BUFFER
    peakValues[0] = buffer.getMagnitude(0, 0, buffer.getNumSamples());
    peakValues[1] = buffer.getMagnitude(1, 0, buffer.getNumSamples());


    
}

//==============================================================================
bool DickoderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DickoderAudioProcessor::createEditor()
{
    return new DickoderAudioProcessorEditor (*this);
}

//==============================================================================
void DickoderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
  
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xmlToSave (state.createXml());
    copyXmlToBinary(*xmlToSave, destData);
}

void DickoderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
   
    std::unique_ptr<juce::XmlElement> xmlToLoad (getXmlFromBinary(data, sizeInBytes));
    
    if (xmlToLoad.get() != nullptr)
    {
        if (xmlToLoad->hasTagName(apvts.state.getType()))
        {
            apvts.replaceState(juce::ValueTree::fromXml(*xmlToLoad));
        }
    }

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DickoderAudioProcessor();
}




//============= THE FUNCTION THAT RETURNS THE PARAMETER LAYOUT FOR APVTS (CALLED IN THE CONSTRUCTOR OF THE PROCESSOR=========================

juce::AudioProcessorValueTreeState::ParameterLayout DickoderAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("MID GAIN", "Mid Gain", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SIDE GAIN", "Side Gain", 0.0f, 1.0f, 0.5f));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>("SOLO MID", "Solo Mid", false, "Solo Mid", nullptr, nullptr));
    params.push_back(std::make_unique<juce::AudioParameterBool>("SOLO SIDE", "Solo Side", false, "Solo Side", nullptr, nullptr));
    params.push_back(std::make_unique<juce::AudioParameterBool>("COLLAPSE SIDE", "Collapse Side", false, "Collapse Side", nullptr, nullptr));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>("SOLO LEFT", "Solo Left", false, "Solo Left", nullptr, nullptr));
    params.push_back(std::make_unique<juce::AudioParameterBool>("SOLO RIGHT", "Solo Right", false, "Solo Right", nullptr, nullptr));
    params.push_back(std::make_unique<juce::AudioParameterBool>("MONO CHECK", "Mono Check", false, "Mono Check", nullptr, nullptr));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOUT TRIM", "Left Output Trim", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ROUT TRIM", "Right Output Trim", 0.0f, 2.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("LINK" , "Link Output Trim Knobs", false, "Link Output Trim Knobs", nullptr, nullptr));
    
    
    return { params.begin(), params.end()};
}


//=================== FUNCTION THAT RETURNS WHICH CHANNELS HAVE PEAKED ================
int DickoderAudioProcessor::channelsWithPeaks(juce::AudioBuffer<float>& buffer)
{
    float maxAbsValueL = buffer.getMagnitude(0, 0, buffer.getNumSamples());
    float maxAbsValueR = buffer.getMagnitude(1, 0, buffer.getNumSamples());
    int channelsWithPeaks = 0;
    
    if (maxAbsValueL > 1.0f )
    {
        channelsWithPeaks += 1;
    }
    if (maxAbsValueR > 1.0f)
    {
        channelsWithPeaks += 2;
    }
    
    return channelsWithPeaks;
}


//======== FUNCTION TO RETURN THE PEAKED CHANNELS TO THE EDITOR ==========
int DickoderAudioProcessor::hasPeaksVoid()
{
    int a = hasPeaksValue;
    hasPeaksValue = 0;
    return a;
}



//======== FUNCTION TO RETURN RMS VALUES TO THE EDITOR ==========
float DickoderAudioProcessor::returnRmsValues(int channel)
{
    float sum = 0.f;

    for (int index = 0; index < bufferHistorySize; ++index)
    {
        sum += bufferHistory[channel][index];
    }
        
    float rmsValue = sqrt(sum / bufferHistorySize);
    return rmsValue;
}


//======== FUNCTION TO RETURN PEAK VALUES TO THE EDITOR ========
float DickoderAudioProcessor::returnPeakValues(int channel)
{
    return peakValues[channel];
}

