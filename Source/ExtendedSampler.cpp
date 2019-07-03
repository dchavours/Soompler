/*
  ==============================================================================

    ExtendedSampler.cpp
    Created: 27 Jun 2019 4:53:19pm
    Author:  artyom

  ==============================================================================
*/

#include "ExtendedSampler.h"

namespace soompler {

ExtendedSound::ExtendedSound (const String& soundName,
                            AudioFormatReader& source,
                            const BigInteger& notes,
                            int midiNoteForNormalPitch,
                            double attackTimeSecs,
                            double releaseTimeSecs,
                            double maxSampleLengthSeconds)
    : name (soundName),
      sourceSampleRate (source.sampleRate),
      midiNotes (notes),
      midiRootNote (midiNoteForNormalPitch)
{
    if (sourceSampleRate > 0 && source.lengthInSamples > 0)
    {
        length = jmin ((int) source.lengthInSamples,
                       (int) (maxSampleLengthSeconds * sourceSampleRate));

        data.reset (new AudioBuffer<float> (jmin (2, (int) source.numChannels), length + 4));

        source.read (data.get(), 0, length + 4, 0, true, true);

        params.attack  = static_cast<float> (attackTimeSecs);
        params.release = static_cast<float> (releaseTimeSecs);
    }
}

bool ExtendedSound::appliesToNote (int midiNoteNumber)
{
    return midiNotes[midiNoteNumber];
}

bool ExtendedSound::appliesToChannel (int /*midiChannel*/)
{
    return true;
}

//==============================================================================
ExtendedVoice::ExtendedVoice(ChangeListener* listener) : eventListener(listener), volume(0)
{
}

bool ExtendedVoice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<const ExtendedSound*> (sound) != nullptr;
}

void ExtendedVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound* s, int /*currentPitchWheelPosition*/)
{
    static constexpr auto notesInOctave = 12.0;

    if (auto* sound = dynamic_cast<const ExtendedSound*> (s))
    {
        // each octave make freq two times bigger
        pitchRatio = std::pow (2.0, (midiNoteNumber - sound->midiRootNote) / notesInOctave)
                        * sound->sourceSampleRate / getSampleRate();

        sourceSamplePosition = 0.0;
        lgain = velocity * volume;
        rgain = velocity * volume;

        adsr.setSampleRate (sound->sourceSampleRate);
        adsr.setParameters (sound->params);

        adsr.noteOn();
    }
    else
    {
        jassertfalse; // this object can only play SamplerSounds!
    }
}

void ExtendedVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        adsr.noteOff();
    }
    else
    {
        clearCurrentNote();
        adsr.reset();
    }
}

void ExtendedVoice::pitchWheelMoved (int /*newValue*/) {}
void ExtendedVoice::controllerMoved (int /*controllerNumber*/, int /*newValue*/) {}

//==============================================================================
void ExtendedVoice::renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    if (auto* playingSound = static_cast<ExtendedSound*> (getCurrentlyPlayingSound().get()))
    {
        // raw audio data
        auto& data = *playingSound->data;
        // pointers for left and right channels
        const float* const inL = data.getReadPointer (0);
        const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer (1) : nullptr;

        // output buffer pointers for channels
        float* outL = outputBuffer.getWritePointer (0, startSample);
        float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer (1, startSample) : nullptr;

        // whiling through of block of samples that need to be rendered
        // block is (128, 256, 512 etc) samples
        while (--numSamples >= 0)
        {
            // position (sample number) in sound raw data that going to be added to output buffer
            // firstSampleToPlay is just offset that set by user by moving start line in sampleview
            // alpha and invAlpha is interpolation things
            auto pos = (int64) sourceSamplePosition + this->firstSampleToPlay;
            auto alpha = (float) (sourceSamplePosition + this->firstSampleToPlay - pos);
            auto invAlpha = 1.0f - alpha;

            // just using a very simple linear interpolation here..
            float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
            float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha)
                                       : l;

            // to l and r samples for both channels
            // mix envelopeValue, google ADSR envelope for more info
            auto envelopeValue = adsr.getNextSample();

            l *= lgain * envelopeValue;
            r *= rgain * envelopeValue;

            // if stereo
            if (outR != nullptr)
            {
                *outL++ += l;
                *outR++ += r;
            }
            else
            {
                // mono is avg of l and r
                *outL++ += (l + r) * 0.5f;
            }

            // TODO what's that, why pitchratio being added
            sourceSamplePosition += pitchRatio;

            // endSample is just sample where end line is placed
            // user can drag that line in gui to change this parameter

            // endSample is not set
            if (endSample == 0) {
                // play full length sound
                endSample = playingSound->length;
            }

            // if current position is bigger than endSample
            if (sourceSamplePosition + firstSampleToPlay > endSample)
            {
                stopNote (0.0f, false);
                break;
            }
        }
    }
}

void ExtendedVoice::setStartSample(int64 sample)
{
    this->firstSampleToPlay = sample;
}

void ExtendedVoice::setEndSample(int64 sample)
{
    this->endSample = sample;
}

double ExtendedVoice::getCurrentPosition() const
{
    if (isPlayingButReleased()) {
        return firstSampleToPlay / getSampleRate();
    }

    return (sourceSamplePosition + firstSampleToPlay) / getSampleRate();
}

void ExtendedVoice::setVolume(float volume)
{
    jassert(volume <= 1.0f && volume >= .0f);
    this->volume = volume;
}

void ExtendedVoice::removeListener()
{
    this->eventListener = nullptr;
}

}
